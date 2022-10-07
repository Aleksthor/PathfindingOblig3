// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathfindNode.generated.h"

class UStaticMeshComponent;
class UWorld;

UENUM(BlueprintType)
enum class ENodeSector : uint8
{

	UP_North_East		UMETA(DisplayName = "UP_North_East"),
	UP_South_East		UMETA(DisplayName = "UP_South_East"),
	UP_South_West       UMETA(DisplayName = "UP_South_West"),
	UP_North_West		UMETA(DisplayName = "UP_North_West"),
	DOWN_North_East		UMETA(DisplayName = "DOWN_North_East"),
	DOWN_South_East		UMETA(DisplayName = "DOWN_South_East"),
	DOWN_South_West     UMETA(DisplayName = "DOWN_South_West"),
	DOWN_North_West		UMETA(DisplayName = "DOWN_North_West"),

	MAX					UMETA(DisplayName = "DefaultMAX")

};

UCLASS()
class OBLIG3_API APathfindNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathfindNode();



	/** Components */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* mesh;

	

	/** Variables */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	int ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	int totalCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	int timeToFinish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	ENodeSector nodeSector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	TArray<APathfindNode*> connections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	TArray<int> movementCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	bool isFound;

	int connectionAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	bool sectorConnected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	APathfindNode* previousNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	bool markedAsFastestRoute;

private: 

	// Private Variables

	UWorld* world;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DebugLine();

	void SetupEnum(float x, float y, float z);

	void SetConnection(TArray<APathfindNode*> fullArray);

	void ConnectSectors(TArray<APathfindNode*> fullArray);

	void SetID(int i);

	int GetID();

	int GetTotalCost(APathfindNode* parent);

	void SetTotalCost(int input);

	void NewConnection(APathfindNode* newConnection);
	

};
