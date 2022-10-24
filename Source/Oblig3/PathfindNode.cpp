// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindNode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"


// Sets default values
APathfindNode::APathfindNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(mesh);



	timeToFinish = FMath::RandRange(5,10);
	connectionAmount = 0;
	sectorConnected = false;
	isFound = false;
	markedAsFastestRoute = false;
}

// Called when the game starts or when spawned
void APathfindNode::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();
	
}

// Called every frame
void APathfindNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (displayLines)
	{
		DebugLine();
	}


	

}

void APathfindNode::SetupEnum(float x, float y, float z)
{


	#pragma region Set Sector

	// UP
	if (x > 0 && y > 0 && z > 0)
	{
		nodeSector = ENodeSector::UP_North_East;

	}
	if (x < 0 && y > 0 && z > 0)
	{
		nodeSector = ENodeSector::UP_South_East;
	}
	if (x < 0 && y < 0 && z > 0)
	{
		nodeSector = ENodeSector::UP_South_West;
	}
	if (x > 0 && y < 0 && z > 0)
	{
		nodeSector = ENodeSector::UP_North_West;
	}

	// DOWN
	if (x > 0 && y > 0 && z < 0)
	{
		nodeSector = ENodeSector::DOWN_North_East;
	}
	if (x < 0 && y > 0 && z < 0)
	{
		nodeSector = ENodeSector::DOWN_South_East;
	}
	if (x < 0 && y < 0 && z < 0)
	{
		nodeSector = ENodeSector::DOWN_South_West;
	}
	if (x > 0 && y < 0 && z < 0)
	{
		nodeSector = ENodeSector::DOWN_North_West;
	}



	#pragma endregion

}

void APathfindNode::SetConnection(TArray<APathfindNode*> fullArray)
{

	for (int i{}; i < fullArray.Num(); i++)
	{
		if (fullArray[i]->nodeSector == nodeSector)
		{
			if (fullArray[i]->ID != ID)
			{
				connections.Add(fullArray[i]);
				connectionAmount++;
			}
		}
	}

}

void APathfindNode::ConnectSectors(TArray<APathfindNode*> fullArray)
{


	if (ID != fullArray[0]->ID &&						// Not ourself
		!connections.Contains(fullArray[0]) &&			// Connections does not contain already
		!sectorConnected)								// Not Sector Connected
	{
		// Loop through all in the same sector
		for (int j{}; j < fullArray.Num(); j++)
		{
			if (fullArray[j]->nodeSector == nodeSector)
			{
				fullArray[j]->sectorConnected = true;
			}
		}
		connections.Add(fullArray[0]);
		connectionAmount++;

		fullArray[0]->NewConnection(this);

	}

	// Setup the lengths for each connection
	for (int i{}; i < connectionAmount; i++)
	{
		int newCost = (GetActorLocation() - connections[i]->GetActorLocation()).Length();
		movementCost.Add(newCost / 50);
	}


}

void APathfindNode::SetID(int i)
{
	ID = i;
}

int APathfindNode::GetID()
{
	return ID;
}



void APathfindNode::DebugLine()
{


	#pragma region DrawDebugLine
	if (connectionAmount > 0)
	{
		for (int i{}; i < connectionAmount; i++)
		{


			if (previousNode != nullptr)
			{
				if (connections[i]->ID == previousNode->ID)
				{
					if (!markedAsFastestRoute)
					{
						#pragma region Draw Red

						UKismetSystemLibrary::DrawDebugArrow
						(world,									// World
							GetActorLocation(),						// Start
							connections[i]->GetActorLocation(),		// End
							20.f,									// ArrowSize
							FColor::Red,							// Color
							0.f,									// Duration
							5.f);									// Thickness
#pragma endregion
					}
					else
					{
						#pragma region Draw Green

						UKismetSystemLibrary::DrawDebugArrow
						(world,									// World
							GetActorLocation(),						// Start
							connections[i]->GetActorLocation(),		// End
							20.f,									// ArrowSize
							FColor::Green,							// Color
							0.f,									// Duration
							5.f);									// Thickness
#pragma endregion
					}


				}
				else
				{
					if (connections[i]->previousNode != nullptr)
					{
						if (connections[i]->previousNode->ID != ID)
						{
							#pragma region Draw White
							UKismetSystemLibrary::DrawDebugArrow
							(world,									// World
								GetActorLocation(),						// Start
								connections[i]->GetActorLocation(),		// End
								20.f,									// ArrowSize
								FColor::White,							// Color
								0.f,									// Duration
								2.5f);									// Thickness

							#pragma endregion


						}

					}
					else
					{
						#pragma region Draw White
						UKismetSystemLibrary::DrawDebugArrow
						(world,									// World
							GetActorLocation(),						// Start
							connections[i]->GetActorLocation(),		// End
							20.f,									// ArrowSize
							FColor::White,							// Color
							0.f,									// Duration
							2.5f);									// Thickness

						#pragma endregion
					}
				}

			}
			else
			{
				if (connections[i]->previousNode != nullptr)
				{
					if (connections[i]->previousNode->ID != ID)
					{
						#pragma region Draw White
						UKismetSystemLibrary::DrawDebugArrow
						(world,									// World
							GetActorLocation(),						// Start
							connections[i]->GetActorLocation(),		// End
							20.f,									// ArrowSize
							FColor::White,							// Color
							0.f,									// Duration
							2.5f);									// Thickness

						#pragma endregion


					}

				}
				else
				{
					#pragma region Draw White
					UKismetSystemLibrary::DrawDebugArrow
					(world,									// World
						GetActorLocation(),						// Start
						connections[i]->GetActorLocation(),		// End
						20.f,									// ArrowSize
						FColor::White,							// Color
						0.f,									// Duration
						2.5f);									// Thickness

#pragma endregion
				}
			}

		}
	}

	#pragma endregion
}



int APathfindNode::GetTotalCost(APathfindNode* parent)
{
	for (int i{}; i < connectionAmount; i++)
	{
		if (connections[i]->ID == parent->ID)
		{
			return movementCost[i] + timeToFinish;
		}
	}
	return 0;
}

void APathfindNode::SetTotalCost(int input)
{
	totalCost = input;
}



void APathfindNode::NewConnection(APathfindNode* newConnection)
{
	connections.Add(newConnection);
	connectionAmount++;

	int newCost = (GetActorLocation() - connections[connectionAmount - 1]->GetActorLocation()).Length();
	movementCost.Add(newCost / 50.f);
}

void APathfindNode::Kill()
{
	this->Destroy();
}

void APathfindNode::ResetMaterial()
{
	mesh->SetMaterial(0, resetMaterial);
}

int APathfindNode::GetDistanceToFinish(APathfindNode* finish)
{
	return (GetActorLocation() - finish->GetActorLocation()).Length() / 50.f;
}

void APathfindNode::ConnectToAll(TArray<APathfindNode*> fullArray)
{
	for (int i{}; i < fullArray.Num(); i++)
	{
		if (fullArray[i]->ID != ID)
		{
			connections.Add(fullArray[i]);
			connectionAmount++;

			int newCost = (GetActorLocation() - connections[connectionAmount - 1]->GetActorLocation()).Length();
			movementCost.Add(newCost / 50.f);
		}
	}

}

void APathfindNode::InitPhermones()
{
	phermones.Empty();
	for (int i{}; i < connectionAmount; i++)
	{
		phermones.Add(1);
	}

}

void APathfindNode::CalcPhermones(int id, float evaporation)
{
	for (int i{}; i < connectionAmount; i++)
	{
		if (connections[i]->ID == id)
		{

			float cost = (float)movementCost[i];
			float add = (1 / cost);

			phermones[i] = (phermones[i] * (1 - evaporation)) + add;

		}
	}

}

void APathfindNode::DisplayAntLines()
{





}


