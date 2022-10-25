// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainController.generated.h"

/** Forward declarations*/

class UCameraComponent;
class UBoxComponent;
class APathfindNode;
class UWorld;
class USpringArmComponent;
class UMaterial;
class UChromosome;



UCLASS()
class OBLIG3_API AMainController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainController();

	/** Variables */

	// Material for the start Node in A* and Djikstra
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodes")
	UMaterial* startMaterial;

	// Material for the end Node in A* and Djikstra
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodes")
	UMaterial* endMaterial;

	// The Amount of Nodes we spawn in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodes")
	int NodeAmount;

	// Min X, Y & Z position for the Nodes 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodes")
	float minPos;

	// Max X, Y & Z position for the Nodes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodes")
	float maxPos;

	// The array of all spawned Nodes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TArray<APathfindNode*> nodes;

	// Reference to the World
	UWorld* world;


	/** Components */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* camera {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UBoxComponent* boxComponent {nullptr};


	/** Nodes */ 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodes")
	TSubclassOf<APathfindNode> pathfindNodeBP;

	// The starting Node of pathfind algorithm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Djikstra")
	int start;

	// The end Node of pathfind algorithm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Djikstra")
	int end;



	/** Genetic Algorithm */

	// Size of population
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetic")
	int population;

	// How many iterations we want to do
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetic")
	int iterations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetic")
	int currentIteration;

	// The population spawned in as chromosomes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetic")
	TArray<UChromosome*> chromosomes;

	// The best "individual" so far
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetic")
	UChromosome* bestRoute;

	// The distance of BestRoute
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance")
	int bestDistance;


	/** Booleans for Tick */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool isRunningDjisktra;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool djisktraFound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool isRunningGenetic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool isRunningAntColony;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool isRunningNearestInsertion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool isRunningAStar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool AStarFound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool geneticFound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool antColonyFound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool nearestInsertionFound;


	/** Ant Colony */

	// Evaporation Rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ant Colony")
	float evaporation;

	// Array of probabilities
	TArray<double> probabilities;

	// Current Ant Path
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ant Colony")
	TArray<int> pathID;

	// Best Ant path so far
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ant Colony")
	TArray<int> bestPathID;

	// Probability nevner
	float nevner;

	// Distance of the best Ant
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ant Colony")
	int bestAntRoute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Insertion")
	int bestInsertionRoute;

	// the currentNode is a pointer to the node we're currently using
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodes")
	APathfindNode* currentNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Insertion")
	int maxIterationsAllowed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Insertion")
	bool slowDownInsertion;

	float iterationFloat;
private:


	TArray<APathfindNode*> tempArray;
	APathfindNode* previousNode;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Tick Function that runs the Djikstra Algorithm */
	void RunDjikstra();

	/** Tick Function that runs the A Star Algorithm */
	void RunAStar();

	/** Tick Function that runs the Genetic Algorithm */
	void RunGeneticAlgorithm();

	/** Generate half of the population
	* 
	* Function will run when we start the Genetic Algorithm
	* 
	*/
	void GenerateRandomChromosomes();

	/** Generate 4 % of the max population
	* 
	* This function will intruduse new Chromosomes to the popualation
	* 
	*/
	void Create5PercentNewChromosomes();

	/** Sort the poplation and remove the worst half */
	void GetBestChromosomes();


	UFUNCTION(BlueprintCallable)
	void StopGenetic();

	/** Display Debug Lines for AntColony*/
	void DisplayFastest();

	/** Tick Function that runs the Ant Colony Algorithm */
	void RunAntColony();

	/** Ant Colony Function
	* 
	* Will tell the ant to go back to start when all nodes have been checked
	* 
	*/
	void ConnectStart();

	/** Returns the offspring from 2 parent Chromosomes
	* 
	* @param Chromosome of parent 1
	* @param Chromosome of parent 2
	* 
	*/
	UChromosome* CreateOffspring(UChromosome* parent1, UChromosome* parent2);


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	/** Initialize Djikstra Algorithm */
	void Djisktra();

	UFUNCTION(BlueprintCallable)
	/** Spawn Nodes for pathfinding */
	void SpawnNodes();

	UFUNCTION(BlueprintCallable)
	/** Spawn Nodes for Traveling Salesman Problem */
	void SpawnTSPNodes();

	UFUNCTION(BlueprintCallable)
	/** Delete all Nodes */
	void DeleteNodes();

	UFUNCTION(BlueprintCallable)
	/** Initialize A* Algorithm */
	void AStar();

	UFUNCTION(BlueprintCallable)
	/** Initialize Genetic Algorithm */
	void GeneticAlgorithm();

	UFUNCTION(BlueprintCallable)
	/** Initialize Ant Colony Algorithm */
	void AntColony();
	

	UFUNCTION(BlueprintCallable)
	/** Initialize Nearest Insertion Algorithm */
	void NearestInsertion();

	/** Tick Function that runs the Nearest Insertion Algorithm */
	void RunNearestinsertion(float deltaTime);

	/** Reset Tick Booleans to false */
	void ResetBooleans();

	/** Returns a mutated path from a parent 
	* 
	* @param Chromosome of parent to mutate
	* 
	*/
	TArray<APathfindNode*> GetMutatedRoute(UChromosome* parent);

};
