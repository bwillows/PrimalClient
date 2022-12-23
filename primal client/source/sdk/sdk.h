#include "../includes.hpp"

#include "ue4/ue4_extra.h"
#include "ue4/UE4.h"

/* class forward declarations */
class UFont;
class AActor;
class ULocalPlayer;
class APlayerState;
class APlayerController;
class AController;
class UPlayer;
class APawn;
class USceneComponent;
class APlayerCameraManager;
class APrimalCharacter;
class AShooterWeapon;
class APrimalDinoCharacter;

inline UFont* get_font()
{
	static UFont* font;
	if (!font)
		font = UObject::find_object<UFont>(skCrypt("Font OpenSansRegular12.OpenSansRegular12").decrypt());

	return font;
}

class APlayerCameraManager
{
public:
	PAD(0x470);
	USceneComponent* TransformComponent;
	PAD(0x8);
	float DefaultFOV; // 0x0480(0x0004)
	PAD(0x4C);

	FRotator get_camera_rotation()
	{
		static auto fn = UObject::find_object<UFunction>(skCrypt("Function Engine.PlayerCameraManager.GetCameraRotation").decrypt());
		struct {
			FRotator ReturnValue;
		} Params;
		process_event(this, fn, &Params);
		return Params.ReturnValue;
	}
	FVector get_camera_location()
	{
		static auto fn = UObject::find_object<UFunction>(skCrypt("Function Engine.PlayerCameraManager.GetCameraLocation").decrypt());
		struct {
			FVector ReturnValue;
		} Params;
		process_event(this, fn, &Params);
		return Params.ReturnValue;
	}
};

class USkeletalMeshComponent
{
	static UClass* static_class()
	{
		static auto ptr = UObject::find_class(skCrypt("Class Engine.SkeletalMeshComponent").decrypt());
		return ptr;
	}
	FName get_bone_name(int BoneIndex)
	{
		static UFunction* fn = nullptr;
		if (!fn)
			fn = UObject::find_object<UFunction>(skCrypt("Function Engine.SkinnedMeshComponent.GetBoneName").decrypt());
		struct {
			int BoneIndex;
			FName ReturnValue;
		} Params;
		Params.BoneIndex = BoneIndex;
		process_event(this, fn, &Params);
		return Params.ReturnValue;
	}
};

class AActor : public UObject {
public:
	FVector k2_get_actor_location()
	{
		static auto fn = UObject::find_object<UFunction>(skCrypt("Function Engine.Actor.K2_GetActorLocation").decrypt());
		struct
		{
			FVector ReturnVector;
		} Params;
		process_event(this, fn, &Params);
		return Params.ReturnVector;
	}
	inline bool is_dino()
	{
		static UClass* obj;
		if (!obj)
			obj = UObject::find_class(skCrypt("Class ShooterGame.PrimalDinoCharacter").decrypt());
		return instance_of(obj);
	}
	inline bool is_player()
	{
		static UClass* obj;
		if (!obj)
			obj = UObject::find_class(skCrypt("Class ShooterGame.ShooterCharacter").decrypt());
		return instance_of(obj);
	}
	bool is_tamed()
	{
		static auto fn = UObject::find_object<UFunction>(skCrypt("Function ShooterGame.PrimalDinoCharacter.BPIsTamed").decrypt());
		struct {
			bool ReturnValue;
		} Params;
		process_event(this, fn, &Params);
		return Params.ReturnValue;
	}
	bool is_local_player()
	{
		static auto fn = UObject::find_object<UFunction>(skCrypt("Function ShooterGame.PrimalCharacter.IsOwningClient").decrypt());
		struct {
			bool ReturnValue;
		} Params;
		process_event(this, fn, &Params);
		return Params.ReturnValue;
	}
	bool is_conscious()
	{
		static auto fn = UObject::find_object<UFunction>(skCrypt("Function ShooterGame.PrimalCharacter.BPIsConscious").decrypt());
		struct {
			bool ReturnValue;
		} Params;
		process_event(this, fn, &Params);
		return Params.ReturnValue;
	}
	bool is_primal_char_friendly(APrimalCharacter* primalChar)
	{
		static auto fn = UObject::find_object<UFunction>(skCrypt("Function ShooterGame.PrimalCharacter.IsPrimalCharFriendly").decrypt());

		struct {
			class APrimalCharacter* primalChar;
			bool ReturnValue;
		}params;
		params.primalChar = primalChar;

		process_event(this, fn, &params);
		return params.ReturnValue;
	}
	bool is_dead()
	{
		static auto fn = UObject::find_object<UFunction>(skCrypt("Function Engine.Actor.IsDead").decrypt());
		struct {
			bool ReturnValue;
		} Params;
		process_event(this, fn, &Params);
		return Params.ReturnValue;
	}
	bool is_structure()
	{
		static auto fn = UObject::find_object<UFunction>(skCrypt("Function Engine.Actor.IsPrimalStructure").decrypt());

		struct {
			bool                           ReturnValue;
		}params;

		process_event(this, fn, &params);

		return params.ReturnValue;
	}
	bool is_item_container()
	{
		static auto fn = UObject::find_object<UFunction>(skCrypt("Function Engine.Actor.IsPrimalStructureItemContainer").decrypt());
		struct {
			bool ReturnValue;
		} Params;
		process_event(this, fn, &Params);
		return Params.ReturnValue;
	}
	struct APrimalDinoCharacter* get_based_or_seating_on_dino()
	{
		static UFunction* fn = nullptr;
		if (!fn)
			fn = UObject::find_object<UFunction>(skCrypt("Function ShooterGame.PrimalCharacter.GetBasedOrSeatingOnDino").decrypt());

		struct {
			struct APrimalDinoCharacter* ReturnValue;
		} Params;

		process_event(this, fn, &Params);
		return Params.ReturnValue;
	}
};

class APawn : public AActor {
public:
	PAD(0x18);
	UClass* AIControllerClass;                                         // 0x0488
	class APlayerState* PlayerState;                                               // 0x0490
	static UClass* static_class()
	{
		static auto ptr = UObject::find_class(skCrypt("Class Engine.Pawn").decrypt());
		return ptr;
	}
};

class APlayerController
{
public:
	PAD(0x4D0);
	APawn* AcknowledgedPawn;
	PAD(0x18);
	class APlayerCameraManager* PlayerCameraManager;
	PAD(0x534);
	float	MaxUseDistance;
};

class UPlayer {
public:
	PAD(0x30);
	APlayerController* PlayerController;											// 0x0030
};

class ULocalPlayer : public UPlayer {

};

class ULevelBase
{
public:
	PAD(0x88);
	TTransArray<AActor*> Actors;				// 0x0088
};

class ULevel : public ULevelBase
{

};

class UGameInstance
{
public:
	PAD(0x38);
	TArray<ULocalPlayer*>                        LocalPlayers;						// 0x0038
};

class UWorld : public UObject
{
public:
	static inline UWorld* GWorld = nullptr;
	PAD(0xD0);
	ULevel* PersistentLevel;          // 0x00F8
	PAD(0x28);
	class AGameState* GameState;				  // 0x0128
	PAD(0x160);
	UGameInstance* OwningGameInstance;       // 0x0290
};

struct AGameState
{
	PAD(0x4c0);
	TArray<APlayerState*> PlayerStates;
};
class UCanvas : public UObject
{
public:
	void k2_draw_text(UFont* RenderFont, const FString& RenderText, const FVector2D& ScreenPosition, const FLinearColor& RenderColor, float Kerning, const FLinearColor& ShadowColor, const FVector2D& ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, const FLinearColor& OutlineColor)
	{
		static UFunction* fn = nullptr;
		if (!fn)
			fn = UObject::find_object<UFunction>(skCrypt("Function Engine.Canvas.K2_DrawText").decrypt());

		struct
		{
			UFont* RenderFont;
			FString RenderText;
			FVector2D ScreenPosition;
			FLinearColor RenderColor;
			float Kerning;
			FLinearColor ShadowColor;
			FVector2D ShadowOffset;
			bool bCentreX;
			bool bCentreY;
			bool bOutlined;
			FLinearColor OutlineColor;
		} params;

		params.RenderFont = RenderFont;
		params.RenderText = RenderText;
		params.ScreenPosition = ScreenPosition;
		params.RenderColor = RenderColor;
		params.Kerning = Kerning;
		params.ShadowColor = ShadowColor;
		params.ShadowOffset = ShadowOffset;
		params.bCentreX = bCentreX;
		params.bCentreY = bCentreY;
		params.bOutlined = bOutlined;
		params.OutlineColor = OutlineColor;

		process_event(this, fn, &params);
	}
	void k2_draw_line(const FVector2D& ScreenPositionA, const FVector2D& ScreenPositionB, float Thickness, const FLinearColor& RenderColor)
	{
		static UFunction* fn = nullptr;
		if (!fn)
			fn = UObject::find_object<UFunction>(skCrypt("Function Engine.Canvas.K2_DrawLine").decrypt());

		struct
		{
			FVector2D ScreenPositionA;
			FVector2D ScreenPositionB;
			float Thickness;
			FLinearColor RenderColor;
		} params;

		params.ScreenPositionA = ScreenPositionA;
		params.ScreenPositionB = ScreenPositionB;
		params.Thickness = Thickness;
		params.RenderColor = RenderColor;

		process_event(this, fn, &params);
	}
};
class AShooterCharacter
{
public:

	static UClass* static_class()
	{
		static auto ptr = UObject::find_class(skCrypt("Class ShooterGame.ShooterCharacter").decrypt());
		return ptr;
	}

	AShooterWeapon* get_weapon()
	{
		static auto fn = UObject::find_object<UFunction>(skCrypt("Function ShooterGame.ShooterCharacter.GetWeapon").decrypt());
		struct {
			AShooterWeapon* ReturnValue;
		} Params;
		process_event(this, fn, &Params);
		return Params.ReturnValue;
	}
	APrimalDinoCharacter* get_riding_dino()
	{
		static UFunction* fn = nullptr;
		if (!fn)
			fn = UObject::find_object<UFunction>(skCrypt("Function ShooterGame.ShooterCharacter.GetRidingDino").decrypt());

		struct AShooterCharacter_GetRidingDino_Params {
			class APrimalDinoCharacter* ReturnValue;
		} params{};

		process_event(this, fn, &params);

		return params.ReturnValue;
	}
};
typedef struct _D3DXMATRIX
{
	union
	{
		struct
		{
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		};
		float m[4][4];
	};
} D3DXMATRIX;
inline void prot_memcpy(void* dst, const void* src, size_t size) {
	DWORD protect;
	if (!VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &protect))
		return;
	memcpy(dst, src, size);
	VirtualProtect(dst, size, protect, &protect);
}
inline float max_arr(float arr[], int n)
{
	int i;
	float max = arr[0];
	for (i = 1; i < n; i++)
	{
		if (arr[i] > max)
		{
			max = arr[i];
		}
		return max;
	}
}
inline float min_arr(float arr[], int n)
{
	int i;
	float min = arr[0];
	for (i = 1; i < n; i++)
	{
		if (arr[i] < min)
		{
			min = arr[i];
		}
		return min;
	}
}
inline D3DXMATRIX matrix(FRotator Rotation, FVector Origin = FVector(0, 0, 0))
{
	float radPitch = (Rotation.Pitch * M_PI / 180.f);
	float radYaw = (Rotation.Yaw * M_PI / 180.f);
	float radRoll = (Rotation.Roll * M_PI / 180.f);
	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);
	D3DXMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;
	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;
	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;
	matrix.m[3][0] = Origin.X;
	matrix.m[3][1] = Origin.Y;
	matrix.m[3][2] = Origin.Z;
	matrix.m[3][3] = 1.f;
	return matrix;
}
inline bool w2s(FVector world_location, FVector2D& screen_location, APlayerController* player_controller, RECT window_rect)
{
	if (world_location == 0.f) return false;

	FVector location = { 0, 0, 0 };
	FRotator rotation = { 0, 0, 0 };
	location = player_controller->PlayerCameraManager->get_camera_location();
	rotation = player_controller->PlayerCameraManager->get_camera_rotation();

	D3DXMATRIX temp_matrix = matrix(rotation);
	FVector v_axis_x, v_axis_y, v_axis_z;
	v_axis_x = FVector(temp_matrix.m[0][0], temp_matrix.m[0][1], temp_matrix.m[0][2]);
	v_axis_y = FVector(temp_matrix.m[1][0], temp_matrix.m[1][1], temp_matrix.m[1][2]);
	v_axis_z = FVector(temp_matrix.m[2][0], temp_matrix.m[2][1], temp_matrix.m[2][2]);
	float w = temp_matrix.m[3][0] * world_location.X + temp_matrix.m[3][1] * world_location.Y + temp_matrix.m[3][2] * world_location.Z + temp_matrix.m[3][3];
	if (w < 0.01) return false;
	FVector v_delta = world_location - location;
	FVector v_transformed = FVector(v_delta.Dot(v_axis_y), v_delta.Dot(v_axis_z), v_delta.Dot(v_axis_x));
	if (v_transformed.Z < 1.0f) v_transformed.Z = 1.f;
	float fov_angle = player_controller->PlayerCameraManager->DefaultFOV;
	float screen_center_x = (window_rect.right - window_rect.left) / 2;
	float screen_center_y = (window_rect.bottom - window_rect.top) / 2;
	screen_location.X = (screen_center_x + v_transformed.X * (screen_center_x / (float)tan(fov_angle * M_PI / 360)) / v_transformed.Z);
	screen_location.Y = (screen_center_y - v_transformed.Y * (screen_center_x / (float)tan(fov_angle * M_PI / 360)) / v_transformed.Z);
	if (screen_location.X < -50 || screen_location.X >((window_rect.right - window_rect.left) + 250)) return false;
	if (screen_location.Y < -50 || screen_location.Y >(window_rect.bottom - window_rect.top)) return false;
	return true;
}
inline uintptr_t pattern_scan(uintptr_t moduleAdress, const char* signature)
{
	static auto patternToByte = [](const char* pattern)
	{
		auto       bytes = std::vector<int>{};
		const auto start = const_cast<char*>(pattern);
		const auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current)
		{
			if (*current == '?')
			{
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else { bytes.push_back(strtoul(current, &current, 16)); }
		}
		return bytes;
	};
	const auto dosHeader = (PIMAGE_DOS_HEADER)moduleAdress;
	const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)moduleAdress + dosHeader->e_lfanew);
	const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto       patternBytes = patternToByte(signature);
	const auto scanBytes = reinterpret_cast<std::uint8_t*>(moduleAdress);
	const auto s = patternBytes.size();
	const auto d = patternBytes.data();
	for (auto i = 0ul; i < sizeOfImage - s; ++i)
	{
		bool found = true;
		for (auto j = 0ul; j < s; ++j)
		{
			if (scanBytes[i + j] != d[j] && d[j] != -1)
			{
				found = false;
				break;
			}
		}
		if (found) { return reinterpret_cast<uintptr_t>(&scanBytes[i]); }
	}
	return NULL;
}