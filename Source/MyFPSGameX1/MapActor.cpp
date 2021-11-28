// Fill out your copyright notice in the Description page of Project Settings.


#include "MapActor.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
AMapActor::AMapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapActor::BeginPlay()
{
	Super::BeginPlay();

}

void AMapActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (FloorClass) {
		UWorld* World = GetWorld();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		AFPSFloorActor* floor = World->SpawnActor<AFPSFloorActor>(FloorClass, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
		if (floor)
		{
			floor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

// Called every frame
void AMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* World = GetWorld();
	TArray<AActor*> Actors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), Actors);
	UGameplayStatics::GetAllActorsWithTag(World, FName(TEXT("StaticMesh")), Actors);


}

