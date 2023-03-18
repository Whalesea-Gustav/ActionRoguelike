// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "ParticleHelper.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

static TAutoConsoleVariable<bool> CVarbSpawnBots(TEXT("su.bSpawnBots"), true, TEXT("Enable Spawning of Bots Via Timer."), ECVF_Cheat);

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // pass in Player ? for Credits
		}
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (Controller)
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;

	CreditsPerKill = 5;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	PlayerStateClass = ASPlayerState::StaticClass();
	
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query failed!"));
		return;
	}
	
	TArray<FVector>	Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{

	if(!CVarbSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawning diabled via car 'CVarbSpawnBots'."))
		return;
	}
	
	int32 NrofAliveBots = 0;

	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		if(USAttributeComponent::GetAttributeComponent(Bot) &&
			USAttributeComponent::IsActorAlive(Bot))
		{
			NrofAliveBots++;
		}
	}

	float MaxBotCount = 10.0f;
	
	if (DifficultyCurve)
	{
		MaxBotCount =  DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrofAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At Maximun Bot Capacity. Skipping Bot Spawn "));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
}

void ASGameModeBase::OnPowerupQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Powerup EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	TArray<FVector> UsedLocation;

	int32 SpawnCounter = 0;

	while(SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLcoation = Locations[RandomLocationIndex];

		bool bValidLocation = true;

		for (FVector OtherLocation : UsedLocation)
		{
			float DistanceInfo = (PickedLcoation - OtherLocation).Size();
			if (DistanceInfo < RequiredPowerupDistance)
			{
				bValidLocation = false;
				break;
			}
		}

		if(!bValidLocation)
		{
			continue;
		}

		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLcoation, FRotator::ZeroRotator);
		
		UsedLocation.Add(PickedLcoation);
		SpawnCounter++;
	}
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

	if (ensure(PowerupClasses.Num() > 0))
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPowerupQuery,
			this, EEnvQueryRunMode::AllMatching, nullptr);

		if(ensure( QueryInstance))
		{
			//When Start Play, Generate All Powerup Actor from PowerupClass through random selection
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerupQueryCompleted);
		}
	}
}

void ASGameModeBase::OnActorKill(AActor* VictimActor, AActor* KillerActor)
{
	ASCharacter* Player  = Cast<ASCharacter>(VictimActor);

	if (Player)
	{
		FTimerHandle TimerHandle_PlayerRespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_PlayerRespawnDelay, Delegate, RespawnDelay, false);
	}
	
	UE_LOG(LogTemp, Log, TEXT("OnActorKill: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(KillerActor));

	APawn* KillerPawn = Cast<APawn>(KillerActor);
	if (KillerPawn)
	{
		ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>();
		if (PS)
		{
			PS->AddCredits(CreditsPerKill);
		}
	}
}
