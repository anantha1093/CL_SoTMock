// Fill out your copyright notice in the Description page of Project Settings.


#include "CookingPot.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CL_SoTMock/CL_SoTMockCharacter.h"
#include "TimerManager.h"
#include "FoodItem.h"

// -----------------------------------------------------------------------------

// Sets default values
ACookingPot::ACookingPot()
{
	CookingAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CookingAreaBox"));
	CookingPotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CookingPotMesh"));
	RootComponent = CookingPotMesh;
	CookingAreaBox->SetupAttachment(RootComponent);
	CookingAreaBox->OnComponentBeginOverlap.AddDynamic(this, &ACookingPot::StartCooking);
	CookingAreaBox->OnComponentEndOverlap.AddDynamic(this, &ACookingPot::LetPlayerStopCooking);
	CurrentCookingTime = 0;
	bHasFoodFinishedCooking = false;
}

// -----------------------------------------------------------------------------

void ACookingPot::StartCooking(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (ACL_SoTMockCharacter* PlayerChar = Cast<ACL_SoTMockCharacter>(OtherActor))
	{
		if (FoodCurrentlyOnPot)
		{
			if (!(TimerHandle_Cooking.IsValid()))
			{
				CurrentCookingTime = 0;
				bHasFoodFinishedCooking = false;
				PlayerChar->AllowPlayerToFinishCooking(false);
				// Check the pot every second to see what's cooking and cook it further
				GetWorldTimerManager().SetTimer(TimerHandle_Cooking, this, &ACookingPot::TickCooking, 1.0f, true, 0.0f);
				UE_LOG(LogTemp, Log, TEXT("Cooking started!"));
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Cook timer is already ticking!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Food actor is invalid!"));
		}
	}
	else if (AFoodItem* FoodCarriedByPlayer = Cast<AFoodItem>(OtherActor))
	{
		if (!FoodCurrentlyOnPot)
		{
			FoodCurrentlyOnPot = FoodCarriedByPlayer;
			FDetachmentTransformRules FoodDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
			FAttachmentTransformRules FoodAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
			FoodCurrentlyOnPot->DetachFromActor(FoodDetachmentTransformRules);
			FoodCurrentlyOnPot->AttachToActor(this, FoodAttachmentTransformRules);
			UE_LOG(LogTemp, Log, TEXT("Food set on pot!"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Something's already cooking."));
		}
	}
}

// -----------------------------------------------------------------------------

void ACookingPot::TickCooking()
{
	if (FoodCurrentlyOnPot)
	{
		if (TimerHandle_Cooking.IsValid())
		{
			CurrentCookingTime++;
			UE_LOG(LogTemp, Log, TEXT("Cooking food..."));
			int32 CookingTimeDifference = CurrentCookingTime - FoodCurrentlyOnPot->GetTimeTakenToCook();
			switch (CookingTimeDifference)
			{
			case 0:
			{
				if (!bHasFoodFinishedCooking)
				{
					bHasFoodFinishedCooking = true;
					UE_LOG(LogTemp, Log, TEXT("The food has finished cooking!"));
				}
				break;
			}
			case 2:
			{
				StartBurningFood();
				UE_LOG(LogTemp, Log, TEXT("FOOD'S GONNA BUUURN!"));
				break;
			}
			default:
			{
				break;
			}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("The food actor is invalid!"));
	}
}

// -----------------------------------------------------------------------------

void ACookingPot::EndCooking()
{
	UE_LOG(LogTemp, Log, TEXT("Ending cooking!"));
	if (FoodCurrentlyOnPot)
	{
		if (TimerHandle_Cooking.IsValid())
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_Cooking);
			TimerHandle_Cooking.Invalidate();
			if (bHasFoodFinishedCooking)
			{
				UE_LOG(LogTemp, Warning, TEXT("Cooking completed successfully!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Cooking timer already cleared!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("The food actor is invalid!"));
	}
}

// -----------------------------------------------------------------------------

void ACookingPot::LetPlayerStopCooking(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACL_SoTMockCharacter* PlayerChar = Cast<ACL_SoTMockCharacter>(OtherActor))
	{
		PlayerChar->AllowPlayerToFinishCooking(true);
		UE_LOG(LogTemp, Log, TEXT("I've asked the player to stop cooking."));
	}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------