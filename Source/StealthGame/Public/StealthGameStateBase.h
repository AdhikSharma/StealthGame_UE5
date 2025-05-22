// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StealthGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API AStealthGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast,Reliable)
	void MulticastOnMissionComplete(APawn* instigatorPawn,bool missionSuccess);
	
};
