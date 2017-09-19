// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "SimplexNoiseBPLibrary.h"
#include "ProcTerrain.generated.h"

UCLASS()
class RMC_TESTING_API AProcTerrain : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * mesh;

	void createTerrain(bool ocataves);
	float octivate(float x, float y);
	
public:	
	// Sets default values for this actor's properties
	AProcTerrain();

	void PostActorCreated();
	void PostLoad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
