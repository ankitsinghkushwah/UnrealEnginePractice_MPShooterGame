// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"

#include "Components/TextBlock.h"

void UOverheadWidget::SetDisplayText(FString InDisplayText)
{
	if (DisplayText != nullptr)
	{
		DisplayText->SetText(FText::FromString(InDisplayText));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	ENetRole LocalRole = InPawn->GetLocalRole();

	FString role = FString("Local Role : ") + StaticEnum<ENetRole>()->GetDisplayNameTextByValue(LocalRole).ToString();
	SetDisplayText(role);
}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();

	Super::NativeDestruct();
}
