// Copyright 2023 Xiao Feng. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MessageObject.generated.h"


namespace EMessageType
{
	enum Type
	{
		Empty,
		/** 32 bit integer */
		Int32,
		/** 32 bit unsigned integer */
		UInt32,
		/** 64 bit integer */
		Int64,
		/** 64 bit unsigned integer */
		UInt64,
		/** Double (8 byte) */
		Double,
		/** String */
		String,
		/** Float (4 byte) */
		Float,
		/** bool data (1 byte) */
		Bool,
		/*Object pointer*/
		Object,
		Vector
	};

	FString ToString(Type InEnum)
	{
		switch (InEnum)
		{
		case Empty:
			return FString("Empty");
		case Int32:
			return FString("Int32");
		case UInt32:
			return FString("Uint32");
		case Int64:
			return FString("Int64");
		case UInt64:
			return FString("Uint64");
		case Double:
			return FString("Double");
		case String:
			return FString("String");
		case Float:
			return FString("Float");
		case Bool:
			return FString("Bool");
		case Object:
			return FString("Object");
		case Vector:
			return FString("Vector");
		default:
			return FString("Unknown");
		}
	}
}

UCLASS()
class EVENTMANAGER_API UMessageObject : public UObject
{
	GENERATED_BODY()
	virtual ~UMessageObject() override;
public:
	/*template<typename T>
	T GetValue(T&);*/

	void GetValue(FString& OutValue);
	void GetValue(bool& OutValue);
	void GetValue(int32& OutValue);
	void GetValue(uint32& OutValue);
	void GetValue(float& OutValue);
	void GetValue(int64& OutValue);
	void GetValue(uint64& OutValue);
	void GetValue(double& OutValue);
	void GetValue(UObject*& OutValue);
	void GetValue(FVector& OutValue);

	void SetValue(FString& Value);
	void SetValue(bool Value);
	void SetValue(int32 Value);
	void SetValue(uint32 Value);
	void SetValue(float Value);
	void SetValue(int64 Value);
	void SetValue(uint64 Value);
	void SetValue(double Value);
	void SetValue(UObject* Value);
	void SetValue(FVector& Value);

	//清空数据（当前仅有String会开辟新内存储存额外数据）
	void Empty();

	EMessageType::Type GetType()const {return DataType;}

	//已实现int、int64、Float、double、bool、FString、UObject、FVector
	UFUNCTION(BlueprintCallable,DisplayName = "Set Value",Category = "Setter",CustomThunk,meta = (CustomStructureParam = "Value"))
	void K2_SetValue(UObject* Value,bool& Success);
	DECLARE_FUNCTION(execK2_SetValue)
	{
		Stack.StepCompiledIn<FProperty>(nullptr);
		uint8* ContainerAddress = Stack.MostRecentPropertyContainer;
		FProperty* Property = Stack.MostRecentProperty;
		//宏提供的是新变量，并不是引用，不能直接修改蓝图值
		//P_GET_PROPERTY(FBoolProperty,Z_ParamBool)

		Stack.StepCompiledIn<FBoolProperty>(nullptr);
		uint8* ResultAddress = Stack.MostRecentPropertyContainer;
		FProperty* ResultProperty = Stack.MostRecentProperty;
		P_FINISH
		
		P_NATIVE_BEGIN
		P_THIS->K2_SetValueInternal(Property,ContainerAddress,ResultProperty,ResultAddress);
		P_NATIVE_END
	}
	//与SetValue相对应，已实现int、int64、float、double、bool、FString、UObject、FVector的逻辑
	UFUNCTION(BlueprintCallable,DisplayName = "Get Value",Category = "Getter",CustomThunk,meta = (CustomStructureParam = "Value"))
	void K2_GetValue(UObject*& Value,bool& Success);
	DECLARE_FUNCTION(execK2_GetValue)
	{
		Stack.StepCompiledIn<FProperty>(nullptr);
		FProperty* Property = Stack.MostRecentProperty;
		uint8* ContainerAddress = Stack.MostRecentPropertyContainer;

		Stack.StepCompiledIn<FBoolProperty>(nullptr);
		uint8* ResultAddress = Stack.MostRecentPropertyContainer;
		FProperty* ResultProperty = Stack.MostRecentProperty;
		P_FINISH
		
		P_NATIVE_BEGIN
		P_THIS->K2_GetValueInternal(Property,ContainerAddress,ResultProperty,ResultAddress);
		P_NATIVE_END
	}
	
private:
	void K2_SetValueInternal(FProperty* Property, void* ContainerAddress,FProperty* ResultProperty,void* ResultAddress);
	void K2_GetValueInternal(FProperty* Property, void* ContainerAddress,FProperty* ResultProperty,void* ResultAddress);
	
private:
	EMessageType::Type DataType;

	//储存Vector、Rotator等,不增加Data大小
	struct XYZ
	{
		double X;
		double Y;
		double Z;
	};
	
	union ValueUnion
	{
		bool AsBool;
		int32 AsInt;
		uint32 AsUInt;
		float AsFloat;
		int64 AsInt64;
		uint64 AsUInt64;
		double AsDouble;
		XYZ* AsVector;
		TCHAR* AsString;
		UObject* AsObject;
	}Data;
};
