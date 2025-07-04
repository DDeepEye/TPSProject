// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void PlayerMove();

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputMappingContext* imc_TPS;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_LookUp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Turn;

	void Turn(const struct FInputActionValue& inputValue);
	void LookUp(const struct FInputActionValue& inputValue);


	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Run;

	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 200;

	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float runSpeed = 600;

	FVector direction;

	void Move(const struct FInputActionValue& inputValue);
	void InputJump(const struct FInputActionValue& inputValue);
	void InputRun();	


	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class UStaticMeshComponent* sniperGunComp;

	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Fire;

	void InputFire(const struct FInputActionValue& inputValue);


	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_GrenadeGun;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_SniperGun;

	bool bUsingGrenadeGun = true;

	void ChangeToGrenadeGun(const struct FInputActionValue& inputValue);
	void ChangeToSniperGun(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Sniper;
	void SniperAim(const struct FInputActionValue& inputValue);
	bool bSniperArm = false;

	UPROPERTY(EditDefaultsOnly, Category=SniperUI)
	TSubclassOf<class UUserWidget> sniperUIFactory;
	UPROPERTY()
	class UUserWidget* _sniperUI;

	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	UPROPERTY()
	class UUserWidget* _crosshairUI;

	UPROPERTY(EditDefaultsOnly, Category = BulletEffect)
	class UParticleSystem* bulletEffectFactory;

	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	TSubclassOf<class UCameraShakeBase> cameraShake;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* bulletSound;
};
