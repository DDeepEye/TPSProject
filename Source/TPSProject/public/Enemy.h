// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"



UCLASS()
class TPSPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnDamageProcess();	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterState")
	int32 hp = 3;

	UPROPERTY(EditAnywhere, Category = "CharacterState")
	float damageDelayTime = 2.0f;
	float currentTime = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "CharacterState")
	bool isDamageState = false;

	UPROPERTY(EditAnywhere, Category = "CharacterState")
	float dieSpeed = 50.0f;
	class UEnemyAnim* anim;

protected:
	void DamageStage();

public:
	UFUNCTION(BlueprintCallable, Category = "CharacterState")
	virtual void Die();
};
