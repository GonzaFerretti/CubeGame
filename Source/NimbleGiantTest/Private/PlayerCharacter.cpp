// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Block.h"
#include "GameFramework/InputSettings.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	UCapsuleComponent* _CapsuleComponent = GetCapsuleComponent();
	_CapsuleComponent->InitCapsuleSize(55.f, 96.0f);

	GetCharacterMovement()->JumpZVelocity = 600;
	GetCharacterMovement()->AirControl = 0.02f;

	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	FollowCamera->SetupAttachment(GetCapsuleComponent());
	FollowCamera->SetRelativeLocation(FVector(-39.56f,1.75f, 64));
	FollowCamera->bUsePawnControlRotation = true;
	
	ViewArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMesh"));
	ViewArmsMesh->SetOnlyOwnerSee(true);
	ViewArmsMesh->SetupAttachment(FollowCamera);
	ViewArmsMesh->bCastDynamicShadow = false;
	ViewArmsMesh->CastShadow = false;
	ViewArmsMesh->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	ViewArmsMesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::MoveForward(float Axis)
{
	if (Axis != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Axis);
	}
}

void APlayerCharacter::MoveRight(float Axis)
{
	if (Axis != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Axis);
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Shoot()
{
	FVector RayStartPosition;
	FRotator RayRotation;
	FVector EndPosition = FVector::ZeroVector;
	FHitResult HitResult;

	FCollisionQueryParams CollisionParams = FCollisionQueryParams();
	CollisionParams.AddIgnoredActor(this->GetOwner());


	APlayerController* const playerController = GetWorld()->GetFirstPlayerController();

	if (playerController)
	{
		playerController->GetPlayerViewPoint(RayStartPosition, RayRotation);
		EndPosition = RayStartPosition + RayRotation.Vector() * MaximumShotRange;
		DrawDebugLine(GetWorld(), RayStartPosition, EndPosition, FColor::Green, false, 1, 0, 1);
	}


	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, RayStartPosition, EndPosition, ECC_Visibility, CollisionParams);
	if (IsHit)
	{
		bool HitObjectIsBlock = HitResult.GetActor()->IsA<ABlock>();
		if (HitObjectIsBlock)
		{
			ABlock* HitBlock = ((ABlock*)HitResult.GetActor());
			HitBlock->StartBlockDestruction();
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::Shoot);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);


	// Camera Control
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUpAtRate);
}

