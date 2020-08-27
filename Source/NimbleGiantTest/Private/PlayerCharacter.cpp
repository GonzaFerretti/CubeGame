// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerState.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Block.h"
#include "CubeGameState.h"
#include "Pyramid.h"
#include "ScoreboardWidget.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerController.h"
#include "CubeGamePlayerController.h"

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
	FollowCamera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64));
	FollowCamera->bUsePawnControlRotation = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(GetCapsuleComponent());

	ViewArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMesh"));
	ViewArmsMesh->SetOnlyOwnerSee(false);
	ViewArmsMesh->SetupAttachment(FollowCamera);
	ViewArmsMesh->bCastDynamicShadow = false;
	ViewArmsMesh->CastShadow = false;
	ViewArmsMesh->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	ViewArmsMesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	GunMesh->SetOnlyOwnerSee(false);			
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;
	GunMesh->SetupAttachment(ViewArmsMesh);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GunMesh->AttachToComponent(ViewArmsMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
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
			FIntPoint BlockCoordinates = HitBlock->GetPyramidCoordinates();
			FLinearColor BlockColor = HitBlock->GetColor();
			GetWorld()->GetGameState<ACubeGameState>()->GetPyramid()->StartBlockCascadeDestruction(BlockCoordinates, BlockColor, GetPlayerState());
		}
	}
}

void APlayerCharacter::SetupClientSideMatchEnd_Implementation()
{
	if (InputComponent)
	{
		for (int i = 0; i < InputComponent->GetNumActionBindings(); i++)
		{
			InputComponent->RemoveActionBindingForHandle(InputComponent->GetActionBinding(0).GetHandle());
		}
		InputComponent->AxisBindings = TArray<FInputAxisBinding>();
	}
	FTimerHandle WaitForScoreTimerHandle;
	GetWorldTimerManager().SetTimer(WaitForScoreTimerHandle, this, &APlayerCharacter::ShowFinalScoreBoard, 1.5f, false);
}

void APlayerCharacter::ShowFinalScoreBoard()
{
	((ACubeGamePlayerController*)GetController())->ScoreBoardUI->ShowGameOver(GetWorld()->GetGameState()->PlayerArray);
}

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

