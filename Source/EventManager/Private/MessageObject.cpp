// Copyright 2023 Xiao Feng. All Rights Reserved.


#include "MessageObject.h"

UMessageObject::~UMessageObject()
{
	Empty();
}

void UMessageObject::GetValue(FString& OutValue)
{
	if (DataType == EMessageType::String)
	{
		OutValue = Data.AsString;
	}
}

void UMessageObject::GetValue(bool& OutValue)
{
	if (DataType == EMessageType::Bool)
	{
		OutValue = Data.AsBool;
	}
}

void UMessageObject::GetValue(int32& OutValue)
{
	if (DataType == EMessageType::Int32)
	{
		OutValue = Data.AsInt;
	}
}

void UMessageObject::GetValue(uint32& OutValue)
{
	if (DataType == EMessageType::UInt32)
	{
		OutValue = Data.AsUInt;
	}
}

void UMessageObject::GetValue(float& OutValue)
{
	if (DataType == EMessageType::Float)
	{
		OutValue = Data.AsFloat;
	}
}

void UMessageObject::GetValue(int64& OutValue)
{
	if (DataType == EMessageType::Int64)
	{
		OutValue = Data.AsInt64;
	}
}

void UMessageObject::GetValue(uint64& OutValue)
{
	if (DataType == EMessageType::UInt64)
	{
		OutValue = Data.AsUInt64;
	}
}

void UMessageObject::GetValue(double& OutValue)
{
	if (DataType == EMessageType::Double)
	{
		OutValue = Data.AsDouble;
	}
}

void UMessageObject::GetValue(UObject*& OutValue)
{
	if (DataType == EMessageType::Object)
	{
		OutValue = Data.AsObject;
	}
}

void UMessageObject::GetValue(FVector& OutValue)
{
	if (DataType == EMessageType::Vector)
	{
		OutValue.X = Data.AsVector->X;
		OutValue.Y = Data.AsVector->Y;
		OutValue.Z = Data.AsVector->Z;
	}
}

void UMessageObject::SetValue(FString& Value)
{
	Empty();
	DataType = EMessageType::String;
	int StrLength = FCString::Strlen(*Value);
	// Allocate a buffer for the string plus terminator
	Data.AsString = new TCHAR[StrLength + 1];
	if (StrLength>0)
	{
		FCString::Strcpy(Data.AsString,StrLength+1,*Value);
	}
	else
	{
		Data.AsString[0] = TEXT('\0');
	}
}

void UMessageObject::SetValue(bool Value)
{
	Empty();
	DataType = EMessageType::Bool;
	Data.AsBool = Value;
}

void UMessageObject::SetValue(int32 Value)
{
	Empty();
	DataType = EMessageType::Int32;
	Data.AsInt = Value;
}

void UMessageObject::SetValue(uint32 Value)
{
	Empty();
	DataType = EMessageType::UInt32;
	Data.AsUInt = Value;
}

void UMessageObject::SetValue(float Value)
{
	Empty();
	DataType = EMessageType::Float;
	Data.AsFloat = Value;
}

void UMessageObject::SetValue(int64 Value)
{
	Empty();
	DataType = EMessageType::Int64;
	Data.AsInt64 = Value;
}

void UMessageObject::SetValue(uint64 Value)
{
	Empty();
	DataType = EMessageType::UInt64;
	Data.AsUInt64 = Value;
}

void UMessageObject::SetValue(double Value)
{
	Empty();
	DataType = EMessageType::Double;
	Data.AsDouble = Value;
}

void UMessageObject::SetValue(UObject* Value)
{
	Empty();
	DataType = EMessageType::Object;
	Data.AsObject = Value;
}

void UMessageObject::SetValue(FVector& Value)
{
	Empty();
	DataType = EMessageType::Vector;
	XYZ* Temp = new XYZ();
	Temp->X = Value.X;
	Temp->Y = Value.Y;
	Temp->Z = Value.Z;
	Data.AsVector = Temp;
}

void UMessageObject::Empty()
{
	switch (DataType)
	{
	case EMessageType::String:
		delete[] Data.AsString;
		break;
	case EMessageType::Vector:
		delete[] Data.AsVector;
		break;
	default:
		break;
	}
	DataType = EMessageType::Empty;
}





void UMessageObject::K2_SetValueInternal(FProperty* Property, void* ContainerAddress,FProperty* ResultProperty, void* ResultAddress)
{
	bool Success = true;
	//TODO:用Property->GetCPPType()是否更合适？
	if (Property->IsA<FStructProperty>())
	{
		FStructProperty* StructProperty = static_cast<FStructProperty*>(Property);
		if (StructProperty->Struct->GetName() == FString("Vector"))
		{
			FVector Value = *Property->ContainerPtrToValuePtr<FVector>(ContainerAddress);
			SetValue(Value);
		}
		else
		{
			Success = false;
		}
	}
	else if (Property->IsA<FIntProperty>())
	{
		int32 Value = *Property->ContainerPtrToValuePtr<int32>(ContainerAddress);
		SetValue(Value);
	}
	else if (Property->IsA<FStrProperty>())
	{
		FString Value = *Property->ContainerPtrToValuePtr<FString>(ContainerAddress);
		SetValue(Value);
	}
	else if (Property->IsA<FObjectProperty>())
	{
		UObject* Value = *Property->ContainerPtrToValuePtr<UObject*>(ContainerAddress);
		SetValue(Value);
	}
	else if (Property->IsA<FBoolProperty>())
	{
		bool Value = *Property->ContainerPtrToValuePtr<bool>(ContainerAddress);
		SetValue(Value);
	}
	else if (Property->IsA<FDoubleProperty>())
	{
		double Value = *Property->ContainerPtrToValuePtr<double>(ContainerAddress);
		SetValue(Value);
	}
	else if (Property->IsA<FInt64Property>())
	{
		int64 Value = *Property->ContainerPtrToValuePtr<int64>(ContainerAddress);
		SetValue(Value);
	}
	else if (Property->IsA<FFloatProperty>())
	{
		float Value = *Property->ContainerPtrToValuePtr<float>(ContainerAddress);
		SetValue(Value);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("MessageObject:不支持的泛型输入：ValueType:%s,InValueType:%s"),*EMessageType::ToString(DataType),*Property->GetCPPType())
		Success = false;
	}
	ResultProperty->SetValue_InContainer(ResultAddress,&Success);

}

void UMessageObject::K2_GetValueInternal(FProperty* Property, void* ContainerAddress , FProperty* ResultProperty , void* ResultAddress)
{
	bool Success = true;

	if (DataType == EMessageType::Vector && Property->IsA<FStructProperty>())
	{
		FStructProperty* StructProperty = CastField<FStructProperty>(Property);
		if (StructProperty->Struct->GetName() == FString("Vector"))
		{
			FVector Value ;
			GetValue(Value);
			StructProperty->SetValue_InContainer(ContainerAddress,&Value);
		}
		else
		{
			Success = false;
		}
	}
	else if (DataType == EMessageType::Int32 && Property->IsA<FIntProperty>())
	{
		int32 Value ;
		GetValue(Value);
		Property->SetValue_InContainer(ContainerAddress,&Value);
	}
	else if (DataType == EMessageType::String && Property->IsA<FStrProperty>())
	{
		FString Value ;
		GetValue(Value);
		Property->SetValue_InContainer(ContainerAddress,&Value);
	}
	else if (DataType == EMessageType::Object && Property->IsA<FObjectProperty>())
	{
		
		UObject* Value ;
		GetValue(Value);
		Property->SetValue_InContainer(ContainerAddress,&Value);
	}
	else if (DataType == EMessageType::Bool && Property->IsA<FBoolProperty>())
	{
		bool Value ;
		GetValue(Value);
		Property->SetValue_InContainer(ContainerAddress,&Value);
	}
	else if (DataType == EMessageType::Double && Property->IsA<FDoubleProperty>())
	{
		double Value ;
		GetValue(Value);
		Property->SetValue_InContainer(ContainerAddress,&Value);
	}
	else if (DataType == EMessageType::Int64 && Property->IsA<FInt64Property>())
	{
		int64 Value ;
		GetValue(Value);
		Property->SetValue_InContainer(ContainerAddress,&Value);
	}
	else if (DataType == EMessageType::Float && Property->IsA<FFloatProperty>())
	{
		float Value ;
		GetValue(Value);
		Property->SetValue_InContainer(ContainerAddress,&Value);
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("MessageObject:不支持的泛型输出：ValueType:%s,OutValueType:%s"),*EMessageType::ToString(DataType),*Property->GetCPPType())
		Success = false;
	}
	ResultProperty->SetValue_InContainer(ResultAddress,&Success);
}


