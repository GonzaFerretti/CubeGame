// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "PlayerCharacter.generated.h"

class UScoreboardWidget;

UCLASS()
class NIMBLEGIANTTEST_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void Shoot();
	UFUNCTION()
	void ShowFinalScoreBoard();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ViewArmsMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* GunMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BodyMesh;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(Client, Reliable)
	void SetupClientSideMatchEnd();
	void SetupClientSideMatchEnd_Implementation();
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float MaximumShotRange;
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	float BaseTurnRate;
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	float BaseLookUpRate;
};
