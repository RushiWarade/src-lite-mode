

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_android.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "StrEnc.h"
#include "obfuscate.h"
#include "imgui/imgui_additional.h"
#include "Tools.h"
#include "UE4.h"
#include "FTools/Font.h"
#include "FTools/Iconcpp.h"
#include "FTools/ImguiPP.h"
#include "FTools/Icon.h"
#include "KittyMemory/MemoryPatch.h"
#include "Helper/plthook.h"
#include "Helper/Includes.h"
#include "Helper/Items.h"
#include "Helper/json.hpp"
#include "FoxCheats/ScanEngine.hpp"
#include "Colors.h"
#include "Syscall.h"
#include "SDK.hpp"
#include <stdio.h>
#include <string.h>
#include <mutex>
#include <chrono>
#include <set>
#include <algorithm>
#include <cctype>
using namespace SDK;
using json = nlohmann::ordered_json;
#include <curl/curl.h>
uintptr_t ProcessEvent_Offset;
#include <openssl/rsa.h>
#include <openssl/pem.h>
//=========================================================================//
json items_data;
std::string g_Token, g_Auth;
android_app * g_App = 0;
uintptr_t UE4, g_LocalPlayer = 0, g_LocalController = 0, g_LocalWeapon = 0;

SDK::ASTExtraPlayerCharacter *UTAM_LocalPlayer = 0;
SDK::ASTExtraPlayerController *UTAM_LocalController = 0;


static std::string EXP = " ";
static std::string expiredDate = "";
//=========================================================================//
static bool Feu;
ImFont* UTAMo = nullptr;
bool PeekUp = true;
float Setspeed = 144.0f;
float Cross = 170.0f;
bool WIDEVIEW = true;
float UTAM = 340.0f;
int languages;
bool UP = false;
bool fastchut = true;
bool autoMagChange = false;
bool fastchut1 = true;
bool UTAM_FLY = false;
float ControlSpeed= 0.10f;
float WIDEVIEW1 = 250.0f;
bool SKYFLY = false;
bool LITE_ESP = false;
bool Targetline;
bool RoZERRK;
bool initImGui = false , bValid = false;
bool ZOOMV3 = false;
bool MAX_DAMAGE = true;   // always on - see the auto-trigger next to Long Hand's
bool UTAM_FLASH  = false;
float FastMovementScale = 1.3f;  // always-on proportional engine speed scale - see UTAMA_HUD
bool show_WindowV4 = false;
bool AimButton2 = true;
bool AMMO = false,
mkmk = false,
mkmkk = false,
mkmkkk = false, 
UrlLink = false,
Godviewup = false,
Godviewdown = false,
FIXSTUCK = false,
SKYSHOT = false,
SKYSHOT1 = false,
STUCK2 = false;
bool Lineee;
bool Boxx;
bool alert;
bool Visibility;
bool Healthh;
bool Namee;
bool Distancee;
bool TeamIDd;
bool Vehiclee;
bool Weaponn;
bool Infoo;
bool Radarr;
bool Alertt;
bool View;
bool Recoil;
bool IgBot;
bool IgKnock;
bool VisCheck;
bool Aim;
bool Unlock;
bool FastChut = true;
bool MK = false;
bool UTAMKILLMSG = true;

#define W2S(w, s) SDK::UGameplayStatics::ProjectWorldToScreen(ZevaController, w, true, s)
class Rect {
public:
    float x;
    float y;
    float width;
    float height;

    Rect() {
        this->x = 0;
        this->y = 0;
        this->width = 0;
        this->height = 0;
    }

    Rect(float x, float y, float width, float height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    bool operator==(const Rect &src) const {
        return (src.x == this->x && src.y == this->y && src.height == this->height &&
                src.width == this->width);
    }

    bool operator!=(const Rect &src) const {
        return (src.x != this->x && src.y != this->y && src.height != this->height &&
                src.width != this->width);
    }
};


using namespace kFox;



//=========================================================================//
namespace Zeva {
ImVec4 particleColour = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red color (RGBA)
}
struct sRegion {
uintptr_t start, end;
};

std::vector<sRegion> trapRegions;
bool isObjectInvalid(UObject *obj) {
if (!Tools::IsPtrValid(obj)) {
return true;
}

if (!Tools::IsPtrValid(obj->ClassPrivate)) {
return true;
}

if (obj->InternalIndex <= 0) {
return true;
}

if (obj->NamePrivate.ComparisonIndex <= 0) {
return true;
}

if ((uintptr_t)(obj) % sizeof(uintptr_t) != 0x0 && (uintptr_t)(obj) % sizeof(uintptr_t) != 0x4) {
return true;
}

if (std::any_of(trapRegions.begin(), trapRegions.end(), [obj](sRegion region) { return ((uintptr_t) obj) >= region.start && ((uintptr_t) obj) <= region.end; }) ||
std::any_of(trapRegions.begin(), trapRegions.end(), [obj](sRegion region) { return ((uintptr_t) obj->ClassPrivate) >= region.start && ((uintptr_t) obj->ClassPrivate) <= region.end; })) {
return true;
}

return false;
}
ImColor outlinecolor = IM_COL32(0, 0, 0, 255);
#define CREATE_COLOR(r, g, b, a) new float[4]{(float)r, (float)g, (float)b, (float)a};
static float isRed = 0.0f, isGreen = 0.01f, isBlue = 0.0f;
int32_t ToColor(float * col) {
return ImGui::ColorConvertFloat4ToU32( * (ImVec4 *)(col));}
#define SLEEP_TIME 1000LL / 120LL
int screenWidth = -1, glWidth, screenHeight = -1, glHeight;
float density = -1;
int Write_Float(long int addr, float value) {
Tools::WriteAddr((void *) (addr), (void *) &value, 4);
return 0;}
bool WriteAddr(void *addr, void *buffer, size_t length) {
unsigned long page_size = sysconf(_SC_PAGESIZE);
unsigned long size = page_size * sizeof(uintptr_t);
return mprotect((void *) ((uintptr_t) addr - ((uintptr_t) addr % page_size) - page_size), (size_t) size, PROT_EXEC | PROT_READ | PROT_WRITE) == 0 && memcpy(addr, buffer, length) != 0;}
template<typename T>
void Write(uintptr_t addr, T value) {
WriteAddr((void *) addr, &value, sizeof(T));}
std::map<int, bool> Items;
//// POWER OF UTAM //////
#define GEngine_Offset 0x4ccea00
#define GNames_Offset 0x19ee5f4
#define GUObject_Offset 0x4fd8d20
#define GNativeAndroidApp_Offset 0x4cc5f14
#define GetWeaponPropSlot_Offset 0x12dbd50
#define CanvasMap_Offset 0x508c67c
#define GetBonePos_Offset 0xf61158
#define GetBoneName_Offset 0x276c43c
#define GetDistanceTo_Offset 0x25b87dc
#define Object_ClassPrivate 0xC
#define Object_NamePrivate_ComparisonIndex 0x10
#define Class_SuperStruct 0x20
#define FNameEntry_AnsiName 0x8
#define Level_Actors 0x70
#define Canvas_ViewProjectionMatrix 0x200
#define Controller_LineOfSightTo 0x35C
#define World_PersistentLevel 0x20
#define Character_Mesh 0x30C
#define Actor_RootComponent 0x140
#define SceneComponent_ComponentVelocity 0x1a0
#define SceneComponent_RelativeLocation 0x118
#define SceneComponent_RelativeRotation 0x124
#define UAEPlayerController_PlayerKey 0x594
#define UAECharacter_PlayerName 0x5f8
#define UAECharacter_PlayerKey 0x610
#define UAECharacter_TeamID 0x620
#define UAECharacter_bIsAI 0x688
#define STExtraCharacter_Health 0x7a0
#define STExtraCharacter_HealthMax 0x7a4
#define STExtraCharacter_bDead 0x7b8
#define STExtraCharacter_PartHitComponent 0x870
#define STExtraCharacter_CurrentVehicle 0x7c4
#define STExtraCharacter_bIsGunADS 0x874
#define STExtraBaseCharacter_WeaponManagerComponent 0x117c
#define STExtraBaseCharacter_NearDeatchComponent 0xca0
#define STExtraBaseCharacter_bIsWeaponFiring 0xb6c
#define WeaponManagerComponent_CurrentWeaponReplicated 0x454
#define STCharacterNearDeathComp_BreathMax 0xc8
#define STCharacterNearDeathComp_Breath 0xf0
#define STExtraShootWeapon_ShootWeaponComponent 0x7f4
#define STExtraShootWeaponComponent_ShootWeaponEntityComponent 0x100
#define ShootWeaponEntity_BulletFireSpeed 0x340
#define Controller_ControlRotation 0x2e4
#define STExtraVehicleBase_VehicleShapeType 0x3ae
#define STExtraVehicleBase_VehicleCommon 0x588
#define VehicleCommonComponent_HP 0xe8
#define VehicleCommonComponent_HPMax 0xe4
#define VehicleCommonComponent_Fuel 0x100
#define VehicleCommonComponent_FuelMax 0xfc
#define PickUpWrapperActor_DefineID 0x2f0
#define PickUpWrapperActor_ItemValue 0x3A0     // FString - read directly, no engine call needed
#define PickUpWrapperActor_ItemCategory 0x3AC  // FString - engine's own category tag for this pickup
#define STExtraPlayerController_STExtraBaseCharacter 0x1448
#define PlayerController_PlayerCameraManager 0x330
#define PlayerCameraManager_CameraCache 0x330
#define STExtraWeapon_WeaponEntityComp 0x468
#define WeaponEntity_WeaponID 0xd0
#define STExtraShootWeapon_CurBulletNumInClip 0x7f8
#define STExtraShootWeapon_CurMaxBulletNumInOneClip 0x7fc
#define STExtraGameStateBase_PlayerNum 0x430
#define STExtraGameStateBase_AlivePlayerNum 0x5b4
#define STExtraGameStateBase_AliveTeamNum 0x5b8
#define GameState_ElapsedTime 0x308
#define Player_PlayerController 0x20
#define World_NetDriver 0x24
#define NetDriver_ServerConnection 0x6c
#define STExtraPlayerController_bIsPressingFireBtn 0x1eb8
#define STExtraShootWeaponComponent_OwnerShootWeapon 0x118

enum EAimTarget {
HeadT = 0,
Chest = 1
};
enum EESPUIType {
    ESPUI01 = 0,
    ESPUI02 = 1
};

enum EAimType
{
    Distance = 0,
    Croshhair = 1
};


enum EAimRange {
R_360 = 0,
};
enum EAimTrigger {
None = 0,
Shooting = 1,
Scoping = 2,
Both = 3,
Any = 4
};
namespace Zeva {
static int Tab = 1;
}
struct sConfig {
bool Bypass;
struct sPlayerESP {
bool Line;
bool Box;
bool Health;
bool Skeleton;
bool Name;
bool Visibility;
bool AutoFire = true;
bool Distance;
bool TeamID;
bool NoBot;
bool Weapon;
bool Vehicle;
bool Alert360 = true;
bool linetarget;
bool Grenade;
bool UtamDeadBox;
int Cross;
bool AdvAlert;
bool Head;
bool autofire;
bool Lootbox;
bool TeamID2;
bool info;
bool TeammateESP = true;   // show name+distance for teammates (not just enemies)
EESPUIType ESPUIType;
};
sPlayerESP PlayerESP{0};
struct sColorsESP {
float * PVLine;
float * PVILine;
float * BVLine;
float * BVILine;
float * PVBox;
float * PVIBox;
float * BVBox;
float * BVIBox;
float * PVSkeleton;
float * PVISkeleton;
float * BVSkeleton;
float * BVISkeleton;
float * TeamID;
float * Name;
float * Distance;
float * Vehicle;
float * Items;
};
sColorsESP ColorsESP{0};
struct sAimMenu {
bool Enable;
bool IgnoreKnocked = true;
bool IgnoreBot = true;
bool AutoFire;
float FOVSize;
bool SKYFLY;
bool SKYUP;
bool AimButton;
EAimType Type;
EAimTarget Target;
EAimRange Range;
EAimTrigger Trigger;
bool DOWE;
bool STABLE;
bool FLY_MENU;
bool VisCheck;
bool Prediction;
};
sAimMenu AimMenu{0};
sAimMenu SilentAim{0};
sAimMenu AimBot{0};
};
sConfig Config {0};
struct sPatches { 
MemoryPatch
recoil,
shake,
cross,
xhit1,
sky10,
sky1,
sky2,
sky3,
sky4,
sky5,
sky6,
sky7,
sky8,
sky9,
xhit2,
nofog,
blacksky,
desert,
carjmp,
carspd,
skyup,
skydown,
skystable,
grass,
night,
ipad,
utam01,
utam02,
utam03,
utam04,
utam05,
utam06,
utam07,
utam08,
utam09,
utam10,
utam11,
utam12,
utam13,
NoGrass,
wshoot10,
flash07,
flash08,
flash09,
flash10,
flash11,
flash12,
flash13,
noshake, noshake1, noshake2, noshake3, noshake4, noshake5, noshake6, noshake7, noshake8, noshake9,
norecoil,
Hooktdmaster;
};
sPatches Patches;


bool isObjectInvalids(SDK::UObject *obj) {
if (!Tools::IsPtrValid(obj)) {
return true;}
if (!Tools::IsPtrValid(obj->ClassPrivate)) {
return true;}
if (obj->InternalIndex <= 0) {
return true;}
if (obj->NamePrivate.ComparisonIndex <= 0) {
return true;}
if ((uintptr_t)(obj) % sizeof(uintptr_t) != 0x0 && (uintptr_t)(obj) % sizeof(uintptr_t) != 0x4) {
return true;}
if (std::any_of(trapRegions.begin(), trapRegions.end(), [obj](sRegion region) { return ((uintptr_t) obj) >= region.start && ((uintptr_t) obj) <= region.end; }) ||
std::any_of(trapRegions.begin(), trapRegions.end(), [obj](sRegion region) { return ((uintptr_t) obj->ClassPrivate) >= region.start && ((uintptr_t) obj->ClassPrivate) <= region.end; })) {
return true;}
return false;}
static SDK::UEngine *GEngine = 0;
SDK::UWorld *GetsWorld() {
while (!GEngine) {
GEngine = SDK::UObject::FindObject<SDK::UEngine>("UAEGameEngine Transient.UAEGameEngine_1");
sleep(1);}
if (GEngine) {
auto ViewPort = GEngine->GameViewport;
if (ViewPort) {
return ViewPort->World;}}
return 0;}
SDK::TNameEntryArray *GGetGNames(){
return ((SDK::TNameEntryArray *(*)()) (UE4 + GNames_Offset))();}
std::vector<SDK::AActor *> Zeva_Actors() {
auto Worlds = GetsWorld();
if (!Worlds)
return std::vector<SDK::AActor *>();
auto PersistentLevell = Worlds->PersistentLevel;
if (!PersistentLevell)
return std::vector<SDK::AActor *>();
auto UTAMA = *(SDK::TArray<SDK::AActor *> *)((uintptr_t) PersistentLevell + Level_Actors);
std::vector<SDK::AActor *> actorss;
for (int i = 0; i < UTAMA.Num(); i++) {
auto UTAMA_xD = UTAMA[i];
if (UTAMA_xD) {
actorss.push_back(UTAMA_xD);}}
return actorss;}
std::string getObjectPath(SDK::UObject *Object) {
std::string s;
for (auto super = Object->ClassPrivate; super; super = (SDK::UClass *) super->SuperStruct) {
if (!s.empty())
s += ".";
s += super->NamePrivate.GetName();}
return s;}
const char *getStringName(SDK::UObject *Object) {
std::string s;
for (auto super = Object->ClassPrivate; super; super = (SDK::UClass *) super->SuperStruct) {
if (!s.empty())
s += ".";
s += super->GetName();}
return s.c_str();}
template<class T>
void GetAllActors(std::vector<T *> &Actors) {
SDK::UGameplayStatics *gGameplayStatics = (SDK::UGameplayStatics *) gGameplayStatics->StaticClass();
auto GWorldvip = GetsWorld();
if (GWorldvip) {
SDK::TArray<SDK::AActor *> Actors2;
gGameplayStatics->GetAllActorsOfClass((SDK::UObject *) GWorldvip, T::StaticClass(), &Actors2);
for (int i = 0; i < Actors2.Num(); i++) {
Actors.push_back((T *) Actors2[i]);}}}


uintptr_t getGWorld() {
return (* (uintptr_t *)(UE4 + GEngine_Offset));}
std::vector<uintptr_t> getActors() {
auto World = getGWorld();
if (!World)
return {};
uintptr_t PersistentLevel = * (uintptr_t *)(World + World_PersistentLevel);
if (!PersistentLevel)
return {};
auto Actors = * (uintptr_t *)(PersistentLevel + Level_Actors);
auto ActorsCount = * (int *)(PersistentLevel + Level_Actors + sizeof(uintptr_t));
std::vector<uintptr_t> result;
// The level's Actors TArray can be reallocated (data pointer + count both change) by the
// game thread while we're reading it here from the render thread - moving around triggers
// this constantly via actor streaming. Reading the data pointer and count as two separate
// loads is not atomic, so a resize mid-read can hand us a stale pointer paired with a count
// that no longer matches it, which used to walk into unmapped/garbage memory: either a
// crash, or every ESP for that frame silently vanishing when isObjectPlayer() choked on
// garbage. A sane upper bound plus a real per-pointer validity probe (a syscall, not just a
// null check) filters that out before it reaches any of the ESP code below.
if (!Actors || ActorsCount <= 0 || ActorsCount > 8192)
    return result;
result.reserve(ActorsCount);
for (int i = 0; i < ActorsCount; i++) {
auto Actor = * (uintptr_t *)(Actors + (i * sizeof(uintptr_t)));
if (Actor && Tools::IsPtrValid((void *)Actor))
result.push_back(Actor);}
return result;}
std::string getObjectName(uintptr_t addr) {
std::string result;
int Index = * (int *)(addr + Object_NamePrivate_ComparisonIndex);
static std::map<int, std::string> cache;
if (cache.find(Index) != cache.end()) {
return cache[Index];}
uintptr_t GNames = ((uintptr_t ( *)())(UE4 + GNames_Offset))();
if (GNames) {
uintptr_t Chunk = * (uintptr_t *)(GNames + (Index / 0x4000) * sizeof(uintptr_t));
uintptr_t WithinChunk = * (uintptr_t *)(Chunk + (Index % 0x4000) * sizeof(uintptr_t));
char AnsiName[1024] {0};
memcpy(AnsiName, (void *)(WithinChunk + FNameEntry_AnsiName), sizeof(AnsiName));
result = AnsiName;
cache[Index] = result;}
return result;}
bool isObjectA(uintptr_t addr, const char * name) {
uintptr_t ClassPrivate = * (uintptr_t *)(addr + Object_ClassPrivate);
if (ClassPrivate) {
if (getObjectName(ClassPrivate) == name)
return true;
auto Child = * (uintptr_t *)(ClassPrivate + Class_SuperStruct);
while (Child) {
if (getObjectName(Child) == name)
return true;
Child = * (uintptr_t *)(Child + Class_SuperStruct);}}
return false;}
bool isObjectPlayer(uintptr_t addr) {
return isObjectA(addr, "STExtraPlayerCharacter");}
bool isObjectGame(uintptr_t addr) {
    return isObjectA(addr, "STExtraGameStateBase");
}
bool isObjectVehicle(uintptr_t addr) {
return isObjectA(addr, "STExtraVehicleBase");}
bool isObjectPickUp(uintptr_t addr) {
return isObjectA(addr, "PickUpWrapperActor");}

// Loot ESP filter: matches the item's live-resolved display name/category against a fixed
// list of wanted categories. Confirmed working - during testing with the filter disabled
// (showing every pickup's raw text) real gun names and 5.56mm/7.62mm ammo text came through
// correctly, so the live lookup itself is reliable; only the filter needed to be re-enabled.
static std::string ToLowerStr(const std::string &s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c){ return std::tolower(c); });
    return out;
}
static bool ContainsAnyCI(const std::string &haystackLower, std::initializer_list<const char *> needles) {
    for (auto n : needles) if (haystackLower.find(n) != std::string::npos) return true;
    return false;
}
static bool IsWantedLootItem(const std::string &name) {
    if (name.empty()) return false;
    std::string n = ToLowerStr(name);

    // Guns - "auto all guns", every weapon shown regardless of model. awm/sks/mosin are
    // already covered here too, listed explicitly since they were called out by name.
    static const std::vector<std::string> guns = {
        "akm", "m416", "scar-l", "scar", "m16a4", "groza", "aug", "qbz", "m762", "beryl",
        "ump45", "ump", "vector", "micro uzi", "uzi", "mp5k", "bizon", "pp-19", "p90",
        "s12k", "s686", "s1897", "dbs", "sawed-off",
        "kar98k", "kar98", "m24", "awm", "mosin", "sks", "mini14", "vss", "qbu", "slr",
        "deagle", "p92", "p1911", "p18c", "r45", "r1895", "skorpion", "crossbow"
    };
    for (auto &g : guns) if (n.find(g) != std::string::npos) return true;

    // Ammo - 5.56mm and 7.62mm only, as requested.
    if (ContainsAnyCI(n, {"5.56mm", "7.62mm"})) return true;

    // Medical - bandage and med kits only.
    if (ContainsAnyCI(n, {"bandage", "first aid kit", "med kit", "medkit"})) return true;

    // Drinks / boosts.
    if (ContainsAnyCI(n, {"energy drink", "painkiller"})) return true;

    // Scopes - 3x/4x/6x only.
    if (ContainsAnyCI(n, {"3x scope", "4x scope", "6x scope"})) return true;

    // Gear - backpack/helmet/vest, level 3 only. Level text format is a best guess; tell me
    // the exact wording you see in-game if a level-3 piece doesn't show and I'll adjust it.
    bool isGear = ContainsAnyCI(n, {"backpack", "helmet", "vest"});
    bool isLevel3 = ContainsAnyCI(n, {"level 3", "lvl 3", "lvl.3", "lv.3", "lv3", "level3"});
    if (isGear && isLevel3) return true;

    return false;
}
bool isObjectGrenade(uintptr_t addr) {
return isObjectA(addr, "BP_Grenade_Shoulei_C") || isObjectA(addr, "BP_Grenade_Burn_C");}
bool isObjectDeadBox(uintptr_t addr) {
return isObjectA(addr, "PickUpListWrapperActor");}
Vector3 WorldToScreen(Vector3 pos) {
Vector3 resultPos = {0, 0, 0};
auto canvasMap = * (uintptr_t *)(UE4 + CanvasMap_Offset);
if (canvasMap) {
auto Canvas = * (uintptr_t *)(canvasMap + (0x8 * 3) + 0x8);
if (Canvas) {
Matrix viewMatrix = * (Matrix *)(Canvas + Canvas_ViewProjectionMatrix);
float screenW = (viewMatrix.M[0][3] * pos.X) + (viewMatrix.M[1][3] * pos.Y) + (viewMatrix.M[2][3] * pos.Z + viewMatrix.M[3][3]);
resultPos.Z = screenW;
float screenY = (viewMatrix.M[0][1] * pos.X) + (viewMatrix.M[1][1] * pos.Y) + (viewMatrix.M[2][1] * pos.Z + viewMatrix.M[3][1]);
float screenX = (viewMatrix.M[0][0] * pos.X) + (viewMatrix.M[1][0] * pos.Y) + (viewMatrix.M[2][0] * pos.Z + viewMatrix.M[3][0]);
resultPos.Y = ((float) glHeight / 2) - ((float) glHeight / 2) * screenY / screenW;
resultPos.X = ((float) glWidth / 2) + ((float) glWidth / 2) * screenX / screenW;}}
return resultPos;}
Vector3 GetBonePos(uintptr_t Actor, int Idx) {
auto GetBonePos = (Vector3( *)(uintptr_t, uint64_t))(UE4 + GetBonePos_Offset);
auto GetBoneName = (uint64_t * ( *)(uint64_t *, uintptr_t, int))(UE4 + GetBoneName_Offset); //correct
auto Mesh = * (uintptr_t *)(Actor + Character_Mesh);
if (Mesh) {
uint64_t BoneName;
GetBoneName(&BoneName, Mesh, Idx);
return GetBonePos(Actor, BoneName);}
return {0, 0, 0};}
bool IsLineOfSightTo(uintptr_t Actor) {
auto LineOfSightTo = (bool( *)(uintptr_t, uintptr_t, Vector3, bool))( * (uintptr_t *)( * (uintptr_t *)(g_LocalController) + Controller_LineOfSightTo));
return LineOfSightTo(g_LocalController, Actor, { 0, 0, 0 }, 0);}
MinimalViewInfo GetPOV() {
if (g_LocalController) {
auto PlayerCameraManager = * (uintptr_t *)(g_LocalController + PlayerController_PlayerCameraManager);
if (PlayerCameraManager) {
CameraCacheEntry CameraCache = * (CameraCacheEntry *)(PlayerCameraManager + PlayerCameraManager_CameraCache);
return CameraCache.POV;}}
return {};}
std::string GetVehicleName(uint8_t type) {
switch (type) {
case 1: return "Bike"; break;
case 2: return "Trike"; break;
case 3: return "Dacia"; break;
case 7: return "Buggy"; break;
case 8:
case 9:
case 10:
case 13: return "UAZ"; break;
case 11: return "PG-117"; break;
case 14:
case 15: return "Mirado"; break;
case 17: return "Scooter"; break;
case 19: return "Tukshai"; break;
default:
return "Vehicle";}
return "";}
float GetDistanceTo(uintptr_t Actor) {
auto GetDistanceTo = (float( *)(uintptr_t, uintptr_t))(UE4 + GetDistanceTo_Offset);
return GetDistanceTo(Actor, g_LocalPlayer);}




auto GetTargetByCrossDist() {
    ASTExtraPlayerCharacter *result = 0;
    float max = std::numeric_limits<float>::infinity();
auto UTAMA = Zeva_Actors();
    auto ZevaPlayer = UTAM_LocalPlayer;
    auto ZevaController = UTAM_LocalController;
    // Null check alone is not enough: UTAM_LocalPlayer stays non-null but dangling
    // the instant your own character is destroyed (killed) - that use-after-free was
    // the app-crash-on-death bug.
    bool localOk = ZevaPlayer && !isObjectInvalids((SDK::UObject *)ZevaPlayer)
        && ZevaController && !isObjectInvalids((SDK::UObject *)ZevaController);
    FVector ViewPosY{0, 0, 0};
    if (localOk)
    {
        ViewPosY = ZevaPlayer->GetBonePos("Head");
        ViewPosY.Z += 10.f;
    }



    if (localOk) {
        for (int i = 0; i < UTAMA.size(); i++) {
        auto UTAMA_xD = UTAMA[i];
          //  auto Actor = Actors[i];
            if (isObjectInvalids(UTAMA_xD))
            continue;

            if (UTAMA_xD->IsA(ASTExtraPlayerCharacter::StaticClass())) {

                auto Player = (ASTExtraPlayerCharacter *) UTAMA_xD;
                float Distance = ZevaPlayer->GetDistanceTo(Player) / 100.0f;
                if (Distance > 500.0f)
                continue;
                if (Player->PlayerKey == ZevaPlayer->PlayerKey)
                continue;

                auto player = (ASTExtraPlayerCharacter *)UTAMA_xD;
                auto Target = (ASTExtraPlayerCharacter *) UTAMA_xD;
                float dist = ZevaPlayer->GetDistanceTo(Target) / 100.0f;
                if (dist > 500)
                continue;

                if (Player->TeamID == ZevaPlayer->TeamID)
                continue;

                if (Player->bDead)
                continue;


                if (Config.AimBot.IgnoreKnocked)
                {
                    if (Player->Health == 0.0f)
                    continue;
                }


                if (Config.AimBot.IgnoreBot)
                {
                    if (Player->bIsAI)
                    continue;
                }

                if (!ZevaController->LineOfSightTo(Player, ViewPosY, true))
                continue;

                float Dist = ZevaPlayer->GetDistanceTo(Player);
                if (dist < max) {
                    max = dist;
                    result = Player;
                }
            }
        }
    }

    return result;
}

FRotator ToRotator(FVector local, FVector target){
    FVector rotation = UKismetMathLibrary::Subtract_VectorVector(local, target);
    float hyp = sqrt(rotation.X * rotation.X + rotation.Y * rotation.Y);
    FRotator newViewAngle = {0};
    newViewAngle.Pitch = -atan(rotation.Z / hyp) * (180.f / (float)3.14159265358979323846);
    newViewAngle.Yaw = atan(rotation.Y / rotation.X) * (180.f / (float)3.14159265358979323846);
    newViewAngle.Roll = (float)0.f;
    if (rotation.X >= 0.f)
    newViewAngle.Yaw += 180.0f;
    return newViewAngle;
}

FVector CalculateWeaponMuzzlePosition(const FVector& weaponLocation, const FRotator& weaponRotation, float weaponBodyLength) {    
    float radPitch = weaponRotation.Pitch * (3.14159265358979323846 / 180.0f);
    float radYaw = weaponRotation.Yaw * (3.14159265358979323846 / 180.0f);        
    float forwardX = std::cos(radPitch) * std::cos(radYaw);
    float forwardY = std::cos(radPitch) * std::sin(radYaw);
    float forwardZ = std::sin(radPitch);    
    
    FVector muzzlePosition = {
        weaponLocation.X + forwardX * weaponBodyLength,
        weaponLocation.Y + forwardY * weaponBodyLength,
        weaponLocation.Z + forwardZ * weaponBodyLength
    };
    
    return muzzlePosition;
}

auto GetTargetByDistance(){
    ASTExtraPlayerCharacter *result = 0;
    float max = std::numeric_limits<float>::infinity();
    
    auto ZevaPlayer = UTAM_LocalPlayer;
    auto ZevaController = UTAM_LocalController;
   auto UTAMA = Zeva_Actors();

    // See GetTargetByCrossDist for why this needs a real validity check, not just
    // a null check - this is the fix for the app-crash-on-death bug.
    bool localOk = ZevaPlayer && !isObjectInvalids((SDK::UObject *)ZevaPlayer)
        && ZevaController && !isObjectInvalids((SDK::UObject *)ZevaController);
    FVector ViewPosY{0, 0, 0};
    if (localOk)
    {
        ViewPosY = ZevaPlayer->GetBonePos("Head");
        ViewPosY.Z += 10.f;
    }
    if (localOk)
    {
        for (int i = 0; i < UTAMA.size(); i++)
        {

        auto UTAMA_xD = UTAMA[i];
          //  auto Actor = Actors[i];
            if (isObjectInvalids(UTAMA_xD))
            continue;
            if (UTAMA_xD->IsA(ASTExtraPlayerCharacter::StaticClass()))
            {
                auto Player = (ASTExtraPlayerCharacter *)UTAMA_xD;
                if (Player->PlayerKey == ZevaPlayer->PlayerKey)
                continue;
                if (Player->TeamID == ZevaPlayer->TeamID)
                continue;
                if (Player->bDead)
                continue;
                if (Config.AimBot.IgnoreBot) {
                    if (Player->bIsAI)
                    continue;
                }

                if (Config.AimBot.VisCheck) {
                    auto CurrentWeaponReplicated = (ASTExtraShootWeapon*)ZevaPlayer->WeaponManagerComponent->CurrentWeaponReplicated;                    
                    if (CurrentWeaponReplicated) {                        
                        FVector weaponlocation = CurrentWeaponReplicated->K2_GetActorLocation();                        
                        FRotator weaponRotation = CurrentWeaponReplicated->K2_GetActorRotation();                       
                        float weaponBodyLength = CurrentWeaponReplicated->GetWeaponBodyLength();                        
                        FVector muzzlePosition = CalculateWeaponMuzzlePosition(weaponlocation, weaponRotation, weaponBodyLength / 1.9);                        
                        if (!ZevaController->LineOfSightTo(Player, muzzlePosition, true))
                        continue;                        
                    }
                }
                
                if (Config.AimBot.IgnoreKnocked) {
                    if (Player->Health == 0.0f)
                    continue;
                }                
                float dist = UTAM_LocalPlayer->GetDistanceTo(Player);
                if (dist < max)
                {
                    max = dist;
                    result = Player;
                }
            }
        }
    }
    return result;
}

ASTExtraPlayerCharacter *GetTargetForAim() {
    if (Config.AimMenu.Type == EAimType::Distance) {
        return GetTargetByDistance();
    } else if (Config.AimMenu.Type == EAimType::Croshhair) {
        return GetTargetByCrossDist();
    }
    return 0;
}

void (*orig_shoot_event)(USTExtraShootWeaponComponent *Thiz, FVector start, FRotator rot, void *unk1, int unk2) = 0;
void shoot_event(USTExtraShootWeaponComponent *thiz, FVector start, FRotator rot, ASTExtraShootWeapon *weapon, int unk1)
{

    if (Config.SilentAim.Enable)
    {
        ASTExtraPlayerCharacter *Target = GetTargetForAim();
        if (FastChut) {
            auto ShootWeaponEntityComponent = thiz->ShootWeaponEntityComponent;
            if (ShootWeaponEntityComponent) {
                ShootWeaponEntityComponent->ShootInterval = ControlSpeed;
            }
        }
        if (Target){
            FVector targetAimPos = Target->GetBonePos("Head");
           targetAimPos.Z -= -25.0f;
            FRotator sex = ToRotator(start, targetAimPos);
            return orig_shoot_event(thiz, targetAimPos, sex, weapon, unk1);
        }
    }
    return orig_shoot_event(thiz, start, rot, weapon, unk1);
}


FRotator (*oCalcShootRot)(USTExtraShootWeaponComponent *pObj);
FRotator CalcShootRot(USTExtraShootWeaponComponent *pObj)
{

    if (Config.SilentAim.Enable)
    {
        ASTExtraPlayerCharacter *Target = GetTargetForAim();
        // Native hook fired on every shot, including shots fired near you by someone
        // else. UTAM_LocalPlayer/UTAM_LocalController become dangling the instant your
        // own character is destroyed (killed) - dereferencing them unguarded here was
        // the app-crash-on-death bug.
        if (Target && UTAM_LocalPlayer && !isObjectInvalids((SDK::UObject *)UTAM_LocalPlayer)
            && UTAM_LocalController && !isObjectInvalids((SDK::UObject *)UTAM_LocalController))
        {
            bool triggerOk = false;
            if (Config.AimMenu.Trigger != EAimTrigger::None)
            {
                if (Config.AimMenu.Trigger == EAimTrigger::Shooting)
                {
                    triggerOk = UTAM_LocalPlayer->bIsWeaponFiring;
                }
                else if (Config.AimMenu.Trigger == EAimTrigger::Scoping)
                {
                    triggerOk = UTAM_LocalPlayer->bIsGunADS;
                }
                else if (Config.AimMenu.Trigger == EAimTrigger::Both)
                {
                    triggerOk = UTAM_LocalPlayer->bIsWeaponFiring && UTAM_LocalPlayer->bIsGunADS;
                }
                else if (Config.AimMenu.Trigger == EAimTrigger::Any)
                {
                    triggerOk = UTAM_LocalPlayer->bIsWeaponFiring || UTAM_LocalPlayer->bIsGunADS;
                }
            }
            else
            triggerOk = true;
            if (triggerOk)
            {
                FVector targetAimPos = Target->GetBonePos("Head");
                if (Config.AimMenu.Target == EAimTarget::HeadT)
                {
                                }
                else if (Config.AimMenu.Target == EAimTarget::Chest)
                {
               
                targetAimPos.Z -= -09.0f;//Head
                }
                UShootWeaponEntity *ShootWeaponEntityComponent = pObj->ShootWeaponEntityComponent;
                if (ShootWeaponEntityComponent)
                {
                    ASTExtraVehicleBase *CurrentVehicle = Target->CurrentVehicle;
                    if (CurrentVehicle)
                    {
                        FVector LinearVelocity = CurrentVehicle->ReplicatedMovement.LinearVelocity;
                        float dist = UTAM_LocalPlayer->GetDistanceTo(Target);
                        auto timeToTravel = dist / ShootWeaponEntityComponent->BulletFireSpeed;
                        targetAimPos = UKismetMathLibrary::Add_VectorVector(targetAimPos, UKismetMathLibrary::Multiply_VectorFloat(LinearVelocity, timeToTravel));
                    }
                    else
                    {
                        FVector Velocity = Target->GetVelocity();
                        float dist = UTAM_LocalPlayer->GetDistanceTo(Target);
                        auto timeToTravel = dist / ShootWeaponEntityComponent->BulletFireSpeed;
                        targetAimPos = UKismetMathLibrary::Add_VectorVector(targetAimPos, UKismetMathLibrary::Multiply_VectorFloat(Velocity, timeToTravel));
                    }
                    FVector fDir = UKismetMathLibrary::Subtract_VectorVector(targetAimPos, UTAM_LocalController->PlayerCameraManager->CameraCache.POV.Location);
                    return UKismetMathLibrary::Conv_VectorToRotator(fDir);
                }
            }
        }
    }
    return oCalcShootRot(pObj);
}

bool AutoFireOff2 = false;
bool AutoFire2;
bool AutoFireOff = false;
bool AutoFire;
void StopFire(uintptr_t OwnerShootWeapon) {
auto StopFire = *(void (**)(uintptr_t, uint8_t))(*(uintptr_t *)OwnerShootWeapon + 0x408);
 return StopFire(OwnerShootWeapon, 0);}
void OnPressFire() {
auto OnPressFire = (void (*)(uintptr_t))(UE4 + 0xFEB308);
return OnPressFire(g_LocalController);}
void OnReleaseFire() {
auto OnReleaseFire = (void (*)(uintptr_t))(UE4 + 0xFEB37C);
return OnReleaseFire(g_LocalController);}
void UTAMA_HUD(AHUD *HUD, int ScreenWidth, int ScreenHeight){
if (AutoFire2){
        // Null check alone is not enough: UTAM_LocalPlayer stays non-null but dangling
        // the instant your own character is destroyed (killed) - this ran every frame
        // while AUTO FIRE was on, and was the app-crash-on-death bug.
        if (UTAM_LocalPlayer && !isObjectInvalids((SDK::UObject *)UTAM_LocalPlayer) && UTAM_LocalPlayer->RootComponent
            && UTAM_LocalController && !isObjectInvalids((SDK::UObject *)UTAM_LocalController))
        {
            ASTExtraPlayerCharacter *Target;
            Target = GetTargetForAim();
            auto WeaponManagerComponent = UTAM_LocalPlayer->WeaponManagerComponent;
            if (WeaponManagerComponent)
            {
                auto propSlot = WeaponManagerComponent->GetCurrentUsingPropSlot();
                if ((int)propSlot.GetValue() >= 1 && (int)propSlot.GetValue() <= 3)
                {
                    if (AutoFireOff2)
                    {
                        UTAM_LocalController->OnReleaseFire();
                    }
                    if (Target && Target->RootComponent)
                    {
                        UTAM_LocalController->OnPressFire();
                    }
                    else
                    {
                        AutoFireOff2 = true;
                    }
                }
            }
        }
    }
    else
    {
        AutoFireOff2 = true;
    }


UCanvas *Canvas = HUD->Canvas;
SDK::ASTExtraShootWeapon *localWeapon = 0;
if (Canvas){
uintptr_t GameWorld = getGWorld();
if (GameWorld) {
 uintptr_t NetDriver = * (uintptr_t *)(GameWorld + World_NetDriver);
 if (NetDriver) {
 uintptr_t ServerConnection = * (uintptr_t *)(NetDriver + NetDriver_ServerConnection);
 if (ServerConnection) {
g_LocalController = * (uintptr_t *)(ServerConnection + Player_PlayerController); }}}
if (g_LocalController) {
 auto Actors = getActors();
 for (auto Actor : Actors) {
 if (!isObjectPlayer(Actor))
continue;
 if ( * (uint32_t *)(Actor + UAECharacter_PlayerKey) == * (uint32_t *)(g_LocalController + UAEPlayerController_PlayerKey)) {
g_LocalPlayer = Actor;
break; } }

if (UTAM_LocalPlayer && !isObjectInvalids((SDK::UObject *)UTAM_LocalPlayer)) {
// FAST MOVEMENT: a proportional engine speed-scale field (network-replicated), not a raw
// memory patch. A byte-patch speed hack fights the server's movement validation and causes
// rubber-band/stall lag - this field is how the game itself scales speed (e.g. for game
// modes), so the server accepts it and it also lifts the ADS/peek movement-speed penalty.
UTAM_LocalPlayer->CharacterOverrideAttrs.GameModeOverride_SpeedScaleModifier = FastMovementScale;
// FAST PARACHUTE: always on, no toggle needed - this feature was never ported into this
// source at all (that's why it never worked), not a bug in existing code.
auto ParachuteComp = UTAM_LocalPlayer->ParachuteComponent;
if (ParachuteComp && !isObjectInvalids((SDK::UObject *)ParachuteComp)) {
    ParachuteComp->CurrentFallSpeed = 9999.9f;
}
auto WeaponManagerComponent = UTAM_LocalPlayer->WeaponManagerComponent;
if (WeaponManagerComponent){
auto CurrentWeaponReplicated = (SDK::ASTExtraShootWeapon *)WeaponManagerComponent->CurrentWeaponReplicated;
if (CurrentWeaponReplicated){
localWeapon = CurrentWeaponReplicated;        
if (localWeapon) {    
auto ShootWeaponComponent = CurrentWeaponReplicated->ShootWeaponComponent;
if (ShootWeaponComponent) {
SDK::UShootWeaponEntity *ShootWeaponEntityComponent = ShootWeaponComponent->ShootWeaponEntityComponent;
if (ShootWeaponEntityComponent) {}}
if (AMMO && UTAM_LocalPlayer && !isObjectInvalids((SDK::UObject *)UTAM_LocalPlayer)
    && UTAM_LocalPlayer->WeaponManagerComponent
    && !UTAM_LocalPlayer->bIsGunADS   // never re-attach the mag while scoped - that action
                                       // is treated like a reload and kicks you out of ADS
    && UTAM_LocalController && !isObjectInvalids((SDK::UObject *)UTAM_LocalController))
{
    if (UTAM_LocalController->BackpackComponent
        && !isObjectInvalids((SDK::UObject *)UTAM_LocalController->BackpackComponent))
    {
        auto currentWeapon = (SDK::ASTExtraShootWeapon*)UTAM_LocalPlayer->WeaponManagerComponent->CurrentWeaponReplicated;
        if (currentWeapon && !isObjectInvalids((SDK::UObject *)currentWeapon) && currentWeapon->ShootWeaponEntityComp)
        {
            static auto lastUseTime = std::chrono::steady_clock::now();
            auto currentTime = std::chrono::steady_clock::now();
            
            if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUseTime).count() > 2000)
            {
                SDK::UBackpackUtils* utils = (SDK::UBackpackUtils*)SDK::UBackpackUtils::StaticClass();
                if (utils) 
                {
                    try {
                        
                        SDK::TArray<SDK::FBattleItemData> items = utils->GetAllItemsInBackpack(UTAM_LocalController->BackpackComponent, 1);
                         
                        for (int i = 0; i < items.Num(); i++) 
                        {
                            const SDK::FBattleItemData& item = items[i];
                            
                            
                            // Only the AR mag IDs are confirmed (204011/204013 = "Extended
                            // Quickdraw Mag (AR)"). To make single-fire weapons (sniper/SMG/
                            // shotgun/pistol) benefit from unlimited ammo too, add their real
                            // mag TypeSpecificIDs to this set - find them with the "SHOW ITEM
                            // IDS" loot toggle by dropping/picking up that weapon's magazine.
                            static const std::set<int> UnlimitedAmmoMagIDs = { 204011, 204013 };
                            if (UnlimitedAmmoMagIDs.count(item.DefineID.TypeSpecificID))
                            {
                                if (UTAM_LocalController && !isObjectInvalids((SDK::UObject *)UTAM_LocalController)
                                    && UTAM_LocalController->BackpackComponent
                                    && !isObjectInvalids((SDK::UObject *)UTAM_LocalController->BackpackComponent))
                                {
                                  
                                    UTAM_LocalController->ServerUseItem(
                                        item.DefineID, 
                                        SDK::FBattleItemUseTarget(), 
                                        (SDK::EBattleItemUseReason)0
                                    );
                                                                       
                                    lastUseTime = currentTime;
                                    break; 
                                }
                            }
                        }
                    } 
                    catch (...) {
                        
                    }
                }
            }
        }
    }
}
}
}
}
}

if(UTAM_LocalPlayer&&!isObjectInvalids((SDK::UObject*)UTAM_LocalPlayer)&&UTAM_LocalController&&!isObjectInvalids((SDK::UObject*)UTAM_LocalController)&&UTAMKILLMSG){
auto WeaponManagerComponent=UTAM_LocalPlayer->WeaponManagerComponent;
if(WeaponManagerComponent){
auto CurrentWeaponReplicated=(ASTExtraShootWeapon*)WeaponManagerComponent->CurrentWeaponReplicated;
if(CurrentWeaponReplicated){
auto Records=UTAM_LocalController->ClientFatalDamageRecords;
std::string myName=UTAM_LocalPlayer->PlayerName.ToString();
for(int i=0;i<Records.Num();i++){
auto& record=Records[i];
if(record.Causer.ToString()==myName){
const char* weaponName=CurrentWeaponReplicated->GetWeaponName().ToString();
if(strstr(weaponName,"AKM"))record.CauserWeaponAvatarID=1101001089;
else if(strstr(weaponName,"M416"))record.CauserWeaponAvatarID=1101004062;
else if(strstr(weaponName,"G36C"))record.CauserWeaponAvatarID=1101002056;
else if(strstr(weaponName,"SCAR-L"))record.CauserWeaponAvatarID=1101003057;
else if(strstr(weaponName,"M762"))record.CauserWeaponAvatarID=1101008026;
else if(strstr(weaponName,"GROZA"))record.CauserWeaponAvatarID=1101005019;
else if(strstr(weaponName,"UZI"))record.CauserWeaponAvatarID=1102001024;
else if(strstr(weaponName,"UMP45"))record.CauserWeaponAvatarID=1102002053;
else if(strstr(weaponName,"Vector"))record.CauserWeaponAvatarID=1102003020;
else if(strstr(weaponName,"Thompson"))record.CauserWeaponAvatarID=1102004018;
else if(strstr(weaponName,"PP-19 Bizon"))record.CauserWeaponAvatarID=1102005007;
else if(strstr(weaponName,"Kar98K"))record.CauserWeaponAvatarID=1103001060;
else if(strstr(weaponName,"M24"))record.CauserWeaponAvatarID=1103002018;
else if(strstr(weaponName,"AWM"))record.CauserWeaponAvatarID=1103003022;
else if(strstr(weaponName,"DP28"))record.CauserWeaponAvatarID=1105002018;
else if(strstr(weaponName,"M16A4"))record.CauserWeaponAvatarID=1101002029;
else if(strstr(weaponName,"QBZ"))record.CauserWeaponAvatarID=1101007025;
else if(strstr(weaponName,"M249"))record.CauserWeaponAvatarID=1105001020;
else if(strstr(weaponName,"Mini 14"))record.CauserWeaponAvatarID=1101007025;
else if(strstr(weaponName,"SLR"))record.CauserWeaponAvatarID=1103009022;
else if(strstr(weaponName,"SKS"))record.CauserWeaponAvatarID=1103004037;
else if(strstr(weaponName,"Pan"))record.CauserWeaponAvatarID=1108004125;
else if(strstr(weaponName,"Vss"))record.CauserWeaponAvatarID=1103005024;
else if(strstr(weaponName,"S1897"))record.CauserWeaponAvatarID=1104002022;
else if(strstr(weaponName,"S12K"))record.CauserWeaponAvatarID=1104003026;}}}}}

if (g_LocalPlayer) {
auto infinity = std::numeric_limits<float>::infinity();
 auto WeaponManagerComponent = *(uintptr_t *) (g_LocalPlayer + 0x117c);
 if (AutoFire) {
if ((g_LocalPlayer && *(uintptr_t *) (g_LocalPlayer + 0x140)) && g_LocalController){
auto Target = GetTargetForAim();
auto WeaponManagerComponent = *(uintptr_t *) (g_LocalPlayer + 0x117c);
if (WeaponManagerComponent){
 auto propSlot = (uint8_t( *)(uintptr_t))(UE4 + 0x12dbd50);
 if (propSlot(WeaponManagerComponent) >= 1 && propSlot(WeaponManagerComponent) <= 3) {
if (AutoFireOff){
OnReleaseFire();}
if (Target && *(uintptr_t *) (Target + 0x140)){
 OnPressFire();}
else{
 AutoFireOff = true;}}}}}
else{
AutoFireOff = true;}}}}}
int OpenURL(const char* url){
JavaVM* java_vm = g_App->activity->vm;
JNIEnv* java_env = NULL;
jint jni_return = java_vm->GetEnv((void**)&java_env, JNI_VERSION_1_6);
if (jni_return == JNI_ERR)
 return -1;
jni_return = java_vm->AttachCurrentThread(&java_env, NULL);
if (jni_return != JNI_OK)
 return -2;
jclass native_activity_clazz = java_env->GetObjectClass(g_App->activity->clazz);
if (native_activity_clazz == NULL)
 return -3;
jmethodID method_id = java_env->GetMethodID(native_activity_clazz, "AndroidThunkJava_LaunchURL", "(Ljava/lang/String;)V");
if (method_id == NULL)
 return -4;
jstring retStr = java_env->NewStringUTF(url);
java_env->CallVoidMethod(g_App->activity->clazz, method_id, retStr);
jni_return = java_vm->DetachCurrentThread();
if (jni_return != JNI_OK)
 return -5;
return 0;}
void DrawText(ImDrawList *draw, const std::string &text, const Vector2 &position, ImU32 color, float fontSize)
{
    draw->AddText(NULL, fontSize, {position.X, position.Y}, color, text.c_str());
}
void DrawTextWithBorder(ImDrawList *draw, const std::string &text, const Vector2 &position, ImU32 textColor, ImU32 borderColor, float fontSize)
{
    float borderSize = 1.0f;

    for (int x = -1; x <= 1; ++x)
    {
for (int y = -1; y <= 1; ++y)
{
    if (x == 0 && y == 0)
continue;

    DrawText(draw, text, {position.X + x * borderSize, position.Y + y * borderSize}, borderColor, fontSize);
}
    }
    DrawText(draw, text, position, textColor, fontSize);
}

bool (*orig_GetUnlimitedAmmo)(int* miri);
bool hk_GetUnlimitedAmmo(int* miri) {
    if(AMMO){
      return true;
    }
    return orig_GetUnlimitedAmmo(miri);
}
ImColor 紫色2 = ImColor(255,0,255);
ImColor 浅蓝 = ImColor(ImVec4(36/255.f, 249/255.f, 217/255.f, 255/255.f));
ImColor 蓝色 = ImColor(ImVec4(170/255.f, 203/255.f, 244/255.f, 0.95f));
ImColor 白色 = ImColor(ImVec4(255/255.f, 255/255.f, 258/255.f, 0.95f));
ImColor 浅粉 = ImColor(ImVec4(255/255.f, 200/255.f, 250/255.f, 0.95f));
ImColor 黑色 = ImColor(ImVec4(0/255.f, 0/255.f, 0/255.f, 0.7f));
ImColor 半黑 = ImColor(ImVec4(0/255.f, 0/255.f, 0/255.f, 0.18f));
ImColor 血色 = ImColor(ImVec4(0/255.f, 249/255.f, 0/255.f, 0.35f));
ImColor 红色 = ImColor(ImVec4(233/255.f, 55/255.f, 51/255.f, 0.95f));
ImColor 绿色 = ImColor(ImVec4(50/255.f, 222/215.f, 50/255.f, 0.95f));
ImColor 黄色 = ImColor(ImVec4(255/255.f, 255/255.f, 0/255.f, 0.95f));
ImColor 橘黄 = ImColor(ImVec4(255/255.f, 150/255.f, 30/255.f, 0.95f));
ImColor 粉红 = ImColor(ImVec4(220/255.f, 108/255.f, 1202/255.f, 0.95f));
ImColor 紫色 = ImColor(ImVec4(169/255.f, 120/255.f, 223/255.f, 0.95f));
ImColor 空白 = ImColor(ImVec4(1.0/255.f, 1.0/255.f, 1.0/255.f, 0.0f));
ImColor 青绿 = ImColor(ImVec4(145/255.f,232/255.f,135/255.f,255/255.f));
void HueText(const char* text, ImVec4 color){
static auto start_time = std::chrono::high_resolution_clock::now();
auto elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count() / 1000.0f; 
ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0,255)); //ImVec ji4(1.0f, 1.0f, 1.0f, 0.0f)); 
for (int i = 0; i < strlen(text); i++){
ImGui::SameLine();
float t = fmodf(elapsed_seconds + (float)i / (float)strlen(text), 1.0f); 
ImVec4 currentColor = ImVec4(color.x * t, color.y * t, color.z * t, color.w); 
ImGui::PushStyleColor(ImGuiCol_Text, currentColor); 
ImGui::Text("%c", text[i]); 
ImGui::PopStyleColor(); }
ImGui::PopStyleColor(); }
static float 旋转角度 = 0.0f;
const float 旋转速度 = 0.09999f;
auto 绘制彩虹FOV = [](float 半径, float 旋转 = 0.75f, int 段数 = 100) -> void {
// ImVec2 中心 = ImGui::GetIO().DisplaySize / 2; //abhi abhi
 ImVec2 中心 = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);
for (int i = 0; i < 段数; ++i) {
float 起始角度 = (i / (float)段数) * 2.0f * IM_PI + 旋转;
float 结束角度 = ((i + 1) / (float)段数) * 2.0f * IM_PI + 旋转;
ImGui::GetBackgroundDrawList()->AddLine(
ImVec2(cos(起始角度) * 半径 + 中心.x, sin(起始角度) * 半径 + 中心.y),
ImVec2(cos(结束角度) * 半径 + 中心.x, sin(结束角度) * 半径 + 中心.y),
ImColor::HSV((float)i / (float)段数, 1.0f, 1.0f),
1.0f
);
}
};
	#define CREATE_COLOR(r, g, b, a) new float[4] {(float)r, (float)g, (float)b, (float)a};

namespace Active
{
inline int lastMotorcycleID = 0;
inline int newMotorcycleID = 0;
inline int lastScooterID = 0;
inline int newScooterID = 0;
inline int lastmonstertruckID = 0;
inline int newmonstertruckID = 0;
inline int lastBuggyID = 0;
inline int newBuggyID = 0;
inline int lastUazID = 0;
inline int newUazID = 0;
inline int lastDaciaID = 0;
inline int newDaciaID = 0;
inline int lastCoupeID = 0;
inline int newCoupeID = 0;
}

long Colorpoints(int index) {
    srand(index);

    int a = 255;
    int r = ((Color[rand() % sizeof(Color)] & 0xFF0000) >> 17);
    int g = ((Color[rand() % sizeof(Color)] & 0x00FF00) >> 9);
    int b = (Color[rand() % sizeof(Color)] & 0x0000FF);

    return IM_COL32(r, g, b, a);
}
long Colorpoints1(int index) {
    srand(index);

    int a = 130;
    int r = ((Color[rand() % sizeof(Color)] & 0xFF0000) >> 17);
    int g = ((Color[rand() % sizeof(Color)] & 0x00FF00) >> 9);
    int b = (Color[rand() % sizeof(Color)] & 0x0000FF);

    return IM_COL32(r, g, b, a);
}

// kFox's memory-scan engine (MemorySearch/MemoryOffset/MemoryWrite/ClearResult) keeps its
// candidate-result list in global state and is not reentrant. Two of these case blocks
// (e.g. HIGH DAMAGE and LONG HAND) can be triggered close together - one from a menu click,
// one from the per-match auto-trigger - and if their threads overlap, one thread's
// ClearResult() frees the buffer the other is still reading, crashing inside
// ScanEngine.cpp's MemoryOffset_FLOAT. A mutex around ONLY the kFox calls (see g_ScanEngineMutex
// usages below, in cases 6/850/1001) fixes that without affecting anything else. This must NOT
// wrap the whole function - cases 101/102 (God View) spin in an infinite loop for as long as
// their toggle is on, and locking the whole function here previously made that thread hold
// the lock indefinitely, deadlocking every other feature (Long Hand, High Damage, NEW FLASH,
// SKY FLY...) for as long as God View stayed on. Fixed by never letting a lock span an
// unbounded loop - the God View logic itself is unchanged and untouched below.
static std::mutex g_ScanEngineMutex;
void *run_thread(void *arguments)
{
switch ((int)arguments)
{
int dValue;
float fValue;
//======================================================//
case 4451:
if (UTAM_FLY){
long c1 = getPointer(getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x18) + 0x1C8)+0x3C8;
long c3 = getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x2CC)+0x60;
long c2 = getPointer(getPointer(getPointer(getPointer(UE4 + 0x5090DC8) + 0x20) + 0x2CC)+0x310)+0x1AC;
writefloat(c2, 99999);
writefloat(c1, 1.15);
writefloat(c3, 1.35);
} else {
long d1 = getPointer(getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x18) + 0x1C8)+0x3C8;
long d3 = getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x2CC)+0x60;
long d2 = getPointer(getPointer(getPointer(getPointer(UE4 + 0x5090DC8) + 0x20) + 0x2CC)+0x310)+0x1AC;
long d4 = getPointer(getPointer(UE4 + 0x50B7600) + 0xA8)+0x30;
writefloat(d2, 8192);
writefloat(d4,-980);
writefloat(d1, 1);
writefloat(d3, 1);
}
break;


case 4449:
if(UTAM_FLASH){
long a1 = getPointer(UE4 + 0x50A5EAC) +0x20;//145
long a2 = getPointer(UE4 + 0x50A5EAC) +0x24;//145
long a3 = getPointer(getPointer(getPointer(UE4 + 0x4F08EB8) + 0xA8) + 0x24C)+0x24;
long a4 = getPointer(getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x2CC) + 0x310)+0x138;
long a5 = getPointer(getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x18) + 0x1C8)+0x3C8;
long a6 = getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x2CC)+0x60;
long a7 = getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x2CC)+0x14F4;
long a8 = getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x2CC)+0x14F0;
long a9 = getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x2CC)+0x1558;// fix 
writefloat(a1,165);
writefloat(a2,165);
writefloat(a3,1000000);
writefloat(a4,100);
writefloat(a5,1.25);
writefloat(a6,1.41);
writefloat(a7,9999);
writefloat(a8,9999);
writefloat(a9,75); // fix 
Patches.utam01.Modify();
Patches.utam02.Modify();
Patches.utam03.Modify();
Patches.utam04.Modify();
Patches.utam05.Modify();
Patches.utam06.Modify();
Patches.utam07.Modify();
Patches.utam08.Modify();
Patches.utam09.Modify();
Patches.utam10.Modify();        
Patches.utam11.Modify();
Patches.utam12.Modify();
Patches.utam13.Modify();
} else {
Patches.utam01.Restore();
Patches.utam02.Restore();
Patches.utam03.Restore();
Patches.utam04.Restore();
Patches.utam05.Restore();
Patches.utam06.Restore();
Patches.utam07.Restore();
Patches.utam08.Restore();
Patches.utam09.Restore();
Patches.utam10.Restore();        
Patches.utam11.Restore();
Patches.utam12.Restore();
Patches.utam13.Restore();
long a1 = getPointer(getPointer(getPointer(UE4 + 0x4F08EB8) + 0xA8) + 0x24C)+0x24;
long a2 = getPointer(getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x18) + 0x1C8)+0x3C8;
long a3 = getPointer(getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x2CC) + 0x310)+0x138;
long a5 = getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x2CC)+0x60;
long a7 = getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x2CC)+0x14F4;
long a8 = getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x2CC)+0x14F0;
long a9 = getPointer(getPointer(getPointer(UE4 + 0X5090DC8) + 0x20) + 0x2CC)+0x1558; // fix
writefloat(a1,10000);
writefloat(a2,1);
writefloat(a3,35);
writefloat(a5,1);
writefloat(a7,479.5);
writefloat(a8,1);
writefloat(a9,88); // fix
}
break;
								


case 6://HEADSHOT ON
if (MAX_DAMAGE){
long f1 = getPointer(UE4 + 0x50A5EAC) +0x20;
long f2 = getPointer(UE4 + 0x50A5EAC) +0x24;
writefloat(f1, 999999);
writefloat(f2, 999999);
{
std::lock_guard<std::mutex> scanEngineLock(g_ScanEngineMutex);
kFox::SetSearchRange(RegionType::ANONYMOUS);
kFox::MemorySearch("23",Type::TYPE_FLOAT);
kFox::MemoryOffset("25",4,Type::TYPE_FLOAT);
kFox::MemoryOffset("30.5",8,Type::TYPE_FLOAT);
kFox::MemoryWrite("140",0,Type::TYPE_FLOAT);
kFox::MemoryWrite("140",4,Type::TYPE_FLOAT);
kFox::MemoryWrite("140",8,Type::TYPE_FLOAT);
kFox::ClearResult();
kFox::SetSearchRange(RegionType::ANONYMOUS);
kFox::MemorySearch( "10.0", Type::TYPE_FLOAT);
kFox::MemoryOffset( "46.0", 4, Type::TYPE_FLOAT);
kFox::MemoryWrite( "99999", 0, Type::TYPE_FLOAT);
kFox::ClearResult();
kFox::SetSearchRange(RegionType::ANONYMOUS);
kFox::MemorySearch("3000.0",Type::TYPE_FLOAT);
kFox::MemoryOffset("5000.0",4,Type::TYPE_FLOAT);
kFox::MemoryOffset("1024.0",8,Type::TYPE_FLOAT);
kFox::MemoryOffset("89.0",36,Type::TYPE_FLOAT);
kFox::MemoryWrite("200000",0,Type::TYPE_FLOAT);
kFox::MemoryWrite("200000",4,Type::TYPE_FLOAT);
kFox::MemoryWrite("200000",8,Type::TYPE_FLOAT);
kFox::ClearResult();// FAST LANDING
MemoryPatch::createWithHex("libUE4.so", 0x21857B4,"00 00 00 00").Modify(); //small
MemoryPatch::createWithHex("libUE4.so", 0xDB51F8, "00 00 02 43").Modify(); //
MemoryPatch::createWithHex("libUE4.so", 0x1172DE4, "00 A0 A0 E3").Modify();
MemoryPatch::createWithHex("libUE4.so", 0x1172718, "00 00 00 00").Modify();
kFox::SetSearchRange(RegionType::ANONYMOUS);
kFox::MemorySearch("10.0", Type::TYPE_FLOAT);
kFox::MemoryOffset("46.0", 4, Type::TYPE_FLOAT);
kFox::MemoryWrite("2700.0", 0, Type::TYPE_FLOAT);
kFox::ClearResult();
// The "HIT EFFECT" block that used to be here (search "23" -> offset 25@4, 30.5@8 -> write
// 140/140/140) was a byte-for-byte duplicate of the search/offset/write sequence already
// done above in this same case - it changed nothing, since the first pass already wrote
// those exact targets. It was also the confirmed crash site (via logcat+addr2line, twice,
// across two different builds) - the redundant repeat of a "search a common float value
// across all of process memory" scan doubled the odds of one candidate's offset landing
// outside a mapped page. Removed rather than re-locked, since a mutex only prevents two
// threads corrupting each other's scan state - it does nothing for an out-of-bounds read
// inside a single, unshared, already-redundant scan.
}
Patches.noshake1.Modify();
Patches.noshake2.Modify();
Patches.noshake3.Modify();
Patches.noshake4.Modify();
Patches.noshake5.Modify();
Patches.noshake6.Modify();
} else {
Patches.noshake1.Restore();
Patches.noshake2.Restore();
Patches.noshake3.Restore();
Patches.noshake4.Restore();
Patches.noshake5.Restore();
Patches.noshake6.Restore();
}
break;

case 850: { // LONG HAND (extended melee/reach) - auto-triggered once per match
std::lock_guard<std::mutex> scanEngineLock(g_ScanEngineMutex);
kFox::SetSearchRange(RegionType::ANONYMOUS);
kFox::MemorySearch("0.0060901641845703125", Type::TYPE_FLOAT);
kFox::MemoryOffset("23.52225685119629", -8, Type::TYPE_FLOAT);
kFox::MemoryOffset("2.2840499877929688E-4", -4, Type::TYPE_FLOAT);
kFox::MemoryWrite("100", -8, Type::TYPE_FLOAT);
kFox::MemoryWrite("200", -4, Type::TYPE_FLOAT);
kFox::ClearResult();
break;
}

case 1001:
if (SKYSHOT){
{
std::lock_guard<std::mutex> scanEngineLock(g_ScanEngineMutex);
kFox::MemorySearch("4719772412750681353",Type::TYPE_QWORD);
kFox::MemoryOffset("4742290408720039936",4, Type::TYPE_QWORD);
kFox::MemoryWrite(" -460", 8, Type::TYPE_FLOAT);
kFox::ClearResult(); //Running Shot
}
Write_Float(UE4 + 0x26F9C00, 0.0f);
} else {
Write_Float(UE4 + 0x26F9C00, -3.74440972e28);
}
break;

case 101:
if (Godviewup) {
auto lund = (g_LocalPlayer + 0x140);
auto lundd = *(uintptr_t*)lund + 0x158;
float up;
Tools::PVM_ReadAddr((void *)(lundd), & up, sizeof(up));
while (true) {
Write<float>(lundd, up + 295);
if (!Godviewup) {
break;
}}}
case 102:
if (Godviewdown) {
auto lund = (g_LocalPlayer + 0x140);
auto lundd = *(uintptr_t*)lund + 0x158;
float up;
Tools::PVM_ReadAddr((void *)(lundd), & up, sizeof(up));
while (true) {
Write<float>(lundd, up - 295);
if (!Godviewdown) {
break;
}}}

    }

    return 0;
}

ImFont* g_fa_solid = nullptr;
ImFont* g_big_font = nullptr;

struct ThanosParticle {
    ImVec2 pos;
    ImVec2 vel;
    float life;
    float radius;
    ImU32 col;
};

static float thanos_timer = 0.0f;
static float cyan_timer = 0.0f;
static std::vector<ThanosParticle> particles;
static bool particles_spawned = false;
static const float visibleDuration = 5.0f;
static const float disDuration = 3.0f;
static const float cycleDuration = visibleDuration + disDuration;
static const int particlesPerLine = 250;

static float randf(float a, float b) {
    return a + (b - a) * ((float)rand() / RAND_MAX);
}

// ===== VAPORWAVE COLOR GENERATOR =====
ImU32 GetCyanColor(float t) {
    float shimmer = (sinf(t * 2.5f) + 1.0f) * 0.5f;
    // Cyan ki jagah Hot Pink se Neon Magenta glow
    float r = 215.0f + (255.0f - 215.0f) * shimmer;
    float g = 25.0f  + (80.0f  - 25.0f)  * shimmer;
    float b = 115.0f + (150.0f - 115.0f) * shimmer;
    return IM_COL32((int)r, (int)g, (int)b, 255);
}

void DrawTextOutlined(ImDrawList* draw, ImVec2 pos, ImU32 col, const char* text, float size = 26.0f) {
    // Darker Purple outline for high contrast
    ImU32 outline = IM_COL32(25, 5, 15, 255);
    draw->AddText(NULL, size, ImVec2(pos.x - 2, pos.y - 2), outline, text);
    draw->AddText(NULL, size, ImVec2(pos.x + 2, pos.y - 2), outline, text);
    draw->AddText(NULL, size, ImVec2(pos.x - 2, pos.y + 2), outline, text);
    draw->AddText(NULL, size, ImVec2(pos.x + 2, pos.y + 2), outline, text);
    draw->AddText(NULL, size, pos, col, text);
}

void DrawPinkThanosText(const char* l1, const char* l2, const char* l3) {
    ImDrawList* draw = ImGui::GetForegroundDrawList();
    ImVec2 p1(20, 560);
    ImVec2 p2(20, 595);
    ImVec2 p3(20, 630);

    cyan_timer += 0.02f;
    thanos_timer += 0.016f;

    if (thanos_timer > cycleDuration) {
        thanos_timer = 0.0f;
        particles.clear();
        particles_spawned = false;
    }

    bool visible = (thanos_timer <= visibleDuration);
    bool dis = !visible;

    if (dis && !particles_spawned) {
        particles_spawned = true;
        ImVec2 s1 = ImGui::CalcTextSize(l1);
        ImVec2 s2 = ImGui::CalcTextSize(l2);
        ImVec2 s3 = ImGui::CalcTextSize(l3);

        auto spawn = [&](ImVec2 pos, ImVec2 size) {
            for (int i = 0; i < particlesPerLine; i++) {
                ThanosParticle p;
                p.pos = ImVec2(pos.x + randf(0, size.x), pos.y + randf(0, size.y));
                p.vel = ImVec2(randf(-25, 25), randf(-70, -25));
                p.life = 1.0f;
                p.radius = randf(1.5f, 3.5f);
                p.col = GetCyanColor(cyan_timer + randf(0.0f, 1.0f));
                particles.push_back(p);
            }
        };

        spawn(p1, s1);
        spawn(p2, s2);
        spawn(p3, s3);
    }

    if (visible) {
        ImU32 cyan = GetCyanColor(cyan_timer);
        DrawTextOutlined(draw, p1, cyan, l1, 26.0f);
        DrawTextOutlined(draw, p2, cyan, l2, 26.0f);
        DrawTextOutlined(draw, p3, cyan, l3, 26.0f);
    }

    if (dis) {
        for (auto& p : particles) {
            p.vel.x *= 0.98f;
            p.vel.y += 180.0f * 0.016f;
            p.pos.x += p.vel.x * 0.016f;
            p.pos.y += p.vel.y * 0.016f;
            p.life -= 0.016f / disDuration;
            if (p.life < 0) p.life = 0;
            int a = (int)(p.life * 255.0f);
            ImU32 finalCol = (p.col & 0x00FFFFFF) | ((ImU32)a << 24);
            draw->AddCircleFilled(p.pos, p.radius, finalCol);
        }
    }
}

ImVec2 GetPointOnBorder(float distance, float w, float h, float x, float y) {
    float perimeter = 2 * (w + h);
    distance = fmodf(distance, perimeter);
    if (distance < 0) distance += perimeter;
    if (distance < w) return ImVec2(x + distance, y);
    else if (distance < w + h) return ImVec2(x + w, y + (distance - w));
    else if (distance < 2 * w + h) return ImVec2(x + w - (distance - (w + h)), y + h);
    else return ImVec2(x, y + h - (distance - (2 * w + h)));
}

void DrawPinkMeteorBorder(float thickness) {
    ImVec2 pos = ImGui::GetWindowPos();
    ImVec2 size = ImGui::GetWindowSize();
    auto* draw = ImGui::GetForegroundDrawList();

    static float time = 0.0f;
    time += ImGui::GetIO().DeltaTime * 2.5f;
    float glow = (sinf(time * 2.0f) + 1.0f) * 0.5f;

    float x = pos.x;
    float y = pos.y;
    float w = size.x;
    float h = size.y;
    float rounding = 6.0f;

    // Pink / Purple Neon Meteor Glow Values
    int r1 = (int)(180 + 75 * glow);
    int g1 = (int)(20  + 30 * glow);
    int b1 = (int)(90  + 40 * glow);

    int r2 = (int)(215 + 40 * glow);
    int g2 = (int)(30  + 35 * glow);
    int b2 = (int)(115 + 35 * glow);

    int r3 = (int)(255);
    int g3 = (int)(50  + 50 * glow);
    int b3 = (int)(150 + 50 * glow);

    draw->AddRect(pos, ImVec2(x + w, y + h), IM_COL32(r1, g1, b1, 120), rounding, 0, thickness + 1.5f);
    draw->AddRect(ImVec2(x + thickness / 2, y + thickness / 2), ImVec2(x + w - thickness / 2, y + h - thickness / 2), IM_COL32(r2, g2, b2, 200), rounding, 0, thickness);
    draw->AddRect(ImVec2(x - thickness / 2, y - thickness / 2), ImVec2(x + w + thickness / 2, y + h + thickness / 2), IM_COL32(r3, g3, b3, 160), rounding, 0, 1.2f);
}

void DrawPinkBackground()
{
    ImDrawList* pDrawList = ImGui::GetWindowDrawList();

    const ImVec2 windowPos  = ImGui::GetWindowPos();
    const ImVec2 windowSize = ImGui::GetWindowSize();

    float time = ImGui::GetTime();

    // =========================================
    // PINK GLOW / PULSE FLICKER
    // =========================================
    auto PinkPulse = [&](float offset, float speed)
    {
        return (int)(
            30.0f +
            40.0f *
            (0.5f + 0.5f * sinf(time * speed + offset))
        );
    };

    // =========================================
    // VAPORWAVE HOT PINK BACKGROUND COLORS 🌸
    // =========================================

    // Deep Dark Velvet Base (Window Background)
    ImU32 colorDarkPinkBase = IM_COL32(23, 5, 15, 255);

    // Hot Pink Top Accent
    ImU32 colorPink1 = IM_COL32(
        255,
        50 + PinkPulse(0.0f, 3.0f),
        150,
        255
    );

    // Deep Magenta Accent
    ImU32 colorPink2 = IM_COL32(
        215,
        30 + PinkPulse(1.5f, 2.5f),
        115,
        255
    );

    // Rich Dark Plum / Purple Base
    ImU32 colorMaroon = IM_COL32(45, 10, 30, 255);

    // =========================================
    // MAIN PINK GRADIENT BACKGROUND
    // =========================================
    pDrawList->AddRectFilledMultiColor(
        windowPos,
        ImVec2(
            windowPos.x + windowSize.x,
            windowPos.y + windowSize.y
        ),

        colorDarkPinkBase, // Top Left
        colorPink1,        // Top Right

        colorMaroon,       // Bottom Right
        colorPink2         // Bottom Left
    );

    // =========================================
    // PINK GLOW OVERLAY
    // =========================================
    for (int i = 0; i < 4; i++)
    {
        float glowExpand = (float)i * 2.0f;

        pDrawList->AddRect(
            ImVec2(
                windowPos.x - glowExpand,
                windowPos.y - glowExpand
            ),

            ImVec2(
                windowPos.x + windowSize.x + glowExpand,
                windowPos.y + windowSize.y + glowExpand
            ),

            IM_COL32(
                255,
                30,
                130,
                35 - i * 7
            ),

            12.0f,
            0,
            2.0f
        );
    }

    // =========================================
    // NEON WAVE EFFECT
    // =========================================
    for (int i = 0; i < 6; i++)
    {
        float y =
            windowPos.y +
            40.0f +
            i * 55.0f;

        float wave =
            sinf(time * 3.0f + i) * 12.0f;

        pDrawList->AddBezierCubic(
            ImVec2(windowPos.x + 10, y),

            ImVec2(
                windowPos.x + windowSize.x * 0.25f,
                y + wave
            ),

            ImVec2(
                windowPos.x + windowSize.x * 0.75f,
                y - wave
            ),

            ImVec2(
                windowPos.x + windowSize.x - 10,
                y
            ),

            IM_COL32(
                255,
                50,
                150,
                25
            ),

            2.0f
        );
    }

    // =========================================
    // PINK EMBERS & GLOW PARTICLES
    // =========================================
    const int emberCount = 150;

    for (int i = 0; i < emberCount; i++)
    {
        float seed = i * 91.713f;

        float x =
            windowPos.x +
            fmodf(seed * 17.0f, windowSize.x);

        float speed =
            20.0f +
            fmodf(seed, 60.0f);

        float y =
            windowPos.y +
            windowSize.y -
            fmodf(time * speed + seed * 5.0f,
                   windowSize.y + 40.0f);

        float size =
            1.0f +
            fmodf(seed, 2.5f);

        ImU32 emberColor;

        if (i % 2 == 0)
        {
            emberColor = IM_COL32(255, 120, 190, 150); // Light Pink Ember
        }
        else
        {
            emberColor = IM_COL32(215, 30, 115, 120);  // Magenta Ember
        }

        ImVec2 p(x, y);

        // Ember dot
        pDrawList->AddCircleFilled(
            p,
            size,
            emberColor,
            16
        );

        // Ember trail
        pDrawList->AddLine(
            p,
            ImVec2(x, y + 8),

            (emberColor & 0x00FFFFFF) | 0x18000000,

            1.0f
        );
    }

    // =========================================
    // TOP HOT PINK GLOW ACCENT
    // =========================================
    pDrawList->AddRectFilledMultiColor(
        windowPos,

        ImVec2(
            windowPos.x + windowSize.x,
            windowPos.y + 80
        ),

        IM_COL32(255, 50, 150, 70),
        IM_COL32(215, 30, 115, 30),

        IM_COL32(0, 0, 0, 0),
        IM_COL32(0, 0, 0, 0)
    );
}

void DrawPinkGlowShimmerLine() {
    ImDrawList* draw = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float width = ImGui::GetContentRegionAvail().x;

    const int segments = 120;
    float segmentLength = width / segments;
    float time = ImGui::GetTime();

    // Dark Purple base to Bright Vaporwave Pink
    ImVec4 colorDarkBase = ImVec4(45.0f / 255.0f, 10.0f / 255.0f, 30.0f / 255.0f, 1.0f);
    float brightness = 0.8f + 0.4f * (0.5f + 0.5f * sinf(time * 2.0f));
    ImVec4 colorBrightPink = ImVec4((255.0f / 255.0f) * brightness, (50.0f / 255.0f) * brightness, (150.0f / 255.0f) * brightness, 1.0f);

    static float flowOffset = 0.0f;
    flowOffset += 0.004f;
    if (flowOffset > 1.0f) flowOffset -= 1.0f;

    float baseThickness = 5.0f;
    float y = pos.y + baseThickness / 2.0f;

    for (int i = 0; i < segments; i++) {
        float t = (float)i / segments;
        float dist = fabsf(t - 0.5f) * 2.0f;
        float thickness = baseThickness * (1.0f - (dist * 0.7f));
        float wave = (sinf((t + flowOffset) * 3.14159f * 2.0f) + 1.0f) * 0.5f;

        ImVec4 finalColor;
        finalColor.x = colorDarkBase.x * (1.0f - wave) + colorBrightPink.x * wave;
        finalColor.y = colorDarkBase.y * (1.0f - wave) + colorBrightPink.y * wave;
        finalColor.z = colorDarkBase.z * (1.0f - wave) + colorBrightPink.z * wave;
        finalColor.w = 1.0f;

        ImU32 col = ImGui::ColorConvertFloat4ToU32(finalColor);
        draw->AddLine(ImVec2(pos.x + i * segmentLength, y), ImVec2(pos.x + (i + 1) * segmentLength, y), col, thickness);
    }

    ImGui::Dummy(ImVec2(0, baseThickness + 2.0f));
}
//=========≠================≠==============DrawEsp================≠==============≠================≠=======
void DrawEsp(ImDrawList * draw) {
MemoryPatch::createWithHex("libanogs.so", 0x30D21C,"00 00 00 00").Modify();  // 10 year blocker
MemoryPatch::createWithHex("libanogs.so", 0x30D220,"00 00 00 00").Modify();  // 10 year blocker
MemoryPatch::createWithHex("libanogs.so", 0x49904,"00 20 70 47").Modify();  // 10 year blocker
//MemoryPatch::createWithHex("libUE4.so", 0x16D7C40,"00 00 00 00").Modify();  // termination fix
MemoryPatch::createWithHex("libanogs.so", 0x8e5f6,"00 20 70 47").Modify();  // 1 day fix 
MemoryPatch::createWithHex("libanogs.so", 0x8E6E4,"00 20 70 47").Modify();  //  1 day fix  
MemoryPatch::createWithHex("libanogs.so", 0xACAD2,"00 20 70 47").Modify();  // crash
    
if(WIDEVIEW) {
Write_Float(UE4 + 0x26C7B90, WIDEVIEW1);
} else {
Write_Float(UE4 + 0x26C7B90, 360.0f);
}

if(Config.SilentAim.SKYUP){
long a1 = getPointer(getPointer(UE4 + 0x50B7600) + 0xA8)+0x30;
writefloat(a1,1500);
}
if(Config.SilentAim.STABLE){
long a1 = getPointer(getPointer(UE4 + 0x50B7600) + 0xA8)+0x30;
writefloat(a1,0);
}
if(Config.SilentAim.DOWE){
long a1 = getPointer(getPointer(UE4 + 0x50B7600) + 0xA8)+0x30;
writefloat(a1,-1500);
}

//=====================================================================//
if (UTAM_FLY)
{
    ImGui::SetNextWindowPos(ImVec2(450, 360), ImGuiCond_FirstUseEver);

    // ===== Reduced Vertical Padding & Spacing =====
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6, 4)); 
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 4));   
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

    // ===== Vaporwave Sunset Theme Colors =====
    ImGui::PushStyleColor(ImGuiCol_WindowBg,        ImVec4(0.09f, 0.02f, 0.06f, 0.95f));
    ImGui::PushStyleColor(ImGuiCol_Border,          ImVec4(1.00f, 0.10f, 0.50f, 0.85f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg,         ImVec4(0.15f, 0.02f, 0.08f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive,   ImVec4(0.80f, 0.08f, 0.40f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_Text,            ImVec4(1.00f, 0.90f, 0.95f, 1.00f));

    if (ImGui::Begin(OBFUSCATE("F L Y"), nullptr,
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings))
    {
        DrawPinkMeteorBorder(4.0f);
        
        ImDrawList* pDrawList = ImGui::GetWindowDrawList();
        ImVec2 windowPos  = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();

        float time = ImGui::GetTime();

        // ===== Dynamic Background Glow (Pink/Magenta Shading) =====
        auto VaporwaveShade = [&](float offset)
        {
            float glow = 0.5f + 0.5f * sinf(time * 0.8f + offset);
            int r = (int)(160.0f + 75.0f * glow);
            int g = (int)(20.0f  + 30.0f * glow);
            int b = (int)(80.0f  + 50.0f * glow);
            return IM_COL32(r, g, b, 255);
        };

        ImU32 colorDark   = IM_COL32(23, 5, 15, 255);
        ImU32 colorPink1  = VaporwaveShade(0.0f);
        ImU32 colorPink2  = VaporwaveShade(3.14f);

        pDrawList->AddRectFilledMultiColor(
            windowPos,
            ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
            colorDark,
            colorPink1,
            colorDark,
            colorPink2
        );

        ImVec4 btnBorderCol = ImVec4(1.00f, 0.10f, 0.50f, 0.85f);

        // ===== Custom Vaporwave Button Style Function =====
        auto VaporwaveButton = [&](bool active, const char* name, ImVec2 size, bool& var)
        {
            ImVec4 bg, txt, hov, act;

            if (active)
            {
                bg  = ImVec4(1.00f, 0.20f, 0.55f, 1.00f);
                txt = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                hov = ImVec4(1.00f, 0.35f, 0.65f, 1.00f);
                act = ImVec4(0.80f, 0.10f, 0.40f, 1.00f);
            }
            else
            {
                bg  = ImVec4(0.22f, 0.03f, 0.12f, 0.80f);
                txt = ImVec4(0.85f, 0.50f, 0.65f, 1.00f);
                hov = ImVec4(0.85f, 0.12f, 0.45f, 1.00f);
                act = ImVec4(1.00f, 0.20f, 0.55f, 1.00f);
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.5f));

            ImGui::PushStyleColor(ImGuiCol_Border,        btnBorderCol);
            ImGui::PushStyleColor(ImGuiCol_Button,        bg);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hov);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,  act);
            ImGui::PushStyleColor(ImGuiCol_Text,          txt);

            if (ImGui::Button(name, size))
                var = !var;

            ImGui::PopStyleColor(5);
            ImGui::PopStyleVar(4);
        };
        
        DrawPinkGlowShimmerLine();
        DrawPinkGlowShimmerLine();

        // UTAM Buttons with matching pink/magenta theme
        VaporwaveButton(Config.SilentAim.SKYUP,  "SKY UP", ImVec2(110, 28), Config.SilentAim.SKYUP);
        VaporwaveButton(Config.SilentAim.STABLE, "STABLE", ImVec2(110, 28), Config.SilentAim.STABLE);
        VaporwaveButton(Config.SilentAim.DOWE,   "DOWN",   ImVec2(110, 28), Config.SilentAim.DOWE);
        
        DrawPinkGlowShimmerLine();
        DrawPinkGlowShimmerLine();
        
        ImGui::End();
    }

    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar(4);
}
 //=====================================================================//
if(LITE_ESP){
MemoryPatch::createWithHex(OBFUSCATE("libgcloud.so"), 0x62FFC,OBFUSCATE("1E FF 2F E1")).Modify();
} else {
MemoryPatch::createWithHex("libgcloud.so", 0x62FFC,"89 EA 04 04").Modify();// Iceland
}
//=====================================================================//



if(LITE_ESP){
Config.PlayerESP.Line = true;
Config.PlayerESP.Distance = true;
Config.PlayerESP.Health = true;
Config.PlayerESP.Skeleton = true;
Config.PlayerESP.Name = true;
Config.PlayerESP.Visibility = true;
Config.SilentAim.Enable = true;
Config.PlayerESP.TeamID = true;
Config.AimBot.VisCheck = true;
Write_Float(UE4 + 0x1949A58, 0.0f);
UTAMKILLMSG = true;
Config.PlayerESP.UtamDeadBox = true;
Config.PlayerESP.TeammateESP = true;
Config.PlayerESP.Weapon = true;
Config.PlayerESP.Vehicle = true;
}else{
// ESP is the master switch: turning it off shuts every other panel feature off too.
// HIGH DAMAGE is excluded - it's always on now, independent of ESP.
AutoFire2 = false;
AMMO = false;
UTAM_FLASH = false;
Godviewup = false;
Godviewdown = false;
Config.PlayerESP.UtamDeadBox = false;
Config.PlayerESP.TeammateESP = false;
Config.PlayerESP.Weapon = false;
Config.PlayerESP.Vehicle = false;
UTAMKILLMSG = false;
Config.PlayerESP.Visibility = false;
Config.PlayerESP.Line = false;
Config.PlayerESP.Distance = false;
Config.PlayerESP.Health = false;
Config.PlayerESP.TeamID = false;
Config.PlayerESP.Skeleton = false;
Config.PlayerESP.Name = false;
Config.SilentAim.Enable = false;
Config.AimBot.VisCheck = false;
}
//=====================================================================//
int totalEnemies = 0, totalBots = 0;
uintptr_t localPlayer = 0, localController = 0/*, localWeapon = 0*/;


SDK::ASTExtraPlayerCharacter *ZevaPlayer = 0;
SDK::ASTExtraPlayerController *ZevaController = 0;
SDK::ASTExtraShootWeapon *localWeapon = 0;

auto UTAMA = Zeva_Actors();
    
    for (int i = 0; i < UTAMA.size(); i++){
        auto UTAMA_xD = UTAMA[i];
        if (isObjectInvalids(UTAMA_xD))
            continue;
        if (UTAMA_xD->IsA(SDK::ASTExtraPlayerController::StaticClass())){
            ZevaController = (SDK::ASTExtraPlayerController *)UTAMA_xD;
            break;
        }
    }
    
	

	
    if (ZevaController){
    for (int i = 0; i < UTAMA.size(); i++){
        auto UTAMA_xD = UTAMA[i];
        if (isObjectInvalids(UTAMA_xD))
            continue;
        if (UTAMA_xD->IsA(SDK::ASTExtraPlayerCharacter::StaticClass())){
            if (((SDK::ASTExtraPlayerCharacter *)UTAMA_xD)->PlayerKey == ZevaController->PlayerKey){
                ZevaPlayer = (SDK::ASTExtraPlayerCharacter *)UTAMA_xD;
                break;
            }
        }
    }
}

 
for (int i = 0; i < UTAMA.size(); i++) {
          auto UTAMA_xD = UTAMA[i];
       if (isObjectInvalids(UTAMA_xD))
          continue;

		  }

uintptr_t GameWorld = getGWorld();
if (GameWorld) {
uintptr_t NetDriver = * (uintptr_t *)(GameWorld + World_NetDriver);
if (NetDriver) {
uintptr_t ServerConnection = * (uintptr_t *)(NetDriver + NetDriver_ServerConnection);
if (ServerConnection) {
localController = * (uintptr_t *)(ServerConnection + Player_PlayerController);
}}}
    
if (localController) {
auto Actors = getActors();
for (auto Actor : Actors) {
if (!isObjectPlayer(Actor))
continue;
if ( * (uint32_t *)(Actor + UAECharacter_PlayerKey) == * (uint32_t *)(localController + UAEPlayerController_PlayerKey)) {
localPlayer = Actor;
break;
}}
// LONG HAND: auto-triggered once per match, the moment the player character exists.
// Its memory scan finds per-match values, so it must fire fresh each match rather
// than once at app startup.
static uintptr_t lastValidPlayerForLongHand = 0;
if (localPlayer && localPlayer != lastValidPlayerForLongHand) {
    lastValidPlayerForLongHand = localPlayer;
    pthread_t tLongHand;
    if (pthread_create(&tLongHand, 0, run_thread, (void *)(850)) == 0) pthread_detach(tLongHand);
}
if (!localPlayer) lastValidPlayerForLongHand = 0;
// HIGH DAMAGE: same auto-trigger pattern - always on now, no toggle, so it has to fire
// itself once per match instead of waiting for a button click.
static uintptr_t lastValidPlayerForHighDamage = 0;
if (localPlayer && localPlayer != lastValidPlayerForHighDamage) {
    lastValidPlayerForHighDamage = localPlayer;
    pthread_t tHighDamage;
    if (pthread_create(&tHighDamage, 0, run_thread, (void *)(6)) == 0) pthread_detach(tHighDamage);
}
if (!localPlayer) lastValidPlayerForHighDamage = 0;
// ESP-while-dead removed entirely per request - it was the source of repeated crash-on-
// death reports (cachedTeamID/localPlayer=0 fallback reading stale actor state). ESP now
// simply stops when your character is gone, same as before that feature was added.
if (localPlayer) {
// UTAM_LocalPlayer is a separate global from localPlayer (set by a different hook) and
// localPlayer being valid doesn't guarantee UTAM_LocalPlayer still is - it needs its own check.
if (localPlayer && Config.SilentAim.Enable && UTAM_LocalPlayer && !isObjectInvalids((SDK::UObject *)UTAM_LocalPlayer))
        {
            auto WeaponManagerComponent = UTAM_LocalPlayer->WeaponManagerComponent;
            if (WeaponManagerComponent)
            {
                auto propSlot = WeaponManagerComponent->GetCurrentUsingPropSlot();
                if ((int)propSlot.GetValue() >= 1 && (int)propSlot.GetValue() <= 3)
                {
                    auto CurrentWeaponReplicated =
                    (ASTExtraShootWeapon *)
                    WeaponManagerComponent->CurrentWeaponReplicated;
                    if (CurrentWeaponReplicated)
                    {
                        auto ShootWeaponComponent =
                        CurrentWeaponReplicated->ShootWeaponComponent;
                        if (ShootWeaponComponent)
                        {
                            int shoot_event_idx = 140;
                            auto VTable = (void **)ShootWeaponComponent->VTable;
                            auto f_mprotect = [](uintptr_t addr, size_t len,
                            int32_t prot) -> int32_t
                            {
                                static_assert(PAGE_SIZE == 4096);
                                constexpr size_t page_size = static_cast<size_t>(PAGE_SIZE);
                                void *start = reinterpret_cast<void *>(addr & -page_size);
                                uintptr_t end = (addr + len + page_size - 20) & -page_size;
                                return mprotect(start, end - reinterpret_cast<uintptr_t>(start), prot);
                            };
                            if (VTable && (VTable[shoot_event_idx] != shoot_event))
                            {
                                orig_shoot_event =
                                decltype(orig_shoot_event)(VTable[shoot_event_idx]);
                                f_mprotect((uintptr_t)(&VTable[shoot_event_idx]),
                                sizeof(uintptr_t), PROT_READ | PROT_WRITE);
                                VTable[shoot_event_idx] = (void *)shoot_event;
                            }
                        }
                    }
                }
            }
        }
Vector3 myPos = GetBonePos(localPlayer, 0);
            int myTeamID = *(int *)(localPlayer + UAECharacter_TeamID);
            ImU32 U1SCOLOR, U1HPCOLOR;
            ImU32 SCOLOR, SCOLOR2, SCOLOR3, SCOLOR4, NAME,HPColor;
            for (auto Actor : Actors) {
    if (!Actor) continue;
    // Actor was a live pointer when getActors() snapshotted it, but this whole loop body
    // (bone lookups, screen projection, skeleton draw) takes real time per actor, and the
    // game thread can destroy an actor (e.g. a kill during a firefight) while we're still
    // partway through reading it here on the render thread. That race is far more likely to
    // land mid-iteration exactly when ESP is on, since ESP is what makes this loop do
    // meaningful per-actor work in the first place - matching "no ESP, no crash on death".
    if (isObjectInvalids((SDK::UObject *)Actor)) continue;

    if (isObjectPlayer(Actor)) {
        if (Actor == localPlayer)
            continue;
            
        int TeamID = *(int *)(Actor + UAECharacter_TeamID);

        bool bDead = *(bool *)(Actor + STExtraCharacter_bDead);
        if (bDead)
            continue;

        // Distance computed from bone position rather than the native GetDistanceTo -
        // that call reads the global g_LocalPlayer internally, which can be a stale
        // pointer once your own character is gone (dead/spectating).
        Vector3 TeamCheckRootPos = GetBonePos(Actor, 0);
        float Distance = Vector3::Distance(myPos, TeamCheckRootPos) / 100.f;
        if (Distance > 400.f)
            continue;

        if (TeamID == myTeamID) {
            // Teammate: minimal ESP (name + distance only), no combat info.
            if (Config.PlayerESP.TeammateESP) {
                Vector3 TeamHeadSC = WorldToScreen(GetBonePos(Actor, 6));
                if (TeamHeadSC.Z > 0) {
                    std::string tName = "Teammate " + std::to_string((int)Distance) + "m";
                    draw->AddText(NULL, 16.0f, {TeamHeadSC.X - 30.0f, TeamHeadSC.Y - 20.0f}, IM_COL32(60, 200, 255, 255), tName.c_str());
                }
            }
            continue;
        }

        bool isBot = *(bool *)(Actor + UAECharacter_bIsAI);
        if (Config.PlayerESP.NoBot && isBot)
            continue;

        // Colors set
        SCOLOR = IM_COL32(255, 255, 255, 255);
        SCOLOR3 = IM_COL32(255, 255, 255, 255);
        SCOLOR2 = IM_COL32(255, 140, 26, 255);
        SCOLOR4 = IM_COL32(255, 140, 26, 255);

        if (IsLineOfSightTo(Actor)) {
            SCOLOR = IM_COL32(26, 255, 255, 255);
            SCOLOR3 = IM_COL32(26, 255, 255, 255);
            SCOLOR2 = IM_COL32(255, 0, 0, 255);
            SCOLOR4 = IM_COL32(255, 0, 0, 255);
        }

        if (isBot) {
            totalBots++;
            U1SCOLOR = IM_COL32(0, 255, 0, 255);
            U1HPCOLOR = IM_COL32(0, 255, 0, 170);
            NAME = IM_COL32(255, 255, 255, 255);

            if (IsLineOfSightTo(Actor)) {
                U1SCOLOR = IM_COL32(255, 0, 0, 255);
                U1HPCOLOR = IM_COL32(255, 0, 0, 170);
                NAME = IM_COL32(255, 255, 255, 255);
            }
        } else {
            totalEnemies++;
            U1SCOLOR = IM_COL32(0, 255, 0, 255);
            U1HPCOLOR = IM_COL32(0, 255, 0, 170);
            NAME = IM_COL32(255, 255, 255, 255);

            if (IsLineOfSightTo(Actor)) {
                U1SCOLOR = IM_COL32(255, 0, 0, 255);
                U1HPCOLOR = IM_COL32(255, 0, 0, 170);
                NAME = IM_COL32(255, 255, 255, 255);
            }
        }

        Vector3 RootPos = TeamCheckRootPos;   // already computed above for the distance check
        Vector3 RootPosSC = WorldToScreen(RootPos);
        Vector3 HeadPosSC = WorldToScreen(GetBonePos(Actor, 6));

if (RootPosSC.Z > 0 && HeadPosSC.Z > 0) {
                        
if (Config.PlayerESP.ESPUIType == EESPUIType::ESPUI01) {

    if (Config.PlayerESP.Line) {
        // Was `HeadPosSC.Y - 45.0f` - the line stopped 45px short of the actual head bone
        // position, showing as a gap between the line and the skeleton/box. The dead
        // `else if` below (same condition, unreachable) had the correct, offset-free version.
        draw->AddLine({(float)glWidth / 2, 90}, {HeadPosSC.X, HeadPosSC.Y}, U1SCOLOR, 1.5f);
    }
    if (Config.PlayerESP.Skeleton) {
        static std::vector < std::vector < int>> skeleton {
            {
                BoneID::neck_01,
                BoneID::clavicle_r,
                BoneID::upperarm_r,
                BoneID::lowerarm_r,
                BoneID::hand_r,
                BoneID::item_r
            },
            {
                BoneID::neck_01,
                BoneID::clavicle_l,
                BoneID::upperarm_l,
                BoneID::lowerarm_l,
                BoneID::hand_l,
                BoneID::item_l
            },
            {
                BoneID::Head,
                BoneID::neck_01,
                BoneID::spine_03,
                BoneID::spine_02,
                BoneID::spine_01,
                BoneID::pelvis
            },
            {
                BoneID::pelvis,
                BoneID::thigh_r,
                BoneID::calf_r,
                BoneID::foot_r
            },
            {
                BoneID::pelvis,
                BoneID::thigh_l,
                BoneID::calf_l,
                BoneID::foot_l
            }
        };

        for (const auto & boneStructure: skeleton) {
            int lastBone = 0;
            for (int currentBone: boneStructure) {
                if (lastBone != 0) {
                    Vector3 boneFrom = WorldToScreen(GetBonePos(Actor, currentBone));
                    Vector3 boneTo = WorldToScreen(GetBonePos(Actor, lastBone));
                    if (boneFrom.Z > 0 && boneTo.Z > 0) {
                        draw->AddLine({boneFrom.X, boneFrom.Y}, {boneTo.X, boneTo.Y}, U1SCOLOR, 1.8f);
                    }
                }
                lastBone = currentBone;
            }
        }
    }

                          
    ImVec2 rectCenter;

    		        if (Config.PlayerESP.Health) {
                             auto PlayerHealth = *(float *)(uintptr_t(Actor) + STExtraCharacter_Health);
        auto PlayerHealthMax = *(float *)(uintptr_t(Actor) + STExtraCharacter_HealthMax);
        int CurHP = std::clamp(static_cast < int > (PlayerHealth), 0, static_cast < int > (PlayerHealthMax));
        int MaxHP = static_cast < int > (PlayerHealthMax);
        long HPColor;

                                HPColor = U1HPCOLOR;
										  
                                                       
                           //             int CurHP = (int) std::max(0, std::min((int) Player->Health,100));

float boxWidth = density / 1.6f;
                                                boxWidth -= std::min(
                                                        ((boxWidth / 2) / 0.001f) * Distance,
                                                        boxWidth / 2);
                                                float boxHeight = boxWidth * 0.15f;


                                                ImVec2 vStart = {HeadPosSC.X - (boxWidth / 2), HeadPosSC.Y - (boxHeight * 2.001f)};

                                                ImVec2 vEndFilled = {vStart.x + (CurHP * boxWidth / MaxHP), 
												                     vStart.y + boxHeight};
												
                                                ImVec2 vEndRect = {vStart.x + boxWidth, vStart.y + boxHeight};
																   
												// Removed the filled red/green health box (was AddRectFilledMultiColor + AddRect
// here) - it rendered as an unwanted colored background box near the name.
                                                (void)vEndFilled; (void)vEndRect; (void)HPColor;
                                            }
                     
                                            
                                                                   
                if (Config.PlayerESP.Name && localPlayer && !isObjectInvalids((SDK::UObject *)localPlayer)) {
                    // localPlayer being dereferenced unguarded here (via CurrentVehicle/
                    // RootComponent below) is what crashed the game on death: localPlayer
                    // correctly becomes 0 once your character is destroyed, but this radar
                    // block never checked for that before reading (localPlayer + offset).
                    // localPlayer can also go non-zero-but-dangling mid-frame if the engine
                    // destroys the character while this loop is still running, which is why
                    // isObjectInvalids() is needed here too, not just a null check.
					          				bool shit = false;
                            Vector3 MyPosition, EnemyPosition;
                            auto CurrentVehiclea = *(uintptr_t *) (Actor + STExtraCharacter_CurrentVehicle);
                            if (CurrentVehiclea && !isObjectInvalids((SDK::UObject *)CurrentVehiclea)) {
                                auto RootComponent = *(uintptr_t *)(CurrentVehiclea + Actor_RootComponent);
                                if (RootComponent) {
                                    MyPosition = *(Vector3 *)(RootComponent + SceneComponent_RelativeLocation);
                                }
                            } else {
                                auto RootComponent = *(uintptr_t *)(Actor + Actor_RootComponent);
                                if (RootComponent) {
                                    MyPosition = *(Vector3 *)(RootComponent + SceneComponent_RelativeLocation);
                                }
                            }
                            auto CurrentVehicle = *(uintptr_t *) (localPlayer + STExtraCharacter_CurrentVehicle);
                            if (CurrentVehicle && !isObjectInvalids((SDK::UObject *)CurrentVehicle)) {
                                auto RootComponent = *(uintptr_t *)(CurrentVehicle + Actor_RootComponent);
                                if (RootComponent) {
                                    EnemyPosition = *(Vector3 *)(RootComponent + SceneComponent_RelativeLocation);
                                }
                            } else {
                                auto RootComponent = *(uintptr_t *)(localPlayer + Actor_RootComponent);
                                if (RootComponent) {
                                    EnemyPosition = *(Vector3 *)(RootComponent + SceneComponent_RelativeLocation);
                                }
                            }
                            Vector3 EntityPos = WorldToRadar(GetPOV().Rotation.Yaw, MyPosition, EnemyPosition, NULL, NULL, Vector3(glWidth, glHeight, 0), shit);
                            Vector3 angle = Vector3();
                            Vector3 forward = Vector3((float)(glWidth / 2) - EntityPos.X, (float)(glHeight / 2) - EntityPos.Y, 0.0f);
                            VectorAnglesRadar(forward, angle);
                            const auto angle_yaw_rad = DEG2RAD(angle.Y + 180.f);
                            const auto new_point_x = (glWidth / 2) + (55/*alert dist from me*/) / 2 * 8 * cosf(angle_yaw_rad);  
                            const auto new_point_y = (glHeight / 2) + (55/*alert dist from me*/) / 2 * 8 * sinf(angle_yaw_rad);  
                            std::array<Vector3, 3> points { Vector3(new_point_x - ((90) / 4 + 3.5f) / 2, new_point_y - ((55) / 4 + 3.5f) / 2, 0.f), Vector3(new_point_x + ((90) / 4 + 3.5f) / 4, new_point_y, 0.f), Vector3(new_point_x - ((90) / 4 + 3.5f) / 2, new_point_y + ((55) / 4 + 3.5f) / 2, 0.f)};
                          
      
        
        std::string s;
        s += std::to_string((int)Distance);   
        //auto textSize = ImGui::CalcTextSize(s.c_str(), 0, ((float) density / 20.0f));
      //  draw->AddText(NULL, ((float) density / 20.0f), {new_point_x - (textSize.x / 2), new_point_y + 7.f}, IM_COL32(255,255,255,255), s.c_str());                 
           RotateTriangle(points, angle.Y + 180.f);  
     draw->AddCircle(ImVec2(new_point_x, new_point_y), 13.0f, IM_COL32(255,255,255,255));    
        draw->AddCircleFilled(ImVec2(new_point_x, new_point_y), 8.0f, U1SCOLOR);
  }                                                                          
                                                                                                                 
                                                                                                                                                               
    			if (Config.PlayerESP.Name){

					float boxWidth = density / 1.6f;
                                                boxWidth -= std::min(
                                                        ((boxWidth / 2) / 00.0f) * Distance,
                                                        boxWidth / 2);
                                                float boxHeight = boxWidth * 0.15f;


                                                std::string s;
                                                if (isBot) {
                                                    s += "      BOT";
                                                } else {
s += (*(FString *)(Actor + UAECharacter_PlayerName)).ToString();
                                                }
                                                if (Config.PlayerESP.TeamID) {
                                                    s += " [" + std::to_string(TeamID) + "]";
                                                }
                                                // Moved above the health bar (was overlapping it at 1.83,
                                                // which is why the name looked like it had a green/red
                                                // background - that was the health bar's fill showing
                                                // through behind the overlapping text).
                                                draw->AddText(NULL, ((float) density / 30.0f),
                                                              {HeadPosSC.X - (boxWidth / 3.5f),
                                                               HeadPosSC.Y - (boxHeight * 2.6f)},
                                                              NAME,
                                                              s.c_str());
                                                if (Config.PlayerESP.Weapon) {
                                                    auto EnemyChar = (ASTExtraPlayerCharacter *)Actor;
                                                    auto EnemyWM = EnemyChar->WeaponManagerComponent;
                                                    if (EnemyWM && !isObjectInvalids((SDK::UObject *)EnemyWM)) {
                                                        auto EnemyWeapon = (ASTExtraShootWeapon *)EnemyWM->CurrentWeaponReplicated;
                                                        // GetWeaponName() is a live engine call (ProcessEvent), not a raw memory
                                                        // read - calling it on a weapon actor that was just swapped/destroyed
                                                        // (an enemy dying or reloading during a firefight) crashes, and this
                                                        // runs every frame for every visible enemy whenever ESP is on.
                                                        if (EnemyWeapon && !isObjectInvalids((SDK::UObject *)EnemyWeapon)) {
                                                            std::string w = EnemyWeapon->GetWeaponName().ToString();
                                                            if (!w.empty()) {
                                                                draw->AddText(NULL, ((float) density / 32.0f),
                                                                              {HeadPosSC.X - (boxWidth / 3.5f),
                                                                               HeadPosSC.Y - (boxHeight * 1.6f)},
                                                                              IM_COL32(255, 200, 60, 255),
                                                                              w.c_str());
                                                            }
                                                        }
                                                    }
                                                }
    }

               if (Config.PlayerESP.Distance) {
                            std::string s = std::to_string((int) Distance) + "M";
        auto textSize = ImGui::CalcTextSize2(s.c_str(), 0, ((float) density / 22.0f));
        ImVec2 textPos(RootPosSC.X - (textSize.x / 2.0f), RootPosSC.Y);
        draw->AddText(NULL, ((float) density / 22.0f), textPos, IM_COL32(255, 252, 0, 255), s.c_str());
    }}					
				}}   
    else if (Config.PlayerESP.UtamDeadBox && isObjectDeadBox(Actor)) {
        uintptr_t AM11 = *(uintptr_t*)(Actor + 0x140);
        if (AM11) {
            Vector3 boxLocation = *(Vector3*)(AM11 + 0x118);
            Vector3 myPos = GetBonePos(localPlayer ? localPlayer : g_LocalPlayer, 0);
            float Distance = Vector3::Distance(myPos, boxLocation) / 100.0f;
            if (Distance <= 500.0f) {
                Vector3 screenPos = WorldToScreen(boxLocation);
                if (screenPos.Z > 0) {
                    std::string label = "Loot Box - " + std::to_string((int)Distance) + "M";
                    ImU32 textColor = IM_COL32(255, 0, 255, 255);
                    ImU32 outlineColor = IM_COL32(0, 0, 0, 255);
                    draw->AddText(NULL, density / 25.0f, {screenPos.X + 1, screenPos.Y + 1}, outlineColor, label.c_str());
                    draw->AddText(NULL, density / 25.0f, {screenPos.X, screenPos.Y}, textColor, label.c_str());
                }
            }
        }
    }
    else if (Config.PlayerESP.Vehicle && isObjectVehicle(Actor)) {
        uintptr_t VRoot = *(uintptr_t *)(Actor + Actor_RootComponent);
        if (VRoot) {
            Vector3 VLoc = *(Vector3 *)(VRoot + SceneComponent_RelativeLocation);
            Vector3 myPos = GetBonePos(localPlayer ? localPlayer : g_LocalPlayer, 0);
            float Distance = Vector3::Distance(myPos, VLoc) / 100.0f;
            if (Distance <= 300.0f) {
                Vector3 screenPos = WorldToScreen(VLoc);
                if (screenPos.Z > 0) {
                    uint8_t vType = *(uint8_t *)(Actor + STExtraVehicleBase_VehicleShapeType);
                    std::string label = GetVehicleName(vType) + " - " + std::to_string((int)Distance) + "M";
                    draw->AddText(NULL, density / 26.0f, {screenPos.X + 1, screenPos.Y + 1}, IM_COL32(0, 0, 0, 255), label.c_str());
                    draw->AddText(NULL, density / 26.0f, {screenPos.X, screenPos.Y}, IM_COL32(80, 200, 255, 255), label.c_str());
                }
            }
        }
    }
    else if (isObjectPickUp(Actor)) {
        int ZY14 = *(int *)(Actor + PickUpWrapperActor_DefineID);         // FItemDefineID.Type
        int ZY13 = *(int *)(Actor + PickUpWrapperActor_DefineID + 0x4);   // FItemDefineID.TypeSpecificID
        uintptr_t ZY11 = *(uintptr_t *)(Actor + Actor_RootComponent);
        if (ZY11) {
            Vector3 ZY12 = *(Vector3 *)(ZY11 + SceneComponent_RelativeLocation);
            Vector3 myPos = GetBonePos(localPlayer ? localPlayer : g_LocalPlayer, 0);
            float Distance = Vector3::Distance(myPos, ZY12) / 100.0f;
            if (Distance <= 200.0f) {
                Vector3 itemPos = WorldToScreen(ZY12);
                std::string itemName;
                uint32_t textColor = 0xFFFFFFFF;
                bool isConfirmedDbEntry = false;   // Items.h match (scope/AR mag) - always shown
                for (auto &category : items_data) {
                    for (auto &item : category["Items"]) {
                        if (item["itemId"] == ZY13) {
                            itemName = item["itemName"].get<std::string>();
                            textColor = std::stoul(item["itemTextColor"].get<std::string>(), nullptr, 16);
                            isConfirmedDbEntry = true;
                            break;
                        }
                    }
                }
                // Read the engine's own category/value tags straight off the pickup actor -
                // confirmed (by testing with the filter disabled) to carry real, readable
                // text for guns/ammo, not just scopes.
                std::string itemCategory = (*(SDK::FString *)(Actor + PickUpWrapperActor_ItemCategory)).ToString();
                std::string itemValue = (*(SDK::FString *)(Actor + PickUpWrapperActor_ItemValue)).ToString();
                if (itemName.empty()) {
                    static std::map<int64_t, std::string> g_LiveItemNameCache;
                    int64_t cacheKey = ((int64_t)ZY14 << 32) | (uint32_t)ZY13;
                    auto cached = g_LiveItemNameCache.find(cacheKey);
                    if (cached != g_LiveItemNameCache.end()) {
                        itemName = cached->second;
                    } else if (UTAM_LocalController && !isObjectInvalids((SDK::UObject *)UTAM_LocalController)
                               && UTAM_LocalController->BackpackComponent
                               && !isObjectInvalids((SDK::UObject *)UTAM_LocalController->BackpackComponent)) {
                        SDK::FItemDefineID defineID{};
                        defineID.Type = ZY14;
                        defineID.TypeSpecificID = ZY13;
                        defineID.bValidItem = true;
                        auto resolved = UTAM_LocalController->BackpackComponent->GetItemByDefineID(defineID);
                        itemName = resolved.Name.ToString();
                        g_LiveItemNameCache[cacheKey] = itemName;   // cache the miss too
                    }
                }
                std::string classifyText = itemName + " " + itemValue + " " + itemCategory;
                if (itemPos.Z > 0 && (isConfirmedDbEntry || IsWantedLootItem(classifyText))) {
                    std::string displayName = !itemName.empty() ? itemName
                                             : !itemValue.empty() ? itemValue
                                             : !itemCategory.empty() ? itemCategory
                                             : std::string("Item");
                    std::string distanceText = " - " + std::to_string(static_cast<int>(Distance)) + "M";
                    std::string displayText = displayName + distanceText;
                    DrawTextWithBorder(draw, displayText, {itemPos.X, itemPos.Y}, textColor, outlinecolor, 15.0f);
                }
            }
        }
    }
} // Loop ends correctly here

if (Targetline) {
    auto Target = GetTargetByDistance();
    if (Target) {
        auto target = Target->GetBonePos("Head");
        auto footL = Target->GetBonePos("Root");
        ImVec2 targetSC, footLPos;
        if (W2S(target, (FVector2D*)&targetSC) && W2S(footL, (FVector2D*)&footLPos)) {
            ImU32 lineColor = ImColor(255, 0, 0, 255);
            draw->AddLine({(float)glWidth / 2, 716}, {footLPos.x - 2, footLPos.y}, lineColor, 2.5f);
        }
    }
}
//==================== ESP COUNT=======================//
char extraa[50];    
char extraaa[50];                                                                 

g_LocalController=localController;
g_LocalPlayer=localPlayer;

UTAM_LocalController = ZevaController;
UTAM_LocalPlayer = ZevaPlayer;

std::string s="Players "+std::to_string((int)totalEnemies)+" | Bots "+std::to_string((int)totalBots);

ImVec2 vStart={glWidth/2-90,45};
ImVec2 vEnd={glWidth/2+90,75};
ImVec2 rectCenter={(vStart.x+vEnd.x)/2.0f,(vStart.y+vEnd.y)/2.0f};

ImVec2 textSize=ImGui::CalcTextSize2(s.c_str(),0,((float)density/19.0f));
ImVec2 textPos={rectCenter.x-textSize.x/2.0f,rectCenter.y-textSize.y/2.0f};

bool enemiesNearby=(totalEnemies>0);

// =========================================
// HOT PINK & NEON MAGENTA THEME COLORS
// =========================================
ImU32 colBase=enemiesNearby?IM_COL32(255,50,150,255):IM_COL32(215,30,115,255);
ImU32 colGlowBase=enemiesNearby?IM_COL32(255,90,175,140):IM_COL32(230,50,130,140);

// Dark Purple Window Background
draw->AddRectFilled(vStart,vEnd,IM_COL32(23,5,15,230),8.0f);
draw->AddRect(vStart,vEnd,colBase,8.0f,0,3.0f);

float time=ImGui::GetTime();

std::vector<ImVec2> verts={
{vStart.x,vStart.y},
{vEnd.x,vStart.y},
{vEnd.x,vEnd.y},
{vStart.x,vEnd.y}
};

float totalLen=0;
std::vector<float> seg;
for(int i=0;i<4;i++){
float L=sqrtf(
(verts[(i+1)%4].x-verts[i].x)*(verts[(i+1)%4].x-verts[i].x)+
(verts[(i+1)%4].y-verts[i].y)*(verts[(i+1)%4].y-verts[i].y)
);
seg.push_back(L);
totalLen+=L;
}

float head=fmodf(time*totalLen*0.38f,totalLen);
float tailLen=totalLen*0.22f;

for(int j=0;j<35;j++){
float t=(float)j/34.0f;
float pos=head-t*tailLen;
if(pos<0)pos+=totalLen;
float run=0;
ImVec2 dot=verts[0];
for(int i=0;i<4;i++){
if(pos>=run&&pos<=run+seg[i]){
float s2=(pos-run)/seg[i];
dot={
verts[i].x+(verts[(i+1)%4].x-verts[i].x)*s2,
verts[i].y+(verts[(i+1)%4].y-verts[i].y)*s2
};
break;
}
run+=seg[i];
}
float fade=1.0f-t;
fade*=fade;
ImU32 glowCol=(colGlowBase&0x00FFFFFF)|((int)(((colGlowBase>>24)&0xFF)*fade)<<24);
draw->AddCircleFilled(dot,4.2f,glowCol,10);
}

draw->AddText(nullptr,((float)density/19.0f),textPos+ImVec2(1,1),colGlowBase,s.c_str());
draw->AddText(nullptr,((float)density/19.0f),textPos,colBase,s.c_str());
}}}
std::string getClipboardText() {
	if (!g_App)
		return "";
	auto activity = g_App->activity;
	if (!activity)
		return "";
	auto vm = activity->vm;
	if (!vm)
		return "";
	auto object = activity->clazz;
	if (!object)
		return "";
	std::string result;
	JNIEnv * env;
	vm->AttachCurrentThread( & env, 0);
	{
		auto ContextClass = env->FindClass("android/content/Context");
		auto getSystemServiceMethod = env->GetMethodID(ContextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
		auto str = env->NewStringUTF("clipboard");
		auto clipboardManager = env->CallObjectMethod(object, getSystemServiceMethod, str);
		env->DeleteLocalRef(str);
		auto ClipboardManagerClass = env->FindClass("android/content/ClipboardManager");
		auto getText = env->GetMethodID(ClipboardManagerClass, "getText", "()Ljava/lang/CharSequence;");
		auto CharSequenceClass = env->FindClass("java/lang/CharSequence");
		auto toStringMethod = env->GetMethodID(CharSequenceClass, "toString", "()Ljava/lang/String;");
		auto text = env->CallObjectMethod(clipboardManager, getText);
		if (text) {
			str = (jstring)env->CallObjectMethod(text, toStringMethod);
			result = env->GetStringUTFChars(str, 0);
			env->DeleteLocalRef(str);
			env->DeleteLocalRef(text);
		}
		env->DeleteLocalRef(CharSequenceClass);
		env->DeleteLocalRef(ClipboardManagerClass);
		env->DeleteLocalRef(clipboardManager);
		env->DeleteLocalRef(ContextClass);
	}
	vm->DetachCurrentThread();
	return result;
}
const char * GetAndroidID(JNIEnv * env, jobject context) {
	jclass contextClass = env->FindClass("android/content/Context");
	jmethodID getContentResolverMethod = env->GetMethodID(contextClass, "getContentResolver", "()Landroid/content/ContentResolver;");
	jclass settingSecureClass = env->FindClass("android/provider/Settings$Secure");
	jmethodID getStringMethod = env->GetStaticMethodID(settingSecureClass, "getString", "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");
	auto obj = env->CallObjectMethod(context, getContentResolverMethod);
	auto str = (jstring)env->CallStaticObjectMethod(settingSecureClass, getStringMethod, obj, env->NewStringUTF("android_id"));
	return env->GetStringUTFChars(str, 0);
}
const char * GetDeviceModel(JNIEnv * env) {
	jclass buildClass = env->FindClass("android/os/Build");
	jfieldID modelId = env->GetStaticFieldID(buildClass, "MODEL", "Ljava/lang/String;");
	auto str = (jstring)env->GetStaticObjectField(buildClass, modelId);
	return env->GetStringUTFChars(str, 0);
}
const char * GetDeviceBrand(JNIEnv * env) {
	jclass buildClass = env->FindClass("android/os/Build");
	jfieldID modelId = env->GetStaticFieldID(buildClass, "BRAND", "Ljava/lang/String;");
	auto str = (jstring)env->GetStaticObjectField(buildClass, modelId);
	return env->GetStringUTFChars(str, 0);
}
const char * GetPackageName(JNIEnv * env, jobject context) {
	jclass contextClass = env->FindClass("android/content/Context");
	jmethodID getPackageNameId = env->GetMethodID(contextClass, "getPackageName", "()Ljava/lang/String;");
	auto str = (jstring)env->CallObjectMethod(context, getPackageNameId);
	return env->GetStringUTFChars(str, 0);
}
const char * GetDeviceUniqueIdentifier(JNIEnv * env, const char * uuid) {
	jclass uuidClass = env->FindClass("java/util/UUID");
	auto len = strlen(uuid);
	jbyteArray myJByteArray = env->NewByteArray(len);
	env->SetByteArrayRegion(myJByteArray, 0, len, (jbyte *)uuid);
	jmethodID nameUUIDFromBytesMethod = env->GetStaticMethodID(uuidClass, "nameUUIDFromBytes", "([B)Ljava/util/UUID;");
	jmethodID toStringMethod = env->GetMethodID(uuidClass, "toString", "()Ljava/lang/String;");
	auto obj = env->CallStaticObjectMethod(uuidClass, nameUUIDFromBytesMethod, myJByteArray);
	auto str = (jstring)env->CallObjectMethod(obj, toStringMethod);
	return env->GetStringUTFChars(str, 0);
}
struct MemoryStruct {
	char * memory;
	size_t size;
};
static size_t WriteMemoryCallback(void * contents, size_t size, size_t nmemb, void * userp) {
	size_t realsize = size * nmemb;
	struct MemoryStruct * mem = (struct MemoryStruct *)userp;
	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		return 0;
	}
	memcpy( & (mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
	return realsize;
}


std::string Login(const char *user_key) {
if (!g_App)
return "Internal Error";
auto activity = g_App->activity;
if (!activity)
return "Internal Error";
auto vm = activity->vm;
if (!vm)
return "Internal Error";
auto object = activity->clazz;
if (!object)
return "Internal Error";
JNIEnv *env;
vm->AttachCurrentThread(&env, 0);
std::string hwid = user_key;
hwid += GetAndroidID(env, object);
hwid += GetDeviceModel(env);
hwid += GetDeviceBrand(env);
std::string UUID = GetDeviceUniqueIdentifier(env, hwid.c_str());
vm->DetachCurrentThread();
std::string errMsg;
struct MemoryStruct chunk{};
chunk.memory = (char *) malloc(1);
chunk.size = 0;
CURL *curl;
CURLcode res;
curl = curl_easy_init();
if (curl) {
curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, /*POST*/ StrEnc(",IL=", "\x7C\x06\x1F\x69", 4).c_str());
std::string BHATIA = OBFUSCATE ("https:connect");
curl_easy_setopt(curl, CURLOPT_URL ,BHATIA.c_str());
curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, /*https*/ StrEnc("!mLBO", "\x49\x19\x38\x32\x3C", 5).c_str());
struct curl_slist *headers = NULL;
headers = curl_slist_append(headers, /*Content-Type: application/x-www-form-urlencoded*/ StrEnc("@;Ls\\(KP4Qrop`b#d3094/r1cf<c<=H)AiiBG6i|Ta66s2[", "\x03\x54\x22\x07\x39\x46\x3F\x7D\x60\x28\x02\x0A\x4A\x40\x03\x53\x14\x5F\x59\x5A\x55\x5B\x1B\x5E\x0D\x49\x44\x4E\x4B\x4A\x3F\x04\x27\x06\x1B\x2F\x6A\x43\x1B\x10\x31\x0F\x55\x59\x17\x57\x3F", 47).c_str());
curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
char data[4096];
sprintf(data, /*game=PUBG&user_key=%s&serial=%s*/ StrEnc("qu2yXK,YkJyGD@ut0.u~Nb'5(:.:chK", "\x16\x14\x5F\x1C\x65\x1B\x79\x1B\x2C\x6C\x0C\x34\x21\x32\x2A\x1F\x55\x57\x48\x5B\x3D\x44\x54\x50\x5A\x53\x4F\x56\x5E\x4D\x38", 31).c_str(), user_key, UUID.c_str());
curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
res = curl_easy_perform(curl);
if (res == CURLE_OK) {
try {
json result = json::parse(chunk.memory);
if (result[/*status*/ StrEnc("(>_LBm", "\x5B\x4A\x3E\x38\x37\x1E", 6).c_str()] == true) {
std::string token = result[/*data*/ StrEnc("fAVA", "\x02\x20\x22\x20", 4).c_str()][/*token*/ StrEnc("{>3Lr", "\x0F\x51\x58\x29\x1C", 5).c_str()].get<std::string>();
time_t rng = result[/*data*/ StrEnc("fAVA", "\x02\x20\x22\x20", 4).c_str()][/*rng*/ StrEnc("+n,", "\x59\x00\x4B", 3).c_str()].get<time_t>();
//BHATIA_SLOT = result["data"]["BHATIA_SLOT"].get<std::string>();
expiredDate= result["data"]["EXP"].get<std::string>();
if (rng + 30 > time(0)) {
std::string auth = /*PUBG*/ StrEnc("Q*) ", "\x01\x7F\x6B\x67", 4).c_str();;
auth += "-";
auth += user_key;
auth += "-";
auth += UUID;
auth += "-";
auth += /*Vm8Lk7Uj2JmsjCPVPVjrLa7zgfx3uz9E*/ StrEnc("-2:uwZdV^%]?{{wHs2V,+(^NJU;kC*_{", "\x7B\x5F\x02\x39\x1C\x6D\x31\x3C\x6C\x6F\x30\x4C\x11\x38\x27\x1E\x23\x64\x3C\x5E\x67\x49\x69\x34\x2D\x33\x43\x58\x36\x50\x66\x3E", 32).c_str();
std::string outputAuth = Tools::CalcMD5(auth);
EXP = result["data"]["EXP"].get<std::string>();
g_Token = token;
g_Auth = outputAuth;
bValid = g_Token == g_Auth;
}
} else {
errMsg = result[/*reason*/ StrEnc("LW(3(c", "\x3E\x32\x49\x40\x47\x0D", 6).c_str()].get<std::string>();
}
} catch (json::exception &e) {
errMsg = "{";
errMsg += e.what();
errMsg += "}\n{";
errMsg += chunk.memory;
errMsg += "}";
}
} else {
errMsg = curl_easy_strerror(res);
}
}
curl_easy_cleanup(curl);
return bValid ? "OK" : errMsg;
}
// ======================================================================== //
void DrawTextCentered(const char *text) {
ImGui::Separator();
ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text).x) / 2.f);
ImGui::Text(text);
ImGui::Separator();}
void DrawText(std::string text) {
ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text.c_str()).x) / 2.f);
ImGui::Text(text.c_str());}
EGLBoolean( * orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean _eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
	eglQuerySurface(dpy, surface, EGL_WIDTH, & glWidth);
	eglQuerySurface(dpy, surface, EGL_HEIGHT, & glHeight);
	if (glWidth <= 0 || glHeight <= 0)
		return orig_eglSwapBuffers(dpy, surface);
	
if (!g_App)
return eglSwapBuffers(dpy, surface);

screenWidth = ANativeWindow_getWidth(g_App->window);
screenHeight = ANativeWindow_getHeight(g_App->window);
density = AConfiguration_getDensity(g_App->config);


     if (!initImGui) {
        ImGui::CreateContext();
        ImGuiStyle * style = & ImGui::GetStyle();
        style->WindowRounding = 5.5f;
        style->FrameRounding = 8.0f;
        style->ScrollbarRounding = 1.0f;
        style->FrameBorderSize = 1.5f;
        style->WindowBorderSize = 1.5f;
        style->ScrollbarSize = 10;
        style->CircleTessellationMaxError    = 6.0f;
        style->WindowTitleAlign = ImVec2(0.5, 0.5);
        style->FramePadding = ImVec2(6, 4);
        
        ImVec4 *colors = style->Colors;
        colors[ImGuiCol_Text]                   = ImColor(0, 0, 0, 255);
        colors[ImGuiCol_TextDisabled]           = ImColor(0, 0, 0, 255);
        colors[ImGuiCol_WindowBg]               = ImColor(255, 255, 255, 155);
        colors[ImGuiCol_ChildBg]                = ImColor(255, 255, 255, 0);
        colors[ImGuiCol_PopupBg]                = ImColor(255, 255, 255, 155);
        colors[ImGuiCol_Border]                 = ImColor(255, 255, 255, 255);
        colors[ImGuiCol_CheckMark]              = ImColor(0, 0, 0, 255);
        colors[ImGuiCol_BorderShadow]           = ImColor(255, 255, 255, 255);
        colors[ImGuiCol_FrameBg]                = ImColor(255, 255, 255, 155);
        colors[ImGuiCol_FrameBgHovered]         = ImColor(255, 255, 255, 155);
        colors[ImGuiCol_FrameBgActive]          = ImColor(255, 255, 255, 155);
        colors[ImGuiCol_TitleBg]                = ImColor(255, 255, 255, 155);
        colors[ImGuiCol_TitleBgActive]          = ImColor(255, 255, 255, 155);
        colors[ImGuiCol_TitleBgCollapsed]       = ImColor(255, 255, 255, 155);
        colors[ImGuiCol_Button]                 = ImColor(255, 255, 255, 155);
        colors[ImGuiCol_ButtonHovered]          = ImColor(255, 255, 255, 155);
        colors[ImGuiCol_ButtonActive]           = ImColor(255, 255, 255, 155);
    
        style->WindowTitleAlign = ImVec2(0.5f, 0.5f) ;
        style->ScaleAllSizes(std::max(1.0f, density / 180.0f));
        style->ScrollbarSize *= 0.7f;
        
        
ImGui_ImplOpenGL3_Init("#version 300 es");

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.IniFilename = NULL;

        // 
        static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
          ImFontConfig icons_config;
        ImFontConfig CustomFont;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.OversampleH = 2.5;
        icons_config.OversampleV = 2.5;


        CustomFont.FontDataOwnedByAtlas = false;
io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t *>(Custom), sizeof(Custom), 25.0f, &CustomFont);
 io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 24.0f, &icons_config, icons_ranges);
        ImFontConfig cfg;
        cfg.SizePixels = ((float) density / 40.0f);
        io.Fonts->AddFontDefault(&cfg);

        memset(&Config, 0, sizeof(Config));
        initImGui = true;
        Config.SilentAim.Enable = true;
        
    }


    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    ImGui::NewFrame();
  DrawEsp(ImGui::GetBackgroundDrawList());
 
ImVec2 center = ImGui::GetMainViewport()->GetCenter();

  if (LITE_ESP) {   // God Menu quick-access window (UP/DN + BOT/NOK/BT) - tied to the ESP switch.
	  ImGui::SetNextWindowSize(ImVec2(210, 0), ImGuiCond_Always);
	  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
	  if (ImGui::Begin(" ", 0, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar )) {
ImGui::SetWindowFontScale(0.85f);
ImGui::TableNextColumn();

pthread_t t;
const float gbw = 60.0f;
bool upOn = Godviewup, dnOn = Godviewdown;
if (upOn) { ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 190, 90, 230)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 210, 110, 255)); }
if (ImGui::Button("UP", ImVec2(gbw, 28))) {
Godviewup = !Godviewup;
if (Godviewup) { Godviewdown = false; pthread_create(&t, 0, run_thread, (void *)(101)); }
}
if (upOn) ImGui::PopStyleColor(2);
ImGui::SameLine();
if (dnOn) { ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(220, 120, 0, 230)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(240, 140, 0, 255)); }
if (ImGui::Button("DN", ImVec2(gbw, 28))) {
Godviewdown = !Godviewdown;
if (Godviewdown) { Godviewup = false; pthread_create(&t, 0, run_thread, (void *)(102)); }
}
if (dnOn) ImGui::PopStyleColor(2);

ImGui::Checkbox("BOT", &Config.AimBot.IgnoreBot);
ImGui::Checkbox("NOK", &Config.AimBot.IgnoreKnocked);
ImGui::Checkbox("BT", &SKYSHOT);
	}
	ImGui::End();
	ImGui::PopStyleVar();
	}
ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
ImGui::SetNextWindowSize(ImVec2((float)glWidth * 0.16f, (float)glHeight * 0.55f), ImGuiCond_Once);


if (ImGui::Begin((OBFUSCATE("VILLEN_BHAI")), 0, ImGuiWindowFlags_NoSavedSettings))
{
ImGui::SetWindowFontScale(0.72f);
DrawPinkBackground();
DrawPinkMeteorBorder(4.0f);

static bool isLogin = true;
if (!isLogin) {
static std::string err;
ImGui::Text(OBFUSCATE("Please Login! (Copy Key to Clipboard)"));
ImGui::PushItemWidth(-1);
static char s[64];
ImGui::InputText(OBFUSCATE("##key"), s, sizeof s);
auto key = getClipboardText();
strncpy(s, key.c_str(), sizeof s);
err = Login(s);
if (err == "OK") {
isLogin = bValid && g_Auth == g_Token;}
ImGui::PopItemWidth();
ImGui::PushItemWidth(-1);
if (ImGui::Button(OBFUSCATE("Paste Key"), ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
auto key = getClipboardText();
strncpy(s, key.c_str(), sizeof s);}
ImGui::PopItemWidth();
ImGui::PushItemWidth(-1);
if (ImGui::Button(OBFUSCATE("Login"), ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
err = Login(s);
if (err == "OK") {
isLogin = bValid && g_Auth == g_Token;}}
ImGui::PopItemWidth();
if (!err.empty() && err != "OK") {
ImGui::Text(OBFUSCATE("Error: %s"), err.c_str());}
ImGui::PopItemWidth();
} else{
 ImGui::TableNextColumn();
ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.6f);
ImGui::PushStyleColor(ImGuiCol_Border, ImColor(255, 255, 255, 255).Value);
ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColor(9, 36, 89, 0).Value);
ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
ImGui::BeginChild("mainchild", ImVec2(0, 0), true);
ImGui::PopStyleVar(2);
ImGui::PopStyleColor(2);       

pthread_t t;
DrawPinkGlowShimmerLine();
ImGui::SpacebarToggle("ESP", &LITE_ESP);
// Teammate ESP is not a separate toggle - it's tied directly to the main ESP switch
// above (see the LITE_ESP block that sets Config.PlayerESP.TeammateESP). ESP-while-dead
// was removed entirely - ESP now simply stops when your character is gone. The God Menu
// quick-access window (UP/DN + BOT/NOK/BT) is also tied to this same ESP switch now.

// HIGH DAMAGE toggle removed - always on now, auto-triggered once per match (see the
// Long Hand auto-trigger just below the local-player lookup for the same pattern).

DrawPinkGlowShimmerLine();
ImGui::SpacebarToggle("AUTO FIRE", &AutoFire2);

DrawPinkGlowShimmerLine();
ImGui::SpacebarToggle("UNLIMETED AIMMO", &AMMO);

DrawPinkGlowShimmerLine();
if (ImGui::SpacebarToggle("NEW FLASH", &UTAM_FLASH)) {
	pthread_t t;
pthread_create(&t, 0, run_thread, (void *)(4449));
}
// Sliders use a smaller FramePadding.y than the panel default (style->FramePadding, which
// also drives buttons/checkboxes) so only these three get shorter, not everything else.
ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 1));

DrawPinkGlowShimmerLine();
ImGui::SetNextItemWidth(-1.0f);
ImGui::SliderFloat("##VIEW", &WIDEVIEW1, 360.0f, 200.0f, "VIEW: %.0f");
ImGui::Spacing();

DrawPinkGlowShimmerLine();
ImGui::SetNextItemWidth(-1.0f);
// Already wired to the weapon's ShootInterval (see shoot_event, gated by SilentAim
// + FastChut) - it just had no UI control before. Lower = faster fire rate.
ImGui::SliderFloat("##FIRE", &ControlSpeed, 0.05f, 0.5f, "FIRE: %.2f");
ImGui::Spacing();

DrawPinkGlowShimmerLine();
// Uses the engine's own speed-scale field (see UTAMA_HUD) rather than a raw memory patch,
// so it also lifts the movement slowdown while aiming down sights / peeking, and the
// server accepts it instead of fighting it (which is what a byte-patch speed hack does).
// Always on - no toggle, only the scale is adjustable.
ImGui::SetNextItemWidth(-1.0f);
ImGui::SliderFloat("##MOVESPEED", &FastMovementScale, 1.0f, 2.5f, "SPEED: %.2fx");
ImGui::Spacing();

ImGui::PopStyleVar();

DrawPinkGlowShimmerLine();
DrawPinkGlowShimmerLine();

float totalWidth = ImGui::GetContentRegionAvail().x;
float spacing = ImGui::GetStyle().ItemSpacing.x;
float buttonWidth = (totalWidth - spacing) * 0.5f;

// GOD MENU toggle button removed - the quick-access window (UP/DN + BOT/NOK/BT) is
// always on screen now, so a separate on/off switch for it no longer does anything.

// ===== SKY PANEL Button =====
if (UTAM_FLY) {
    // Active State: Neon Hot Pink / Magenta Glow
    ImGui::PushStyleColor(ImGuiCol_Button,        IM_COL32(255, 50, 150, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(255, 90, 175, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  IM_COL32(204, 25, 115, 255));
    ImGui::Button("SKY FLY", ImVec2(buttonWidth, 50)); 
    ImGui::PopStyleColor(3);
} else {
    // Inactive State: Dark Purple Base
    ImGui::PushStyleColor(ImGuiCol_Button,        IM_COL32(56, 8, 31, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(89, 13, 51, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  IM_COL32(204, 25, 115, 255));
    ImGui::Button("SKY FLY", ImVec2(buttonWidth, 50)); //@UTAMARAM95
    ImGui::PopStyleColor(3);
}

if (ImGui::IsItemClicked()) {
    pthread_t t;
    UTAM_FLY = !UTAM_FLY;

pthread_create(&t, 0, run_thread, (void *)(4451));}
ImGui::Spacing(); 
DrawPinkGlowShimmerLine();
DrawPinkGlowShimmerLine();
ImGui::TableNextColumn();
}}
ImGui::End();
ImGui::Render();        
 ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
 return orig_eglSwapBuffers(dpy, surface);
    }
int32_t ( * orig_onInputEvent)(struct android_app * app, AInputEvent * inputEvent);
int32_t onInputEvent(struct android_app * app, AInputEvent * inputEvent) {
if (initImGui) {
ImGui_ImplAndroid_HandleInputEvent(inputEvent, {(float)screenWidth / (float)glWidth, (float)screenHeight / (float)glHeight});}
return orig_onInputEvent(app, inputEvent);}
void *(*oProcessEvent)(UObject *pObj, UFunction *pFunc, void *pArgs);
void *hkProcessEvent(UObject *pObj, UFunction *pFunc, void *pArgs) {
const char *EngineHUD = ("Function Engine.HUD.ReceiveDrawHUD");
if (pFunc) {
if (pFunc->GetFullName() == EngineHUD) {
AHUD *pHUD = (AHUD *) pObj;
if (pHUD) {
auto Params = (AHUD_ReceiveDrawHUD_Params *) pArgs;
if (Params) {
UTAMA_HUD(pHUD, Params->SizeX, Params->SizeY);}}}}
return oProcessEvent(pObj, pFunc, pArgs);}
TNameEntryArray *GetGNames(){
return ((TNameEntryArray * (*)())(UE4 + 0x19ee5f4))();}

void * main_thread(void *) {

UE4 = Tools::GetBaseAddress("libUE4.so");
while (!UE4) {
UE4 = Tools::GetBaseAddress("libUE4.so");
sleep(1);}
while (!g_App) {
g_App = * (android_app * *)(UE4 + GNativeAndroidApp_Offset);
sleep(1);}
FName::GNames = GetGNames();
while (!FName::GNames){
FName::GNames = GetGNames();
sleep(1);}
UObject::GUObjectArray = (FUObjectArray *)(UE4 + 0x4fd8d20);
while (!g_App->onInputEvent)
sleep(1);
orig_onInputEvent = decltype(orig_onInputEvent)(g_App->onInputEvent);
g_App->onInputEvent = onInputEvent;
plthook_t * plthook;
if (plthook_open( & plthook, "libUE4.so") == 0) {
plthook_replace(plthook, "eglSwapBuffers", (void *)_eglSwapBuffers, (void * *) & orig_eglSwapBuffers);
plthook_close(plthook);}
Tools::Hook((void *)(UE4 + 0x12C35C8), (void *)CalcShootRot, (void **)&oCalcShootRot);
Tools::Hook((void *)(UE4 + 0x25a5dc8), (void *)hkProcessEvent, (void **)&oProcessEvent);
Tools::Hook((void *) (UE4 + 0x12AA918), (void *) hk_GetUnlimitedAmmo,(void **) &orig_GetUnlimitedAmmo);

Patches.noshake1 = MemoryPatch::createWithHex("libUE4.so", 0xDB4C38, "00 00 00 00");
Patches.noshake3 = MemoryPatch::createWithHex("libUE4.so", 0x2648B80, "00 00 00 00");
Patches.noshake4 = MemoryPatch::createWithHex("libUE4.so", 0x26C4CAC, "00 00 00 00");
Patches.noshake5 = MemoryPatch::createWithHex("libUE4.so", 0x2742334, "00 00 00 00");
Patches.noshake6 = MemoryPatch::createWithHex("libUE4.so", 0x2742354, "00 00 00 00");

Patches.utam01 = MemoryPatch::createWithHex("libUE4.so", 0x2D107D0, "00 00 00 00"); 
Patches.utam02 = MemoryPatch::createWithHex("libUE4.so", 0x2C3BA14, "00 00 10 41"); 
Patches.utam03 = MemoryPatch::createWithHex("libUE4.so", 0xCBCB00, "00 00 50 E3"); 
Patches.utam04 = MemoryPatch::createWithHex("libUE4.so", 0x2684E28, "00 00 00 40"); 
Patches.utam05 = MemoryPatch::createWithHex("libUE4.so", 0x2704F44, "8F C2 75 3E"); 
Patches.utam06 = MemoryPatch::createWithHex("libUE4.so", 0x2710C8C, "00 00 00 00"); 
Patches.utam07 = MemoryPatch::createWithHex("libUE4.so", 0x2C3B974, "00 00 00 00");
Patches.utam08 = MemoryPatch::createWithHex("libUE4.so", 0x2C3B98C, "00 00 00 00"); 
Patches.utam09 = MemoryPatch::createWithHex("libUE4.so", 0x2C3BA18, "00 00 00 00"); 
Patches.utam10 = MemoryPatch::createWithHex("libUE4.so", 0x2A9DA10, "00 00 00 00"); 
Patches.utam11 = MemoryPatch::createWithHex("libUE4.so", 0x26F9C00, "00 00 00 00");

items_data = json::parse(JSON_ITEMS);
for (auto &category : items_data) {
    for (auto &item : category["Items"]) {
        Items[item["itemId"].get<int>()] = true;
    }
}
return 0;}
__attribute__((constructor)) void _init() {
pthread_t t;
pthread_create( & t, 0, main_thread, 0);}







