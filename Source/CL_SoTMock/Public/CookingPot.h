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

//USTRUCT(BlueprintType, Blueprintable)
//struct FFoodTableMember : public FTableRowBase
//{
//	GENERATED_BODY()
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Food)
//	FName FoodName;
//	
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Food)
//	int32 TimeNeededToPrepare;
//
//	// Default constructor
//	FFoodTableMember()
//	{
//		FoodName = NAME_None;
//		TimeNeededToPrepare = 0;
//	}
//
//	// Parameterised constructor
//	FFoodTableMember(FName InFoodName, int32 InTimeNeededToPrepare) :
//		FoodName(InFoodName),
//		TimeNeededToPrepare(InTimeNeededToPrepare)
//	{}
//};

// -----------------------------------------------------------------------------

UCLASS()
class CL_SOTMOCK_API ACookingPot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACookingPot();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	//// Used to keep track of present recipes and current cooking time
	//FFoodTableMember CurrentlyCookingFood;

	// How many seconds have passed since cooking has started
	int32 CurrentCookingTime;

	// The food that's cooking now
	AFoodItem* FoodCurrentlyOnPot;

	bool bHasFoodFinishedCooking;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartBurningFood();

};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------