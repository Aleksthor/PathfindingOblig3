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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodes")
	UMaterial* startMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodes")
	UMaterial* endMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodes")
	int NodeAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodes")
	float minPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodes")
	float maxPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TArray<APathfindNode*> nodes;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Djikstra")
	int start;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Djikstra")
	int end;

	/** Genetic Algorithm */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetic")
	int population;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetic")
	int iterations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetic")
	int currentIteration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetic")
	TArray<UChromosome*> chromosomes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Genetic")
	UChromosome* bestRoute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance")
	int bestDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool isRunningDjisktra;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool djisktraFound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool isRunningGenetic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool isRunningAntColony;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool isRunningAStar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool AStarFound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool geneticFound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booleans")
	bool antColonyFound;

	/** Ant Colony */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ant Colony")
	float evaporation;

	TArray<double> probabilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ant Colony")
	TArray<int> pathID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ant Colony")
	TArray<int> bestPathID;

	float nevner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ant Colony")
	int bestAntRoute;


private:


	APathfindNode* currentNode;
	TArray<APathfindNode*> tempArray;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RunDjikstra();

	void RunAStar();

	void RunGeneticAlgorithm();

	void GenerateRandomChromosomes();

	void CreateFourRandomChromosomes();

	void GetBestChromosomes();

	UFUNCTION(BlueprintCallable)
	void StopGenetic();

	void DisplayFastest();

	void RunAntColony();

	void ConnectStart();

	UChromosome* CreateOffspring(UChromosome* parent1, UChromosome* parent2);


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintCallable)
	void Djisktra();

	UFUNCTION(BlueprintCallable)
	void SpawnNodes();

	UFUNCTION(BlueprintCallable)
	void SpawnTSPNodes();

	UFUNCTION(BlueprintCallable)
	void DeleteNodes();

	UFUNCTION(BlueprintCallable)
	void AStar();

	UFUNCTION(BlueprintCallable)
	void GeneticAlgorithm();

	UFUNCTION(BlueprintCallable)
	void AntColony();
	

	UFUNCTION(BlueprintCallable)
	void NearestInsertion();

	void RunNearestinsertion();

	void ResetBooleans();

	TArray<APathfindNode*> GetMutatedRoute(UChromosome* parent);

};
