// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuard.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "DrawDebugHelpers.h"
#include "StealthGame/StealthGameGameMode.h"
#include "Engine/TargetPoint.h"
#include "AIController.h"                        
#include "NavigationSystem.h"                    
#include "BehaviorTree/BlackboardComponent.h"    
#include "Kismet/GameplayStatics.h" 
#include "Net/UnrealNetwork.h"

// Sets default values
AAIGuard::AAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightHearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("SightHearingConfig"));

	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = 2500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	SightHearingConfig->HearingRange = 2000.f;
	SightHearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightHearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightHearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);

	AIPerceptionComponent->ConfigureSense(*SightHearingConfig);

	GuardState = EAIState::Idle;
	
}

// Called when the game starts or when spawned
void AAIGuard::BeginPlay()
{
	Super::BeginPlay();
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIGuard::OnPawnSeen);

	OriginalRotation = GetActorRotation();
	GetWorldTimerManager().SetTimer(timerHandleResetRotation, this, &AAIGuard::MoveToPatrolPoint, 3.f, false);
	
}

void AAIGuard::OnPawnSeen(AActor* actor, FAIStimulus stimulus)
{
	if (actor == nullptr) return;

	if (stimulus.WasSuccessfullySensed())
	{
		AAIController* aIController = Cast<AAIController>(GetController());

		if (aIController)
		{
			aIController->StopMovement();
		}
		if (stimulus.Type == SightConfig->GetSenseID())//sight
		{
			DrawDebugSphere(GetWorld(), actor->GetActorLocation(), 32.f, 12, FColor::Red, false, 10.f);

			AStealthGameGameMode* gameMode = Cast<AStealthGameGameMode>(GetWorld()->GetAuthGameMode());

			APawn* actorPawn = Cast<APawn>(actor);

			if (gameMode && actorPawn)
			{
				gameMode->MissionComplete(actorPawn, false);
			}

			GetWorldTimerManager().ClearTimer(timerHandleResetRotation);
			SetGuardState(EAIState::Alerted);

		}
		else if (stimulus.Type == SightHearingConfig->GetSenseID())//hearing
		{
			if (GuardState == EAIState::Alerted)
			{
				return;
			}

			DrawDebugSphere(GetWorld(), stimulus.StimulusLocation, 32.f, 12, FColor::Black, false, 10.f);

			FVector direction = stimulus.StimulusLocation - GetActorLocation();
			FRotator newLookAt = FRotationMatrix::MakeFromX(direction).Rotator();
			newLookAt.Pitch = 0.f;
			newLookAt.Roll = 0.f;
			SetActorRotation(newLookAt);

			GetWorldTimerManager().ClearTimer(timerHandleResetRotation);
			GetWorldTimerManager().SetTimer(timerHandleResetRotation, this, &AAIGuard::ResumePatroling, 3.f, false);

			SetGuardState(EAIState::Suspicious);
			
		}
		else
		{
			
		}
	}
}

void AAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	SetActorRotation(OriginalRotation);

	SetGuardState(EAIState::Idle);
}

void AAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}

void AAIGuard::SetGuardState(EAIState newState)
{
	if (GuardState == newState) 
	{
		return;
	}

	GuardState = newState;
	OnRep_GuardState();

}

void AAIGuard::MoveToPatrolPoint()
{
	SetGuardState(EAIState::Patrol);
	CurrentPatrolPointIndex++;

	if (CurrentPatrolPointIndex < PatrolPoints.Num())
	{
		CurrentPatrolPoint = PatrolPoints[CurrentPatrolPointIndex];
	}
	else 
	{
		CurrentPatrolPointIndex = 0;
		CurrentPatrolPoint = PatrolPoints[0];
	}

	AAIController* aIController = Cast<AAIController>(GetController());

	if (aIController)
	{
		aIController->MoveToActor(CurrentPatrolPoint);
	}

}

void AAIGuard::ResumePatroling()
{
	SetGuardState(EAIState::Patrol);
	AAIController* aIController = Cast<AAIController>(GetController());

	if (aIController)
	{
		aIController->MoveToActor(CurrentPatrolPoint);
	}
}

// Called every frame
void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) return;

	if (GuardState == EAIState::Patrol && CurrentPatrolPoint)
	{
		//check how many distance is remaining

		const float distanceToTarget = FVector::Dist(GetActorLocation(), CurrentPatrolPoint->GetActorLocation());

		if (distanceToTarget < 100.f) 
		{
			SetGuardState(EAIState::Idle);
			GetWorldTimerManager().SetTimer(timerHandleResetRotation, this, &AAIGuard::MoveToPatrolPoint, 3.f, false);
		}
	}

}

void AAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAIGuard, GuardState);
}

FString AAIGuard::EnumToString_Reflected(EAIState State)
{
	const UEnum* EnumPtr = StaticEnum<EAIState>();
	if (!EnumPtr) return TEXT("Invalid");

	return EnumPtr->GetNameStringByValue((int8)State);
}


