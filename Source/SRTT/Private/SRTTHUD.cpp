// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTTHUD.h"
#include "Blueprint/UserWidget.h"
#include "SRTTUserWidget.h" // We need to include our custom widget header

void ASRTTHUD::BeginPlay()
{
	Super::BeginPlay();

	// Check if a widget class has been assigned in the editor.
	if (HUDWidgetClass)
	{
		// Get the owning Player Controller.
		APlayerController* PC = GetOwningPlayerController();
		if (PC)
		{
			// Create the widget.
			HUDWidget = CreateWidget<USRTTUserWidget>(PC, HUDWidgetClass);

			// If the widget was created successfully, add it to the viewport.
			if (HUDWidget)
			{
				HUDWidget->AddToViewport();
			}
		}
	}
}
