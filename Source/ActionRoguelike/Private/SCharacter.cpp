// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "GPUMessaging.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";
	HandSocketName = "Muzzle_01";
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	AttackAnimDelay = 0.2f;
}

void ASCharacter::MoveForward(float value)
{
	FRotator controlRot = GetControlRotation();
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;
	AddMovementInput(controlRot.Vector(), value);
}

void ASCharacter::MoveRight(float value)
{
	FRotator controlRot = GetControlRotation();
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(controlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, value);
}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = CameraComp->GetComponentLocation();

		// endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
		FVector TraceEnd = CameraComp->GetComponentLocation() + (CameraComp->GetComponentRotation().Vector() * 5000);

		FHitResult Hit;
		// returns true if we got to a blocking hit
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			// Overwrite trace end with impact point in world
			TraceEnd = Hit.ImpactPoint;
		}

		// find new direction/rotation from Hand pointing to impact point in world.
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

void ASCharacter::SpawnProjectile_v2(TSubclassOf<AActor> ClassToSpawn)
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	TArray<FHitResult> Hits;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FVector Start, End;
	FRotator CameraRotator = CameraComp->GetComponentRotation();
	Start = CameraComp->GetComponentLocation();
	End = Start + 2000.0f * CameraRotator.Vector();
	
	bool bBlockingTrace = GetWorld()->LineTraceMultiByObjectType(Hits, Start, End, ObjectQueryParams);

	DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.0f, 0.0f, 1.0f);
	
	FVector ImpactLocation;
	float ImpactDistance;
	if (bBlockingTrace)
	{
		ImpactDistance = Hits.Last().Distance;
		ImpactLocation = Hits.Last().ImpactPoint;
		for (FHitResult& Hit : Hits)
		{
			if (Hit.Distance < ImpactDistance)
			{
				ImpactDistance = Hit.Distance;
				ImpactLocation = Hit.ImpactPoint;
			}
		}	
	}
	else
	{
		ImpactLocation = End;
	}

	DrawDebugSphere(GetWorld(), ImpactLocation, 20.0f, 12, FColor::Yellow, false, 2.0f, 0.0f, 1.0f);
	
	SpawnTM = FTransform(UKismetMathLibrary::FindLookAtRotation(HandLocation, ImpactLocation), HandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
}

void ASCharacter::StartAttackEffect()
{
	PlayAnimMontage(AttackAnim);

	UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), HandSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
}

void ASCharacter::PrimaryAttack()
{

	StartAttackEffect();

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, AttackAnimDelay);

	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	TArray<FHitResult> Hits;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FVector Start, End;
	FRotator CameraRotator = CameraComp->GetComponentRotation();
	Start = CameraComp->GetComponentLocation();
	End = Start + 2000.0f * CameraRotator.Vector();
	
	bool bBlockingTrace = GetWorld()->LineTraceMultiByObjectType(Hits, Start, End, ObjectQueryParams);

	DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.0f, 0.0f, 1.0f);
	
	FVector ImpactLocation;
	float ImpactDistance;
	if (bBlockingTrace)
	{
		ImpactDistance = Hits.Last().Distance;
		ImpactLocation = Hits.Last().ImpactPoint;
		for (FHitResult& Hit : Hits)
		{
			if (Hit.Distance < ImpactDistance)
			{
				ImpactDistance = Hit.Distance;
				ImpactLocation = Hit.ImpactPoint;
			}
		}	
	}
	else
	{
		ImpactLocation = End;
	}

	DrawDebugSphere(GetWorld(), ImpactLocation, 20.0f, 12, FColor::Yellow, false, 2.0f, 0.0f, 1.0f);
	
	SpawnTM = FTransform(UKismetMathLibrary::FindLookAtRotation(HandLocation, ImpactLocation), HandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}


void ASCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::Dash()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ASCharacter::Dash_TimeElapsed, AttackAnimDelay);
}

void ASCharacter::Dash_TimeElapsed()
{
	//SpawnProjectile(DashProjectileClass);
	SpawnProjectile_v2(DashProjectileClass);
}

void ASCharacter::BlackholeAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_BlackholeAttack, this, &ASCharacter::BlackHoleAttack_TimeElaped, AttackAnimDelay);
}

void ASCharacter::BlackHoleAttack_TimeElaped()
{
	//SpawnProjectile(BlackHoleProjectileClass);
	SpawnProjectile_v2(BlackHoleProjectileClass);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);


	FRotator controlRot = GetControlRotation();
	//controlRot.Pitch = 0.0f;
	//controlRot.Roll = 0.0f;

	//FVector RightVector = FRotationMatrix(controlRot).GetScaledAxis(EAxis::Y);
	FVector controllerDir = controlRot.Vector();
	
	FVector ControllerDirection_LineEnd = LineStart + (controllerDir * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::BlackholeAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

