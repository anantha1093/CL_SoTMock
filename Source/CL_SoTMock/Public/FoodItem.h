// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoodItem.generated.h"

// -----------------------------------------------------------------------------

class USphereComponent;
class UStaticMeshComponent;

// -----------------------------------------------------------------------------

UCLASS()
class CL_SOTMOCK_API AFoodItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoodItem();

	// To tell the pot and the player how long this takes to cook
	int32 GetTimeTakenToCook() const { return TimeTakenToCook; }

protected:

	UPROPERTY(EditDefaultsOnly)
	FName FoodName;

	// Visual indicator of food to player
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* FoodMesh;

	UPROPERTY(EditDefaultsOnly)
	int32 TimeTakenToCook;

};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------