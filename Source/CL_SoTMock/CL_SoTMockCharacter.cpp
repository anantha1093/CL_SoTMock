// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CL_SoTMockCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "FoodItem.h"
#include "DrawDebugHelpers.h"
#include "CookingPot.h"

// -----------------------------------------------------------------------------

ACL_SoTMockCharacter::ACL_SoTMockCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// This is the expected default socket
	FoodSocketName = FName(TEXT("FoodItemSocket"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

// -----------------------------------------------------------------------------

// Called when the game starts or when spawned
void ACL_SoTMockCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (QuestTableReference)
	{
		static const FString ContextString(TEXT("Quest data"));
		PlayerQuestDetails = *(QuestTableReference->FindRow<FQuestDetails>(TEXT("Quest_1"), ContextString, true));
		UE_LOG(LogTemp, Log, TEXT("Successfully got quest data from table!"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Can't get quest data from table!"));
	}
}

// -----------------------------------------------------------------------------

void ACL_SoTMockCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &ACL_SoTMockCharacter::PickUpObject);
	PlayerInputComponent->BindAction("EndCooking", IE_Pressed, this, &ACL_SoTMockCharacter::TellPotToEndCooking);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACL_SoTMockCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACL_SoTMockCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACL_SoTMockCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACL_SoTMockCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACL_SoTMockCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACL_SoTMockCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACL_SoTMockCharacter::OnResetVR);
}

// -----------------------------------------------------------------------------

void ACL_SoTMockCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

// -----------------------------------------------------------------------------

void ACL_SoTMockCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

// -----------------------------------------------------------------------------

void ACL_SoTMockCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

// -----------------------------------------------------------------------------

void ACL_SoTMockCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// -----------------------------------------------------------------------------

void ACL_SoTMockCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// -----------------------------------------------------------------------------

void ACL_SoTMockCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

// -----------------------------------------------------------------------------

void ACL_SoTMockCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

// -----------------------------------------------------------------------------

void ACL_SoTMockCharacter::SetCurrentPlayerQuest(FQuestDetails QuestToBeMadeCurrent)
{
	PlayerQuestDetails = QuestToBeMadeCurrent;
}

// -----------------------------------------------------------------------------

void ACL_SoTMockCharacter::PickUpObject()
{
	FVector TraceStartLocation;
	FVector TraceEndLocation;
	float LineTraceDistance = 400.0f;
	
	TraceStartLocation = FollowCamera->GetComponentLocation();
	FRotator CameraRot = FollowCamera->GetComponentRotation();

	TraceEndLocation = TraceStartLocation + (CameraRot.Vector() * LineTraceDistance);

	FCollisionQueryParams PickupTraceParams(FName(TEXT("PickupTrace")), false, nullptr);

	TArray<FHitResult> PickupHitResults;

	if (GetWorld()->SweepMultiByChannel(
		PickupHitResults,
		TraceStartLocation,
		TraceEndLocation, 
		FQuat::Identity, 
		ECollisionChannel::ECC_GameTraceChannel1, 
		FCollisionShape::MakeSphere(100.0f), 
		PickupTraceParams))
	{
		DrawDebugSphere(GetWorld(), TraceStartLocation, 100.0f, 12, FColor::Red, true, 10.0f, 1, 2.0f);
		for (int32 i = 0; i < PickupHitResults.Num(); i++)
		{
			FHitResult PickupHitResult = PickupHitResults[i];
			if (AFoodItem* FoodItemToPickup = Cast<AFoodItem>(PickupHitResult.Actor))
			{
				DrawDebugSphere(GetWorld(), PickupHitResult.ImpactPoint, 100.0f, 12, FColor::Red, true, 10.0f, 1, 2.0f);
				FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);
				FoodItemToPickup->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FoodSocketName);
				UE_LOG(LogTemp, Log, TEXT("Attached food to character!"));
			}
			else
			{
				DrawDebugSphere(GetWorld(), PickupHitResult.ImpactPoint, 100.0f, 12, FColor::Yellow, true, 10.0f, 1, 2.0f);
				UE_LOG(LogTemp, Log, TEXT("Didn't hit a food item."));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Trace hit nothing."));
	}
}

// -----------------------------------------------------------------------------

void ACL_SoTMockCharacter::TellPotToEndCooking()
{
	if (CookingPotClass)
	{
		TArray<AActor*> OverlappedActors;
		GetOverlappingActors(OverlappedActors, CookingPotClass);
		for (int32 i = 0; i < OverlappedActors.Num(); i++)
		{
			if (ACookingPot* CookingPot = Cast<ACookingPot>(OverlappedActors[i]))
			{
				CookingPot->EndCooking();
			}
		}
	}
}

void ACL_SoTMockCharacter::AllowPlayerToFinishCooking(bool bPlayerCanFinishCooking)
{
	bCanFinishCooking = bPlayerCanFinishCooking;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------