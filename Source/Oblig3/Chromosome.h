// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Chromosome.generated.h"


class APathfindNode;
/**
 * 
 */
UCLASS()
class OBLIG3_API UChromosome : public UObject
{
	GENERATED_BODY()

	
public:

	TArray<APathfindNode*> route;

	int distance;

	void CalculatePath();
	
};
