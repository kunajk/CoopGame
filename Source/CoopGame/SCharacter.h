// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();
	virtual FVector GetPawnViewLocation() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void BeginCrouch();
	void EndCrouch();
	void BeginJump();
	void BeginZoom();
	void EndZoom();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coop|Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coop|Components")
	USpringArmComponent* StringArmComp;

	bool WantsToZoom{false};

	UPROPERTY(EditDefaultsOnly, Category="Coop|Config")
	float ZoomedFOV{65.0f};

	UPROPERTY(EditDefaultsOnly, Category="Coop|Config", meta=(ClampMin=0.1, ClampMax=100.0))
	float ZoomInterpSpeed{20.0};

	float DefaultFOV{90.0f};
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
