// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/IntVector.h"
#include "Pyramid.generated.h"

class ABlock;
class ACubeGameState;
class APlayerState;

UCLASS()
class NIMBLEGIANTTEST_API APyramid : public AActor
{
	GENERATED_BODY()
	
public:	
	APyramid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = Blocks)
	TSubclassOf<ABlock> BlockBP;
	UPROPERTY(EditAnywhere, Category = "Blocks")
	FVector StartingPosition;
	UPROPERTY(EditAnywhere, Category = "Blocks")
	TMap<FIntPoint, ABlock*> PyramidCoordinates;
	UPROPERTY(EditAnywhere, Category = "Blocks")
	int LastPointCount;
	UPROPERTY(EditAnywhere, Category = "Blocks")
	TMap<FIntPoint, FVector> PyramidBlockWorldPositions;
	void GeneratePyramidLevel(int Level, float Padding, int LevelAmount);
	UWorld* World;
	UPROPERTY(EditAnywhere, Category = "Block Settings")
	TArray<FLinearColor> PossibleCubeColors;
	int PyramidSize;
	UPROPERTY(EditAnywhere, Category = "Block Settings")
	TArray<FIntPoint> DirectionsToCheck;
	UPROPERTY(EditAnywhere, Category = "Game State")
	ACubeGameState* GameState;
	void EnableFallForFloatingBlocks();
	bool IsTheBlockFloating(FIntPoint BlockCoordinates);
	FIntPoint FindTargetCoordinateForFallingBlock(FIntPoint FallingBlockCoordinates);
	int ContinueBlockDestructionCascade(FIntPoint TargetedBlockCoordinates, FLinearColor ColorToCompare, int LastFibonacciValue, int SecondLastFibonacciValue, int PartialSum);

public:	
	virtual void Tick(float DeltaTime) override;
	void GeneratePyramid(int Height, float Padding);
	void StartBlockCascadeDestruction(FIntPoint TargetedBlockCoordinates, FLinearColor ColorToCompare);
	void AddCubeToPyramidCoordinates(ABlock* BlockToAdd, FIntPoint Coordinates);
};
