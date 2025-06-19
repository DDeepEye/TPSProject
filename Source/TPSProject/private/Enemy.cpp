// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAnim.h"
#include <Components/CapsuleComponent.h>

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(0.84f));
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/BluePrints/ABP_Enemy.ABP_Enemy_C'"));
	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	anim->animState = EEnemyState::Idle;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isDamageState)
	{
		DamageStage();
	}	
	if (hp <= 0)
	{
		Die();		
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnDamageProcess()
{
	hp--;
	if (hp > 0)
	{
		isDamageState = true;
		currentTime = 0;
	}
	else
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);		
		anim->animState = EEnemyState::Die;
	}
}

void AEnemy::DamageStage()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime)
	{
		isDamageState = false;		
	}	
}

void AEnemy::Die()
{
	FVector P0 = GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	SetActorLocation(P);

	if (P.Z < -200.0f)
	{
		SetActorHiddenInGame(true);
		Destroy(true);
	}
}



