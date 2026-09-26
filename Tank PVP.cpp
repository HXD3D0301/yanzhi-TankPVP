#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

const char* MAP_NAMES[]     = { "废墟", "工厂", "实验室" };
const int   MAP_NAMES_COUNT = 3;

const char* TANK_NAMES[] = {
    "游侠", "穿云", "斥星", "深蓝", "钩锁",
    "狂热", "魅影", "磁暴", "基岩", "霜鸟", "暗猎"
};
const int   TANK_NAMES_COUNT = 11;

const char* TANK_ACTIVE_NAME[] = {
    "履带加速", "喷气机动", "震荡波", "量子护盾", "钢索钩爪",
    "枪管强化", "量子隧穿", "电磁脉冲", "离子方块", "零度领域", "数据采集"
};
const char* TANK_PASSIVE_NAME[] = {
    "恢复", "加速", "余威", "恢复", "散射开火",
    "机炮模式", "特质炮弹", "数据干扰", "构造感知", "低温炮弹", "弱点分析"
};
const char* TANK_SKILL_DESC[] = {
    "按住技能键增加移速",
    "八向喷气位移，方向由按键决定",
    "击退周围的敌人并造成伤害",
    "开启护盾以抵挡炮弹",
    "向前方发射钩锁，命中墙壁将自身拉向墙体，命中敌人将其拉向自身",
    "开启后射击开火间隔缩短至70%，枪口变热减慢，持续3秒",
    "立即进入虚化状态，无视墙体与炮弹，无法开火，持续5秒",
    "释放电磁干扰圈，受影响的坦克无法开火和使用技能",
    "在自身位置放置虚拟墙壁，可阻挡敌方坦克与炮弹",
    "释放零度领域，领域内内敌人持续受到减速效果",
    "释放探测圆环：触碰圆环的敌人被施加数据印记"
};
const char* TANK_PASSIVE_DESC[] = {
    "炮弹命中恢复部分能量条",
    "喷气位移后获得一段加速",
    "击退敌方坦克使其撞墙可造成禁止移动效果",
    "护盾成功抵挡后回复护盾耐久",
    "特殊开火模式：散弹",
    "特殊开火模式：机枪",
    "炮弹可以穿墙，每穿一格损失50速度和5伤害",
    "被磁暴炮弹命中的坦克CD条进度减少20%",
    "放置出的墙体不会阻挡自身的移动和炮弹",
    "炮弹命中的坦克受到减速效果",
    "炮弹命中叠加数据印记，每层印记使目标多受10伤害，最多3层"
};

#define SND_HUADONG  "snd_huadong"
#define SND_UI       "snd_ui"
#define SND_SHIQU    "snd_shiqu"
#define SND_WIN      "snd_win"
#define SND_BGM_GAME "bgm_game"
#define SND_BGM_MENU "bgm_menu"
#define SND_BAOZHA   "snd_baozha"
#define SND_JN2      "snd_jn2"
#define SND_JN3      "snd_jn3"
#define SND_JN4_1    "snd_jn4_1"
#define SND_JN4_2    "snd_jn4_2"
#define SND_JN4_3    "snd_jn4_3"
#define SND_JN5_1    "snd_jn5_1"
#define SND_JN5_2    "snd_jn5_2"
#define SND_JN6      "snd_jn6"
#define SND_JN7_1    "snd_jn7_1"
#define SND_JN7_2    "snd_jn7_2"
#define SND_JN8      "snd_jn8"
#define SND_JN9      "snd_jn9"
#define SND_JN10     "snd_jn10"
#define SND_JN11     "snd_jn11"
#define SND_KP1      "snd_kp1"
#define SND_KP2      "snd_kp2"
#define SND_KP3      "snd_kp3"
#define SND_KP5      "snd_kp5"
#define SND_KILL     "snd_kill"

#define SND_KP6_CH 4
const char* SND_KP6_ALIASES[SND_KP6_CH] = {
    "snd_kp6_0", "snd_kp6_1", "snd_kp6_2", "snd_kp6_3"
};
int g_kp6NextCh = 0;

#define P1_FWD       'W'
#define P1_BACK      'S'
#define P1_LEFT      'Q'
#define P1_RIGHT     'E'
#define P1_STRAFEL   'A'
#define P1_STRAFER   'D'
#define P1_FIRE      'F'
#define P1_SKILL     'G'
#define P2_FWD       'I'
#define P2_BACK      'K'
#define P2_LEFT      'U'
#define P2_RIGHT     'O'
#define P2_STRAFEL   'J'
#define P2_STRAFER   'L'
#define P2_FIRE      VK_OEM_1
#define P2_SKILL     VK_OEM_7
#define P3_FWD       VK_NUMPAD5
#define P3_BACK      VK_NUMPAD2
#define P3_STRAFEL   VK_NUMPAD1
#define P3_STRAFER   VK_NUMPAD3
#define P3_LEFT      VK_NUMPAD4
#define P3_RIGHT     VK_NUMPAD6
#define P3_FIRE      VK_NUMPAD9
#define P3_SKILL     VK_ADD

const int WIN_W = 800;
const int WIN_H = 600;

#define MAX_PLAYERS     3
#define MENU_COUNT      5
#define BASE_FONT_H     28
#define SELECT_SCALE    1.2f
#define FONT_NAME       "黑体"
#define MAP_W           16
#define MAP_H           16
#define TILE            100
#define WORLD_W         (MAP_W * TILE)
#define WORLD_H         (MAP_H * TILE)
#define MAX_MAPS        8
#define MAX_TANKS       11
#define PI              3.14159265f
#define PIXEL_ART_MODE  1
#define ALPHA_CUTOFF    128

#define DEF_FIRE_CD     0.5f
#define DEF_MOVE_SPD    160.0f
#define DEF_TURN_SPD    3.0f
#define DEF_DAMAGE      20.0f
#define DEF_BULLET_SPD  900.0f
#define DEF_HP          100.0f

#define TANK_SIZE       80.0f
#define TANK_MOVE_EASE  9.0f
#define BULLET_SIZE     44.0f
#define MUZZLE_TIME     0.03f
#define HP_SHADOW_SPD   120.0f
#define HP_SHADOW_RANGE 30.0f
#define CAM_EASE        5.0f

#define HIT_FLASH_TIME  0.5f
#define HIT_FLASH_COUNT 3
#define SKILL_FLASH_TIME 0.18f
#define SHIELD_FLASH_TIME 0.25f
#define CD_FULL_FLASH_TIME 0.6f
#define CD_FULL_FLASH_SEG  0.15f

#define CAM_MIN_SCALE   0.18f
#define CAM_MAX_SCALE   1.20f
#define CAM_PADDING     210.0f

#define MAX_BULLETS     128
#define MAX_PATH_LEN    256
#define MAX_AFTERIMG    32
#define AFTERIMG_DUR_CHUANYUN 0.95f
#define AFTERIMG_DUR_YOUXIA   0.26f
#define AFTERIMG_INTERVAL_YOUXIA 0.11f

#define SELECT_SLIDE_DIST    700.0f
#define SELECT_MIN_SCALE     0.30f
#define SELECT_ANIM_DURATION 0.45f

#define STAT_FIRE_MIN   0.10f
#define STAT_FIRE_MAX   5.00f
#define STAT_MOVE_MIN   0.0f
#define STAT_MOVE_MAX   300.0f
#define STAT_DMG_MIN    0.0f
#define STAT_DMG_MAX    100.0f
#define STAT_HP_MIN     50.0f
#define STAT_HP_MAX     200.0f

#define SKILL1_ACTIVATE_TH  0.30f
#define SKILL1_DRAIN_RATE   0.21f
#define SKILL1_RECOVER_RATE 0.105f
#define SKILL1_SPEED_MUL    1.60f
#define SKILL1_PASSIVE_CD   0.30f
#define SKILL1_STRAFE_COST  0.15f
#define SKILL1_STRAFE_DIST  (1.0f * TILE * 0.65f)
#define SKILL1_STRAFE_TIME  0.37f

#define SKILL2_RECOVER_RATE 0.125f
#define SKILL2_DASH_LONG    (2.5f * TILE)
#define SKILL2_DASH_SHORT   (2.0f * TILE)
#define SKILL2_DASH_DURATION 0.45f
#define SKILL2_DASH_TURN_MUL 0.50f
#define SKILL2_PASSIVE_TIME   3.0f
#define SKILL2_PASSIVE_MUL    1.30f

#define SKILL3_RECOVER_RATE  (0.20f / 1.70f)
#define SKILL3_RANGE         (2 * TILE)
#define SKILL3_KNOCK_DIST    (2 * TILE)
#define SKILL3_KNOCK_TIME    0.50f
#define SKILL3_STUN_TIME     2.00f
#define SKILL3_DAMAGE        10

#define SKILL4_OPEN_COST     0.15f
#define SKILL4_DRAIN_RATE    0.10f
#define SKILL4_RECOVER_RATE  0.05f
#define SKILL4_BLOCK_RECOVER 0.20f
#define SHIELD_DOT_THRESHOLD 0.65f
#define SHIELD_HALF_SPAN     (PI * 0.28f)
#define SHIELD_RADIUS        (TANK_SIZE * 0.62f)

#define HOOK_RANGE           (5.0f * TILE)
#define HOOK_FLY_SPEED       900.0f
#define HOOK_PULL_SELF_SPD   300.0f
#define HOOK_PULL_ENEMY_SPD  130.0f
#define HOOK_ENEMY_DMG_PER_TILE 5
#define HOOK_SELF_MOVE_MUL   0.40f
#define HOOK_CD              (1.0f / 8.0f)
#define HOOK_RELEASE_DIST    (TILE * 1.0f)

#define HEAT_MAX             100.0f
#define HEAT_RATE_UP         15.0f
#define HEAT_RATE_DOWN       12.0f
#define HEAT_OVERHEAT        100.0f
#define HEAT_RECOVER_TH      60.0f
#define FRENZY_DURATION      3.0f
#define FRENZY_FIRE_MUL      0.70f
#define FRENZY_HEAT_RATE     5.0f
#define FRENZY_CD            15.0f

#define PHASE_DURATION       5.0f
#define PHASE_CD             10.0f
#define PHASE_FADE_IN        1.0f
#define PHASE_WALL_DPS       5.0f

#define EMP_RADIUS           (3.0f * TILE)
#define EMP_EXPAND_TIME      1.0f
#define EMP_HOLD_TIME        5.0f
#define EMP_SHRINK_TIME      1.0f
#define EMP_TOTAL_TIME       7.0f
#define EMP_CD               12.0f
#define EMP_SILENCE_CD_CUT   0.20f

#define BEDROCK_PLACE_COST   0.20f
#define BEDROCK_RECOVER_RATE 0.06f
#define VWALL_HP             100.0f
#define VWALL_DECAY          5.0f
#define VWALL_FADE_IN        0.5f
#define VWALL_FADE_OUT       1.0f
#define MAX_VWALLS           64

#define FROST_RADIUS         (4.0f * TILE)
#define FROST_EXPAND         2.0f
#define FROST_HOLD           5.0f
#define FROST_SHRINK         2.0f
#define FROST_TOTAL          9.0f
#define FROST_CD             15.0f
#define FROST_SLOW_RATE      0.20f
#define FROST_MIN_FACTOR     0.40f
#define FROST_SHELL_FACTOR   0.70f
#define FROST_SHELL_TIME     1.5f

#define HUNTER_RING_MAX      (5.0f * TILE)
#define HUNTER_EXPAND        2.0f
#define HUNTER_FADE          2.0f
#define HUNTER_TOTAL         4.0f
#define HUNTER_CD            10.0f
#define HUNTER_MARK_DMG      10
#define HUNTER_MARK_MAX      3
#define HUNTER_RING_ALPHA    170

#define COMP_RESPAWN_TIME    5.0f
#define COMP_RESPAWN_INV     3.0f
#define COMP_DEFAULT_TARGET  5

#define MARK_ALPHA_1         76
#define MARK_ALPHA_2         127
#define MARK_ALPHA_3         204
#define MARK_RGB_R           20
#define MARK_RGB_G           90
#define MARK_RGB_B           30

#define STRAFE_SPEED_MUL     0.70f

#define AI_RECOMPUTE_INTERVAL 0.80f
#define AI_ESCAPE_LOCK        0.60f
#define AI_AIM_NOISE          0.10f

#define INVINCIBLE_TIME      3.0f

#define MAX_BUFFS            8
#define BUFF_HEAL            0
#define BUFF_SPEED           1
#define BUFF_FIRE            2
#define BUFF_DAMAGE          3
#define BUFF_SIZE            80.0f
#define BUFF_PICKUP_R        80.0f
#define BUFF_HEAL_TIME       5.0f
#define BUFF_HEAL_PER_SEC    8.0f
#define BUFF_SPEED_TIME      10.0f
#define BUFF_SPEED_MUL       1.30f
#define BUFF_FIRE_TIME       8.0f
#define BUFF_FIRE_MUL        0.70f
#define BUFF_DAMAGE_TIME     10.0f
#define BUFF_DAMAGE_MUL      1.30f
#define BUFF_FADE_IN         2.0f
#define BUFF_BLINK_CYCLE     0.25f
#define BUFF_BLINK_COUNT     3

#define BT_NORMAL   0
#define BT_PIERCE   1

#define PIERCE_SPD_LOSS 50.0f
#define PIERCE_DMG_LOSS 5

#define RANK_BASE_Y  120
#define RANK_ROW_H   26
#define RANK_PANEL_W 150
#define RANK_PANEL_H 170

enum GameState
{
    ST_MENU = 0, ST_TRANS_IN, ST_TRANS_HOLD, ST_TRANS_OUT,
    ST_MAP_SELECT, ST_TANK_SELECT, ST_TANK_INFO,
    ST_PVP_COUNT, ST_PVP_PLAY, ST_PVP_OVER, ST_HELP
};

enum GameMode { GM_SINGLE = 0, GM_DEATHMATCH, GM_COMPETITIVE };

struct MenuItem { char text[32]; int cx, cy; };

struct Tank
{
    float x, y, bodyAngle, vel;
    int   hp, maxHp;
    float shadowHp;
    float cooldown, muzzleTimer, hitFlashTimer, skillFlashTimer, skillCd;
    bool  skillActive, dashActive;
    float dashTimer, dashDuration, dashStartX, dashStartY, dashTargetX, dashTargetY;
    bool  knockbackActive;
    float knockbackTimer, knockbackDuration;
    float knockbackStartX, knockbackStartY, knockbackTargetX, knockbackTargetY;
    bool  knockbackHitWall;
    float stunTimer, speedBoostTimer, afterimageSpawnTimer;
    bool  shieldActive;
    float shieldFlashTimer, cdFullFlashTimer;
    int   hookState;
    float hookX, hookY, hookAngle, hookDist, hookStartX, hookStartY;
    int   hookTarget;
    float dmgAccum;
    float heat;
    bool  overheated;
    float frenzyTimer, frenzyCd;
    float phaseTimer, phaseAlpha;
    bool  inWall, phaseStuckInWall;
    float phaseStuckDmg;
    float empTimer;
    float deathFade;
    bool  deathSoundPlayed;
    int   buffType;
    float buffTimer, buffTotal, buffHealAccum;
    float slowFactor;
    float frostShellTimer;
    int   marks[MAX_PLAYERS];
};

struct Bullet
{
    float x, y, vx, vy, angle;
    bool alive;
    int  owner, damage, type;
    int  lastCellC, lastCellR;
};

struct RingFx { bool active; float timer, duration, x, y; };
struct AfterImage { bool active; float timer, duration, x, y, angle; int tankIdx; };
struct BuffDrop { bool active; int type; float x, y, spawnTimer; };
struct VirtualWall { bool active; int col, row; float hp, alpha; bool fadingOut; int owner; };
struct FrostField { bool active; float timer, x, y; };
struct HunterRing { bool active; float timer, x, y; bool hit[MAX_PLAYERS]; };

GameState g_state      = ST_MENU;
GameState g_transPrev  = ST_MENU;
GameState g_transNext  = ST_MENU;
float     g_transTimer = 0.0f;

MenuItem g_menu[MENU_COUNT];
int      g_selected = 0;
int      g_menuSubIdx[MENU_COUNT] = {0, 0, 0, 0, 0};
const char* MENU_SUB_NAMES[] = { "双人", "三人" };

GameMode g_gameMode = GM_DEATHMATCH;
int  g_playerCount    = 2;
bool g_isSinglePlayer = false;
float g_invincibleTimer = 0.0f;

int   g_score[MAX_PLAYERS] = {0, 0, 0};
int   g_winsumTarget = COMP_DEFAULT_TARGET;
float g_respawnTimer[MAX_PLAYERS] = {0, 0, 0};
float g_respawnInv[MAX_PLAYERS] = {0, 0, 0};
float g_rankPlayerY[MAX_PLAYERS] = {120, 146, 172};
int   g_rankPlayerSlot[MAX_PLAYERS] = {0, 1, 2};

int   g_marks[MAX_PLAYERS][MAX_PLAYERS];

float g_aiPathTimer[MAX_PLAYERS]  = {0, 0, 0};
float g_aiStuckTimer[MAX_PLAYERS] = {0, 0, 0};
float g_aiEscapeTimer[MAX_PLAYERS]= {0, 0, 0};
float g_aiLastX[MAX_PLAYERS]      = {0, 0, 0};
float g_aiLastY[MAX_PLAYERS]      = {0, 0, 0};
int   g_aiPathR[MAX_PLAYERS][MAX_PATH_LEN];
int   g_aiPathC[MAX_PLAYERS][MAX_PATH_LEN];
int   g_aiPathLen[MAX_PLAYERS]    = {0, 0, 0};
int   g_aiCellR[MAX_PLAYERS]      = {-1, -1, -1};
int   g_aiCellC[MAX_PLAYERS]      = {-1, -1, -1};

float g_prevSkillCd[MAX_PLAYERS]  = {1, 1, 1};

IMAGE g_bg, g_helpImg, g_gousuoTex, g_qiangtiTex;
IMAGE g_mapWall[MAX_MAPS], g_mapFloor[MAX_MAPS];
int   g_mapCount = 0;
IMAGE g_tankTex[MAX_TANKS], g_tankMuzzle[MAX_TANKS], g_tankBullet[MAX_TANKS];
IMAGE g_buffTex[4];
int   g_tankCount = 0;
float g_tankStats[MAX_TANKS][6];

int   g_mapSelCurIdx = 0, g_mapSelOldIdx = 0;
float g_mapSelAnimT  = 1.0f;
int   g_mapSelDir    = 1;

int   g_tankSelCurIdx[MAX_PLAYERS] = {0, 0, 0};
int   g_tankSelOldIdx[MAX_PLAYERS] = {0, 0, 0};
float g_tankSelAnimT[MAX_PLAYERS]  = {1.0f, 1.0f, 1.0f};
int   g_tankSelDir[MAX_PLAYERS]    = {1, 1, 1};

int   g_tankInfoCurIdx = 0, g_tankInfoOldIdx = 0;
float g_tankInfoAnimT  = 1.0f;
int   g_tankInfoDir    = 1;
float g_tankInfoBar[4] = {0.5f, 0.5f, 0.5f, 0.5f};

int   g_selMapIdx = 0;
int   g_selTankIdx[MAX_PLAYERS] = {0, 0, 0};

int    g_map[MAP_H][MAP_W];
Tank   g_tank[MAX_PLAYERS];
Bullet g_bullets[MAX_BULLETS];
RingFx g_rings[MAX_PLAYERS];
AfterImage g_afterImgs[MAX_AFTERIMG];
BuffDrop   g_buffs[MAX_BUFFS];
VirtualWall g_vwalls[MAX_VWALLS];
FrostField  g_frostFields[MAX_PLAYERS];
HunterRing  g_hunterRings[MAX_PLAYERS];
float  g_buffSpawnTimer = 0.0f;
float  g_buffSpawnNext  = 15.0f;

int g_ignoreVWallOwner = -1;
float g_animTime = 0.0f;

float g_camX = 0.0f, g_camY = 0.0f, g_camScale = 1.0f;
bool  g_camInit = false;

float g_countTimer = 0.0f, g_overTimer = 0.0f, g_overMaskA = 0.0f;

bool  g_tintWhite = false, g_tintDark = false;
float g_globalAlpha = 255.0f;
int   g_currentBgm = -1;

bool  g_keyCur[256], g_keyPrev[256];
DWORD g_lastTick = 0;
float g_dt = 0.0f;
bool  g_quit = false;

void InitGame();
void UpdateCamera();
float CalcStatRatio(int tIdx, int statIdx);
bool CollideWithWall(float x, float y);
bool LineOfSight(float x1, float y1, float x2, float y2);
void SpawnAfterImage(int tankIdx, float x, float y, float angle, float duration);
bool StartDash(int idx, float wdx, float wdy, float maxDist, float duration, bool isChuanyun);
void FireBullet(int idx, float aimOffset, int dmgOverride, int bType);
float GetEmpRadius(const Tank& t);
bool IsTankSilenced(int idx);
void LoadWinsum();
void CastKnockback(int casterIdx);
void UpdateShieldLogic(Tank& t, bool skillPressed);
bool PlaceVirtualWall(int tankIdx);
void UpdateAITank(int idx);
void LoadResources();
void LoadTankStats();

void SetAudioVolume(const char* alias, int vol)
{
    if (vol < 0) vol = 0; if (vol > 1000) vol = 1000;
    char cmd[128];
    sprintf(cmd, "setaudio %s volume to %d", alias, vol);
    mciSendString(cmd, NULL, 0, NULL);
}

void OpenAudio()
{
    mciSendString("open \"ui/huadong.mp3\" alias " SND_HUADONG, NULL, 0, NULL);
    mciSendString("open \"ui/UI.mp3\" alias " SND_UI, NULL, 0, NULL);
    mciSendString("open \"map/shiqu.mp3\" alias " SND_SHIQU, NULL, 0, NULL);
    mciSendString("open \"map/win.mp3\" alias " SND_WIN, NULL, 0, NULL);
    mciSendString("open \"ui/music1.mp3\" alias " SND_BGM_GAME, NULL, 0, NULL);
    mciSendString("open \"ui/music2.mp3\" alias " SND_BGM_MENU, NULL, 0, NULL);
    mciSendString("open \"tank/baozha.mp3\" alias " SND_BAOZHA, NULL, 0, NULL);
    mciSendString("open \"tank/jinengmp3/jineng2.mp3\" alias " SND_JN2, NULL, 0, NULL);
    mciSendString("open \"tank/jinengmp3/jineng3.mp3\" alias " SND_JN3, NULL, 0, NULL);
    mciSendString("open \"tank/jinengmp3/jineng4_1.mp3\" alias " SND_JN4_1, NULL, 0, NULL);
    mciSendString("open \"tank/jinengmp3/jineng4_2.mp3\" alias " SND_JN4_2, NULL, 0, NULL);
    mciSendString("open \"tank/jinengmp3/jineng4_3.mp3\" alias " SND_JN4_3, NULL, 0, NULL);
    mciSendString("open \"tank/jinengmp3/jineng5_1.mp3\" alias " SND_JN5_1, NULL, 0, NULL);
    mciSendString("open \"tank/jinengmp3/jineng5_2.mp3\" alias " SND_JN5_2, NULL, 0, NULL);
    mciSendString("open \"tank/jinengmp3/jineng6.mp3\" alias " SND_JN6, NULL, 0, NULL);
    mciSendString("open \"tank/jinengmp3/jineng7_1.mp3\" alias " SND_JN7_1, NULL, 0, NULL);
    mciSendString("open \"tank/jinengmp3/jineng7_2.mp3\" alias " SND_JN7_2, NULL, 0, NULL);
    mciSendString("open \"tank/jinengmp3/jineng8.mp3\" alias " SND_JN8, NULL, 0, NULL);
    mciSendString("open \"tank/jinengmp3/jineng9.mp3\" alias " SND_JN9, NULL, 0, NULL);
    mciSendString("open \"tank/jinengmp3/jineng10.mp3\" alias " SND_JN10, NULL, 0, NULL);
    mciSendString("open \"tank/jinengmp3/jineng11.mp3\" alias " SND_JN11, NULL, 0, NULL);
    mciSendString("open \"tank/kaipaomp3/kaipao1.mp3\" alias " SND_KP1, NULL, 0, NULL);
    mciSendString("open \"tank/kaipaomp3/kaipao2.mp3\" alias " SND_KP2, NULL, 0, NULL);
    mciSendString("open \"tank/kaipaomp3/kaipao3.mp3\" alias " SND_KP3, NULL, 0, NULL);
    mciSendString("open \"tank/kaipaomp3/kaipao5.mp3\" alias " SND_KP5, NULL, 0, NULL);
    mciSendString("open \"tank/kill.mp3\" alias " SND_KILL, NULL, 0, NULL);
    for (int i = 0; i < SND_KP6_CH; i++)
    {
        char cmd[200];
        sprintf(cmd, "open \"tank/kaipaomp3/kaipao6.mp3\" alias %s", SND_KP6_ALIASES[i]);
        mciSendString(cmd, NULL, 0, NULL);
    }
    SetAudioVolume(SND_HUADONG, 600);
    SetAudioVolume(SND_BGM_MENU, 490);
    SetAudioVolume(SND_BGM_GAME, 240);
    SetAudioVolume(SND_UI, 1000);
    SetAudioVolume(SND_SHIQU, 1000);
    SetAudioVolume(SND_WIN, 1000);
    SetAudioVolume(SND_KILL, 1000);
}

void CloseAudio()
{
    const char* aliases[] = {
        SND_HUADONG, SND_UI, SND_SHIQU, SND_WIN, SND_BGM_GAME, SND_BGM_MENU,
        SND_BAOZHA, SND_JN2, SND_JN3, SND_JN4_1, SND_JN4_2, SND_JN4_3,
        SND_JN5_1, SND_JN5_2, SND_JN6, SND_JN7_1, SND_JN7_2, SND_JN8,
        SND_JN9, SND_JN10, SND_JN11, SND_KP1, SND_KP2, SND_KP3, SND_KP5, SND_KILL
    };
    for (int i = 0; i < (int)(sizeof(aliases) / sizeof(aliases[0])); i++)
    {
        char cmd[128];
        sprintf(cmd, "close %s", aliases[i]);
        mciSendString(cmd, NULL, 0, NULL);
    }
    for (int i = 0; i < SND_KP6_CH; i++)
    {
        char cmd[128];
        sprintf(cmd, "close %s", SND_KP6_ALIASES[i]);
        mciSendString(cmd, NULL, 0, NULL);
    }
}

void PlaySFX(const char* alias)
{
    char cmd[128];
    sprintf(cmd, "stop %s", alias);          mciSendString(cmd, NULL, 0, NULL);
    sprintf(cmd, "seek %s to start", alias); mciSendString(cmd, NULL, 0, NULL);
    sprintf(cmd, "play %s", alias);          mciSendString(cmd, NULL, 0, NULL);
}

void PlaySFXKp6()
{
    char cmd[200], status[64];
    for (int i = 0; i < SND_KP6_CH; i++)
    {
        int ch = (g_kp6NextCh + i) % SND_KP6_CH;
        status[0] = 0;
        sprintf(cmd, "status %s mode", SND_KP6_ALIASES[ch]);
        mciSendString(cmd, status, 63, NULL);
        if (strstr(status, "playing") == NULL)
        {
            sprintf(cmd, "seek %s to start", SND_KP6_ALIASES[ch]); mciSendString(cmd, NULL, 0, NULL);
            sprintf(cmd, "play %s", SND_KP6_ALIASES[ch]); mciSendString(cmd, NULL, 0, NULL);
            g_kp6NextCh = (ch + 1) % SND_KP6_CH;
            return;
        }
    }
    int ch = g_kp6NextCh;
    sprintf(cmd, "seek %s to start", SND_KP6_ALIASES[ch]); mciSendString(cmd, NULL, 0, NULL);
    sprintf(cmd, "play %s", SND_KP6_ALIASES[ch]); mciSendString(cmd, NULL, 0, NULL);
    g_kp6NextCh = (ch + 1) % SND_KP6_CH;
}

void PlayBgm(const char* alias)
{
    mciSendString("stop " SND_BGM_GAME, NULL, 0, NULL);
    mciSendString("stop " SND_BGM_MENU, NULL, 0, NULL);
    char cmd[128];
    sprintf(cmd, "seek %s to start", alias); mciSendString(cmd, NULL, 0, NULL);
    sprintf(cmd, "play %s repeat", alias);   mciSendString(cmd, NULL, 0, NULL);
}

void SwitchBgm(int which)
{
    if (which == g_currentBgm) return;
    g_currentBgm = which;
    if (which == 0) PlayBgm(SND_BGM_MENU);
    else            PlayBgm(SND_BGM_GAME);
}

bool KeyDown(int vk)    { return g_keyCur[vk]; }
bool KeyPressed(int vk) { return g_keyCur[vk] && !g_keyPrev[vk]; }
void ResetKeys() { for (int i = 0; i < 256; i++) g_keyPrev[i] = true; }

float WorldToScreenX(float wx) { return (wx - g_camX) * g_camScale + WIN_W * 0.5f; }
float WorldToScreenY(float wy) { return (wy - g_camY) * g_camScale + WIN_H * 0.5f; }

bool CircleRectCollide(float cx, float cy, float r, float rx, float ry, float rw, float rh)
{
    float closestX = cx;
    if (closestX < rx) closestX = rx;
    else if (closestX > rx + rw) closestX = rx + rw;
    float closestY = cy;
    if (closestY < ry) closestY = ry;
    else if (closestY > ry + rh) closestY = ry + rh;
    float dx = cx - closestX, dy = cy - closestY;
    return dx * dx + dy * dy < r * r;
}

bool CircleHitAnyWall(float cx, float cy, float r)
{
    if (cx - r < 0.0f || cy - r < 0.0f || cx + r > (float)WORLD_W || cy + r > (float)WORLD_H) return true;
    int c0 = (int)floorf((cx - r) / TILE), c1 = (int)floorf((cx + r) / TILE);
    int r0 = (int)floorf((cy - r) / TILE), r1 = (int)floorf((cy + r) / TILE);
    if (c0 < 0) c0 = 0; if (r0 < 0) r0 = 0;
    if (c1 >= MAP_W) c1 = MAP_W - 1; if (r1 >= MAP_H) r1 = MAP_H - 1;
    for (int rr = r0; rr <= r1; rr++)
        for (int cc = c0; cc <= c1; cc++)
            if (g_map[rr][cc] == 1)
                if (CircleRectCollide(cx, cy, r, cc*(float)TILE, rr*(float)TILE, (float)TILE, (float)TILE)) return true;
    for (int i = 0; i < MAX_VWALLS; i++)
    {
        if (!g_vwalls[i].active) continue;
        if (g_vwalls[i].fadingOut) continue;
        if (g_ignoreVWallOwner >= 0 && g_vwalls[i].owner == g_ignoreVWallOwner) continue;
        float rx = g_vwalls[i].col * (float)TILE, ry = g_vwalls[i].row * (float)TILE;
        if (CircleRectCollide(cx, cy, r, rx, ry, (float)TILE, (float)TILE)) return true;
    }
    return false;
}

bool CollideWithWall(float x, float y) { return CircleHitAnyWall(x, y, TANK_SIZE * 0.5f); }

bool IsCellWall(int c, int r)
{
    if (c < 0 || r < 0 || c >= MAP_W || r >= MAP_H) return true;
    return g_map[r][c] == 1;
}

bool LineOfSight(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1, dy = y2 - y1;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 1.0f) return true;
    float r = TANK_SIZE * 0.5f;
    int steps = (int)(dist / 15.0f) + 1;
    if (steps < 2) steps = 2;
    for (int i = 1; i < steps; i++)
    {
        float t = (float)i / (float)steps;
        float px = x1 + dx * t, py = y1 + dy * t;
        if (CircleHitAnyWall(px, py, r)) return false;
    }
    return true;
}

void LoadWinsum()
{
    g_winsumTarget = COMP_DEFAULT_TARGET;
    FILE* fp = fopen("map/winsum.txt", "r");
    if (!fp) return;
    int v;
    if (fscanf(fp, "%d", &v) == 1 && v > 0) g_winsumTarget = v;
    fclose(fp);
}

void FillRectAlpha(int x, int y, int w, int h, int alpha)
{
    if (alpha <= 0) return; if (alpha > 255) alpha = 255;
    DWORD* buf = GetImageBuffer(NULL);
    if (!buf) return;
    int x0 = x, y0 = y, x1 = x + w, y1 = y + h;
    if (x0 < 0) x0 = 0; if (y0 < 0) y0 = 0;
    if (x1 > WIN_W) x1 = WIN_W; if (y1 > WIN_H) y1 = WIN_H;
    int ia = 255 - alpha;
    for (int yy = y0; yy < y1; yy++)
    {
        DWORD* row = buf + yy * WIN_W;
        for (int xx = x0; xx < x1; xx++)
        {
            DWORD d = row[xx];
            int dr = (d >> 16) & 0xFF, dg = (d >> 8) & 0xFF, db = d & 0xFF;
            row[xx] = 0xFF000000 | ((dr * ia / 255) << 16) | ((dg * ia / 255) << 8) | (db * ia / 255);
        }
    }
}

void DrawCircleRing(float cx, float cy, float radius, float thickness, int alpha, int cR, int cG, int cB)
{
    if (alpha <= 0 || alpha > 255 || radius <= 0.5f) return;
    DWORD* buf = GetImageBuffer(NULL);
    if (!buf) return;
    float rIn = radius - thickness * 0.5f, rOut = radius + thickness * 0.5f;
    if (rIn < 0.0f) rIn = 0.0f;
    int x0 = (int)(cx - rOut) - 1, y0 = (int)(cy - rOut) - 1;
    int x1 = (int)(cx + rOut) + 2, y1 = (int)(cy + rOut) + 2;
    if (x0 < 0) x0 = 0; if (y0 < 0) y0 = 0;
    if (x1 > WIN_W) x1 = WIN_W; if (y1 > WIN_H) y1 = WIN_H;
    if (x0 >= x1 || y0 >= y1) return;
    float rIn2 = rIn * rIn, rOut2 = rOut * rOut;
    int ia = 255 - alpha;
    for (int yy = y0; yy < y1; yy++)
    {
        DWORD* row = buf + yy * WIN_W;
        float dy = yy - cy, dy2 = dy * dy;
        for (int xx = x0; xx < x1; xx++)
        {
            float dx = xx - cx, d2 = dx * dx + dy2;
            if (d2 < rIn2 || d2 > rOut2) continue;
            DWORD c = row[xx];
            int cr = (c >> 16) & 0xFF, cg = (c >> 8) & 0xFF, cb = c & 0xFF;
            row[xx] = 0xFF000000 | (((cR * alpha + cr * ia) / 255) << 16)
                    | (((cG * alpha + cg * ia) / 255) << 8) | ((cB * alpha + cb * ia) / 255);
        }
    }
}

void DrawInvincibleShield(float cx, float cy, float radius)
{
    if (radius <= 0.5f) return;
    DWORD* buf = GetImageBuffer(NULL);
    if (!buf) return;
    int x0 = (int)(cx - radius) - 1, y0 = (int)(cy - radius) - 1;
    int x1 = (int)(cx + radius) + 2, y1 = (int)(cy + radius) + 2;
    if (x0 < 0) x0 = 0; if (y0 < 0) y0 = 0;
    if (x1 > WIN_W) x1 = WIN_W; if (y1 > WIN_H) y1 = WIN_H;
    if (x0 >= x1 || y0 >= y1) return;
    float r2 = radius * radius;
    for (int yy = y0; yy < y1; yy++)
    {
        DWORD* row = buf + yy * WIN_W;
        float dy = yy - cy, dy2 = dy * dy;
        for (int xx = x0; xx < x1; xx++)
        {
            float dx = xx - cx, d2 = dx * dx + dy2;
            if (d2 > r2) continue;
            float ratio = sqrtf(d2) / radius;
            int alpha = (int)(ratio * ratio * ratio * 220.0f);
            if (alpha <= 0) continue;
            int sr = 100, sg = 180, sb = 255;
            int ia = 255 - alpha;
            DWORD c = row[xx];
            int cr = (c >> 16) & 0xFF, cg = (c >> 8) & 0xFF, cb = c & 0xFF;
            row[xx] = 0xFF000000 | (((sr * alpha + cr * ia) / 255) << 16)
                    | (((sg * alpha + cg * ia) / 255) << 8) | ((sb * alpha + cb * ia) / 255);
        }
    }
}

void DrawShieldArc(float cx, float cy, float radius, float angle, bool flash)
{
    DWORD* buf = GetImageBuffer(NULL);
    if (!buf) return;
    float th = 9.0f;
    float rIn = radius - th * 0.5f, rOut = radius + th * 0.5f;
    if (rIn < 0.0f) rIn = 0.0f;
    int x0 = (int)(cx - rOut) - 1, y0 = (int)(cy - rOut) - 1;
    int x1 = (int)(cx + rOut) + 2, y1 = (int)(cy + rOut) + 2;
    if (x0 < 0) x0 = 0; if (y0 < 0) y0 = 0;
    if (x1 > WIN_W) x1 = WIN_W; if (y1 > WIN_H) y1 = WIN_H;
    if (x0 >= x1 || y0 >= y1) return;
    int sr = flash ? 255 : 90, sg = flash ? 255 : 180, sb = flash ? 255 : 255;
    int alpha = flash ? 240 : 190;
    float rIn2 = rIn * rIn, rOut2 = rOut * rOut;
    int ia = 255 - alpha;
    float halfSpan = SHIELD_HALF_SPAN;
    for (int yy = y0; yy < y1; yy++)
    {
        DWORD* row = buf + yy * WIN_W;
        float dy = yy - cy, dy2 = dy * dy;
        for (int xx = x0; xx < x1; xx++)
        {
            float dx = xx - cx, d2 = dx * dx + dy2;
            if (d2 < rIn2 || d2 > rOut2) continue;
            float a = atan2f(dy, dx);
            float diff = a - angle;
            while (diff > PI) diff -= 2 * PI;
            while (diff < -PI) diff += 2 * PI;
            if (fabsf(diff) > halfSpan) continue;
            DWORD c = row[xx];
            int cr = (c >> 16) & 0xFF, cg = (c >> 8) & 0xFF, cb = c & 0xFF;
            row[xx] = 0xFF000000 | (((sr * alpha + cr * ia) / 255) << 16)
                    | (((sg * alpha + cg * ia) / 255) << 8) | ((sb * alpha + cb * ia) / 255);
        }
    }
}

void DrawHookLine(float x1, float y1, float x2, float y2)
{
    setlinecolor(RGB(150, 150, 150));
    setlinestyle(PS_SOLID, 3);
    line((int)x1, (int)y1, (int)x2, (int)y2);
    setlinestyle(PS_SOLID, 1);
}

void DrawSprite(IMAGE* src, float scrCx, float scrCy, float scale, float angle)
{
    if (!src) return;
    int sw = src->getwidth(), sh = src->getheight();
    if (sw <= 0 || sh <= 0 || scale <= 0.0001f) return;
    DWORD* srcBuf = GetImageBuffer(src);
    DWORD* scrBuf = GetImageBuffer(NULL);
    if (!srcBuf || !scrBuf) return;
    int cxI = (int)(scrCx + 0.5f), cyI = (int)(scrCy + 0.5f);
    float ca = cosf(angle), sa = sinf(angle);
    float dw = sw * scale, dh = sh * scale;
    float halfWf = 0.5f * (fabs(dw * ca) + fabs(dh * sa));
    float halfHf = 0.5f * (fabs(dw * sa) + fabs(dh * ca));
    int halfW = (int)ceilf(halfWf) + 1, halfH = (int)ceilf(halfHf) + 1;
    int bw = halfW * 2 + 1, bh = halfH * 2 + 1;
    int ox = cxI - halfW, oy = cyI - halfH;
    int x0 = 0, y0 = 0, x1 = bw, y1 = bh;
    if (ox + x1 <= 0 || ox + x0 >= WIN_W) return;
    if (oy + y1 <= 0 || oy + y0 >= WIN_H) return;
    if (ox + x0 < 0) x0 = -ox;
    if (oy + y0 < 0) y0 = -oy;
    if (ox + x1 > WIN_W) x1 = WIN_W - ox;
    if (oy + y1 > WIN_H) y1 = WIN_H - oy;
    float invS = 1.0f / scale;
    float ssw = sw * 0.5f, ssh = sh * 0.5f;
    bool tW = g_tintWhite, tD = g_tintDark;
    float gA = g_globalAlpha;
    for (int y = y0; y < y1; y++)
    {
        DWORD* scrRow = scrBuf + (oy + y) * WIN_W + ox;
        float sdy = (float)(oy + y) + 0.5f - (float)cyI;
        for (int x = x0; x < x1; x++)
        {
            float sdx = (float)(ox + x) + 0.5f - (float)cxI;
            float src_dx = sdx * ca + sdy * sa;
            float src_dy = -sdx * sa + sdy * ca;
            float spx = src_dx * invS + ssw, spy = src_dy * invS + ssh;
            int ix = (int)floorf(spx), iy = (int)floorf(spy);
            if (ix < 0 || iy < 0 || ix >= sw || iy >= sh) continue;
            DWORD c = srcBuf[iy * sw + ix];
            int a = (c >> 24) & 0xFF;
#if PIXEL_ART_MODE
            if (a < ALPHA_CUTOFF) continue;
            a = 255;
#else
            if (a == 0) continue;
#endif
            if (gA < 255) { a = (int)(a * gA / 255.0f); if (a == 0) continue; }
            int sr = (c >> 16) & 0xFF, sg = (c >> 8) & 0xFF, sb = c & 0xFF;
            if (tW) { sr = 255; sg = 255; sb = 255; }
            else if (tD) { sr = sr * 45 / 100; sg = sg * 45 / 100; sb = sb * 45 / 100; }
            DWORD d = scrRow[x];
            int nr, ng, nb;
            if (a == 255) { nr = sr; ng = sg; nb = sb; }
            else
            {
                int ia = 255 - a;
                int dr = (d >> 16) & 0xFF, dg = (d >> 8) & 0xFF, db = d & 0xFF;
                nr = (sr * a + dr * ia) / 255;
                ng = (sg * a + dg * ia) / 255;
                nb = (sb * a + db * ia) / 255;
            }
            scrRow[x] = 0xFF000000 | (nr << 16) | (ng << 8) | nb;
        }
    }
}

void DrawSpriteStretch(IMAGE* src, int dx, int dy, int dw, int dh)
{
    if (!src) return;
    int sw = src->getwidth(), sh = src->getheight();
    if (sw <= 0 || sh <= 0 || dw <= 0 || dh <= 0) return;
    DWORD* srcBuf = GetImageBuffer(src);
    DWORD* scrBuf = GetImageBuffer(NULL);
    if (!srcBuf || !scrBuf) return;
    int x0 = 0, y0 = 0, x1 = dw, y1 = dh;
    if (dx + x1 <= 0 || dx + x0 >= WIN_W) return;
    if (dy + y1 <= 0 || dy + y0 >= WIN_H) return;
    if (dx + x0 < 0) x0 = -dx;
    if (dy + y0 < 0) y0 = -dy;
    if (dx + x1 > WIN_W) x1 = WIN_W - dx;
    if (dy + y1 > WIN_H) y1 = WIN_H - dy;
    float gA = g_globalAlpha;
    for (int y = y0; y < y1; y++)
    {
        int iy = (int)((y * (long long)sh) / dh);
        if (iy < 0) iy = 0; if (iy >= sh) iy = sh - 1;
        DWORD* srcRow = srcBuf + iy * sw;
        DWORD* scrRow = scrBuf + (dy + y) * WIN_W + dx;
        for (int x = x0; x < x1; x++)
        {
            int ix = (int)((x * (long long)sw) / dw);
            if (ix < 0) ix = 0; if (ix >= sw) ix = sw - 1;
            DWORD c = srcRow[ix];
            int a = (c >> 24) & 0xFF;
#if PIXEL_ART_MODE
            if (a < ALPHA_CUTOFF) continue;
#endif
            if (a == 0) continue;
            if (gA < 255.0f)
            {
                int fa = (int)(a * gA / 255.0f);
                if (fa <= 0) continue;
                if (fa >= 255) scrRow[x] = (c & 0x00FFFFFF) | 0xFF000000;
                else
                {
                    DWORD d = scrRow[x];
                    int sr = (c >> 16) & 0xFF, sg = (c >> 8) & 0xFF, sb = c & 0xFF;
                    int dr = (d >> 16) & 0xFF, dg = (d >> 8) & 0xFF, db = d & 0xFF;
                    int ia = 255 - fa;
                    scrRow[x] = 0xFF000000 | (((sr * fa + dr * ia) / 255) << 16)
                              | (((sg * fa + dg * ia) / 255) << 8) | ((sb * fa + db * ia) / 255);
                }
            }
            else scrRow[x] = (c & 0x00FFFFFF) | 0xFF000000;
        }
    }
}

void DrawWrappedCN(const char* text, int x, int y, int maxW, int lineH)
{
    int len = (int)strlen(text);
    char line[512]; line[0] = 0;
    int lineLen = 0, lineY = y, i = 0;
    while (i < len)
    {
        unsigned char c = (unsigned char)text[i];
        int cb = 1;
        if (c >= 0x81 && c <= 0xFE && i + 1 < len) cb = 2;
        char temp[512];
        memcpy(temp, line, lineLen);
        for (int k = 0; k < cb && i + k < len; k++) temp[lineLen + k] = text[i + k];
        temp[lineLen + cb] = 0;
        if (textwidth(temp) > maxW && lineLen > 0)
        { outtextxy(x, lineY, line); lineY += lineH; line[0] = 0; lineLen = 0; }
        else { memcpy(line, temp, lineLen + cb + 1); lineLen += cb; i += cb; }
    }
    if (lineLen > 0) outtextxy(x, lineY, line);
}

bool PointInTri(float px, float py, float x1, float y1, float x2, float y2, float x3, float y3)
{
    float d1 = (px - x2) * (y1 - y2) - (x1 - x2) * (py - y2);
    float d2 = (px - x3) * (y2 - y3) - (x2 - x3) * (py - y3);
    float d3 = (px - x1) * (y3 - y1) - (x3 - x1) * (py - y1);
    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(has_neg && has_pos);
}

void DrawTriangleAlpha(float cx, float cy, float angle, float size, int alpha, int r, int g, int b)
{
    if (alpha <= 0) return;
    float tipX = cx + cosf(angle) * size, tipY = cy + sinf(angle) * size;
    float a1 = angle + PI * 0.75f, a2 = angle - PI * 0.75f;
    float b1X = cx + cosf(a1) * size * 0.85f, b1Y = cy + sinf(a1) * size * 0.85f;
    float b2X = cx + cosf(a2) * size * 0.85f, b2Y = cy + sinf(a2) * size * 0.85f;
    float xMin = fminf(fminf(tipX, b1X), b2X), xMax = fmaxf(fmaxf(tipX, b1X), b2X);
    float yMin = fminf(fminf(tipY, b1Y), b2Y), yMax = fmaxf(fmaxf(tipY, b1Y), b2Y);
    int x0 = (int)floorf(xMin) - 1, y0 = (int)floorf(yMin) - 1;
    int x1 = (int)ceilf(xMax) + 1, y1 = (int)ceilf(yMax) + 1;
    if (x0 < 0) x0 = 0; if (y0 < 0) y0 = 0;
    if (x1 > WIN_W) x1 = WIN_W; if (y1 > WIN_H) y1 = WIN_H;
    if (x0 >= x1 || y0 >= y1) return;
    DWORD* buf = GetImageBuffer(NULL);
    if (!buf) return;
    int ia = 255 - alpha;
    for (int yy = y0; yy < y1; yy++)
    {
        DWORD* row = buf + yy * WIN_W;
        for (int xx = x0; xx < x1; xx++)
        {
            if (!PointInTri(xx + 0.5f, yy + 0.5f, tipX, tipY, b1X, b1Y, b2X, b2Y)) continue;
            DWORD c = row[xx];
            int cr = (c >> 16) & 0xFF, cg = (c >> 8) & 0xFF, cb = c & 0xFF;
            row[xx] = 0xFF000000 | (((r * alpha + cr * ia) / 255) << 16)
                    | (((g * alpha + cg * ia) / 255) << 8) | ((b * alpha + cb * ia) / 255);
        }
    }
}
float CalcStatRatio(int tIdx, int statIdx)
{
    if (tIdx < 0 || tIdx >= g_tankCount) return 0.0f;
    float r = 0.0f;
    if (statIdx == 0) {
        float cd = g_tankStats[tIdx][0];
        if (cd < STAT_FIRE_MIN) cd = STAT_FIRE_MIN;
        if (cd > STAT_FIRE_MAX) cd = STAT_FIRE_MAX;
        r = (STAT_FIRE_MAX - cd) / (STAT_FIRE_MAX - STAT_FIRE_MIN);
    } else if (statIdx == 1) {
        float v = g_tankStats[tIdx][1];
        if (v < STAT_MOVE_MIN) v = STAT_MOVE_MIN;
        if (v > STAT_MOVE_MAX) v = STAT_MOVE_MAX;
        r = (v - STAT_MOVE_MIN) / (STAT_MOVE_MAX - STAT_MOVE_MIN);
    } else if (statIdx == 2) {
        float v = g_tankStats[tIdx][3];
        if (v < STAT_DMG_MIN) v = STAT_DMG_MIN;
        if (v > STAT_DMG_MAX) v = STAT_DMG_MAX;
        r = (v - STAT_DMG_MIN) / (STAT_DMG_MAX - STAT_DMG_MIN);
    } else {
        float v = g_tankStats[tIdx][5];
        if (v < STAT_HP_MIN) v = STAT_HP_MIN;
        if (v > STAT_HP_MAX) v = STAT_HP_MAX;
        r = (v - STAT_HP_MIN) / (STAT_HP_MAX - STAT_HP_MIN);
    }
    if (r < 0.0f) r = 0.0f;
    if (r > 1.0f) r = 1.0f;
    return r;
}

void LoadTankStats()
{
    for (int i = 0; i < MAX_TANKS; i++)
    {
        g_tankStats[i][0] = DEF_FIRE_CD; g_tankStats[i][1] = DEF_MOVE_SPD;
        g_tankStats[i][2] = DEF_TURN_SPD; g_tankStats[i][3] = DEF_DAMAGE;
        g_tankStats[i][4] = DEF_BULLET_SPD; g_tankStats[i][5] = DEF_HP;
    }
    FILE* fp = fopen("tank/tankshuzhi.txt", "r");
    if (!fp) return;
    char line[256];
    int i = 0;
    while (i < MAX_TANKS && fgets(line, sizeof(line), fp))
    {
        float v[6];
        v[0] = DEF_FIRE_CD; v[1] = DEF_MOVE_SPD; v[2] = DEF_TURN_SPD;
        v[3] = DEF_DAMAGE; v[4] = DEF_BULLET_SPD; v[5] = DEF_HP;
        int n = sscanf(line, "%f %f %f %f %f %f",
                       &v[0], &v[1], &v[2], &v[3], &v[4], &v[5]);
        if (n < 5) continue;
        for (int j = 0; j < 6; j++) g_tankStats[i][j] = v[j];
        i++;
    }
    fclose(fp);
}

void LoadResources()
{
    loadimage(&g_bg, "ui/zhuye.png");
    loadimage(&g_helpImg, "ui/zhuye4.png");
    loadimage(&g_gousuoTex, "tank/jinengpng/gousuo.png");
    loadimage(&g_qiangtiTex, "tank/jinengpng/qiangti.png");
    g_mapCount = MAP_NAMES_COUNT;
    if (g_mapCount > MAX_MAPS) g_mapCount = MAX_MAPS;
    for (int i = 0; i < g_mapCount; i++)
    {
        char path[128];
        sprintf(path, "map/qiang%d.png", i + 1); loadimage(&g_mapWall[i], path);
        sprintf(path, "map/kongdi%d.png", i + 1); loadimage(&g_mapFloor[i], path);
    }
    g_tankCount = TANK_NAMES_COUNT;
    if (g_tankCount > MAX_TANKS) g_tankCount = MAX_TANKS;
    for (int i = 0; i < g_tankCount; i++)
    {
        char path[160];
        sprintf(path, "tank/tankpng/tank%d.png", i + 1);
        loadimage(&g_tankTex[i], path);
        sprintf(path, "tank/huoguangpng/huoguang%d.png", i + 1);
        loadimage(&g_tankMuzzle[i], path);
        sprintf(path, "tank/paodanpng/paodan%d.png", i + 1);
        loadimage(&g_tankBullet[i], path);
    }
    for (int i = 0; i < 4; i++)
    {
        char path[128];
        sprintf(path, "map/buff%d.png", i + 1);
        loadimage(&g_buffTex[i], path);
    }
    LoadTankStats();
}

int FindPath(int startR, int startC, int goalR, int goalC,
             int* outPathR, int* outPathC, int maxLen)
{
    if (startR < 0 || startC < 0 || startR >= MAP_H || startC >= MAP_W) return 0;
    if (goalR  < 0 || goalC  < 0 || goalR  >= MAP_H || goalC  >= MAP_W) return 0;
    if (g_map[startR][startC] == 1) return 0;
    if (g_map[goalR][goalC] == 1) return 0;
    if (startR == goalR && startC == goalC) return 0;
    int gScore[MAP_H][MAP_W], parentR[MAP_H][MAP_W], parentC[MAP_H][MAP_W];
    bool closed[MAP_H][MAP_W], open[MAP_H][MAP_W];
    for (int r = 0; r < MAP_H; r++)
        for (int c = 0; c < MAP_W; c++)
        { gScore[r][c] = 9999; parentR[r][c] = -1; parentC[r][c] = -1;
          closed[r][c] = false; open[r][c] = false; }
    gScore[startR][startC] = 0; open[startR][startC] = true;
    bool found = false;
    for (int iter = 0; iter < MAP_W * MAP_H; iter++)
    {
        int bestR = -1, bestC = -1, bestF = 99999, bestTie = 99999;
        for (int r = 0; r < MAP_H; r++)
            for (int c = 0; c < MAP_W; c++)
            {
                if (!open[r][c]) continue;
                int h = abs(r - goalR) + abs(c - goalC);
                int f = gScore[r][c] + h;
                int tie = (r * 3 + c * 7) & 7;
                if (f < bestF || (f == bestF && tie < bestTie))
                { bestF = f; bestTie = tie; bestR = r; bestC = c; }
            }
        if (bestR == -1) break;
        if (bestR == goalR && bestC == goalC) { found = true; break; }
        open[bestR][bestC] = false; closed[bestR][bestC] = true;
        int dr[4] = { -1, 1, 0, 0 }, dc[4] = { 0, 0, -1, 1 };
        for (int k = 0; k < 4; k++)
        {
            int nr = bestR + dr[k], nc = bestC + dc[k];
            if (nr < 0 || nc < 0 || nr >= MAP_H || nc >= MAP_W) continue;
            if (g_map[nr][nc] == 1) continue;
            if (closed[nr][nc]) continue;
            int ng = gScore[bestR][bestC] + 1;
            if (ng < gScore[nr][nc])
            { gScore[nr][nc] = ng; parentR[nr][nc] = bestR; parentC[nr][nc] = bestC; open[nr][nc] = true; }
        }
    }
    if (!found) return 0;
    int len = 0, r = goalR, c = goalC;
    while (!(r == startR && c == startC))
    {
        if (len >= maxLen) return 0;
        outPathR[len] = r; outPathC[len] = c; len++;
        int pr = parentR[r][c], pc = parentC[r][c];
        if (pr == -1) return 0;
        r = pr; c = pc;
    }
    for (int i = 0; i < len / 2; i++)
    {
        int tr = outPathR[i]; outPathR[i] = outPathR[len-1-i]; outPathR[len-1-i] = tr;
        int tc = outPathC[i]; outPathC[i] = outPathC[len-1-i]; outPathC[len-1-i] = tc;
    }
    return len;
}

void SpawnAfterImage(int tankIdx, float x, float y, float angle, float duration)
{
    for (int i = 0; i < MAX_AFTERIMG; i++)
    {
        if (!g_afterImgs[i].active)
        {
            g_afterImgs[i].active = true; g_afterImgs[i].timer = 0.0f;
            g_afterImgs[i].duration = duration;
            g_afterImgs[i].x = x; g_afterImgs[i].y = y;
            g_afterImgs[i].angle = angle; g_afterImgs[i].tankIdx = tankIdx;
            return;
        }
    }
    g_afterImgs[0].active = true; g_afterImgs[0].timer = 0.0f;
    g_afterImgs[0].duration = duration;
    g_afterImgs[0].x = x; g_afterImgs[0].y = y;
    g_afterImgs[0].angle = angle; g_afterImgs[0].tankIdx = tankIdx;
}

void UpdateAfterImages()
{
    for (int i = 0; i < MAX_AFTERIMG; i++)
    {
        if (!g_afterImgs[i].active) continue;
        g_afterImgs[i].timer += g_dt;
        if (g_afterImgs[i].timer >= g_afterImgs[i].duration)
            g_afterImgs[i].active = false;
    }
}

void DrawAfterImages()
{
    for (int i = 0; i < MAX_AFTERIMG; i++)
    {
        if (!g_afterImgs[i].active) continue;
        float t = g_afterImgs[i].timer / g_afterImgs[i].duration;
        if (t > 1.0f) t = 1.0f;
        int alpha = (int)(200.0f * (1.0f - t));
        if (alpha <= 0) continue;
        int tidx = g_afterImgs[i].tankIdx;
        if (tidx < 0 || tidx >= g_tankCount) continue;
        float sx = WorldToScreenX(g_afterImgs[i].x);
        float sy = WorldToScreenY(g_afterImgs[i].y);
        if (sx < -300 || sx > WIN_W + 300 || sy < -300 || sy > WIN_H + 300) continue;
        int tw = g_tankTex[tidx].getwidth();
        float sc = (tw > 0) ? (TANK_SIZE * g_camScale / (float)tw) : 1.0f;
        g_globalAlpha = (float)alpha;
        DrawSprite(&g_tankTex[tidx], sx, sy, sc, g_afterImgs[i].angle);
        g_globalAlpha = 255.0f;
    }
}

float GetEmpRadius(const Tank& t)
{
    if (t.empTimer < 0.0f) return 0.0f;
    if (t.empTimer < EMP_EXPAND_TIME) return EMP_RADIUS * (t.empTimer / EMP_EXPAND_TIME);
    float holdEnd = EMP_EXPAND_TIME + EMP_HOLD_TIME;
    if (t.empTimer < holdEnd) return EMP_RADIUS;
    float shrinkEnd = holdEnd + EMP_SHRINK_TIME;
    if (t.empTimer < shrinkEnd) return EMP_RADIUS * (1.0f - (t.empTimer - holdEnd) / EMP_SHRINK_TIME);
    return 0.0f;
}

bool IsTankSilenced(int idx)
{
    for (int i = 0; i < g_playerCount; i++)
    {
        if (i == idx) continue;
        if (g_tank[i].hp <= 0) continue;
        if (g_selTankIdx[i] != 7) continue;
        if (g_tank[i].empTimer < 0.0f) continue;
        float r = GetEmpRadius(g_tank[i]);
        if (r <= 1.0f) continue;
        float dx = g_tank[idx].x - g_tank[i].x;
        float dy = g_tank[idx].y - g_tank[i].y;
        if (dx * dx + dy * dy < r * r) return true;
    }
    return false;
}

float GetFrostRadius(const FrostField& f)
{
    if (!f.active) return 0.0f;
    if (f.timer < FROST_EXPAND) return FROST_RADIUS * (f.timer / FROST_EXPAND);
    float holdEnd = FROST_EXPAND + FROST_HOLD;
    if (f.timer < holdEnd) return FROST_RADIUS;
    float shrinkEnd = holdEnd + FROST_SHRINK;
    if (f.timer < shrinkEnd) return FROST_RADIUS * (1.0f - (f.timer - holdEnd) / FROST_SHRINK);
    return 0.0f;
}

float GetHunterRingRadius(const HunterRing& r)
{
    if (!r.active) return 0.0f;
    if (r.timer < HUNTER_EXPAND) return HUNTER_RING_MAX * (r.timer / HUNTER_EXPAND);
    float fadeT = (r.timer - HUNTER_EXPAND) / HUNTER_FADE;
    if (fadeT > 1.0f) fadeT = 1.0f;
    return HUNTER_RING_MAX * (1.0f + fadeT);
}

int GetHunterRingAlpha(const HunterRing& r)
{
    if (!r.active) return 0;
    if (r.timer < HUNTER_EXPAND) return HUNTER_RING_ALPHA;
    float t = (r.timer - HUNTER_EXPAND) / HUNTER_FADE;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return (int)(HUNTER_RING_ALPHA * (1.0f - t));
}

void InterruptSkillByEmp(int victimIdx)
{
    Tank& v = g_tank[victimIdx];
    v.skillActive = false;
    v.shieldActive = false;
    v.frenzyTimer = 0.0f;
    if (v.hookState != 0) { v.hookState = 0; v.hookTarget = -1; }
    if (v.phaseTimer > 0.0f)
    {
        v.phaseTimer = 0.0f;
        v.phaseAlpha = 0.0f;
        if (CollideWithWall(v.x, v.y)) v.phaseStuckInWall = true;
        else { v.inWall = false; v.phaseStuckDmg = 0.0f; }
    }
}

void UpdateAITank(int idx)
{
    Tank& t = g_tank[idx];
    int tidx = g_selTankIdx[idx];
    g_ignoreVWallOwner = idx;

    if (t.hp <= 0)
    {
        if (!t.deathSoundPlayed) { t.deathSoundPlayed = true; PlaySFX(SND_KILL); }
        t.deathFade += g_dt * 0.6f;
        if (t.deathFade > 1.0f) t.deathFade = 1.0f;
        return;
    }

    int enemyIdx = -1; float bestDist = 1e9f;
    for (int i = 0; i < g_playerCount; i++)
    {
        if (i == idx) continue;
        if (g_tank[i].hp <= 0) continue;
        float dx = g_tank[i].x - t.x, dy = g_tank[i].y - t.y;
        float d = sqrtf(dx * dx + dy * dy);
        if (d < bestDist) { bestDist = d; enemyIdx = i; }
    }
    if (enemyIdx < 0) return;
    Tank& e = g_tank[enemyIdx];

    bool silenced = IsTankSilenced(idx);

    float fireCd = g_tankStats[tidx][0];
    float moveSpd = g_tankStats[tidx][1];
    float turnSpd = g_tankStats[tidx][2];

    if (t.cooldown > 0.0f) t.cooldown -= g_dt;
    if (t.muzzleTimer > 0.0f) t.muzzleTimer -= g_dt;
    if (t.hitFlashTimer > 0.0f) t.hitFlashTimer -= g_dt;
    if (t.skillFlashTimer > 0.0f) t.skillFlashTimer -= g_dt;
    if (t.speedBoostTimer > 0.0f) t.speedBoostTimer -= g_dt;
    if (t.frenzyTimer > 0.0f) t.frenzyTimer -= g_dt;
    if (t.frenzyCd > 0.0f) t.frenzyCd -= g_dt;

    if (t.stunTimer > 0.0f) { t.stunTimer -= g_dt; return; }

    if (t.knockbackActive)
    {
        t.knockbackTimer += g_dt;
        float tt = t.knockbackTimer / t.knockbackDuration;
        if (tt > 1.0f) tt = 1.0f;
        float eased = tt * tt * (3.0f - 2.0f * tt);
        t.x = t.knockbackStartX + (t.knockbackTargetX - t.knockbackStartX) * eased;
        t.y = t.knockbackStartY + (t.knockbackTargetY - t.knockbackStartY) * eased;
        if (tt >= 1.0f)
        { t.knockbackActive = false; if (t.knockbackHitWall) t.stunTimer = SKILL3_STUN_TIME; }
        return;
    }
    if (t.dashActive)
    {
        t.dashTimer += g_dt;
        float tt = t.dashTimer / t.dashDuration;
        if (tt > 1.0f) tt = 1.0f;
        float eased = tt * tt * (3.0f - 2.0f * tt);
        t.x = t.dashStartX + (t.dashTargetX - t.dashStartX) * eased;
        t.y = t.dashStartY + (t.dashTargetY - t.dashStartY) * eased;
        if (tt >= 1.0f) t.dashActive = false;
        return;
    }

    if (tidx == 4 && t.hookState != 0)
    {
        if (t.hookState == 2)
        {
            float hdx = t.hookX - t.x, hdy = t.hookY - t.y;
            float hlen = sqrtf(hdx * hdx + hdy * hdy);
            if (hlen < 5.0f) t.hookState = 0;
            else
            {
                float hnx = hdx / hlen, hny = hdy / hlen;
                float hstep = HOOK_PULL_SELF_SPD * g_dt;
                if (hstep > hlen) hstep = hlen;
                float htx = t.x + hnx * hstep, hty = t.y + hny * hstep;
                if (!CollideWithWall(htx, hty)) { t.x = htx; t.y = hty; }
                else t.hookState = 0;
            }
        }
        else if (t.hookState == 1)
        {
            float hstep = HOOK_FLY_SPEED * g_dt;
            t.hookDist += hstep;
            t.hookX = t.hookStartX + cosf(t.hookAngle) * t.hookDist;
            t.hookY = t.hookStartY + sinf(t.hookAngle) * t.hookDist;
            int cc = (int)floorf(t.hookX / TILE);
            int rr = (int)floorf(t.hookY / TILE);
            if (cc < 0 || rr < 0 || cc >= MAP_W || rr >= MAP_H || g_map[rr][cc] == 1)
            { t.hookState = 2; PlaySFX(SND_JN5_1); }
            else
            {
                for (int j = 0; j < g_playerCount; j++)
                {
                    if (j == idx) continue;
                    if (g_tank[j].hp <= 0) continue;
                    float ddx = t.hookX - g_tank[j].x, ddy = t.hookY - g_tank[j].y;
                    if (ddx * ddx + ddy * ddy < (TANK_SIZE * 0.5f) * (TANK_SIZE * 0.5f))
                    { t.hookState = 3; t.hookTarget = j; PlaySFX(SND_JN5_1); break; }
                }
            }
            if (t.hookState == 1 && t.hookDist > HOOK_RANGE)
            { t.hookState = 4; t.hookDist = HOOK_RANGE; }
        }
        else if (t.hookState == 4)
        {
            float hstep = HOOK_FLY_SPEED * g_dt;
            t.hookDist -= hstep;
            if (t.hookDist < 0.0f) t.hookDist = 0.0f;
            t.hookX = t.hookStartX + cosf(t.hookAngle) * t.hookDist;
            t.hookY = t.hookStartY + sinf(t.hookAngle) * t.hookDist;
            if (t.hookDist <= 0.0f) t.hookState = 0;
        }
        else if (t.hookState == 3)
        {
            int j = t.hookTarget;
            if (j < 0 || j >= g_playerCount || g_tank[j].hp <= 0) { t.hookState = 0; t.hookTarget = -1; }
            else
            {
                Tank& enemy = g_tank[j];
                float dx = t.x - enemy.x, dy = t.y - enemy.y;
                float len = sqrtf(dx * dx + dy * dy);
                if (len <= HOOK_RELEASE_DIST) { t.hookState = 0; t.hookTarget = -1; }
                else
                {
                    float nx = dx / len, ny = dy / len;
                    float step = HOOK_PULL_ENEMY_SPD * g_dt;
                    if (step > len - HOOK_RELEASE_DIST) step = len - HOOK_RELEASE_DIST;
                    if (step > len) step = len;
                    float tx = enemy.x + nx * step, ty = enemy.y + ny * step;
                    float moved = 0.0f;
                    if (!CollideWithWall(tx, ty))
                    { moved = step; enemy.x = tx; enemy.y = ty; t.hookX = enemy.x; t.hookY = enemy.y; }
                    else { t.hookState = 0; t.hookTarget = -1; }
                    enemy.dmgAccum += moved;
                    while (enemy.dmgAccum >= TILE)
                    {
                        enemy.dmgAccum -= TILE;
                        enemy.hp -= HOOK_ENEMY_DMG_PER_TILE;
                        if (enemy.hp < 0) enemy.hp = 0;
                        enemy.skillFlashTimer = SKILL_FLASH_TIME;
                    }
                }
            }
        }
    }

    float dx = e.x - t.x, dy = e.y - t.y;
    float dist = sqrtf(dx * dx + dy * dy);
    if (dist < 0.001f) dist = 0.001f;
    float targetAngle = atan2f(dy, dx);
    float angleDiff = targetAngle - t.bodyAngle;
    while (angleDiff > PI) angleDiff -= 2 * PI;
    while (angleDiff < -PI) angleDiff += 2 * PI;
    bool los = LineOfSight(t.x, t.y, e.x, e.y);

    if (tidx == 0)
    {
        if (!silenced && !t.skillActive && dist > 3.5f * TILE && t.skillCd >= SKILL1_ACTIVATE_TH)
        { t.skillActive = true; t.afterimageSpawnTimer = 0.0f; }
        if (t.skillActive && dist < 2.0f * TILE) t.skillActive = false;
        if (t.skillActive)
        {
            t.skillCd -= SKILL1_DRAIN_RATE * g_dt;
            if (t.skillCd <= 0.0f) { t.skillCd = 0.0f; t.skillActive = false; }
            t.afterimageSpawnTimer += g_dt;
            if (t.afterimageSpawnTimer >= AFTERIMG_INTERVAL_YOUXIA)
            { SpawnAfterImage(tidx, t.x, t.y, t.bodyAngle, AFTERIMG_DUR_YOUXIA); t.afterimageSpawnTimer = 0.0f; }
        }
        else { if (t.skillCd < 1.0f) { t.skillCd += SKILL1_RECOVER_RATE * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; } }
    }
    else if (tidx == 2)
    {
        if (!silenced && dist < 1.8f * TILE && t.skillCd >= 1.0f)
        { CastKnockback(idx); t.skillCd = 0.0f; PlaySFX(SND_JN3); }
        else { if (t.skillCd < 1.0f) { t.skillCd += SKILL3_RECOVER_RATE * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; } }
    }
    else if (tidx == 3)
    {
        bool danger = false;
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (!g_bullets[i].alive) continue;
            if (g_bullets[i].owner == idx) continue;
            float bx = g_bullets[i].x - t.x, by = g_bullets[i].y - t.y;
            float bdist = sqrtf(bx * bx + by * by);
            if (bdist > 4.0f * TILE) continue;
            float vlen = sqrtf(g_bullets[i].vx * g_bullets[i].vx + g_bullets[i].vy * g_bullets[i].vy);
            if (vlen < 0.01f) continue;
            float dot = (g_bullets[i].vx * bx + g_bullets[i].vy * by) / (vlen * bdist);
            if (dot > 0.85f) { danger = true; break; }
        }
        if (silenced) { }
        else if (t.shieldActive) { bool sp = !danger; UpdateShieldLogic(t, sp); }
        else { bool sp = danger; UpdateShieldLogic(t, sp); }
    }
    else if (tidx == 4)
    {
        if (!silenced && t.hookState == 0 && t.skillCd >= 1.0f && los && dist < 4.5f * TILE)
        {
            t.hookState = 1;
            t.hookStartX = t.x; t.hookStartY = t.y;
            t.hookX = t.x; t.hookY = t.y;
            t.hookAngle = t.bodyAngle;
            t.hookDist = 0.0f;
            t.skillCd = 0.0f;
            PlaySFX(SND_JN5_2);
        }
        else if (t.hookState == 0) { if (t.skillCd < 1.0f) { t.skillCd += HOOK_CD * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; } }
    }
    else if (tidx == 5)
    {
        if (t.overheated)
        {
            t.heat -= HEAT_RATE_DOWN * g_dt;
            if (t.heat < 0.0f) t.heat = 0.0f;
            if (t.heat <= HEAT_RECOVER_TH) t.overheated = false;
        }
        else
        {
            float rate = (t.frenzyTimer > 0.0f) ? FRENZY_HEAT_RATE : HEAT_RATE_UP;
            t.heat += rate * g_dt;
            if (t.heat >= HEAT_OVERHEAT)
            { t.heat = HEAT_OVERHEAT; t.overheated = true; PlaySFX(SND_JN6); }
        }
        if (!silenced && t.frenzyCd <= 0.0f && los) { t.frenzyTimer = FRENZY_DURATION; t.frenzyCd = FRENZY_CD; }
    }
    else if (tidx == 7)
    {
        if (!silenced && t.empTimer < 0.0f && t.skillCd >= 1.0f && los && dist < 4.0f * TILE)
        { t.empTimer = 0.0f; t.skillCd = 0.0f; PlaySFX(SND_JN8); }
        else
        {
            if (t.skillCd < 1.0f)
            { t.skillCd += (1.0f / EMP_CD) * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; }
        }
    }
    else if (tidx == 8)
    {
        if (!silenced && t.skillCd >= BEDROCK_PLACE_COST && dist < 5.0f * TILE)
        {
            if (PlaceVirtualWall(idx))
            {
                t.skillCd -= BEDROCK_PLACE_COST;
                if (t.skillCd < 0.0f) t.skillCd = 0.0f;
                PlaySFX(SND_JN9);
            }
        }
        if (t.skillCd < 1.0f)
        {
            t.skillCd += BEDROCK_RECOVER_RATE * g_dt;
            if (t.skillCd > 1.0f) t.skillCd = 1.0f;
        }
    }
    else if (tidx == 9)
    {
        if (!silenced && t.skillCd >= 1.0f && dist < 4.5f * TILE)
        {
            g_frostFields[idx].active = true;
            g_frostFields[idx].timer = 0.0f;
            g_frostFields[idx].x = t.x;
            g_frostFields[idx].y = t.y;
            t.skillCd = 0.0f;
            PlaySFX(SND_JN10);
        }
        else
        {
            if (t.skillCd < 1.0f)
            { t.skillCd += (1.0f / FROST_CD) * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; }
        }
    }
    else if (tidx == 10)
    {
        if (!silenced && t.skillCd >= 1.0f && dist < 5.0f * TILE)
        {
            g_hunterRings[idx].active = true;
            g_hunterRings[idx].timer = 0.0f;
            g_hunterRings[idx].x = t.x;
            g_hunterRings[idx].y = t.y;
            for (int j = 0; j < MAX_PLAYERS; j++) g_hunterRings[idx].hit[j] = false;
            t.skillCd = 0.0f;
            PlaySFX(SND_JN11);
        }
        else
        {
            if (t.skillCd < 1.0f)
            { t.skillCd += (1.0f / HUNTER_CD) * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; }
        }
    }

    float targetVel = 0.0f;
    float desiredAngle = t.bodyAngle;
    if (los)
    {
        desiredAngle = targetAngle;
        if (dist > TANK_SIZE * 3.0f) targetVel = moveSpd;
        else if (dist < TANK_SIZE * 1.2f) targetVel = -moveSpd * 0.5f;
        else targetVel = 0.0f;
        g_aiPathLen[idx] = 0;
    }
    else
    {
        g_aiPathTimer[idx] += g_dt;
        int rawR = (int)floorf(t.y / TILE), rawC = (int)floorf(t.x / TILE);
        if (rawR < 0) rawR = 0; if (rawC < 0) rawC = 0;
        if (rawR >= MAP_H) rawR = MAP_H - 1;
        if (rawC >= MAP_W) rawC = MAP_W - 1;
        int curR = rawR, curC = rawC;
        int stickyR = g_aiCellR[idx], stickyC = g_aiCellC[idx];
        if (stickyR >= 0 && stickyR < MAP_H && stickyC >= 0 && stickyC < MAP_W && g_map[stickyR][stickyC] != 1)
        {
            float ctrX = stickyC * TILE + TILE * 0.5f, ctrY = stickyR * TILE + TILE * 0.5f;
            float ddx = t.x - ctrX, ddy = t.y - ctrY;
            if (ddx * ddx + ddy * ddy < (TILE * 0.95f) * (TILE * 0.95f)) { curR = stickyR; curC = stickyC; }
        }
        g_aiCellR[idx] = curR; g_aiCellC[idx] = curC;
        int enemyR = (int)floorf(e.y / TILE), enemyC = (int)floorf(e.x / TILE);
        if (enemyR < 0) enemyR = 0; if (enemyC < 0) enemyC = 0;
        if (enemyR >= MAP_H) enemyR = MAP_H - 1;
        if (enemyC >= MAP_W) enemyC = MAP_W - 1;
        if (g_map[enemyR][enemyC] == 1)
        {
            int dr[8] = {-1,-1,-1,0,0,1,1,1}, dc[8] = {-1,0,1,-1,1,-1,0,1};
            bool ok = false;
            for (int k2 = 0; k2 < 8; k2++)
            {
                int nr = enemyR + dr[k2], nc = enemyC + dc[k2];
                if (nr >= 0 && nc >= 0 && nr < MAP_H && nc < MAP_W && g_map[nr][nc] != 1)
                { enemyR = nr; enemyC = nc; ok = true; break; }
            }
            if (!ok) { enemyR = curR; enemyC = curC; }
        }
        bool needRecompute = false;
        if (g_aiPathTimer[idx] > AI_RECOMPUTE_INTERVAL) needRecompute = true;
        if (g_aiPathLen[idx] == 0 && g_aiPathTimer[idx] > 0.15f) needRecompute = true;
        if (g_aiPathLen[idx] > 0)
        {
            int lastR = g_aiPathR[idx][g_aiPathLen[idx]-1];
            int lastC = g_aiPathC[idx][g_aiPathLen[idx]-1];
            float lx = lastC * TILE + TILE * 0.5f, ly = lastR * TILE + TILE * 0.5f;
            float ddx = lx - t.x, ddy = ly - t.y;
            if (ddx * ddx + ddy * ddy < 25.0f) needRecompute = true;
        }
        if (needRecompute)
        {
            g_aiPathLen[idx] = FindPath(curR, curC, enemyR, enemyC, g_aiPathR[idx], g_aiPathC[idx], MAX_PATH_LEN);
            g_aiPathTimer[idx] = 0.0f;
        }
        int bestJ = -1;
        for (int j = g_aiPathLen[idx] - 1; j >= 0; j--)
        {
            float gx = g_aiPathC[idx][j] * TILE + TILE * 0.5f;
            float gy = g_aiPathR[idx][j] * TILE + TILE * 0.5f;
            if (LineOfSight(t.x, t.y, gx, gy)) { bestJ = j; break; }
        }
        if (bestJ >= 0)
        {
            float gx = g_aiPathC[idx][bestJ] * TILE + TILE * 0.5f;
            float gy = g_aiPathR[idx][bestJ] * TILE + TILE * 0.5f;
            float dxT = gx - t.x, dyT = gy - t.y;
            float dT = sqrtf(dxT * dxT + dyT * dyT);
            if (dT > 0.5f) { desiredAngle = atan2f(dyT, dxT); targetVel = moveSpd; }
        }
        else { desiredAngle = targetAngle; targetVel = moveSpd * 0.7f; }
    }
    bool inEscape = (g_aiEscapeTimer[idx] > 0.0f);
    if (inEscape)
    { g_aiEscapeTimer[idx] -= g_dt; desiredAngle = t.bodyAngle; targetVel = moveSpd; }
    float rawDiff = desiredAngle - t.bodyAngle;
    while (rawDiff > PI) rawDiff -= 2 * PI;
    while (rawDiff < -PI) rawDiff += 2 * PI;
    float maxTurn = turnSpd * g_dt, turnStep = rawDiff;
    if (turnStep > maxTurn) turnStep = maxTurn;
    if (turnStep < -maxTurn) turnStep = -maxTurn;
    t.bodyAngle += turnStep;
    float alignFactor = 1.0f - fabsf(rawDiff) / 1.2f;
    if (alignFactor < 0.15f) alignFactor = 0.15f;
    if (alignFactor > 1.0f) alignFactor = 1.0f;
    if (!inEscape)
    { if (fabsf(rawDiff) > 0.4f) targetVel = 0.0f; else targetVel *= alignFactor; }
    float k = 1.0f - expf(-TANK_MOVE_EASE * g_dt);
    t.vel += (targetVel - t.vel) * k;
    if (fabs(t.vel) < 1.5f && targetVel == 0.0f) t.vel = 0.0f;
    bool blockedBoth = false;
    if (fabs(t.vel) > 0.5f)
    {
        float speedMul = 1.0f;
        if (tidx == 0 && t.skillActive) speedMul = SKILL1_SPEED_MUL;
        if (t.speedBoostTimer > 0.0f) speedMul *= SKILL2_PASSIVE_MUL;
        if (t.buffType == BUFF_SPEED && t.buffTimer > 0.0f) speedMul *= BUFF_SPEED_MUL;
        speedMul *= t.slowFactor;
        float nx = t.x + cosf(t.bodyAngle) * t.vel * speedMul * g_dt;
        float ny = t.y + sinf(t.bodyAngle) * t.vel * speedMul * g_dt;
        bool okX = !CollideWithWall(nx, t.y);
        bool okY = !CollideWithWall(t.x, ny);
        if (okX) t.x = nx;
        if (okY) t.y = ny;
        if (!okX && !okY) { blockedBoth = true; t.vel = 0.0f; }
    }
    float movedDist = sqrtf((t.x - g_aiLastX[idx]) * (t.x - g_aiLastX[idx]) +
                            (t.y - g_aiLastY[idx]) * (t.y - g_aiLastY[idx]));
    if (targetVel > 0.1f && movedDist < 0.5f) g_aiStuckTimer[idx] += g_dt;
    else g_aiStuckTimer[idx] = 0.0f;
    g_aiLastX[idx] = t.x; g_aiLastY[idx] = t.y;
    if (!inEscape && (blockedBoth || g_aiStuckTimer[idx] > 0.35f))
    {
        const float offsets[8] = { 0.0f, PI*0.25f, -PI*0.25f, PI*0.5f, -PI*0.5f, PI*0.75f, -PI*0.75f, PI };
        bool found = false;
        for (int k2 = 0; k2 < 8; k2++)
        {
            float testAngle = t.bodyAngle + offsets[k2];
            bool ok = true;
            for (float d = 15.0f; d <= 45.0f; d += 15.0f)
            {
                float tx = t.x + cosf(testAngle) * d;
                float ty = t.y + sinf(testAngle) * d;
                if (CollideWithWall(tx, ty)) { ok = false; break; }
            }
            if (ok) { t.bodyAngle = testAngle; found = true; break; }
        }
        if (found)
        {
            g_aiEscapeTimer[idx] = AI_ESCAPE_LOCK;
            g_aiStuckTimer[idx] = 0.0f; g_aiPathTimer[idx] = 0.0f;
            g_aiPathLen[idx] = 0; t.vel = 0.0f;
            g_aiCellR[idx] = -1; g_aiCellC[idx] = -1;
        }
        else { t.vel = 0.0f; g_aiStuckTimer[idx] = 0.0f; }
    }
    if (los && fabs(angleDiff) < 0.18f && !silenced)
    {
        float r1 = (float)(rand() % 1000) / 1000.0f;
        float r2 = (float)(rand() % 1000) / 1000.0f;
        float aimNoise = (r1 + r2 - 1.0f) * AI_AIM_NOISE;
        if (t.cooldown <= 0.0f && tidx != 5)
        {
            Tank& tt = g_tank[idx];
            float a = tt.bodyAngle + aimNoise;
            float bulletSpd = g_tankStats[tidx][4];
            int dmg = (int)g_tankStats[tidx][3];
            if (tt.buffType == BUFF_DAMAGE && tt.buffTimer > 0.0f)
                dmg = (int)(dmg * BUFF_DAMAGE_MUL + 0.5f);
            int bType = (tidx == 6) ? BT_PIERCE : BT_NORMAL;
            float px = tt.x + cosf(a) * TANK_SIZE * 0.55f;
            float py = tt.y + sinf(a) * TANK_SIZE * 0.55f;
            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (!g_bullets[i].alive)
                {
                    g_bullets[i].x = px; g_bullets[i].y = py;
                    g_bullets[i].vx = cosf(a) * bulletSpd;
                    g_bullets[i].vy = sinf(a) * bulletSpd;
                    g_bullets[i].angle = a; g_bullets[i].alive = true;
                    g_bullets[i].owner = idx; g_bullets[i].damage = dmg;
                    g_bullets[i].type = bType;
                    g_bullets[i].lastCellC = (int)floorf(px / TILE);
                    g_bullets[i].lastCellR = (int)floorf(py / TILE);
                    break;
                }
            }
            float cd = fireCd;
            if (tt.buffType == BUFF_FIRE && tt.buffTimer > 0.0f) cd *= BUFF_FIRE_MUL;
            cd /= t.slowFactor;
            tt.cooldown = cd;
            tt.muzzleTimer = MUZZLE_TIME;
            if (tidx == 0) PlaySFX(SND_KP1);
            else if (tidx == 1) PlaySFX(SND_KP2);
            else if (tidx == 4) PlaySFX(SND_KP5);
            else if (tidx >= 2) PlaySFX(SND_KP3);
        }
    }
}

void InitMenu()
{
    const char* names[MENU_COUNT] = { "单人游玩", "多人死斗", "多人竞技", "坦克图鉴", "操作说明" };
    int cx = WIN_W / 2 + WIN_W / 4;
    int startY = 120, gap = 90;
    for (int i = 0; i < MENU_COUNT; i++)
    {
        strcpy(g_menu[i].text, names[i]);
        g_menu[i].cx = cx; g_menu[i].cy = startY + i * gap;
        g_menuSubIdx[i] = 0;
    }
}

void DrawMenuText()
{
    setbkmode(TRANSPARENT);
    for (int i = 0; i < MENU_COUNT; i++)
    {
        int fontH = (i == g_selected) ? (int)(BASE_FONT_H * SELECT_SCALE + 0.5f) : BASE_FONT_H;
        settextstyle(fontH, 0, FONT_NAME);
        char buf[64];
        if (i == g_selected) sprintf(buf, "> %s <", g_menu[i].text);
        else strcpy(buf, g_menu[i].text);
        settextcolor(WHITE);
        int tw = textwidth(buf), th = textheight(buf);
        outtextxy(g_menu[i].cx - tw / 2, g_menu[i].cy - th / 2, buf);
        if (i == g_selected && (i == 1 || i == 2))
        {
            int subBaseX = g_menu[i].cx - tw / 2 - 130;
            int subY = g_menu[i].cy;
            for (int s = 0; s < 2; s++)
            {
                bool sel = (g_menuSubIdx[i] == s);
                int fh = sel ? 32 : 24;
                settextstyle(fh, 0, FONT_NAME);
                settextcolor(WHITE);
                int stw = textwidth(MENU_SUB_NAMES[s]);
                int sth = textheight(MENU_SUB_NAMES[s]);
                int sx = subBaseX + s * 80;
                outtextxy(sx - stw / 2, subY - sth / 2, MENU_SUB_NAMES[s]);
            }
        }
    }
}

void StartTransition(GameState next)
{
    g_transPrev = g_state; g_transNext = next;
    g_state = ST_TRANS_IN; g_transTimer = 0.0f; ResetKeys();
}

void EnterMapSelect()
{
    g_mapSelCurIdx = g_selMapIdx; g_mapSelOldIdx = g_selMapIdx;
    g_mapSelAnimT = 1.0f; g_mapSelDir = 1;
    StartTransition(ST_MAP_SELECT);
}

void UpdateMenu()
{
    bool moved = false;
    if (KeyPressed(VK_UP) || KeyPressed('W'))
    { g_selected--; if (g_selected < 0) g_selected = MENU_COUNT - 1; moved = true; }
    if (KeyPressed(VK_DOWN) || KeyPressed('S'))
    { g_selected++; if (g_selected >= MENU_COUNT) g_selected = 0; moved = true; }
    if (g_selected == 1 || g_selected == 2)
    {
        if (KeyPressed(VK_LEFT) || KeyPressed('A'))
        { g_menuSubIdx[g_selected] = 0; PlaySFX(SND_HUADONG); }
        if (KeyPressed(VK_RIGHT) || KeyPressed('D'))
        { g_menuSubIdx[g_selected] = 1; PlaySFX(SND_HUADONG); }
    }
    if (moved) PlaySFX(SND_HUADONG);
    if (KeyPressed(VK_SPACE))
    {
        PlaySFX(SND_UI);
        if (g_selected == 0)
        {
            g_gameMode = GM_SINGLE;
            g_playerCount = 2;
            g_isSinglePlayer = true;
            EnterMapSelect();
        }
        else if (g_selected == 1 || g_selected == 2)
        {
            g_gameMode = (g_selected == 1) ? GM_DEATHMATCH : GM_COMPETITIVE;
            g_playerCount = (g_menuSubIdx[g_selected] == 0) ? 2 : 3;
            g_isSinglePlayer = false;
            EnterMapSelect();
        }
        else if (g_selected == 3)
        {
            g_tankInfoCurIdx = 0; g_tankInfoOldIdx = 0;
            g_tankInfoAnimT = 1.0f; g_tankInfoDir = 1;
            for (int i = 0; i < 4; i++) g_tankInfoBar[i] = CalcStatRatio(g_tankInfoCurIdx, i);
            StartTransition(ST_TANK_INFO);
        }
        else if (g_selected == 4) StartTransition(ST_HELP);
    }
}

void DrawMapCover(int idx, float scrCx, float scrCy, float scale)
{
    if (idx < 0 || idx >= g_mapCount) return;
    IMAGE* tex = &g_mapWall[idx];
    int sw = tex->getwidth(), sh = tex->getheight();
    if (sw <= 0 || sh <= 0) return;
    float baseSc = 250.0f / (float)(sw > sh ? sw : sh);
    DrawSprite(tex, scrCx, scrCy, baseSc * scale, 0.0f);
    int fs = (int)(28 * scale + 0.5f); if (fs < 8) fs = 8;
    settextstyle(fs, 0, FONT_NAME); setbkmode(TRANSPARENT); settextcolor(WHITE);
    const char* name = MAP_NAMES[idx];
    int tw = textwidth(name);
    outtextxy((int)scrCx - tw / 2, (int)(scrCy + 150 * scale), name);
}

void DrawMapSelect()
{
    setfillcolor(RGB(150, 150, 150)); solidrectangle(0, 0, WIN_W, WIN_H);
    int cx = WIN_W / 2, cy = 250;
    if (g_mapSelAnimT < 1.0f)
    {
        float t = g_mapSelAnimT, eased = 1.0f - (1.0f - t) * (1.0f - t);
        float oldX = cx - g_mapSelDir * eased * SELECT_SLIDE_DIST;
        float oldScale = 1.0f - eased * (1.0f - SELECT_MIN_SCALE);
        DrawMapCover(g_mapSelOldIdx, oldX, (float)cy, oldScale);
        float newX = cx + g_mapSelDir * (1.0f - eased) * SELECT_SLIDE_DIST;
        float newScale = SELECT_MIN_SCALE + eased * (1.0f - SELECT_MIN_SCALE);
        DrawMapCover(g_mapSelCurIdx, newX, (float)cy, newScale);
    }
    else DrawMapCover(g_mapSelCurIdx, (float)cx, (float)cy, 1.0f);
    settextstyle(24, 0, FONT_NAME); setbkmode(TRANSPARENT); settextcolor(WHITE);
    const char* hint = "空格键 选择";
    int tw = textwidth(hint);
    outtextxy(WIN_W - 20 - tw, WIN_H - 40, hint);
}

void UpdateMapSelect()
{
    int dir = 0;
    if (KeyPressed(VK_LEFT) || KeyPressed('A') || KeyPressed('H')) dir = -1;
    else if (KeyPressed(VK_RIGHT) || KeyPressed('D') || KeyPressed('K')) dir = 1;
    if (dir != 0 && g_mapCount > 1)
    {
        PlaySFX(SND_HUADONG);
        g_mapSelOldIdx = g_mapSelCurIdx;
        g_mapSelCurIdx = (g_mapSelCurIdx + dir + g_mapCount) % g_mapCount;
        g_mapSelDir = dir; g_mapSelAnimT = 0.0f;
    }
    else if (g_mapSelAnimT < 1.0f)
    { g_mapSelAnimT += g_dt / SELECT_ANIM_DURATION; if (g_mapSelAnimT >= 1.0f) g_mapSelAnimT = 1.0f; }
    if (KeyPressed(VK_SPACE))
    {
        PlaySFX(SND_UI);
        g_selMapIdx = g_mapSelCurIdx;
        for (int i = 0; i < MAX_PLAYERS; i++)
        { g_tankSelCurIdx[i] = g_selTankIdx[i]; g_tankSelOldIdx[i] = g_selTankIdx[i];
          g_tankSelAnimT[i] = 1.0f; g_tankSelDir[i] = 1; }
        StartTransition(ST_TANK_SELECT);
    }
}

void DrawTankCoverSide(int pIdx, int tIdx, float scrCx, float scrCy, float scale)
{
    if (tIdx < 0 || tIdx >= g_tankCount) return;
    IMAGE* tex = &g_tankTex[tIdx];
    int sw = tex->getwidth(), sh = tex->getheight();
    if (sw <= 0 || sh <= 0) return;
    float baseSc = 140.0f / (float)(sw > sh ? sw : sh);
    DrawSprite(tex, scrCx, scrCy, baseSc * scale, 0.0f);
    int fs = (int)(22 * scale + 0.5f); if (fs < 8) fs = 8;
    settextstyle(fs, 0, FONT_NAME); setbkmode(TRANSPARENT); settextcolor(WHITE);
    const char* name = TANK_NAMES[tIdx];
    int tw = textwidth(name);
    outtextxy((int)scrCx - tw / 2, (int)(scrCy + 120 * scale), name);
}

int GetTankSelCenterX(int pIdx)
{
    if (g_playerCount == 2) return (pIdx == 0) ? (WIN_W / 4) : (3 * WIN_W / 4);
    return (2 * pIdx + 1) * WIN_W / 6;
}

void DrawTankSelectSide(int pIdx)
{
    int cx = GetTankSelCenterX(pIdx), cy = 280;
    settextstyle(28, 0, FONT_NAME); setbkmode(TRANSPARENT); settextcolor(WHITE);
    const char* label;
    if (pIdx == 0) label = "P1";
    else if (pIdx == 1) label = (g_isSinglePlayer ? "AI操控" : "P2");
    else label = "P3";
    int lw = textwidth(label);
    outtextxy(cx - lw / 2, 30, label);
    if (g_tankSelAnimT[pIdx] < 1.0f)
    {
        float t = g_tankSelAnimT[pIdx];
        float eased = 1.0f - (1.0f - t) * (1.0f - t);
        int dir = g_tankSelDir[pIdx];
        float oldY = cy - dir * eased * 500.0f;
        float oldScale = 1.0f - eased * (1.0f - SELECT_MIN_SCALE);
        DrawTankCoverSide(pIdx, g_tankSelOldIdx[pIdx], (float)cx, oldY, oldScale);
        float newY = cy + dir * (1.0f - eased) * 500.0f;
        float newScale = SELECT_MIN_SCALE + eased * (1.0f - SELECT_MIN_SCALE);
        DrawTankCoverSide(pIdx, g_tankSelCurIdx[pIdx], (float)cx, newY, newScale);
    }
    else DrawTankCoverSide(pIdx, g_tankSelCurIdx[pIdx], (float)cx, (float)cy, 1.0f);
}

void DrawTankSelect()
{
    setfillcolor(RGB(150, 150, 150)); solidrectangle(0, 0, WIN_W, WIN_H);
    setlinecolor(RGB(100, 100, 100));
    if (g_playerCount == 2) line(WIN_W / 2, 0, WIN_W / 2, WIN_H);
    else { line(WIN_W / 3, 0, WIN_W / 3, WIN_H); line(2 * WIN_W / 3, 0, 2 * WIN_W / 3, WIN_H); }
    for (int i = 0; i < g_playerCount; i++) DrawTankSelectSide(i);
    settextstyle(24, 0, FONT_NAME); setbkmode(TRANSPARENT); settextcolor(WHITE);
    const char* hint = "空格键 选择";
    int tw = textwidth(hint);
    outtextxy(WIN_W - 20 - tw, WIN_H - 40, hint);
}

void UpdateTankSelect()
{
    for (int p = 0; p < g_playerCount; p++)
    {
        if (p == 1 && g_isSinglePlayer) continue;
        int dir = 0;
        if (p == 0)
        { if (KeyPressed('W') || KeyPressed('A')) dir = -1; else if (KeyPressed('S') || KeyPressed('D')) dir = 1; }
        else if (p == 1)
        { if (KeyPressed('I') || KeyPressed('J')) dir = -1; else if (KeyPressed('K') || KeyPressed('L')) dir = 1; }
        else
        { if (KeyPressed(VK_NUMPAD5) || KeyPressed(VK_NUMPAD4)) dir = -1;
          else if (KeyPressed(VK_NUMPAD2) || KeyPressed(VK_NUMPAD6)) dir = 1; }
        if (dir != 0 && g_tankCount > 1)
        {
            PlaySFX(SND_HUADONG);
            g_tankSelOldIdx[p] = g_tankSelCurIdx[p];
            g_tankSelCurIdx[p] = (g_tankSelCurIdx[p] + dir + g_tankCount) % g_tankCount;
            g_tankSelDir[p] = dir; g_tankSelAnimT[p] = 0.0f;
        }
    }
    for (int p = 0; p < g_playerCount; p++)
    {
        if (g_tankSelAnimT[p] < 1.0f)
        { g_tankSelAnimT[p] += g_dt / SELECT_ANIM_DURATION;
          if (g_tankSelAnimT[p] >= 1.0f) g_tankSelAnimT[p] = 1.0f; }
    }
    if (KeyPressed(VK_SPACE))
    {
        PlaySFX(SND_UI);
        for (int i = 0; i < MAX_PLAYERS; i++) g_selTankIdx[i] = g_tankSelCurIdx[i];
        StartTransition(ST_PVP_COUNT);
    }
}

void DrawTankInfoCover(int tIdx, float scrCx, float scrCy, float scale)
{
    if (tIdx < 0 || tIdx >= g_tankCount) return;
    IMAGE* tex = &g_tankTex[tIdx];
    int sw = tex->getwidth(), sh = tex->getheight();
    if (sw <= 0 || sh <= 0) return;
    float baseSc = 200.0f / (float)(sw > sh ? sw : sh);
    DrawSprite(tex, scrCx, scrCy, baseSc * scale, 0.0f);
    int fs = (int)(30 * scale + 0.5f); if (fs < 8) fs = 8;
    settextstyle(fs, 0, FONT_NAME); setbkmode(TRANSPARENT); settextcolor(WHITE);
    const char* name = TANK_NAMES[tIdx];
    int tw = textwidth(name);
    outtextxy((int)scrCx - tw / 2, (int)(scrCy + 150 * scale), name);
}

void DrawStatBar(int x, int y, int w, int h, float ratio, const char* label)
{
    settextstyle(20, 0, FONT_NAME); setbkmode(TRANSPARENT); settextcolor(WHITE);
    int tw = textwidth(label), th = textheight(label);
    outtextxy(x - tw - 12, y + h / 2 - th / 2, label);
    setfillcolor(RGB(50, 50, 50)); solidrectangle(x, y, x + w, y + h);
    int fillW = (int)(w * ratio);
    if (fillW > 0) { setfillcolor(RGB(255, 255, 255)); solidrectangle(x, y, x + fillW, y + h); }
    setlinecolor(RGB(220, 220, 220)); rectangle(x, y, x + w, y + h);
}

void DrawTankInfo()
{
    setfillcolor(RGB(150, 150, 150)); solidrectangle(0, 0, WIN_W, WIN_H);
    setfillcolor(RGB(60, 60, 60)); solidrectangle(WIN_W / 2, 0, WIN_W, WIN_H);
    int cx = WIN_W / 4, cy = WIN_H / 2 - 30;
    if (g_tankInfoAnimT < 1.0f)
    {
        float t = g_tankInfoAnimT;
        float eased = 1.0f - (1.0f - t) * (1.0f - t);
        int dir = g_tankInfoDir;
        float oldY = cy - dir * eased * 500.0f;
        float oldScale = 1.0f - eased * (1.0f - SELECT_MIN_SCALE);
        DrawTankInfoCover(g_tankInfoOldIdx, (float)cx, oldY, oldScale);
        float newY = cy + dir * (1.0f - eased) * 500.0f;
        float newScale = SELECT_MIN_SCALE + eased * (1.0f - SELECT_MIN_SCALE);
        DrawTankInfoCover(g_tankInfoCurIdx, (float)cx, newY, newScale);
    }
    else DrawTankInfoCover(g_tankInfoCurIdx, (float)cx, (float)cy, 1.0f);
    int barX = 480, barW = 250, barH = 24, rowGap = 50, firstY = 110;
    const char* statLabels[4] = { "射速", "移速", "伤害", "血量" };
    for (int i = 0; i < 4; i++)
        DrawStatBar(barX, firstY + i * rowGap, barW, barH, g_tankInfoBar[i], statLabels[i]);
    if (g_tankInfoCurIdx >= 0 && g_tankInfoCurIdx < TANK_NAMES_COUNT)
    {
        int textX = WIN_W / 2 + 30, textW = WIN_W / 2 - 60, lineH = 26;
        int descY = firstY + 4 * rowGap + 20;
        settextstyle(20, 0, FONT_NAME); setbkmode(TRANSPARENT);
        char activeLabel[64];
        sprintf(activeLabel, "主动技能：%s", TANK_ACTIVE_NAME[g_tankInfoCurIdx]);
        int aw = textwidth(activeLabel), ah = textheight(activeLabel);
        setfillcolor(RGB(55, 55, 55));
        solidrectangle(textX - 6, descY - 4, textX + aw + 6, descY + ah + 4);
        settextcolor(WHITE); outtextxy(textX, descY, activeLabel);
        int activeDescY = descY + ah + 8;
        DrawWrappedCN(TANK_SKILL_DESC[g_tankInfoCurIdx], textX, activeDescY, textW, lineH);
        int passiveLabelY = activeDescY + 55;
        char passiveLabel[64];
        sprintf(passiveLabel, "被动技能：%s", TANK_PASSIVE_NAME[g_tankInfoCurIdx]);
        int pw = textwidth(passiveLabel), ph = textheight(passiveLabel);
        setfillcolor(RGB(140, 140, 140));
        solidrectangle(textX - 6, passiveLabelY - 4, textX + pw + 6, passiveLabelY + ph + 4);
        settextcolor(WHITE); outtextxy(textX, passiveLabelY, passiveLabel);
        int passiveDescY = passiveLabelY + ph + 8;
        DrawWrappedCN(TANK_PASSIVE_DESC[g_tankInfoCurIdx], textX, passiveDescY, textW, lineH);
    }
    settextstyle(20, 0, FONT_NAME); setbkmode(TRANSPARENT); settextcolor(WHITE);
    const char* hint = "空格键 返回";
    int tw = textwidth(hint);
    outtextxy(WIN_W - 20 - tw, WIN_H - 36, hint);
}

void UpdateTankInfo()
{
    if (g_tankInfoAnimT < 1.0f)
    { g_tankInfoAnimT += g_dt / SELECT_ANIM_DURATION; if (g_tankInfoAnimT >= 1.0f) g_tankInfoAnimT = 1.0f; }
    for (int i = 0; i < 4; i++)
    {
        float target = CalcStatRatio(g_tankInfoCurIdx, i);
        float k = 1.0f - expf(-6.0f * g_dt);
        g_tankInfoBar[i] += (target - g_tankInfoBar[i]) * k;
    }
    int dir = 0;
    if (KeyPressed('W') || KeyPressed('A') || KeyPressed('U') || KeyPressed('H')) dir = -1;
    else if (KeyPressed('S') || KeyPressed('D') || KeyPressed('J') || KeyPressed('K')) dir = 1;
    if (dir != 0 && g_tankCount > 1)
    {
        PlaySFX(SND_HUADONG);
        g_tankInfoOldIdx = g_tankInfoCurIdx;
        g_tankInfoCurIdx = (g_tankInfoCurIdx + dir + g_tankCount) % g_tankCount;
        g_tankInfoDir = dir; g_tankInfoAnimT = 0.0f;
    }
    if (KeyPressed(VK_SPACE)) { PlaySFX(SND_UI); StartTransition(ST_MENU); }
}

void ResetTankForRespawn(int i)
{
    Tank& t = g_tank[i];
    int tidx = g_selTankIdx[i];
    int mhp = (int)g_tankStats[tidx][5];
    if (mhp < 50) mhp = 50;
    if (mhp > 200) mhp = 200;
    t.maxHp = mhp; t.hp = mhp; t.shadowHp = (float)mhp;
    t.bodyAngle = 0.0f; t.vel = 0.0f;
    t.cooldown = 0.0f; t.muzzleTimer = 0.0f;
    t.hitFlashTimer = 0.0f; t.skillFlashTimer = 0.0f;
    t.skillCd = 1.0f; t.skillActive = false;
    t.dashActive = false;
    t.knockbackActive = false;
    t.stunTimer = 0.0f; t.speedBoostTimer = 0.0f;
    t.afterimageSpawnTimer = 0.0f;
    t.shieldActive = false; t.shieldFlashTimer = 0.0f;
    t.hookState = 0; t.hookTarget = -1;
    t.dmgAccum = 0.0f;
    t.heat = 0.0f; t.overheated = false;
    t.frenzyTimer = 0.0f; t.frenzyCd = 0.0f;
    t.phaseTimer = 0.0f; t.phaseAlpha = 0.0f;
    t.inWall = false; t.phaseStuckInWall = false; t.phaseStuckDmg = 0.0f;
    t.empTimer = -1.0f;
    t.deathFade = 0.0f;
    t.deathSoundPlayed = false;
    t.buffType = -1; t.buffTimer = 0.0f;
    t.buffTotal = 0.0f; t.buffHealAccum = 0.0f;
    t.slowFactor = 1.0f; t.frostShellTimer = 0.0f;
    for (int j = 0; j < MAX_PLAYERS; j++) t.marks[j] = 0;
    t.x = WORLD_W * 0.5f; t.y = WORLD_H * 0.5f;
    char path[128];
    sprintf(path, "map/map%d.txt", g_selMapIdx + 1);
    FILE* fp = fopen(path, "r");
    if (fp)
    {
        for (int r = 0; r < MAP_H; r++)
        {
            char line[512];
            if (!fgets(line, sizeof(line), fp)) break;
            int len = (int)strlen(line);
            for (int c = 0; c < MAP_W && c < len; c++)
            {
                char ch = line[c];
                if (i == 0 && ch == 'A') { t.x = c * TILE + TILE * 0.5f; t.y = r * TILE + TILE * 0.5f; }
                if (i == 1 && ch == 'B') { t.x = c * TILE + TILE * 0.5f; t.y = r * TILE + TILE * 0.5f; }
                if (i == 2 && ch == 'C') { t.x = c * TILE + TILE * 0.5f; t.y = r * TILE + TILE * 0.5f; }
            }
        }
        fclose(fp);
    }
    g_respawnInv[i] = COMP_RESPAWN_INV;
}

void InitGame()
{
    LoadWinsum();
    memset(g_map, 0, sizeof(g_map));
    for (int i = 0; i < MAX_PLAYERS; i++) { g_tank[i].x = WORLD_W * 0.5f; g_tank[i].y = WORLD_H * 0.5f; }
    char path[128];
    sprintf(path, "map/map%d.txt", g_selMapIdx + 1);
    FILE* fp = fopen(path, "r");
    if (fp)
    {
        for (int r = 0; r < MAP_H; r++)
        {
            char line[512];
            if (!fgets(line, sizeof(line), fp)) break;
            int len = (int)strlen(line);
            for (int c = 0; c < MAP_W && c < len; c++)
            {
                char ch = line[c];
                if (ch == '\n' || ch == '\r' || ch == 0) break;
                if (ch == 'A') { g_tank[0].x = c * TILE + TILE * 0.5f; g_tank[0].y = r * TILE + TILE * 0.5f; g_map[r][c] = 0; }
                else if (ch == 'B') { g_tank[1].x = c * TILE + TILE * 0.5f; g_tank[1].y = r * TILE + TILE * 0.5f; g_map[r][c] = 0; }
                else if (ch == 'C') { g_tank[2].x = c * TILE + TILE * 0.5f; g_tank[2].y = r * TILE + TILE * 0.5f; g_map[r][c] = 0; }
                else if (ch >= '0' && ch <= '9') g_map[r][c] = ch - '0';
                else g_map[r][c] = 0;
            }
        }
        fclose(fp);
    }
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        int tidx = g_selTankIdx[i];
        int mhp = (int)g_tankStats[tidx][5];
        if (mhp < 50) mhp = 50;
        if (mhp > 200) mhp = 200;
        g_tank[i].maxHp = mhp; g_tank[i].hp = mhp; g_tank[i].shadowHp = (float)mhp;
        g_tank[i].bodyAngle = 0.0f; g_tank[i].vel = 0.0f;
        g_tank[i].cooldown = 0.0f; g_tank[i].muzzleTimer = 0.0f;
        g_tank[i].hitFlashTimer = 0.0f; g_tank[i].skillFlashTimer = 0.0f;
        g_tank[i].skillCd = 1.0f; g_tank[i].skillActive = false;
        g_tank[i].dashActive = false;
        g_tank[i].dashTimer = 0.0f; g_tank[i].dashDuration = 0.0f;
        g_tank[i].dashStartX = 0.0f; g_tank[i].dashStartY = 0.0f;
        g_tank[i].dashTargetX = 0.0f; g_tank[i].dashTargetY = 0.0f;
        g_tank[i].knockbackActive = false;
        g_tank[i].knockbackTimer = 0.0f; g_tank[i].knockbackDuration = 0.0f;
        g_tank[i].knockbackStartX = 0.0f; g_tank[i].knockbackStartY = 0.0f;
        g_tank[i].knockbackTargetX = 0.0f; g_tank[i].knockbackTargetY = 0.0f;
        g_tank[i].knockbackHitWall = false;
        g_tank[i].stunTimer = 0.0f; g_tank[i].speedBoostTimer = 0.0f;
        g_tank[i].afterimageSpawnTimer = 0.0f;
        g_tank[i].shieldActive = false; g_tank[i].shieldFlashTimer = 0.0f;
        g_tank[i].cdFullFlashTimer = 0.0f;
        g_tank[i].hookState = 0; g_tank[i].hookTarget = -1;
        g_tank[i].dmgAccum = 0.0f;
        g_tank[i].heat = 0.0f; g_tank[i].overheated = false;
        g_tank[i].frenzyTimer = 0.0f; g_tank[i].frenzyCd = 0.0f;
        g_tank[i].phaseTimer = 0.0f; g_tank[i].phaseAlpha = 0.0f;
        g_tank[i].inWall = false; g_tank[i].phaseStuckInWall = false; g_tank[i].phaseStuckDmg = 0.0f;
        g_tank[i].empTimer = -1.0f;
        g_tank[i].deathFade = 0.0f;
        g_tank[i].deathSoundPlayed = false;
        g_tank[i].buffType = -1; g_tank[i].buffTimer = 0.0f;
        g_tank[i].buffTotal = 0.0f; g_tank[i].buffHealAccum = 0.0f;
        g_tank[i].slowFactor = 1.0f; g_tank[i].frostShellTimer = 0.0f;
        for (int j = 0; j < MAX_PLAYERS; j++) g_tank[i].marks[j] = 0;
        g_aiPathTimer[i] = 0.0f; g_aiStuckTimer[i] = 0.0f; g_aiEscapeTimer[i] = 0.0f;
        g_aiLastX[i] = g_tank[i].x; g_aiLastY[i] = g_tank[i].y;
        g_aiPathLen[i] = 0; g_aiCellR[i] = -1; g_aiCellC[i] = -1;
        g_prevSkillCd[i] = 1.0f;
        g_score[i] = 0;
        g_respawnTimer[i] = 0.0f;
        g_respawnInv[i] = 0.0f;
        for (int j = 0; j < MAX_PLAYERS; j++) g_marks[i][j] = 0;
    }
    for (int i = 0; i < MAX_BULLETS; i++) g_bullets[i].alive = false;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        g_rings[i].active = false; g_rings[i].timer = 0.0f;
        g_rings[i].duration = SKILL3_KNOCK_TIME;
        g_rings[i].x = 0.0f; g_rings[i].y = 0.0f;
        g_frostFields[i].active = false; g_frostFields[i].timer = 0.0f;
        g_frostFields[i].x = 0.0f; g_frostFields[i].y = 0.0f;
        g_hunterRings[i].active = false; g_hunterRings[i].timer = 0.0f;
        g_hunterRings[i].x = 0.0f; g_hunterRings[i].y = 0.0f;
        for (int j = 0; j < MAX_PLAYERS; j++) g_hunterRings[i].hit[j] = false;
    }
    for (int i = 0; i < MAX_AFTERIMG; i++) g_afterImgs[i].active = false;
    for (int i = 0; i < MAX_BUFFS; i++) g_buffs[i].active = false;
    for (int i = 0; i < MAX_VWALLS; i++) g_vwalls[i].active = false;
    g_ignoreVWallOwner = -1;
    g_buffSpawnTimer = 0.0f; g_buffSpawnNext = 15.0f;
    g_countTimer = 0.0f; g_overTimer = 0.0f; g_overMaskA = 0.0f;
    g_invincibleTimer = INVINCIBLE_TIME;
    if (g_gameMode == GM_COMPETITIVE)
    {
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            g_rankPlayerY[i] = RANK_BASE_Y + RANK_ROW_H * (i + 1);
            g_rankPlayerSlot[i] = i;
        }
    }
    g_camInit = false;
    UpdateCamera();
}

void UpdateCamera()
{
    float minX = 1e9f, maxX = -1e9f, minY = 1e9f, maxY = -1e9f;
    for (int i = 0; i < g_playerCount; i++)
    {
        if (g_tank[i].x < minX) minX = g_tank[i].x;
        if (g_tank[i].x > maxX) maxX = g_tank[i].x;
        if (g_tank[i].y < minY) minY = g_tank[i].y;
        if (g_tank[i].y > maxY) maxY = g_tank[i].y;
    }
    minX -= CAM_PADDING; maxX += CAM_PADDING;
    minY -= CAM_PADDING; maxY += CAM_PADDING;
    float w = maxX - minX, h = maxY - minY;
    if (w < 1.0f) w = 1.0f;
    if (h < 1.0f) h = 1.0f;
    float sx = WIN_W / w, sy = WIN_H / h;
    float s = sx < sy ? sx : sy;
    if (s < CAM_MIN_SCALE) s = CAM_MIN_SCALE;
    if (s > CAM_MAX_SCALE) s = CAM_MAX_SCALE;
    float tX = (minX + maxX) * 0.5f, tY = (minY + maxY) * 0.5f;
    if (!g_camInit) { g_camScale = s; g_camX = tX; g_camY = tY; g_camInit = true; return; }
    float k = 1.0f - expf(-CAM_EASE * g_dt);
    g_camX += (tX - g_camX) * k; g_camY += (tY - g_camY) * k;
    g_camScale += (s - g_camScale) * k;
}

void FireBullet(int idx, float aimOffset, int dmgOverride, int bType)
{
    Tank& t = g_tank[idx];
    int tidx = g_selTankIdx[idx];
    float a = t.bodyAngle + aimOffset;
    float bulletSpd = g_tankStats[tidx][4];
    int damage = (dmgOverride >= 0) ? dmgOverride : (int)g_tankStats[tidx][3];
    if (t.buffType == BUFF_DAMAGE && t.buffTimer > 0.0f)
        damage = (int)(damage * BUFF_DAMAGE_MUL + 0.5f);
    float px = t.x + cosf(a) * TANK_SIZE * 0.55f;
    float py = t.y + sinf(a) * TANK_SIZE * 0.55f;
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!g_bullets[i].alive)
        {
            g_bullets[i].x = px; g_bullets[i].y = py;
            g_bullets[i].vx = cosf(a) * bulletSpd;
            g_bullets[i].vy = sinf(a) * bulletSpd;
            g_bullets[i].angle = a; g_bullets[i].alive = true;
            g_bullets[i].owner = idx; g_bullets[i].damage = damage;
            g_bullets[i].type = bType;
            g_bullets[i].lastCellC = (int)floorf(px / TILE);
            g_bullets[i].lastCellR = (int)floorf(py / TILE);
            break;
        }
    }
}

void DoFireNormal(int idx)
{
    int tidx = g_selTankIdx[idx];
    if (tidx == 4)
    {
        for (int k = 0; k < 5; k++)
        {
            float off = (-17.5f + 7.0f * k) * PI / 180.0f;
            FireBullet(idx, off, -1, BT_NORMAL);
        }
    }
    else if (tidx == 6) FireBullet(idx, 0.0f, -1, BT_PIERCE);
    else FireBullet(idx, 0.0f, -1, BT_NORMAL);
    if (tidx == 0) PlaySFX(SND_KP1);
    else if (tidx == 1) PlaySFX(SND_KP2);
    else if (tidx == 4) PlaySFX(SND_KP5);
    else if (tidx == 5) PlaySFXKp6();
    else if (tidx >= 2) PlaySFX(SND_KP3);
}

void TryFire(int idx, int kFire)
{
    Tank& t = g_tank[idx];
    int tidx = g_selTankIdx[idx];
    if (t.hp <= 0) return;
    if (IsTankSilenced(idx)) return;
    if (tidx == 5)
    {
        bool wantFire = KeyDown(kFire);
        if (wantFire && !t.overheated && t.cooldown <= 0.0f)
        {
            DoFireNormal(idx);
            float fireCd = g_tankStats[5][0];
            if (t.frenzyTimer > 0.0f) fireCd *= FRENZY_FIRE_MUL;
            if (t.buffType == BUFF_FIRE && t.buffTimer > 0.0f) fireCd *= BUFF_FIRE_MUL;
            fireCd /= t.slowFactor;
            t.cooldown = fireCd; t.muzzleTimer = MUZZLE_TIME;
        }
        return;
    }
    if (KeyDown(kFire) && t.cooldown <= 0.0f)
    {
        DoFireNormal(idx);
        float fireCd = g_tankStats[tidx][0];
        if (t.buffType == BUFF_FIRE && t.buffTimer > 0.0f) fireCd *= BUFF_FIRE_MUL;
        fireCd /= t.slowFactor;
        t.cooldown = fireCd; t.muzzleTimer = MUZZLE_TIME;
    }
}

bool StartDash(int idx, float wdx, float wdy, float maxDist, float duration, bool isChuanyun)
{
    Tank& t = g_tank[idx];
    float step = 5.0f, reached = 0.0f;
    while (reached < maxDist)
    {
        float s = step;
        if (reached + s > maxDist) s = maxDist - reached;
        float tx = t.x + wdx * (reached + s);
        float ty = t.y + wdy * (reached + s);
        if (CollideWithWall(tx, ty)) break;
        reached += s;
    }
    if (reached < 5.0f) return false;
    t.dashActive = true; t.dashTimer = 0.0f; t.dashDuration = duration;
    t.dashStartX = t.x; t.dashStartY = t.y;
    t.dashTargetX = t.x + wdx * reached; t.dashTargetY = t.y + wdy * reached;
    if (isChuanyun) { t.skillCd = 0.0f; t.speedBoostTimer = SKILL2_PASSIVE_TIME; }
    return true;
}

void CastKnockback(int casterIdx)
{
    Tank& caster = g_tank[casterIdx];
    g_rings[casterIdx].active = true; g_rings[casterIdx].timer = 0.0f;
    g_rings[casterIdx].duration = SKILL3_KNOCK_TIME;
    g_rings[casterIdx].x = caster.x; g_rings[casterIdx].y = caster.y;
    for (int other = 0; other < g_playerCount; other++)
    {
        if (other == casterIdx) continue;
        Tank& target = g_tank[other];
        if (target.hp <= 0) continue;
        float dx = target.x - caster.x, dy = target.y - caster.y;
        float d2 = dx * dx + dy * dy;
        if (d2 > SKILL3_RANGE * SKILL3_RANGE) continue;
        target.hp -= SKILL3_DAMAGE;
        if (target.hp < 0) target.hp = 0;
        target.skillFlashTimer = SKILL_FLASH_TIME;
        float len = sqrtf(d2);
        if (len < 0.0001f) { dx = 1.0f; dy = 0.0f; len = 1.0f; }
        float nx = dx / len, ny = dy / len;
        float step = 5.0f, reached = 0.0f, maxDist = SKILL3_KNOCK_DIST;
        while (reached < maxDist)
        {
            float s = step;
            if (reached + s > maxDist) s = maxDist - reached;
            float tx = target.x + nx * (reached + s);
            float ty = target.y + ny * (reached + s);
            if (CollideWithWall(tx, ty)) break;
            reached += s;
        }
        target.knockbackActive = true; target.knockbackTimer = 0.0f;
        target.knockbackDuration = SKILL3_KNOCK_TIME;
        target.knockbackStartX = target.x; target.knockbackStartY = target.y;
        target.knockbackTargetX = target.x + nx * reached;
        target.knockbackTargetY = target.y + ny * reached;
        target.knockbackHitWall = (reached < maxDist - 1.0f);
        target.dashActive = false; target.skillActive = false;
    }
}

void UpdateShieldLogic(Tank& t, bool skillPressed)
{
    if (t.shieldFlashTimer > 0.0f) t.shieldFlashTimer -= g_dt;
    if (t.shieldActive)
    {
        t.skillCd -= SKILL4_DRAIN_RATE * g_dt;
        if (t.skillCd <= 0.0f) { t.skillCd = 0.0f; t.shieldActive = false; PlaySFX(SND_JN4_2); return; }
        if (skillPressed) { t.shieldActive = false; PlaySFX(SND_JN4_2); return; }
    }
    else
    {
        if (t.skillCd < 1.0f) { t.skillCd += SKILL4_RECOVER_RATE * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; }
        if (skillPressed && t.skillCd >= SKILL4_OPEN_COST)
        {
            t.shieldActive = true;
            t.skillCd -= SKILL4_OPEN_COST;
            if (t.skillCd < 0.0f) t.skillCd = 0.0f;
            PlaySFX(SND_JN4_1);
        }
    }
}

bool PlaceVirtualWall(int tankIdx)
{
    Tank& t = g_tank[tankIdx];
    int c = (int)floorf(t.x / TILE);
    int r = (int)floorf(t.y / TILE);
    if (c < 0 || r < 0 || c >= MAP_W || r >= MAP_H) return false;
    if (g_map[r][c] == 1) return false;
    for (int w = 0; w < MAX_VWALLS; w++)
        if (g_vwalls[w].active && g_vwalls[w].col == c && g_vwalls[w].row == r) return false;
    for (int w = 0; w < MAX_VWALLS; w++)
    {
        if (!g_vwalls[w].active)
        {
            g_vwalls[w].active = true;
            g_vwalls[w].col = c; g_vwalls[w].row = r;
            g_vwalls[w].hp = VWALL_HP;
            g_vwalls[w].alpha = 0.0f;
            g_vwalls[w].fadingOut = false;
            g_vwalls[w].owner = tankIdx;
            return true;
        }
    }
    return false;
}

void UpdateVirtualWalls()
{
    for (int i = 0; i < MAX_VWALLS; i++)
    {
        if (!g_vwalls[i].active) continue;
        if (g_vwalls[i].fadingOut)
        {
            g_vwalls[i].alpha -= g_dt / VWALL_FADE_OUT;
            if (g_vwalls[i].alpha <= 0.0f) { g_vwalls[i].alpha = 0.0f; g_vwalls[i].active = false; }
        }
        else
        {
            if (g_vwalls[i].alpha < 1.0f)
            {
                g_vwalls[i].alpha += g_dt / VWALL_FADE_IN;
                if (g_vwalls[i].alpha > 1.0f) g_vwalls[i].alpha = 1.0f;
            }
            g_vwalls[i].hp -= VWALL_DECAY * g_dt;
            if (g_vwalls[i].hp <= 0.0f) { g_vwalls[i].hp = 0.0f; g_vwalls[i].fadingOut = true; }
        }
    }
}

void UpdateFrostFields()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!g_frostFields[i].active) continue;
        g_frostFields[i].timer += g_dt;
        if (g_frostFields[i].timer >= FROST_TOTAL) g_frostFields[i].active = false;
    }
    for (int i = 0; i < g_playerCount; i++)
    {
        float target = 1.0f;
        for (int j = 0; j < MAX_PLAYERS; j++)
        {
            if (!g_frostFields[j].active) continue;
            if (j == i) continue;
            float r = GetFrostRadius(g_frostFields[j]);
            if (r <= 1.0f) continue;
            float dx = g_tank[i].x - g_frostFields[j].x;
            float dy = g_tank[i].y - g_frostFields[j].y;
            if (dx * dx + dy * dy < r * r)
            {
                g_tank[i].slowFactor -= FROST_SLOW_RATE * g_dt;
                if (g_tank[i].slowFactor < FROST_MIN_FACTOR) g_tank[i].slowFactor = FROST_MIN_FACTOR;
                target = -1.0f;
                break;
            }
        }
        if (target > 0.0f)
        {
            if (g_tank[i].slowFactor < 1.0f)
            {
                g_tank[i].slowFactor += FROST_SLOW_RATE * g_dt;
                if (g_tank[i].slowFactor > 1.0f) g_tank[i].slowFactor = 1.0f;
            }
        }
        if (g_tank[i].frostShellTimer > 0.0f)
        {
            g_tank[i].frostShellTimer -= g_dt;
            if (g_tank[i].frostShellTimer < 0.0f) g_tank[i].frostShellTimer = 0.0f;
        }
    }
}

void UpdateHunterRings()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        HunterRing& r = g_hunterRings[i];
        if (!r.active) continue;
        r.timer += g_dt;
        if (r.timer >= HUNTER_TOTAL) { r.active = false; continue; }
        float radius = GetHunterRingRadius(r);
        int alpha = GetHunterRingAlpha(r);
        if (alpha <= 0) continue;
        float ringWidth = 16.0f;
        for (int j = 0; j < g_playerCount; j++)
        {
            if (j == i) continue;
            if (r.hit[j]) continue;
            if (g_tank[j].hp <= 0) continue;
            float dx = g_tank[j].x - r.x;
            float dy = g_tank[j].y - r.y;
            float d = sqrtf(dx * dx + dy * dy);
            if (fabsf(d - radius) < ringWidth * 0.5f + TANK_SIZE * 0.5f)
            {
                r.hit[j] = true;
                if (g_tank[j].marks[i] < HUNTER_MARK_MAX) g_tank[j].marks[i]++;
                PlaySFX(SND_JN11);
            }
        }
    }
}

void UpdateTank(int idx, int kFwd, int kBack, int kLeft, int kRight,
                int kFire, int kSkill, int kStrafeL, int kStrafeR)
{
    Tank& t = g_tank[idx];
    int tidx = g_selTankIdx[idx];

    g_ignoreVWallOwner = idx;
    if (IsTankSilenced(idx)) kSkill = 0;

    float moveSpd = g_tankStats[tidx][1];
    float turnSpd = g_tankStats[tidx][2];

    if (t.cooldown > 0.0f) t.cooldown -= g_dt;
    if (t.muzzleTimer > 0.0f) t.muzzleTimer -= g_dt;
    if (t.hitFlashTimer > 0.0f) t.hitFlashTimer -= g_dt;
    if (t.skillFlashTimer > 0.0f) t.skillFlashTimer -= g_dt;
    if (t.speedBoostTimer > 0.0f) t.speedBoostTimer -= g_dt;
    if (t.frenzyTimer > 0.0f) t.frenzyTimer -= g_dt;
    if (t.frenzyCd > 0.0f) t.frenzyCd -= g_dt;

    if (t.hp <= 0)
    {
        if (!t.deathSoundPlayed) { t.deathSoundPlayed = true; PlaySFX(SND_KILL); }
        t.deathFade += g_dt * 0.6f;
        if (t.deathFade > 1.0f) t.deathFade = 1.0f;
        return;
    }

    float slowMul = t.slowFactor;
    if (t.frostShellTimer > 0.0f) slowMul *= FROST_SHELL_FACTOR;
    if (slowMul < 0.01f) slowMul = 0.01f;

    if (tidx == 6 && (t.phaseTimer > 0.0f || t.phaseStuckInWall))
    {
        if (t.phaseTimer > 0.0f)
        {
            t.phaseTimer -= g_dt;
            float elapsed = PHASE_DURATION - t.phaseTimer;
            if (elapsed < PHASE_FADE_IN) t.phaseAlpha = elapsed / PHASE_FADE_IN;
            else t.phaseAlpha = 1.0f;
            int cc = (int)floorf(t.x / TILE), rr = (int)floorf(t.y / TILE);
            t.inWall = (cc >= 0 && rr >= 0 && cc < MAP_W && rr < MAP_H && g_map[rr][cc] == 1);
            if (t.phaseTimer <= 0.0f)
            {
                PlaySFX(SND_JN7_1);
                t.phaseTimer = 0.0f;
                t.phaseAlpha = 0.0f;
                if (CollideWithWall(t.x, t.y)) t.phaseStuckInWall = true;
                else { t.inWall = false; t.phaseStuckDmg = 0.0f; }
            }
        }
        else
        {
            t.phaseStuckDmg += 5.0f * g_dt;
            while (t.phaseStuckDmg >= 1.0f)
            {
                t.phaseStuckDmg -= 1.0f;
                t.hp -= 1;
                if (t.hp < 0) { t.hp = 0; break; }
            }
            if (!CollideWithWall(t.x, t.y))
            { t.phaseStuckInWall = false; t.inWall = false; t.phaseStuckDmg = 0.0f; }
        }
        if (t.hp <= 0) return;
        float turn = 0.0f;
        if (KeyDown(kLeft)) turn -= 1.0f;
        if (KeyDown(kRight)) turn += 1.0f;
        t.bodyAngle += turn * turnSpd * g_dt;
        float move = 0.0f;
        if (KeyDown(kFwd)) move += 1.0f;
        if (KeyDown(kBack)) move -= 1.0f;
        float strafe = 0.0f;
        if (KeyDown(kStrafeL)) strafe += 1.0f;
        if (KeyDown(kStrafeR)) strafe -= 1.0f;
        float vx = 0.0f, vy = 0.0f;
        if (move != 0.0f)
        { vx += cosf(t.bodyAngle) * move * moveSpd; vy += sinf(t.bodyAngle) * move * moveSpd; }
        if (strafe != 0.0f)
        {
            float ca = cosf(t.bodyAngle), sa = sinf(t.bodyAngle);
            vx += sa * strafe * moveSpd * STRAFE_SPEED_MUL;
            vy += -ca * strafe * moveSpd * STRAFE_SPEED_MUL;
        }
        t.x += vx * g_dt;
        t.y += vy * g_dt;
        return;
    }

    if (t.stunTimer > 0.0f) { t.stunTimer -= g_dt; TryFire(idx, kFire); return; }

    if (tidx == 4 && t.hookState != 0)
    {
        if (t.hookState == 2)
        {
            float dx = t.hookX - t.x, dy = t.hookY - t.y;
            float len = sqrtf(dx * dx + dy * dy);
            if (len < 5.0f) t.hookState = 0;
            else
            {
                float nx = dx / len, ny = dy / len;
                float step = HOOK_PULL_SELF_SPD * g_dt;
                if (step > len) step = len;
                float tx = t.x + nx * step, ty = t.y + ny * step;
                if (!CollideWithWall(tx, ty)) { t.x = tx; t.y = ty; }
                else t.hookState = 0;
            }
        }
        else if (t.hookState == 1)
        {
            float step = HOOK_FLY_SPEED * g_dt;
            t.hookDist += step;
            t.hookX = t.hookStartX + cosf(t.hookAngle) * t.hookDist;
            t.hookY = t.hookStartY + sinf(t.hookAngle) * t.hookDist;
            int cc = (int)floorf(t.hookX / TILE);
            int rr = (int)floorf(t.hookY / TILE);
            if (cc < 0 || rr < 0 || cc >= MAP_W || rr >= MAP_H || g_map[rr][cc] == 1)
            { t.hookState = 2; PlaySFX(SND_JN5_1); }
            else
            {
                for (int j = 0; j < g_playerCount; j++)
                {
                    if (j == idx) continue;
                    if (g_tank[j].hp <= 0) continue;
                    float ddx = t.hookX - g_tank[j].x, ddy = t.hookY - g_tank[j].y;
                    if (ddx * ddx + ddy * ddy < (TANK_SIZE * 0.5f) * (TANK_SIZE * 0.5f))
                    { t.hookState = 3; t.hookTarget = j; PlaySFX(SND_JN5_1); break; }
                }
            }
            if (t.hookState == 1 && t.hookDist > HOOK_RANGE)
            { t.hookState = 4; t.hookDist = HOOK_RANGE; }
        }
        else if (t.hookState == 4)
        {
            float step = HOOK_FLY_SPEED * g_dt;
            t.hookDist -= step;
            if (t.hookDist < 0.0f) t.hookDist = 0.0f;
            t.hookX = t.hookStartX + cosf(t.hookAngle) * t.hookDist;
            t.hookY = t.hookStartY + sinf(t.hookAngle) * t.hookDist;
            if (t.hookDist <= 0.0f) t.hookState = 0;
        }
        else if (t.hookState == 3)
        {
            int j = t.hookTarget;
            if (j < 0 || j >= g_playerCount || g_tank[j].hp <= 0) { t.hookState = 0; t.hookTarget = -1; }
            else
            {
                Tank& enemy = g_tank[j];
                float dx = t.x - enemy.x, dy = t.y - enemy.y;
                float len = sqrtf(dx * dx + dy * dy);
                if (len <= HOOK_RELEASE_DIST) { t.hookState = 0; t.hookTarget = -1; }
                else
                {
                    float nx = dx / len, ny = dy / len;
                    float step = HOOK_PULL_ENEMY_SPD * g_dt;
                    if (step > len - HOOK_RELEASE_DIST) step = len - HOOK_RELEASE_DIST;
                    if (step > len) step = len;
                    float tx = enemy.x + nx * step, ty = enemy.y + ny * step;
                    float moved = 0.0f;
                    if (!CollideWithWall(tx, ty))
                    { moved = step; enemy.x = tx; enemy.y = ty; t.hookX = enemy.x; t.hookY = enemy.y; }
                    else { t.hookState = 0; t.hookTarget = -1; }
                    enemy.dmgAccum += moved;
                    while (enemy.dmgAccum >= TILE)
                    {
                        enemy.dmgAccum -= TILE;
                        enemy.hp -= HOOK_ENEMY_DMG_PER_TILE;
                        if (enemy.hp < 0) enemy.hp = 0;
                        enemy.skillFlashTimer = SKILL_FLASH_TIME;
                    }
                }
            }
        }
    }

    if (t.knockbackActive)
    {
        t.knockbackTimer += g_dt;
        float tt = t.knockbackTimer / t.knockbackDuration;
        if (tt > 1.0f) tt = 1.0f;
        float eased = tt * tt * (3.0f - 2.0f * tt);
        t.x = t.knockbackStartX + (t.knockbackTargetX - t.knockbackStartX) * eased;
        t.y = t.knockbackStartY + (t.knockbackTargetY - t.knockbackStartY) * eased;
        TryFire(idx, kFire);
        if (tt >= 1.0f)
        { t.knockbackActive = false; if (t.knockbackHitWall) t.stunTimer = SKILL3_STUN_TIME; }
        return;
    }

    if (t.dashActive)
    {
        float turn = 0.0f;
        if (KeyDown(kLeft)) turn -= 1.0f;
        if (KeyDown(kRight)) turn += 1.0f;
        t.bodyAngle += turn * turnSpd * SKILL2_DASH_TURN_MUL * g_dt;
        t.dashTimer += g_dt;
        float tt = t.dashTimer / t.dashDuration;
        if (tt > 1.0f) tt = 1.0f;
        float eased = tt * tt * (3.0f - 2.0f * tt);
        t.x = t.dashStartX + (t.dashTargetX - t.dashStartX) * eased;
        t.y = t.dashStartY + (t.dashTargetY - t.dashStartY) * eased;
        if (tt >= 1.0f) t.dashActive = false;
        if (tidx == 1 && t.skillCd < 1.0f)
        { t.skillCd += SKILL2_RECOVER_RATE * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; }
        TryFire(idx, kFire);
        return;
    }

    float speedMulSelf = 1.0f, turnSpdSelf = turnSpd;
    if (tidx == 4 && t.hookState == 2)
    { speedMulSelf = HOOK_SELF_MOVE_MUL; turnSpdSelf = turnSpd * HOOK_SELF_MOVE_MUL; }

    float turn = 0.0f;
    if (KeyDown(kLeft)) turn -= 1.0f;
    if (KeyDown(kRight)) turn += 1.0f;
    t.bodyAngle += turn * turnSpdSelf * slowMul * g_dt;

    if (tidx == 0)
    {
        if (KeyPressed(kSkill) && !t.skillActive && t.skillCd >= SKILL1_ACTIVATE_TH)
        { t.skillActive = true; t.afterimageSpawnTimer = 0.0f; }
        if (t.skillActive)
        {
            t.skillCd -= SKILL1_DRAIN_RATE * g_dt;
            if (t.skillCd <= 0.0f) { t.skillCd = 0.0f; t.skillActive = false; }
            if (!KeyDown(kSkill)) t.skillActive = false;
            t.afterimageSpawnTimer += g_dt;
            if (t.afterimageSpawnTimer >= AFTERIMG_INTERVAL_YOUXIA)
            { SpawnAfterImage(tidx, t.x, t.y, t.bodyAngle, AFTERIMG_DUR_YOUXIA); t.afterimageSpawnTimer = 0.0f; }
        }
        else { if (t.skillCd < 1.0f) { t.skillCd += SKILL1_RECOVER_RATE * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; } }
        if (t.skillActive && !t.dashActive && t.skillCd >= SKILL1_STRAFE_COST)
        {
            float dirSign = 0.0f;
            if (KeyPressed(kStrafeL)) dirSign = 1.0f;
            else if (KeyPressed(kStrafeR)) dirSign = -1.0f;
            if (dirSign != 0.0f)
            {
                float ca = cosf(t.bodyAngle), sa = sinf(t.bodyAngle);
                float wdx = sa * dirSign, wdy = -ca * dirSign;
                if (StartDash(idx, wdx, wdy, SKILL1_STRAFE_DIST, SKILL1_STRAFE_TIME, false))
                { t.skillCd -= SKILL1_STRAFE_COST; if (t.skillCd < 0.0f) t.skillCd = 0.0f; }
            }
        }
    }
    else if (tidx == 1)
    {
        if (KeyPressed(kSkill) && t.skillCd >= 1.0f)
        {
            bool fwd = KeyDown(kFwd), back = KeyDown(kBack);
            bool left = KeyDown(kStrafeL), right = KeyDown(kStrafeR);
            float localX = 0.0f, localY = 0.0f;
            if (fwd && !back) localX = 1.0f;
            else if (back && !fwd) localX = -1.0f;
            if (left && !right) localY = 1.0f;
            else if (right && !left) localY = -1.0f;
            if (localX == 0.0f && localY == 0.0f) localX = 1.0f;
            float ca = cosf(t.bodyAngle), sa = sinf(t.bodyAngle);
            float wdx = localX * ca + localY * sa;
            float wdy = localX * sa - localY * ca;
            float len = sqrtf(wdx * wdx + wdy * wdy);
            if (len > 0.0001f) { wdx /= len; wdy /= len; }
            float dist = (localX > 0.5f) ? (float)SKILL2_DASH_LONG : (float)SKILL2_DASH_SHORT;
            float oldX = t.x, oldY = t.y, oldAngle = t.bodyAngle;
            if (StartDash(idx, wdx, wdy, dist, SKILL2_DASH_DURATION, true))
            { SpawnAfterImage(tidx, oldX, oldY, oldAngle, AFTERIMG_DUR_CHUANYUN); PlaySFX(SND_JN2); }
        }
        else { if (t.skillCd < 1.0f) { t.skillCd += SKILL2_RECOVER_RATE * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; } }
    }
    else if (tidx == 2)
    {
        if (KeyPressed(kSkill) && t.skillCd >= 1.0f)
        { CastKnockback(idx); t.skillCd = 0.0f; PlaySFX(SND_JN3); }
        else { if (t.skillCd < 1.0f) { t.skillCd += SKILL3_RECOVER_RATE * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; } }
    }
    else if (tidx == 3)
    {
        bool sp = KeyPressed(kSkill);
        UpdateShieldLogic(t, sp);
    }
    else if (tidx == 4)
    {
        if (KeyPressed(kSkill) && t.hookState == 0 && t.skillCd >= 1.0f)
        {
            t.hookState = 1;
            t.hookStartX = t.x; t.hookStartY = t.y;
            t.hookX = t.x; t.hookY = t.y;
            t.hookAngle = t.bodyAngle;
            t.hookDist = 0.0f;
            t.skillCd = 0.0f;
            PlaySFX(SND_JN5_2);
        }
        else { if (t.skillCd < 1.0f) { t.skillCd += HOOK_CD * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; } }
    }
    else if (tidx == 5)
    {
        if (t.overheated)
        {
            t.heat -= HEAT_RATE_DOWN * g_dt;
            if (t.heat < 0.0f) t.heat = 0.0f;
            if (t.heat <= HEAT_RECOVER_TH) t.overheated = false;
        }
        else
        {
            bool firing = KeyDown(kFire);
            if (firing)
            {
                float rate = (t.frenzyTimer > 0.0f) ? FRENZY_HEAT_RATE : HEAT_RATE_UP;
                t.heat += rate * g_dt;
                if (t.heat >= HEAT_OVERHEAT)
                { t.heat = HEAT_OVERHEAT; t.overheated = true; PlaySFX(SND_JN6); }
            }
            else
            {
                t.heat -= HEAT_RATE_DOWN * g_dt;
                if (t.heat < 0.0f) t.heat = 0.0f;
            }
        }
        if (KeyPressed(kSkill) && t.frenzyCd <= 0.0f)
        { t.frenzyTimer = FRENZY_DURATION; t.frenzyCd = FRENZY_CD; }
    }
    else if (tidx == 6)
    {
        if (KeyPressed(kSkill) && t.skillCd >= 1.0f)
        { t.phaseTimer = PHASE_DURATION; t.phaseAlpha = 0.0f; t.skillCd = 0.0f; PlaySFX(SND_JN7_2); }
        else { if (t.skillCd < 1.0f) { t.skillCd += (1.0f / PHASE_CD) * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; } }
    }
    else if (tidx == 7)
    {
        if (KeyPressed(kSkill) && t.empTimer < 0.0f && t.skillCd >= 1.0f)
        { t.empTimer = 0.0f; t.skillCd = 0.0f; PlaySFX(SND_JN8); }
        else
        {
            if (t.skillCd < 1.0f)
            { t.skillCd += (1.0f / EMP_CD) * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; }
        }
    }
    else if (tidx == 8)
    {
        if (KeyPressed(kSkill) && t.skillCd >= BEDROCK_PLACE_COST)
        {
            if (PlaceVirtualWall(idx))
            {
                t.skillCd -= BEDROCK_PLACE_COST;
                if (t.skillCd < 0.0f) t.skillCd = 0.0f;
                PlaySFX(SND_JN9);
            }
        }
        if (t.skillCd < 1.0f)
        {
            t.skillCd += BEDROCK_RECOVER_RATE * g_dt;
            if (t.skillCd > 1.0f) t.skillCd = 1.0f;
        }
    }
    else if (tidx == 9)
    {
        if (KeyPressed(kSkill) && t.skillCd >= 1.0f)
        {
            g_frostFields[idx].active = true;
            g_frostFields[idx].timer = 0.0f;
            g_frostFields[idx].x = t.x;
            g_frostFields[idx].y = t.y;
            t.skillCd = 0.0f;
            PlaySFX(SND_JN10);
        }
        else
        {
            if (t.skillCd < 1.0f)
            { t.skillCd += (1.0f / FROST_CD) * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; }
        }
    }
    else if (tidx == 10)
    {
        if (KeyPressed(kSkill) && t.skillCd >= 1.0f)
        {
            g_hunterRings[idx].active = true;
            g_hunterRings[idx].timer = 0.0f;
            g_hunterRings[idx].x = t.x;
            g_hunterRings[idx].y = t.y;
            for (int j = 0; j < MAX_PLAYERS; j++) g_hunterRings[idx].hit[j] = false;
            t.skillCd = 0.0f;
            PlaySFX(SND_JN11);
        }
        else
        {
            if (t.skillCd < 1.0f)
            { t.skillCd += (1.0f / HUNTER_CD) * g_dt; if (t.skillCd > 1.0f) t.skillCd = 1.0f; }
        }
    }

    float move = 0.0f;
    if (KeyDown(kFwd)) move += 1.0f;
    if (KeyDown(kBack)) move -= 1.0f;
    float strafe = 0.0f;
    if (KeyDown(kStrafeL)) strafe += 1.0f;
    if (KeyDown(kStrafeR)) strafe -= 1.0f;
    float curSpeed = moveSpd * speedMulSelf;
    if (tidx == 0 && t.skillActive) curSpeed *= SKILL1_SPEED_MUL;
    if (t.speedBoostTimer > 0.0f) curSpeed *= SKILL2_PASSIVE_MUL;
    if (t.buffType == BUFF_SPEED && t.buffTimer > 0.0f) curSpeed *= BUFF_SPEED_MUL;
    curSpeed *= slowMul;
    float targetVel = move * curSpeed;
    float k = 1.0f - expf(-TANK_MOVE_EASE * g_dt);
    t.vel += (targetVel - t.vel) * k;
    if (fabs(t.vel) < 1.5f && move == 0.0f) t.vel = 0.0f;
    if (fabs(t.vel) > 0.5f)
    {
        float nx = t.x + cosf(t.bodyAngle) * t.vel * g_dt;
        float ny = t.y + sinf(t.bodyAngle) * t.vel * g_dt;
        bool okX = !CollideWithWall(nx, t.y);
        bool okY = !CollideWithWall(t.x, ny);
        if (okX) t.x = nx;
        if (okY) t.y = ny;
        if (!okX && !okY) t.vel = 0.0f;
    }
    if (!(tidx == 0 && t.skillActive))
    {
        if (strafe != 0.0f)
        {
            float strafeSpeed = curSpeed * STRAFE_SPEED_MUL;
            float ca = cosf(t.bodyAngle), sa = sinf(t.bodyAngle);
            float sdx = sa * strafe, sdy = -ca * strafe;
            float nx = t.x + sdx * strafeSpeed * g_dt;
            float ny = t.y + sdy * strafeSpeed * g_dt;
            bool okX = !CollideWithWall(nx, t.y);
            bool okY = !CollideWithWall(t.x, ny);
            if (okX) t.x = nx;
            if (okY) t.y = ny;
        }
    }
    TryFire(idx, kFire);
}
void UpdateBullets()
{
    float halfSize = TANK_SIZE * 0.5f;
    float halfSize2 = halfSize * halfSize;
    float shieldR2 = SHIELD_RADIUS * SHIELD_RADIUS;
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!g_bullets[i].alive) continue;
        Bullet& b = g_bullets[i];
        float nx = b.x + b.vx * g_dt;
        float ny = b.y + b.vy * g_dt;

        if (nx < 0 || ny < 0 || nx >= WORLD_W || ny >= WORLD_H) { b.alive = false; continue; }

        int cc = (int)floorf(nx / TILE), rr = (int)floorf(ny / TILE);
        bool hitWall = (cc < 0 || rr < 0 || cc >= MAP_W || rr >= MAP_H || g_map[rr][cc] == 1);

        if (hitWall)
        {
            if (b.type == BT_PIERCE)
            {
                if (cc != b.lastCellC || rr != b.lastCellR)
                {
                    b.lastCellC = cc; b.lastCellR = rr;
                    float vlen = sqrtf(b.vx * b.vx + b.vy * b.vy);
                    if (vlen > 0.01f)
                    {
                        float newLen = vlen - PIERCE_SPD_LOSS;
                        if (newLen < 0.0f) newLen = 0.0f;
                        b.vx = b.vx / vlen * newLen;
                        b.vy = b.vy / vlen * newLen;
                    }
                    b.damage -= PIERCE_DMG_LOSS;
                    if (b.damage <= 0 || (b.vx == 0.0f && b.vy == 0.0f)) { b.alive = false; continue; }
                }
            }
            else { b.alive = false; continue; }
        }

        if (cc >= 0 && rr >= 0 && cc < MAP_W && rr < MAP_H)
        {
            bool hitVWall = false;
            for (int w = 0; w < MAX_VWALLS; w++)
            {
                if (!g_vwalls[w].active) continue;
                if (g_vwalls[w].fadingOut) continue;
                if (g_vwalls[w].owner == b.owner) continue;
                if (g_vwalls[w].col != cc || g_vwalls[w].row != rr) continue;
                g_vwalls[w].hp -= b.damage;
                if (g_vwalls[w].hp <= 0.0f) { g_vwalls[w].hp = 0.0f; g_vwalls[w].fadingOut = true; }
                hitVWall = true;
                break;
            }
            if (hitVWall) { b.alive = false; continue; }
        }

        bool hitTank = false;
        for (int j = 0; j < g_playerCount; j++)
        {
            if (j == b.owner) continue;
            if (g_tank[j].hp <= 0) continue;
            if (g_respawnInv[j] > 0.0f) continue;
            float ddx = nx - g_tank[j].x, ddy = ny - g_tank[j].y;
            float d2 = ddx * ddx + ddy * ddy;
            if (g_invincibleTimer > 0.0f)
            { if (d2 < halfSize2) { b.alive = false; hitTank = true; break; } continue; }
            if (g_selTankIdx[j] == 6 && (g_tank[j].phaseTimer > 0.0f || g_tank[j].phaseStuckInWall)) continue;
            int tIdxJ = g_selTankIdx[j];
            if (tIdxJ == 3 && g_tank[j].shieldActive && d2 < shieldR2)
            {
                float bLen = sqrtf(d2);
                if (bLen > 0.5f)
                {
                    float bnx = ddx / bLen, bny = ddy / bLen;
                    float ta = g_tank[j].bodyAngle;
                    float tcx = cosf(ta), tcy = sinf(ta);
                    float dot = bnx * tcx + bny * tcy;
                    if (dot > SHIELD_DOT_THRESHOLD)
                    {
                        g_tank[j].shieldFlashTimer = SHIELD_FLASH_TIME;
                        g_tank[j].skillCd += SKILL4_BLOCK_RECOVER;
                        if (g_tank[j].skillCd > 1.0f) g_tank[j].skillCd = 1.0f;
                        PlaySFX(SND_JN4_3);
                        b.alive = false; hitTank = true; break;
                    }
                }
            }
            if (d2 < halfSize2)
            {
                int ownerIdx = b.owner;
                int ownerTank = g_selTankIdx[ownerIdx];

                int finalDmg = b.damage;
                if (ownerTank == 10)
                {
                    int marks = g_tank[j].marks[ownerIdx];
                    if (marks > 3) marks = 3;
                    finalDmg += HUNTER_MARK_DMG * marks;
                }

                g_tank[j].hp -= finalDmg;
                if (g_tank[j].hp < 0) g_tank[j].hp = 0;
                g_tank[j].hitFlashTimer = HIT_FLASH_TIME;

                if (g_selTankIdx[ownerIdx] == 7)
                {
                    g_tank[j].skillCd -= EMP_SILENCE_CD_CUT;
                    if (g_tank[j].skillCd < 0.0f) g_tank[j].skillCd = 0.0f;
                }

                if (ownerTank == 10)
                {
                    int marks = g_tank[j].marks[ownerIdx];
                    g_tank[j].marks[ownerIdx] = 0;
                    if (marks == 0 && g_tank[j].marks[ownerIdx] < HUNTER_MARK_MAX)
                        g_tank[j].marks[ownerIdx] = 1;
                }
                if (ownerTank == 9)
                {
                    g_tank[j].frostShellTimer = FROST_SHELL_TIME;
                }
                if (ownerTank == 0)
                {
                    g_tank[ownerIdx].skillCd += SKILL1_PASSIVE_CD;
                    if (g_tank[ownerIdx].skillCd > 1.0f) g_tank[ownerIdx].skillCd = 1.0f;
                }
                b.alive = false; hitTank = true;
                PlaySFX(SND_BAOZHA);

                if (g_tank[j].hp <= 0 && g_gameMode == GM_COMPETITIVE)
                {
                    if (b.owner != j) g_score[b.owner]++;
                    g_respawnTimer[j] = 0.0f;
                }
                break;
            }
        }
        if (hitTank) continue;
        b.x = nx; b.y = ny;
    }
}

void UpdateRings()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!g_rings[i].active) continue;
        g_rings[i].timer += g_dt;
        if (g_rings[i].timer >= g_rings[i].duration) g_rings[i].active = false;
    }
}

void TrySpawnBuff()
{
    int slot = -1;
    for (int i = 0; i < MAX_BUFFS; i++) if (!g_buffs[i].active) { slot = i; break; }
    if (slot < 0) return;
    for (int tries = 0; tries < 60; tries++)
    {
        int c = rand() % MAP_W;
        int r = rand() % MAP_H;
        if (g_map[r][c] == 1) continue;
        float wx = c * TILE + TILE * 0.5f;
        float wy = r * TILE + TILE * 0.5f;
        bool overlap = false;
        for (int i = 0; i < MAX_BUFFS; i++)
        {
            if (!g_buffs[i].active) continue;
            float ddx = g_buffs[i].x - wx, ddy = g_buffs[i].y - wy;
            if (ddx*ddx + ddy*ddy < (TILE*3)*(TILE*3)) { overlap = true; break; }
        }
        if (overlap) continue;
        int roll = rand() % 100;
        int type;
        if (roll < 20) type = BUFF_HEAL;
        else if (roll < 50) type = BUFF_SPEED;
        else if (roll < 80) type = BUFF_FIRE;
        else type = BUFF_DAMAGE;
        g_buffs[slot].active = true;
        g_buffs[slot].type = type;
        g_buffs[slot].x = wx; g_buffs[slot].y = wy;
        g_buffs[slot].spawnTimer = 0.0f;
        return;
    }
}

void UpdateBuffSpawns()
{
    g_buffSpawnTimer += g_dt;
    if (g_buffSpawnTimer >= g_buffSpawnNext)
    {
        g_buffSpawnTimer = 0.0f;
        g_buffSpawnNext = 10.0f + (rand() % 200) / 10.0f;
        TrySpawnBuff();
    }
    for (int i = 0; i < MAX_BUFFS; i++)
    {
        if (!g_buffs[i].active) continue;
        g_buffs[i].spawnTimer += g_dt;
    }
}

void ApplyBuff(int tankIdx, int type)
{
    Tank& t = g_tank[tankIdx];
    t.buffType = type;
    switch (type)
    {
        case BUFF_HEAL:   t.buffTotal = BUFF_HEAL_TIME;   t.buffTimer = BUFF_HEAL_TIME;   break;
        case BUFF_SPEED:  t.buffTotal = BUFF_SPEED_TIME;  t.buffTimer = BUFF_SPEED_TIME;  break;
        case BUFF_FIRE:   t.buffTotal = BUFF_FIRE_TIME;   t.buffTimer = BUFF_FIRE_TIME;   break;
        case BUFF_DAMAGE: t.buffTotal = BUFF_DAMAGE_TIME; t.buffTimer = BUFF_DAMAGE_TIME; break;
    }
    t.buffHealAccum = 0.0f;
}

void CheckBuffPickup()
{
    for (int b = 0; b < MAX_BUFFS; b++)
    {
        if (!g_buffs[b].active) continue;
        for (int i = 0; i < g_playerCount; i++)
        {
            if (g_tank[i].hp <= 0) continue;
            float ddx = g_tank[i].x - g_buffs[b].x;
            float ddy = g_tank[i].y - g_buffs[b].y;
            if (ddx*ddx + ddy*ddy < BUFF_PICKUP_R * BUFF_PICKUP_R)
            {
                ApplyBuff(i, g_buffs[b].type);
                g_buffs[b].active = false;
                PlaySFX(SND_SHIQU);
                break;
            }
        }
    }
}

void UpdateBuffEffects()
{
    for (int i = 0; i < g_playerCount; i++)
    {
        Tank& t = g_tank[i];
        if (t.buffType < 0) continue;
        if (t.buffTimer > 0.0f) t.buffTimer -= g_dt;
        if (t.buffTimer <= 0.0f)
        {
            t.buffTimer = 0.0f;
            t.buffType = -1;
            t.buffHealAccum = 0.0f;
            continue;
        }
        if (t.buffType == BUFF_HEAL)
        {
            t.buffHealAccum += BUFF_HEAL_PER_SEC * g_dt;
            while (t.buffHealAccum >= 1.0f)
            {
                t.buffHealAccum -= 1.0f;
                if (t.hp < t.maxHp) t.hp++;
            }
        }
    }
}

void UpdateEmpTimers()
{
    for (int i = 0; i < g_playerCount; i++)
    {
        if (g_selTankIdx[i] != 7) continue;
        if (g_tank[i].empTimer < 0.0f) continue;
        g_tank[i].empTimer += g_dt;
        if (g_tank[i].empTimer >= EMP_TOTAL_TIME) g_tank[i].empTimer = -1.0f;
    }
    for (int i = 0; i < g_playerCount; i++)
    {
        if (g_selTankIdx[i] != 7) continue;
        if (g_tank[i].empTimer < 0.0f) continue;
        float r = GetEmpRadius(g_tank[i]);
        if (r <= 1.0f) continue;
        for (int j = 0; j < g_playerCount; j++)
        {
            if (j == i) continue;
            if (g_tank[j].hp <= 0) continue;
            float ddx = g_tank[j].x - g_tank[i].x;
            float ddy = g_tank[j].y - g_tank[i].y;
            if (ddx*ddx + ddy*ddy >= r*r) continue;
            InterruptSkillByEmp(j);
        }
    }
}

void HandleRespawn()
{
    for (int i = 0; i < g_playerCount; i++)
    {
        if (g_respawnInv[i] > 0.0f)
        {
            g_respawnInv[i] -= g_dt;
            if (g_respawnInv[i] < 0.0f) g_respawnInv[i] = 0.0f;
        }
        if (g_tank[i].hp <= 0 && g_gameMode == GM_COMPETITIVE)
        {
            g_respawnTimer[i] += g_dt;
            if (g_respawnTimer[i] >= COMP_RESPAWN_TIME)
            {
                ResetTankForRespawn(i);
                g_respawnTimer[i] = 0.0f;
            }
        }
    }
}

void UpdatePvP()
{
    HandleRespawn();
    UpdateTank(0, P1_FWD, P1_BACK, P1_LEFT, P1_RIGHT, P1_FIRE, P1_SKILL, P1_STRAFEL, P1_STRAFER);
    if (g_isSinglePlayer) UpdateAITank(1);
    else UpdateTank(1, P2_FWD, P2_BACK, P2_LEFT, P2_RIGHT, P2_FIRE, P2_SKILL, P2_STRAFEL, P2_STRAFER);
    if (g_playerCount >= 3)
        UpdateTank(2, P3_FWD, P3_BACK, P3_LEFT, P3_RIGHT, P3_FIRE, P3_SKILL, P3_STRAFEL, P3_STRAFER);

    g_ignoreVWallOwner = -1;
    UpdateEmpTimers();
    UpdateVirtualWalls();
    UpdateFrostFields();
    UpdateHunterRings();
    UpdateBuffSpawns();
    CheckBuffPickup();
    UpdateBuffEffects();

    g_ignoreVWallOwner = -1;
    for (int a = 0; a < g_playerCount; a++)
    {
        for (int b = a + 1; b < g_playerCount; b++)
        {
            if (g_tank[a].hp <= 0 || g_tank[b].hp <= 0) continue;
            float dx = g_tank[b].x - g_tank[a].x;
            float dy = g_tank[b].y - g_tank[a].y;
            float d2 = dx * dx + dy * dy;
            if (d2 < 0.0001f) { dx = 1.0f; dy = 0.0f; d2 = 1.0f; }
            float d = sqrtf(d2), minD = TANK_SIZE;
            if (d < minD)
            {
                float overlap = (minD - d) * 0.5f;
                float nx = dx / d, ny = dy / d;
                float nax = g_tank[a].x - nx * overlap, nay = g_tank[a].y - ny * overlap;
                float nbx = g_tank[b].x + nx * overlap, nby = g_tank[b].y + ny * overlap;
                int saveA = g_ignoreVWallOwner;
                g_ignoreVWallOwner = a;
                bool okA = !CollideWithWall(nax, nay);
                g_ignoreVWallOwner = b;
                bool okB = !CollideWithWall(nbx, nby);
                g_ignoreVWallOwner = saveA;
                if (okA) { g_tank[a].x = nax; g_tank[a].y = nay; }
                if (okB) { g_tank[b].x = nbx; g_tank[b].y = nby; }
            }
        }
    }

    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        Tank& t = g_tank[i];
        float diff = t.shadowHp - (float)t.hp;
        if (diff > 0.001f)
        {
            float sc = diff / HP_SHADOW_RANGE;
            if (sc > 1.0f) sc = 1.0f;
            t.shadowHp -= HP_SHADOW_SPD * sc * g_dt;
            if (t.shadowHp < (float)t.hp) t.shadowHp = (float)t.hp;
        }
        else t.shadowHp = (float)t.hp;
    }

    UpdateBullets();
    UpdateRings();
    UpdateAfterImages();
    UpdateCamera();

    if (g_invincibleTimer > 0.0f)
    { g_invincibleTimer -= g_dt; if (g_invincibleTimer < 0.0f) g_invincibleTimer = 0.0f; }

    for (int i = 0; i < g_playerCount; i++)
    {
        if (g_tank[i].cdFullFlashTimer > 0.0f) g_tank[i].cdFullFlashTimer -= g_dt;
        int tidx = g_selTankIdx[i];
        if (tidx == 1 || tidx == 2)
        {
            if (g_prevSkillCd[i] < 0.999f && g_tank[i].skillCd >= 0.999f)
                g_tank[i].cdFullFlashTimer = CD_FULL_FLASH_TIME;
        }
        g_prevSkillCd[i] = g_tank[i].skillCd;
    }

    if (g_gameMode == GM_DEATHMATCH || g_gameMode == GM_SINGLE)
    {
        int aliveCount = 0;
        for (int i = 0; i < g_playerCount; i++) if (g_tank[i].hp > 0) aliveCount++;
        if (aliveCount <= 1)
        {
            PlaySFX(SND_WIN);
            g_state = ST_PVP_OVER; g_overTimer = 0.0f; g_overMaskA = 0.0f;
            ResetKeys();
        }
    }
    else if (g_gameMode == GM_COMPETITIVE)
    {
        for (int i = 0; i < g_playerCount; i++)
        {
            if (g_score[i] >= g_winsumTarget)
            {
                PlaySFX(SND_WIN);
                g_state = ST_PVP_OVER; g_overTimer = 0.0f; g_overMaskA = 0.0f;
                ResetKeys();
                break;
            }
        }
    }
}

void UpdateCountdown()
{
    g_countTimer += g_dt;
    UpdateCamera();
    if (g_countTimer >= 4.0f) { g_state = ST_PVP_PLAY; g_countTimer = 0.0f; }
}

void UpdatePvpOver()
{
    g_overTimer += g_dt;
    float targetA = 200.0f;
    float k = 1.0f - expf(-4.0f * g_dt);
    g_overMaskA += (targetA - g_overMaskA) * k;
    if (g_overMaskA > targetA) g_overMaskA = targetA;
    if (KeyPressed(VK_SPACE)) { PlaySFX(SND_UI); StartTransition(ST_MENU); return; }
    if (g_overTimer >= 3.0f) StartTransition(ST_PVP_COUNT);
}

void DrawMap()
{
    float invS = 1.0f / g_camScale;
    float wl = g_camX - WIN_W * 0.5f * invS, wr = g_camX + WIN_W * 0.5f * invS;
    float wt = g_camY - WIN_H * 0.5f * invS, wb = g_camY + WIN_H * 0.5f * invS;
    int c0 = (int)floorf(wl / TILE) - 1, c1 = (int)floorf(wr / TILE) + 1;
    int r0 = (int)floorf(wt / TILE) - 1, r1 = (int)floorf(wb / TILE) + 1;
    if (c0 < 0) c0 = 0; if (r0 < 0) r0 = 0;
    if (c1 >= MAP_W) c1 = MAP_W - 1; if (r1 >= MAP_H) r1 = MAP_H - 1;
    for (int r = r0; r <= r1; r++)
    {
        for (int c = c0; c <= c1; c++)
        {
            IMAGE* tex = (g_map[r][c] == 1) ? &g_mapWall[g_selMapIdx] : &g_mapFloor[g_selMapIdx];
            int tw = tex->getwidth(), th = tex->getheight();
            if (tw <= 0 || th <= 0) continue;
            float fSx = WorldToScreenX(c * (float)TILE);
            float fSy = WorldToScreenY(r * (float)TILE);
            float fEx = WorldToScreenX((c + 1) * (float)TILE);
            float fEy = WorldToScreenY((r + 1) * (float)TILE);
            int sx = (int)floorf(fSx), sy = (int)floorf(fSy);
            int ex = (int)ceilf(fEx), ey = (int)ceilf(fEy);
            int w = ex - sx, h = ey - sy;
            if (w <= 0 || h <= 0) continue;
            DrawSpriteStretch(tex, sx, sy, w, h);
        }
    }
}

void DrawRings()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!g_rings[i].active) continue;
        float t = g_rings[i].timer / g_rings[i].duration;
        if (t > 1.0f) t = 1.0f;
        float maxWorldR = SKILL3_RANGE * 1.1f;
        float r = maxWorldR * t;
        int alpha = (int)(220.0f * (1.0f - t));
        if (alpha < 0) alpha = 0;
        float sx = WorldToScreenX(g_rings[i].x);
        float sy = WorldToScreenY(g_rings[i].y);
        float scrR = r * g_camScale;
        float thick = 10.0f * g_camScale;
        if (thick < 2.0f) thick = 2.0f;
        DrawCircleRing(sx, sy, scrR, thick, alpha, 255, 255, 255);
    }
}

void DrawEmpFields()
{
    for (int i = 0; i < g_playerCount; i++)
    {
        Tank& t = g_tank[i];
        if (g_selTankIdx[i] != 7) continue;
        if (t.empTimer < 0.0f) continue;
        float r = GetEmpRadius(t);
        if (r <= 1.0f) continue;
        float sx = WorldToScreenX(t.x);
        float sy = WorldToScreenY(t.y);
        float sr = r * g_camScale;
        float lifeA = 1.0f;
        if (t.empTimer < EMP_EXPAND_TIME) lifeA = t.empTimer / EMP_EXPAND_TIME;
        else if (t.empTimer > EMP_EXPAND_TIME + EMP_HOLD_TIME)
            lifeA = 1.0f - (t.empTimer - EMP_EXPAND_TIME - EMP_HOLD_TIME) / EMP_SHRINK_TIME;
        if (lifeA < 0.0f) lifeA = 0.0f;
        if (lifeA > 1.0f) lifeA = 1.0f;
        int fillA = (int)(70.0f * lifeA);
        if (fillA > 0) DrawCircleRing(sx, sy, sr * 0.5f, sr, fillA, 150, 210, 255);
        int ringA = (int)(240.0f * lifeA);
        if (ringA > 0) DrawCircleRing(sx, sy, sr, 10.0f, ringA, 30, 110, 230);
    }
}

void DrawFrostFields()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!g_frostFields[i].active) continue;
        float r = GetFrostRadius(g_frostFields[i]);
        if (r <= 1.0f) continue;
        float sx = WorldToScreenX(g_frostFields[i].x);
        float sy = WorldToScreenY(g_frostFields[i].y);
        float sr = r * g_camScale;
        int fillA = 70;
        DrawCircleRing(sx, sy, sr * 0.5f, sr, fillA, 180, 220, 255);
        int ringA = 240;
        DrawCircleRing(sx, sy, sr, 8.0f, ringA, 100, 180, 240);
    }
}

void DrawHunterRings()
{
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (!g_hunterRings[i].active) continue;
        float r = GetHunterRingRadius(g_hunterRings[i]);
        if (r <= 1.0f) continue;
        int alpha = GetHunterRingAlpha(g_hunterRings[i]);
        if (alpha <= 0) continue;
        float sx = WorldToScreenX(g_hunterRings[i].x);
        float sy = WorldToScreenY(g_hunterRings[i].y);
        float sr = r * g_camScale;
        DrawCircleRing(sx, sy, sr, 14.0f, alpha, 20, 130, 40);
    }
}

int GetBuffAlpha(float spawnTimer)
{
    if (spawnTimer < BUFF_FADE_IN) return (int)(255.0f * (spawnTimer / BUFF_FADE_IN));
    float blinkStart = BUFF_FADE_IN;
    float blinkEnd = blinkStart + BUFF_BLINK_COUNT * BUFF_BLINK_CYCLE;
    if (spawnTimer < blinkEnd)
    {
        float bt = spawnTimer - blinkStart;
        int ph = (int)(bt / BUFF_BLINK_CYCLE);
        float pt = bt - ph * BUFF_BLINK_CYCLE;
        if (pt < 0.10f) return 0;
        return 255;
    }
    return 255;
}

void DrawBuffDrops()
{
    for (int i = 0; i < MAX_BUFFS; i++)
    {
        BuffDrop& b = g_buffs[i];
        if (!b.active) continue;
        int alpha = GetBuffAlpha(b.spawnTimer);
        if (alpha <= 0) continue;
        float sx = WorldToScreenX(b.x);
        float sy = WorldToScreenY(b.y);
        if (sx < -200 || sx > WIN_W + 200 || sy < -200 || sy > WIN_H + 200) continue;
        IMAGE* tex = &g_buffTex[b.type];
        int tw = tex->getwidth();
        float sc = (tw > 0) ? (BUFF_SIZE * g_camScale / (float)tw) : 1.0f;
        float prev = g_globalAlpha;
        g_globalAlpha = (float)alpha;
        DrawSprite(tex, sx, sy, sc, 0.0f);
        g_globalAlpha = prev;
    }
}

void DrawVirtualWalls()
{
    for (int i = 0; i < MAX_VWALLS; i++)
    {
        if (!g_vwalls[i].active) continue;
        if (g_vwalls[i].alpha <= 0.0f) continue;
        float fsx = WorldToScreenX(g_vwalls[i].col * (float)TILE);
        float fsy = WorldToScreenY(g_vwalls[i].row * (float)TILE);
        float fex = WorldToScreenX((g_vwalls[i].col + 1) * (float)TILE);
        float fey = WorldToScreenY((g_vwalls[i].row + 1) * (float)TILE);
        int x = (int)floorf(fsx), y = (int)floorf(fsy);
        int w = (int)ceilf(fex) - x, h = (int)ceilf(fey) - y;
        if (w <= 0 || h <= 0) continue;
        if (x + w <= 0 || x >= WIN_W) continue;
        if (y + h <= 0 || y >= WIN_H) continue;
        float prev = g_globalAlpha;
        g_globalAlpha = g_vwalls[i].alpha * 255.0f;
        DrawSpriteStretch(&g_qiangtiTex, x, y, w, h);
        g_globalAlpha = prev;
    }
}

bool IsTankFlashWhite(float timer)
{
    if (timer <= 0.0f) return false;
    float elapsed = HIT_FLASH_TIME - timer;
    float seg = HIT_FLASH_TIME / (float)(HIT_FLASH_COUNT * 2);
    int phase = (int)(elapsed / seg);
    return (phase % 2) == 0;
}
bool IsSkillFlashWhite(float timer) { return timer > 0.0f; }

void DrawFrostMaskOnTank(float sx, float sy, float sc, float factor)
{
    if (factor >= 0.999f) return;
    float t = (1.0f - factor) / (1.0f - FROST_MIN_FACTOR);
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    int alpha = (int)(40.0f + 80.0f * t);
    int radius = (int)(TANK_SIZE * 0.5f * g_camScale);
    DWORD* buf = GetImageBuffer(NULL);
    if (!buf) return;
    int x0 = (int)(sx - radius), y0 = (int)(sy - radius);
    int x1 = (int)(sx + radius), y1 = (int)(sy + radius);
    if (x0 < 0) x0 = 0; if (y0 < 0) y0 = 0;
    if (x1 > WIN_W) x1 = WIN_W; if (y1 > WIN_H) y1 = WIN_H;
    if (x0 >= x1 || y0 >= y1) return;
    float r2 = (float)(radius * radius);
    int ia = 255 - alpha;
    for (int yy = y0; yy < y1; yy++)
    {
        DWORD* row = buf + yy * WIN_W;
        float dy = yy - sy, dy2 = dy * dy;
        for (int xx = x0; xx < x1; xx++)
        {
            float dx = xx - sx, d2 = dx * dx + dy2;
            if (d2 > r2) continue;
            DWORD c = row[xx];
            int cr = (c >> 16) & 0xFF, cg = (c >> 8) & 0xFF, cb = c & 0xFF;
            row[xx] = 0xFF000000 | (((180 * alpha + cr * ia) / 255) << 16)
                    | (((220 * alpha + cg * ia) / 255) << 8) | ((255 * alpha + cb * ia) / 255);
        }
    }
}

void DrawHunterMarksOnTank(int tankIdx, float sx, float sy, float angle)
{
    int maxMarks = 0;
    for (int k = 0; k < g_playerCount; k++)
    {
        int m = g_tank[tankIdx].marks[k];
        if (g_selTankIdx[k] != 10) continue;
        if (m > maxMarks) maxMarks = m;
    }
    if (maxMarks <= 0) return;
    int alpha = MARK_ALPHA_1;
    if (maxMarks == 1) alpha = MARK_ALPHA_1;
    else if (maxMarks == 2) alpha = MARK_ALPHA_2;
    else alpha = MARK_ALPHA_3;
    float size = TANK_SIZE * 1.05f * g_camScale;
    DrawTriangleAlpha(sx, sy, angle, size, alpha, MARK_RGB_R, MARK_RGB_G, MARK_RGB_B);
}

void DrawTanks()
{
    for (int i = 0; i < g_playerCount; i++)
    {
        Tank& t = g_tank[i];
        int tidx = g_selTankIdx[i];
        float sx = WorldToScreenX(t.x);
        float sy = WorldToScreenY(t.y);
        if (sx < -300 || sx > WIN_W + 300 || sy < -300 || sy > WIN_H + 300) continue;

        if (t.hp <= 0)
        {
            int tw = g_tankTex[tidx].getwidth();
            float sc = (tw > 0) ? (TANK_SIZE * g_camScale / (float)tw) : 1.0f;
            g_tintDark = true;
            DrawSprite(&g_tankTex[tidx], sx, sy, sc, t.bodyAngle);
            g_tintDark = false;
            continue;
        }

        if (tidx == 4 && t.hookState != 0)
        {
            float hx = WorldToScreenX(t.hookX);
            float hy = WorldToScreenY(t.hookY);
            DrawHookLine(sx, sy, hx, hy);
            if (g_gousuoTex.getwidth() > 0)
            {
                float headSc = (TANK_SIZE * 0.35f) * g_camScale / (float)g_gousuoTex.getwidth();
                float headAngle = t.hookAngle + PI * 0.25f;
                DrawSprite(&g_gousuoTex, hx, hy, headSc, headAngle);
            }
        }

        DrawHunterMarksOnTank(i, sx, sy, t.bodyAngle);

        int tw = g_tankTex[tidx].getwidth();
        float sc = (tw > 0) ? (TANK_SIZE * g_camScale / (float)tw) : 1.0f;
        if (g_invincibleTimer > 0.0f || g_respawnInv[i] > 0.0f)
        {
            float invR = TANK_SIZE * 0.72f * g_camScale;
            DrawInvincibleShield(sx, sy, invR);
        }
        if (tidx == 3 && t.shieldActive)
        {
            float shieldR = SHIELD_RADIUS * g_camScale;
            bool sflash = (t.shieldFlashTimer > 0.0f);
            DrawShieldArc(sx, sy, shieldR, t.bodyAngle, sflash);
        }

        bool white = IsTankFlashWhite(t.hitFlashTimer) || IsSkillFlashWhite(t.skillFlashTimer);
        bool dark = (t.stunTimer > 0.0f);

        float prevAlpha = g_globalAlpha;
        if (tidx == 6 && t.phaseTimer > 0.0f)
        {
            float alpha = 1.0f - t.phaseAlpha * 0.5f;
            g_globalAlpha = 255.0f * alpha;
        }

        int prevTintD = g_tintDark;
        g_tintWhite = white; g_tintDark = dark;
        DrawSprite(&g_tankTex[tidx], sx, sy, sc, t.bodyAngle);
        g_globalAlpha = prevAlpha;
        g_tintWhite = false; g_tintDark = prevTintD;

        if (t.slowFactor < 0.999f) DrawFrostMaskOnTank(sx, sy, sc, t.slowFactor);

        if (t.muzzleTimer > 0.0f)
        {
            float ma = t.bodyAngle;
            float mx = sx + cosf(ma) * TANK_SIZE * 0.55f * g_camScale;
            float my = sy + sinf(ma) * TANK_SIZE * 0.55f * g_camScale;
            int mw = g_tankMuzzle[tidx].getwidth();
            float mScale = (mw > 0) ? (TANK_SIZE * 0.7f * g_camScale / (float)mw) : 1.0f;
            DrawSprite(&g_tankMuzzle[tidx], mx, my, mScale, ma);
        }
    }
}

void DrawBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!g_bullets[i].alive) continue;
        int tidx = g_selTankIdx[g_bullets[i].owner];
        float sx = WorldToScreenX(g_bullets[i].x);
        float sy = WorldToScreenY(g_bullets[i].y);
        int bw = g_tankBullet[tidx].getwidth();
        float bScale = (bw > 0) ? (BULLET_SIZE * g_camScale / (float)bw) : 1.0f;
        DrawSprite(&g_tankBullet[tidx], sx, sy, bScale, g_bullets[i].angle);
    }
}

void DrawHealthBar(int x, int y, int w, int h, int hp, float shadowHp, int maxHp,
                   const char* label, int playerIdx)
{
    setfillcolor(RGB(90, 0, 0));
    solidrectangle(x - 4, y - 4, x + w + 4, y + h + 4);
    setfillcolor(RGB(35, 35, 35)); solidrectangle(x, y, x + w, y + h);
    float ratio = (maxHp > 0) ? (float)hp / (float)maxHp : 0.0f;
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;
    float sRatio = (maxHp > 0) ? shadowHp / (float)maxHp : 0.0f;
    if (sRatio < ratio) sRatio = ratio;
    if (sRatio > 1.0f) sRatio = 1.0f;
    int fillW = (int)(w * ratio), shadowW = (int)(w * sRatio);
    if (shadowW > 0) { setfillcolor(RGB(120, 20, 20)); solidrectangle(x, y, x + shadowW, y + h); }
    if (fillW > 0) { setfillcolor(RGB(220, 45, 45)); solidrectangle(x, y, x + fillW, y + h); }

    settextstyle(16, 0, FONT_NAME); setbkmode(TRANSPARENT); settextcolor(WHITE);

    if (g_gameMode == GM_COMPETITIVE && hp <= 0 && g_respawnTimer[playerIdx] > 0.0f)
    {
        int remain = (int)ceilf(COMP_RESPAWN_TIME - g_respawnTimer[playerIdx]);
        if (remain < 0) remain = 0;
        char buf[32];
        sprintf(buf, "%d", remain);
        int tw = textwidth(buf), th = textheight(buf);
        outtextxy(x + w / 2 - tw / 2, y + h / 2 - th / 2, buf);
    }
    else
    {
        char buf[32]; sprintf(buf, "%s/HP", label);
        int tw = textwidth(buf), th = textheight(buf);
        outtextxy(x + w / 2 - tw / 2, y + h / 2 - th / 2, buf);
    }
}

void DrawSkillBar(int x, int y, int w, int h, float cd, bool drawMark, float flashTimer, bool silenced)
{
    setfillcolor(RGB(40, 40, 40)); solidrectangle(x, y, x + w, y + h);
    int fillW = (int)(w * cd);
    if (fillW > 0)
    {
        if (silenced) setfillcolor(RGB(80, 160, 255));
        else
        {
            bool whitePhase = false;
            if (flashTimer > 0.0f)
            {
                float elapsed = CD_FULL_FLASH_TIME - flashTimer;
                if (elapsed < 0.0f) elapsed = 0.0f;
                int phase = (int)(elapsed / CD_FULL_FLASH_SEG);
                if (phase % 2 == 0) whitePhase = true;
            }
            if (whitePhase) setfillcolor(RGB(255, 255, 255));
            else setfillcolor(RGB(180, 180, 180));
        }
        solidrectangle(x, y, x + fillW, y + h);
    }
    if (drawMark && !silenced)
    {
        int markX = x + (int)(w * SKILL1_ACTIVATE_TH);
        setlinecolor(RGB(255, 255, 255)); line(markX, y, markX, y + h);
    }
    setlinecolor(RGB(230, 230, 230)); rectangle(x, y, x + w, y + h);
}

void DrawHeatBar(int x, int y, int w, int h, float heat, bool overheated, bool dead,
                 bool frenzyActive, bool silenced)
{
    setfillcolor(RGB(40, 40, 40)); solidrectangle(x, y, x + w, y + h);
    if (!dead)
    {
        int fillW = (int)(w * (heat / HEAT_MAX));
        if (fillW > 0)
        {
            if (silenced) setfillcolor(RGB(80, 160, 255));
            else
            {
                int r, g, b;
                float t = heat / HEAT_MAX;
                if (overheated) { r = 255; g = 0; b = 0; }
                else if (t < 0.25f) { r = 255; g = 255; b = 255; }
                else if (t < 0.5f) { r = 255; g = 255; b = 0; }
                else if (t < 0.75f) { r = 255; g = 140; b = 0; }
                else { r = 255; g = 40; b = 0; }
                setfillcolor(RGB(r, g, b));
            }
            solidrectangle(x, y, x + fillW, y + h);
        }
    }
    if (frenzyActive) setlinecolor(RGB(255, 60, 60));
    else setlinecolor(RGB(230, 230, 230));
    rectangle(x, y, x + w, y + h);
}

void DrawInvincibleBar()
{
    if (g_invincibleTimer <= 0.0f) return;
    int barW = 360, barH = 32;
    int barX = (WIN_W - barW) / 2, barY = WIN_H - 90;
    setfillcolor(RGB(60, 80, 120)); solidrectangle(barX - 3, barY - 3, barX + barW + 3, barY + barH + 3);
    setfillcolor(RGB(70, 70, 70)); solidrectangle(barX, barY, barX + barW, barY + barH);
    float ratio = g_invincibleTimer / INVINCIBLE_TIME;
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;
    int fillW = (int)(barW * ratio);
    if (fillW > 0) { setfillcolor(RGB(80, 150, 230)); solidrectangle(barX, barY, barX + fillW, barY + barH); }
    settextstyle(20, 0, FONT_NAME); setbkmode(TRANSPARENT); settextcolor(WHITE);
    const char* txt = "无敌时间";
    int tw = textwidth(txt), th = textheight(txt);
    outtextxy(barX + barW / 2 - tw / 2, barY + barH / 2 - th / 2, txt);
}

void DrawBuffBar(int i, int x, int y, int w, int h)
{
    Tank& t = g_tank[i];
    if (t.buffType < 0 || t.buffTimer <= 0.0f) return;
    setfillcolor(RGB(60, 60, 60));
    solidrectangle(x, y, x + w, y + h);
    float ratio = (t.buffTotal > 0.0f) ? (t.buffTimer / t.buffTotal) : 0.0f;
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;
    int fillW = (int)(w * ratio);
    if (fillW > 0)
    {
        setfillcolor(RGB(255, 255, 255));
        solidrectangle(x, y, x + fillW, y + h);
    }
    setlinecolor(RGB(180, 180, 180));
    rectangle(x, y, x + w, y + h);
    settextstyle(14, 0, FONT_NAME);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    const char* name = "";
    switch (t.buffType)
    {
        case BUFF_HEAL:   name = "修理包"; break;
        case BUFF_SPEED:  name = "移速加快"; break;
        case BUFF_FIRE:   name = "射速加快"; break;
        case BUFF_DAMAGE: name = "伤害提升"; break;
    }
    int tw = textwidth(name), th = textheight(name);
    outtextxy(x + w / 2 - tw / 2, y + h / 2 - th / 2, name);
}

void DrawTankCDBar(int i, int x, int y, int w, int h, bool dead)
{
    int tidx = g_selTankIdx[i];
    Tank& t = g_tank[i];
    bool silenced = IsTankSilenced(i);
    if (tidx == 5) DrawHeatBar(x, y, w, h, t.heat, t.overheated, dead, t.frenzyTimer > 0.0f, silenced);
    else
    {
        float flash = (tidx == 1 || tidx == 2) ? t.cdFullFlashTimer : 0.0f;
        bool mark = (tidx == 0);
        if (dead)
        {
            setfillcolor(RGB(40, 40, 40)); solidrectangle(x, y, x + w, y + h);
            setlinecolor(RGB(230, 230, 230)); rectangle(x, y, x + w, y + h);
        }
        else DrawSkillBar(x, y, w, h, t.skillCd, mark, flash, silenced);
    }
    if (silenced && !dead)
    {
        settextstyle(14, 0, FONT_NAME);
        setbkmode(TRANSPARENT);
        settextcolor(RGB(10, 30, 90));
        const char* warn = "受干扰无法使用";
        int tw = textwidth(warn), th = textheight(warn);
        float jx = sinf(g_animTime * 41.0f) * 1.6f + sinf(g_animTime * 137.0f) * 0.7f;
        float jy = cosf(g_animTime * 53.0f) * 1.1f + cosf(g_animTime * 149.0f) * 0.5f;
        int blinkPhase = (int)(g_animTime * 8.0f) % 6;
        if (blinkPhase == 0) jx += (rand() % 7 - 3) * 0.7f;
        if (blinkPhase == 2) jy += (rand() % 5 - 2) * 0.6f;
        if (blinkPhase == 4) { jx += (rand() % 5 - 2) * 0.5f; jy += (rand() % 5 - 2) * 0.4f; }
        int tx = x + w / 2 - tw / 2 + (int)jx;
        int ty = y + h / 2 - th / 2 + (int)jy;
        outtextxy(tx, ty, warn);
    }
}

void DrawHUD()
{
    if (g_playerCount == 2)
    {
        DrawHealthBar(15, 15, 250, 26, g_tank[0].hp, g_tank[0].shadowHp, g_tank[0].maxHp, "P1", 0);
        const char* p2Label = g_isSinglePlayer ? "AI" : "P2";
        DrawHealthBar(WIN_W - 15 - 250, 15, 250, 26, g_tank[1].hp, g_tank[1].shadowHp, g_tank[1].maxHp, p2Label, 1);
    }
    else
    {
        int bw = 200, bh = 26, gap = 20;
        int total = 3 * bw + 2 * gap;
        int startX = (WIN_W - total) / 2;
        for (int i = 0; i < 3; i++)
        {
            char label[8]; sprintf(label, "P%d", i + 1);
            DrawHealthBar(startX + i * (bw + gap), 15, bw, bh,
                          g_tank[i].hp, g_tank[i].shadowHp, g_tank[i].maxHp, label, i);
        }
    }
    int barW = 220, barH = 18;
    int barY = WIN_H - 15 - barH;
    if (g_playerCount == 2)
    {
        DrawTankCDBar(0, 15, barY, barW, barH, g_tank[0].hp <= 0);
        DrawTankCDBar(1, WIN_W - 15 - barW, barY, barW, barH, g_tank[1].hp <= 0);
    }
    else
    {
        int bw2 = 180;
        int total = 3 * bw2 + 2 * 20;
        int startX = (WIN_W - total) / 2;
        for (int i = 0; i < 3; i++)
            DrawTankCDBar(i, startX + i * (bw2 + 20), barY, bw2, barH, g_tank[i].hp <= 0);
    }
    int buffH = 16, buffGap = 4;
    if (g_playerCount == 2)
    {
        DrawBuffBar(0, 15, barY - buffGap - buffH, barW / 2, buffH);
        DrawBuffBar(1, WIN_W - 15 - barW / 2, barY - buffGap - buffH, barW / 2, buffH);
    }
    else
    {
        int bw2 = 180;
        int total = 3 * bw2 + 2 * 20;
        int startX = (WIN_W - total) / 2;
        for (int i = 0; i < 3; i++)
            DrawBuffBar(i, startX + i * (bw2 + 20), barY - buffGap - buffH, bw2 / 2, buffH);
    }
    DrawInvincibleBar();
}

void DrawCompetitivePanel()
{
    if (g_gameMode != GM_COMPETITIVE) return;
    int panelX = WIN_W - RANK_PANEL_W - 10;
    int panelY = 60;
    FillRectAlpha(panelX, panelY, RANK_PANEL_W, RANK_PANEL_H, 128);

    setbkmode(TRANSPARENT);
    settextstyle(16, 0, FONT_NAME);
    settextcolor(RGB(160, 160, 160));
    outtextxy(panelX + 12, panelY + 6, "玩家");
    outtextxy(panelX + RANK_PANEL_W - 52, panelY + 6, "得分");

    settextstyle(14, 0, FONT_NAME);
    settextcolor(RGB(160, 160, 160));
    char tbuf[32];
    sprintf(tbuf, "达到%d分获胜", g_winsumTarget);
    outtextxy(panelX + 12, panelY + RANK_PANEL_H - 40, tbuf);

    int order[MAX_PLAYERS] = {0, 1, 2};
    for (int i = 0; i < g_playerCount; i++)
        for (int j = i + 1; j < g_playerCount; j++)
            if (g_score[order[j]] > g_score[order[i]])
            { int tmp = order[i]; order[i] = order[j]; order[j] = tmp; }

    float targetY[MAX_PLAYERS];
    for (int slot = 0; slot < g_playerCount; slot++)
        targetY[order[slot]] = (float)(panelY + 30 + slot * RANK_ROW_H);

    for (int i = 0; i < g_playerCount; i++)
    {
        float desired = targetY[i];
        float diff = desired - g_rankPlayerY[i];
        if (fabsf(diff) < 0.5f) g_rankPlayerY[i] = desired;
        else g_rankPlayerY[i] += diff * (1.0f - expf(-6.0f * g_dt));
    }

    for (int i = 0; i < g_playerCount; i++)
    {
        int y = (int)g_rankPlayerY[i];
        const char* name;
        char nbuf[8];
        if (i == 0) name = "P1";
        else if (i == 1) name = (g_isSinglePlayer ? "AI" : "P2");
        else name = "P3";
        if (order[0] == i) settextcolor(WHITE);
        else settextcolor(RGB(200, 200, 200));
        settextstyle(16, 0, FONT_NAME);
        outtextxy(panelX + 12, y, name);
        sprintf(nbuf, "%d", g_score[i]);
        int nw = textwidth(nbuf);
        outtextxy(panelX + RANK_PANEL_W - 22 - nw, y, nbuf);
    }
}

void RenderGameWorld()
{
    DrawMap();
    DrawVirtualWalls();
    DrawRings();
    DrawEmpFields();
    DrawFrostFields();
    DrawHunterRings();
    DrawBuffDrops();
    DrawBullets();
    DrawAfterImages();
    DrawTanks();
    DrawHUD();
    DrawCompetitivePanel();
}

void DrawCountdown()
{
    FillRectAlpha(0, 0, WIN_W, WIN_H, 120);
    char buf[16];
    if (g_countTimer < 1.0f) strcpy(buf, "3");
    else if (g_countTimer < 2.0f) strcpy(buf, "2");
    else if (g_countTimer < 3.0f) strcpy(buf, "1");
    else strcpy(buf, "Fight!");
    settextstyle(96, 0, FONT_NAME); setbkmode(TRANSPARENT); settextcolor(WHITE);
    int tw = textwidth(buf), th = textheight(buf);
    outtextxy(WIN_W / 2 - tw / 2, WIN_H / 2 - th / 2, buf);
}

void DrawPvpOver()
{
    RenderGameWorld();
    if (g_overMaskA > 1.0f) FillRectAlpha(0, 0, WIN_W, WIN_H, (int)g_overMaskA);
    setbkmode(TRANSPARENT); settextcolor(WHITE);
    char buf[64];
    if (g_gameMode == GM_COMPETITIVE)
    {
        int best = -1, bestScore = -1;
        for (int i = 0; i < g_playerCount; i++)
        {
            if (g_score[i] > bestScore) { bestScore = g_score[i]; best = i; }
        }
        if (best == 1 && g_isSinglePlayer) sprintf(buf, "AI 获胜");
        else sprintf(buf, "P%d 获胜", best + 1);
    }
    else
    {
        int aliveCount = 0, aliveIdx = -1;
        for (int i = 0; i < g_playerCount; i++) if (g_tank[i].hp > 0) { aliveCount++; aliveIdx = i; }
        if (aliveCount == 0) strcpy(buf, "平局");
        else
        {
            if (aliveIdx == 1 && g_isSinglePlayer) sprintf(buf, "AI 获胜");
            else sprintf(buf, "P%d 获胜", aliveIdx + 1);
        }
    }
    settextstyle(72, 0, FONT_NAME);
    int tw = textwidth(buf), th = textheight(buf);
    outtextxy(WIN_W / 2 - tw / 2, (int)(WIN_H * 0.38f) - th / 2, buf);
    settextstyle(28, 0, FONT_NAME);
    const char* l1 = "3秒后重新开始";
    const char* l2 = "按下空格键返回主页";
    tw = textwidth(l1); th = textheight(l1);
    outtextxy(WIN_W / 2 - tw / 2, (int)(WIN_H * 0.72f) - th / 2, l1);
    tw = textwidth(l2); th = textheight(l2);
    outtextxy(WIN_W / 2 - tw / 2, (int)(WIN_H * 0.82f) - th / 2, l2);
}

void UpdateTransition()
{
    g_transTimer += g_dt;
    if (g_state == ST_TRANS_IN)
    { if (g_transTimer >= 0.5f) { g_state = ST_TRANS_HOLD; g_transTimer = 0.0f; } }
    else if (g_state == ST_TRANS_HOLD)
    {
        if (g_transTimer >= 0.5f)
        { if (g_transNext == ST_PVP_COUNT) InitGame(); g_state = ST_TRANS_OUT; g_transTimer = 0.0f; }
    }
    else if (g_state == ST_TRANS_OUT)
    {
        if (g_transTimer >= 0.5f)
        { g_state = g_transNext; g_transTimer = 0.0f; ResetKeys(); }
    }
}

void DrawTransitionOverlay()
{
    int left = 0, right = 0, maskA = 0;
    if (g_state == ST_TRANS_IN)
    {
        float t = g_transTimer / 0.5f; if (t > 1.0f) t = 1.0f;
        left = 0; right = (int)(WIN_W * t); maskA = (int)(255.0f * t);
    }
    else if (g_state == ST_TRANS_HOLD)
    { left = 0; right = WIN_W; maskA = 255; }
    else
    {
        float t = g_transTimer / 0.5f; if (t > 1.0f) t = 1.0f;
        left = (int)(WIN_W * t); right = WIN_W; maskA = (int)(255.0f * (1.0f - t));
    }
    if (maskA > 0) FillRectAlpha(0, 0, WIN_W, WIN_H, maskA);
    if (right > left) { setfillcolor(BLACK); solidrectangle(left, 0, right, WIN_H); }
}

void UpdateHelp()
{
    if (KeyPressed(VK_SPACE)) { PlaySFX(SND_UI); StartTransition(ST_MENU); }
}

void RenderScene(GameState st)
{
    if (st == ST_MENU) { putimage(0, 0, &g_bg); DrawMenuText(); }
    else if (st == ST_HELP) { putimage(0, 0, &g_helpImg); }
    else if (st == ST_MAP_SELECT) DrawMapSelect();
    else if (st == ST_TANK_SELECT) DrawTankSelect();
    else if (st == ST_TANK_INFO) DrawTankInfo();
    else if (st == ST_PVP_COUNT || st == ST_PVP_PLAY)
    { RenderGameWorld(); if (st == ST_PVP_COUNT) DrawCountdown(); }
    else if (st == ST_PVP_OVER) DrawPvpOver();
}

int main()
{
    srand((unsigned)time(NULL));
    initgraph(WIN_W, WIN_H);
    BeginBatchDraw();
    OpenAudio();
    LoadResources();
    InitMenu();
    g_mapSelCurIdx = 0; g_mapSelOldIdx = 0; g_mapSelAnimT = 1.0f; g_mapSelDir = 1;
    for (int i = 0; i < MAX_PLAYERS; i++)
    { g_tankSelCurIdx[i] = 0; g_tankSelOldIdx[i] = 0;
      g_tankSelAnimT[i] = 1.0f; g_tankSelDir[i] = 1; g_selTankIdx[i] = 0; }
    g_tankInfoCurIdx = 0; g_tankInfoOldIdx = 0;
    g_tankInfoAnimT = 1.0f; g_tankInfoDir = 1;
    g_selMapIdx = 0;
    for (int i = 0; i < MAX_AFTERIMG; i++) g_afterImgs[i].active = false;
    for (int i = 0; i < MAX_BUFFS; i++) g_buffs[i].active = false;
    for (int i = 0; i < MAX_VWALLS; i++) g_vwalls[i].active = false;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        g_frostFields[i].active = false;
        g_hunterRings[i].active = false;
    }
    memset(g_keyCur, 0, sizeof(g_keyCur));
    memset(g_keyPrev, 0, sizeof(g_keyPrev));
    g_lastTick = GetTickCount();
    while (!g_quit)
    {
        DWORD now = GetTickCount();
        g_dt = (now - g_lastTick) / 1000.0f;
        g_lastTick = now;
        if (g_dt > 0.1f) g_dt = 0.1f;
        if (g_dt < 0.0f) g_dt = 0.0f;
        g_animTime += g_dt;
        for (int i = 0; i < 256; i++)
        { g_keyPrev[i] = g_keyCur[i]; g_keyCur[i] = (GetAsyncKeyState(i) & 0x8000) != 0; }
        if (g_state != ST_TRANS_IN && g_state != ST_TRANS_HOLD && g_state != ST_TRANS_OUT)
        {
            if (g_state == ST_PVP_COUNT || g_state == ST_PVP_PLAY || g_state == ST_PVP_OVER)
                SwitchBgm(1);
            else SwitchBgm(0);
        }
        bool inTrans = (g_state == ST_TRANS_IN || g_state == ST_TRANS_HOLD || g_state == ST_TRANS_OUT);
        if (inTrans) UpdateTransition();
        else
        {
            switch (g_state)
            {
            case ST_MENU: UpdateMenu(); break;
            case ST_MAP_SELECT: UpdateMapSelect(); break;
            case ST_TANK_SELECT: UpdateTankSelect(); break;
            case ST_TANK_INFO: UpdateTankInfo(); break;
            case ST_PVP_COUNT: UpdateCountdown(); break;
            case ST_PVP_PLAY: UpdatePvP(); break;
            case ST_PVP_OVER: UpdatePvpOver(); break;
            case ST_HELP: UpdateHelp(); break;
            default: break;
            }
        }
        if (KeyPressed(VK_ESCAPE)) g_quit = true;
        setbkcolor(BLACK);
        cleardevice();
        if (inTrans)
        {
            if (g_state == ST_TRANS_IN) { RenderScene(g_transPrev); DrawTransitionOverlay(); }
            else if (g_state == ST_TRANS_HOLD) { setfillcolor(BLACK); solidrectangle(0, 0, WIN_W, WIN_H); }
            else { RenderScene(g_transNext); DrawTransitionOverlay(); }
        }
        else RenderScene(g_state);
        FlushBatchDraw();
        Sleep(10);
    }
    CloseAudio();
    EndBatchDraw();
    closegraph();
    return 0;
}
