// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TreeActor.h"
#include "GameFramework/Actor.h"
#include "FPSFloorActor.generated.h"

UCLASS()
class MYFPSGAMEX1_API AFPSFloorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSFloorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* FloorMesh;

	UPROPERTY(EditAnywhere, Category = Tree)
		TSubclassOf<class ATreeActor> TreeActorClass;

};
