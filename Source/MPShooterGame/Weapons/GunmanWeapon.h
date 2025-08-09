// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunmanWeapon.generated.h"

UCLASS()
class MPSHOOTERGAME_API AGunmanWeapon : public AActor
{
	GENERATED_BODY()

public:
	AGunmanWeapon();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ShowPickupWidget(bool show);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Custom Components")
	UStaticMeshComponent* WeaponMesh = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Custom Components")
	class USphereComponent* CollisionVolume = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Custom Components")
	class UWidgetComponent* PickUpWidget = nullptr;
};
