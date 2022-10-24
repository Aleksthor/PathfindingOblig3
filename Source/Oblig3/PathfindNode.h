// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathfindNode.generated.h"

class UStaticMeshComponent;
class UWorld;
class UMaterial;


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
	int costToFinish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	int timeToFinish;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	ENodeSector nodeSector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	TArray<APathfindNode*> connections;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	TArray<float> phermones;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	UMaterial* resetMaterial;

	bool displayLines = true;

	float randomValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	APathfindNode* Connection1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeVariables")
	APathfindNode* Connection2;

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

	/** Setup the NodeSector Enum 
	* 
	* @param X Location of the Node
	* @param Y Location of the Node
	* @param Z Location of the Node
	* 
	*/
	void SetupEnum(float x, float y, float z);


	/** Make connections to the nodes in the same sector 
	*
	* @param Full Array of nodes
	*
	*/
	void SetConnection(TArray<APathfindNode*> fullArray);

	/** Make a connections from all sectors to ehe "capital"
	*
	* @param Full Array of nodes
	*
	*/
	void ConnectSectors(TArray<APathfindNode*> fullArray);

	/** Set ID of the node
	*
	* @param ID as integer
	*
	*/
	void SetID(int i);

	/** Returns the ID of this node */
	int GetID();

	/** Returns the cost from us to parent
	*
	* @param parent node
	*
	*/
	int GetTotalCost(APathfindNode* parent);

	/** Set the Total Cost of this Node
	*
	* @param The cost to set
	*
	*/
	void SetTotalCost(int input);

	/** Create a Connection to another Node
	*
	* @param Node to connect to
	*
	*/
	void NewConnection(APathfindNode* newConnection);
	
	/** Destroy this Node */
	void Kill();

	/** Reset Material to empty */
	void ResetMaterial();

	/** Returns the length from this node to end
	*
	* @param End node
	*
	*/
	int GetDistanceToFinish(APathfindNode* finish);

	/** Connect to all other nodes in the array
	*
	* @param Full array of all nodes
	*
	*/
	void ConnectToAll(TArray<APathfindNode*> fullArray);

	/** Set the phermone level to 1 for all paths */
	void InitPhermones();
	
	/** Update the phermones of a path 
	* 
	* @param The ID of the node we're walking down
	* @param The evaporation rate
	* 
	*/
	void CalcPhermones(int ID, float evaporation);

	/** Display the Lines the ant walked down */
	void DisplayAntLines();

};
