// Fill out your copyright notice in the Description page of Project Settings.


#include "SLauncher.h"

void ASLauncher::Fire()
{
	AActor* myOwner = GetOwner();
	if(myOwner && ProjectileClass)
	{
		FVector eyeLoc;
		FRotator eyeRot;
		myOwner->GetActorEyesViewPoint(eyeLoc, eyeRot);

		FVector shootDirection = eyeRot.Vector();
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, eyeRot, params);
	}
}
