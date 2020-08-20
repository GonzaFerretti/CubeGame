 // Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "Math/IntVector.h"
#include "Pyramid.h"

// Sets default values
ABlock::ABlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
	//UMaterialInstanceDynamic* TintedMaterial = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), this, TEXT("TintedMaterial"));
	//TintedMaterial->SetVectorParameterValue("TintColor", TestColor);
	//Mesh->SetMaterial(0, TintedMaterial);
	
}

void ABlock::SetTintedMaterial(UMaterialInstanceDynamic* TintedMaterial, FLinearColor Color)
{
	Mesh->SetMaterial(0, TintedMaterial);
	CubeColor = Color;
}

UMaterialInterface* ABlock::GetBaseMaterial()
{
	return Mesh->GetMaterial(0);
}

void ABlock::InitializeBlock(APyramid* _ParentPyramid, FIntVector _CurrentCoordinates)
{
	ParentPyramid = _ParentPyramid;
	CurrentPyramidCoordinates = _CurrentCoordinates;
}

void ABlock::StartBlockDestruction()
{
	ParentPyramid->ExecuteAttackOnBlock(CurrentPyramidCoordinates, CubeColor);
	Destroy();
}

FLinearColor ABlock::GetColor()
{
	return CubeColor;
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

