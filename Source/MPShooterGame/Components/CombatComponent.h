// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AGunmanWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MPSHOOTERGAME_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	friend class AGunman;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	void EquipWeapon(AGunmanWeapon* Weapon);

protected:
	virtual void BeginPlay() override;

	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void dummy()
	{
	}

private:
	class AGunman* Gunman = nullptr;

	UPROPERTY(Replicated)
	AGunmanWeapon* EquippedWeapon = nullptr;

	UPROPERTY(Replicated)
	bool bAiming = false;
};
