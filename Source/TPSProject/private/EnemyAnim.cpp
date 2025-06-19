// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"


void UEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());
	if (enemy)
	{
		FVector velocity = enemy->GetVelocity();
		animState = velocity.Length() > 0 ? EEnemyState::Move : EEnemyState::Idle;
	}
}