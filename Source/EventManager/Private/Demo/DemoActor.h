// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventReceiveMessageInterface.h"
#include "GameFramework/Actor.h"
#include "DemoActor.generated.h"



UCLASS()
class EVENTMANAGER_API ADemoActor : public AActor,public IEventReceiveMessage
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADemoActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ReceiveMessage_Implementation(FName ControlSymbol, const TMap<FName, UMessageObject*>& Message) override;
};
