// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSFloorActor.h"

// Sets default values
AFPSFloorActor::AFPSFloorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if(FloorMesh==nullptr)
	{
		FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
		check(FloorMesh != nullptr);
		FloorMesh->SetupAttachment(RootComponent);
	}

}

void AFPSFloorActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	if (TreeActorClass != nullptr) {
		UWorld* world = GetWorld();
		for (int i = 0; i < 512 / 200; i++) {
			AActor* Tree = world->SpawnActor<AActor>(TreeActorClass, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
			check(Tree != nullptr);
			Tree->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

// Called when the game starts or when spawned
void AFPSFloorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSFloorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

