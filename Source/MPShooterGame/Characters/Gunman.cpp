// Fill out your copyright notice in the Description page of Project Settings.


#include "Gunman.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MPShooterGame/Components/CombatComponent.h"
#include "MPShooterGame/Weapons/GunmanWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Physics/Experimental/PhysScene_Chaos.h"

// Sets default values
AGunman::AGunman()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	mSpringArm->SetupAttachment(GetMesh());
	mSpringArm->TargetArmLength = 600.0f;
	mSpringArm->bUsePawnControlRotation = true;

	mFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	mFollowCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);
	mFollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AGunman::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGunman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGunman::SetOverlappingWeapon(class AGunmanWeapon* Weapon)
{
	if (Weapon == nullptr && IsLocallyControlled())
	{
		OverlappedWeapon->ShowPickupWidget(false);
	}

	OverlappedWeapon = Weapon;
	if (OverlappedWeapon && IsLocallyControlled())
	{
		OverlappedWeapon->ShowPickupWidget(true);
	}
}

// Called to bind functionality to input
void AGunman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(mMovementAction, ETriggerEvent::Triggered, this, &AGunman::Move);
		EnhancedInputComponent->BindAction(mJumpAction, ETriggerEvent::Triggered, this, &AGunman::Jump);
		EnhancedInputComponent->BindAction(mTurn, ETriggerEvent::Triggered, this, &AGunman::ChangeYaw);
		EnhancedInputComponent->BindAction(mLookUp, ETriggerEvent::Triggered, this, &AGunman::ChangePitch);
		EnhancedInputComponent->BindAction(mEquip, ETriggerEvent::Triggered, this, &AGunman::OnEquip);
	}
}

void AGunman::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AGunman, OverlappedWeapon, COND_OwnerOnly);
}

void AGunman::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComponent)
	{
		CombatComponent->Gunman = this;
	}
}

void AGunman::ChangeYaw(const FInputActionValue& value)
{
	AddControllerYawInput(value.Get<float>());
}

void AGunman::ChangePitch(const FInputActionValue& value)
{
	AddControllerPitchInput(value.Get<float>() * mMouseSenstivity);
}

void AGunman::OnEquip()
{
	if (CombatComponent == nullptr)
	{
		return;
	}
	if (HasAuthority())
	{
		CombatComponent->EquipWeapon(OverlappedWeapon);
	}
	else
	{
		RPC_EquipButtonPressed();
	}
}

void AGunman::RPC_EquipButtonPressed_Implementation()
{
	if (CombatComponent == nullptr || HasAuthority() == false)
	{
		return;
	}

	CombatComponent->EquipWeapon(OverlappedWeapon);
}


void AGunman::OnRep_OverlappingWeapon(AGunmanWeapon* LastWeapon)
{
	if (OverlappedWeapon)
	{
		OverlappedWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}


void AGunman::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}
