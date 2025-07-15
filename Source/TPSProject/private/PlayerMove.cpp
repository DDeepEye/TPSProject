// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMove.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

UPlayerMove::UPlayerMove()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();
	moveComp->MaxWalkSpeed = walkSpeed;
}

void UPlayerMove::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PlayerMove();
}

void UPlayerMove::PlayerMove()
{
	direction = (FTransform(me->GetControlRotation()).TransformVector(direction)).GetSafeNormal();
	me->AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void UPlayerMove::Turn(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	me->AddControllerYawInput(value);
}

void UPlayerMove::LookUp(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	me->AddControllerPitchInput(value);
}

void UPlayerMove::Move(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	direction.X = value.X;
	direction.Y = value.Y;
}

void UPlayerMove::InputJump(const struct FInputActionValue& inputValue)
{
	me->Jump();
}

void UPlayerMove::InputRun(const struct FInputActionValue& inputValue)
{
	UCharacterMovementComponent* movement = me->GetCharacterMovement();
	if (movement->MaxWalkSpeed > walkSpeed)
	{
		movement->MaxWalkSpeed = walkSpeed;
	}
	else
	{
		movement->MaxWalkSpeed = runSpeed;
	}
}

void UPlayerMove::SetupInputBinding(class UEnhancedInputComponent* PlayerInput)
{
	PlayerInput->BindAction(ia_Turn, ETriggerEvent::Triggered, this, &UPlayerMove::Turn);
	PlayerInput->BindAction(ia_LookUp, ETriggerEvent::Triggered, this, &UPlayerMove::LookUp);
	PlayerInput->BindAction(ia_Move, ETriggerEvent::Triggered, this, &UPlayerMove::Move);
	PlayerInput->BindAction(ia_Jump, ETriggerEvent::Started, this, &UPlayerMove::InputJump);
	PlayerInput->BindAction(ia_Run, ETriggerEvent::Started, this, &UPlayerMove::InputRun);
	PlayerInput->BindAction(ia_Run, ETriggerEvent::Completed, this, &UPlayerMove::InputRun);
}
