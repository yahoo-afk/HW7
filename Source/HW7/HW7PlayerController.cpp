
#include "HW7PlayerController.h"
#include "EnhancedInputSubsystems.h"
AHW7PlayerController::AHW7PlayerController()
	:InputMappingContext(nullptr),
	JumpAction(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr)
{ 
}

void AHW7PlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Yahoo's Controller is Active!"));

	if(ULocalPlayer* LocalPlayer  =GetLocalPlayer())
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem 
			= LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if(InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}