// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Canvas.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYFPSGAMEX1_API AFPSHUD : public AHUD
{
	GENERATED_BODY()

protected:
    // ������������Ļ���ġ�
    UPROPERTY(EditDefaultsOnly)
        UTexture2D* CrosshairTexture;

public:
    // HUD���Ƶ���Ҫ���á�
    virtual void DrawHUD() override;

};
