// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class UWorld;
class USActionComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;
	
public:

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStartAction(AActor* Instigator);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	virtual UWorld* GetWorld() const override;
};
