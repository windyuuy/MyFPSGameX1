// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSFloorActor.h"
PRAGMA_DISABLE_OPTIMIZATION

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

	//2193*548
	if (TreeActorClass != nullptr) {
		srand(567);
		UWorld* world = GetWorld();
		float maxDiv = 200;
		for (int i = 0; i < 2193 / maxDiv; i++) {
			float randX = ((float)(rand() % 1000 + 1)) / 1000.0f;
			float signX = 0;
			if (randX > 0.5f) {
				signX = 1;
			}
			else if (randX < 0.5f) {
				signX = -1;
			}
			float rangeX = (randX * 120/2+350/2)* signX;

			const float maxLen = 2193;
			float randY = ((float)(rand() % 53682)) / 53682.0;
			float rangeY = (randY- 0.5f) * 2193 ;

			FVector location = FVector(rangeX, rangeY, 0);
			AActor* Tree = world->SpawnActor<AActor>(TreeActorClass, location, FRotator(0, 0, 0), SpawnParams);
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

