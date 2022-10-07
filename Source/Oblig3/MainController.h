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

private:
	bool isRunningDjisktra;
	bool djisktraFound;

	APathfindNode* currentNode;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnNodes();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintCallable)
	void Djisktra();

};
