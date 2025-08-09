// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "MPShooterGame/Characters/Gunman.h"
#include "MPShooterGame/Weapons/GunmanWeapon.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::EquipWeapon(AGunmanWeapon* Weapon)
{
	if (Gunman == nullptr || Weapon == nullptr)
	{
		return;
	}

	EquippedWeapon = Weapon;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* Socket = Gunman->GetMesh()->GetSocketByName(FName("WeaponSocket"));
	if (Socket)
	{
		Socket->AttachActor(EquippedWeapon, Gunman->GetMesh());
	}

	EquippedWeapon->SetOwner(Gunman);
	EquippedWeapon->ShowPickupWidget(false);
}
