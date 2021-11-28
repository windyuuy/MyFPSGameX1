// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSFloorActor.h"
#include "MapActor.generated.h"

UCLASS()
class MYFPSGAMEX1_API AMapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Ҫ���ɵķ������ࡣ
	UPROPERTY(EditAnywhere, Category = Floor)
		TSubclassOf<class AFPSFloorActor> FloorClass;

};
