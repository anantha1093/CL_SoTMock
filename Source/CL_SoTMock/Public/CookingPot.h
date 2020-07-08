// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CookingPot.generated.h"

// -----------------------------------------------------------------------------

class UBoxComponent;
class UStaticMeshComponent;
class AFoodItem;

// -----------------------------------------------------------------------------

UCLASS()
class CL_SOTMOCK_API ACookingPot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACookingPot();

	UFUNCTION()
	void StartCooking(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	void TickCooking();

	void EndCooking();

	UFUNCTION()
	void LetPlayerStopCooking(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:

	// Used to detect when a food item is within reach of the pot
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* CookingAreaBox;

	// Visual indicator for the player
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CookingPotMesh;

	// Used to tick and keep track of cooking
	FTimerHandle TimerHandle_Cooking;

	// How many seconds have passed since cooking has started
	int32 CurrentCookingTime;

	// The food that's cooking now
	AFoodItem* FoodCurrentlyOnPot;

	bool bHasFoodFinishedCooking;

	UFUNCTION(BlueprintImplementableEvent)
	void StartBurningFood();

};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------