// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIGuard.generated.h"

class UAIPerceptionComponent;

UENUM(BlueprintType)
enum class EAIState : uint8 
{
	Idle UMETA(DisplayName = "Idle"),
	Suspicious UMETA(DisplayName = "Suspicious"),
	Alerted UMETA(DisplayName = "Alerted"),
	Patrol UMETA(DisplayName = "Patrol")

};

UCLASS()
class STEALTHGAME_API AAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY()
	class UAISenseConfig_Hearing* SightHearingConfig;

	UFUNCTION()
	void OnPawnSeen(AActor* actor, FAIStimulus stimulus);

	FRotator OriginalRotation;
	FTimerHandle timerHandleResetRotation;

	UFUNCTION()
	void ResetOrientation();

	UPROPERTY(ReplicatedUsing = OnRep_GuardState);
	EAIState GuardState;

	UFUNCTION()
	void OnRep_GuardState();

	void SetGuardState(EAIState newState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState newState);

	UPROPERTY(EditInstanceOnly, Category = "AI")
	TArray<class ATargetPoint*> PatrolPoints;

	void MoveToPatrolPoint();

	void ResumePatroling();

	int32 CurrentPatrolPointIndex = -1;

	ATargetPoint* CurrentPatrolPoint;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FString EnumToString_Reflected(EAIState State);


};
