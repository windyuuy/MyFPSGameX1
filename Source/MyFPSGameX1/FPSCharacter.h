// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FPSProjectile.h"
#include "FPSCharacter.generated.h"

UCLASS()
class MYFPSGAMEX1_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int CurBlockIndex=0;

	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SphereVisual;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION()
		void MoveForward(float value);

	UFUNCTION()
		void MoveRight(float value);


	UFUNCTION()
		void StartJump();
	UFUNCTION()
		void StopJump();

	UFUNCTION()
		void RollUp(float value);

	UFUNCTION()
		void RollAround(float value);

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPSCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FPSMesh;

	UFUNCTION()
		void Fire();

	// 枪口相对于摄像机位置的偏移。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	// 要生成的发射物类。
	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<class AFPSProjectile> ProjectileClass;

};
