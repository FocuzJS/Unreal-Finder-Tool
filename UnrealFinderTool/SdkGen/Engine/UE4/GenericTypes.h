#pragma once
#include <string>
#include <vector>
#include <array>

#include "PropertyFlags.h"
#include "FunctionFlags.h"
#include "IGenerator.h"
#include "SdkGen/EngineClasses.h"
#include "Utils.h"

// TODO: change all Get methods, how it's return

class UEClass;

class UEObject
{
protected:
	// This vars to speedup
	mutable int64_t packageIndex = -1;
	mutable std::string objName, fullName, nameCpp;

public:
	UObject Object;

	UEObject() = default;
	UEObject(const UObject object) : Object(object)
	{
	}

	bool IsValid() const;
	size_t GetIndex() const;
	uintptr_t GetAddress() const;

	std::string GetName() const;
	std::string GetNameCPP() const;
	std::string GetFullName() const;

	UEClass GetClass() const;
	UEObject& GetOuter() const;
	UEObject& GetPackageObject() const;

	void ReadInformation();

	template<typename Base>
	Base Cast(uintptr_t objAddress = 0) const
	{
		Base tmp;
		
		tmp.UEObject::Object = Object;
		if (objAddress != 0)
			tmp.UEObject::Object.ObjAddress = objAddress;

		return std::move(tmp);
	}

	template<typename T>
	bool IsA() const;

	// Check type in target process (Remote check type)
	bool IsA(const std::string& typeName) const;

	static UEClass StaticClass();
};

namespace std
{
	template<>
	struct hash<UEObject>
	{
		size_t operator()(const UEObject& obj) const noexcept
		{
			return std::hash<uintptr_t>()(obj.GetAddress());
		}
	};
}

inline UEObject UEObjectEmpty; // IsValid Will Return False for it
inline bool operator==(const UEObject& lhs, const UEObject& rhs) { return rhs.GetAddress() == lhs.GetAddress(); }
inline bool operator!=(const UEObject& lhs, const UEObject& rhs) { return !(lhs.GetAddress() == rhs.GetAddress()); }

class UEField : public UEObject
{
	mutable UField next;

public:
	using UEObject::UEObject;
	mutable UField Object;

	UEField()
	{
		Object = UEObject::Object.Cast<UField>();
	}

	UEField GetNext() const;

	static UEClass StaticClass();
};

class UEEnum : public UEField
{
public:
	using UEField::UEField;
	mutable UEnum Object;

	UEEnum()
	{
		Object = UEObject::Object.Cast<UEnum>();
	}

	std::vector<std::string> GetNames() const;

	static UEClass StaticClass();
};

class UEConst : public UEField
{
public:
	using UEField::UEField;

	UEConst() = default;

	std::string GetValue() const;

	static UEClass StaticClass();
};

class UEStruct : public UEField
{
	mutable UStruct superField;
	mutable UField children;

public:
	using UEField::UEField;
	mutable UStruct Object;

	UEStruct()
	{
		Object = UEObject::Object.Cast<UStruct>();
	}

	UEStruct GetSuper() const;

	UEField GetChildren() const;

	size_t GetPropertySize() const;

	static UEClass StaticClass();
};

class UEScriptStruct : public UEStruct
{
public:
	using UEStruct::UEStruct;

	UEScriptStruct() = default;

	static UEClass StaticClass();
};

class UEFunction : public UEStruct
{
public:
	using UEStruct::UEStruct;
	mutable UFunction Object;

	UEFunction()
	{
		Object = UEObject::Object.Cast<UFunction>();
	}

	UEFunctionFlags GetFunctionFlags() const;

	static UEClass StaticClass();
};

class UEClass : public UEStruct
{
public:
	using UEStruct::UEStruct;

	UEClass()
	{
		// CHeck if it's call UEStruct Constructor
	}

	static UEClass StaticClass();
};
inline UEClass UEClassEmpty; // IsValid Will Return False for it

class UEProperty : public UEField
{
public:
	enum class PropertyType
	{
		Unknown,
		Primitive,
		PredefinedStruct,
		CustomStruct,
		Container
	};

	struct Info
	{
		PropertyType Type = PropertyType::Unknown;
		size_t Size = 0;
		bool CanBeReference = false;
		std::string CppType;

		static Info Create(const PropertyType type, const size_t size, const bool reference, std::string&& cppType)
		{
			extern IGenerator* generator;

			return { type, size, reference, generator->GetOverrideType(cppType) };
		}
	};

private:
	mutable bool infoChanged = false;
	mutable Info curInfo{};

public:
	using UEField::UEField;
	mutable UProperty Object;

	UEProperty()
	{
		Object = UEObject::Object.Cast<UProperty>();
	}

	size_t GetArrayDim() const;

	size_t GetElementSize() const;

	UEPropertyFlags GetPropertyFlags() const;

	size_t GetOffset() const;

	Info GetInfo() const;

	static UEClass StaticClass();
};

class UENumericProperty : public UEProperty
{
public:
	using UEProperty::UEProperty;

	UENumericProperty() = default;

	static UEClass StaticClass();
};

class UEByteProperty : public UENumericProperty
{
	mutable UEnum enumProperty;

public:
	using UENumericProperty::UENumericProperty;
	mutable UByteProperty Object;

	bool IsEnum() const;

	UEByteProperty()
	{
		Object = UEObject::Object.Cast<UByteProperty>();
	}

	UEEnum GetEnum() const;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEUInt16Property : public UENumericProperty
{
public:
	using UENumericProperty::UENumericProperty;

	UEUInt16Property() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEUInt32Property : public UENumericProperty
{
public:
	using UENumericProperty::UENumericProperty;

	UEUInt32Property() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEUInt64Property : public UENumericProperty
{
public:
	using UENumericProperty::UENumericProperty;

	UEUInt64Property() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEInt8Property : public UENumericProperty
{
public:
	using UENumericProperty::UENumericProperty;

	UEInt8Property() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEInt16Property : public UENumericProperty
{
public:
	using UENumericProperty::UENumericProperty;

	UEInt16Property() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEIntProperty : public UENumericProperty
{
public:
	using UENumericProperty::UENumericProperty;

	UEIntProperty() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEInt64Property : public UENumericProperty
{
public:
	using UENumericProperty::UENumericProperty;

	UEInt64Property() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEFloatProperty : public UENumericProperty
{
public:
	using UENumericProperty::UENumericProperty;

	UEFloatProperty() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEDoubleProperty : public UENumericProperty
{
public:
	using UENumericProperty::UENumericProperty;

	UEDoubleProperty() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEBoolProperty : public UEProperty
{
public:
	using UEProperty::UEProperty;
	mutable UBoolProperty Object;

	UEBoolProperty()
	{
		Object = UEObject::Object.Cast<UBoolProperty>();
	}

	bool IsNativeBool() const { return GetFieldMask() == 0xFF; }

	bool IsBitfield() const { return !IsNativeBool(); }

	uint8_t GetFieldSize() const;

	uint8_t GetByteOffset() const;

	uint8_t GetByteMask() const;

	uint8_t GetFieldMask() const;

	std::array<int, 2> GetMissingBitsCount(const UEBoolProperty& other) const;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

inline bool operator<(const UEBoolProperty & lhs, const UEBoolProperty & rhs)
{
	if (lhs.GetByteOffset() == rhs.GetByteOffset())
	{
		return lhs.GetByteMask() < rhs.GetByteMask();
	}
	return lhs.GetByteOffset() < rhs.GetByteOffset();
}

class UEObjectPropertyBase : public UEProperty
{
	mutable UObjectPropertyBase propertyClass{};

public:
	using UEProperty::UEProperty;
	mutable UObjectPropertyBase Object;

	UEObjectPropertyBase()
	{
		Object = UEObject::Object.Cast<UObjectPropertyBase>();
	}

	UEClass GetPropertyClass() const;

	static UEClass StaticClass();
};

class UEObjectProperty : public UEObjectPropertyBase
{
public:
	using UEObjectPropertyBase::UEObjectPropertyBase;

	UEObjectProperty() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEClassProperty : public UEObjectProperty
{
	mutable UClassProperty metaClass{};

public:
	using UEObjectProperty::UEObjectProperty;
	mutable UClassProperty Object;

	UEClassProperty()
	{
		Object = UEObject::Object.Cast<UClassProperty>();
	}

	UEClass GetMetaClass() const;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEInterfaceProperty : public UEProperty
{
	mutable UInterfaceProperty interfaceClass{};

public:
	using UEProperty::UEProperty;
	mutable UInterfaceProperty Object;

	UEInterfaceProperty()
	{
		Object = UEObject::Object.Cast<UInterfaceProperty>();
	}

	UEClass GetInterfaceClass() const;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEWeakObjectProperty : public UEObjectPropertyBase
{
public:
	using UEObjectPropertyBase::UEObjectPropertyBase;

	UEWeakObjectProperty() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UELazyObjectProperty : public UEObjectPropertyBase
{
public:
	using UEObjectPropertyBase::UEObjectPropertyBase;

	UELazyObjectProperty() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEAssetObjectProperty : public UEObjectPropertyBase
{
public:
	using UEObjectPropertyBase::UEObjectPropertyBase;

	UEAssetObjectProperty() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEAssetClassProperty : public UEAssetObjectProperty
{
	mutable UClass metaClass;

public:
	using UEAssetObjectProperty::UEAssetObjectProperty;
	mutable UAssetClassProperty Object;

	UEAssetClassProperty()
	{
		Object = UEObject::Object.Cast<UAssetClassProperty>();
	}

	UEClass GetMetaClass() const;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UENameProperty : public UEProperty
{
public:
	using UEProperty::UEProperty;

	UENameProperty() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEStructProperty : public UEProperty
{
	mutable UStructProperty objStruct{};

public:
	using UEProperty::UEProperty;
	mutable UStructProperty Object;

	UEStructProperty()
	{
		Object = UEObject::Object.Cast<UStructProperty>();
	}

	UEScriptStruct GetStruct() const;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEStrProperty : public UEProperty
{
public:
	using UEProperty::UEProperty;

	UEStrProperty() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UETextProperty : public UEProperty
{
public:
	using UEProperty::UEProperty;

	UETextProperty() = default;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEArrayProperty : public UEProperty
{
	mutable UArrayProperty inner{};

public:
	using UEProperty::UEProperty;
	mutable UArrayProperty Object;

	UEArrayProperty()
	{
		Object = UEObject::Object.Cast<UArrayProperty>();
	}

	UEProperty GetInner() const;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEMapProperty : public UEProperty
{
	mutable UMapProperty keyProp{}, valueProp{};

public:
	using UEProperty::UEProperty;
	mutable UMapProperty Object;

	UEMapProperty()
	{
		Object = UEObject::Object.Cast<UMapProperty>();
	}

	UEProperty GetKeyProperty() const;
	UEProperty GetValueProperty() const;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEDelegateProperty : public UEProperty
{
	mutable UDelegateProperty signatureFunction{};

public:
	using UEProperty::UEProperty;
	mutable UDelegateProperty Object;

	UEDelegateProperty()
	{
		Object = UEObject::Object.Cast<UDelegateProperty>();
	}

	UEFunction GetSignatureFunction() const;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEMulticastDelegateProperty : public UEProperty
{
	mutable UDelegateProperty signatureFunction{};

public:
	using UEProperty::UEProperty;
	mutable UDelegateProperty Object; // DelegateProperty

	UEMulticastDelegateProperty()
	{
		Object = UEObject::Object.Cast<UDelegateProperty>();
	}

	UEFunction GetSignatureFunction() const;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

class UEEnumProperty : public UEProperty
{
	mutable UEnumProperty underlyingProp{};
	mutable UEnumProperty Enum{};

public:
	using UEProperty::UEProperty;
	mutable UEnumProperty Object;

	UEEnumProperty()
	{
		Object = UEObject::Object.Cast<UEnumProperty>();
	}

	UENumericProperty GetUnderlyingProperty() const;

	UEEnum GetEnum() const;

	UEProperty::Info GetInfo() const;

	static UEClass StaticClass();
};

template<typename T>
bool UEObject::IsA() const
{
	auto cmp = T::StaticClass();
	if (!cmp.IsValid())
	{
		return false;
	}

	for (UEClass super = GetClass(); super.IsValid(); super = super.GetSuper().Cast<UEClass>())
	{
		if (super.Object.IsEqual(cmp.Object))
			return true;
	}

	return false;
} 