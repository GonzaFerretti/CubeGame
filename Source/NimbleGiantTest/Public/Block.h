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
	UPROPERTY(EditAnywhere, Category = Visual)
	FLinearColor CubeColor;
	UPROPERTY(EditAnywhere, Category = "Pyramid Data")
	FIntVector CurrentPyramidCoordinates;
	UPROPERTY(EditAnywhere, Category = "Pyramid Data")
	APyramid* ParentPyramid;

public:	
	virtual void Tick(float DeltaTime) override;
	void SetTintedMaterial(UMaterialInstanceDynamic* TintedMaterial, FLinearColor Color);
	void InitializeBlock(APyramid* _ParentPyramid, FIntVector _CurrentCoordinates);
	void StartBlockDestruction();
	UMaterialInterface* GetBaseMaterial();
	FLinearColor GetColor();
};
