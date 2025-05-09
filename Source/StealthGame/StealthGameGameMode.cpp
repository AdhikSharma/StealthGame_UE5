// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthGameGameMode.h"
#include "StealthGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AStealthGameGameMode::AStealthGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AStealthGameGameMode::MissionComplete(APawn* instigatorPawn, bool missionSuccess)
{
	if (instigatorPawn) 
	{
		instigatorPawn->DisableInput(nullptr);

		if (SpectatingViewpointClass) 
		{
			//Get all actors of spectating class
			TArray<AActor*> viewTargets;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, viewTargets);

			AActor* viewTarget = nullptr;

			if (viewTargets.Num() > 0)
			{
				viewTarget = viewTargets[0];

				APlayerController* playerController = Cast<APlayerController>(instigatorPawn->GetController());

				if (playerController)
				{
					//Blend
					playerController->SetViewTargetWithBlend(viewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("SPectating class is null"));
		}

		
	}

	OnMissionCompleted(instigatorPawn, missionSuccess);
	
}
