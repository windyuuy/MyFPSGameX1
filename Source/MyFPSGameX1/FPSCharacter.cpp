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

		// ����ĳЩ������Ӱ�Ա�ʵ��ֻ�е���������ĸо���
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
	// ��ȡ��ǰ����X�᷽��
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, value);
}

void AFPSCharacter::MoveRight(float value)
{
	// ��ȡ��ǰ����Y�᷽��
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
	// ��ͼ���䷢���
	if (ProjectileClass)
	{
		// ��ȡ������任��
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// ����MuzzleOffset�����Կ��������ǰ���ɷ����
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// ��MuzzleOffset��������ռ�任������ռ䡣
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// ʹĿ�귽����������б��
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// ��ǹ��λ�����ɷ����
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// ���÷�����ĳ�ʼ�켣��
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

// ���������������ʱ����õĺ�����
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}

	Destroy();
}
