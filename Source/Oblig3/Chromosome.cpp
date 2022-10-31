// Fill out your copyright notice in the Description page of Project Settings.


#include "Chromosome.h"
#include "PathfindNode.h"

void UChromosome::CalculatePath()
{
	float dist{};

	for (int i{}; i < route.Num() - 1; i++)
	{
		if (i == 0)
		{
			dist += (route[route.Num()-1]->GetActorLocation() - route[0]->GetActorLocation()).Length() / 50.f;
			dist += (route[i]->GetActorLocation() - route[i + 1]->GetActorLocation()).Length() / 50.f;
		}
		else
		{
			dist += (route[i]->GetActorLocation() - route[i + 1]->GetActorLocation()).Length() / 50.f;
		}
		distance = dist;
	}


}
