// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "EnhancedinputSubsystems.h"
#include "EnhancedinputComponent.h"
#include "InputActionValue.h"
#include "Bullet.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "Enemy.h"


// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0,-90,0));
	}

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;

	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}

	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	if (TempSniperMesh.Succeeded())
	{
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		sniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}

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

	bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(imc_TPS, 0);
		}
	}

	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	_crosshairUI->AddToViewport();

	ChangeToSniperGun(FInputActionValue());
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerMove();
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (PlayerInput)
	{
		PlayerInput->BindAction(ia_Turn, ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);
		PlayerInput->BindAction(ia_LookUp, ETriggerEvent::Triggered, this, &ATPSPlayer::LookUp);
		PlayerInput->BindAction(ia_Move, ETriggerEvent::Triggered, this, &ATPSPlayer::Move);
		PlayerInput->BindAction(ia_Jump, ETriggerEvent::Started, this, &ATPSPlayer::InputJump);
		PlayerInput->BindAction(ia_Fire, ETriggerEvent::Started, this, &ATPSPlayer::InputFire);
		PlayerInput->BindAction(ia_GrenadeGun, ETriggerEvent::Started, this, &ATPSPlayer::ChangeToGrenadeGun);
		PlayerInput->BindAction(ia_SniperGun, ETriggerEvent::Started, this, &ATPSPlayer::ChangeToSniperGun);
		PlayerInput->BindAction(ia_Sniper, ETriggerEvent::Started, this, &ATPSPlayer::SniperAim);
		PlayerInput->BindAction(ia_Sniper, ETriggerEvent::Completed, this, &ATPSPlayer::SniperAim);
	}
}

void ATPSPlayer::PlayerMove()
{
	direction = (FTransform(GetControlRotation()).TransformVector(direction)).GetSafeNormal();
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void ATPSPlayer::Turn(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerYawInput(value);
}

void ATPSPlayer::LookUp(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value);
}

void ATPSPlayer::Move(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	direction.X = value.X;
	direction.Y = value.Y;
}

void ATPSPlayer::InputJump(const struct FInputActionValue& inputValue)
{
	Jump();
}

void ATPSPlayer::InputFire(const struct FInputActionValue& inputValue)
{
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
		params.AddIgnoredActor(this);

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
			if (hitInfo.GetActor()->IsA(AEnemy::StaticClass()))
			{
				Cast<AEnemy>(hitInfo.GetActor())->OnDamageProcess();
			}
		}
	}
}


void ATPSPlayer::ChangeToGrenadeGun(const struct FInputActionValue& inputValue)
{
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}
void ATPSPlayer::ChangeToSniperGun(const struct FInputActionValue& inputValue)
{
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void ATPSPlayer::SniperAim(const struct FInputActionValue& inputValue)
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