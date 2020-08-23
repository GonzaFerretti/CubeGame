// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/IntVector.h"
#include "Block.generated.h"

class APyramid;

UCLASS()
class NIMBLEGIANTTEST_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlock();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, Category = Visual, ReplicatedUsing = OnRep_CubeColorClientUpdate)
	FLinearColor CubeColor;
	UPROPERTY(EditAnywhere, Category = Visual, Replicated)
	UMaterialInstanceDynamic* CubeMaterial;
	UPROPERTY(EditAnywhere, Category = "Pyramid Data", Replicated)
	FIntPoint CurrentPyramidCoordinates;
	UPROPERTY(EditAnywhere, Category = "Pyramid Data")
	APyramid* ParentPyramid;


	UPROPERTY(EditAnywhere, Category = Fall)
	float FallSnapDistance;
	UPROPERTY(EditAnywhere, Category = Fall)
	float FallSpeed;
	UPROPERTY(VisibleAnywhere, Category = Fall)
	bool bIsFalling;
	UPROPERTY(VisibleAnywhere, Category = Fall)
	FVector _TargetPosition;
	UPROPERTY(VisibleAnywhere, Category = Fall)
	FIntPoint _TargetCoordinates;
	void MoveTowardsFallPosition(float DeltaTime);
	void CreateAndSetTintedMaterial();

public:	
	virtual void Tick(float DeltaTime) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetCubeColor(FLinearColor Color);
	void InitializeBlock(APyramid* _ParentPyramid, FIntPoint _CurrentCoordinates);
	void SetBlockFall(FVector TargetPosition, FIntPoint TargetCoordinates);
	UFUNCTION()
	void OnRep_CubeColorClientUpdate();
	FLinearColor GetColor();
	bool IsFalling();
	FIntPoint GetPyramidCoordinates();
};
