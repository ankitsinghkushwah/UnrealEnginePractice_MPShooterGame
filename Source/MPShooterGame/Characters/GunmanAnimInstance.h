// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GunmanAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MPSHOOTERGAME_API UGunmanAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess=true))
	class AGunman* mGunman;
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess=true))
	float mSpeed = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess=true))
	bool mIsInAir = false;
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess=true))
	bool mIsAccelerating = false;
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess=true))
	bool mWeaponEquipped = false;
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess=true))
	bool mIsCrouched = false;
};
