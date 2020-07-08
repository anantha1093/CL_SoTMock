// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// -----------------------------------------------------------------------------

// Sets default values
AFoodItem::AFoodItem()
{
	FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoodMesh"));
	FoodName = NAME_None;
	RootComponent = FoodMesh;
	TimeTakenToCook = 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------