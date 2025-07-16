// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"
#include <Camera/CameraComponent.h>
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Bullet.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "EnemyFSM.h"
#include "PlayerAnim.h"

// Sets default values for this component's properties
UPlayerFire::UPlayerFire()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ConstructorHelpers::FClassFinder<UUserWidget> TempUserWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/WBP_SniperUI.WBP_SniperUI_C'"));
	if (TempUserWidget.Succeeded())
	{
		sniperUIFactory = TempUserWidget.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> TempCrosshairWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrints/WBP_Crosshair.WBP_Crosshair_C'"));
	if (TempCrosshairWidget.Succeeded())
	{
		crosshairUIFactory = TempCrosshairWidget.Class;
	}	

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundWave'/Game/SniperGun/Rifle.Rifle''"));
	if (tempSound.Succeeded())
	{
		bulletSound = tempSound.Object;
	}

	// ...
}


// Called when the game starts
void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	// ...
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	_crosshairUI->AddToViewport();

	tpsCamComp = me->tpsCamComp;
	gunMeshComp = me->gunMeshComp;
	sniperGunComp = me->sniperGunComp;

	ChangeToSniperGun(FInputActionValue());
}

void UPlayerFire::SetupInputBinding(class UEnhancedInputComponent* PlayerInput)
{
	PlayerInput->BindAction(ia_Fire, ETriggerEvent::Started, this, &UPlayerFire::InputFire);	
	PlayerInput->BindAction(ia_GrenadeGun, ETriggerEvent::Started, this, &UPlayerFire::ChangeToGrenadeGun);
	PlayerInput->BindAction(ia_SniperGun, ETriggerEvent::Started, this, &UPlayerFire::ChangeToSniperGun);
	PlayerInput->BindAction(ia_Sniper, ETriggerEvent::Started, this, &UPlayerFire::SniperAim);
	PlayerInput->BindAction(ia_Sniper, ETriggerEvent::Completed, this, &UPlayerFire::SniperAim);
}

void UPlayerFire::ChangeToGrenadeGun(const struct FInputActionValue& inputValue)
{
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}

void UPlayerFire::ChangeToSniperGun(const struct FInputActionValue& inputValue)
{
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void UPlayerFire::SniperAim(const struct FInputActionValue& inputValue)
{
	if (bUsingGrenadeGun)
		return;

	if (!bSniperArm)
	{
		bSniperArm = true;
		_sniperUI->AddToViewport();
		_crosshairUI->RemoveFromParent();
		tpsCamComp->SetFieldOfView(45.0f);

	}
	else
	{
		bSniperArm = false;
		_crosshairUI->AddToViewport();
		_sniperUI->RemoveFromParent();
		tpsCamComp->SetFieldOfView(90.0f);
	}
}

void UPlayerFire::InputFire(const struct FInputActionValue& inputValue)
{
	UGameplayStatics::PlaySound2D(GetWorld(), bulletSound);
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(cameraShake);
	UPlayerAnim* anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();
	if (bUsingGrenadeGun)
	{
		FTransform firePosition = gunMeshComp->GetSocketTransform((TEXT("FirePosition")));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	else
	{
		FVector startPos = tpsCamComp->GetComponentLocation();
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;
		FHitResult hitInfo;
		FCollisionQueryParams params;
		params.AddIgnoredActor(me);

		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		if (bHit)
		{
			FTransform bulletTrans;
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);
			UPrimitiveComponent* hitComp = hitInfo.GetComponent();
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				FVector dir = (endPos - startPos).GetSafeNormal();
				FVector force = dir * hitComp->GetMass() * 5000;
				hitComp->AddForceAtLocation(force, hitInfo.ImpactPoint);
			}

			UObject* enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				UEnemyFSM* enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();
			}
		}
	}
}


