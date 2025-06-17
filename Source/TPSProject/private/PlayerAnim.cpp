// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	ATPSPlayer* player = Cast<ATPSPlayer>(TryGetPawnOwner());
	if (player)
	{
		FVector velocity = player->GetVelocity();
		FVector forward = player->GetActorForwardVector();
		speed = FVector::DotProduct(velocity, forward);		

		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		isInair = player->GetMovementComponent()->IsFalling();
	}
}

void UPlayerAnim::PlayerAttackAnim()
{
	Montage_Play(attackAnimMontage);
}