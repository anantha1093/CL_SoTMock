// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/DataTable.h"
#include "QuestInteraction.generated.h"

// -----------------------------------------------------------------------------

class AQuestMarker;

// -----------------------------------------------------------------------------

/**
* This structure has information about each quest and where the quest branches to.
* It will be the baseline for the data table.
*/

USTRUCT(BlueprintType, Blueprintable)
struct FQuestDetails : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	// Default constructor
	FQuestDetails()
	{
		QuestName = TEXT("None");
		QuestDescription = TEXT("SomeText");		
	}

	// Parameterised constructor
	FQuestDetails(FName QName, FString QDesc, TArray<FName> NextQs, TArray<FName> PrevQs)
	{
		QuestName = QName;
		QuestDescription = QDesc;
		NextQuests = NextQs;
		PreviousQuests = PrevQs;
	}

	// Used instead of directly accessing the variable to maintain orthogonality
	//UFUNCTION(BlueprintCallable, Category = Quest)
	bool IsQuestBranched()
	{
		if (NextQuests.Num() < 1) return false;
		else return true;
	}

	// Simple way to move to the next branch of quest
	//UFUNCTION(BlueprintCallable, Category = Quest)
	FName NextQuestBranch()
	{
		int32 RandomBranch = FMath::RandRange(0, (NextQuests.Num() - 1));
		return NextQuests[RandomBranch];
	}

	FName CurrentQuestName()
	{
		return QuestName;
	}

protected:

	// Used as a unique identifier of the quest in the table
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Quest)
	FName QuestName;

	// The info that goes to the UI to be displayed to the player
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Quest)
	FString QuestDescription;

	// Array has more than one element if it's a branched quest. One of them is randomly chosen when branching happens
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Quest)
	TArray<FName> NextQuests;

	// Used to make sure the quest order is maintained. Multiple in case this is a convergence of branches
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Quest)
	TArray<FName> PreviousQuests;
};

// -----------------------------------------------------------------------------

/**
 * This interface is so that objects can talk to the player character and vice versa.
 * There's a function for finding the next quest in there.
 */

UINTERFACE(BlueprintType, Blueprintable)
class UQuestInteractionInterface : public UInterface
{
	GENERATED_BODY()	
};

// -----------------------------------------------------------------------------

class IQuestInteractionInterface
{
	GENERATED_BODY()

	public:

	// Use this function anywhere to get the next quest - ensure you pass the data table through though!
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Quest)
	FQuestDetails FindNextQuest(FName CurrentQuest, UDataTable* QuestTable);
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------