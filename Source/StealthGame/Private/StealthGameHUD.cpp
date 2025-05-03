// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthGameHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

AStealthGameHUD::AStealthGameHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/UI/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}

void AStealthGameHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas

	const FVector2D CrosshairDrawPosition((Center.X),
		(Center.Y + 20.0f));

	FTexture* SlateTexture = CrosshairTex->GetResource();
	if (SlateTexture)
	{
		FCanvasTileItem TileItem(CrosshairDrawPosition, SlateTexture, FVector2D(CrosshairTex->GetSurfaceWidth(), CrosshairTex->GetSurfaceHeight()), FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}
