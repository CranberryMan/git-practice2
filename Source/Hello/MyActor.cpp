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


	// Move���� �̵��� ó���Ϸ��� �Լ��� ��°�� ��� ���ľ� �ؼ� �׳� ��θ� �����ϰ�,
	// MoveChair���� �̵� ó���� ��
	// (Move�� ���� ���������� MoveChair�� ���� ������ �ƴϱ� ������
	// ��ȿ�� ���̳� ���� ������ �Ѽ����� �ʱ� ����)
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
			/// x, y �� �� 0�� ��� �ٽ� ������ ����
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

	// �� �̵��Ÿ��� �� �̺�Ʈ �ߵ� Ƚ�� ǥ��
	// �� �̵��Ÿ��� �������κ����� �� �̵��Ÿ�����, �Դٰ��� �� �͵� ���� ����ϴ� ���� Ȯ��ġ �ʾ�
	// �׳� �� �� �����
	fromOriginDist = distance(FVector2D(0, 0), newPoint);
	UE_LOG(LogTemp, Error, TEXT("Total moved distance : %f"), totalDist);
	UE_LOG(LogTemp, Error, TEXT("Distance from origin : %f"), fromOriginDist);
	UE_LOG(LogTemp, Error, TEXT("Total event count : %d"), eventCount);

}

int32 AMyActor::step()
{

	int minV = 0; // -1�� �ϴ°� �ڿ��������� ���� ������ 0���� �Ǿ��־� �״�� ��
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

// �̷л� step�� 50%Ȯ���� 0, 1�� ��ȯ������
// step�� 1ĭ�� �ƴ϶� 2ĭ, 3ĭ ��� ���� ���ɼ��� ��������
// step()�� ������� �ʰ� Ȯ���� ���� ���
bool AMyActor::myPercent(int32 _PER)
{
	// ���� ������
	_PER = FMath::Clamp(_PER, 0, 100);

	static FRandomStream RandomStream;
	int32 RandomNumber = RandomStream.RandRange(0, 99);

	// �������� ������ ���ڰ� _PER���� ������ true ��ȯ
	// ���� ��� _PER�� 30�̸�, 0~29 ������ ���ڰ� ������ true�� �Ǿ� 30%�� Ȯ���̴�
	return RandomNumber < _PER;
}

void AMyActor::MoveChair()
{
	// Ÿ�̸� ���߱��
	if (chairMoveCount >= 10)
	{
		GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
		return;
	}
	

	// ��ġ�̵�, 0, 1�� �̵��ϸ� �̵� ��ġ�� �ʹ� �̹��ؼ� ������ ���� ȮȮ ���� ����
	// 10�� ������
	FVector NewLocation = FVector(path[chairMoveCount][0]*100, path[chairMoveCount][1] * 100, 0.0f);
	SM_ChairActorReference->SetActorLocation(NewLocation);
	chairMoveCount++;
	
}