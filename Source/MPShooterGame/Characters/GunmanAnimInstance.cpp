// Fill out your copyright notice in the Description page of Project Settings.


#include "GunmanAnimInstance.h"
#include "Gunman.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGunmanAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	mGunman = Cast<AGunman>(TryGetPawnOwner());
}

void UGunmanAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (mGunman == nullptr)
	{
		return;
	}

	FVector Velocity = mGunman->GetVelocity();
	Velocity.Z = 0.0f;
	mSpeed = Velocity.Size();

	mIsInAir = mGunman->GetCharacterMovement()->IsFalling();

	mIsAccelerating = mGunman->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;

	mWeaponEquipped = mGunman->IsWeaponEquipped();

	mIsCrouched = mGunman->IsCrouched();

	bIsAiming = mGunman->IsAiming();
}
