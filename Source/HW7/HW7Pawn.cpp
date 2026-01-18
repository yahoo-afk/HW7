#include "HW7Pawn.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "HW7PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

//로직정리 해라주말에

AHW7Pawn::AHW7Pawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SecneRoot"));
	SetRootComponent(Capsule);

	AirPlane = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player"));
	AirPlane->SetupAttachment(Capsule);
	if (AirPlane)
	{
		AirPlane->SetSimulatePhysics(false);
	}

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(Capsule);
	SpringArmComp->TargetArmLength = 500.f;
	SpringArmComp->bUsePawnControlRotation = true;
	//SpringArmComp->bInheritPitch = true;
	//SpringArmComp->bInheritYaw = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;
	
	CurrentZSpeed = 0;
}

void AHW7Pawn::BeginPlay()
{
	Super::BeginPlay();
	Radius = Capsule->GetScaledCapsuleRadius();
	HalfHeigt = Capsule->GetScaledCapsuleHalfHeight();
}

void AHW7Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentZSpeed += (-980 * DeltaTime);                          // 현재속도				

	//충돌검사
	if (Capsule)
	{
		//도형의 크기
		FCollisionShape CharacterShape = FCollisionShape::MakeCapsule(Radius, HalfHeigt);
		//출동결과를 받을상자
		FHitResult HitResult;
		//무시할 대상설정
		FCollisionQueryParams Paramas;
		Paramas.AddIgnoredActor(this);
		//엑터의 시작위치
		FVector StartLocation = GetActorLocation();
		// 엑터의 마지막위치
		FVector EndLocation = StartLocation + FVector(0, 0, -1);

		GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, GetActorQuat(), ECollisionChannel::ECC_Visibility, CharacterShape, Paramas);
		
		if (HitResult.bBlockingHit)
		{
			CurrentZSpeed = 0;
			bIsFalling = false;
		}
		else
		{
			bIsFalling = true;
		}
		if (CurrentZSpeed > 3000)
		{
			CurrentZSpeed = 980;
		}
	}
	AddActorWorldOffset(FVector(0, 0, CurrentZSpeed * DeltaTime));//위치의변화량 
}

void AHW7Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// UEnhancedInputComponent타입의 변수EnhancedInput 만들고 PlayerInputComponent 가 실재로 UEnhancedInputComponent 이타입이라면 애를UEnhancedInputComponent 애로 바라보고 그주소를 넣어주겟다?
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 그리고 내가 만든 플레이어 컨트롤러의 포인터로 플레이어 컨트롤러라는 변수를 만들고 겟컨트롤러의 형태를 내가만든 컨트롤러형태로 바꾸겠다.
		if (AHW7PlayerController* PlayerController = Cast<AHW7PlayerController>(GetController()))
		{

			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AHW7Pawn::Move
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AHW7Pawn::Look
				);
			}
		}
	}
}

void AHW7Pawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;
	//월드의 델타타임을 가져오는함수 tick에있는 델타타임 호출
	const float Deltatime = GetWorld()->GetDeltaSeconds();
	//가독성을위해 한번더 포장해주기
	// 애는 메모리복사 해서 통채로 메모리 복사가 일어나고
	//const FVector Input = value.Get<FVector>();
	//애는 래퍼런스로 참조해서 주소메모리만큼만 잡아먹는다
	const FVector& Input = value.Get<FVector>();
	float TargetSpeed = Input.Y * 1000.0f;
	float TargetRightSpeed = Input.X * 1000.0f;
	float TargetupSpeed = Input.Z * 3000.0f;
			//x축으로 이동할때1초에 1000만큼,y축으로 이동할때1초 1000만큼,z축으로 이동할때1초에 3000만큼
	FVector FinalVector(TargetSpeed, TargetRightSpeed, TargetupSpeed);

	//공중이라면 0.3만큼 곱하고 아니라면 1만큼 곱한다
	FinalVector *= bIsFalling ? 0.3 : 1;

	//방향의 속도가 0에 가까우면 0으로치니까 0이 아니라면
	if(!FinalVector.IsNearlyZero())
	{
		//파이널벡터 에 델타타임을 곱하고
		FinalVector *= Deltatime;
		//현재 엑터의 기준으로 FinalVector 움직이고 충돌허용 false라면 충돌 비허용
		AddActorLocalOffset(FinalVector, true);
	}
}

void AHW7Pawn::Look(const FInputActionValue& value)
{
	//회전률 1로
	float Sensitivity = 1.0f;
	//가독성을 vs 메모리 효율;
	const FVector& LookInput = value.Get<FVector>();
	FVector DeltaDegrees = LookInput * Sensitivity;
	FRotator CurrentRotator = GetActorRotation();

	CurrentRotator.Yaw += DeltaDegrees.X;
	CurrentRotator.Pitch += DeltaDegrees.Y;
	CurrentRotator.Roll += DeltaDegrees.Z;

	SetActorRotation(CurrentRotator);
}
	/*FRotator CameraSensitivity;
	CameraSensitivity = SpringArmComp->GetRelativeRotation();
	CameraSensitivity.Pitch += DeltaDegrees.Y;
	CameraSensitivity.Yaw += DeltaDegrees.X;
	CameraSensitivity.Roll += DeltaDegrees.Z;*/
