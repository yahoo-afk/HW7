// Fill out your copyright notice in the Description page of Project Settings.
#include "HW7GameMode.h"
#include "HW7PlayerController.h"
#include "HW7Pawn.h"

AHW7GameMode::AHW7GameMode()
{
	DefaultPawnClass = AHW7Pawn::StaticClass();
	PlayerControllerClass = AHW7PlayerController::StaticClass();
}
