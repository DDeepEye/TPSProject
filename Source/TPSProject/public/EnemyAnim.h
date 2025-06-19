// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die,
};

UCLASS()
class TPSPROJECT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	EEnemyState animState = EEnemyState::Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	bool bAttackPlay = false;

public:
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
