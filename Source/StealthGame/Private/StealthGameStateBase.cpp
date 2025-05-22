// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthGameStateBase.h"
#include "EngineUtils.h"
#include "StealthGamePlayerController.h"

void AStealthGameStateBase::MulticastOnMissionComplete_Implementation(APawn* instigatorPawn, bool missionSuccess)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("MulticastOnMissionComplete_Implementation"));
    for (FConstControllerIterator i = GetWorld()->GetControllerIterator(); i; i++) 
    {
        AStealthGamePlayerController* pc = Cast<AStealthGamePlayerController>(i->Get());

        if (pc && pc->IsLocalController())
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Got the controller"));
            pc->OnMissionCompleted(instigatorPawn,missionSuccess);

            APawn* pawn = pc->GetPawn();
            if (pawn && pawn->IsLocallyControlled())
            {
                pawn->DisableInput(nullptr);
            }
        }
    }
}
