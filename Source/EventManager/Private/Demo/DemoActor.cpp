// Copyright 2023 Xiao Feng. All Rights Reserved.


#include "DemoActor.h"
#include "MessageObject.h"
#include "Demo/Define.h"


// Sets default values
ADemoActor::ADemoActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADemoActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADemoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADemoActor::ReceiveMessage_Implementation(FName ControlSymbol,const TMap<FName, UMessageObject*>& Message)
{
	if (ControlSymbol == FName("SetLocation"))
	{
		if (auto ValueObject = *Message.Find(FName("Location")))
		{
			FVector NewLocation;
			ValueObject->GetValue(NewLocation);
			SetActorLocation(NewLocation);
		}
	}
	else if (ControlSymbol == FName("Destroy"))
	{
		Destroy();
	}
}

