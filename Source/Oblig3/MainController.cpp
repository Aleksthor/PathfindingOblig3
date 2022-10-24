// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "PathfindNode.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Chromosome.h"

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

	population = 100;

	// Variables
	isRunningDjisktra = false;
	NodeAmount = 1;

	minPos = -1000.f;
	maxPos = 1000.f;
	djisktraFound = false;

	isRunningAStar = false;
	AStarFound = false;
	isRunningGenetic = false;
	geneticFound = false;
	isRunningAntColony = false;
	antColonyFound = false;
	isRunningNearestInsertion = false;
	nearestInsertionFound = false;
}

// Called when the game starts or when spawned
void AMainController::BeginPlay()
{
	Super::BeginPlay();
	SpawnNodes();
}




// Called every frame
void AMainController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRotation(GetActorRotation() + FRotator(0, 0.1f, 0));



	RunDjikstra();
	RunAStar();
	RunGeneticAlgorithm();
	RunAntColony();
	RunNearestinsertion();

}

// Called to bind functionality to input
void AMainController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AMainController::SpawnNodes()
{
	if (nodes.Num() > 0)
	{
		return;
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

void AMainController::SpawnTSPNodes()
{
	if (nodes.Num() > 0)
	{
		return;
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
			newNode->displayLines = false;
			nodes.Add(newNode);


		}
		// Connect to all nodes except yourself
		for (int i{}; i < NodeAmount; i++)
		{
			nodes[i]->ConnectToAll(nodes);
		}


	}
}

// Delete all Nodes
void AMainController::DeleteNodes()
{
	if (nodes.Num() > 0)
	{
		for (int i = nodes.Num() - 1; i > -1; i--)
		{
			nodes[i]->Kill();
		}
		nodes.Empty();
		djisktraFound = false;
		isRunningDjisktra = false;
		AStarFound = false;
		isRunningAStar = false;
	}
	ResetBooleans();
}

// Initialize Djikstra
void AMainController::Djisktra()
{

	#pragma region Initialize

		start = FMath::RandRange(0, NodeAmount - 1);
		// Random Start and End
		end = FMath::RandRange(0, NodeAmount - 1);

		// reset booleans
		isRunningAStar = false;
		AStarFound = false;
		isRunningDjisktra = false;
		djisktraFound = false;

		while (start == end)
		{
			end = FMath::RandRange(0, NodeAmount - 1);
		}

		// Initialize all values 
		for (int i{}; i < NodeAmount; i++)
		{
			nodes[i]->totalCost = INT_MAX;
			nodes[i]->isFound = false;
			nodes[i]->previousNode = nullptr;
			nodes[i]->markedAsFastestRoute = false;
			nodes[i]->ResetMaterial();
		}

		// Initialize Start
		nodes[start]->totalCost = 0;
		nodes[start]->mesh->SetMaterial(0, startMaterial);
		nodes[end]->mesh->SetMaterial(0, endMaterial);
		currentNode = nodes[start];

		// Check all children of currentNode
		for (int i{}; i < currentNode->connectionAmount; i++)
		{
			APathfindNode* checkingNode = currentNode->connections[i];

			if (checkingNode->GetTotalCost(currentNode) < checkingNode->totalCost)
			{			
				
				int newCost = checkingNode->GetTotalCost(currentNode) + currentNode->totalCost;
				checkingNode->SetTotalCost(newCost);
				checkingNode->previousNode = currentNode;
				
			}
			
		}
		currentNode->isFound = true;

	#pragma endregion

		isRunningDjisktra = true;

}

void AMainController::RunDjikstra()
{
	if (isRunningDjisktra)
	{

		// What current node are we changing to?
		int lowestIndex = 0;
		int lowestAmount = INT_MAX;
		// Find the closest vertex to start
		for (int j{}; j < NodeAmount; j++)
		{
			if (!nodes[j]->isFound && nodes[j]->totalCost < lowestAmount)
			{
				lowestIndex = j;
				lowestAmount = nodes[j]->totalCost;
			}
		}
		currentNode = nodes[lowestIndex];


		// Check all children of currentNode
		for (int j{}; j < currentNode->connectionAmount; j++)
		{
			APathfindNode* checkingNode = currentNode->connections[j];
			if (checkingNode->GetTotalCost(currentNode) + currentNode->totalCost < checkingNode->totalCost && !checkingNode->isFound)
			{		
				int newCost = checkingNode->GetTotalCost(currentNode) + currentNode->totalCost;
				checkingNode->SetTotalCost(newCost);
				checkingNode->previousNode = currentNode;				
			}

		}
		currentNode->isFound = true;



		// Check if we are done
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

	// Tell nodes to Display according to the results of the Algorithm
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



// Initialize A* Algorithm
void AMainController::AStar()
{
	

	start = FMath::RandRange(0, NodeAmount - 1);
	// Random Start and End
	end = FMath::RandRange(0, NodeAmount - 1);

	// Reset Booleans
	ResetBooleans();

	while (start == end)
	{
		end = FMath::RandRange(0, NodeAmount - 1);
	}

	// initialize all values 
	for (int i{}; i < NodeAmount; i++)
	{
		nodes[i]->totalCost = INT_MAX;
		nodes[i]->costToFinish = INT_MAX;
		nodes[i]->isFound = false;
		nodes[i]->previousNode = nullptr;
		nodes[i]->markedAsFastestRoute = false;
		nodes[i]->ResetMaterial();
	}

	// inistialize Start
	nodes[start]->totalCost = 0;
	nodes[start]->mesh->SetMaterial(0, startMaterial);
	nodes[end]->mesh->SetMaterial(0, endMaterial);
	currentNode = nodes[start];

	// Check all children of currentNode
	for (int i{}; i < currentNode->connectionAmount; i++)
	{
		APathfindNode* checkingNode = currentNode->connections[i];

		if (checkingNode->GetTotalCost(currentNode) + currentNode->totalCost + checkingNode->GetDistanceToFinish(nodes[end]) < checkingNode->totalCost)
		{
			int movementCost = checkingNode->GetTotalCost(currentNode) + currentNode->totalCost;
			int costToFinsih = checkingNode->GetDistanceToFinish(nodes[end]);

			checkingNode->SetTotalCost(movementCost + costToFinsih);
			checkingNode->previousNode = currentNode;

		}

	}
	currentNode->isFound = true;


	isRunningAStar = true;

}

void AMainController::RunAStar()
{
	if (isRunningAStar)
	{
		// What current node are we changing to?
		int lowestIndex = 0;
		int lowestAmount = INT_MAX;
		// Find the closes vertex to start
		for (int j{}; j < NodeAmount; j++)
		{
			if (nodes[j]->totalCost < lowestAmount && !nodes[j]->isFound)
			{
				lowestIndex = j;
				lowestAmount = nodes[j]->totalCost;
			}
			
		}
		currentNode = nodes[lowestIndex];


		// Check all children of currentNode
		for (int i{}; i < currentNode->connectionAmount; i++)
		{
			APathfindNode* checkingNode = currentNode->connections[i];

			if (checkingNode->GetTotalCost(currentNode) + currentNode->totalCost + checkingNode->GetDistanceToFinish(nodes[end]) < checkingNode->totalCost)
			{
				int movementCost = checkingNode->GetTotalCost(currentNode) + currentNode->totalCost;
				int costToFinsih = checkingNode->GetDistanceToFinish(nodes[end]);

				checkingNode->SetTotalCost(movementCost + costToFinsih);
				checkingNode->previousNode = currentNode;
;
			}

		}
		currentNode->isFound = true;


		// Check all children of currentNode
		isRunningAStar = false;
		AStarFound = true;
		for (int i{}; i < NodeAmount; i++)
		{
			if (!nodes[i]->isFound)
			{
				if (nodes[end]->totalCost > nodes[i]->totalCost)
				{
					isRunningAStar = true;
					AStarFound = false;
				}
			}

		}


		// Update nodes to display the results
		if (AStarFound)
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
			AStarFound = false;
		}


	}



}

// reset the functions
void AMainController::ResetBooleans()
{
	isRunningAStar = false;
	AStarFound = false;
	isRunningDjisktra = false;
	djisktraFound = false;
	isRunningGenetic = false;
	geneticFound = false;
	isRunningAntColony = false;
	antColonyFound = false;
	isRunningNearestInsertion = false;
	nearestInsertionFound = false;
}

// Initialize Genetic Algorithm
void AMainController::GeneticAlgorithm()
{

	ResetBooleans();

	chromosomes.Empty();
	currentIteration = 0;
	isRunningGenetic = true;
	bestDistance = INT_MAX;

	// Generate half of max population
	GenerateRandomChromosomes();
	
	
}



void AMainController::RunGeneticAlgorithm()
{
	if (isRunningGenetic == true)
	{
		if (currentIteration < iterations)
		{

			// Make 50% of the new population from offsprings
			for (int i{}; i < population - 1; i = i + 2)
			{
				if (chromosomes.Num() > i + 1)
				{
					UChromosome* parent1 = chromosomes[i];
					UChromosome* parent2 = chromosomes[i + 1];
					UChromosome* offspring = CreateOffspring(parent1, parent2);
					offspring->CalculatePath();
					chromosomes.Add(offspring);

				}

			}


			// Make 25% of the new population from offsprings
			for (int i{}; i < population - 1; i = i + 4)
			{
				if (chromosomes.Num() > i + 1)
				{
					UChromosome* parent1 = chromosomes[i];
						UChromosome* parent2 = chromosomes[i + 1];
						UChromosome* offspring = CreateOffspring(parent1, parent2);
						offspring->CalculatePath();
						chromosomes.Add(offspring);
				}
			}

			// Generate 20 % of the new poplation from Mutations
			for (int i{}; i < population - 1; i  = i + 5)
			{
				if (chromosomes.Num() > i + 1)
				{
					UChromosome* parent = chromosomes[FMath::RandRange(0, chromosomes.Num() - 1)];
						UChromosome* mutated = NewObject<UChromosome>();
						mutated->route = GetMutatedRoute(parent);
						mutated->CalculatePath();
						chromosomes.Add(mutated);
				}
			}

			// Generate 5 % random Chromosomes to the population
			Create5PercentNewChromosomes();
			
			// Remove the worst half of the poplation
			GetBestChromosomes();

			// Display the currently fastest route
			DisplayFastest();
			currentIteration++;
		}
		else
		{
			isRunningGenetic = false;
			geneticFound = true;
			DisplayFastest();
		}

	}
	// Display the best route found in the search
	if (geneticFound)
	{
		DisplayFastest();
	}

}


void AMainController::GenerateRandomChromosomes()
{
	// Generate half of max population
	for (int i{}; i < population / 2; i++)
	{
		
		TArray<APathfindNode*> newRoute;

		// Add a random value to all nodes
		for (int j{}; j < NodeAmount; j++)
		{
			nodes[j]->randomValue = FMath::RandRange(0, 10);
			newRoute.Add(nodes[j]);
			
		}

		// Sort Array based on random value 
		for (int k{}; k < newRoute.Num(); k++)
		{
			int minIndex = k;
			for (int j{}; j < newRoute.Num(); j++)
			{
				if (newRoute[j]->randomValue < newRoute[minIndex]->randomValue)
				{
					minIndex = j;
				}

				newRoute.SwapMemory(minIndex, k);
			}
		}

		// Instantiate a chromosome and add the path to it
		UChromosome* chromosome = NewObject<UChromosome>();
		chromosome->route = newRoute;
		chromosome->CalculatePath();
		chromosomes.Add(chromosome);

		if (i == 0)
		{
			bestRoute = chromosome;
			bestDistance = chromosome->distance;
		}
		if (chromosomes[i]->distance < bestDistance)
		{
			bestRoute = chromosome;
			bestDistance = chromosome->distance;
		}
	}


}


void AMainController::Create5PercentNewChromosomes()
{

	for (int i{}; i < population; i = i + 20)
	{
		TArray<APathfindNode*> newRoute;
		// Generate a random value for all nodes
		for (int j{}; j < NodeAmount; j++)
		{
			nodes[j]->randomValue = FMath::RandRange(0, 10);
			newRoute.Add(nodes[j]);

		}

		// Sort Array based on this random value
		for (int k{}; k < newRoute.Num(); k++)
		{
			int minIndex = k;
			for (int j{}; j < newRoute.Num(); j++)
			{
				if (newRoute[j]->randomValue < newRoute[minIndex]->randomValue)
				{
					minIndex = j;
				}

				newRoute.SwapMemory(minIndex, k);
			}
		}

		// Instantiate this new Chromosome
		UChromosome* chromosome = NewObject<UChromosome>();
		chromosome->route = newRoute;
		chromosome->CalculatePath();
		chromosomes.Add(chromosome);
	}

}

UChromosome* AMainController::CreateOffspring(UChromosome* parent1, UChromosome* parent2)
{
	// the object we will return gets created
	UChromosome* offspring = NewObject<UChromosome>();

	// Save array in local variable so we dont access pointer too much
	TArray<APathfindNode*> parentArray = parent1->route;

	// Start pos is where we will "split" the chromosome
	int startPos = FMath::RandRange(0, NodeAmount / 2 - 1);


	TArray<APathfindNode*> tempNodes;

	// Add the genomes from the chromosome to a tempArray
	int size = FMath::RandRange(NodeAmount / 5, NodeAmount / 2);
	for (int i{}; i < size; i++)
	{
		tempNodes.Add(parentArray[startPos + i]);
	}
	

	// Save array of parent 2 in local variable to we dont access pointer too much
	parentArray = parent2->route;

	// Shuffle the genomes from parent1 into parent2's chromosome
	for (int i{}; i < tempNodes.Num(); i++)
	{
		parentArray.Remove(tempNodes[i]);
	}
	for (int i{}; i < tempNodes.Num(); i++)
	{
		parentArray.Add(tempNodes[i]);
	}

	// return the offsprint chromosome
	offspring->route = parentArray;
	return offspring;
}

TArray<APathfindNode*> AMainController::GetMutatedRoute(UChromosome* parent)
{
	TArray<APathfindNode*> temp = parent->route;

	int startPos = FMath::RandRange(0, NodeAmount / 2);

	for (int i = startPos; i < (NodeAmount / 2) + startPos - 1; i++)
	{
		int a = FMath::RandRange(startPos, (NodeAmount / 2) + startPos - 1);
		if (i != a)
		{
			temp.Swap(a, i);
		}

	}
	return temp;
}

void AMainController::GetBestChromosomes()
{


	for (int k{}; k < chromosomes.Num(); k++)
	{
		int minIndex = k;
		for (int j{}; j < chromosomes.Num(); j++)
		{

			if (bestRoute->distance > chromosomes[j]->distance)
			{
				bestRoute = chromosomes[j];
				bestDistance = chromosomes[j]->distance;
			}

			if (chromosomes[j]->distance < chromosomes[minIndex]->distance)
			{

				minIndex = j;
			}

			chromosomes.SwapMemory(minIndex, k);
		}
	}

	for (int i = (chromosomes.Num() / 2) - 1; i < chromosomes.Num(); i++)
	{
		chromosomes.RemoveAt(i);
	}


	
}

void AMainController::StopGenetic()
{
	ResetBooleans();
	geneticFound = true;

}

void AMainController::DisplayFastest()
{
	for (int i{}; i < NodeAmount - 1; i++)
	{
		
		if (i == 0)
		{

			UKismetSystemLibrary::DrawDebugArrow
				(world,																// World
				bestRoute->route[NodeAmount - 1]->GetActorLocation(),				// Start
				bestRoute->route[i]->GetActorLocation(),							// End
				20.f,																// ArrowSize
				FColor::White,														// Color
				0.f,																// Duration
				2.5f);																// Thickness

			UKismetSystemLibrary::DrawDebugArrow
			(world,																// World
				bestRoute->route[i]->GetActorLocation(),				// Start
				bestRoute->route[i + 1]->GetActorLocation(),							// End
				20.f,																// ArrowSize
				FColor::White,														// Color
				0.f,																// Duration
				2.5f);																// Thickness
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%d"), bestRoute->route[i]->ID);

			UKismetSystemLibrary::DrawDebugArrow
			(world,																	// World
				bestRoute->route[i]->GetActorLocation(),							// Start
				bestRoute->route[i + 1]->GetActorLocation(),						// End
				20.f,																// ArrowSize
				FColor::White,														// Color
				0.f,																// Duration
				2.5f);																// Thickness
		}

	}

}



// Ant Colony
void AMainController::AntColony()
{
	if (!isRunningAntColony)
	{
		ResetBooleans();
		currentIteration = 0;
		isRunningAntColony = true;

		bestAntRoute = INT_MAX;
		for (int i{}; i < NodeAmount; i++)
		{
			nodes[i]->InitPhermones();
			nodes[i]->isFound = false;
		}
		currentNode = nodes[0];
		currentNode->isFound = true;

	}
}


void AMainController::RunAntColony()
{
	if (isRunningAntColony && nodes.Num() > 0)
	{
		if (currentIteration < iterations)
		{
			if (currentNode != nullptr)
			{
				pathID.Empty();
				for (int j{}; j < NodeAmount + 1; j++)
				{
					pathID.Add(currentNode->ID);
					probabilities.Empty();
					/** Function Probablitites */
					for (int i{}; i < currentNode->connectionAmount; i++)
					{
						if (!currentNode->connections[i]->isFound)
						{
							nevner += (currentNode->phermones[i] * (1 / currentNode->movementCost[i]));
						}
					}
					tempArray.Empty();
					double highestProbablity = 0;
					for (int i{}; i < currentNode->connectionAmount; i++)
					{
						if (!currentNode->connections[i]->isFound)
						{
							double newProbablity = (currentNode->phermones[i] * (1 - evaporation)) + (1 / currentNode->movementCost[i]);
							if (highestProbablity < newProbablity)
							{
								highestProbablity = newProbablity;
							}
							probabilities.Add(newProbablity);
							tempArray.Add(currentNode->connections[i]);
						}
					}

					/** Update Probablities */
					double factor = 1 / highestProbablity;
					for (int i{}; i < probabilities.Num(); i++)
					{
						probabilities[i] *= factor;
					}

					/** Choose a path */
					

					// Choose from array
					int r = FMath::RandRange(0, 100);
					double random = r / 100.f;
					int winner{};
					for (int i{}; i < probabilities.Num(); i++)
					{
						if (probabilities[i] > random)
						{
							winner = i;
							random = probabilities[i];
						}
					}
					UE_LOG(LogTemp, Warning, TEXT("%d"), currentNode->ID);
					currentNode->isFound = true;



					/** Wierd Logic I had to do for the end of the array*/

					if (tempArray.Num() == 0)
					{
						int c{};
						for (int i{}; i < NodeAmount; i++)
						{
							if (!nodes[i]->isFound)
							{
								c = i;
							}
						}
						if (c != 0)
						{
							UE_LOG(LogTemp, Warning, TEXT("Connecting to %d"), nodes[c]->ID);

							UKismetSystemLibrary::DrawDebugArrow
							(world,										// World
								currentNode->GetActorLocation(),		// Start
								nodes[c]->GetActorLocation(),			// End
								20,										// ArrowSize
								FColor::White,							// Color
								0.f,									// Duration
								5);										// Thickness

							currentNode->CalcPhermones(nodes[c]->ID, evaporation);
							nodes[c]->CalcPhermones(currentNode->ID, evaporation);
							currentNode = nodes[c];

						}
						else
						{
							ConnectStart();

							return;
						}
					}
					else
					{
						currentNode->CalcPhermones(tempArray[winner]->ID, evaporation);
						tempArray[winner]->CalcPhermones(currentNode->ID, evaporation);
						UKismetSystemLibrary::DrawDebugArrow
						(world,										// World
							currentNode->GetActorLocation(),		// Start
							tempArray[winner]->GetActorLocation(),			// End
							20,										// ArrowSize
							FColor::White,							// Color
							0.f,									// Duration
							5);										// Thickness
						currentNode = tempArray[winner];

					}
				}
			

			}
		}
		else if (currentIteration >= iterations)
		{
			for (int i{}; i < bestPathID.Num() - 1; i++)
			{
				UKismetSystemLibrary::DrawDebugArrow
					(world,												// World
					nodes[bestPathID[i]]->GetActorLocation(),			// Start
					nodes[bestPathID[i + 1]]->GetActorLocation(),		// End
					20,													// ArrowSize
					FColor::White,										// Color
					0.f,												// Duration
					5);													// Thickness
			}
			UKismetSystemLibrary::DrawDebugArrow
				(world,															// World
				nodes[bestPathID[bestPathID.Num() - 1]]->GetActorLocation(),	// Start
				nodes[bestPathID[0]]->GetActorLocation(),						// End
				20,																// ArrowSize
				FColor::White,													// Color
				0.f,															// Duration
				5);																// Thickness
			
		}

	}


}

void AMainController::ConnectStart()
{
	int s{};
	for (int i{}; i < currentNode->connectionAmount; i++)
	{
		if (currentNode->connections[i]->ID == nodes[0]->ID)
		{
			s = i;
		}
	}
	for (int i{}; i < NodeAmount; i++)
	{
		nodes[i]->isFound = false;
	}
	currentNode->phermones[s] = (currentNode->phermones[s] * (1 - evaporation)) + (1 / currentNode->movementCost[s]);


	float thisRoute{};

	for (int i{}; i < pathID.Num() - 1; i++)
	{
		thisRoute += (nodes[pathID[i]]->GetActorLocation() - nodes[pathID[i + 1]]->GetActorLocation()).Length() / 50.f;
	}
	thisRoute += (nodes[pathID[pathID.Num() - 1]]->GetActorLocation() - nodes[pathID[0]]->GetActorLocation()).Length() / 50.f;


	if (thisRoute < bestAntRoute)
	{
		bestPathID = pathID;
		bestAntRoute = thisRoute;
	}


	UKismetSystemLibrary::DrawDebugArrow
	(world,										// World
		currentNode->GetActorLocation(),		// Start
		nodes[0]->GetActorLocation(),			// End
		20,										// ArrowSize
		FColor::White,							// Color
		0.f,									// Duration
		5);										// Thickness


	currentNode = nodes[0];
	currentIteration++;

}

// Intialize Nearest Insertion
void AMainController::NearestInsertion()
{
	ResetBooleans();
	for (int i{}; i < NodeAmount; i++)
	{
		nodes[i]->nearestInsertionConnections.Empty();
	}
	currentNode = nodes[0];

	int nearest = currentNode->ConnectToNearestTwo(nodes);

	currentNode = nodes[nearest];

	isRunningNearestInsertion = true;
	currentIteration = 0;

}

void AMainController::RunNearestinsertion()
{
	if (isRunningNearestInsertion && currentIteration < maxIterationsAllowed)
	{
		currentIteration++;
		int nearest = currentNode->GetNearestOutsideGraph(nodes);
		currentNode = nodes[nearest];
		UE_LOG(LogTemp, Warning, TEXT("%d"), nearest);
		UE_LOG(LogTemp, Warning, TEXT("%d"), currentNode->ID);


		currentNode->ConnectToGraph(nodes);

		bool weDone = true;
		for (int i{}; i < NodeAmount; i++)
		{
			if (nodes[i]->nearestInsertionConnections.Num() == 0)
			{
				weDone = false;
			}
			nodes[i]->DisplayConnections();
		}

		if (weDone)
		{
			isRunningNearestInsertion = false;
			nearestInsertionFound = true;
		}
	}
	if (nearestInsertionFound || currentIteration == maxIterationsAllowed && nodes.Num() > 0)
	{
		for (int i{}; i < NodeAmount; i++)
		{
			nodes[i]->DisplayConnections();
		}
	}

	bestInsertionRoute = 0;
	if (nearestInsertionFound && nodes.Num() > 0)
	{
		for (int i{}; i < NodeAmount; i++)
		{
			if (previousNode != nodes[i]->nearestInsertionConnections[0])
			{
				bestInsertionRoute += (nodes[i]->GetActorLocation() - nodes[i]->nearestInsertionConnections[0]->GetActorLocation()).Length() / 50.f;
			}
			else
			{
				bestInsertionRoute += (nodes[i]->GetActorLocation() - nodes[i]->nearestInsertionConnections[1]->GetActorLocation()).Length() / 50.f;
			}
			previousNode = nodes[i];
		}
	}

}