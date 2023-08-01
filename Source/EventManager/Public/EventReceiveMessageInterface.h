// Copyright 2023 Xiao Feng. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EventReceiveMessageInterface.generated.h"

UINTERFACE(Blueprintable)
class EVENTMANAGER_API UEventReceiveMessage : public UInterface
{
	GENERATED_BODY()
};

class EVENTMANAGER_API IEventReceiveMessage 
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 接收事件函数
	 * @param ControlSymbol 控制符
	 * @param Message 参数对象列表
	 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category = "Interact")
	void ReceiveMessage(FName ControlSymbol, const TMap<FName,UMessageObject*>& Message);
};

