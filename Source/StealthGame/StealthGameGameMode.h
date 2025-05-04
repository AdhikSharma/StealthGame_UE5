// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StealthGameGameMode.generated.h"

UCLASS(minimalapi)
class AStealthGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStealthGameGameMode();

	void MissionComplete(APawn* instigatorPawn);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionCompleted(APawn* instigatorPawn);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Spectating")
	TSubclassOf<AActor> SpectatingViewpointClass;
};



