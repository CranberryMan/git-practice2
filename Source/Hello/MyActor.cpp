// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	/// Report no.5
	const int moveCount = 10;
	this->Move(moveCount);


	// Move에서 이동을 처리하려면 함수를 통째로 뜯어 고쳐야 해서 그냥 경로만 저장하고,
	// MoveChair에서 이동 처리를 함
	// (Move는 과제 내용이지만 MoveChair는 과제 내용이 아니기 때문에
	// 비효율 적이나 과제 내용을 훼손하지 않기 위함)
	GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, this, &AMyActor::MoveChair, 1.0f, true);
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActor::Move(int32 moveCount)
{
	FVector2D oldPoint(0, 0);
	FVector2D newPoint(0, 0);
	float totalDist = 0.f;
	float fromOriginDist = 0.f;

	for (int i = 0; i < moveCount; i++)
	{
		int moveX = this->step();
		int moveY = this->step();

		while (true)
		{
			/// x, y 둘 다 0일 경우 다시 랜덤값 생성
			if (moveX != 0 || moveY != 0)
			{
				break;
			}
		}
		newPoint[0] = oldPoint[0] + moveX;
		newPoint[1] = oldPoint[1] + moveY;

		float dist = distance(oldPoint, newPoint);
		totalDist += dist;
		createEvent();
		UE_LOG(LogTemp, Warning, TEXT("move %d : now [%f, %f], dist[%f]"), i, newPoint[0], newPoint[1], dist);
		
		oldPoint = newPoint;

		path.Add(newPoint);
	}

	// 총 이동거리와 총 이벤트 발동 횟수 표시
	// 총 이동거리는 원점으로부터의 총 이동거리인지, 왔다갔다 한 것도 전부 계산하는 건지 확실치 않아
	// 그냥 둘 다 출력함
	fromOriginDist = distance(FVector2D(0, 0), newPoint);
	UE_LOG(LogTemp, Error, TEXT("Total moved distance : %f"), totalDist);
	UE_LOG(LogTemp, Error, TEXT("Distance from origin : %f"), fromOriginDist);
	UE_LOG(LogTemp, Error, TEXT("Total event count : %d"), eventCount);

}

int32 AMyActor::step()
{

	int minV = 0; // -1로 하는게 자연스럽지만 발제 문서가 0으로 되어있어 그대로 둠
	int maxV = 1;
	int32 rtrInt = FMath::RandRange(minV, maxV);
	
	return rtrInt;
}

float AMyActor::distance(FVector2D point1, FVector2D point2)
{
	float rtrDist = 0.f;
	float x = (point2[0] - point1[0])* (point2[0] - point1[0]);
	float y = (point2[1] - point1[1]) * (point2[1] - point1[1]);
	rtrDist = FMath::Sqrt(x + y);

	return rtrDist;
}

void AMyActor::createEvent()
{
	if(myPercent(eventPER))
	{
		UE_LOG(LogTemp, Warning, TEXT("Event Created!"));
		eventCount++;
	}

	return;
}

// 이론상 step이 50%확률로 0, 1을 반환하지만
// step은 1칸이 아니라 2칸, 3칸 등등 수정 가능성이 있음으로
// step()을 사용하지 않고 확률을 따로 계산
bool AMyActor::myPercent(int32 _PER)
{
	// 예외 방지용
	_PER = FMath::Clamp(_PER, 0, 100);

	static FRandomStream RandomStream;
	int32 RandomNumber = RandomStream.RandRange(0, 99);

	// 랜덤으로 생성된 숫자가 _PER보다 작으면 true 반환
	// 예를 들어 _PER이 30이면, 0~29 사이의 숫자가 나오면 true가 되어 30%의 확률이다
	return RandomNumber < _PER;
}

void AMyActor::MoveChair()
{
	// 타이머 멈추기용
	if (chairMoveCount >= 10)
	{
		GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
		return;
	}
	

	// 위치이동, 0, 1로 이동하면 이동 위치가 너무 미미해서 눈으로 볼때 확확 눈에 띄라고
	// 10을 곱해줌
	FVector NewLocation = FVector(path[chairMoveCount][0]*100, path[chairMoveCount][1] * 100, 0.0f);
	SM_ChairActorReference->SetActorLocation(NewLocation);
	chairMoveCount++;
	
}