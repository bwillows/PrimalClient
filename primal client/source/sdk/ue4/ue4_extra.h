#include "../../includes.hpp"

/* Predomiantly UE4 class & struct types */

class UObject;
class UClass;

template<class T>
class TArray
{
	friend struct FString;
public:
	TArray() { Count = Max = 0; };

	T& operator[](int i) const { return Data[i]; };

	T* Data;
	int Count; // int32_t
	int Max; // int32_t
};

struct FNameEntry
{
	uint32_t Index;
	uint32_t Pad;
	FNameEntry* HashNext;

	union
	{
		char AnsiName[1024];
		wchar_t WideName[1024];
	};
	const int GetIndex() const { return Index >> 1; }
	const char* GetAnsiName() const { return AnsiName; }
};

class TNameEntryArray
{
public:
	bool IsValidIndex(uint32_t index) const { return index < NumElements; }

	FNameEntry const* GetByID(uint32_t index) const { return *GetItemPtr(index); }

	FNameEntry const* const* GetItemPtr(uint32_t Index) const
	{
		const auto ChunkIndex = Index / 16384;
		const auto WithinChunkIndex = Index % 16384;
		const auto Chunk = Chunks[ChunkIndex];

		return Chunk + WithinChunkIndex;
	}

	FNameEntry** Chunks[128]; // ChunkTableSize
	uint32_t NumElements = 0;
	uint32_t NumChunks = 0;
};

struct FName
{
public:
	int ComparisonIndex = 0;
	int Number = 0;

	static inline TNameEntryArray* GNames = nullptr;

	static const char* GetNameByIDFast(int ID)
	{
		auto NameEntry = GNames->GetByID(ID);
		if (!NameEntry) return nullptr;
		return NameEntry->AnsiName;
	}
	static std::string GetNameByID(int ID)
	{
		auto NameEntry = GNames->GetByID(ID);
		if (!NameEntry) return std::string();
		return NameEntry->AnsiName;
	}
	const char* GetNameFast() const
	{
		auto NameEntry = GNames->GetByID(ComparisonIndex);
		if (!NameEntry) return nullptr;
		return NameEntry->AnsiName;
	}
	const std::string get_name() const
	{
		auto NameEntry = GNames->GetByID(ComparisonIndex);
		if (!NameEntry) return std::string();
		return NameEntry->AnsiName;
	}
	inline bool operator==(const FName& other) const
	{
		return ComparisonIndex == other.ComparisonIndex;
	}

	FName() {}
	FName(const char* nameToFind)
	{
		for (int i = 1000u; i < GNames->NumElements; i++)
		{
			auto Name = GetNameByIDFast(i);
			if (!Name) continue;
			if (strcmp(Name, nameToFind) == 0)
			{
				ComparisonIndex = i;
				return;
			}
		}
	}
};

class FUObjectItem
{
public:
	class UObject* Object;
	int32_t SerialNumber;
	unsigned char pad_C1AOV13XBK[0x4];

	enum class ObjectFlags : int32_t
	{
		None = 0,
		Native = 1 << 25,
		Async = 1 << 26,
		AsyncLoading = 1 << 27,
		Unreachable = 1 << 28,
		PendingKill = 1 << 29,
		RootSet = 1 << 30,
		NoStrongReference = 1 << 31
	};
};

class TUObjectArray
{
	enum
	{
		NumElementsPerChunk = 64 * 1024,
	};
public:
	inline int32_t Num() const
	{
		return NumElements;
	}
	inline int32_t Max() const
	{
		return MaxElements;
	}
	inline bool IsValidIndex(int32_t Index) const
	{
		return Index < Num() && Index >= 0;
	}
	inline FUObjectItem* GetObjectPtr(int32_t Index) const
	{
		const int32_t ChunkIndex = Index / NumElementsPerChunk;
		const int32_t WithinChunkIndex = Index % NumElementsPerChunk;
		if (!IsValidIndex(Index)) return nullptr;
		if (ChunkIndex > NumChunks) return nullptr;
		if (Index > MaxElements) return nullptr;
		FUObjectItem* Chunk = Objects[ChunkIndex];
		if (!Chunk) return nullptr;
		return Chunk + WithinChunkIndex;
	}
	inline UObject* GetByIndex(int32_t index) const
	{
		FUObjectItem* ItemPtr = GetObjectPtr(index);
		if (!ItemPtr) return nullptr;

		return (*ItemPtr).Object;
	}
	inline FUObjectItem* GetItemByIndex(int32_t index) const
	{
		FUObjectItem* ItemPtr = GetObjectPtr(index);
		if (!ItemPtr) return nullptr;
		return ItemPtr;
	}
private:
	PAD(0x10);
	FUObjectItem** Objects;
	FUObjectItem* PreAllocatedObjects;
	int32_t MaxElements;
	int32_t NumElements;
	int32_t MaxChunks;
	int32_t NumChunks;
};

class UObject
{
public:
	static inline TUObjectArray* GObjects = nullptr;												   // 0x0000
	void* VfTable;                                                   // 0x0000
	int32_t                                             Flags;                                                     // 0x0008
	int32_t                                             InternalIndex;                                             // 0x000C
	UClass* Class;                                                     // 0x0010
	FName                                               Name;                                                      // 0x0018
	UObject* Outer;                                                     // 0x0020

	static inline TUObjectArray& GetGlobalObjects()
	{
		return *GObjects;
	}

	std::string get_name() const;
	std::string get_full_name() const;
	bool instance_of(UClass* cmp) const;

	template<typename T>
	static T* find_object(const std::string& name)
	{
		for (int i = 0; i < GetGlobalObjects().Num(); ++i)
		{
			auto object = GetGlobalObjects().GetByIndex(i);

			if (object == nullptr)
				continue;

			if (object->get_full_name() == name)
				return (T*)(object);
		}
		return nullptr;
	}

	template<typename T>
	static T* find_object()
	{
		auto v = T::StaticClass();
		for (int i = 0; i < UObject::GetGlobalObjects().Num(); ++i)
		{
			auto object = UObject::GetGlobalObjects().GetByIndex(i);
			if (object == nullptr)
			{
				continue;
			}
			if (object->instance_of(v))
			{
				return static_cast<T*>(object);
			}
		}
		return nullptr;
	}

	template<typename T>
	static std::vector<T*> find_objects(const std::string& name)
	{
		std::vector<T*> ret;
		for (int i = 0; i < GetGlobalObjects().Num(); ++i)
		{
			auto object = GetGlobalObjects().GetByIndex(i);
			if (object == nullptr)
			{
				continue;
			}
			if (object->get_full_name() == name)
			{
				ret.push_back(static_cast<T*>(object));
			}
		}
		return ret;
	}

	template<typename T>
	static std::vector<T*> find_objects()
	{
		std::vector<T*> ret;
		auto v = T::StaticClass();
		for (int i = 0; i < UObject::GetGlobalObjects().Num(); ++i)
		{
			auto object = UObject::GetGlobalObjects().GetByIndex(i);
			if (object == nullptr)
			{
				continue;
			}
			if (object->instance_of(v))
			{
				ret.push_back(static_cast<T*>(object));
			}
		}
		return ret;
	}
	static UClass* find_class(const std::string& name)
	{
		return find_object<UClass>(name);
	}

	template<typename T>
	static T* GetObjectCasted(std::size_t index)
	{
		return static_cast<T*>(GetGlobalObjects().GetByIndex(index));
	}

	static UClass* StaticClass()
	{
		static auto ptr = UObject::find_class("Class CoreUObject.Object");
		return ptr;
	}
};

class UField : public UObject
{
public:
	class UField* Next;                                        // 0x0028

	static UClass* StaticClass()
	{
		static auto ptr = UObject::find_class("Class CoreUObject.Field");
		return ptr;
	}
};

class UProperty : public UField
{
public:
	unsigned char                                      UnknownData_URFE[0x40];

	static UClass* StaticClass()
	{
		static auto ptr = UObject::find_class("Class CoreUObject.Property");
		return ptr;
	}
};

class UStruct : public UField
{
public:
	class UStruct* SuperField;                                                // 0x0030
	class UField* Children;                                                  // 0x0038
	int32_t                                             PropertySize;                                              // 0x0040
	PAD(0x4);
	TArray<unsigned char>								Script;													   // 0x0048
	int32_t                                             MinAlignment;                                              // 0x0058
	PAD(0x4);
	class UProperty* PropertyLink;                                              // 0x0060
	class UProperty* RefLink;                                                   // 0x0068
	class UProperty* DestructorLink;                                            // 0x0070
	class UProperty* PostConstructLink;                                         // 0x0078
	TArray<UObject*>                                    ScriptAndPropertyObjectReferences;                         // 0x0080

	static UClass* StaticClass()
	{
		static auto ptr = UObject::find_class("Class CoreUObject.Struct");
		return ptr;
	}
};

class UFunction : public UStruct
{
public:
	uint32_t										   FunctionFlags;                                             // 0x0090
	uint16_t                                           RepOffset;                                                 // 0x0094
	PAD(0x2);
	uint16_t                                           ParmsSize;                                                 // 0x0098
	uint16_t                                           ReturnValueOffset;                                         // 0x009A
	uint16_t                                           RPCId;                                                     // 0x009C
	uint16_t                                           RPCResponseId;                                             // 0x009E
	class UProperty* FirstPropertyToInit;                                       // 0x00A0
	void* Func;                                                      // 0x00A8

	static UClass* StaticClass()
	{
		static auto ptr = UObject::find_class("Class CoreUObject.Function");
		return ptr;
	}
};


class UClass : public UStruct
{
public:
	unsigned char                                      UnknownData_BH42[0xF8];

	static UClass* StaticClass()
	{
		static auto ptr = UObject::find_class("Class CoreUObject.Class");
		return ptr;
	}
};

struct FString : public TArray<wchar_t>
{
	FString() = default;
	explicit FString(const wchar_t* other)
	{
		Max = Count = *other ? std::wcslen(other) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	};
	inline bool IsValid() const
	{
		return Data != nullptr;
	}
	inline const wchar_t* cw_str() const
	{
		return Data;
	}
	inline const char* c_str() const
	{
		return (const char*)Data;
	}
	std::string ToString() const
	{
		size_t length = std::wcslen(Data);
		std::string str(length, '\0');
		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
	std::wstring ToWString() const
	{
		std::wstring str(Data);
		return str;
	}
	int Multi(char* name, int size) const
	{
		return WideCharToMultiByte(CP_UTF8, 0, Data, Count, name, size, nullptr, nullptr) - 1;
	}
};

template<typename T>
struct TTransArray : public TArray<T>
{
	UObject* Owner;
};

struct FKey
{
	struct FName								       KeyName;															// 0x0000
	unsigned char                                      UnknownData_QTRK[0x10] = {};										// 0x0010
	FKey() {};
	FKey(const char* InName) : KeyName(FName(InName)) {}
};

inline void process_event(void* obj, UFunction* function, void* parms)
{
	auto vtable = *reinterpret_cast<void***>(obj);
	reinterpret_cast<void(*)(void*, UFunction*, void*)>(vtable[72])(obj, function, parms);
}