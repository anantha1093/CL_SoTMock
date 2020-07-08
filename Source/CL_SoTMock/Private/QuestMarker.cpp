// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestMarker.h"
#include "Components/BoxComponent.h"

// -----------------------------------------------------------------------------

// Sets default values
AQuestMarker::AQuestMarker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	QuestCompletionBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("QuestCompletionBounds"));
}

// -----------------------------------------------------------------------------

// Called when the game starts or when spawned
void AQuestMarker::BeginPlay()
{
	Super::BeginPlay();
}

// -----------------------------------------------------------------------------

// Called every frame
void AQuestMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// -----------------------------------------------------------------------------

FQuestDetails AQuestMarker::FindNextQuest_Implementation(const FName CurrentQuest, const UDataTable* QuestTable)
{
	// Making sure that the table exists. Can't be too careful, eh?
	if (QuestTable)
	{
		if (FQuestDetails* CurrentQuestDetails = QuestTable->FindRow<FQuestDetails>(CurrentQuest, QuestContextString::ContextString, true))
		{
			// Ensure that this is the quest we want to send the player on before proceeding further with this.
			if (CurrentQuestDetails->CurrentQuestName() == MarkerQuestDetails.CurrentQuestName())
			{
				// Let's use the function for orthogonality!
				FName NewQuest = CurrentQuestDetails->NextQuestBranch();
				CurrentQuestDetails = QuestTable->FindRow<FQuestDetails>(NewQuest, QuestContextString::ContextString, true);
				// This is the safest case.
				if (CurrentQuestDetails)
				{
					if (NewQuest != NAME_None)
					{
						UE_LOG(LogTemp, Log, TEXT("Successfully found next quest!"));
						return *(CurrentQuestDetails);
					}
					else
					{
						UE_LOG(LogTemp, Log, TEXT("The quest is correct but there's no new quest data, or the row doesn't exist!"));
						return FQuestDetails::FQuestDetails();
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("This quest isn't the one we want the player to go on!"));
				return FQuestDetails::FQuestDetails();
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("No such row in table!"));
			return FQuestDetails::FQuestDetails();
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Table invalid!"));
		// return nullptr;
		return FQuestDetails::FQuestDetails();
	}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------