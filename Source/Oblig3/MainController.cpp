// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "PathfindNode.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AMainController::AMainController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root
	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(boxComponent);


	// Create Spring Arm and attach it to the Root(CapsuleComponent)
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 3000.f; // Distance from player
	springArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	springArm->bDoCollisionTest = false;

	// Childs
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);


	AutoPossessPlayer = EAutoReceiveInput::Player0;



	// Variables
	isRunningDjisktra = false;
	NodeAmount = 1;

	minPos = -1000.f;
	maxPos = 1000.f;
	djisktraFound = false;

	

}

// Called when the game starts or when spawned
void AMainController::BeginPlay()
{
	Super::BeginPlay();

	SpawnNodes();
	
}

void AMainController::SpawnNodes()
{

	if (nodes.Num() > 0)
	{
		for (int i = NodeAmount - 1; i < 0; i--)
		{
			nodes[i]->Destroy();
		}
	}

	nodes.Empty();
	world = GetWorld();

	if (world != nullptr)
	{

		// Spawn in each node at a random position
		float x;
		float y;
		float z;

		// Spawning and Set all positions
		for (int i{}; i < NodeAmount; i++)
		{
			x = FMath::FRandRange(minPos, maxPos);
			y = FMath::FRandRange(minPos, maxPos);
			z = FMath::FRandRange(minPos, maxPos);

			APathfindNode* newNode = world->SpawnActor<APathfindNode>(pathfindNodeBP, FVector(x, y, z), FRotator(0, 0, 0));
			newNode->SetID(i);
			newNode->SetupEnum(x, y, z);
			nodes.Add(newNode);

			
		}
		// Connect nodes that are in each sector;
		for (int i{}; i < NodeAmount; i++)
		{
			nodes[i]->SetConnection(nodes);
		}
		for (int i{}; i < NodeAmount; i++)
		{
			nodes[i]->ConnectSectors(nodes);
		}

		
	}
}

// Called every frame
void AMainController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotation(GetActorRotation() + FRotator(0, 0.1f, 0));



	if (isRunningDjisktra)
	{
		int lowestIndex = 0;
		int lowestAmount = INT_MAX;
		// Find the closes vertex to start
		for (int j{}; j < NodeAmount; j++)
		{
			if (!nodes[j]->isFound && nodes[j]->totalCost < lowestAmount)
			{
				lowestIndex = j;
				lowestAmount = nodes[j]->totalCost;
			}
		}
		currentNode = nodes[lowestIndex];


		// Check children that are nor already found
		for (int j{}; j < currentNode->connectionAmount; j++)
		{
			if (currentNode->connections[j]->GetTotalCost(currentNode) < currentNode->connections[j]->totalCost && !currentNode->connections[j]->isFound)
			{
				APathfindNode* checkingNode = currentNode->connections[j];

				if (checkingNode->GetTotalCost(currentNode) < checkingNode->totalCost)
				{
					int newCost = checkingNode->GetTotalCost(currentNode) + currentNode->totalCost;
					checkingNode->SetTotalCost(newCost);
					checkingNode->previousNode = currentNode;
				}
			}

		}
		currentNode->isFound = true;




		isRunningDjisktra = false;
		djisktraFound = true;
		for (int i{}; i < NodeAmount; i++)
		{
			if (!nodes[i]->isFound)
			{
				isRunningDjisktra = true;
				djisktraFound = false;
			}
		}
	}

	if (djisktraFound)
	{
		currentNode = nodes[end];
		currentNode->markedAsFastestRoute = true;
		bool fullRoute = false;
		while (!fullRoute)
		{

			if (currentNode->previousNode == nodes[start])
			{
				fullRoute = true;
			}
			else
			{
				currentNode = currentNode->previousNode;
				currentNode->markedAsFastestRoute = true;
			}
		}
		djisktraFound = false;
	}


}

// Called to bind functionality to input
void AMainController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainController::Djisktra()
{


	#pragma region Initialize

		start = 5;
		// Random Start and End
		end = FMath::RandRange(0, NodeAmount - 1);
		while (start == end)
		{
			end = FMath::RandRange(0, NodeAmount - 1);
		}

		// initialize all values 
		for (int i{}; i < NodeAmount; i++)
		{
			nodes[i]->totalCost = INT_MAX;
			nodes[i]->isFound = false;
			nodes[i]->previousNode = nullptr;
		}

		nodes[start]->totalCost = 0;
		nodes[start]->mesh->SetMaterial(0, startMaterial);
		nodes[end]->mesh->SetMaterial(0, endMaterial);



		currentNode = nodes[start];

		// Do first checks for children of start
		for (int i{}; i < currentNode->connectionAmount; i++)
		{
			if (currentNode->connections[i]->GetTotalCost(currentNode) < currentNode->connections[i]->totalCost)
			{
				APathfindNode* checkingNode = currentNode->connections[i];

				if (checkingNode->GetTotalCost(currentNode) < checkingNode->totalCost)
				{
					int newCost = checkingNode->GetTotalCost(currentNode) + currentNode->totalCost;
					checkingNode->SetTotalCost(newCost);
					checkingNode->previousNode = currentNode;
				}
			}
			
		}
		currentNode->isFound = true;

	#pragma endregion

		isRunningDjisktra = true;

}

