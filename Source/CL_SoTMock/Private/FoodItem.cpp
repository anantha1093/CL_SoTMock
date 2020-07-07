// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// -----------------------------------------------------------------------------

// Sets default values
AFoodItem::AFoodItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoodMesh"));
	FoodName = NAME_None;
	RootComponent = FoodMesh;
	TimeTakenToCook = 0;
}

// -----------------------------------------------------------------------------

// Called when the game starts or when spawned
void AFoodItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// -----------------------------------------------------------------------------

// Called every frame
void AFoodItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------