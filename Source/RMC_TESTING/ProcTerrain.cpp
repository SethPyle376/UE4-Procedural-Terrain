// Fill out your copyright notice in the Description page of Project Settings.

#include "ProcTerrain.h"


// Sets default values
AProcTerrain::AProcTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void AProcTerrain::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProcTerrain::PostActorCreated()
{
	Super::PostActorCreated();
	createTerrain(true);
}

void AProcTerrain::PostLoad()
{
	Super::PostLoad();
	createTerrain(true);
}

// Called every frame
void AProcTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProcTerrain::createTerrain(bool ocataves)
{
	int seed = FMath::RandRange(0, 1000);
	float scale = 0.0005f;
	USimplexNoiseBPLibrary::setNoiseSeed(seed);


	int height = 10000;
	int width = 10000;

	int uHeight = 100;
	int uWidth = 100;

	int totalVertices = (height / uHeight) * (width / uWidth);

	TArray<FVector> vertices;

	//Create vertices
	for (int i = 0; i < height; i += uHeight)
	{
		for (int j = 0; j < width; j += uWidth)
		{
			vertices.Add(FVector(i, j, 0));
		}
	}


	for (int i = 0; i < vertices.Num(); i++)
	{
		//Get simplex for height based on x/z values
		vertices[i].Z = (USimplexNoiseBPLibrary::SimplexNoise2D(vertices[i].X * scale, vertices[i].Y * scale) + 1) / 2 * 100;
		if (vertices[i].Z < 25)
			vertices[i].Z = 25;
		vertices[i].Z = octivate(vertices[i].X, vertices[i].Y);
	}


	//Experimental island code
	/*for (int i = 1000; i <= 8000; i += 100)
	{
		for (int j = 10; j <= 80; j++)
		{
			//int index = (i * 1000) + j;
			int index = i + j;
			vertices[index].Z = octivate(vertices[index].X, vertices[index].Y);
		}
	}*/
	

	TArray<int32> Triangles;

	int heightSpacing = height / uHeight;
	int widthSpacing = width / uWidth;

	//Set up triangles based on indices
	for (int i = 0; i < heightSpacing - 1; i++)
	{
		for (int j = 0; j < widthSpacing - 1; j++)
		{
			Triangles.Add((i * widthSpacing) + j);
			Triangles.Add((i * widthSpacing) + j + 1);
			Triangles.Add((i * widthSpacing) + widthSpacing + j);
			
			Triangles.Add((i * widthSpacing) + j + 1);
			Triangles.Add((i * widthSpacing) + j + 1 + widthSpacing);
			Triangles.Add((i * widthSpacing) + widthSpacing + j);
		}
	}
	

	TArray<FVector> normals;
	for (int i = 0; i < totalVertices; i++)
	{
		normals.Add(FVector(0, 0, 1));
	}

	
	TArray<FVector2D> UV0;

	for (int i = 0; i < totalVertices; i++)
	{
		UV0.Add(FVector2D(0, 0));
	}

	TArray<FProcMeshTangent> tangents;

	for (int i = 0; i < totalVertices; i++)
	{
		tangents.Add(FProcMeshTangent(0, 1, 0));
	}

	TArray<FLinearColor> vertexColors;

	for (int i = 0; i < totalVertices; i++)
	{
		vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
	}
	
	mesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);

	mesh->ContainsPhysicsTriMeshData(true);
}

float AProcTerrain::octivate(float x, float y)
{
	int numIterations = 3;
	float height = 0.0f;
	float frequency = 0.00015f;
	float amplitude = 800;
	float persistence = 0.03f;
	float maxAmp = 0.0f;



	for (int i = 0; i < 5; i++)
	{
		height += USimplexNoiseBPLibrary::SimplexNoise2D(x * frequency, y * frequency) * amplitude;
		amplitude *= persistence;
		frequency *= 10;
	}
	if (!(height > 10))
		height = 0;

	//if (height > 400)
		//height = 400;

	return height;
}



