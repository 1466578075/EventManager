// Copyright 2023 Xiao Feng. All Rights Reserved.


#include "EventManagerSubsystem.h"

#include "EventReceiveMessageInterface.h"
#include "GameplayTagsManager.h"
#include "MessageObject.h"

DEFINE_LOG_CATEGORY(EventManager)

void UEventManagerSubsystem::RegistryObject(FGameplayTag Group, FGameplayTag Tag, UObject* Object)
{
	if (!ensure(Group.IsValid() && Tag.IsValid() && IsValid(Object)))
	{
		return;
	}

	if (Object->Implements<UEventReceiveMessage>())
	{
		//如果已经注册过，则覆盖
		if (RegisteredObjectList.Contains(Object) )
		{
			auto Pair = RegisteredObjectList.Find(Object);
			if (Pair->Key != Group || Pair->Value != Tag)
			{
				UnregistryObject(Object);
			}
			else
			{
				UE_LOG(EventManager,Warning,TEXT(""))
				return;
			}
		}
		
		if (!RegisteredObject.Contains(Group))
		{
			TArray<TWeakObjectPtr<UObject>> Array;
			Array.Add(Object);
			TMap<FGameplayTag,TArray<TWeakObjectPtr<UObject>>> Map ;
			Map.Add(Tag,Array);
			RegisteredObject.Add(Group,Map);
		}
		else if (!RegisteredObject.Find(Group)->Contains(Tag))
		{
			TArray<TWeakObjectPtr<UObject>> Array;
			Array.Add(Object);
			RegisteredObject.Find(Group)->Add(Tag,Array);
		}
		else
		{
			RegisteredObject.Find(Group)->Find(Tag)->Add(Object);
		}
		RegisteredObjectList.Add(Object,TPair<FGameplayTag,FGameplayTag>(Group,Tag));
		UE_LOG(EventManager,Verbose,TEXT("Subsystem:注册对象：%p"),Object)
	}
	else
	{
		UE_LOG(EventManager,Warning,TEXT("Subsystem:注册对象失败，对象没有实现IEventReceiveMessage接口：%s"),*Object->GetName())
	}
	
}

void UEventManagerSubsystem::UnregistryObject(UObject* Object)
{
	auto Temp = TWeakObjectPtr<UObject>(Object);
	UnregistryObject(Temp);
}

void UEventManagerSubsystem::UnregistryObject(TWeakObjectPtr<UObject>& Object)
{
	if (auto Pair = RegisteredObjectList.Find(Object))
	{
		if (auto Map = RegisteredObject.Find(Pair->Key))
		{
			if (auto Array = Map->Find(Pair->Value))
			{
				Array->Remove(Object);
				if (Array->Num()==0)
				{
					Map->Remove(Pair->Value);
				}
			}
			if (Map->Num()==0)
			{
				RegisteredObject.Remove(Pair->Key);
			}
		}
		RegisteredObjectList.Remove(Object);
		UE_LOG(EventManager,Verbose,TEXT("Subsystem:注销对象%p"),Object.Get())

	}
	else
	{
		UE_LOG(EventManager,Warning,TEXT("Subsystem:注销未找到对象%p"),Object.Get())
	}
}

void UEventManagerSubsystem::SendMessage(FGameplayTag Group, FGameplayTag Tag, EEventMatchType MathType,FName ControlSymbol,
                                         TMap<FName, UMessageObject*> Message)
{
	if (!ensure(Group.IsValid() && Tag.IsValid()))
	{
		return;
	}
	if (auto Map = RegisteredObject.Find(Group))
	{
		//已失效的对象列表，查找完毕后取消注册。遍历过程更改数组数量会导致异常
		TArray<TWeakObjectPtr<UObject>*> InvalidItems;
		if (MathType == Exact)
		{
			if (auto Items = Map->Find(Tag))
			{
				for (auto& Item : *Items)
				{
					if (Item.IsValid())
					{
						IEventReceiveMessage::Execute_ReceiveMessage(Item.Get(),ControlSymbol,Message);
					}
					else
					{
						InvalidItems.Add(&Item);
					}
				}
			}
		}
		else if (MathType == Contain)
		{
			for (auto TagPairIt = Map->CreateIterator(); TagPairIt; ++TagPairIt)
			{
				if (TagPairIt->Key.MatchesTag(Tag))
				{
					for (auto& Item : TagPairIt->Value)
					{
						if (Item.IsValid())
						{
							IEventReceiveMessage::Execute_ReceiveMessage(Item.Get(),ControlSymbol,Message);
						}
						else
						{
							InvalidItems.Add(&Item);
						}
					}
				}
			}
		}
		//删除已经失效的对象
		for (auto InvalidItem :InvalidItems)
		{
			UnregistryObject(*InvalidItem);
		}
	}
}

UMessageObject* UEventManagerSubsystem::MakeMessageObject()
{
	return NewObject<UMessageObject>();
}
