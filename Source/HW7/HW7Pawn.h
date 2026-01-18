// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "HW7Pawn.generated.h"

class UCameraComponent;
class USpringArmComponent;

struct FInputActionValue;

UCLASS()
class HW7_API AHW7Pawn : public APawn
{
	GENERATED_BODY()

public:
	AHW7Pawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Capsule")
	UCapsuleComponent* Capsule;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Player")
	USkeletalMeshComponent* AirPlane;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="SpringArm")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Camera")
	UCameraComponent* CameraComp;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	
	float CurrentZSpeed;
	bool bIsFalling;
	float HalfHeigt;
	float Radius;

};
