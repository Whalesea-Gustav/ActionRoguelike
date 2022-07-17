// Fill out your copyright notice in the Description page of Project Settings.


#include "WhCharacter.h"

#include "WhAttributeComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "WhInteractionComponent.h"

// Sets default values
AWhCharacter::AWhCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;

	InteractionComp = CreateDefaultSubobject<UWhInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<UWhAttributeComponent>("AttributeComp");
	
	
}

// Called when the game starts or when spawned
void AWhCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AWhCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void AWhCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X = Forward Red
	// Y = Right   Green
	// Z = Up      Blue
	auto RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value);
}

void AWhCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this,
		&AWhCharacter::PrimaryAttack_TimeElasped, 0.2f);

	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

void AWhCharacter::PrimaryAttack_TimeElasped()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");


	//Todo:
	// Camera View Direction
	FMinimalViewInfo viewInfo;
	CameraComp->GetCameraView(0, viewInfo);

	FVector traceStart = viewInfo.Location;
	float traceScale = 3000.0f;
	FVector traceEnd = traceStart + viewInfo.Rotation.Vector() * traceScale;
	FHitResult Hit;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	
	bool bLineHit = GetWorld()->LineTraceSingleByObjectType(Hit, traceStart, traceEnd, ObjectQueryParams);

	FVector ImpactLocation;
	
	if (bLineHit)
	{
		ImpactLocation = Hit.Location;
	}
	else
	{
		ImpactLocation = traceEnd;
	}
	
	FVector ProjectDir = ImpactLocation - HandLocation;
	ProjectDir.Normalize();

	FRotator SpawnRotator = ProjectDir.Rotation();
	
	//FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
	FTransform SpawnTM = FTransform(SpawnRotator, HandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void AWhCharacter::BlackholeAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this,
		&AWhCharacter::BlackholeAttack_TimeElasped, 0.2f);
}

void AWhCharacter::BlackholeAttack_TimeElasped()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");


	//Todo:
	// Camera View Direction
	FMinimalViewInfo viewInfo;
	CameraComp->GetCameraView(0, viewInfo);

	FVector traceStart = viewInfo.Location;
	float traceScale = 3000.0f;
	FVector traceEnd = traceStart + viewInfo.Rotation.Vector() * traceScale;
	FHitResult Hit;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	
	bool bLineHit = GetWorld()->LineTraceSingleByObjectType(Hit, traceStart, traceEnd, ObjectQueryParams);

	FVector ImpactLocation;
	
	if (bLineHit)
	{
		ImpactLocation = Hit.Location;
	}
	else
	{
		ImpactLocation = traceEnd;
	}
	
	FVector ProjectDir = ImpactLocation - HandLocation;
	ProjectDir.Normalize();

	FRotator SpawnRotator = ProjectDir.Rotation();
	
	//FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
	FTransform SpawnTM = FTransform(SpawnRotator, HandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(BlackholeProjectileClass, SpawnTM, SpawnParams);
}

void AWhCharacter::DashAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this,
		&AWhCharacter::DashAttack_TimeElasped, 0.2f);
}

void AWhCharacter::DashAttack_TimeElasped()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");


	//Todo:
	// Camera View Direction
	FMinimalViewInfo viewInfo;
	CameraComp->GetCameraView(0, viewInfo);

	FVector traceStart = viewInfo.Location;
	float traceScale = 3000.0f;
	FVector traceEnd = traceStart + viewInfo.Rotation.Vector() * traceScale;
	FHitResult Hit;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	
	bool bLineHit = GetWorld()->LineTraceSingleByObjectType(Hit, traceStart, traceEnd, ObjectQueryParams);

	FVector ImpactLocation;
	
	if (bLineHit)
	{
		ImpactLocation = Hit.Location;
	}
	else
	{
		ImpactLocation = traceEnd;
	}
	
	FVector ProjectDir = ImpactLocation - HandLocation;
	ProjectDir.Normalize();

	FRotator SpawnRotator = ProjectDir.Rotation();
	
	//FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
	FTransform SpawnTM = FTransform(SpawnRotator, HandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(DashProjectileClass, SpawnTM, SpawnParams);
}

void AWhCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void AWhCharacter::Jump()
{
	Super::Jump();
}

// Called every frame
void AWhCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWhCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AWhCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWhCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AWhCharacter::Jump);
	
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AWhCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AWhCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("BlackholeAttack", IE_Pressed, this, &AWhCharacter::BlackholeAttack);
	PlayerInputComponent->BindAction("DashAttack", IE_Pressed, this, &AWhCharacter::DashAttack);
	
}

