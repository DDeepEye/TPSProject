// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerFire();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void SetupInputBinding(class UEnhancedInputComponent* PlayerInput);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Fire;	

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* bulletSound;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_GrenadeGun;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_SniperGun;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Sniper;			

	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(EditDefaultsOnly, Category = BulletEffect)
	class UParticleSystem* bulletEffectFactory;

	bool bUsingGrenadeGun = true;
	bool bSniperArm = false;

	UPROPERTY(EditDefaultsOnly, Category=SniperUI)
	TSubclassOf<class UUserWidget> sniperUIFactory;
	UPROPERTY()
	class UUserWidget* _sniperUI;

	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	UPROPERTY()
	class UUserWidget* _crosshairUI;

	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	TSubclassOf<class UCameraShakeBase> cameraShake;

	UPROPERTY()
	class UCameraComponent* tpsCamComp;

	UPROPERTY()
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY()
	class UStaticMeshComponent* sniperGunComp;

	// Called to bind functionality to input

	void ChangeToGrenadeGun(const struct FInputActionValue& inputValue);
	void ChangeToSniperGun(const struct FInputActionValue& inputValue);
	void SniperAim(const struct FInputActionValue& inputValue);
	void InputFire(const struct FInputActionValue& inputValue);

};
