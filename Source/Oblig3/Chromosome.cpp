// Fill out your copyright notice in the Description page of Project Settings.


#include "Chromosome.h"
#include "PathfindNode.h"

void UChromosome::CalculatePath()
{
	int dist{};

	for (int i{}; i < route.Num() - 1; i++)
	{
		if (i == 0)
		{
			dist += route[route.Num()-1]->GetDistanceTo(route[i]) / 50.f;
		}
		else
		{
			dist += route[i]->GetDistanceTo(route[i + 1]) / 50.f;
		}
		distance = dist;
	}


}
