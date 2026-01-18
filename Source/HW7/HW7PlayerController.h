
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HW7PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class HW7_API AHW7PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AHW7PlayerController();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Inpuut")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inpuut")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inpuut")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inpuut")
	UInputAction* SprintAction;

protected:
	virtual void BeginPlay() override;
};
