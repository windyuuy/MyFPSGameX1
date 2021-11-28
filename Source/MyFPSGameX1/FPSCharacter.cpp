// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Classes/Engine/StaticMeshActor.h>
#include "Components/StaticMeshComponent.h"
PRAGMA_DISABLE_OPTIMIZATION
// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (FPSCameraComponent == nullptr) {
		FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
		check(FPSCameraComponent != nullptr);

		FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

		//FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0, 50.0f + BaseEyeHeight));
		//FPSCameraComponent->bUsePawnControlRotation = true;
	}

	{
		FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
		check(FPSMesh != nullptr);

		//FPSMesh->SetOnlyOwnerSee(true);
		FPSMesh->SetOwnerNoSee(true);
		FPSMesh->SetupAttachment(FPSCameraComponent);

		// 禁用某些环境阴影以便实现只有单个网格体的感觉。
		FPSMesh->bCastDynamicShadow = false;
		FPSMesh->CastShadow = false;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh>TreeMesh(TEXT("StaticMesh'/Game/Content/Models/M2/Tree/Tree.Tree'"));
	if (TreeMesh.Succeeded()) {
		SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere_Mesh"));
		check(SphereVisual != nullptr);
		SphereVisual->SetStaticMesh(TreeMesh.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f,0,0));
		//SphereVisual->SetWorldScale3D(FVector(0.2f, 0.04f, 0.02f));
		SphereVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereVisual->SetupAttachment(GetRootComponent());
	}

	//GetMesh()->DestroyComponent();
	//USkeletalMeshComponent* mesh = GetMesh();
	////mesh->SetOwnerNoSee(true);
	//mesh->SetVisibility(true);

}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));

	//MoveRight(1);

}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->MoveForward(1);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnAround", this, &AFPSCharacter::RollAround);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::RollUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AFPSCharacter::Fire);

}

void AFPSCharacter::RollUp(float value) {
	this->AddControllerPitchInput(value);
}

void AFPSCharacter::RollAround(float value) {
	this->AddControllerYawInput(value);
}

void AFPSCharacter::MoveForward(float value)
{
	// 获取当前控制X轴方向
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, value);
}

void AFPSCharacter::MoveRight(float value)
{
	// 获取当前控制Y轴方向
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value);
}

void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::StopJump()
{
	bPressedJump = false;
}
void AFPSCharacter::Fire()
{
	// 试图发射发射物。
	if (ProjectileClass)
	{
		// 获取摄像机变换。
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// 设置MuzzleOffset，在略靠近摄像机前生成发射物。
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// 将MuzzleOffset从摄像机空间变换到世界空间。
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// 使目标方向略向上倾斜。
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// 在枪口位置生成发射物。
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// 设置发射物的初始轨迹。
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

// 当发射物击中物体时会调用的函数。
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}

	Destroy();
}
