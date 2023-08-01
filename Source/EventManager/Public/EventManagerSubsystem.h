// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "EventManagerSubsystem.generated.h"

#define EVENT_FUNCNAME FName("FuncName")
DECLARE_LOG_CATEGORY_EXTERN(EventManager,Log,All)

class UMessageObject;
/**
 * 
 */
//匹配模式
UENUM(BlueprintType)
enum EEventMatchType
{
	//精确匹配
	Exact,
	//此Tag及其子项
	Contain
};


UCLASS()
class EVENTMANAGER_API UEventManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	
public:
	/**
	 * @brief 注册对象
	 * @param Group 要注册的目标组
	 * @param Tag 此对象的标签
	 * @param Object 
	 */
	UFUNCTION(BlueprintCallable, Category = "Manager")
	void RegistryObject(FGameplayTag Group,FGameplayTag Tag,UObject* Object);

	//不是必须的
	//并且不要在SendMessage中的逻辑调用此函数。因为SendMessage是遍历过程
	UFUNCTION(BlueprintCallable, Category = "Manager")
	void UnregistryObject(UObject* Object);

	/**
	 * @brief 发送事件
	 * @param Group 所注册的组
	 * @param Tag 对象的标签
	 * @param MathType 匹配方式
	 * @param Message 自定义数据
	 */
	UFUNCTION(BlueprintCallable, Category = "Interact", meta =( AutoCreateRefTerm = "Message"))
	void SendMessage(FGameplayTag Group,FGameplayTag Tag,EEventMatchType MathType,FName ControlSymbol,TMap<FName,UMessageObject*> Message );

	//创建MessageObject对象帮助函数
	UFUNCTION(BlueprintCallable, Category = "Utility")
	UMessageObject* MakeMessageObject();
	
private:
	//列表不会影响对象GC释放。虽然不取消注册、不互动接收事件会导致悬空指针，降低查找效率
	//但会在尝试调用时取消注册,避免仅注册，不使用的即可
	//Group-Tag-Object
	TMap<FGameplayTag,TMap<FGameplayTag,TArray<UObject*>>> RegisteredObject;
	//两个表数据相同。新建一个表是方便通过Object获取Tag	 Object-Group-Tag
	TMap<UObject*,TPair<FGameplayTag,FGameplayTag>> RegisteredObjectList;
};

