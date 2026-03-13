#pragma once


#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <DirectXMath.h>
#include <iostream>
#include "../overlay/imgui/imgui.h"
using namespace std;
#define pi 3.141592654f

class Vector2 final {
public:
    float x, y;

    Vector2(const float x, const float y) : x(x), y(y) { }
    constexpr Vector2( ) : x(0.0f), y(0.0f){ };
};
class Vector3 final
{
public:

	float x, y, z;

	Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator * (const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
	Vector3 operator / (const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }
	bool operator == (const Vector3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	Vector3& operator += (const Vector3& rhs) { return *this = *this + rhs; }
	Vector3& operator -= (const Vector3& rhs) { return *this = *this - rhs; }
	Vector3& operator *= (const float& rhs) { return *this = *this * rhs; }
	Vector3& operator /= (const float& rhs) { return *this = *this / rhs; }
	float Length() const { return sqrt(x * x + y * y + z * z); }
	Vector3 Normalize() const { return *this * (1 / Length()); }
	float Distance(const Vector3& rhs) const { return (*this - rhs).Length(); }
	void Invert() { *this *= -1; }
	static Vector3 FromM128(__m128 in) { return Vector3(in.m128_f32[0], in.m128_f32[1], in.m128_f32[2]); }
};

class Vector4
{
public:
	float x, y, z, w;
};

inline ImVec2 GetWindowSize()
{
	RECT rect;
	HWND hwnd = GetActiveWindow();
	if (GetWindowRect(hwnd, &rect))
	{
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		return ImVec2(width, height);

	}
	return ImVec2(0, 0);
}
const int ROWS = 3;
const int COLS = 3;


struct Matrix
{
	float m[4][4];

	Matrix() {}

	Matrix(float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44)
	{
		m[0][0] = _11; m[0][1] = _12; m[0][2] = _13; m[0][3] = _14;
		m[1][0] = _21; m[1][1] = _22; m[1][2] = _23; m[1][3] = _24;
		m[2][0] = _31; m[2][1] = _32; m[2][2] = _33; m[2][3] = _34;
		m[3][0] = _41; m[3][1] = _42; m[3][2] = _43; m[3][3] = _44;
	}
	   // Functions for manipulating the matrix
    void Identity()
    {
        memset(m, 0, sizeof(Matrix));
        m[0][0] = 1.0f;
        m[1][1] = 1.0f;
        m[2][2] = 1.0f;
        m[3][3] = 1.0f;
    }


};
void transposeMatrix(Matrix& matrix) {
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            std::swap(matrix.m[i][j], matrix.m[j][i]);
        }
    }
}


class Vector3Fix {
public:
	Vector3Fix() = default;

	Vector3Fix(float x, float y, float z) :
		x(x), y(y), z(z)
	{}

	// Conversion de Vector3Fix en Vector3
	operator Vector3() const {
		return Vector3(x, y, z);
	}

public:
	float x{};
private:
	char m_padding1[0x04]{};
public:
	float y{};
private:
	char m_padding2[0x04]{};
public:
	float z{};
private:
	char m_padding3[0x04]{};
};

#define M_PI           3.14159265358979323846 

namespace FiveM {

        using namespace FiveM;
        const char*(__fastcall* getpname)(void*);
        inline DWORD64 flypatt;
        inline uint64_t World, ReplayInterface, W2S, BonePos, Camera, Waypoint, BoneMask, ViewPort, AimCPed;
        inline bool IsOnFiveM;
        typedef __int64(__fastcall* HandleBullet_t)(__int64 BulletPtr, class CWeaponInfo* CWeaponInfo, Vector3 StartPos, Vector3 EndPos, float Range, int weaponHash, char a7, char a8);
        inline HandleBullet_t HandleBullet = nullptr;
        inline DWORD Armor, EntityType, WeaponManager, m_frame_flags, freeze, PlayerInfo, m_player_id, Recoil, Spread, AmmoType, WeaponName, AmmoExplosiveType, Range, ReloadMultiplier, VehiculeReloadMultiplier, IsInAVehicule, EngineHealth, BodyHealth, HealthOfVehicle;
        // inline const char* (__fastcall* GetPlayerNamess)(void*);
        inline bool FiveM_2802;   
		inline bool FiveM_b2944;
		//inline std::vector<std::string> Friend;

        inline ImVec2 WindowSize = ImVec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
        using pizza_to_spaghetti_t = int32_t (*)(intptr_t pointer); // le truc pr convert un ped en handle
        // inline Vector3 WhereImAiming = Vector3(0, 0, 0);

        typedef BOOL(__fastcall* g_bone_mask_t)(DWORD64 CPed, __m128* virtualbonepos, unsigned int bone);
        inline g_bone_mask_t g_bone_mask = nullptr;
        // inline pizza_to_spaghetti_t pointer_to_handle;
        inline DWORD64 GetCamera( ) {
                if (Camera)
            return *(DWORD64*)(Camera);
        }

		DWORD64 GetCameraViewAngles()
		{
			if (Camera)
				return *(DWORD64*)(Camera + 0x0);
		}

       
	inline ImVec2 WorldToScreen(Vector3 pos) {
                HWND hwnd = GetForegroundWindow( );
                RECT rect;
                int width;
                int height;
                if (GetClientRect(hwnd, &rect)) {
            width = rect.right - rect.left;
            height = rect.bottom - rect.top;
                }
                uint64_t viewport = *(uint64_t*)(FiveM::ViewPort);
                D3DXMATRIX matrix = *(D3DXMATRIX*)(viewport + 0x24C);

                D3DXVECTOR3 screen_pos;

                D3DXMatrixTranspose(&matrix, &matrix);
                D3DXVECTOR4 vectorX = D3DXVECTOR4(matrix._21, matrix._22, matrix._23, matrix._24);
                D3DXVECTOR4 vectorY = D3DXVECTOR4(matrix._31, matrix._32, matrix._33, matrix._34);
                D3DXVECTOR4 vectorZ = D3DXVECTOR4(matrix._41, matrix._42, matrix._43, matrix._44);

                screen_pos.x = (vectorX.x * pos.x) + (vectorX.y * pos.y) + (vectorX.z * pos.z) + vectorX.w;
                screen_pos.y = (vectorY.x * pos.x) + (vectorY.y * pos.y) + (vectorY.z * pos.z) + vectorY.w;
                screen_pos.z = (vectorZ.x * pos.x) + (vectorZ.y * pos.y) + (vectorZ.z * pos.z) + vectorZ.w;
                if (screen_pos.z <= 0.01f)
            return ImVec2(0, 0);

                screen_pos.z = 1.0f / screen_pos.z;
                screen_pos.x *= screen_pos.z;
                screen_pos.y *= screen_pos.z;

                float xTmp = width / 2;
                float yTmp = height / 2;

                screen_pos.x += xTmp + (0.5f * screen_pos.x * width + 0.5f);
                screen_pos.y = yTmp - (0.5f * screen_pos.y * height + 0.5f);

                return ImVec2(screen_pos.x, screen_pos.y);
        }


 



  
	inline float pythag(ImVec2 src, ImVec2 dst)
	{
		return sqrtf(pow(src.x - dst.x, 2) + pow(src.y - dst.y, 2));
	}

	inline float pythagVec3(Vector3 src, Vector3 dst)
	{
		return sqrtf(pow(src.x - dst.x, 2) + pow(src.y - dst.y, 2) + pow(src.z - dst.z, 2));
	}
	enum class ePedBoneType
	{
		HEAD,
		L_FOOT,
		R_FOOT,
		L_ANKLE,
		R_ANKLE,
		L_HAND,
		R_HAND,
		NECK,
		ABDOMEN
	};
	/*// Basic
	inline Vector3 GetBonePos(const uint64_t cPed, const int32_t wMask)
	{
		__m128 tempVec4;
		reinterpret_cast<void* (__fastcall*)(uint64_t, __m128*, int32_t)>(BonePos)(cPed, &tempVec4, wMask);
		return Vector3::FromM128(tempVec4);
	}
	inline ImVec2 GetBonePosW2S(const uint64_t cPed, const int32_t wMask)
	{
		__m128 tempVec4;
		reinterpret_cast<void* (__fastcall*)(uint64_t, __m128*, int32_t)>(BonePos)(cPed, &tempVec4, wMask);
		return WorldToScreen(Vector3::FromM128(tempVec4));
	}
	//*/
	// Array
	inline Vector3 GetBoneArrayPos(const uint64_t cPed, int wMask)
	{

		//if (getversion( ) == "b2802") {
  //          if (wMask == 8)
  //              wMask = 0;
  //          else if (wMask == 0)
  //              wMask = 1;
  //          else if (wMask == 1)
  //              wMask = 2;
  //          else if (wMask == 2)
  //              wMask = 3;
  //          else if (wMask == 3)
  //              wMask = 4;
  //          else if (wMask == 4)
  //              wMask = 5;
  //          else if (wMask == 5)
  //              wMask = 6;
  //          else if (wMask == 6)
  //              wMask = 7;
  //          else if (wMask == 7)
  //              wMask = 8;
  //              }
		/*


				b2944 0x410
			b2802 0x400
			else 0x430
		*/
		//	D3DXVECTOR3 bone;
		//	D3DXMATRIX matrix = *(D3DXMATRIX*)(cPed + 0x60);
	 //       if (getversion( ) == "b2944")
		//	{
		//		 bone = *(D3DXVECTOR3*)(cPed + 0x410 + (wMask * 0x10));
		//	}
		//	else if (getversion() == "b2802")
		//	{
		//		bone = *(D3DXVECTOR3*)(cPed + 0x400 + (wMask * 0x10));
		//	}
		//	else
		//	{
		//		bone = *(D3DXVECTOR3*)(cPed + 0x430 + (wMask * 0x10));

		//	}
		//	D3DXVECTOR4 transform;
		//    D3DXVec3Transform(&transform, &bone, &matrix);

		//
		//	return Vector3(transform.x,transform.y,transform.z);
		//}
	}



	inline ImVec2 GetBoneArrayW2SPos(const uint64_t cPed,int wMask)
	{
		return WorldToScreen(GetBoneArrayPos(cPed,wMask));
	}
	//
	inline Vector3 GetBonePosMatrix(const uint64_t cPed, int bone)
	{
		
		return GetBoneArrayPos(cPed,bone);
	}
	inline ImVec2 GetBonePosMatrixW2S(const uint64_t cPed, int bone)
	{

	   return GetBoneArrayW2SPos(cPed,bone);
	}

}


//auto OutlinedText2(ImFont* font, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center) -> VOID {
//        ImDrawList* drawList = ImGui::GetBackgroundDrawList( );
//        drawList->PushTextureID(font->ContainerAtlas->TexID);
//
//        ImVec2 drawPos = pos;
//        float lineHeight = ImGui::GetTextLineHeightWithSpacing( );
//
//        for (const char* lineStart = text.c_str( ); lineStart < text.c_str( ) + text.size( );) {
//           const char* lineEnd = strchr(lineStart, '\n');
//           if (lineEnd == NULL)
//                        lineEnd = lineStart + strlen(lineStart);
//
//           ImVec2 textSize = font->CalcTextSizeA(size, FLT_MAX, 0.0f, lineStart, lineEnd);
//
//           if (center)
//                        drawPos.x = pos.x - textSize.x * 0.5f;
//
//           // Draw outline
//           drawList->AddText(font, size, ImVec2(drawPos.x - 1, drawPos.y - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 1)), lineStart, lineEnd);
//           drawList->AddText(font, size, ImVec2(drawPos.x + 1, drawPos.y - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 1)), lineStart, lineEnd);
//           drawList->AddText(font, size, ImVec2(drawPos.x - 1, drawPos.y + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 1)), lineStart, lineEnd);
//           drawList->AddText(font, size, ImVec2(drawPos.x + 1, drawPos.y + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 1)), lineStart, lineEnd);
//
//           // Draw text
//           drawList->AddText(font, size, drawPos, color, lineStart, lineEnd);
//
//           lineStart = lineEnd + 1;
//           drawPos.y += lineHeight;
//        }
//
//        drawList->PopTextureID( );
//}

auto DrawOutlinedText(ImFont* font, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center) -> VOID {

	ImGui::GetBackgroundDrawList()->PushTextureID(font->ContainerAtlas->TexID);

	//std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int index = 0;

/*	while (std::getline(stream, line))*/ {
		ImVec2 textSize = font->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		ImGui::GetBackgroundDrawList()->AddText(font, font->FontSize, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * index)), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
		ImGui::GetBackgroundDrawList()->AddText(font, font->FontSize, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());

		if (center) {
			ImGui::GetBackgroundDrawList()->AddText(font, font->FontSize, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * index)), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetBackgroundDrawList()->AddText(font, font->FontSize, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());

		}
		else {
			ImGui::GetBackgroundDrawList()->AddText(font, font->FontSize + 1, ImVec2((pos.x) + 1, (pos.y + textSize.y * index)), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetBackgroundDrawList()->AddText(font, font->FontSize, ImVec2(pos.x, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
		}
	}
}

void FillRect(float x, float y, float w, float h, ImColor color, int rounding = 0);
inline bool IsOnScreen(ImVec2 coords)
{
	if (coords.x < 0.1f || coords.y < 0.1 || coords.x > ImGui::GetIO().DisplaySize.x || coords.y > ImGui::GetIO().DisplaySize.y)
	{
		return false;
	}
	else {
		return true;
	}
}
void DrawBorder(float x, float y, float w, float h, float thickness, ImColor BorderColor) {

        FillRect(x, y, w, thickness, BorderColor);

        FillRect(x, y, thickness, h, BorderColor);

        FillRect((x + w), y, thickness, h, BorderColor);

        FillRect(x, y + h, w + thickness, thickness, BorderColor);
}
void FillRect(float x, float y, float w, float h, ImColor color, int rounding) {
        if (IsOnScreen({x, y})) {
                ImGui::GetBackgroundDrawList( )->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, rounding);
        }
}

void Rect(float x, float y, float w, float h, ImColor color, int rounding) {
        if (IsOnScreen({x, y})) {
                ImGui::GetBackgroundDrawList( )->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, rounding);
        }
}
inline void DrawHealthBar(ImVec2 pos, ImVec2 dim, ImColor col) { 
        if (IsOnScreen(pos)) {

                ImGui::GetBackgroundDrawList( )->AddLine(pos, ImVec2(pos.x, pos.y - dim.y), col, dim.x);
        }
}
inline void DrawHealthBarhorizontal(ImVec2 pos, ImVec2 pos2, ImVec2 dim, ImColor col)
{
	if (IsOnScreen(pos))
	{

		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(pos.x + dim.y, pos.y), ImVec2(pos2.x, pos.y), col, dim.x);
	}
}
inline void DrawHealthBarFilling(ImVec2 pos, ImVec2 dim, ImColor col)
{
	if (IsOnScreen(pos))
	{

		ImGui::GetBackgroundDrawList()->AddRect(ImVec2(pos.x - 1, pos.y - 1), ImVec2(pos.x + 1, (pos.y - dim.y) + 1), col);
	}
}



inline const char* get_weapon_name(DWORD hash)
{
	//removed xoring, cba to do properly.
	const char* dagger = ("Dagger");
	const char* bat = ("Bat");
	const char* bottle = ("Bottle");
	const char* crowbar = ("Crow Bar");
	const char* unarmed = ("None");
	const char* flashlight = ("Flash Light");
	const char* golfclub = ("Golf club");
	const char* hammer = ("Hammer");
	const char* hatchet = ("Hatchet");
	const char* knuckle = ("Knuckle");
	const char* knife = ("Knife");
	const char* machete = ("Machete");
	const char* switchblade = ("Switch Blade");
	const char* nightstick = ("Night Stick");
	const char* wrench = ("Wrench");
	const char* battleaxe = ("Battle Axe");
	const char* poolcue = ("Pool Cue");
	const char* pistol = ("Pistol");
	const char* pistolmk2 = ("Pistol MK2");
	const char* combatpistol = ("Combat Pistol");
	const char* appistol = ("AP Pistol");
	const char* stungun = ("Stungun");
	const char* pistol50 = ("Pistol 50");
	const char* snspistol = ("SNS PISTOL");
	const char* snspistolmk2 = ("SNS Pistol MK2");
	const char* heavypistol = ("Heavy Pistol");
	const char* vintagepistol = ("Vintage Pisol");
	const char* flaregun = ("Flare Gun");
	const char* marksmanpistol = ("marksmanpistol");
	const char* revolver = ("Revolver");
	const char* revolvermk2 = ("Revolver MK2");
	static auto doubleaction = ("Double Action");
	static auto microsmg = ("Micro Smg");
	static auto smg = ("Smg");
	static auto tacticalrifle = ("Tactical Rifle");
	static auto smgmk2 = ("Smg MK2");
	static auto assaultsmg = ("Assault Smg");
	static auto combatpdw = ("Combat PDW");
	static auto machinepistol = ("Machine Pistol");
	static auto minismg = ("Mini Smg");
	static auto pumpshotgun = ("Pump Shotgun");
	static auto pumpshotgun_mk2 = ("Pump Shotgun MK2");
	static auto sawnoffshotgun = ("Sawnoff Shotgun");
	static auto assaultshotgun = ("Sssault Shotgun");
	static auto bullpupshotgun = ("Bullpup Shotgun");
	static auto musket = ("Musket");
	static auto heavyshotgun = ("Heavy Shotgun");
	static auto dbshotgun = ("DB Shotgun");
	static auto autoshotgun = ("Auto Shotgun");
	static auto assaultrifle = ("Assault Rifle");
	static auto assaultrifle_mk2 = ("Assault Rifle MK2");
	static auto carbinerifle = ("Carbine Rifle");
	static auto carbinerifle_mk2 = ("Carbine Rifle MK2");
	static auto advancedrifle = ("Advanced Rifle");
	static auto specialcarbine = ("Special Carbine");
	static auto specialcarbine_mk2 = ("Special Carbine MK2");
	static auto bullpuprifle = ("Bullpup Rifle");
	static auto bullpuprifle_mk2 = ("Bullpup Rifle MK2");
	static auto compactrifle = ("Compact Rifle");
	static auto machine_gun = ("Machine Gun");
	static auto combatmg = ("Combat MG");
	static auto combatmg_mk2 = ("Combat MG MK2");
	static auto gusenberg = ("GUSENBERG");
	static auto sniperrifle = ("Sniper Rifle");
	static auto heavysniper = ("AWP");
	static auto heavysniper_mk2 = ("AWP MK2");
	static auto marksmanrifle = ("Marksman Rifle");
	static auto marksmanrifle_mk2 = ("Marksman Rifle MK2");
	static auto rpg = ("RPG");
	static auto grenadelauncher = ("Grenade Launcher");
	static auto grenadelauncher_smoke = ("Grenade Launcher Smoke");
	static auto minigun = ("MiniGun");
	static auto firework = ("FireWork");
	static auto railgun = ("RailGun");
	static auto hominglauncher = ("Homing Launcher");
	static auto compactlauncher = ("Compact Launcher");
	static auto grenade = ("Grenade");
	static auto bzgas = ("BZGAS");
	static auto smokegrenade = ("Smoke Grenade");
	static auto flare = ("Flare");
	static auto molotov = ("Molotov");
	static auto stickybomb = ("Sticky BOMB");
	static auto proxmine = ("Prox Mine");
	static auto snowball = ("SnowBall");
	static auto pipebomb = ("Pipe Bomb");
	static auto ball = ("Ball");
	static auto petrolcan = ("Petrol Can");
	static auto fireextinguisher = ("Fire Extinguisher");
	static auto parachute = ("Parachute");

	switch (hash) {
	case 0xD1D5F52B:
		return tacticalrifle;
		break;
	case 0x92A27487:
		return dagger;
		break;
	case 0x958A4A8F:
		return bat;
		break;
	case 0xF9E6AA4B:
		return bottle;
		break;
	case 0x84BD7BFD:
		return crowbar;
		break;
	case 0xA2719263:
		return unarmed;
		break;
	case 0x8BB05FD7:
		return flashlight;
		break;
	case 0x440E4788:
		return golfclub;
		break;
	case 0x4E875F73:
		return hammer;
		break;
	case 0xF9DCBF2D:
		return hatchet;
		break;
	case 0xD8DF3C3C:
		return knuckle;
		break;
	case 0x99B507EA:
		return knife;
		break;
	case 0xDD5DF8D9:
		return machete;
		break;
	case 0xDFE37640:
		return switchblade;
		break;
	case 0x678B81B1:
		return nightstick;
		break;
	case 0x19044EE0:
		return wrench;
		break;
	case 0xCD274149:
		return battleaxe;
		break;
	case 0x94117305:
		return poolcue;
		break;
	case 0x1B06D571:
		return pistol;
		break;
	case 0xBFE256D4:
		return pistolmk2;
		break;
	case 0x5EF9FEC4:
		return combatpistol;
		break;
	case 0x22D8FE39:
		return appistol;
		break;
	case 0x3656C8C1:
		return stungun;
		break;
	case 0x99AEEB3B:
		return pistol50;
		break;
	case 0xBFD21232:
		return snspistol;
		break;
	case 0x88374054:
		return snspistolmk2;
		break;
	case 0xD205520E:
		return heavypistol;
		break;
	case 0x83839C4:
		return vintagepistol;
		break;
	case 0x47757124:
		return flaregun;
		break;
	case 0xDC4DB296:
		return marksmanpistol;
		break;
	case 0xC1B3C3D1:
		return revolver;
		break;
	case 0xCB96392F:
		return revolvermk2;
		break;
	case 0x97EA20B8:
		return doubleaction;
		break;
	case 0x13532244:
		return microsmg;
		break;
	case 0x2BE6766B:
		return smg;
		break;
	case 0x78A97CD0:
		return smgmk2;
		break;
	case 0xEFE7E2DF:
		return assaultsmg;
		break;
	case 0xA3D4D34:
		return combatpdw;
		break;
	case 0xDB1AA450:
		return machinepistol;
		break;
	case 0xBD248B55:
		return minismg;
		break;
	case 0x1D073A89:
		return pumpshotgun;
		break;
	case 0x555AF99A:
		return pumpshotgun_mk2;
		break;
	case 0x7846A318:
		return sawnoffshotgun;
		break;
	case 0xE284C527:
		return assaultshotgun;
		break;
	case 0x9D61E50F:
		return bullpupshotgun;
		break;
	case 0xA89CB99E:
		return musket;
		break;
	case 0x3AABBBAA:
		return heavyshotgun;
		break;
	case 0xEF951FBB:
		return dbshotgun;
		break;
	case 0x12E82D3D:
		return autoshotgun;
		break;
	case 0xBFEFFF6D:
		return assaultrifle;
		break;
	case 0x394F415C:
		return assaultrifle_mk2;
		break;
	case 0x83BF0278:
		return carbinerifle;
		break;
	case 0xFAD1F1C9:
		return carbinerifle_mk2;
		break;
	case 0xAF113F99:
		return advancedrifle;
		break;
	case 0xC0A3098D:
		return specialcarbine;
		break;
	case 0x969C3D67:
		return specialcarbine_mk2;
		break;
	case 0x7F229F94:
		return bullpuprifle;
		break;
	case 0x84D6FAFD:
		return bullpuprifle_mk2;
		break;
	case 0x624FE830:
		return compactrifle;
		break;
	case 0x9D07F764:
		return machine_gun;
		break;
	case 0x7FD62962:
		return combatmg;
		break;
	case 0xDBBD7280:
		return combatmg_mk2;
		break;
	case 0x61012683:
		return gusenberg;
		break;
	case 0x5FC3C11:
		return sniperrifle;
		break;
	case 0xC472FE2:
		return heavysniper;
		break;
	case 0xA914799:
		return heavysniper_mk2;
		break;
	case 0xC734385A:
		return marksmanrifle;
		break;
	case 0x6A6C02E0:
		return marksmanrifle_mk2;
		break;
	case 0xB1CA77B1:
		return rpg;
		break;
	case 0xA284510B:
		return grenadelauncher;
		break;
	case 0x4DD2DC56:
		return grenadelauncher_smoke;
		break;
	case 0x42BF8A85:
		return minigun;
		break;
	case 0x7F7497E5:
		return firework;
		break;
	case 0x6D544C99:
		return railgun;
		break;
	case 0x63AB0442:
		return hominglauncher;
		break;
	case 0x781FE4A:
		return compactlauncher;
		break;
	case 0x93E220BD:
		return grenade;
		break;
	case 0xA0973D5E:
		return bzgas;
		break;
	case 0xFDBC8A50:
		return smokegrenade;
		break;
	case 0x497FACC3:
		return flare;
		break;
	case 0x24B17070:
		return molotov;
		break;
	case 0x2C3731D9:
		return stickybomb;
		break;
	case 0xAB564B93:
		return proxmine;
		break;
	case 0x787F0BB:
		return snowball;
		break;
	case 0xBA45E8B8:
		return pipebomb;
		break;
	case 0x23C9F95C:
		return ball;
		break;
	case 0x34A67B97:
		return petrolcan;
		break;
	case 0x60EC506:
		return fireextinguisher;
		break;
	case 0xFBAB5776:
		return parachute;
		break;
	default:
		return unarmed;
		break;
	}
}


enum ePedBoneId : uint16_t
{
	SKEL_ROOT = 0x0,
	SKEL_Pelvis = 0x2E28,
	SKEL_L_Thigh = 0xE39F,
	SKEL_L_Calf = 0xF9BB,
	SKEL_L_Foot = 0x3779,
	SKEL_L_Toe0 = 0x83C,
	EO_L_Foot = 0x84C5,
	EO_L_Toe = 0x68BD,
	IK_L_Foot = 0xFEDD,
	PH_L_Foot = 0xE175,
	MH_L_Knee = 0xB3FE,
	SKEL_R_Thigh = 0xCA72,
	SKEL_R_Calf = 0x9000,
	SKEL_R_Foot = 0xCC4D,
	SKEL_R_Toe0 = 0x512D,
	EO_R_Foot = 0x1096,
	EO_R_Toe = 0x7163,
	IK_R_Foot = 0x8AAE,
	PH_R_Foot = 0x60E6,
	MH_R_Knee = 0x3FCF,
	RB_L_ThighRoll = 0x5C57,
	RB_R_ThighRoll = 0x192A,
	SKEL_Spine_Root = 0xE0FD,
	SKEL_Spine0 = 0x5C01,
	SKEL_Spine1 = 0x60F0,
	SKEL_Spine2 = 0x60F1,
	SKEL_Spine3 = 0x60F2,
	SKEL_L_Clavicle = 0xFCD9,
	SKEL_L_UpperArm = 0xB1C5,
	SKEL_L_Forearm = 0xEEEB,
	SKEL_L_Hand = 0x49D9,
	SKEL_L_Finger00 = 0x67F2,
	SKEL_L_Finger01 = 0xFF9,
	SKEL_L_Finger02 = 0xFFA,
	SKEL_L_Finger10 = 0x67F3,
	SKEL_L_Finger11 = 0x1049,
	SKEL_L_Finger12 = 0x104A,
	SKEL_L_Finger20 = 0x67F4,
	SKEL_L_Finger21 = 0x1059,
	SKEL_L_Finger22 = 0x105A,
	SKEL_L_Finger30 = 0x67F5,
	SKEL_L_Finger31 = 0x1029,
	SKEL_L_Finger32 = 0x102A,
	SKEL_L_Finger40 = 0x67F6,
	SKEL_L_Finger41 = 0x1039,
	SKEL_L_Finger42 = 0x103A,
	PH_L_Hand = 0xEB95,
	IK_L_Hand = 0x8CBD,
	RB_L_ForeArmRoll = 0xEE4F,
	RB_L_ArmRoll = 0x1470,
	MH_L_Elbow = 0x58B7,
	SKEL_R_Clavicle = 0x29D2,
	SKEL_R_UpperArm = 0x9D4D,
	SKEL_R_Forearm = 0x6E5C,
	SKEL_R_Hand = 0xDEAD,
	SKEL_R_Finger00 = 0xE5F2,
	SKEL_R_Finger01 = 0xFA10,
	SKEL_R_Finger02 = 0xFA11,
	SKEL_R_Finger10 = 0xE5F3,
	SKEL_R_Finger11 = 0xFA60,
	SKEL_R_Finger12 = 0xFA61,
	SKEL_R_Finger20 = 0xE5F4,
	SKEL_R_Finger21 = 0xFA70,
	SKEL_R_Finger22 = 0xFA71,
	SKEL_R_Finger30 = 0xE5F5,
	SKEL_R_Finger31 = 0xFA40,
	SKEL_R_Finger32 = 0xFA41,
	SKEL_R_Finger40 = 0xE5F6,
	SKEL_R_Finger41 = 0xFA50,
	SKEL_R_Finger42 = 0xFA51,
	PH_R_Hand = 0x6F06,
	IK_R_Hand = 0x188E,
	RB_R_ForeArmRoll = 0xAB22,
	RB_R_ArmRoll = 0x90FF,
	MH_R_Elbow = 0xBB0,
	SKEL_Neck_1 = 0x9995,
	SKEL_Head = 0x796E,
	IK_Head = 0x322C,
	FACIAL_facialRoot = 0xFE2C,
	FB_L_Brow_Out_000 = 0xE3DB,
	FB_L_Lid_Upper_000 = 0xB2B6,
	FB_L_Eye_000 = 0x62AC,
	FB_L_CheekBone_000 = 0x542E,
	FB_L_Lip_Corner_000 = 0x74AC,
	FB_R_Lid_Upper_000 = 0xAA10,
	FB_R_Eye_000 = 0x6B52,
	FB_R_CheekBone_000 = 0x4B88,
	FB_R_Brow_Out_000 = 0x54C,
	FB_R_Lip_Corner_000 = 0x2BA6,
	FB_Brow_Centre_000 = 0x9149,
	FB_UpperLipRoot_000 = 0x4ED2,
	FB_UpperLip_000 = 0xF18F,
	FB_L_Lip_Top_000 = 0x4F37,
	FB_R_Lip_Top_000 = 0x4537,
	FB_Jaw_000 = 0xB4A0,
	FB_LowerLipRoot_000 = 0x4324,
	FB_LowerLip_000 = 0x508F,
	FB_L_Lip_Bot_000 = 0xB93B,
	FB_R_Lip_Bot_000 = 0xC33B,
	FB_Tongue_000 = 0xB987,
	RB_Neck_1 = 0x8B93,
	SPR_L_Breast = 0xFC8E,
	SPR_R_Breast = 0x885F,
	IK_Root = 0xDD1C,
	SKEL_Neck_2 = 0x5FD4,
	SKEL_Pelvis1 = 0xD003,
	SKEL_PelvisRoot = 0x45FC,
	SKEL_SADDLE = 0x9524,
	MH_L_CalfBack = 0x1013,
	MH_L_ThighBack = 0x600D,
	SM_L_Skirt = 0xC419,
	MH_R_CalfBack = 0xB013,
	MH_R_ThighBack = 0x51A3,
	SM_R_Skirt = 0x7712,
	SM_M_BackSkirtRoll = 0xDBB,
	SM_L_BackSkirtRoll = 0x40B2,
	SM_R_BackSkirtRoll = 0xC141,
	SM_M_FrontSkirtRoll = 0xCDBB,
	SM_L_FrontSkirtRoll = 0x9B69,
	SM_R_FrontSkirtRoll = 0x86F1,
	SM_CockNBalls_ROOT = 0xC67D,
	SM_CockNBalls = 0x9D34,
	MH_L_Finger00 = 0x8C63,
	MH_L_FingerBulge00 = 0x5FB8,
	MH_L_Finger10 = 0x8C53,
	MH_L_FingerTop00 = 0xA244,
	MH_L_HandSide = 0xC78A,
	MH_Watch = 0x2738,
	MH_L_Sleeve = 0x933C,
	MH_R_Finger00 = 0x2C63,
	MH_R_FingerBulge00 = 0x69B8,
	MH_R_Finger10 = 0x2C53,
	MH_R_FingerTop00 = 0xEF4B,
	MH_R_HandSide = 0x68FB,
	MH_R_Sleeve = 0x92DC,
	FACIAL_jaw = 0xB21,
	FACIAL_underChin = 0x8A95,
	FACIAL_L_underChin = 0x234E,
	FACIAL_chin = 0xB578,
	FACIAL_chinSkinBottom = 0x98BC,
	FACIAL_L_chinSkinBottom = 0x3E8F,
	FACIAL_R_chinSkinBottom = 0x9E8F,
	FACIAL_tongueA = 0x4A7C,
	FACIAL_tongueB = 0x4A7D,
	FACIAL_tongueC = 0x4A7E,
	FACIAL_tongueD = 0x4A7F,
	FACIAL_tongueE = 0x4A80,
	FACIAL_L_tongueE = 0x35F2,
	FACIAL_R_tongueE = 0x2FF2,
	FACIAL_L_tongueD = 0x35F1,
	FACIAL_R_tongueD = 0x2FF1,
	FACIAL_L_tongueC = 0x35F0,
	FACIAL_R_tongueC = 0x2FF0,
	FACIAL_L_tongueB = 0x35EF,
	FACIAL_R_tongueB = 0x2FEF,
	FACIAL_L_tongueA = 0x35EE,
	FACIAL_R_tongueA = 0x2FEE,
	FACIAL_chinSkinTop = 0x7226,
	FACIAL_L_chinSkinTop = 0x3EB3,
	FACIAL_chinSkinMid = 0x899A,
	FACIAL_L_chinSkinMid = 0x4427,
	FACIAL_L_chinSide = 0x4A5E,
	FACIAL_R_chinSkinMid = 0xF5AF,
	FACIAL_R_chinSkinTop = 0xF03B,
	FACIAL_R_chinSide = 0xAA5E,
	FACIAL_R_underChin = 0x2BF4,
	FACIAL_L_lipLowerSDK = 0xB9E1,
	FACIAL_L_lipLowerAnalog = 0x244A,
	FACIAL_L_lipLowerThicknessV = 0xC749,
	FACIAL_L_lipLowerThicknessH = 0xC67B,
	FACIAL_lipLowerSDK = 0x7285,
	FACIAL_lipLowerAnalog = 0xD97B,
	FACIAL_lipLowerThicknessV = 0xC5BB,
	FACIAL_lipLowerThicknessH = 0xC5ED,
	FACIAL_R_lipLowerSDK = 0xA034,
	FACIAL_R_lipLowerAnalog = 0xC2D9,
	FACIAL_R_lipLowerThicknessV = 0xC6E9,
	FACIAL_R_lipLowerThicknessH = 0xC6DB,
	FACIAL_nose = 0x20F1,
	FACIAL_L_nostril = 0x7322,
	FACIAL_L_nostrilThickness = 0xC15F,
	FACIAL_noseLower = 0xE05A,
	FACIAL_L_noseLowerThickness = 0x79D5,
	FACIAL_R_noseLowerThickness = 0x7975,
	FACIAL_noseTip = 0x6A60,
	FACIAL_R_nostril = 0x7922,
	FACIAL_R_nostrilThickness = 0x36FF,
	FACIAL_noseUpper = 0xA04F,
	FACIAL_L_noseUpper = 0x1FB8,
	FACIAL_noseBridge = 0x9BA3,
	FACIAL_L_nasolabialFurrow = 0x5ACA,
	FACIAL_L_nasolabialBulge = 0xCD78,
	FACIAL_L_cheekLower = 0x6907,
	FACIAL_L_cheekLowerBulge1 = 0xE3FB,
	FACIAL_L_cheekLowerBulge2 = 0xE3FC,
	FACIAL_L_cheekInner = 0xE7AB,
	FACIAL_L_cheekOuter = 0x8161,
	FACIAL_L_eyesackLower = 0x771B,
	FACIAL_L_eyeball = 0x1744,
	FACIAL_L_eyelidLower = 0x998C,
	FACIAL_L_eyelidLowerOuterSDK = 0xFE4C,
	FACIAL_L_eyelidLowerOuterAnalog = 0xB9AA,
	FACIAL_L_eyelashLowerOuter = 0xD7F6,
	FACIAL_L_eyelidLowerInnerSDK = 0xF151,
	FACIAL_L_eyelidLowerInnerAnalog = 0x8242,
	FACIAL_L_eyelashLowerInner = 0x4CCF,
	FACIAL_L_eyelidUpper = 0x97C1,
	FACIAL_L_eyelidUpperOuterSDK = 0xAF15,
	FACIAL_L_eyelidUpperOuterAnalog = 0x67FA,
	FACIAL_L_eyelashUpperOuter = 0x27B7,
	FACIAL_L_eyelidUpperInnerSDK = 0xD341,
	FACIAL_L_eyelidUpperInnerAnalog = 0xF092,
	FACIAL_L_eyelashUpperInner = 0x9B1F,
	FACIAL_L_eyesackUpperOuterBulge = 0xA559,
	FACIAL_L_eyesackUpperInnerBulge = 0x2F2A,
	FACIAL_L_eyesackUpperOuterFurrow = 0xC597,
	FACIAL_L_eyesackUpperInnerFurrow = 0x52A7,
	FACIAL_forehead = 0x9218,
	FACIAL_L_foreheadInner = 0x843,
	FACIAL_L_foreheadInnerBulge = 0x767C,
	FACIAL_L_foreheadOuter = 0x8DCB,
	FACIAL_skull = 0x4221,
	FACIAL_foreheadUpper = 0xF7D6,
	FACIAL_L_foreheadUpperInner = 0xCF13,
	FACIAL_L_foreheadUpperOuter = 0x509B,
	FACIAL_R_foreheadUpperInner = 0xCEF3,
	FACIAL_R_foreheadUpperOuter = 0x507B,
	FACIAL_L_temple = 0xAF79,
	FACIAL_L_ear = 0x19DD,
	FACIAL_L_earLower = 0x6031,
	FACIAL_L_masseter = 0x2810,
	FACIAL_L_jawRecess = 0x9C7A,
	FACIAL_L_cheekOuterSkin = 0x14A5,
	FACIAL_R_cheekLower = 0xF367,
	FACIAL_R_cheekLowerBulge1 = 0x599B,
	FACIAL_R_cheekLowerBulge2 = 0x599C,
	FACIAL_R_masseter = 0x810,
	FACIAL_R_jawRecess = 0x93D4,
	FACIAL_R_ear = 0x1137,
	FACIAL_R_earLower = 0x8031,
	FACIAL_R_eyesackLower = 0x777B,
	FACIAL_R_nasolabialBulge = 0xD61E,
	FACIAL_R_cheekOuter = 0xD32,
	FACIAL_R_cheekInner = 0x737C,
	FACIAL_R_noseUpper = 0x1CD6,
	FACIAL_R_foreheadInner = 0xE43,
	FACIAL_R_foreheadInnerBulge = 0x769C,
	FACIAL_R_foreheadOuter = 0x8FCB,
	FACIAL_R_cheekOuterSkin = 0xB334,
	FACIAL_R_eyesackUpperInnerFurrow = 0x9FAE,
	FACIAL_R_eyesackUpperOuterFurrow = 0x140F,
	FACIAL_R_eyesackUpperInnerBulge = 0xA359,
	FACIAL_R_eyesackUpperOuterBulge = 0x1AF9,
	FACIAL_R_nasolabialFurrow = 0x2CAA,
	FACIAL_R_temple = 0xAF19,
	FACIAL_R_eyeball = 0x1944,
	FACIAL_R_eyelidUpper = 0x7E14,
	FACIAL_R_eyelidUpperOuterSDK = 0xB115,
	FACIAL_R_eyelidUpperOuterAnalog = 0xF25A,
	FACIAL_R_eyelashUpperOuter = 0xE0A,
	FACIAL_R_eyelidUpperInnerSDK = 0xD541,
	FACIAL_R_eyelidUpperInnerAnalog = 0x7C63,
	FACIAL_R_eyelashUpperInner = 0x8172,
	FACIAL_R_eyelidLower = 0x7FDF,
	FACIAL_R_eyelidLowerOuterSDK = 0x1BD,
	FACIAL_R_eyelidLowerOuterAnalog = 0x457B,
	FACIAL_R_eyelashLowerOuter = 0xBE49,
	FACIAL_R_eyelidLowerInnerSDK = 0xF351,
	FACIAL_R_eyelidLowerInnerAnalog = 0xE13,
	FACIAL_R_eyelashLowerInner = 0x3322,
	FACIAL_L_lipUpperSDK = 0x8F30,
	FACIAL_L_lipUpperAnalog = 0xB1CF,
	FACIAL_L_lipUpperThicknessH = 0x37CE,
	FACIAL_L_lipUpperThicknessV = 0x38BC,
	FACIAL_lipUpperSDK = 0x1774,
	FACIAL_lipUpperAnalog = 0xE064,
	FACIAL_lipUpperThicknessH = 0x7993,
	FACIAL_lipUpperThicknessV = 0x7981,
	FACIAL_L_lipCornerSDK = 0xB1C,
	FACIAL_L_lipCornerAnalog = 0xE568,
	FACIAL_L_lipCornerThicknessUpper = 0x7BC,
	FACIAL_L_lipCornerThicknessLower = 0xDD42,
	FACIAL_R_lipUpperSDK = 0x7583,
	FACIAL_R_lipUpperAnalog = 0x51CF,
	FACIAL_R_lipUpperThicknessH = 0x382E,
	FACIAL_R_lipUpperThicknessV = 0x385C,
	FACIAL_R_lipCornerSDK = 0xB3C,
	FACIAL_R_lipCornerAnalog = 0xEE0E,
	FACIAL_R_lipCornerThicknessUpper = 0x54C3,
	FACIAL_R_lipCornerThicknessLower = 0x2BBA,
	MH_MulletRoot = 0x3E73,
	MH_MulletScaler = 0xA1C2,
	MH_Hair_Scale = 0xC664,
	MH_Hair_Crown = 0x1675,
	SM_Torch = 0x8D6,
	FX_Light = 0x8959,
	FX_Light_Scale = 0x5038,
	FX_Light_Switch = 0xE18E,
	BagRoot = 0xAD09,
	BagPivotROOT = 0xB836,
	BagPivot = 0x4D11,
	BagBody = 0xAB6D,
	BagBone_R = 0x937,
	BagBone_L = 0x991,
	SM_LifeSaver_Front = 0x9420,
	SM_R_Pouches_ROOT = 0x2962,
	SM_R_Pouches = 0x4141,
	SM_L_Pouches_ROOT = 0x2A02,
	SM_L_Pouches = 0x4B41,
	SM_Suit_Back_Flapper = 0xDA2D,
	SPR_CopRadio = 0x8245,
	SM_LifeSaver_Back = 0x2127,
	MH_BlushSlider = 0xA0CE,
	SKEL_Tail_01 = 0x347,
	SKEL_Tail_02 = 0x348,
	MH_L_Concertina_B = 0xC988,
	MH_L_Concertina_A = 0xC987,
	MH_R_Concertina_B = 0xC8E8,
	MH_R_Concertina_A = 0xC8E7,
	MH_L_ShoulderBladeRoot = 0x8711,
	MH_L_ShoulderBlade = 0x4EAF,
	MH_R_ShoulderBladeRoot = 0x3A0A,
	MH_R_ShoulderBlade = 0x54AF,
	FB_R_Ear_000 = 0x6CDF,
	SPR_R_Ear = 0x63B6,
	FB_L_Ear_000 = 0x6439,
	SPR_L_Ear = 0x5B10,
	FB_TongueA_000 = 0x4206,
	FB_TongueB_000 = 0x4207,
	FB_TongueC_000 = 0x4208,
	SKEL_L_Toe1 = 0x1D6B,
	SKEL_R_Toe1 = 0xB23F,
	SKEL_Tail_03 = 0x349,
	SKEL_Tail_04 = 0x34A,
	SKEL_Tail_05 = 0x34B,
	SPR_Gonads_ROOT = 0xBFDE,
	SPR_Gonads = 0x1C00,
	FB_L_Brow_Out_001 = 0xE3DB,
	FB_L_Lid_Upper_001 = 0xB2B6,
	FB_L_Eye_001 = 0x62AC,
	FB_L_CheekBone_001 = 0x542E,
	FB_L_Lip_Corner_001 = 0x74AC,
	FB_R_Lid_Upper_001 = 0xAA10,
	FB_R_Eye_001 = 0x6B52,
	FB_R_CheekBone_001 = 0x4B88,
	FB_R_Brow_Out_001 = 0x54C,
	FB_R_Lip_Corner_001 = 0x2BA6,
	FB_Brow_Centre_001 = 0x9149,
	FB_UpperLipRoot_001 = 0x4ED2,
	FB_UpperLip_001 = 0xF18F,
	FB_L_Lip_Top_001 = 0x4F37,
	FB_R_Lip_Top_001 = 0x4537,
	FB_Jaw_001 = 0xB4A0,
	FB_LowerLipRoot_001 = 0x4324,
	FB_LowerLip_001 = 0x508F,
	FB_L_Lip_Bot_001 = 0xB93B,
	FB_R_Lip_Bot_001 = 0xC33B,
	FB_Tongue_001 = 0xB987
};


class hk_AmmoInfo
{
public:

	float SetSpeedBullet(float value)
	{
		if (!this) return 0;
		return *(float*)(this + 0x58) = value;
	}

};

class hk_WeaponInfo
{
public:

	/*float SetMultiBulletSpread(float caca)
	{
		if (!this) return NULL;
		return *(float*)(this + 0x011C) = caca;
	}*/

	float Damage_boost(float caca)
	{
		if (!this) return NULL;
		return *(float*)(this + 0x00B0) = caca;
	}

	/*std::int32_t SetBulletPerShot(std::int32_t caca)
	{
		if (!this) return NULL;
		return *(std::int32_t*)(this + 0x0118) = caca;
	}*/
	uint64_t GetHash()
	{
		if (!this) return NULL;
		return *(uint64_t*)(this + 0x10);
	}

	uint64_t SetHash(DWORD caca)
	{
		if (!this) return NULL;
		return *(uint64_t*)(this + 0x10) = caca;
	}
	float SetSpread(float value)
	{
		if (!this) return 0;
		return *(float*)(this + FiveM::Spread) = value;
	}

	float SetRecoil(float value)
	{
		if (!this) return 0;
		return *(float*)(this + FiveM::Recoil) = value;
	}
	/*float SetDamage(float value)
	{
		if (!this) return 0;
		return *(float*)(this + 0xB0) = value;
	}*/

	hk_AmmoInfo* AmmoInfo()
	{
		if (!this) return 0;
		return (hk_AmmoInfo*)(*(uint64_t*)(this + 0x60));
	}

	float SetReload(float value)
	{
		if (!this) return 0;
		return *(float*)(this + FiveM::ReloadMultiplier) = value;
	}


	float SetRange(float value)
	{
		if (!this) return 0;
		return *(float*)(this + FiveM::Range) = value;
	}
	int32_t  SetAmmoType(float value)
	{
		if (!this) return 0;
		return *(int32_t*)(this + FiveM::AmmoType) = value;
	}
	int32_t  SetAmmoExplosiveType(float value)
	{
		if (!this) return 0;
		return *(int32_t*)(this + FiveM::AmmoExplosiveType) = value;
	}

	char* getweaponname() { 
		return (*reinterpret_cast<char**>((DWORD64)this + 0x05F0)); 
	}

};
class hk_ObjectNavigationVehicle
{
public:
	Vector3 GetCoordinate()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x50);
	}
	Vector3 SetCoordinate(Vector3 Cords)
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x50) = Cords;
	}

	Vector4 SetRotation(Vector4 Coords)
	{
		if (!this) return Vector4{ 0,0,0,0 };
		return *(Vector4*)(this + 0x30) = Coords;
	}

};
class hk_WeaponManager
{
public:
	hk_WeaponInfo* WeaponInfo()
	{
		if (!this) return 0;
		return (hk_WeaponInfo*)(*(uint64_t*)(this + 0x20));
	}

	Vector3 GetWeaponCoordinate()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x1A0);
	}
	Vector3 SetWeaponCoordinate(Vector3 point)
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x1A0) = point;
	}

};

class hk_ObjectNavigationPed
{
public:
	Vector3 GetCoordinate()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x50);
	}
	Vector3 SetCoordinate(Vector3 Cords)
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x50) = Cords;
	}

	Vector4 SetRotation(Vector4 Coords)
	{
		if (!this) return Vector4{ 0,0,0,0 };
		return *(Vector4*)(this + 0x30) = Coords;
	}

};
class hk_VehicleModelInfo
{
public:

	char* GetCharName()
	{
		if (!this) return 0;
		return (char*)(this + 0x298);
	}

};

class CModelInfo
{
public:
	char pad_0000[24]; //0x0000
	uint32_t Hash; //0x0018
	char pad_001C[20]; //0x001C
	Vector3 MinDim; //0x0030
	char pad_003C[4]; //0x003C
	Vector3 MaxDim; //0x0040
	char pad_004C[588]; //0x004C
	char Name[24]; //0x0298
	char pad_02A0[688]; //0x02A0
	char m_manufacturer[12]; //0x02A4
}; //Size: 0x0550


class hk_VehicleHandlingInfo
{
public:

	float SetMass(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x0C) = Health;
	}
	float SetAcceleration(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x4C) = Health;
	}
	float SetHandBrake(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x7C) = Health;
	}
	float SetBrake(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x6C) = Health;
	}


};
class hk_Vehicle
{
public:
	hk_VehicleModelInfo* VehicleModelInfo()
	{
		if (!this) return 0;
		return (hk_VehicleModelInfo*)(*(uint64_t*)(this + 0x20));
	}
	hk_VehicleHandlingInfo* VehicleHandlingInfo()
	{
		if (!this) return 0;
		return (hk_VehicleHandlingInfo*)(*(uint64_t*)(this + 0x938));
	}
	hk_ObjectNavigationVehicle* ObjectNavigation()
	{
		if (!this) return 0;
		return (hk_ObjectNavigationVehicle*)(*(uint64_t*)(this + 0x30));
	}
	Vector3 GetCoordinate()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x90);
	}
	Vector3 SetCoordinate(Vector3 Cords)
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x90) = Cords;
	}
	Vector3 m_visual_pos()
	{
		if (!this) return { 0, 0, 0 };
		return *(Vector3*)(this + 0x90);
	}
	Vector3 SetVelocity()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x320) = Vector3(0, 0, 0);
	}
	float GetHealth()
	{
		if (!this) return 0;
		return *(float*)(this + 0x280);
	}
	bool Godmode(bool value)
	{
		if (!this) return 0;
		return *(bool*)(this + 0x189) = value;
	}
	float GetMaxHealth()
	{
		if (!this) return 0;
		return *(float*)(this + 0x284);
	}
	float SetHealth(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x280) = Health;
	}
	float SetGravity(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0xC5C) = Health;
	}
	float SetBodyHealth(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x840) = Health;
	}
	float SetEngineHealth(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x908) = Health;
	}
	float primarycolor(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x0388 + 0x03A0) = Health;
	}
};

class hk_Gravity
{
public:

	bool IsNoGravity()
	{

		if (*(uint16_t*)(this + 0x1A) == 780)
		{
			return true;
		}
		else
		{
			return false;
		}

	}

	uint16_t SetNoGravity(bool caca)
	{
		if (!this) return 0;
		if (caca)
			return *(uint16_t*)(this + 0x1A) = 0x30C;
		else
			return *(uint16_t*)(this + 0x1A) = 0x304;
	}

};
class hk_PlayerInfo
{
public:
	uintptr_t NetInfo()
	{
		if (!this) return 0;
		return *(DWORD64*)(this + 0x0020);

	}
	uint32_t SetSuperJump(bool toggle)
	{
		if (toggle)
		{
			if (!this) return 0;
			return *(BYTE*)(this + 0x218) |= 1 << 14;;
		}
		else
		{
			if (!this) return 0;
			return *(BYTE*)(this + 0x218) = 0;
		}

		



	}

	float SetSpeed(float value)
	{
		if (!this) return 0;
		return *(float*)(this + 0x0CF0) = value;
	}




};
class hk_PedModelInfo
{
public:

	Vector3 GetMin()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x30);
	}
	Vector3 GetMax()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x40);
	}

};

class hk_Ped
{
public:
        void freezePlayer(hk_Ped* cPlayerInfo, bool v = true, uintptr_t type = 2) {

                if (v)
                        *(uintptr_t*)(cPlayerInfo + FiveM::freeze) = type; // 0x238
                else
                        *(uintptr_t*)(cPlayerInfo + FiveM::freeze) = 0;
        }
        hk_Ped* m_player_info( ) {
                if (!this)
                        return 0;
                return (hk_Ped*)*(uintptr_t*)(this + FiveM::PlayerInfo);
        }
        int m_player_id( ) {
                if (!this)
                        return 0;
                return *(int*)(this + FiveM::m_player_id);
        }
        Vector3 m_set_visual_pos(Vector3 end_pos) {
                if (!this)
                        return Vector3(0, 0, 0);
                return *(Vector3*)(this + 0x50) = end_pos;
        }

		Vector3 set_visual_pos(Vector3 Pos) {
                if (!this)
                        return Vector3(0, 0, 0);
                return *(Vector3*)(this + 0x90) = Pos;
        }
        Vector4 m_rot(Vector4 rot) {
                if (!this)
                        return Vector4(0, 0, 0, 0);
                return *(Vector4*)(this + 0x30) = rot;
        }
        Vector3 m_get_rot( ) {
                if (!this)
                        return {0, 0, 0};
                return *(Vector3*)(this + 0x30);
        }
        Vector3 m_rot_vec3(Vector3 rot) {
                if (!this)
                        return {45, 45, 45};
                return *(Vector3*)(this + 0x30) = rot;
        }
        Vector3 m_heading(Vector3 heading) {
                if (!this)
                        return {0, 0, 0};
                return *(Vector3*)(this + 0x70) = heading;
        }
        float fHeading( ) {
                if (!this)
                        return 0;
                return *(float*)(this + 0x0020);
        } // 0x0020
        float fHeading2( ) {
                if (!this)
                        return 0;
                return *(float*)(this + 0x0024);
        } // 0x0024

        float set_heading(float heading) {
                if (!this)
                        return 0;
                return *(float*)(this + 0x20) = heading;
        }
        Vector3 m_visual_pos( ) {
                if (!this)
                        return Vector3{0, 0, 0};
                return *(Vector3*)(this + 0x90);
        }

        Vector3 m_velo_city(Vector3 velo) {
                if (!this)
                        return Vector3(0, 0, 0);
                return *(Vector3*)(this + 0x320) = velo;
        }
	//_------------
	BYTE visible(BYTE val)
	{
		if (!this) return 0;
		return *(BYTE*)(this + 0x2C) = val;
	}
       
	hk_PedModelInfo* PedModelInfo()
	{
		if (!this) return 0;
		return (hk_PedModelInfo*)(*(uint64_t*)(this + 0x20));
	}
	hk_PlayerInfo* PlayerInfo()
	{
		if (!this) return 0;
		return (hk_PlayerInfo*)(*(uint64_t*)(this + FiveM::PlayerInfo));
	}
	hk_WeaponManager* WeaponManager()
	{
		if (!this) return 0;
		return (hk_WeaponManager*)(*(uint64_t*)(this + FiveM::WeaponManager));
	}
	hk_Gravity* GravityManager()
	{
		if (!this) return 0;
		return (hk_Gravity*)(*(uintptr_t*)(this + 0x1110));
	}
	hk_Vehicle* VehicleManager()
	{
		if (!this) return 0;
		return (hk_Vehicle*)(*(uint64_t*)(this + 0xD30));
	}
	hk_ObjectNavigationPed* ObjectNavigation()
	{
		if (!this) return 0;
		return (hk_ObjectNavigationPed*)(*(uint64_t*)(this + 0x30));
	}
	bool IsInAVehicule()
	{
		if (!this) return false;
		if (*(BYTE*)(this + FiveM::IsInAVehicule) == 0x40)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool SetFreeze(bool toggle)
	{

		if (!this) return 0;
		return *(BYTE*)(this + 0x218) = toggle;
	}

	uint32_t SetSuperJump(bool toggle)
	{
		if (toggle)
		{
			if (!this) return 0;
			return *(BYTE*)(this + 0x218) |= 1 << 14;;
		}


	}



	Vector3 GetCoordinate()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x90);
	}
	Vector3 SetCoordinate(Vector3 Cords)
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x90) = Cords;
	}

	Vector3 SetVelocity()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x320) = Vector3(0, 0, 0);
	}
	bool in_vehicle()
	{
		if (!this) return 0;
		uint8_t ped_task = *(uint8_t*)(this + 0x144B);
		if (ped_task == 1 << 6)
			return true; 
	}
	float GetHealth()
	{
		if (!this) return 0;
		return *(float*)(this + 0x280);
	}
	float GetArmor()
	{
		if (!this) return 0;
		return *(float*)(this + FiveM::Armor);
	}
	float GetMaxHealth()
	{
		if (!this) return 0;
		return *(float*)(this + 0x284);
	}
	float SetHealth(float Health)
	{
		if (!this) return 0;
		return *(float*)(this + 0x280) = Health;
	}
	uint32_t frameflags(uint32_t flags)
	{
		if (!this) return 0;
		return *(uint32_t*)(this + FiveM::m_frame_flags) = flags; 
	}
	float SetArmor(float Armor)
	{
		if (!this) return 0;
		return *(float*)(this + FiveM::Armor) = Armor;
	}
	float SetMaxHealth()
	{
		if (!this) return 0;
		return *(float*)(this + 0x280) = GetMaxHealth();
	}
	uint32_t GetPedType()
	{
		if (!this) return 0;
		return *(uint32_t*)(this + FiveM::EntityType);
	}
	bool IsPedOrFalse()
	{
		if (!this) return 0;
		auto ped_type = this->GetPedType();

		ped_type = ped_type << 11 >> 25;

		if (ped_type != 2)
		{
			return true;

		}
		else
			return false;
	}

	unsigned char Set_Ragdoll(bool value)
	{
		if (!this) return 0;
		if (value)
		{
			return *(unsigned char*)(this + 0x10B8) = 1;
		}
		else
		{
			return *(unsigned char*)(this + 0x10B8) = 32;
		}

	}



};
class hk_ObjectInfo
{
public:
	char* ModelChar()
	{
		if (!this) return 0;
		return (char*)(this + 0x18);
	}




};
class hk_Object
{
public:
	hk_ObjectInfo* ObjectModelInfo()
	{
		if (!this) return 0;
		return (hk_ObjectInfo*)(*(uint64_t*)(this + 0x20));
	}
	Vector3 GetCoordinate()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3*)(this + 0x90);
	}



};
class hk_ObjectList
{
public:
	hk_Object* Object(int index)
	{
		if (!this) return 0;
		return (hk_Object*)(*(uint64_t*)(this + (index * 0x10)));
	}
};

class hk_PedList
{
public:
	hk_Ped* Ped(int index)
	{
		if (!this) return 0;
		return (hk_Ped*)(*(uint64_t*)(this + (index * 0x10)));
	}
};

class hk_VehicleList
{
public:
	hk_Vehicle* Vehicle(int index)
	{
		if (!this) return 0;
		return (hk_Vehicle*)(*(uint64_t*)(this + (index * 0x10)));
	}

};
class hk_VehicleInterface
{
public:
	uint64_t VehicleMaximum()
	{
		if (!this) return 0;
		return (uint64_t)(*(uint64_t*)(this + 0x188));
	}

	hk_VehicleList* VehicleList()
	{
		if (!this) return 0;
		return (hk_VehicleList*)(*(uint64_t*)(this + 0x180));
	}
};
class hk_PedInterface
{
public:
	uint64_t PedMaximum()
	{
		if (!this) return 0;
		return (uint64_t)(*(uint64_t*)(this + 0x108));
	}

	hk_PedList* PedList()
	{
		if (!this) return 0;
		return (hk_PedList*)(*(uint64_t*)(this + 0x100));
	}
};

class hk_ObjectInterface
{
public:
	uint64_t ObjectMaximum()
	{
		if (!this) return 0;
		return (uint64_t)(*(uint64_t*)(this + 0x160));
	}

	hk_ObjectList* ObjectList()
	{
		if (!this) return 0;
		return (hk_ObjectList*)(*(uint64_t*)(this + 0x0158));
	}
};
class hk_ReplayInterface
{
public:
	hk_PedInterface* PedInterface()
	{
		if (!this) return 0;
		return (hk_PedInterface*)(*(uint64_t*)(this + 0x18));
	}
	hk_VehicleInterface* VehicleInterface()
	{
		if (!this) return 0;
		return (hk_VehicleInterface*)(*(uint64_t*)(this + 0x10));
	}
	hk_ObjectInterface* ObjectInterface()
	{
		if (!this) return 0;
		return (hk_ObjectInterface*)(*(uint64_t*)(this + 0x28));
	}
};

class hk_World
{
public:
	hk_Ped* LocalPlayer()
	{
		if (!this) return 0;
		return (hk_Ped*)(*(uint64_t*)(this + 0x8));
	}
};

