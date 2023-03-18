// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UCurveFloat;
class ASPowerupActor;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UFUNCTION(Exec)
	void KillAll();
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;
	
	FTimerHandle TimerHandle_SpawnBots;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Credits")
	int32 CreditsPerKill;

	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	UEnvQuery* SpawnPowerupQuery;
	
	UPROPERTY(EditAnywhere, Category = "Powerup")
	TArray<TSubclassOf<AActor>> PowerupClasses;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Powerup")
	int DesiredPowerupCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Powerup")
	int RequiredPowerupDistance;
	
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
	
public:

	ASGameModeBase();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnPowerupQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	virtual void StartPlay() override;

	virtual void OnActorKill(AActor* VictimActor, AActor* KillerActor);
	
	
};
