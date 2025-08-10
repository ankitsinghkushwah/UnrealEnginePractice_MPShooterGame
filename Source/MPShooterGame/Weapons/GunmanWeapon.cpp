// Fill out your copyright notice in the Description page of Project Settings.


#include "GunmanWeapon.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "MPShooterGame/Characters/Gunman.h"
#include "MPShooterGame/Components/CombatComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGunmanWeapon::AGunmanWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	CollisionVolume->SetupAttachment(RootComponent);
	CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionVolume->SetCollisionResponseToAllChannels(ECR_Ignore);

	PickUpWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickUpWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGunmanWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CollisionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CollisionVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AGunmanWeapon::OnCollisionOverlapBegin);
		CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AGunmanWeapon::OnCollisionOverlapEnd);
	}

	if (PickUpWidget != nullptr)
	{
		PickUpWidget->SetVisibility(false);
	}
}

void AGunmanWeapon::OnRep_WeaponState()
{
	switch (CurrentWeaponState)
	{
	case EWS_Equipped:
		ShowPickupWidget(false);
		break;
	}
}

void AGunmanWeapon::SetWeaponState(EWeaponState state)
{
	if (HasAuthority() == false)
	{
		return;
	}

	CurrentWeaponState = state;
	switch (state)
	{
	case EWeaponState::EWS_Equipped:
		CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

// Called every frame
void AGunmanWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGunmanWeapon::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGunmanWeapon, CurrentWeaponState);
}

void AGunmanWeapon::OnCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                            const FHitResult& SweepResult)
{
	AGunman* Gunman = Cast<AGunman>(OtherActor);
	if (Gunman)
	{
		Gunman->SetOverlappingWeapon(this);
	}
}

void AGunmanWeapon::OnCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGunman* Gunman = Cast<AGunman>(OtherActor);
	if (Gunman)
	{
		Gunman->SetOverlappingWeapon(nullptr);
	}
}

void AGunmanWeapon::ShowPickupWidget(bool show)
{
	if (PickUpWidget)
	{
		PickUpWidget->SetVisibility(show);
	}
}
