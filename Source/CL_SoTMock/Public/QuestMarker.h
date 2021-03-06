// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestInteraction.h"
#include "Engine/DataTable.h"
#include "QuestMarker.generated.h"

// -----------------------------------------------------------------------------

class UBoxComponent;

// -----------------------------------------------------------------------------

UCLASS()
class CL_SOTMOCK_API AQuestMarker : public AActor, public IQuestInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestMarker();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Quest)
	FQuestDetails FindNextQuest(FName CurrentQuest, UDataTable* QuestTable);
	virtual FQuestDetails FindNextQuest_Implementation(const FName CurrentQuest, const UDataTable* QuestTable) override;

protected:
	
	// Complete quest upon overlapping this
	UPROPERTY(EditDefaultsOnly, Category = Quest)
	UBoxComponent* QuestCompletionBounds;
	
	// So that we know what quest to complete and then send the player on
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = Quest)
	FQuestDetails MarkerQuestDetails;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------