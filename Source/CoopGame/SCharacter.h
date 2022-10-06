// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ASWeapon;
class USHealthComponent;

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
	virtual void GetLifetimeReplicatedProps(class TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void BeginCrouch();
	void EndCrouch();
	void BeginJump();
	void BeginZoom();
	void EndZoom();

	UFUNCTION(BlueprintCallable, Category="Coop|Player")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category="Coop|Player")
	void StopFire();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coop|Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coop|Components")
	USpringArmComponent* StringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coop|Components")
	USHealthComponent* HealthComponent;

	bool WantsToZoom{false};

	UPROPERTY(EditDefaultsOnly, Category="Coop|Config")
	float ZoomedFOV{65.0f};

	UPROPERTY(EditDefaultsOnly, Category="Coop|Config", meta=(ClampMin=0.1, ClampMax=100.0))
	float ZoomInterpSpeed{20.0};

	float DefaultFOV{90.0f};

	UPROPERTY(Replicated)
	ASWeapon* CurrentWeapon;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Coop|Player")
	bool IsDead{false};
	
	UPROPERTY(EditDefaultsOnly, Category="Coop|Layer");
	TSubclassOf<ASWeapon> StartWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category="Coop|Config")
	FName WeapomAttachmenSocketName;

	UFUNCTION()
	void HandleHealthChanged(USHealthComponent* MyHealthComponent, float Health, float HealthDelta, const UDamageType* DamageType, AController* Instigated, AActor* DamageCauser);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};


