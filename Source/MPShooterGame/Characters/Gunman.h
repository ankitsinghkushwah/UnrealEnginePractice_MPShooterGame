// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Gunman.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class MPSHOOTERGAME_API AGunman : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGunman();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void ChangeYaw(const FInputActionValue& value);

	UFUNCTION()
	void ChangePitch(const FInputActionValue& value);

protected:
	UPROPERTY(EditAnywhere, Category=Input)
	class UInputAction* mMovementAction;
	UPROPERTY(EditAnywhere, Category=Input)
	class UInputAction* mJumpAction;
	UPROPERTY(EditAnywhere, Category=Input)
	class UInputAction* mTurn;
	UPROPERTY(EditAnywhere, Category=Input)
	class UInputAction* mLookUp;
	UPROPERTY(EditAnywhere, Category=Input)
	float mMouseSenstivity = 0.1f;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* mSpringArm;

	class UCameraComponent* mFollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;
};
