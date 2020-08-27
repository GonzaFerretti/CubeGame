 // Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "Math/IntVector.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Pyramid.h"

// Sets default values
ABlock::ABlock()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
}

void ABlock::SetCubeColor(FLinearColor Color)
{
	CubeColor = Color;
	// The blocks on the client server won't be able to create their materials until the Color is replicated. The server, however, already can.
	if (HasAuthority())
	{
		ABlock::CreateAndSetTintedMaterial();
	}
}

void ABlock::InitializeBlock(APyramid* _ParentPyramid, FIntPoint _CurrentCoordinates)
{
	ParentPyramid = _ParentPyramid;
	CurrentPyramidCoordinates = _CurrentCoordinates;
}

FLinearColor ABlock::GetColor()
{
	return CubeColor;
}

void ABlock::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABlock, CubeColor);
	DOREPLIFETIME(ABlock, CurrentPyramidCoordinates);
}

void ABlock::OnRep_CubeColorClientUpdate()
{
	ABlock::CreateAndSetTintedMaterial();
}

void ABlock::CreateAndSetTintedMaterial()
{
	UMaterialInstanceDynamic* TintedMaterial = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), this, FName("Material"));
	TintedMaterial->SetVectorParameterValue("TintColor", CubeColor);
	Mesh->SetMaterial(0, TintedMaterial);
}

void ABlock::SetBlockFall(FVector TargetPosition, FIntPoint TargetCoordinates)
{
	_TargetPosition = TargetPosition;
	_TargetCoordinates = TargetCoordinates;
	CurrentPyramidCoordinates = TargetCoordinates;
	ParentPyramid->AddCubeToPyramidCoordinates(this, CurrentPyramidCoordinates);
	bIsFalling = true;
}

void ABlock::MoveTowardsFallPosition(float DeltaTime)
{
	FVector NextCubePosition = GetActorLocation() + FVector::DownVector * FallSpeed * DeltaTime;
	if (FMath::Abs(NextCubePosition.Z - _TargetPosition.Z) < FallSnapDistance)
	{
		SetActorLocation(_TargetPosition);
		CurrentPyramidCoordinates = _TargetCoordinates;
		bIsFalling = false;
	}
	else
	{
		SetActorLocation(NextCubePosition);
	}
}

bool ABlock::IsFalling()
{
	return bIsFalling;
}

FIntPoint ABlock::GetPyramidCoordinates()
{
	return CurrentPyramidCoordinates;
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsFalling)
	{
		ABlock::MoveTowardsFallPosition(DeltaTime);
	}
}

