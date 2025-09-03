// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class HELLO_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Actor")
	AActor* SM_ChairActorReference;

private:
	void Move(int32 count);
	int32 step();
	float distance(FVector2D point1, FVector2D point2);
	void createEvent();
	bool myPercent(int32 _PER);
	int32 eventCount = 0;
	const int32 eventPER = 50;
	
	///  의자 움직이는 용	
	void MoveChair();
	int32 chairMoveCount = 0;
	FTimerHandle MoveTimerHandle;
	TArray<FVector2D> path;

};
