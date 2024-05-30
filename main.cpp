#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <windows.h>
#include <chrono>

const int Wx = 1920, Wy = 1080, treeLine = 40, yLane[] = { 420, 570, 740, 890 }, dx[] = { -1,-1,-1,0,0,0,1,1,1 }, dy[] = { -1,0,1,-1,0,1,-1,0,1 }, CONST_enemyCarCount = 14, CONST_maxBananaCount = 5, CONST_maxDrinks = 4, CONST_maxQuotes = 100,
CONST_maxKidCount = 6, CONST_kidCount = 2, CONST_maxCarCount = 3, CONST_animationFrameCount = 3, CONST_maxExplosions = 25, CONST_playerCarCount = 2, CONST_numberOfSongs[] = { 0, 4, 3, 4 }, CONST_maxNumberOfSongs = 4, CONST_hudiconCount = 10,
CONST_maxEndscreenText = 8, CONST_maxBullets = 40, CONST_huditemCount = 6, CONST_maxTargets = 5, CONST_carSpawnChance[] = { 50, 100, 100, 100, 100, 50, 100, 100, 100, 100, 100, 100, 100, 100 }, CONST_maxBalls = 10,
CONST_maxVendorItems = 3, CONST_defaultSeaLevel = 1040, CONST_maxFishes = 7, yFlane[] = { 250, 350, 450, 550, 650, 750 }, CONST_maxFishFrames = 3, CONST_fishSpawnChance[] = { 200, 75, 50, 20, 20, 20, 35},
CONST_fishFrames[] = { 2, 3, 3, 3, 3, 3, 2 }, CONST_maxFishShadows = 4, CONST_fishShadows[] = { 1, 1, 1, 1, 4, 1, 2 }, CONST_numberOfASongs[] = { 0, 0, 0, 4 }, CONST_maxMoneybags = 1, CONST_moneybagValue[] = { 30 }, 
CONST_oyeNumber = 8, CONST_itemPrice[] = { 120, 30, 70 };

const int PRIORITY_car = 5, PRIORITY_explosion = 0, PRIORITY_kid = 4, PRIORITY_banana = 1, PRIORITY_shot = 2, PRIORITY_rpg = 3;

class Car
{
public:
    sf::Vector2u size;
    sf::Texture tex, tex2;
    sf::Sprite spr;
    float speedX = 0, speedY = 0;
};

class Tree
{
public:
    sf::Vector2u size;
    sf::Texture tex;
    sf::Sprite spr;
    bool active = 0;
};

class EnemyCar
{
public:
    sf::Vector2u size;
    sf::Texture tex;
    sf::Sprite spr[5];
    bool active[5] = {};
    int Lane[5] = {}, poz[5] = {}, hp[5] = {};
    float speed[5] = {};
};

class PoliceCar
{
public:
    sf::Vector2u size;
    sf::Texture tex;
    sf::Sprite spr;
    bool active = 0;
    int Lane = 0;
};

class Explosion
{
public:
    sf::Vector2u size;
    sf::Texture tex;
    sf::Sprite spr;
    sf::Sound sound;
    int frame = 0, scale = 0;
    bool active = 0;
};

class Banana
{
public:
    sf::Vector2u size;
    sf::Texture tex;
    sf::Sprite spr[CONST_maxBananaCount];
    bool active[CONST_maxBananaCount] = {};
};

class Kid
{
public:
    sf::Vector2u size;
    sf::Texture tex;
    sf::Sprite spr[CONST_maxKidCount];
    bool active[CONST_maxBananaCount] = {};
};

class WhiskeyShot
{
public:
    sf::Vector2u size;
    sf::Texture tex;
    sf::Sprite spr;
    bool active;
};

class Weapon
{
public:
    sf::Vector2u size;
    sf::Texture tex;
    sf::Sprite spr;
    bool active;
};

class Hudicon
{
public:
    sf::Vector2u size;
    sf::Texture tex;
    sf::Sprite spr;
    sf::Text txt;
    int time;
};

class Huditem
{
public:
    sf::Vector2u size;
    sf::Texture tex;
    sf::Sprite spr;
    bool active;
};

class Bullet
{
public:
    sf::Vector2u size;
    sf::Sprite spr;
    bool active;
};

class Powerup
{
public:
    sf::Vector2u size;
    sf::Texture tex;
    sf::Sprite spr;
    bool active;
};

struct IndexCar {
    int x, y;
};

struct DisplayableSprite
{
    sf::Sprite spr;
    float y = Wy + 1;
    int priority = 100;
};

class Target
{
public:
    sf::Vector2u size;
    sf::Sprite spr;
    int x, y;
};

class Ball
{
public:
    sf::Vector2u size;
    sf::Sprite spr, shadowSpr;
    int Lane, time, shadowOpacity, speed;
    float shadowSize;
    bool active;
};

class Vendor
{
public:
    sf::Vector2u size, isize;
    sf::Sprite spr, itemSpr, auraSpr;
    int Lane, frame, iframe, idir, item, time, price;
    bool active, bad, good, sale;
};

class Fish
{
public:
    sf::Vector2u size;
    sf::Sprite spr, shadowSpr;
    int Flane, frame, time, rotates, nframe, state;
    float speed, speed1, speed0;
    bool active, dir;
};

class Whale
{
public:
    sf::Vector2u size;
    sf::Texture tex1, tex2, tex3, tex4, eyeTex1, eyeTex2;
    sf::Sprite spr1, spr2, spr3, spr4, eyeSpr1, eyeSpr2;
    std::string quote, fullQuote;
    int frame, quoteIndex, quoteLength, textOpacity;
    bool active, stop, noReturn, carDespawn, onShore;

    void Construct()
    {
        size = tex3.getSize();
        spr1.setTexture(tex1);
        spr2.setTexture(tex2);
        spr3.setTexture(tex3);
        spr4.setTexture(tex4);
        eyeSpr1.setTexture(eyeTex1);
        eyeSpr2.setTexture(eyeTex2);
    }
};

class Moneybag
{
public:
    sf::Vector2u size, shadowSize;
    sf::Texture tex, shadowTex;
    sf::Sprite spr, shadowSpr;
    int value, frame, shadowOpacity;
    float scale;
    bool active;

    void Construct()
    {
        size = tex.getSize();
        shadowSize = shadowTex.getSize();
        spr.setTexture(tex);
        spr.setOrigin(0, size.y);
        shadowSpr.setTexture(shadowTex);
        shadowSpr.setOrigin(shadowSize.x / 2, shadowSize.y / 2);
    }
};

class Hudvitem
{
public:
    sf::Vector2u size;
    sf::Texture tex;
    sf::Sprite spr;
    sf::Text text;
    bool active;

    void Construct()
    {
        size = tex.getSize();
        spr.setTexture(tex);
        spr.setOrigin(size.x / 2, size.y / 2);
        spr.setPosition(152, 810);
    }
};

class Button
{
public:
    sf::Texture tex;
    sf::Sprite spr;
};

std::fstream fin;
std::ofstream fout;
std::ofstream fdebug("Assets/Strings/debug.txt");
int shr, spawnTree, indexTree, activeTrees, lastTree, Rand, status, drink, drinkOld, usedCar, titleFlicker, playerCar, playerCarFrame, playerCarFrameCounter, activeCars, lastCar[10], lastCarRate[10], newLane, lastBanana, activeBananas,
objectCount, bananaTime, scoreDigits, quotesNumber[CONST_maxDrinks], usedQuotesNumber[CONST_maxDrinks], lastQuote, quoteTime, newHighscoreTime, bonusScoreTime, bonusScoreValue, bonusScoreGain, activeEvent, lastEvent, eventTime,
eventTimeFrame, deathKeyCooldown, carChaseTime, policeMoveTime, shotTime, policeSirenFrame, newSongDelay, kidsRunOver, jackpotTime, prevHudicon, shakeTimer, endscreenMoveTime, endscreenValue[CONST_maxEndscreenText], timeCounter,
endscreenBananaCounter, endscreenKidCounter, endscreenShotCounter, endscreenRpgCounter, endscreenPoliceCounter, endscreenTimeHelper, maxEndscreenEvents, currentEndscreenEvent, endscreenEventTimer, flickerTime, imagineTime,
usedSongsNumber[CONST_maxDrinks], timeCounterSec, bulletCount, akCooldown, gunflashTime, slowdownTime, prevHuditem, fogOpacity, bombCount, bombX, bombY, bombShadowOpacity, bombAlarmFrame, bombAlarmDelay, bombAlarmLinger, endscreenCarsDestroyed,
endscreenAirstrikeCounter, endscreenBlizzardCounter, endscreenShotsFired, endscreenSlowdownCounter, lastDrink, lastBall[4], activeBalls, balledTime, seaLevel, tideTime, tideActive, tideCooldown, tideLine, tideRest, tideLane,
transitionTime, seashineOpacity, seaOpacity, activeFishes, jellyfishTime, goldfishTime, underwaterTime, darknessOpacity, whaleQuotesNumber, usedWhaleQuotesNumber, usedWhaleQuotes[CONST_maxQuotes], songVolume, alternateVolume,
usedASongsNumber[CONST_maxDrinks], playerMoney, oyeLevel, oyeTime, oyeOpacity, immunityTime, vendorQuotesNumber, vendorQuotesNumberGood, vendorQuotesNumberBad, donutOpacity, donutTime, prevStatus, oldTime, deltaTime, tsunamiCooldown,
endscreenMoneySpent, endscreenGoldfishCounter, endscreenWhaleCounter, endscreenBallCounter, endscreenJellyfishCounter;
long long score, highscore[CONST_maxDrinks];
float roadSpeed, policeMoveX, policeMoveY, carVelocityX, carVelocityY, shx, shy, imagineVolume, bombShadowScale, donutScale, musicVolume, sfxVolume;
long double endscreenIncrement, endscreenCurrentValue;
bool ir, keyPressed, isDrinkMoving, drinkDirection, leftPressed, rightPressed, upPressed, downPressed, spacePressed, xPressed, zPressed, usedQuotes[CONST_maxDrinks][CONST_maxQuotes], carSpriteDisplayed, bananaed, activeQuote,
newHighscoreReached, kidWave1, kidWave2, usedLane[10], policeLaser, carChaseActive, rpgEquipped, rpgSpriteOn, rpgSpriteOff, limitReachedX, limitReachedY, usedSong[CONST_maxDrinks][CONST_maxNumberOfSongs], firstTimeSong, endscreenOn,
newHighscoreReachedEnd, lennonActive, hardmode, speedMilestone[10], slowdownEquipped, planeStart, bombAlarmActive, underwaterActive, usedFlane[6], usedASong[CONST_maxDrinks][CONST_maxNumberOfSongs], tpActive, fullscreenOn,
leftPressed2, rightPressed2, upPressed2, downPressed2, bellfishEquipped, cPressed, jPressed, kPressed, lPressed, seashellEquipped, donutEquipped, cornEquipped, summonWhale, escPressed, mouseHeld, ball1Held, ball2Held, settingsActive,
killedJL, p1Dead, hasSeenWhale, hasBeenUnderwater, exitActive, gameExited;
std::string fileName, quotes[CONST_maxDrinks][CONST_maxQuotes], drinkNames[CONST_maxDrinks], drinkDesc[CONST_maxDrinks], endscreenString[CONST_maxDrinks][CONST_maxEndscreenText], whaleQuotes[CONST_maxQuotes], vendorQuotes[CONST_maxQuotes],
vendorQuotesGood[CONST_maxQuotes], vendorQuotesBad[CONST_maxQuotes];
sf::Texture RoadTex, RoadTex1, MenuTex, DrinkTex[CONST_maxDrinks], TitleScreenTex, TitleScreenTex1, RoadSchoolTex, RoadSchoolTex1, RoadPoliceTex, RoadPoliceTexOff, RoadPoliceTexTrg, PoliceSirenTex0, PoliceSirenTex1, EndscreenTex,
MenuTex1, BulletTex, GunflashTex, Road1Tex, Road1Tex1, FogTex, FogTex1, SnowstormTex, TargetTex, BombShadowTex, BombTex, PlaneTex, BombAlarmTex0, BombAlarmTex1, Road2Tex, Road2Tex1, BallTex, VendorTex[12], VendorItemTex[CONST_maxVendorItems],
AuraTex, SeaTex, SeaTex1, SeashineTex, SearoadTex, SearoadTex1, LoadingTex, FishTex[CONST_maxFishes][2][CONST_maxFishFrames], FishShadowTex[CONST_maxFishes][CONST_maxFishShadows], CarAuraTex, DarknessTex, CapsuleTex, CarAbsentTex,
CapsuleBrokenTex, OyeTex[CONST_oyeNumber], CornAuraTex, ImmunityAuraTex, VendorWindowTex, VendorWindowTexGood, VendorWindowTexBad, DonutTex, CursorTex, SettingsTex, BallTex1, BallTex2, BarTex1, BarTex2, WhiteBarTex, SettingsButtonTex,
CheckTex, ArrowTex1, ArrowTex2, DriveTex;
sf::Sprite RoadSpr, RoadSpr1, MenuSpr, DrinkSpr[CONST_maxDrinks], TitleScreenSpr, TitleScreenSpr1, PoliceSirenSpr, EndscreenSpr, GunflashSpr, FogSpr, FogSpr1, SnowstormSpr, SnowstormSpr1, PlaneSpr, BombAlarmSpr, SeaSpr, SeaSpr1, SeashineSpr,
LoadingSpr, CarAuraSpr, DarknessSpr, CapsuleSpr, CapsuleBrokenSpr, OyeSpr, CornAuraSpr, ImmunityAuraSpr, VendorWindowSpr, DonutSpr, CursorSpr, SettingsSpr, BallSpr1, BallSpr2, BarSpr1, BarSpr2, WhiteBarSpr, SettingsButtonSpr,
CheckSpr[10], ArrowSpr1, ArrowSpr2, DriveSpr;
sf::Font libre, typewriter, scorefont, terar, miology, whalefont;
sf::Text txtTitleScreen, selectDrink, scoreText, quoteText, highscoreMenu, newHighscore, bonusScoreText, jackpotText, endscreenText[CONST_maxEndscreenText], newHighscoreEnd, whaleQuoteText, moneyText, vendorQuoteText,
vendorMoneyText;
sf::SoundBuffer ExplosionSoundB, SongB[CONST_maxDrinks][CONST_maxNumberOfSongs], ASongB[CONST_maxDrinks][CONST_maxNumberOfSongs], PoliceSirenB, AmbulanceSoundB, BananaSoundB, SlurpSoundB, BonusSoundB[6], JackpotSoundB,
MaximumComboSoundB, EngineSoundB, RpgPickupSoundB, MenuSongB, PoliceClickSoundB, NewHighscoreSoundB, EndscreenSoundB, ImagineSoundB, BulletSoundB, AmmoboxSoundB, SlowdownSoundB, SlowdownEquipSoundB, BombAlarmB, PlaneSoundB,
BlizzardSoundB, SlowdownEndSoundB, BulletHitSoundB, BoingSoundB, TideSound1B, TideSound2B, TideRestSoundB, BigTideSoundB, GlassSoundB, BiteSoundB, GoldfishSoundB, ReturnfishSoundB, AnglerSoundB, JellyfishSoundB, WhalethrowSoundB,
WhaleVoiceSoundB, MoneySoundB, BellfishSoundB, SeacallSoundB, VendorGoodSoundB, VendorBadSoundB, VendorNewSoundB, BloodSoundB, RevolverSoundB, DonutSoundB, BellSoundB;
sf::Sound CurrentSong, AlternateSong, PoliceSiren, AmbulanceSound[5], BananaSound, SlurpSound, BonusSound[6], JackpotSound, MaximumComboSound, EngineSound, RpgPickupSound, MenuSong, PoliceClickSound, NewHighscoreSound, EndscreenSound,
ImagineSound, BulletSound, AmmoboxSound, SlowdownSound, SlowdownEquipSound, BombAlarm, PlaneSound, BlizzardSound, SlowdownEndSound, BulletHitSound, BoingSound, TideSound1, TideSound2, TideRestSound, BigTideSound, GlassSound,
BiteSound, GoldfishSound, ReturnfishSound, AnglerSound, JellyfishSound, WhalethrowSound, WhaleVoiceSound, MoneySound, BellfishSound, SeacallSound, VendorGoodSound, VendorBadSound, VendorNewSound, RevolverSound, DonutSound,
BellSound;
sf::Vector2i MousePosition, PrevMousePosition;
IndexCar ieCar[10];
DisplayableSprite object[300];
Car car[20][5], car2[20][5];
Tree tree[20];
EnemyCar enemyCar[30][5];
PoliceCar policeCar[5];
Explosion explosion[CONST_maxExplosions], blood[CONST_maxExplosions];
Banana banana;
Kid kid[CONST_kidCount];
WhiskeyShot shot[5];
Weapon rpg[5], rocket;
Bullet bullet[CONST_maxBullets];
Hudicon hudicon[CONST_hudiconCount];
Powerup ammobox[5], slowdown[5];
Huditem huditem[CONST_huditemCount];
Target target[CONST_maxTargets], bombShadow[CONST_maxTargets], bomb[CONST_maxTargets];
Ball ball[CONST_maxBalls];
Vendor vendor;
Fish fish[CONST_maxFishes][4];
Whale whale;
Moneybag moneybag[CONST_maxMoneybags];
Hudvitem hudvitem[CONST_maxVendorItems];
Button* exitButton, * exitConfirm;

// status guide:
// 0 - title screen
// 1 - driving
// 2 - dead
// 3 - drinking menu init
// 4 - level startup
// 5 - drinking menu real


bool ieCarCond(IndexCar A, IndexCar B)
{
    if (A.x == -1 or B.y == -1)
        return A.x < B.y;
    return enemyCar[A.x][0].Lane[A.y] < enemyCar[B.x][0].Lane[B.y];
}

bool displayCond(DisplayableSprite A, DisplayableSprite B)
{
    if (A.y == B.y)
        return A.priority < B.priority;
    return A.y < B.y;
}

void HudvitemToText()
{
    int i;
    std::string localString;

    for (i = 0; i < CONST_maxVendorItems; i++)
    {
        if (i == 0)
        {
            localString = "Sacred Corn";
            hudvitem[i].text.setCharacterSize(24);
        }
        else if (i == 1)
        {
            localString = "Cool Seashell";
            hudvitem[i].text.setCharacterSize(24);
        }
        else if (i == 2)
        {
            localString = "Suspicious-Looking";
            localString += '\n';
            localString += "         Donut";
            hudvitem[i].text.setCharacterSize(18);
        }

        hudvitem[i].text.setFont(terar);
        hudvitem[i].text.setFillColor(sf::Color(255, 255, 255));
        hudvitem[i].text.setOutlineColor(sf::Color(0, 0, 0));
        hudvitem[i].text.setOutlineThickness(2);
        hudvitem[i].text.setString(localString);
        hudvitem[i].text.setOrigin(hudvitem[i].text.getGlobalBounds().getSize().x / 2, 0);
        hudvitem[i].text.setPosition(150, 840);
    }
}

void LoadStuff()
{
    int i, j, l;
    for (i = 0; i < CONST_maxDrinks; i++)
    {
        if (i == 0)
        {
            if (!DrinkTex[i].loadFromFile("Assets/Textures/Drinks/water.png"));
        }
        else if (i == 1)
        {
            if (!DrinkTex[i].loadFromFile("Assets/Textures/Drinks/jack.png"));
        }
        else if (i == 2)
        {
            if (!DrinkTex[i].loadFromFile("Assets/Textures/Drinks/vodka.png"));
        }
        else if (i == 3)
        {
            if (!DrinkTex[i].loadFromFile("Assets/Textures/Drinks/beer.png"));
        }
        DrinkSpr[i].setTexture(DrinkTex[i]);
        DrinkSpr[i].setOrigin(DrinkTex[i].getSize().x / 2, DrinkTex[i].getSize().y);
    }

    if (!RoadTex.loadFromFile("Assets/Textures/Background/road.png"));
    if (!RoadTex1.loadFromFile("Assets/Textures/Background/road_mirrored.png"));
    if (!Road1Tex.loadFromFile("Assets/Textures/Background/road1.png"));
    if (!Road1Tex1.loadFromFile("Assets/Textures/Background/road1_mirrored.png"));
    if (!Road2Tex.loadFromFile("Assets/Textures/Background/road2.png"));
    if (!Road2Tex1.loadFromFile("Assets/Textures/Background/road2_mirrored.png"));

    if (!MenuTex.loadFromFile("Assets/Textures/Background/menu.png"));
    if (!MenuTex1.loadFromFile("Assets/Textures/Background/menu1.png"));
    MenuSpr.setTexture(MenuTex);

    for (i = 0; i < 9; i++)
    {
        if (!tree[i].tex.loadFromFile("Assets/Textures/Obstacles/tree1.png"));
        tree[i].size = tree[i].tex.getSize();
        tree[i].spr.setTexture(tree[i].tex);
    }

    if (!TitleScreenTex.loadFromFile("Assets/Textures/Background/title_screen.png"));
    TitleScreenSpr.setTexture(TitleScreenTex);

    if (!TitleScreenTex1.loadFromFile("Assets/Textures/Background/title_screen1.png"));
    TitleScreenSpr1.setTexture(TitleScreenTex1);

    if (!libre.loadFromFile("Assets/Fonts/libre.ttf"));
    if (!typewriter.loadFromFile("Assets/Fonts/typewriter.ttf"));
    if (!terar.loadFromFile("Assets/Fonts/terar.ttf"));
    if (!scorefont.loadFromFile("Assets/Fonts/scorefont.otf"));
    if (!miology.loadFromFile("Assets/Fonts/miology.otf"));
    if (!whalefont.loadFromFile("Assets/Fonts/whale.ttf"));

    selectDrink.setFont(typewriter);
    selectDrink.setString("Select your beverage of choice.");
    selectDrink.setCharacterSize(54);
    selectDrink.setPosition(40, 100);
    selectDrink.setOutlineColor(sf::Color(0, 0, 0, 128));
    selectDrink.setOutlineThickness(5);

    scoreText.setFont(scorefont);
    scoreText.setCharacterSize(46);
    scoreText.setPosition(30, 10);
    scoreText.setFillColor(sf::Color(169, 195, 255));
    scoreText.setOutlineColor(sf::Color(0, 0, 0));
    scoreText.setOutlineThickness(5);

    quoteText.setFont(terar);
    quoteText.setCharacterSize(40);
    quoteText.setPosition(100, 1005);
    quoteText.setOutlineColor(sf::Color(0, 0, 0));
    quoteText.setOutlineThickness(5);

    highscoreMenu.setFont(terar);
    highscoreMenu.setCharacterSize(46);
    highscoreMenu.setPosition(100, 850);
    highscoreMenu.setOutlineColor(sf::Color(0, 0, 0));
    highscoreMenu.setOutlineThickness(5);

    newHighscore.setFont(scorefont);
    newHighscore.setCharacterSize(46);
    newHighscore.setString("NEW HIGHSCORE!");
    newHighscore.setFillColor(sf::Color(169, 195, 255));
    newHighscore.setOutlineColor(sf::Color(0, 0, 0));
    newHighscore.setOutlineThickness(5);

    newHighscoreEnd.setFont(terar);
    newHighscoreEnd.setCharacterSize(48);
    newHighscoreEnd.setString("NEW HIGHSCORE!");
    newHighscoreEnd.setFillColor(sf::Color(204, 255, 255));
    newHighscoreEnd.setOutlineColor(sf::Color(0, 0, 0));
    newHighscoreEnd.setOutlineThickness(3);

    bonusScoreText.setFont(scorefont);
    bonusScoreText.setCharacterSize(46);
    bonusScoreText.setPosition(175, 55);
    bonusScoreText.setFillColor(sf::Color(169, 195, 255));
    bonusScoreText.setOutlineColor(sf::Color(0, 0, 0));
    bonusScoreText.setOutlineThickness(5);

    jackpotText.setFont(scorefont);
    jackpotText.setCharacterSize(46);
    jackpotText.setString("JACKPOT!");
    jackpotText.setFillColor(sf::Color(255, 223, 0));
    jackpotText.setOutlineColor(sf::Color(0, 0, 0));
    jackpotText.setOutlineThickness(5);

    i = 0;
    fin.open("Assets/Strings/drink_names.txt");
    while (std::getline(fin, drinkNames[i]), std::getline(fin, drinkDesc[i]))
        i++;
    fin.close();

    for (i = 0; i < CONST_maxDrinks; i++)
    {
        fileName = "Assets/Strings/drink";
        fileName += char('0' + i);
        fileName += ".txt";
        fin.open(fileName);
        j = 0;
        while (std::getline(fin, quotes[i][j]))
            j++;
        quotesNumber[i] = j;
        fin.close();
    }

    fileName = "Assets/Strings/whale.txt";
    fin.open(fileName);
    j = 0;
    while (std::getline(fin, whaleQuotes[j]))
        j++;
    whaleQuotesNumber = j;
    fin.close();

    fileName = "Assets/Strings/vendor.txt";
    fin.open(fileName);
    j = 0;
    while (std::getline(fin, vendorQuotes[j]))
        j++;
    vendorQuotesNumber = j;
    fin.close();

    fileName = "Assets/Strings/vendor_good.txt";
    fin.open(fileName);
    j = 0;
    while (std::getline(fin, vendorQuotesGood[j]))
        j++;
    vendorQuotesNumberGood = j;
    fin.close();

    fileName = "Assets/Strings/vendor_bad.txt";
    fin.open(fileName);
    j = 0;
    while (std::getline(fin, vendorQuotesBad[j]))
        j++;
    vendorQuotesNumberBad = j;
    fin.close();

    for (i = 0; i < CONST_maxDrinks; i++)
    {
        for (j = 0; j < 3; j++)
        {
            fileName = "Assets/Textures/Cars/car";
            fileName += char('0' + i);
            fileName += '_';
            fileName += char('0' + j);
            fileName += ".png";
            if (!car[i][j].tex.loadFromFile(fileName));
            car[i][j].size = car[i][j].tex.getSize();
            car[i][j].spr.setTexture(car[i][j].tex);
            car[i][j].spr.setOrigin(car[i][j].size.x / 2, car[i][j].size.y / 2);
            if (!car2[i][j].tex.loadFromFile(fileName));
            car2[i][j].size = car2[i][j].tex.getSize();
            car2[i][j].spr.setTexture(car2[i][j].tex);
            car2[i][j].spr.setOrigin(car2[i][j].size.x / 2, car2[i][j].size.y / 2);
        }
    }

    i = 1;
    for (j = 0; j < 3; j++)
    {
        fileName = "Assets/Textures/Cars/car";
        fileName += char('0' + i);
        fileName += '_';
        fileName += char('0' + j);
        fileName += "_rpg.png";
        if (!car[i][j].tex2.loadFromFile(fileName));
    }

    i = 3;
    for (j = 0; j < 3; j++)
    {
        fileName = "Assets/Textures/Cars/car";
        fileName += char('0' + i);
        fileName += '_';
        fileName += char('0' + j);
        fileName += "_u.png";
        if (!car[i][j].tex2.loadFromFile(fileName));
    }

    for (i = 0; i < 3; i++)
    {
        fileName = "Assets/Textures/EnemyCars/carp";
        fileName += '_';
        fileName += char('0' + i);
        fileName += ".png";
        if (!policeCar[i].tex.loadFromFile(fileName));
        policeCar[i].size = policeCar[i].tex.getSize();
        policeCar[i].spr.setTexture(policeCar[i].tex);
        policeCar[i].spr.setOrigin(0, policeCar[i].size.y);
    }

    for (i = 1; i <= CONST_enemyCarCount; i++)
    {
        for (j = 0; j < 3; j++)
        {
            fileName = "Assets/Textures/EnemyCars/car";
            fileName += std::to_string(i);
            fileName += '_';
            fileName += char('0' + j);
            fileName += ".png";
            if (!enemyCar[i][j].tex.loadFromFile(fileName));
            enemyCar[i][j].size = enemyCar[i][j].tex.getSize();

            for (l = 0; l < 5; l++)
            {
                enemyCar[i][j].spr[l].setTexture(enemyCar[i][j].tex);
                enemyCar[i][j].spr[l].setOrigin(0, enemyCar[i][j].size.y);
            }
        }
    }

    if (!banana.tex.loadFromFile("Assets/Textures/Obstacles/banana.png"));
    banana.size = banana.tex.getSize();
    for (i = 0; i < CONST_maxBananaCount; i++)
    {
        banana.spr[i].setTexture(banana.tex);
        banana.spr[i].setOrigin(banana.size.x / 2, banana.size.y / 2);
    }

    if (!RoadSchoolTex.loadFromFile("Assets/Textures/Background/road_school.png"));
    if (!RoadSchoolTex1.loadFromFile("Assets/Textures/Background/road_school_mirrored.png"));
    if (!RoadPoliceTex.loadFromFile("Assets/Textures/Background/road_police.png"));
    if (!RoadPoliceTexOff.loadFromFile("Assets/Textures/Background/road_police_off.png"));
    if (!RoadPoliceTexTrg.loadFromFile("Assets/Textures/Background/road_police_triggered.png"));

    for (i = 0; i < CONST_kidCount; i++)
    {
        fileName = "Assets/Textures/Obstacles/kid_";
        fileName += char('0' + i);
        fileName += ".png";
        if (!kid[i].tex.loadFromFile(fileName));
        kid[i].size = kid[i].tex.getSize();

        for (j = 0; j < CONST_maxKidCount; j++)
            kid[i].spr[j].setTexture(kid[i].tex);
    }

    for (i = 0; i < 3; i++)
    {
        fileName = "Assets/Textures/Obstacles/shot_";
        fileName += char('0' + i);
        fileName += ".png";
        if (!shot[i].tex.loadFromFile(fileName));
        shot[i].size = shot[i].tex.getSize();
        shot[i].spr.setTexture(shot[i].tex);
        shot[i].spr.setOrigin(shot[i].size.x / 2, shot[i].size.y / 2);
    }

    if (!PoliceSirenTex0.loadFromFile("Assets/Textures/Background/police_siren_0.png"));
    if (!PoliceSirenTex1.loadFromFile("Assets/Textures/Background/police_siren_1.png"));

    for (i = 0; i < 3; i++)
    {
        fileName = "Assets/Textures/Weapons/rpg_";
        fileName += char('0' + i);
        fileName += ".png";
        if (!rpg[i].tex.loadFromFile(fileName));
        rpg[i].size = rpg[i].tex.getSize();
        rpg[i].spr.setTexture(rpg[i].tex);
        rpg[i].spr.setOrigin(rpg[i].size.x / 2, rpg[i].size.y / 2);
    }
    
    if (!rocket.tex.loadFromFile("Assets/Textures/Weapons/rocket.png"));
    rocket.size = rocket.tex.getSize();
    rocket.spr.setTexture(rocket.tex);
    rocket.spr.setOrigin(rocket.size.x / 2, rocket.size.y / 2);

    if (!ExplosionSoundB.loadFromFile("Assets/Sounds/SFX/explosion.ogg"));
    for (i = 0; i < CONST_maxExplosions; i++)
        explosion[i].sound.setBuffer(ExplosionSoundB);
    if (!BloodSoundB.loadFromFile("Assets/Sounds/SFX/blood.ogg"));
    for (i = 0; i < CONST_maxExplosions; i++)
        blood[i].sound.setBuffer(BloodSoundB);

    for (i = 1; i < CONST_maxDrinks; i++)
        for (j = 0; j < CONST_numberOfSongs[i]; j++)
        {
            fileName = "Assets/Sounds/Music/song_";
            fileName += char('0' + i);
            fileName += '_';
            fileName += char('0' + j);
            fileName += ".ogg";
            if (!SongB[i][j].loadFromFile(fileName));
        }

    for (i = 1; i < CONST_maxDrinks; i++)
        for (j = 0; j < CONST_numberOfASongs[i]; j++)
        {
            fileName = "Assets/Sounds/Music/asong_";
            fileName += char('0' + i);
            fileName += '_';
            fileName += char('0' + j);
            fileName += ".ogg";
            if (!ASongB[i][j].loadFromFile(fileName));
        }

    if (!PoliceSirenB.loadFromFile("Assets/Sounds/SFX/police.ogg"));
    PoliceSiren.setBuffer(PoliceSirenB);

    if (!AmbulanceSoundB.loadFromFile("Assets/Sounds/SFX/ambulance.ogg"));
    for (i = 0; i < 5; i++)
        AmbulanceSound[i].setBuffer(AmbulanceSoundB);

    if (!BananaSoundB.loadFromFile("Assets/Sounds/SFX/banana.ogg"));
    BananaSound.setBuffer(BananaSoundB);
    if (!SlurpSoundB.loadFromFile("Assets/Sounds/SFX/slurp.ogg"));
    SlurpSound.setBuffer(SlurpSoundB);

    for (i = 0; i < 6; i++)
    {
        fileName = "Assets/Sounds/SFX/bonus_";
        fileName += char('0' + i);
        fileName += ".ogg";
        if (!BonusSoundB[i].loadFromFile(fileName));
        BonusSound[i].setBuffer(BonusSoundB[i]);
    }

    if (!JackpotSoundB.loadFromFile("Assets/Sounds/SFX/jackpot.ogg"));
    JackpotSound.setBuffer(JackpotSoundB);
    if (!MaximumComboSoundB.loadFromFile("Assets/Sounds/SFX/maximum_combo.ogg"));
    MaximumComboSound.setBuffer(MaximumComboSoundB);
    if (!EngineSoundB.loadFromFile("Assets/Sounds/SFX/engine.ogg"));
    EngineSound.setBuffer(EngineSoundB);
    EngineSound.setLoop(true);
    if (!RpgPickupSoundB.loadFromFile("Assets/Sounds/SFX/rpg_pickup.ogg"));
    RpgPickupSound.setBuffer(RpgPickupSoundB);
    if (!MenuSongB.loadFromFile("Assets/Sounds/Music/menu.ogg"));
    MenuSong.setBuffer(MenuSongB);
    MenuSong.setLoop(true);
    MenuSong.setVolume(70 * musicVolume);
    if (!PoliceClickSoundB.loadFromFile("Assets/Sounds/SFX/police_click.ogg"));
    PoliceClickSound.setBuffer(PoliceClickSoundB);
    if (!NewHighscoreSoundB.loadFromFile("Assets/Sounds/SFX/new_highscore.ogg"));
    NewHighscoreSound.setBuffer(NewHighscoreSoundB);
    if (!EndscreenSoundB.loadFromFile("Assets/Sounds/SFX/win.ogg"));
    EndscreenSound.setBuffer(EndscreenSoundB);
    if (!ImagineSoundB.loadFromFile("Assets/Sounds/SFX/imagine.ogg"));
    ImagineSound.setBuffer(ImagineSoundB);
    ImagineSound.setLoop(true);
    if (!BulletSoundB.loadFromFile("Assets/Sounds/SFX/bullet.ogg"));
    BulletSound.setBuffer(BulletSoundB);
    if (!AmmoboxSoundB.loadFromFile("Assets/Sounds/SFX/ammobox.ogg"));
    AmmoboxSound.setBuffer(AmmoboxSoundB);
    if (!SlowdownSoundB.loadFromFile("Assets/Sounds/SFX/slowdown.ogg"));
    SlowdownSound.setBuffer(SlowdownSoundB);
    if (!SlowdownEndSoundB.loadFromFile("Assets/Sounds/SFX/slowdown_end.ogg"));
    SlowdownEndSound.setBuffer(SlowdownEndSoundB);
    if (!SlowdownEquipSoundB.loadFromFile("Assets/Sounds/SFX/slowdown_equip.ogg"));
    SlowdownEquipSound.setBuffer(SlowdownEquipSoundB);
    if (!BombAlarmB.loadFromFile("Assets/Sounds/SFX/bomb_alarm.ogg"));
    BombAlarm.setBuffer(BombAlarmB);
    if (!PlaneSoundB.loadFromFile("Assets/Sounds/SFX/plane.ogg"));
    PlaneSound.setBuffer(PlaneSoundB);
    if (!BlizzardSoundB.loadFromFile("Assets/Sounds/SFX/blizzard.ogg"));
    BlizzardSound.setBuffer(BlizzardSoundB);
    if (!BulletHitSoundB.loadFromFile("Assets/Sounds/SFX/bullet_hit.ogg"));
    BulletHitSound.setBuffer(BulletHitSoundB);
    if (!BoingSoundB.loadFromFile("Assets/Sounds/SFX/boing.ogg"));
    BoingSound.setBuffer(BoingSoundB);
    if (!TideSound1B.loadFromFile("Assets/Sounds/SFX/tide1.ogg"));
    TideSound1.setBuffer(TideSound1B);
    if (!TideSound2B.loadFromFile("Assets/Sounds/SFX/tide2.ogg"));
    TideSound2.setBuffer(TideSound2B);
    if (!TideRestSoundB.loadFromFile("Assets/Sounds/SFX/tide_rest.ogg"));
    TideRestSound.setBuffer(TideRestSoundB);
    if (!BigTideSoundB.loadFromFile("Assets/Sounds/SFX/big_tide.ogg"));
    BigTideSound.setBuffer(BigTideSoundB);
    if (!GlassSoundB.loadFromFile("Assets/Sounds/SFX/glass.ogg"));
    GlassSound.setBuffer(GlassSoundB);
    if (!GoldfishSoundB.loadFromFile("Assets/Sounds/SFX/goldfish.ogg"));
    GoldfishSound.setBuffer(GoldfishSoundB);
    if (!BiteSoundB.loadFromFile("Assets/Sounds/SFX/bite.ogg"));
    BiteSound.setBuffer(BiteSoundB);
    if (!ReturnfishSoundB.loadFromFile("Assets/Sounds/SFX/returnfish.ogg"));
    ReturnfishSound.setBuffer(ReturnfishSoundB);
    if (!AnglerSoundB.loadFromFile("Assets/Sounds/SFX/angler.ogg"));
    AnglerSound.setBuffer(AnglerSoundB);
    if (!JellyfishSoundB.loadFromFile("Assets/Sounds/SFX/jellyfish.ogg"));
    JellyfishSound.setBuffer(JellyfishSoundB);
    if (!WhalethrowSoundB.loadFromFile("Assets/Sounds/SFX/whalethrow.ogg"));
    WhalethrowSound.setBuffer(WhalethrowSoundB);
    if (!WhaleVoiceSoundB.loadFromFile("Assets/Sounds/SFX/whale_voice.ogg"));
    WhaleVoiceSound.setBuffer(WhaleVoiceSoundB);
    if (!MoneySoundB.loadFromFile("Assets/Sounds/SFX/money.ogg"));
    MoneySound.setBuffer(MoneySoundB);
    if (!BellfishSoundB.loadFromFile("Assets/Sounds/SFX/bellfish.ogg"));
    BellfishSound.setBuffer(BellfishSoundB);
    if (!SeacallSoundB.loadFromFile("Assets/Sounds/SFX/seacall.ogg"));
    SeacallSound.setBuffer(SeacallSoundB);
    if (!VendorGoodSoundB.loadFromFile("Assets/Sounds/SFX/vendor_good.ogg"));
    VendorGoodSound.setBuffer(VendorGoodSoundB);
    if (!VendorBadSoundB.loadFromFile("Assets/Sounds/SFX/vendor_bad.ogg"));
    VendorBadSound.setBuffer(VendorBadSoundB);
    if (!VendorNewSoundB.loadFromFile("Assets/Sounds/SFX/vendor_new.ogg"));
    VendorNewSound.setBuffer(VendorNewSoundB);
    if (!RevolverSoundB.loadFromFile("Assets/Sounds/SFX/revolver.ogg"));
    RevolverSound.setBuffer(RevolverSoundB);
    if (!DonutSoundB.loadFromFile("Assets/Sounds/SFX/donut.ogg"));
    DonutSound.setBuffer(DonutSoundB);
    if (!BellSoundB.loadFromFile("Assets/Sounds/SFX/bell.ogg"));
    BellSound.setBuffer(BellSoundB);
    

    for (i = 0; i < CONST_hudiconCount; i++)
    {
        fileName = "Assets/Textures/HUD/hudicon_";
        fileName += char('0' + i);
        fileName += ".png";
        if (!hudicon[i].tex.loadFromFile(fileName));
        hudicon[i].size = hudicon[i].tex.getSize();
        hudicon[i].spr.setTexture(hudicon[i].tex);
        hudicon[i].txt.setFont(terar);
        hudicon[i].txt.setCharacterSize(30);
        hudicon[i].txt.setOutlineColor(sf::Color(0, 0, 0));
        hudicon[i].txt.setOutlineThickness(2);
    }

    if (!EndscreenTex.loadFromFile("Assets/Textures/Background/endscreen.png"));
    EndscreenSpr.setTexture(EndscreenTex);
    EndscreenSpr.setOrigin(EndscreenTex.getSize().x / 2, EndscreenTex.getSize().y);

    for (i = 0; i < CONST_maxEndscreenText; i++)
    {
        endscreenText[i].setFont(terar);
        endscreenText[i].setCharacterSize(34);
        endscreenText[i].setOutlineColor(sf::Color(0, 0, 0));
        endscreenText[i].setFillColor(sf::Color(204, 255, 255));
        endscreenText[i].setOutlineThickness(2);
    }

    endscreenText[0].setCharacterSize(48);
    endscreenText[1].setCharacterSize(48);
    endscreenText[0].setOutlineThickness(3);
    endscreenText[1].setOutlineThickness(3);

    for (i = 0; i < CONST_maxDrinks; i++)
    {
        endscreenString[i][0] = "Score: ";
        endscreenString[i][1] = "Time: ";
    }
    endscreenString[0][2] = "Banana slips: ";
    endscreenString[1][2] = "Banana slips: ";
    endscreenString[1][3] = "Whiskey shots taken: ";
    endscreenString[1][4] = "Infant obstacles removed: ";
    endscreenString[1][5] = "Police evasions: ";
    endscreenString[1][6] = "Cars demolished: ";
    endscreenString[2][2] = "Banana slips: ";
    endscreenString[2][3] = "Shots fired: ";
    endscreenString[2][4] = "Freezing vodka shots drunk: ";
    endscreenString[2][5] = "Cars gunned down: ";
    endscreenString[2][6] = "Russian airstrikes survived: ";
    endscreenString[2][7] = "Blizzards survived: ";
    endscreenString[3][2] = "Total money spent: $";
    endscreenString[3][3] = "Beach balls tanked: ";
    endscreenString[3][4] = "Goldfish eaten: ";
    endscreenString[3][5] = "Beer jellyfish eaten: ";
    endscreenString[3][6] = "Whale rides taken: ";

    if (!BulletTex.loadFromFile("Assets/Textures/Weapons/bullet.png"));
    for (i = 0; i < CONST_maxBullets; i++)
    {
        bullet[i].spr.setTexture(BulletTex);
        bullet[i].size = BulletTex.getSize();
        bullet[i].spr.setOrigin(bullet[i].size.x / 2, bullet[i].size.y / 2);
    }

    if (!GunflashTex.loadFromFile("Assets/Textures/Weapons/gunflash.png"));
    GunflashSpr.setTexture(GunflashTex);

    for (i = 0; i < 3; i++)
    {
        fileName = "Assets/Textures/Obstacles/ammobox_";
        fileName += char('0' + i);
        fileName += ".png";
        if (!ammobox[i].tex.loadFromFile(fileName));
        ammobox[i].size = ammobox[i].tex.getSize();
        ammobox[i].spr.setTexture(ammobox[i].tex);
        ammobox[i].spr.setOrigin(ammobox[i].size.x / 2, ammobox[i].size.y / 2);
    }

    for (i = 0; i < 3; i++)
    {
        fileName = "Assets/Textures/Obstacles/slowdown_";
        fileName += char('0' + i);
        fileName += ".png";
        if (!slowdown[i].tex.loadFromFile(fileName));
        slowdown[i].size = slowdown[i].tex.getSize();
        slowdown[i].spr.setTexture(slowdown[i].tex);
        slowdown[i].spr.setOrigin(slowdown[i].size.x / 2, slowdown[i].size.y / 2);
    }

    for (i = 0; i < CONST_huditemCount; i++)
    {
        fileName = "Assets/Textures/HUD/item_";
        fileName += char('0' + i);
        fileName += ".png";
        if (!huditem[i].tex.loadFromFile(fileName));
        huditem[i].size = huditem[i].tex.getSize();
        huditem[i].spr.setTexture(huditem[i].tex);
    }

    for (i = 0; i < CONST_maxVendorItems; i++)
    {
        fileName = "Assets/Textures/HUD/vitem_";
        fileName += char('0' + i);
        fileName += ".png";
        if (!hudvitem[i].tex.loadFromFile(fileName));
        hudvitem[i].Construct();
    }
    HudvitemToText();

    if (!FogTex.loadFromFile("Assets/Textures/Background/fog.png"));
    if (!FogTex1.loadFromFile("Assets/Textures/Background/fog_again.png"));
    FogSpr.setTexture(FogTex);
    FogSpr1.setTexture(FogTex1);

    if (!SnowstormTex.loadFromFile("Assets/Textures/Background/snowstorm.png"));
    SnowstormSpr.setTexture(SnowstormTex);
    SnowstormSpr1.setTexture(SnowstormTex);

    if (!TargetTex.loadFromFile("Assets/Textures/Obstacles/target.png"));
    for (i = 0; i < CONST_maxTargets; i++)
    {
        target[i].spr.setTexture(TargetTex);
        target[i].size = TargetTex.getSize();
        target[i].spr.setOrigin(target[i].size.x / 2, target[i].size.y / 2);
    }

    if (!BombShadowTex.loadFromFile("Assets/Textures/Obstacles/bomb_shadow.png"));
    for (i = 0; i < CONST_maxTargets; i++)
    {
        bombShadow[i].spr.setTexture(BombShadowTex);
        bombShadow[i].size = BombShadowTex.getSize();
        bombShadow[i].spr.setOrigin(bombShadow[i].size.x / 2, bombShadow[i].size.y / 2);
    }

    if (!BombTex.loadFromFile("Assets/Textures/Obstacles/bomb.png"));
    for (i = 0; i < CONST_maxTargets; i++)
    {
        bomb[i].spr.setTexture(BombTex);
        bomb[i].size = BombTex.getSize();
        bomb[i].spr.setOrigin(bomb[i].size.x / 2, bomb[i].size.y);
    }

    if (!PlaneTex.loadFromFile("Assets/Textures/Background/plane.png"));
    PlaneSpr.setTexture(PlaneTex);

    if (!BombAlarmTex0.loadFromFile("Assets/Textures/Background/bomb_alarm_0.png"));
    if (!BombAlarmTex1.loadFromFile("Assets/Textures/Background/bomb_alarm_1.png"));

    if (!BallTex.loadFromFile("Assets/Textures/Obstacles/ball.png"));
    for (i = 0; i < CONST_maxBalls; i++)
    {
        ball[i].spr.setTexture(BallTex);
        ball[i].size = BallTex.getSize();
        ball[i].spr.setOrigin(ball[i].size.x / 2, ball[i].size.y / 2);

        ball[i].shadowSpr.setTexture(BombShadowTex);
        ball[i].shadowSpr.setOrigin(bombShadow[0].size.x / 2, bombShadow[0].size.y / 2);
    }

    for (i = 0; i < 11; i++)
    {
        fileName = "Assets/Textures/Obstacles/vendor_";
        if (i < 10)
            fileName += char('0' + i);
        else
        {
            fileName += char('0' + i / 10);
            fileName += char('0' + i % 10);
        }
        fileName += ".png";
        if (!VendorTex[i].loadFromFile(fileName));
    }

    for (i = 0; i < CONST_maxVendorItems; i++)
    {
        fileName = "Assets/Textures/Obstacles/vitem_";
        fileName += char('0' + i);
        fileName += ".png";
        if (!VendorItemTex[i].loadFromFile(fileName));
    }

    if (!AuraTex.loadFromFile("Assets/Textures/Obstacles/aura.png"));
    vendor.auraSpr.setTexture(AuraTex);

    if (!SeaTex.loadFromFile("Assets/Textures/Background/sea.png"));
    if (!SeaTex1.loadFromFile("Assets/Textures/Background/sea_mirrored.png"));
    SeaSpr.setTexture(SeaTex);
    SeaSpr1.setTexture(SeaTex1);

    if (!SearoadTex.loadFromFile("Assets/Textures/Background/searoad.png"));
    if (!SearoadTex1.loadFromFile("Assets/Textures/Background/searoad_mirrored.png"));
    if (!SeashineTex.loadFromFile("Assets/Textures/Background/seashine.png"));
    SeashineSpr.setTexture(SeashineTex);

    for (i = 0; i < CONST_maxFishes; i++)
        for (j = 0 ; j < 2; j++)
            for (l = 0; l < CONST_fishFrames[i]; l++)
            {
                fileName = "Assets/Textures/Entities/fish_";
                fileName += char('0' + i);
                fileName += '_';
                fileName += char('0' + j);
                fileName += '_';
                fileName += char('0' + l);
                fileName += ".png";
                if (!FishTex[i][j][l].loadFromFile(fileName));

                for (int ii = 0; ii < 4; ii++)
                    fish[i][ii].size = FishTex[i][j][l].getSize();
            }

    for (i = 0; i < CONST_maxFishes; i++)
    {
        for (j = 0; j < CONST_fishShadows[i]; j++)
        {
            fileName = "Assets/Textures/Entities/fish_";
            fileName += char('0' + i);
            fileName += "_shadow_";
            fileName += char('0' + j);
            fileName += ".png";
            if (!FishShadowTex[i][j].loadFromFile(fileName));

            for (l = 0; l < 4; l++)
            {
                fish[i][l].shadowSpr.setTexture(FishShadowTex[i][j]);
                fish[i][l].shadowSpr.setOrigin(0, fish[i][l].size.y);
            }
        }
    }

    if (!CarAuraTex.loadFromFile("Assets/Textures/Cars/car_aura.png"));
    CarAuraSpr.setTexture(CarAuraTex);
    CarAuraSpr.setOrigin(CarAuraTex.getSize().x / 2, CarAuraTex.getSize().y / 2);

    if (!CornAuraTex.loadFromFile("Assets/Textures/Cars/corn_aura.png"));
    CornAuraSpr.setTexture(CornAuraTex);
    CornAuraSpr.setOrigin(CornAuraTex.getSize().x / 2, CornAuraTex.getSize().y / 2);

    if (!ImmunityAuraTex.loadFromFile("Assets/Textures/Cars/immunity_aura.png"));
    ImmunityAuraSpr.setTexture(ImmunityAuraTex);
    ImmunityAuraSpr.setOrigin(ImmunityAuraTex.getSize().x / 2, ImmunityAuraTex.getSize().y / 2);

    if (!whale.tex1.loadFromFile("Assets/Textures/Background/whale1.png"));
    if (!whale.tex2.loadFromFile("Assets/Textures/Background/whale2.png"));
    if (!whale.tex3.loadFromFile("Assets/Textures/Background/whale3.png"));
    if (!whale.tex4.loadFromFile("Assets/Textures/Background/whale4.png"));
    if (!whale.eyeTex1.loadFromFile("Assets/Textures/Background/whale_eye1.png"));
    if (!whale.eyeTex2.loadFromFile("Assets/Textures/Background/whale_eye2.png"));
    whale.Construct();

    if (!DarknessTex.loadFromFile("Assets/Textures/Background/darkness.png"));
    DarknessSpr.setTexture(DarknessTex);
    if (!CapsuleTex.loadFromFile("Assets/Textures/Cars/capsule.png"));
    CapsuleSpr.setTexture(CapsuleTex);
    if (!CarAbsentTex.loadFromFile("Assets/Textures/Cars/car3_a.png"));
    if (!CapsuleBrokenTex.loadFromFile("Assets/Textures/Cars/capsule_broken.png"));
    CapsuleBrokenSpr.setTexture(CapsuleBrokenTex);
    if (!VendorWindowTex.loadFromFile("Assets/Textures/HUD/vendor_window.png"));
    if (!VendorWindowTexBad.loadFromFile("Assets/Textures/HUD/vendor_window_bad.png"));
    if (!VendorWindowTexGood.loadFromFile("Assets/Textures/HUD/vendor_window_good.png"));
    if (!DonutTex.loadFromFile("Assets/Textures/Weapons/donut.png"));
    DonutSpr.setTexture(DonutTex);
    if (!CursorTex.loadFromFile("Assets/Textures/HUD/cursor.png"));
    CursorSpr.setTexture(CursorTex);
    if (!SettingsTex.loadFromFile("Assets/Textures/HUD/settings.png"));
    SettingsSpr.setTexture(SettingsTex);
    if (!BallTex1.loadFromFile("Assets/Textures/HUD/ball1.png"));
    BallSpr1.setTexture(BallTex1);
    if (!BallTex2.loadFromFile("Assets/Textures/HUD/ball2.png"));
    BallSpr2.setTexture(BallTex2);
    if (!BarTex1.loadFromFile("Assets/Textures/HUD/bar1.png"));
    BarSpr1.setTexture(BarTex1);
    if (!BarTex2.loadFromFile("Assets/Textures/HUD/bar2.png"));
    BarSpr2.setTexture(BarTex2);
    if (!WhiteBarTex.loadFromFile("Assets/Textures/HUD/whitebar.png"));
    WhiteBarSpr.setTexture(WhiteBarTex);
    if (!SettingsButtonTex.loadFromFile("Assets/Textures/HUD/settings_button.png"));
    SettingsButtonSpr.setTexture(SettingsButtonTex);
    SettingsButtonSpr.setPosition(1630, 914);
    if (!CheckTex.loadFromFile("Assets/Textures/HUD/check.png"));
    for (i = 0; i < 10; i++)
        CheckSpr[i].setTexture(CheckTex);
    CheckSpr[0].setPosition(646, 460);
    CheckSpr[1].setPosition(708, 528);
    if (!ArrowTex1.loadFromFile("Assets/Textures/HUD/arrow1.png"));
    ArrowSpr1.setTexture(ArrowTex1);
    if (!ArrowTex2.loadFromFile("Assets/Textures/HUD/arrow2.png"));
    ArrowSpr2.setTexture(ArrowTex2);
    ArrowSpr1.setOrigin(ArrowTex1.getSize().x, 0);
    i = Wx / 2 - 20;
    ArrowSpr1.setPosition(i - 350, 470);
    ArrowSpr2.setPosition(i + 350, 470);
    if (!DriveTex.loadFromFile("Assets/Textures/HUD/drive.png"));
    DriveSpr.setTexture(DriveTex);
    DriveSpr.setPosition(786, 838);

    whaleQuoteText.setFont(whalefont);
    whaleQuoteText.setCharacterSize(44);
    whaleQuoteText.setFillColor(sf::Color(63, 56, 156));
    whaleQuoteText.setOutlineColor(sf::Color(0, 0, 0));
    whaleQuoteText.setOutlineThickness(3);

    moneyText.setFont(terar);
    moneyText.setCharacterSize(48);
    moneyText.setFillColor(sf::Color(0, 150, 70));
    moneyText.setOutlineColor(sf::Color(0, 0, 0));
    moneyText.setOutlineThickness(5);

    vendorMoneyText.setFont(terar);
    vendorMoneyText.setCharacterSize(46);
    vendorMoneyText.setFillColor(sf::Color(0, 140, 60));
    vendorMoneyText.setOutlineColor(sf::Color(0, 0, 0));
    vendorMoneyText.setOutlineThickness(3);

    vendorQuoteText.setFont(terar);
    vendorQuoteText.setCharacterSize(22);
    vendorQuoteText.setFillColor(sf::Color(255, 200, 200));
    vendorQuoteText.setOutlineColor(sf::Color(0, 0, 0));
    vendorQuoteText.setOutlineThickness(2);
    vendorQuoteText.setPosition(34, 880);

    for (i = 0; i < CONST_maxMoneybags; i++)
    {
        fileName = "Assets/Textures/Obstacles/moneybag_";
        fileName += char('0' + i);
        fileName += ".png";
        if (!moneybag[i].tex.loadFromFile(fileName));

        fileName = "Assets/Textures/Obstacles/moneybag_";
        fileName += char('0' + i);
        fileName += "_shadow.png";
        if (!moneybag[i].shadowTex.loadFromFile(fileName));

        moneybag[i].value = CONST_moneybagValue[i];

        moneybag[i].Construct();
    }

    for (i = 0; i < CONST_oyeNumber; i++)
    {
        fileName = "Assets/Textures/Effects/Boom/oye";
        fileName += char('0' + i);
        fileName += ".png";
        if (!OyeTex[i].loadFromFile(fileName));
    }
}

void ScoreToText()
{
    long long aux, mscore = 0;
    std::string scoreString;

    scoreDigits = 0;

    aux = score;
    do
    {
        scoreDigits++;
        mscore *= 10;
        mscore += aux % 10;
        aux /= 10;
    } while (aux);

    scoreString = "Score: ";
    while (scoreDigits)
    {
        scoreString += char('0' + mscore % 10);
        mscore /= 10;
        scoreDigits--;
        if (scoreDigits % 3 == 0)
            scoreString += ' ';
    }

    scoreText.setString(scoreString);
}

void HighscoreToText()
{
    long long aux, mscore = 0;
    std::string scoreString;

    scoreDigits = 0;

    aux = highscore[drink];
    do
    {
        scoreDigits++;
        mscore *= 10;
        mscore += aux % 10;
        aux /= 10;
    } while (aux);

    scoreString = "Highscore: ";
    while (scoreDigits)
    {
        scoreString += char('0' + mscore % 10);
        mscore /= 10;
        scoreDigits--;
        if (scoreDigits % 3 == 0)
            scoreString += ' ';
    }

    highscoreMenu.setString(scoreString);
    highscoreMenu.setOrigin(highscoreMenu.getGlobalBounds().getSize().x / 2, 0);
    highscoreMenu.setPosition(290, 890);
}

void BonusScoreToText()
{
    long long aux, mscore = 0;
    std::string scoreString;

    scoreDigits = 0;

    aux = bonusScoreGain;
    do
    {
        scoreDigits++;
        mscore *= 10;
        mscore += aux % 10;
        aux /= 10;
    } while (aux);

    scoreString = "+ ";
    while (scoreDigits)
    {
        scoreString += char('0' + mscore % 10);
        mscore /= 10;
        scoreDigits--;
        if (scoreDigits % 3 == 0)
            scoreString += ' ';
    }

    bonusScoreText.setString(scoreString);
}

void PlayerMoneyToText()
{
    long long aux, mscore = 0;
    int localDigits;
    std::string localString;

    localDigits = 0;

    aux = playerMoney;
    do
    {
        localDigits++;
        mscore *= 10;
        mscore += aux % 10;
        aux /= 10;
    } while (aux);

    localString = "Money: $";
    while (localDigits)
    {
        localString += char('0' + mscore % 10);
        mscore /= 10;
        localDigits--;
        if (localDigits % 3 == 0 and localDigits)
            localString += ',';
    }

    moneyText.setString(localString);
    //moneyText.setOrigin(moneyText.getGlobalBounds().getSize().x, 0);
    moneyText.setPosition(35, 110);
}

void HudiconToText(int i)
{
    int mtime, t1, t2;
    std::string localString;
    
    if (i == 0)
        localString = std::to_string(bulletCount);
    else if (i == 9)
        localString = "";
    else
    {
        mtime = hudicon[i].time + 6;
        t1 = mtime / 60;
        t2 = mtime % 60 / 6;

        localString = std::to_string(t1);
        localString += '.';
        localString += char('0' + t2);
        localString += 's';
    }

    hudicon[i].txt.setString(localString);
}

void EndscreenToText(int i)
{
    long long aux, mscore = 0, localValue;
    std::string localString;

    localString = endscreenString[drink][i];

    if (drink == 3)
    {
        if (!hasBeenUnderwater)
        {
            if (i == 4 or i == 5)
                localString = "???: ";
        }
        if (!hasSeenWhale)
        {
            if (i == 6)
                localString = "???: ";
        }
    }

    scoreDigits = 0;
    
    localValue = endscreenCurrentValue;
    if (endscreenEventTimer == 0)
        localValue = endscreenValue[i];

    if (i == 1)
    {
        aux = localValue / 600;
        if (aux)
        {
            do
            {
                scoreDigits++;
                mscore *= 10;
                mscore += aux % 10;
                aux /= 10;
            } while (aux);

            while (scoreDigits)
            {
                localString += char('0' + mscore % 10);
                mscore /= 10;
                scoreDigits--;
                if (scoreDigits % 3 == 0 and scoreDigits)
                    localString += ' ';
            }
            localString += "m ";
        }
        aux = localValue % 600;
        if (aux >= 100)
            localString += char('0' + aux / 100);
        localString += char('0' + aux % 100 / 10);
        if (localValue < 600)
        {
            localString += '.';
            localString += char('0' + aux % 10);
        }
        localString += 's';
    }
    else
    {
        aux = localValue;

        do
        {
            scoreDigits++;
            mscore *= 10;
            mscore += aux % 10;
            aux /= 10;
        } while (aux);

        while (scoreDigits)
        {
            localString += char('0' + mscore % 10);
            mscore /= 10;
            scoreDigits--;
            if (scoreDigits % 3 == 0 and scoreDigits != 0)
            {
                if (drink == 3 and i == 2)
                    localString += ',';
                else
                    localString += ' ';
            }
        }

        if (i == 0 and endscreenEventTimer == 0 and newHighscoreReached)
        {
            newHighscoreReachedEnd = 1;
            newHighscoreEnd.setPosition(endscreenText[i].getPosition().x + endscreenText[i].getGlobalBounds().getSize().x + 70, endscreenText[i].getPosition().y);
        }
    }

    endscreenText[i].setString(localString);
}

void GenQuote()
{
    int i;
    std::string quoteString;

    if (usedQuotesNumber[drink] == quotesNumber[drink])
    {
        usedQuotesNumber[drink] = 0;
        for (i = 0; i < quotesNumber[drink]; i++)
            usedQuotes[drink][i] = 0;
    }

    if (quotesNumber[drink] == 0)
        return;

    Rand = rand() % quotesNumber[drink];
    for (i = 0; i < 10 and usedQuotes[drink][Rand]; i++)
        Rand = rand() % quotesNumber[drink];

    for (i = Rand; usedQuotes[drink][i]; i++, i %= quotesNumber[drink]);

    usedQuotes[drink][i] = 1;

    usedQuotesNumber[drink]++;

    if (drink == 0)
        quoteString = "Driver: ";
    else if (drink == 1)
        quoteString = "Drivr: ";
    else if (drink == 2)
        quoteString = "Dreiver: ";
    else if (drink == 3)
        quoteString = "Driveer: ";

    quoteString += quotes[drink][i];
    quoteText.setString(quoteString);
}

void GenWhaleQuote()
{
    int i;

    if (usedWhaleQuotesNumber == whaleQuotesNumber)
    {
        usedWhaleQuotesNumber = 0;
        for (i = 0; i < whaleQuotesNumber; i++)
            usedWhaleQuotes[i] = 0;
    }

    if (whaleQuotesNumber == 0)
        return;

    Rand = rand() % whaleQuotesNumber;
    for (i = 0; i < 10 and usedWhaleQuotes[Rand]; i++)
        Rand = rand() % whaleQuotesNumber;

    for (i = Rand;  usedWhaleQuotes[i]; i++, i %= whaleQuotesNumber);

    usedWhaleQuotes[i] = 1;

    usedWhaleQuotesNumber++;

    whale.fullQuote = whaleQuotes[i];
    whaleQuoteText.setString("");
}

void GenSong()
{
    int i, j, ldrink = drink;
    if (ldrink == 0)
        ldrink = 1;

    if (usedSongsNumber[ldrink] == CONST_numberOfSongs[ldrink])
    {
        usedSongsNumber[ldrink] = 0;
        for (i = 0; i < CONST_numberOfSongs[ldrink]; i++)
            usedSong[ldrink][i] = 0;
    }

    Rand = rand() % CONST_numberOfSongs[ldrink];
    for (i = 0; i < 10 and usedSong[ldrink][Rand]; i++)
        Rand = rand() % CONST_numberOfSongs[ldrink];

    for (i = Rand; usedSong[ldrink][i]; i++, i %= CONST_numberOfSongs[ldrink]);

    usedSong[ldrink][i] = 1;
    usedSongsNumber[ldrink]++;

    CurrentSong.setBuffer(SongB[ldrink][i]);
}

void GenASong()
{
    int i, j, ldrink = drink;
    if (ldrink == 0)
        ldrink = 1;

    if (usedASongsNumber[ldrink] == CONST_numberOfASongs[ldrink])
    {
        usedASongsNumber[ldrink] = 0;
        for (i = 0; i < CONST_numberOfASongs[ldrink]; i++)
            usedASong[ldrink][i] = 0;
            usedASong[ldrink][i] = 0;
    }

    Rand = rand() % CONST_numberOfASongs[ldrink];
    for (i = 0; i < 10 and usedASong[ldrink][Rand]; i++)
        Rand = rand() % CONST_numberOfASongs[ldrink];

    for (i = Rand; usedASong[ldrink][i]; i++, i %= CONST_numberOfASongs[ldrink]);

    usedASong[ldrink][i] = 1;
    usedASongsNumber[ldrink]++;

    AlternateSong.setBuffer(ASongB[ldrink][i]);
}

void OverwriteSave()
{
    fout.open("Assets/Strings/save.txt", std::ofstream::trunc);
    for (int i = 0; i < CONST_maxDrinks; i++)
        fout << highscore[i] << '\n';
    fout << '\n';
    fout << BallSpr1.getPosition().x << '\n';
    fout << BallSpr2.getPosition().x << '\n';
    fout << fullscreenOn << '\n';
    fout << killedJL << '\n';
    fout.close();
}

bool CarSpawnCondition()
{
    if (!hardmode)
    {
        if (tideActive or tideRest < 0 or CONST_defaultSeaLevel - seaLevel > 0)
            if (tideLane == 0)
                return 0;
            else if (tideLane == 1)
                return activeCars == 0;
            else
                return activeCars < 1;

        if (activeEvent == 3)
        {
            if (activeCars < 3)
                return 1;
            else
                return 0;
        }
        if (activeCars < 2)
        {
            if (activeEvent == 0)
                return 1;
            else if (activeEvent == 1)
                return eventTime <= Wx / 2 or eventTime >= 5 * Wx / 4;
            else if (activeEvent == 2)
                return 1;
            else if (activeEvent == 4)
                return 1;
            else if (activeEvent == 5)
                return 1;
        }
        else
            return 0;
    }
    return 0;
}

bool CarSpawnChance()
{
    Rand = rand() % 1000;

    if (drink == 3 and tideActive)
        return Rand < 60;

    if (!carChaseActive)
        return Rand < 45; // era 35,    32 hardmode
    else
        return Rand < 27; // era 25,     22 hardmode
}

void EndscreenManager()
{
    endscreenValue[0] = score - 5;
    endscreenValue[1] = timeCounter;
    maxEndscreenEvents = 2;

    if (drink == 1)
    {
        maxEndscreenEvents = 7;
        endscreenValue[2] = endscreenBananaCounter;
        endscreenValue[3] = endscreenShotCounter;
        endscreenValue[4] = endscreenKidCounter;
        endscreenValue[5] = endscreenPoliceCounter;
        endscreenValue[6] = endscreenRpgCounter;
    }
    else if (drink == 2)
    {
        maxEndscreenEvents = 8;
        endscreenValue[2] = endscreenBananaCounter;
        endscreenValue[3] = endscreenShotsFired;
        endscreenValue[4] = endscreenSlowdownCounter;
        endscreenValue[5] = endscreenCarsDestroyed;
        endscreenValue[6] = endscreenAirstrikeCounter;
        endscreenValue[7] = endscreenBlizzardCounter;
    }
    else if (drink == 3)
    {
        maxEndscreenEvents = 7;
        endscreenValue[2] = endscreenMoneySpent;
        endscreenValue[3] = endscreenBallCounter;
        endscreenValue[4] = endscreenGoldfishCounter;
        endscreenValue[5] = endscreenJellyfishCounter;
        endscreenValue[6] = endscreenWhaleCounter;
    }

    endscreenText[0].setPosition(650, -500);
    for (int i = 1; i < CONST_maxEndscreenText; i++)
    {
        endscreenText[i].setPosition(650, endscreenText[i - 1].getPosition().y + endscreenText[i - 1].getCharacterSize() + 5);
        if (i == 2)
            endscreenText[i].move(0, 30);
    }

    currentEndscreenEvent = -1;
    endscreenEventTimer = 0;
}

int CarSpawnRNG()
{
    int S = 0, S2 = 0, carCount = 0, i;
    if (drink == 0)
        carCount = 5;
    else if (drink == 1)
        carCount = 10;
    else if (drink == 2)
        carCount = 12;
    else
        carCount = CONST_enemyCarCount;

    for (i = 0; i < carCount; i++)
        S += CONST_carSpawnChance[i];

    Rand = rand() % S;

    for (i = 0; i < carCount; i++)
    {
        S2 += CONST_carSpawnChance[i];
        if (Rand < S2)
        {
            if (i == 8 and (carChaseActive or (activeEvent != 0 and activeEvent != 5)))
                i = 0;
            if (i == 5 and enemyCar[6][0].active[0])
                i = 0;
            return i + 1;
        }
    }

}

void CarSpawn()
{
    Rand = rand() % 4;
    while (lastCar[Rand] < 0)
        Rand = rand() % 4;

    if (yLane[Rand] > seaLevel)
        return;

    newLane = Rand;

    int i, j = 0, localSpeed = 0;

    i = CarSpawnRNG();

    if (i == 1)
        localSpeed = 6 + rand() % 5;
    else if (i == 2)
        localSpeed = 7 + rand() % 5;
    else if (i == 3)
        localSpeed = 6 + rand() % 5;
    else if (i == 4)
        localSpeed = 5 + rand() % 5;
    else if (i == 5)
        localSpeed = 8 + rand() % 4;
    else if (i == 6)
        localSpeed = 5 + rand() % 3;
    else if (i == 7)
        localSpeed = 6 + rand() % 5;
    else if (i == 8)
        localSpeed = 6 + rand() % 5;
    else if (i == 9)
        localSpeed = 20;
    else if (i == 10)
        localSpeed = 7 + rand() % 5;
    else 
        localSpeed = 7 + rand() % 5;

    if (i == 6)
    {
        if (imagineTime == 0 and !killedJL)
            ImagineSound.play();
        imagineTime = -90;
    }

    activeCars++;

    while (enemyCar[i][0].active[j])
        j++;

    enemyCar[i][0].speed[j] = localSpeed;
    enemyCar[i][0].hp[j] = 3;

    for (int l = 0; l < 3; l++)
    {
        enemyCar[i][l].spr[j].setPosition(Wx, yLane[newLane]);
    }

    if (i == 9)
    {
        enemyCar[i][0].speed[j] = 20;
        enemyCar[i][0].hp[j] = 5;

        if (AmbulanceSound[j].getStatus() == sf::SoundSource::Status::Playing)
            AmbulanceSound[j].stop();
        AmbulanceSound[j].play();

        for (int l = 0; l < 3; l++)
        {
            enemyCar[i][l].spr[j].setPosition(2 * Wx, yLane[newLane]);
        }
    }

    if (activeEvent == 3)
        enemyCar[i][0].speed[j] -= 2;

    enemyCar[i][0].Lane[j] = newLane;

    lastCar[newLane] = 0;
    lastCar[newLane] -= enemyCar[i][0].size.x;
    lastCarRate[newLane] = roadSpeed + enemyCar[i][0].speed[j];

    for (int l = 0; l < 3; l++)
        if (ieCar[l].x == i and ieCar[l].y == j)
            enemyCar[i][0].active[j] = 1;

    if (!enemyCar[i][0].active[j])
    {
        for (int l = 0; l < 3; l++)
            if (ieCar[l].x == -1 or !enemyCar[ieCar[l].x][0].active[ieCar[l].y])
            {
                enemyCar[i][0].active[j] = 1;
                ieCar[l].x = i;
                ieCar[l].y = j;
                l = 3;
            }
    }

    enemyCar[i][0].active[j] = 1;
}

void FishSpawn()
{
    Rand = rand() % 6;
    for (int i = 0; i < 10 and usedFlane[Rand]; i++)
        Rand = rand() % 6;

    if (usedFlane[Rand])
        return;

    newLane = Rand;

    int i, j = 0, localSpeed = 0, S = 0, S2 = 0, fishCount = CONST_maxFishes;

    for (i = 0; i < fishCount; i++)
        S += CONST_fishSpawnChance[i];

    Rand = rand() % S;

    for (i = 0; i < fishCount; i++)
    {
        S2 += CONST_fishSpawnChance[i];
        if (Rand < S2)
            break;
    }

    if (i == 5 and bellfishEquipped)
        i = 0;

    activeFishes++;

    while (fish[i][j].active)
        j++;

    fish[i][j].dir = rand() % 2;

    if (i == 0)
    {
        if (fish[i][j].dir == 0)
            localSpeed = roadSpeed + rand() % 3;
        else
            localSpeed = int(roadSpeed / 2) + rand() % 6;
    }
    else if (i == 1)
    {
       fish[i][j].speed0 = roadSpeed;
       fish[i][j].speed1 = int(roadSpeed / 2) + rand() % 3;

       if (fish[i][j].dir == 0)
           localSpeed = fish[i][j].speed0;
       else
           localSpeed = fish[i][j].speed1;

        fish[i][j].rotates = 1 + rand() % 3;
    }
    else if (i == 2)
    {
        if (fish[i][j].dir == 0)
            localSpeed = roadSpeed;
        else
            localSpeed = int(roadSpeed / 2) + rand() % 3;
    }
    else if (i == 3)
    {
        if (fish[i][j].dir == 0)
            localSpeed = roadSpeed + rand() % 3;
        else
            localSpeed = int(roadSpeed / 2) + rand() % 6;
    }
    else if (i == 4)
    {
        if (fish[i][j].dir == 0)
            localSpeed = roadSpeed + rand() % 3;
        else
            localSpeed = int(roadSpeed / 2) + rand() % 6;

        if (fish[i][j].dir == 0)
            fish[i][j].shadowSpr.setTexture(FishShadowTex[i][0]);
        else
            fish[i][j].shadowSpr.setTexture(FishShadowTex[i][2]);

        fish[i][j].state = 0;
    }
    else if (i == 5)
    {
        if (fish[i][j].dir == 0)
            localSpeed = roadSpeed + rand() % 3;
        else
            localSpeed = int(roadSpeed / 2) + rand() % 6;
    }
    else if (i == 6)
    {

        if (fish[i][j].dir == 0)
            localSpeed = roadSpeed + rand() % 3;
        else
            localSpeed = int(roadSpeed / 2) + rand() % 6;

        if (fish[i][j].dir == 0)
            fish[i][j].shadowSpr.setTexture(FishShadowTex[i][0]);
        else
            fish[i][j].shadowSpr.setTexture(FishShadowTex[i][1]);
    }

    fish[i][j].speed = localSpeed;
    fish[i][j].active = 1;
    fish[i][j].Flane = newLane;
    fish[i][j].frame = 0;
    fish[i][j].time = 0;
    fish[i][j].nframe = 0;
    usedFlane[newLane] = 1;

    fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][0]);
    fish[i][j].spr.setOrigin(0, fish[i][j].size.y);

    if (!fish[i][j].dir)
        fish[i][j].spr.setPosition(Wx, yFlane[newLane]), fish[i][j].shadowSpr.setPosition(Wx, yFlane[newLane]);
    else
        fish[i][j].spr.setPosition(-int(fish[i][j].size.x), yFlane[newLane]), fish[i][j].shadowSpr.setPosition(-int(fish[i][j].size.x), yFlane[newLane]);
}

bool IsAnyKeyPressed()
{
    for (int k = -1; k < sf::Keyboard::KeyCount; ++k)
    {
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(k)))
            return true;
    }
    return false;
}


void GenOye()
{
    Rand = rand() % CONST_oyeNumber;
    OyeSpr.setTexture(OyeTex[Rand]);
    oyeOpacity = 240;
    OyeSpr.setColor(sf::Color(255, 255, 255, oyeOpacity));
    oyeTime = -40;
    RpgPickupSound.play();
}

void OyeDetector()
{
    if (IsAnyKeyPressed())
    {
        switch (oyeLevel)
        {
        case 2:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
                GenOye(), oyeLevel = 0;
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y))
                oyeLevel = 0;
            break;

        case 1:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y))
                oyeLevel++;
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O))
                oyeLevel = 0;
            break;

        case 0:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O))
                oyeLevel++;
            break;

        default:
            break;
        }
    }
}

void VendorMoneyToText()
{
    long long aux, mscore = 0;
    int localDigits;
    std::string localString;

    localDigits = 0;

    aux = vendor.price;
    do
    {
        localDigits++;
        mscore *= 10;
        mscore += aux % 10;
        aux /= 10;
    } while (aux);

    localString = "$";
    while (localDigits)
    {
        localString += char('0' + mscore % 10);
        mscore /= 10;
        localDigits--;
        if (localDigits % 3 == 0 and localDigits)
            localString += ',';
    }

    vendorMoneyText.setString(localString);
    vendorMoneyText.setOrigin(vendorMoneyText.getGlobalBounds().getSize().x / 2, vendorMoneyText.getGlobalBounds().getSize().y / 2);
    vendorMoneyText.setPosition(242, 810);
}

void VendorQuoteToText()
{
    if (vendorQuotesNumber == 0 or vendorQuotesNumberGood == 0 and vendorQuotesNumberBad == 0)
        return;

    if (vendor.good)
    {
        Rand = rand() % vendorQuotesNumberGood;
        vendorQuoteText.setString(vendorQuotesGood[Rand]);
        return;
    }

    if (vendor.bad)
    {
        Rand = rand() % vendorQuotesNumberBad;
        vendorQuoteText.setString(vendorQuotesBad[Rand]);
        return;
    }

    Rand = rand() % vendorQuotesNumber;
    vendorQuoteText.setString(vendorQuotes[Rand]);
}

void ExplodeEnemyCar(int i, int j)
{
    int l;
    sf::Vector2f P1;
    P1 = enemyCar[i][0].spr[j].getPosition();

    enemyCar[i][0].active[j] = 0;
    activeCars--;

    for (l = 0; l < CONST_maxExplosions and explosion[l].active; l++);
    if (l == CONST_maxExplosions)
        l = 0;

    if (!explosion[l].tex.loadFromFile("Assets/Textures/Effects/Explosion/frame25.png"));
    explosion[l].sound.play();
    explosion[l].spr.setTexture(explosion[l].tex);
    explosion[l].scale = 4;
    explosion[l].spr.setScale(explosion[l].scale, explosion[l].scale);
    if (i == 9)
        explosion[l].spr.setPosition(P1.x + enemyCar[i][0].size.x / 4, P1.y - 3 * enemyCar[i][0].size.y / 2);
    else
        explosion[l].spr.setPosition(P1.x, P1.y - 2 * enemyCar[i][0].size.y);
    explosion[l].frame = -1;
    explosion[l].active = 1;

    if (i == 6)
        imagineTime = 0;
}

void ExplodeFish(int i, int j)
{
    int l;
    sf::Vector2f P1;
    P1 = fish[i][j].spr.getPosition();

    fish[i][j].active = 0;
    activeFishes--;
    usedFlane[fish[i][j].Flane] = 0;

    for (l = 0; l < CONST_maxExplosions and blood[l].active; l++);
    if (l == CONST_maxExplosions)
        l = 0;

    if (!blood[l].tex.loadFromFile("Assets/Textures/Effects/Blood/frame0.png"));
    blood[l].spr.setTexture(blood[l].tex);
    blood[l].spr.setOrigin(blood[l].tex.getSize().x / 2, blood[l].tex.getSize().y / 2);
    blood[l].spr.setPosition(P1.x + fish[i][j].size.x / 2, P1.y);
    blood[l].sound.play();
    blood[l].frame = -1;
    blood[l].active = 1;
}

void ExplodePlayerCar()
{
    int l;
    sf::Vector2f p1;
    p1 = car[playerCar][0].spr.getPosition();

    status = 2;

    for (l = 0; l < CONST_maxExplosions and explosion[l].active; l++);
    if (l == CONST_maxExplosions)
        l = 0;

    if (!explosion[l].tex.loadFromFile("Assets/Textures/Effects/Explosion/frame25.png"));
    explosion[l].sound.play();
    explosion[l].spr.setTexture(explosion[l].tex);
    explosion[l].scale = 4;
    explosion[l].spr.setScale(explosion[l].scale, explosion[l].scale);
    explosion[l].spr.setPosition(p1.x - car[playerCar][0].size.x / 2, p1.y - 3 * car[playerCar][0].size.y / 2);
    explosion[l].frame = -1;
    explosion[l].active = 1;
}

bool DonutCollision(sf::Vector2f p1, sf::Vector2f P1)
{
    long double aux = donutScale * donutScale;
    long long aux2 = ceil(aux);
    long long side1 = (p1.x - P1.x) * (p1.x - P1.x) + (p1.y - P1.y) * (p1.y - P1.y);
    long long side2 = aux2 * DonutTex.getSize().x * DonutTex.getSize().x / 4;

    return side1 <= side2;
}

void AddObject(sf::Sprite spr, float y, int priority)
{
    object[objectCount].spr = spr;
    object[objectCount].y = y;
    object[objectCount].priority = priority;
    objectCount++;
}

void DrawStuff(sf::RenderWindow &window)
{
    sf::Vector2f p1 = car[playerCar][playerCarFrame].spr.getPosition();

    window.draw(RoadSpr);                                   
    window.draw(RoadSpr1);

    if (drink == 3)
    {
        if (activeEvent != 6 or whale.active)
        {
            window.draw(SeaSpr);
            window.draw(SeaSpr1);
        }
    }

    if (whale.active)
        window.draw(whale.spr1);

    if (whale.onShore and !whale.active)
    {
        window.draw(CapsuleBrokenSpr);
    }

    if (drink < 2)
    {
        for (int i = 0; i < 12; i++)
            if (tree[i].active)
                window.draw(tree[i].spr);
    }

    for (int i = 0; i < bombCount; i++)
        window.draw(target[i].spr);

    for (int i = 0; i < CONST_maxBalls; i++)
        if (ball[i].active)
            window.draw(ball[i].shadowSpr);

    for (int i = 0; i < CONST_maxFishes; i++)
        for (int j = 0; j < 4; j++)
            if (fish[i][j].active)
                window.draw(fish[i][j].shadowSpr);

    for (int i = 0; i < CONST_maxMoneybags; i++)
        if (moneybag[i].active)
            window.draw(moneybag[i].shadowSpr);

    objectCount = 0;

    if (!p1Dead and (!whale.carDespawn or whale.onShore))
        AddObject(car[playerCar][playerCarFrame].spr, p1.y + car[playerCar][playerCarFrame].size.y / 2, PRIORITY_car);

    for (int l = 0; l < 3; l++)
    {
        int i, j;
        i = ieCar[l].x;
        j = ieCar[l].y;
        if (i != -1)
            if (enemyCar[i][0].active[j])
                AddObject(enemyCar[i][playerCarFrame].spr[j], yLane[enemyCar[i][0].Lane[j]], PRIORITY_car);
    }

    for (int i = 0; i < CONST_maxBananaCount; i++)
        if (banana.active[i])
            AddObject(banana.spr[i], banana.spr[i].getPosition().y, PRIORITY_banana);

    for (int i = 0; i < CONST_kidCount; i++)
        for (int j = 0; j < 3; j++)
            if (kid[i].active[j])
                AddObject(kid[i].spr[j], kid[i].spr[j].getPosition().y + kid[i].size.y, PRIORITY_kid);

    for (int i = 0; i < CONST_maxExplosions; i++)
        if (explosion[i].active)
            AddObject(explosion[i].spr, explosion[i].spr.getPosition().y + explosion[i].scale * explosion[i].tex.getSize().y, PRIORITY_explosion);

    for (int i = 0; i < CONST_maxExplosions; i++)
        if (blood[i].active)
            AddObject(blood[i].spr, blood[i].spr.getPosition().y + blood[i].tex.getSize().y / 2, PRIORITY_explosion);

    if (carChaseActive)
        AddObject(policeCar[playerCarFrame].spr, policeCar[playerCarFrame].spr.getPosition().y, PRIORITY_car);

    if (shot[0].active)
        AddObject(shot[playerCarFrame].spr, shot[playerCarFrame].spr.getPosition().y + shot[0].size.y / 2, PRIORITY_shot);

    if (ammobox[0].active)
        AddObject(ammobox[playerCarFrame].spr, ammobox[playerCarFrame].spr.getPosition().y + ammobox[0].size.y / 2, PRIORITY_shot);

    if (slowdown[0].active)
        AddObject(slowdown[playerCarFrame].spr, slowdown[playerCarFrame].spr.getPosition().y + slowdown[0].size.y / 2, PRIORITY_shot);

    if (rpg[0].active)
        AddObject(rpg[playerCarFrame].spr, rpg[playerCarFrame].spr.getPosition().y + rpg[0].size.y / 2, PRIORITY_rpg);

    if (rocket.active)
        AddObject(rocket.spr, rocket.spr.getPosition().y + rocket.size.y / 2, PRIORITY_car);

    for (int i = 0; i < CONST_maxBullets; i++)
        if (bullet[i].active)
            AddObject(bullet[i].spr, bullet[i].spr.getPosition().y + bullet[i].size.y / 2, PRIORITY_car);

    for (int i = 0; i < CONST_maxBalls; i++)
        if (ball[i].active)
            AddObject(ball[i].spr, yLane[ball[i].Lane], PRIORITY_car);

    if (vendor.active)
    {
        AddObject(vendor.spr, vendor.spr.getPosition().y, PRIORITY_car);

        if (!vendor.good and !vendor.bad)
        {
            AddObject(vendor.itemSpr, vendor.spr.getPosition().y, PRIORITY_car);
            AddObject(vendor.auraSpr, vendor.spr.getPosition().y, PRIORITY_kid);
        }
    }

    for (int i = 0; i < CONST_maxFishes; i++)
        for (int j = 0; j < 4; j++)
            if (fish[i][j].active)
                AddObject(fish[i][j].spr, fish[i][j].spr.getPosition().y + 150, PRIORITY_car);

    if (goldfishTime < 0)
        AddObject(CarAuraSpr, CarAuraSpr.getPosition().y + CarAuraTex.getSize().y / 2, PRIORITY_car + 1);

    if (cornEquipped and !whale.carDespawn)
        AddObject(CornAuraSpr, CornAuraSpr.getPosition().y + CornAuraTex.getSize().y / 2, PRIORITY_car + 1);

    if (immunityTime < 0)
        AddObject(ImmunityAuraSpr, ImmunityAuraSpr.getPosition().y + ImmunityAuraTex.getSize().y / 2, PRIORITY_car + 2);

    for (int i = 0; i < CONST_maxMoneybags; i++)
        if (moneybag[i].active)
            AddObject(moneybag[i].spr, moneybag[i].shadowSpr.getPosition().y, PRIORITY_shot);

    if (gunflashTime < 0)
    {
        gunflashTime++;
        GunflashSpr.setPosition(p1.x + 250, p1.y - 62);
        AddObject(GunflashSpr, p1.y + car[playerCar][playerCarFrame].size.y / 2, PRIORITY_car);
    }

    std::sort(object, object + objectCount, displayCond);

    for (int i = 0; i < objectCount; i++)
        window.draw(object[i].spr);

    if (donutTime < 0)
        window.draw(DonutSpr);

    if (whale.onShore)
    {
        window.draw(whale.spr3);
        if (whale.active)
            window.draw(CapsuleSpr);
        window.draw(whale.spr4);
    }

    if (whale.active)
    {
        window.draw(whale.spr2);
        window.draw(whale.eyeSpr1);
        window.draw(whale.eyeSpr2);

        if (whale.stop and !whale.onShore)
            window.draw(whaleQuoteText);
    }

    for (int i = 0; i < bombCount; i++)
        window.draw(bombShadow[i].spr);

    for (int i = 0; i < bombCount; i++)
        window.draw(bomb[i].spr);

    if (activeEvent == 4)
        window.draw(PlaneSpr);

    if (carChaseActive)
        window.draw(PoliceSirenSpr);

    if (bombAlarmActive)
        window.draw(BombAlarmSpr);

    if (activeQuote and !whale.stop)
        window.draw(quoteText);

    window.draw(FogSpr);
    window.draw(FogSpr1);
    window.draw(SnowstormSpr);
    window.draw(SnowstormSpr1);
    window.draw(SeashineSpr);

    window.draw(scoreText);

    if (drink == 3)
        window.draw(moneyText);

    if (bonusScoreTime < 0)
    {
        bonusScoreTime++;
        window.draw(bonusScoreText);

        if (bonusScoreTime == 0)
            bonusScoreGain = 0;
    }

    if (highscore[drink] < score and !newHighscoreReached)
    {
        newHighscoreReached = 1;
        newHighscoreTime = -180;
        newHighscore.setPosition(scoreText.getGlobalBounds().getSize().x + 100, 10);
        NewHighscoreSound.play();
    }

    if (newHighscoreTime < 0)
    {
        newHighscoreTime++;
        window.draw(newHighscore);
    }

    if (jackpotTime < 0)
    {
        jackpotText.setPosition(bonusScoreText.getGlobalBounds().getSize().x + 200, 55);
        if ((-jackpotTime) / 15 % 2 == 1)
            window.draw(jackpotText);
        jackpotTime++;
    }

    for (int i = 0; i < CONST_hudiconCount; i++)
        if (hudicon[i].time)
        {
            window.draw(hudicon[i].spr);
            window.draw(hudicon[i].txt);
        }

    for (int i = 0; i < CONST_huditemCount; i++)
        if (huditem[i].active)
            window.draw(huditem[i].spr);

    if (vendor.active)
    {
        window.draw(VendorWindowSpr);
        if (!vendor.good)
        {
            window.draw(hudvitem[vendor.item].spr);
            window.draw(hudvitem[vendor.item].text);
        }
        window.draw(vendorMoneyText);
        window.draw(vendorQuoteText);
    }

    if (activeEvent == 6 and !whale.active)
    {
        window.draw(SeaSpr);
        window.draw(SeaSpr1);
    }

    window.draw(DarknessSpr);
}

void PauseIfPlaying(sf::Sound &sound)
{
    if (sound.getStatus() == sf::SoundSource::Status::Playing)
        sound.pause();
}

void ResumeSound(sf::Sound &sound)
{
    if (sound.getStatus() == sf::SoundSource::Status::Paused)
        sound.play();
}

void PauseAllSounds()
{
    PauseIfPlaying(CurrentSong);
    PauseIfPlaying(AlternateSong);
    PauseIfPlaying(ImagineSound);
    PauseIfPlaying(EngineSound);
    PauseIfPlaying(SlowdownEndSound);
    PauseIfPlaying(TideSound1);
    PauseIfPlaying(TideSound2);
    PauseIfPlaying(BigTideSound);
    PauseIfPlaying(PoliceSiren);
    PauseIfPlaying(EndscreenSound);
    PauseIfPlaying(BombAlarm);
    PauseIfPlaying(PlaneSound);
    PauseIfPlaying(BlizzardSound);
    PauseIfPlaying(BananaSound);
    PauseIfPlaying(SlurpSound);
    PauseIfPlaying(JackpotSound);
    PauseIfPlaying(MaximumComboSound);
    PauseIfPlaying(RpgPickupSound);
    PauseIfPlaying(NewHighscoreSound);
    PauseIfPlaying(AmmoboxSound);
    PauseIfPlaying(SlowdownSound);
    PauseIfPlaying(SlowdownEquipSound);
    PauseIfPlaying(BoingSound);
    PauseIfPlaying(TideRestSound);
    PauseIfPlaying(GlassSound);
    PauseIfPlaying(BiteSound);
    PauseIfPlaying(GoldfishSound);
    PauseIfPlaying(ReturnfishSound);
    PauseIfPlaying(AnglerSound);
    PauseIfPlaying(JellyfishSound);
    PauseIfPlaying(WhalethrowSound);
    PauseIfPlaying(MoneySound);
    PauseIfPlaying(BellfishSound);
    PauseIfPlaying(SeacallSound);
    PauseIfPlaying(VendorGoodSound);
    PauseIfPlaying(VendorBadSound);
    PauseIfPlaying(VendorNewSound);

    for (int i = 0; i < CONST_maxExplosions; i++)
        PauseIfPlaying(explosion[i].sound);

    for (int i = 0; i < 5; i++)
        PauseIfPlaying(AmbulanceSound[i]);

    for (int i = 0; i < 6; i++)
        PauseIfPlaying(BonusSound[i]);
}

void ResumeAllSounds()
{
    //if (!underwaterActive)
        //ResumeSound(CurrentSong);
    //else
       //ResumeSound(AlternateSong);

    ResumeSound(ImagineSound);
    ResumeSound(EngineSound);
    ResumeSound(SlowdownEndSound);
    ResumeSound(TideSound1);
    ResumeSound(TideSound2);
    ResumeSound(BigTideSound);
    ResumeSound(PoliceSiren);
    ResumeSound(EndscreenSound);
    ResumeSound(BombAlarm);
    ResumeSound(PlaneSound);
    ResumeSound(BlizzardSound);
    ResumeSound(BananaSound);
    ResumeSound(SlurpSound);
    ResumeSound(JackpotSound);
    ResumeSound(MaximumComboSound);
    ResumeSound(RpgPickupSound);
    ResumeSound(NewHighscoreSound);
    ResumeSound(AmmoboxSound);
    ResumeSound(SlowdownSound);
    ResumeSound(SlowdownEquipSound);
    ResumeSound(BoingSound);
    ResumeSound(TideRestSound);
    ResumeSound(GlassSound);
    ResumeSound(BiteSound);
    ResumeSound(GoldfishSound);
    ResumeSound(ReturnfishSound);
    ResumeSound(AnglerSound);
    ResumeSound(JellyfishSound);
    ResumeSound(WhalethrowSound);
    ResumeSound(MoneySound);
    ResumeSound(BellfishSound);
    ResumeSound(SeacallSound);
    ResumeSound(VendorGoodSound);
    ResumeSound(VendorBadSound);
    ResumeSound(VendorNewSound);

    for (int i = 0; i < CONST_maxExplosions; i++)
        ResumeSound(explosion[i].sound);

    for (int i = 0; i < 5; i++)
        ResumeSound(AmbulanceSound[i]);

    for (int i = 0; i < 6; i++)
        ResumeSound(BonusSound[i]);
}


void SetSFXVolume()
{
    EngineSound.setVolume(100 * sfxVolume);
    SlowdownEndSound.setVolume(100 * sfxVolume);
    TideSound1.setVolume(100 * sfxVolume);
    TideSound2.setVolume(100 * sfxVolume);
    BigTideSound.setVolume(100 * sfxVolume);
    PoliceSiren.setVolume(100 * sfxVolume);
    EndscreenSound.setVolume(100 * sfxVolume);
    BombAlarm.setVolume(100 * sfxVolume);
    PlaneSound.setVolume(100 * sfxVolume);
    BlizzardSound.setVolume(100 * sfxVolume);
    BananaSound.setVolume(100 * sfxVolume);
    SlurpSound.setVolume(100 * sfxVolume);
    JackpotSound.setVolume(100 * sfxVolume);
    MaximumComboSound.setVolume(100 * sfxVolume);
    RpgPickupSound.setVolume(100 * sfxVolume);
    NewHighscoreSound.setVolume(100 * sfxVolume);
    AmmoboxSound.setVolume(100 * sfxVolume);
    SlowdownSound.setVolume(100 * sfxVolume);
    SlowdownEquipSound.setVolume(100 * sfxVolume);
    BoingSound.setVolume(100 * sfxVolume);
    TideRestSound.setVolume(100 * sfxVolume);
    GlassSound.setVolume(100 * sfxVolume);
    BiteSound.setVolume(100 * sfxVolume);
    GoldfishSound.setVolume(100 * sfxVolume);
    ReturnfishSound.setVolume(100 * sfxVolume);
    AnglerSound.setVolume(100 * sfxVolume);
    JellyfishSound.setVolume(100 * sfxVolume);
    WhalethrowSound.setVolume(100 * sfxVolume);
    MoneySound.setVolume(100 * sfxVolume);
    BellfishSound.setVolume(100 * sfxVolume);
    SeacallSound.setVolume(100 * sfxVolume);
    VendorGoodSound.setVolume(100 * sfxVolume);
    VendorBadSound.setVolume(100 * sfxVolume);
    PoliceClickSound.setVolume(100 * sfxVolume);
    BulletSound.setVolume(100 * sfxVolume);
    BulletHitSound.setVolume(100 * sfxVolume);
    WhaleVoiceSound.setVolume(100 * sfxVolume);
    VendorNewSound.setVolume(100 * sfxVolume);
    RevolverSound.setVolume(100 * sfxVolume);

    for (int i = 0; i < CONST_maxExplosions; i++)
        explosion[i].sound.setVolume(100 * sfxVolume);

    for (int i = 0; i < CONST_maxExplosions; i++)
        blood[i].sound.setVolume(100 * sfxVolume);

    for (int i = 0; i < 5; i++)
        AmbulanceSound[i].setVolume(100 * sfxVolume);

    for (int i = 0; i < 6; i++)
        BonusSound[i].setVolume(100 * sfxVolume);
}

void ProcessSettings(sf::RenderWindow &window)
{
    int dist = MousePosition.x - PrevMousePosition.x;
    sf::Vector2f p1, p2;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (ball1Held)
        {
            BallSpr1.move(dist, 0);

            p1 = BallSpr1.getPosition();
            if (p1.x < -250)
                p1.x = -250;
            if (p1.x > 0)
                p1.x = 0;
            BallSpr1.setPosition(p1);

            p1 = BallSpr1.getPosition();
            p1.x += 250;
            musicVolume = p1.x / 250;

            CurrentSong.setVolume(songVolume * musicVolume);
            AlternateSong.setVolume(alternateVolume * musicVolume);
            MenuSong.setVolume(70 * musicVolume);
        }
        else if (ball2Held)
        {
            BallSpr2.move(dist, 0);

            p1 = BallSpr2.getPosition();
            if (p1.x < -250)
                p1.x = -250;
            if (p1.x > 0)
                p1.x = 0;
            BallSpr2.setPosition(p1);

            p1 = BallSpr2.getPosition();
            p1.x += 250;
            sfxVolume = p1.x / 250;

            SetSFXVolume();
        }
    }

    p1 = BallSpr1.getPosition();
    p2 = BallSpr2.getPosition();

    if (!mouseHeld and sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (MousePosition.x >= 737 + p1.x and MousePosition.x <= 780 + p1.x and MousePosition.y >= 324 and MousePosition.y <= 367) // 737 324, 780 367
        {
            ball1Held = 1;
        }
        else if (MousePosition.x >= 737 + p2.x and MousePosition.x <= 780 + p2.x and MousePosition.y >= 370 and MousePosition.y <= 412)
        {
            ball2Held = 1;
        }
        else if (MousePosition.x >= 1530 and MousePosition.y >= 192 and MousePosition.x <= 1624 and MousePosition.y <= 292)
        {
            if (status == 6)
                escPressed = 1;
            else
                settingsActive = 0;

            OverwriteSave();
        }
        else if (MousePosition.x >= 646 and MousePosition.y >= 465 and MousePosition.x <= 692 and MousePosition.y <= 511)
        {
            window.close();
            if (!fullscreenOn)
                window.create(sf::VideoMode(Wx, Wy), "Drunk Driving Simulator", sf::Style::Fullscreen);
            else
                window.create(sf::VideoMode(Wx, Wy), "Drunk Driving Simulator");

            window.setFramerateLimit(60);
            window.setVerticalSyncEnabled(true);
            window.setMouseCursorVisible(false);

            fullscreenOn = !fullscreenOn;
        }
        else if (MousePosition.x >= 708 and MousePosition.y >= 533 and MousePosition.x <= 755 and MousePosition.y <= 580)
        {
            killedJL = !killedJL;
            if (killedJL)
                RevolverSound.play();
        }
    }
    else if (mouseHeld and !sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        ball1Held = ball2Held = 0;
    }

    BarSpr1.setPosition(p1.x + 250, 0);
    BarSpr2.setPosition(p2.x + 250, 0);

    window.draw(WhiteBarSpr);
    window.draw(BarSpr1);
    window.draw(BarSpr2);
    window.draw(SettingsSpr);
    window.draw(BallSpr1);
    window.draw(BallSpr2);
    if (fullscreenOn)
        window.draw(CheckSpr[0]);
    if (killedJL)
        window.draw(CheckSpr[1]);
}

void DetectSettingsPressed()
{
    sf::Vector2f p1;
    p1 = SettingsButtonSpr.getPosition();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and !mouseHeld)
    {
        if (MousePosition.x >= p1.x and MousePosition.y >= p1.y and MousePosition.x <= p1.x + SettingsButtonTex.getSize().x and MousePosition.y <= p1.y + SettingsButtonTex.getSize().y)
            settingsActive = 1;
    }
}

void DetectMenuStuffPressed()
{
    sf::Vector2f p1, p2, p3;
    p1 = ArrowSpr1.getPosition();
    p2 = ArrowSpr2.getPosition();
    p3 = DriveSpr.getPosition();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and !mouseHeld and !settingsActive)
    {
        if (MousePosition.x <= p1.x and MousePosition.y >= p1.y and MousePosition.x >= p1.x - ArrowTex1.getSize().x and MousePosition.y <= p1.y + ArrowTex1.getSize().y)
            leftPressed = 1;

        if (MousePosition.x >= p2.x and MousePosition.y >= p2.y and MousePosition.x <= p2.x + ArrowTex2.getSize().x and MousePosition.y <= p2.y + ArrowTex2.getSize().y)
            rightPressed = 1;

        if (MousePosition.x >= p3.x and MousePosition.y >= p3.y and MousePosition.x <= p3.x + DriveTex.getSize().x and MousePosition.y <= p3.y + DriveTex.getSize().y)
            spacePressed = 1;
    }

}

void DetectExitButton()
{
    sf::Vector2f p1;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and !mouseHeld and !exitActive)
    {
        p1 = exitButton->spr.getPosition();

        if (MousePosition.x >= p1.x and MousePosition.y >= p1.y and MousePosition.x <= p1.x + exitButton->tex.getSize().x and MousePosition.y <= p1.y + exitButton->tex.getSize().y)
        {
            exitActive = 1;

            exitConfirm = new Button;
            exitConfirm->tex.loadFromFile("Assets/Textures/HUD/exit_confirm.png");
            exitConfirm->spr.setTexture(exitConfirm->tex);
            exitConfirm->spr.setPosition(701, 447);
        }
    }
}

void DetectConfirmExit()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and !mouseHeld)
    {
        if (MousePosition.x >= 766 and MousePosition.y >= 610 and MousePosition.x <= 884 and MousePosition.y <= 680)
            gameExited = 1;
        else if (MousePosition.x >= 999 and MousePosition.y >= 609 and MousePosition.x <= 1128 and MousePosition.y <= 679)
        {
            exitActive = 0;
            delete exitConfirm;

        }
    }
}

void InitLoad()
{
    int i;
    float x, y;
    if (!LoadingTex.loadFromFile("Assets/Textures/Background/loading.png"));
    LoadingSpr.setTexture(LoadingTex);


    fin.open("Assets/Strings/save.txt");
    for (i = 0; i < CONST_maxDrinks; i++)
        fin >> highscore[i];
    fin >> x >> y;
    BallSpr1.setPosition(x, 0);
    BallSpr2.setPosition(y, 0);
    x += 250;
    y += 250;
    musicVolume = x / 250;
    sfxVolume = y / 250;
    CurrentSong.setVolume(songVolume * musicVolume);
    AlternateSong.setVolume(alternateVolume * musicVolume);
    MenuSong.setVolume(70 * musicVolume);
    SetSFXVolume();

    fin >> fullscreenOn >> killedJL;
    fin.close();
}


















int main()
{
    srand(time(NULL));
    InitLoad();
    sf::RenderWindow window;
    if (fullscreenOn)
        window.create(sf::VideoMode(Wx, Wy), "Drunk Driving Simulator", sf::Style::Fullscreen);
    else
        window.create(sf::VideoMode(Wx, Wy), "Drunk Driving Simulator");

    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);
    
    sf::Listener::setGlobalVolume(35);

    status = -1;

    while (window.isOpen())
    {
        sf::Event event;

        keyPressed = leftPressed = rightPressed = upPressed = downPressed = spacePressed = escPressed = 0;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    keyPressed = 1;
                    //if (status != 1)
                    {
                        switch (event.key.scancode)
                        {
                            case sf::Keyboard::Scan::Left:
                                leftPressed = 1;
                                break;
                            case sf::Keyboard::Scan::Right:
                                rightPressed = 1;
                                break;
                            case sf::Keyboard::Scan::A:
                                leftPressed = 1;
                                break;
                            case sf::Keyboard::Scan::D:
                                rightPressed = 1;
                                break;
                            case sf::Keyboard::Scan::Up:
                                upPressed = 1;
                                break;
                            case sf::Keyboard::Scan::Down:
                                downPressed = 1;
                                break;
                            case sf::Keyboard::Scan::W:
                                upPressed = 1;
                                break;
                            case sf::Keyboard::Scan::S:
                                downPressed = 1;
                                break;
                            case sf::Keyboard::Scan::Space:
                                spacePressed = 1;
                                break;
                            case sf::Keyboard::Scan::Enter:
                                spacePressed = 1;
                                break;
                            case sf::Keyboard::Scan::Escape:
                                escPressed = 1;
                                break;
                            default:
                                break;
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        window.clear(sf::Color::Black);

        sf::Vector2f poz, p1, p2;
        
        PrevMousePosition = MousePosition;
        MousePosition = sf::Mouse::getPosition(window);

        if (status == 1)                                                 // driving real !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {
            if (slowdownTime == 0 or (slowdownTime % 2 == 0))
                playerCarFrameCounter++;
            playerCarFrameCounter %= 45;

            playerCarFrame = playerCarFrameCounter / 15;

            leftPressed = rightPressed = upPressed = downPressed = xPressed = zPressed = cPressed = jPressed = kPressed = lPressed = 0;

            bonusScoreValue = 0;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) or sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                leftPressed = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) or sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                rightPressed = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) or sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                upPressed = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) or sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                downPressed = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
                xPressed = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
                zPressed = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
                cPressed = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
                jPressed = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
                kPressed = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
                lPressed = 1;

            endscreenTimeHelper++;
            if (endscreenTimeHelper == 6)
            {
                timeCounter++;
                endscreenTimeHelper = 0;
            }

            if (rpgSpriteOn)
            {
                rpgSpriteOn = 0;
                for (int i = 0; i < 3; i++)
                    car[playerCar][i].spr.setTexture(car[playerCar][i].tex2);
            }

            if (rpgSpriteOff)
            {
                rpgSpriteOff = 0;
                for (int i = 0; i < 3; i++)
                    car[playerCar][i].spr.setTexture(car[playerCar][i].tex);
            }

            if (jellyfishTime < 0)
            {
                jellyfishTime++;

                std::swap(upPressed, downPressed);
                std::swap(rightPressed, leftPressed);
            }

            if (leftPressed and rightPressed)
                leftPressed = rightPressed = 0;

            if (downPressed and upPressed)
                downPressed = upPressed = 0;

            if (bananaed)                                                                                  // car movement
            {
                limitReachedX = limitReachedY = 0;
                for (int i = 0; i < 3; i++)
                {
                    car[playerCar][i].spr.move(carVelocityX / 2, carVelocityY / 2);

                    poz = car[playerCar][i].spr.getPosition();
                    if (carVelocityX < 0 and poz.x < car[playerCar][i].size.x / 2)
                        car[playerCar][i].spr.setPosition(car[playerCar][i].size.x / 2, poz.y), limitReachedX = 1;
                    poz = car[playerCar][i].spr.getPosition();
                    if (carVelocityX > 0 and poz.x > Wx - car[playerCar][i].size.x / 2)
                        car[playerCar][i].spr.setPosition(Wx - car[playerCar][i].size.x / 2, poz.y), limitReachedX = 1;
                        poz = car[playerCar][i].spr.getPosition();
                    if (carVelocityY < 0 and poz.y < treeLine + 90 + car[playerCar][i].size.y / 2)
                        car[playerCar][i].spr.setPosition(poz.x, treeLine + 90 + car[playerCar][i].size.y / 2), limitReachedY = 1;
                    poz = car[playerCar][i].spr.getPosition();
                    if (carVelocityY > 0 and poz.y > 810)
                        car[playerCar][i].spr.setPosition(poz.x, 810), limitReachedY = 1;

                    if (drink == 3)
                    {
                        poz = car[playerCar][i].spr.getPosition();
                        if (poz.y + car[playerCar][i].size.y / 2 > seaLevel and activeEvent != 6)
                            car[playerCar][i].spr.setPosition(poz.x, seaLevel - car[playerCar][i].size.y / 2), limitReachedY = 1;
                    }
                }

                if (limitReachedX)
                    carVelocityX = 0;
                if (limitReachedY)
                    carVelocityY = 0;

                bananaTime++;
                if (bananaTime == 18)
                    bananaed = 0;

            }
            else
            {
                if (drink == 0)
                {
                    if (leftPressed)
                        carVelocityX -= 2;
                    if (rightPressed)
                        carVelocityX += 2;
                    if (upPressed)
                        carVelocityY -= 4;
                    if (downPressed)
                        carVelocityY += 4;

                    if (!leftPressed and !rightPressed)
                        carVelocityX = 0;

                    if (!upPressed and !downPressed)
                        carVelocityY = 0;

                    if (carVelocityX > car[playerCar][0].speedX)
                        carVelocityX = car[playerCar][0].speedX;
                    if (carVelocityX < -car[playerCar][0].speedX)
                        carVelocityX = -car[playerCar][0].speedX;
                    if (carVelocityY > car[playerCar][0].speedY)
                        carVelocityY = car[playerCar][0].speedY;
                    if (carVelocityY < -car[playerCar][0].speedY)
                        carVelocityY = -car[playerCar][0].speedY;
                }
                else
                {
                    if (shotTime == 0 and balledTime == 0)
                    {
                        if (leftPressed)
                            carVelocityX -= 1;
                        if (rightPressed)
                            carVelocityX += 1;
                        if (upPressed)
                            carVelocityY -= 2;
                        if (downPressed)
                            carVelocityY += 2;

                        if (!leftPressed and !rightPressed)
                            if (carVelocityX > 0)
                                carVelocityX -= 0.5f;
                            else if (carVelocityX < 0)
                                carVelocityX += 0.5f;

                        if (!upPressed and !downPressed)
                            if (carVelocityY > 0)
                                carVelocityY -= 1;
                            else if (carVelocityY < 0)
                                carVelocityY += 1;

                        if (whale.stop and carVelocityX < int(3 * roadSpeed / 4))
                            carVelocityX = int(3 * roadSpeed / 4);

                        if (whale.active and whale.onShore)
                            carVelocityX = carVelocityY = 0;
                    }
                    else if (balledTime < 0)
                    {
                        if (upPressed)
                            carVelocityY -= 2;
                        if (downPressed)
                            carVelocityY += 2;

                        if (!upPressed and !downPressed)
                            if (carVelocityY > 0)
                                carVelocityY -= 1;
                            else if (carVelocityY < 0)
                                carVelocityY += 1;

                        if (carVelocityX < 0)
                            carVelocityX += 2;
                    }
                    else if (shotTime < 0)
                    {
                        if (leftPressed)
                            carVelocityX -= 0.5f;
                        if (rightPressed)
                            carVelocityX += 0.5f;
                        if (upPressed)
                            carVelocityY -= 1;
                        if (downPressed)
                            carVelocityY += 1;

                        if (!leftPressed and !rightPressed)
                            if (carVelocityX > 0)
                                carVelocityX -= 0.25f;
                            else if (carVelocityX < 0)
                                carVelocityX += 0.25f;

                        if (!upPressed and !downPressed)
                            if (carVelocityY > 0)
                                carVelocityY -= 0.5f;
                            else if (carVelocityY < 0)
                                carVelocityY += 0.5f;

                    }

                    if (balledTime == 0)
                    {
                        if (carVelocityX > car[playerCar][0].speedX)
                            carVelocityX = car[playerCar][0].speedX;
                        if (carVelocityX < -car[playerCar][0].speedX)
                            carVelocityX = -car[playerCar][0].speedX;
                    }

                    if (carVelocityY > car[playerCar][0].speedY)
                        carVelocityY = car[playerCar][0].speedY;
                    if (carVelocityY < -car[playerCar][0].speedY)
                        carVelocityY = -car[playerCar][0].speedY;

                }

                limitReachedX = limitReachedY = 0;

                for (int i = 0; i < 3; i++)
                {
                    car[playerCar][i].spr.move(carVelocityX, carVelocityY);

                    poz = car[playerCar][i].spr.getPosition();
                    if (carVelocityX < 0 and poz.x < car[playerCar][i].size.x / 2)
                        car[playerCar][i].spr.setPosition(car[playerCar][i].size.x / 2, poz.y), limitReachedX = 1;
                    poz = car[playerCar][i].spr.getPosition();
                    if (carVelocityX > 0 and poz.x > Wx - car[playerCar][i].size.x / 2)
                        car[playerCar][i].spr.setPosition(Wx - car[playerCar][i].size.x / 2, poz.y), limitReachedX = 1;
                    poz = car[playerCar][i].spr.getPosition();
                    if (carVelocityY < 0 and poz.y < treeLine + 90 + car[playerCar][i].size.y / 2)
                        car[playerCar][i].spr.setPosition(poz.x, treeLine + 90 + car[playerCar][i].size.y / 2), limitReachedY = 1;
                    poz = car[playerCar][i].spr.getPosition();
                    if (carVelocityY > 0 and poz.y > 810)
                        car[playerCar][i].spr.setPosition(poz.x, 810), limitReachedY = 1;

                    if (drink == 3)
                    {
                        poz = car[playerCar][i].spr.getPosition();
                        if (poz.y + car[playerCar][i].size.y / 2 > seaLevel and activeEvent != 6)
                            car[playerCar][i].spr.setPosition(poz.x, seaLevel - car[playerCar][i].size.y / 2), limitReachedY = 1;
                    }
                }

                if (limitReachedX)
                    carVelocityX = 0;
                if (limitReachedY)
                    carVelocityY = 0;
            }

            if (slowdownTime < 0)
            {
                slowdownTime++;
                if (slowdownTime == 0)
                {
                    CurrentSong.setPitch(1.0f);
                    ImagineSound.setPitch(1.0f);
                    BombAlarm.setPitch(1.0f);
                    BlizzardSound.setPitch(1.0f);
                    PlaneSound.setPitch(1.0f);
                }
            }

            if (!whale.active or !whale.onShore)
            {
                shakeTimer++;                                            // car shake
                shakeTimer %= 12;
                shr = rand() % 9;
                for (int i = 0; i < 3; i++)
                    car[playerCar][i].spr.move(-shx, -shy);
                if (shotTime < 0 or shakeTimer == 0)
                {
                    shx = dx[shr];
                    shy = dy[shr];
                }
                for (int i = 0; i < 3; i++)
                    car[playerCar][i].spr.move(shx, shy);
            }


            p1 = car[playerCar][0].spr.getPosition();

            timeCounterSec = timeCounter / 10;                           // event and road processing
            if (timeCounterSec == 15 and !speedMilestone[0])
                speedMilestone[0] = 1, roadSpeed += 1;
            else if (timeCounterSec == 30 and !speedMilestone[1])
                speedMilestone[1] = 1, roadSpeed += 1;
            else if (timeCounterSec == 45 and !speedMilestone[2])
                speedMilestone[2] = 1, roadSpeed += 1;
            else if (timeCounterSec == 60 and !speedMilestone[3])
                speedMilestone[3] = 1, roadSpeed += 1;
            else if (timeCounterSec == 75 and !speedMilestone[4])
                speedMilestone[4] = 1, roadSpeed += 1;
            else if (timeCounterSec == 90 and !speedMilestone[5])
                speedMilestone[5] = 1, roadSpeed += 1;

            if (activeEvent)                                   
            {
                eventTime += roadSpeed;
                if (activeEvent != 4 or slowdownTime % 2 == 0)
                    eventTimeFrame++;
            }

            if (!underwaterActive)
            {
                if (slowdownTime < 0)
                {
                    RoadSpr.move(int(-roadSpeed / 2), 0);
                    RoadSpr1.move(int(-roadSpeed / 2), 0);
                }
                else
                {
                    RoadSpr.move(-roadSpeed, 0);
                    RoadSpr1.move(-roadSpeed, 0);
                }
            }
            else if (!whale.stop)
            {
                RoadSpr.move(-int(3 * roadSpeed / 4), 0);
                RoadSpr1.move(-int(3 * roadSpeed / 4), 0);
            }

            if (slowdownTime < 0)
            {
                FogSpr.move(-40, 0);
                FogSpr1.move(-40, 0);
            }
            else
            {
                FogSpr.move(-80, 0);
                FogSpr1.move(-80, 0);
            }

            if (slowdownTime < 0)
            {
                SnowstormSpr.move(-48, 27);
                SnowstormSpr1.move(-48, 27);
            }
            else
            {
                SnowstormSpr.move(-96, 54);
                SnowstormSpr1.move(-96, 54);
            }

            poz = RoadSpr.getPosition();
            if (poz.x <= -Wx)
            {
                if (drink == 1)
                {
                    if (lastEvent == 0 and !carChaseActive)
                    {
                        Rand = rand() % 3;
                        if (Rand == 0)
                        {
                            activeEvent = 1;
                            lastEvent = -1;
                            eventTime = 0;
                            eventTimeFrame = 0;
                            kidWave1 = kidWave2 = 0;
                            RoadSpr.setTexture(RoadSchoolTex);
                            for (int i = 0; i < CONST_kidCount; i++)
                                for (int j = 0; j < 3; j++)
                                    kid[i].active[j] = 0;
                            kidsRunOver = 0;
                        }
                        else if (Rand == 1)
                        {
                            activeEvent = 2;
                            lastEvent = -1;
                            eventTime = 0;
                            eventTimeFrame = 0;
                            policeLaser = 1;
                            carChaseActive = 0;
                            carChaseTime = 0;
                            RoadSpr.setTexture(RoadPoliceTex);
                        }
                    }
                    else
                    {
                        if (activeEvent == 2)
                        {
                            if (!carChaseActive)
                            {
                                bonusScoreValue += 6000;
                                endscreenPoliceCounter++;
                            }
                            activeEvent = 0;
                        }

                        if (activeEvent == 0 and !carChaseActive)
                            lastEvent++;
                        RoadSpr.setTexture(RoadTex);
                    }
                }
                else if (drink == 2)
                {
                    if (lastEvent == 0 and activeEvent == 0)
                    {
                        Rand = rand() % 3;
                        if (Rand == 0)
                        {
                            activeEvent = 3;
                            lastEvent = -1;
                            eventTime = 0;
                            eventTimeFrame = 0;
                            hudicon[5].time = 900;
                            roadSpeed -= 2;
                            for (int i = 0; i < 3; i++)
                            {
                                car[playerCar][i].speedX -= 4;
                                car[playerCar][i].speedY -= 6;
                            }

                            fogOpacity = 0;

                            BlizzardSound.play();
                        }
                        else if (Rand == 1)
                        {
                            activeEvent = 4;
                            lastEvent = -1;
                            eventTime = 0;
                            eventTimeFrame = 0;
                            bombCount = 0;
                            planeStart = 0;
                            bombAlarmDelay = bombAlarmLinger = 0;

                            PlaneSpr.setPosition(Wx, 0);
                            PlaneSound.play();
                        }
                    }
                    else if (activeEvent == 0)
                        lastEvent++;
                }
                else if (drink == 3)
                {
                    if (lastEvent == 0 and activeEvent == 0)
                    {
                        Rand = rand() % 5;

                        if (Rand == 0 and tsunamiCooldown == 0)
                        {
                            activeEvent = 7;
                            lastEvent = -1;
                            eventTime = 0;
                            eventTimeFrame = 0;
                        }
                        else if (Rand < 3)
                        {
                            activeEvent = 5;
                            lastEvent = -1;
                            eventTime = 0;
                            eventTimeFrame = 0;
                        }
                    }
                    else if (activeEvent == 0)
                        lastEvent++;
                }

                RoadSpr.move(2 * Wx, 0);
            }

            if (tsunamiCooldown < 0)
                tsunamiCooldown++;

            poz = RoadSpr1.getPosition();
            if (poz.x <= -Wx)
            {
                if (drink == 1)
                {
                    if (activeEvent == 1 and eventTime < 2 * Wx)
                    {
                        RoadSpr1.setTexture(RoadSchoolTex1);
                    }
                    else if (activeEvent == 2)
                        RoadSpr1.setTexture(RoadTex1);
                    else
                    {
                        activeEvent = 0;
                        RoadSpr1.setTexture(RoadTex1);
                    }
                }

                RoadSpr1.move(2 * Wx, 0);
            }

            poz = FogSpr.getPosition();
            if (poz.x <= -Wx)
                FogSpr.move(2 * Wx, 0);

            poz = FogSpr1.getPosition();
            if (poz.x <= -Wx)
                FogSpr1.move(2 * Wx, 0);

            poz = SnowstormSpr.getPosition();
            if (poz.y >= Wy)
                SnowstormSpr.move(2 * Wx, -2 * Wy);

            poz = SnowstormSpr1.getPosition();
            if (poz.y >= Wy)
                SnowstormSpr1.move(2 * Wx, -2 * Wy);

            if (activeEvent == 2)                                  // police event
            {
                if (eventTimeFrame % 46 == 0 and !carChaseActive)
                {
                    if (eventTime >= 600)
                        PoliceClickSound.play();
                    policeLaser = !policeLaser;
                    if (policeLaser)
                        RoadSpr.setTexture(RoadPoliceTex);
                    else
                        RoadSpr.setTexture(RoadPoliceTexOff);
                }

                if (policeLaser and !carChaseActive and Wx + (1465 - eventTime) <= p1.x + car[playerCar][0].size.x / 2 and Wx + (1475 - eventTime) >= p1.x - car[playerCar][0].size.x / 2)
                {
                    carChaseActive = 1;
                    carChaseTime = 0;
                    hudicon[1].time = 960;
                    RoadSpr.setTexture(RoadPoliceTexTrg);
                    roadSpeed += 2;

                    Rand = rand() % 4;
                    for (int i = 0; i < 3; i++)
                    {
                        int j = 0;
                        j -= policeCar[i].size.x;
                        policeCar[i].spr.setPosition(j, yLane[Rand]);
                        policeCar[i].Lane = Rand;
                    }

                    policeSirenFrame = 0;
                    PoliceSirenSpr.setTexture(PoliceSirenTex0);

                    policeMoveTime = 0;
                    policeMoveX = policeMoveY = 0;

                    PoliceSiren.play();
                    CurrentSong.setPitch(1.2f);
                }
            }

            if (activeEvent == 3)                        // blizzard event
            {
                if (eventTimeFrame < 100)
                {
                    if (eventTimeFrame % 2 == 0)
                    {
                        fogOpacity += 4; // era 5 !!!!!
                        FogSpr.setColor(sf::Color(255, 255, 255, fogOpacity));
                        FogSpr1.setColor(sf::Color(255, 255, 255, fogOpacity));
                        SnowstormSpr.setColor(sf::Color(255, 255, 255, fogOpacity));
                        SnowstormSpr1.setColor(sf::Color(255, 255, 255, fogOpacity));
                    }
                }
                else if (eventTimeFrame >= 800 and eventTimeFrame < 900)
                {
                    if (eventTimeFrame % 2 == 0)
                    {
                        fogOpacity -= 4;
                        FogSpr.setColor(sf::Color(255, 255, 255, fogOpacity));
                        FogSpr1.setColor(sf::Color(255, 255, 255, fogOpacity));
                        SnowstormSpr.setColor(sf::Color(255, 255, 255, fogOpacity));
                        SnowstormSpr1.setColor(sf::Color(255, 255, 255, fogOpacity));
                    }
                }
                else if (eventTimeFrame == 900)
                {
                    activeEvent = 0;
                    roadSpeed += 2;

                    for (int i = 0; i < 3; i++)
                    {
                        car[playerCar][i].speedX += 4;
                        car[playerCar][i].speedY += 6;
                    }

                    FogSpr.setColor(sf::Color(255, 255, 255, 0));
                    FogSpr1.setColor(sf::Color(255, 255, 255, 0));
                    SnowstormSpr.setColor(sf::Color(255, 255, 255, 0));
                    SnowstormSpr1.setColor(sf::Color(255, 255, 255, 0));

                    endscreenBlizzardCounter++;
                    bonusScoreValue += 8000;
                }
            }

            if (activeEvent == 4)                                // airstrike event
            {
                if (!planeStart and eventTimeFrame == 60)
                {
                    planeStart = 1;
                    eventTimeFrame = 0;
                }
                if (eventTimeFrame < 120 and planeStart)
                {
                    if (slowdownTime < 0)
                        PlaneSpr.move(-16, 0);
                    else
                        PlaneSpr.move(-32, 0);
                }

                if (slowdownTime % 2) {}
                else if (eventTimeFrame == 60)
                {
                    BombAlarm.play();
                    BombAlarmSpr.setTexture(BombAlarmTex0);
                    bombAlarmFrame = 0;
                    bombAlarmDelay = -1;
                }
                else if (eventTimeFrame == 90)
                {
                    bombCount = 4;
                    for (int i = 0; i < bombCount; i++)
                    {
                        bombX = 120 + rand() % (Wx - 240);
                        bombY = yLane[0] + rand() % (yLane[3] - yLane[0]);

                        target[i].x = bombX;
                        target[i].y = bombY;
                        target[i].spr.setPosition(bombX, bombY);
                        
                        bombShadowScale = 0.4f;
                        bombShadowOpacity = 10;
                        bombShadow[i].spr.setPosition(bombX, bombY);
                        bombShadow[i].spr.setScale(bombShadowScale, bombShadowScale);
                        bombShadow[i].spr.setColor(sf::Color(255, 255, 255, bombShadowOpacity));

                        bomb[i].spr.setPosition(bombX, bombY - 1500);
                    }
                }
                else if (eventTimeFrame > 90 and eventTimeFrame < 570)
                {
                    bombShadowScale += 0.005f;
                    if (eventTimeFrame % 8 < 3)
                        bombShadowOpacity++;

                    for (int i = 0; i < bombCount; i++)
                    {
                        bombShadow[i].spr.setScale(bombShadowScale, bombShadowScale);
                        bombShadow[i].spr.setColor(sf::Color(255, 255, 255, bombShadowOpacity));
                    }
                }
                else if (eventTimeFrame == 570)
                {
                    for (int i = 0; i < bombCount; i++)
                    {
                        int l = 0;
                        while (explosion[l].active)
                            l++;

                        if (!explosion[l].tex.loadFromFile("Assets/Textures/Effects/Explosion/frame25.png"));
                        explosion[l].sound.play();
                        explosion[l].spr.setTexture(explosion[l].tex);
                        explosion[l].scale = 3;
                        explosion[l].spr.setScale(explosion[l].scale, explosion[l].scale);
                        explosion[l].spr.setPosition(target[i].x - 150, target[i].y - 320);
                        explosion[l].frame = -1;
                        explosion[l].active = 1;

                        if (target[i].x - target[i].size.x / 4 <= p1.x + car[playerCar][0].size.x / 2 and target[i].x + target[i].size.x / 4 >= p1.x - car[playerCar][0].size.x / 2 and target[i].y - target[i].size.y / 6 <= p1.y + car[playerCar][0].size.y / 2 and target[i].y - target[i].size.y / 10 >= p1.y - car[playerCar][0].size.y / 2)
                            ExplodePlayerCar();

                        for (l = 0; l < 3; l++)
                        {
                            int i, j;
                            i = ieCar[l].x;
                            j = ieCar[l].y;
                            sf::Vector2f P1;
                            P1 = enemyCar[i][0].spr[j].getPosition();
                            if (i != -1 and j != -1 and enemyCar[i][0].active[j])
                                if (P1.x + enemyCar[i][0].size.x >= target[i].x - 2 * target[i].size.x and P1.x <= target[i].x + 2 * target[i].size.x and P1.y - enemyCar[i][0].size.y / 2 <= target[i].y + 2 * target[i].size.y and P1.y - enemyCar[i][0].size.y / 2 >= target[i].y - 2 * target[i].size.y)
                                {
                                    ExplodeEnemyCar(i, j);

                                    bonusScoreValue += 3000;
                                }
                        }
                    }

                    bombAlarmLinger = -60;
                    bombCount = 0;
                    activeEvent = 0;

                    if (status != 2)
                    {
                        endscreenAirstrikeCounter++;
                        bonusScoreValue += 8000;
                    }
                }

                if (eventTimeFrame >= 540 and eventTimeFrame < 570)
                    for (int i = 0; i < bombCount; i++)
                        if (slowdownTime < 0)
                            bomb[i].spr.move(0, 25);
                        else
                            bomb[i].spr.move(0, 50);


                if (bombAlarmActive)
                {
                    bombAlarmFrame++;
                    bombAlarmFrame %= 120;

                    if (bombAlarmFrame == 0)
                        BombAlarmSpr.setTexture(BombAlarmTex0);
                    else if (bombAlarmFrame == 60)
                        BombAlarmSpr.setTexture(BombAlarmTex1);
                }
            }

            if (bombAlarmDelay < 0)
            {
                bombAlarmDelay++;
                if (bombAlarmDelay == 0)
                    bombAlarmActive = 1;
            }

            if (bombAlarmLinger < 0)
            {
                bombAlarmLinger++;
                if (bombAlarmLinger == 0)
                    bombAlarmActive = 0;
            }

            if (activeEvent == 5)             // beach ball event
            {
                if (eventTimeFrame < 300)
                {
                    Rand = rand() % 10000;
                    if (Rand < 200)
                    {
                        int i, j, l, count = 0;

                        for (i = 0; i < CONST_maxBalls and ball[i].active; i++);
                        if (i != CONST_maxBalls)
                        {
                            for (j = 0; j < 4; j++)
                                if (lastBall[j] == 0)
                                    count++;

                            if (count)
                            {
                                Rand = rand() % count;
                                for (j = 0, l = 0; j < 4; j++)
                                    if (lastBall[j] == 0)
                                    {
                                        if (Rand == l)
                                        {
                                            ball[i].Lane = j;
                                            lastBall[j] = -20;
                                        }
                                        l++;
                                    }

                                ball[i].active = 1;
                                ball[i].time = -35;

                                Rand = rand() % 300;
                                ball[i].spr.setPosition(Wx + ball[i].size.x / 2 + Rand, yLane[ball[i].Lane] - ball[i].size.y / 2);
                                ball[i].shadowSpr.setPosition(Wx + ball[i].size.x / 2 + Rand, yLane[ball[i].Lane] - ball[i].size.y / 2 + 70);
                                ball[i].spr.setRotation(0);

                                ball[i].shadowOpacity = 180;
                                ball[i].shadowSize = 1.6f;
                                ball[i].shadowSpr.setScale(ball[i].shadowSize, ball[i].shadowSize);
                                ball[i].shadowSpr.setColor(sf::Color(255, 255, 255, ball[i].shadowOpacity));

                                int localSpeed = roadSpeed;
                                Rand = 3 * roadSpeed / 2 + rand() % localSpeed;
                                ball[i].speed = Rand;

                                BoingSound.play();
                            }

                        }
                    }
                }
                else
                    activeEvent = 0;
            }

            for (int i = 0; i < 4; i++)
                if (lastBall[i] < 0)
                    lastBall[i]++;

            for (int i = 0; i < CONST_maxBalls; i++)
                if (ball[i].active)
                {
                    ball[i].spr.move(-ball[i].speed, 0);
                    ball[i].shadowSpr.move(-ball[i].speed, 0);

                    ball[i].spr.rotate(4.0f);

                    if (ball[i].time < -30)
                        ball[i].spr.move(0, -14);
                    else if (ball[i].time < -25)
                        ball[i].spr.move(0, -10);
                    else if (ball[i].time < -20)
                        ball[i].spr.move(0, -9);
                    else if (ball[i].time < -15)
                        ball[i].spr.move(0, -7);
                    else if (ball[i].time < -10)
                        ball[i].spr.move(0, -5);
                    else if (ball[i].time < -5)
                        ball[i].spr.move(0, -3);
                    else if (ball[i].time < -2)
                        ball[i].spr.move(0, -1);
                    else if (ball[i].time < 2)
                        ball[i].spr.move(0, 0);
                    else if (ball[i].time < 5)
                        ball[i].spr.move(0, 1);
                    else if (ball[i].time < 10)
                        ball[i].spr.move(0, 3);
                    else if (ball[i].time < 15)
                        ball[i].spr.move(0, 5);
                    else if (ball[i].time < 20)
                        ball[i].spr.move(0, 7);
                    else if (ball[i].time < 25)
                        ball[i].spr.move(0, 9);
                    else if (ball[i].time < 30)
                        ball[i].spr.move(0, 10);
                    else
                        ball[i].spr.move(0, 14);

                    if (ball[i].time < 0)
                    {
                        ball[i].shadowSize -= 0.01f;
                        ball[i].shadowOpacity -= 2;
                    }
                    else
                    {
                        ball[i].shadowSize += 0.01f;
                        ball[i].shadowOpacity += 2;
                    }

                    ball[i].shadowSpr.setScale(ball[i].shadowSize, ball[i].shadowSize);
                    ball[i].shadowSpr.setColor(sf::Color(255, 255, 255, ball[i].shadowOpacity));

                    ball[i].time++;
                    if (ball[i].time > 35)
                    {
                        ball[i].time = -35;
                        BoingSound.play();
                    }

                    //ballHeight = ball[i].spr.getPosition().y - yLane[ball[i].Lane];
                    p2 = ball[i].spr.getPosition();
                    if (yLane[ball[i].Lane] <= p1.y + 5 * car[playerCar][0].size.y / 6 and yLane[ball[i].Lane] >= p1.y + car[playerCar][0].size.y / 6 and p1.x + car[playerCar][0].size.x / 2 >= p2.x and p1.x - car[playerCar][0].size.x / 2 <= p2.x)
                    {
                        if (!bananaed)
                        {
                            balledTime = -15;
                            carVelocityX = -30;
                            ball[i].active = 0;
                            bonusScoreValue += 2000;
                            endscreenBallCounter++;
                        }
                    }

                    if (ball[i].spr.getPosition().x + ball[i].size.x / 2 < 0)
                        ball[i].active = 0;
                }

            if (balledTime < 0)
                balledTime++;

            if (drink < 2)
            {
                lastTree++;                                        // tree generation


                if (activeEvent == 0 or (activeEvent == 1 and (eventTime <= 400 or eventTime >= Wx)) or (activeEvent == 2 and (eventTime <= 900 or eventTime >= Wx)))
                {
                    if (lastTree >= 15 and activeTrees < 9)
                    {
                        spawnTree = rand() % 1000;
                        if (spawnTree < 25)
                        {
                            int i;
                            for (i = 0; tree[i].active and i < 9; i++);
                            tree[i].active = 1;
                            tree[i].spr.setPosition(Wx, treeLine);
                            activeTrees++;
                            lastTree = 0;
                        }
                    }
                }

                for (int i = 0; i < 9; i++)                         // tree processing
                {
                    if (tree[i].active)
                    {
                        p2 = tree[i].spr.getPosition();
                        if (slowdownTime < 0)
                            tree[i].spr.move(int(-roadSpeed / 2), 0);
                        else
                            tree[i].spr.move(-roadSpeed, 0);
                        if (p2.x + tree[i].size.x <= 0)
                        {
                            tree[i].active = 0;
                            activeTrees--;
                        }
                    }
                }
            }

            for (int i = 0; i < 4; i++)
            {
                lastCar[i] += lastCarRate[i];
                if (lastCar[i] > 0)
                    lastCar[i] = 1;
            }

            if (CarSpawnCondition() and CarSpawnChance())  // enemy car spawn
                CarSpawn();

            if (activeCars > 0)                                             // enemy car processing
            {
                for (int l1 = 0; l1 < 2; l1++)
                    for (int l2 = l1 + 1; l2 < 3; l2++)
                    {
                        int i1, i2, j1, j2;
                        i1 = ieCar[l1].x;
                        j1 = ieCar[l1].y;
                        i2 = ieCar[l2].x;
                        j2 = ieCar[l2].y;
                        sf::Vector2f P1, P2;
                        P1 = enemyCar[i1][0].spr[j1].getPosition();
                        P2 = enemyCar[i2][0].spr[j2].getPosition();
                        if (i1 != -1 and i2 != -1 and enemyCar[i1][0].active[j1] and enemyCar[i2][0].active[j2])
                            if (enemyCar[i1][0].Lane[j1] == enemyCar[i2][0].Lane[j2] and std::max(P1.x, P2.x) <= std::min(P1.x + enemyCar[i1][0].size.x, P2.x + enemyCar[i2][0].size.x))
                            {
                                int l = 0;

                                if (i1 != 9)
                                {
                                    ExplodeEnemyCar(i1, j1);

                                    bonusScoreValue += 1500;
                                }

                                if (i2 != 9)
                                {
                                    ExplodeEnemyCar(i2, j2);

                                    bonusScoreValue += 1500;
                                }
                            }
                    }

                for (int i = 1; i <= CONST_enemyCarCount; i++)               // enemy car movement and collision
                    for (int j = 0; j < 3; j++)
                        if (enemyCar[i][0].active[j])
                        {
                            for (int l = 0; l < 3; l++)
                                if (slowdownTime < 0)
                                    enemyCar[i][l].spr[j].move(int((-roadSpeed - enemyCar[i][0].speed[j]) / 2), 0);
                                else
                                    enemyCar[i][l].spr[j].move(-roadSpeed - enemyCar[i][0].speed[j], 0);

                            p2 = enemyCar[i][0].spr[j].getPosition();

                            if (p2.x + enemyCar[i][0].size.x < 0)
                            {
                                activeCars--;
                                enemyCar[i][0].active[j] = 0;
                            }
                            else if (immunityTime == 0)
                            {
                                if (i == 9)
                                {
                                    if (p1.x + car[playerCar][0].size.x / 2 >= p2.x + enemyCar[i][0].size.x / 10 and p1.x - car[playerCar][0].size.x / 2 <= p2.x + 9 * enemyCar[i][0].size.x / 10 and p1.y <= p2.y - enemyCar[i][0].size.y / 8 and p1.y >= p2.y - 2 * enemyCar[i][0].size.y / 3)
                                        ExplodePlayerCar();
                                }
                                else if (i == 13)
                                {
                                    p2.x += 90;
                                    if (p1.x + car[playerCar][0].size.x / 2 >= p2.x + enemyCar[1][0].size.x / 10 and p1.x - car[playerCar][0].size.x / 2 <= p2.x + 9 * enemyCar[1][0].size.x / 10 and p1.y <= p2.y - enemyCar[1][0].size.y / 6 and p1.y >= p2.y - 5 * enemyCar[1][0].size.y / 6)
                                        ExplodePlayerCar();
                                }
                                else if (i == 14)
                                {
                                    if (p1.x + car[playerCar][0].size.x / 2 >= p2.x + enemyCar[1][0].size.x / 10 and p1.x - car[playerCar][0].size.x / 2 <= p2.x + 9 * enemyCar[1][0].size.x / 10 and p1.y <= p2.y - enemyCar[1][0].size.y / 6 and p1.y >= p2.y - 5 * enemyCar[1][0].size.y / 6)
                                        ExplodePlayerCar();
                                }
                                else if (p1.x + car[playerCar][0].size.x / 2 >= p2.x + enemyCar[i][0].size.x / 10 and p1.x - car[playerCar][0].size.x / 2 <= p2.x + 9 * enemyCar[i][0].size.x / 10 and p1.y <= p2.y - enemyCar[i][0].size.y / 6 and p1.y >= p2.y - 5 * enemyCar[i][0].size.y / 6)
                                {
                                    ExplodePlayerCar();
                                }
                            }

                            if (p2.y - 40 >= seaLevel)
                            {
                                ExplodeEnemyCar(i, j);

                                bonusScoreValue += 1000;
                            }
                        }
            }

            if (!enemyCar[6][0].active[0])
            {
                if (imagineTime == 0)
                    ImagineSound.stop();
                else
                {
                    imagineVolume *= 0.85f;
                    ImagineSound.setVolume(imagineVolume * sfxVolume);
                    imagineTime++;
                }
            }
            else
            {
                float i;
                p2 = enemyCar[6][0].spr[0].getPosition();
                i = 100 * abs(p1.x - p2.x);
                imagineVolume = 100 - i / Wx;
                ImagineSound.setVolume(imagineVolume * sfxVolume);
            }

            for (int i = 0; i < CONST_maxExplosions; i++)                                 // render explosions
                if (explosion[i].active)
                {
                    int j = explosion[i].frame / 4;
                    fileName = "Assets/Textures/Effects/Explosion/frame";
                    fileName += char('0' + j / 10);
                    fileName += char('0' + j % 10);
                    fileName += ".png";
                    if (!explosion[i].tex.loadFromFile(fileName));
                    explosion[i].spr.setTexture(explosion[i].tex);

                    explosion[i].frame++;
                    if (explosion[i].frame == 104)
                        explosion[i].active = 0;
                    if (slowdownTime < 0)
                        explosion[i].spr.move(int(-roadSpeed / 2), 0);
                    else
                        explosion[i].spr.move(-roadSpeed, 0);
                }

            for (int i = 0; i < CONST_maxExplosions; i++)                                 // render blood
                if (blood[i].active)
                {
                    int j = blood[i].frame / 5;
                    fileName = "Assets/Textures/Effects/Blood/frame";
                    fileName += char('0' + j);
                    fileName += ".png";
                    if (!blood[i].tex.loadFromFile(fileName));
                    blood[i].spr.setTexture(blood[i].tex);

                    blood[i].frame++;
                    if (blood[i].frame == 30)
                        blood[i].active = 0;
                    
                    if (!underwaterActive)
                        blood[i].spr.move(-roadSpeed, 0);
                    else
                        blood[i].spr.move(-int(3 * roadSpeed / 4), 0);
                }

            if (lastBanana <= 0)
                lastBanana++;

            if (activeBananas < CONST_maxBananaCount and lastBanana > 0 and drink != 3)          // banana spawn
            {
                Rand = rand() % 1000;
                if (Rand < 2)
                {
                    for (int i = 0; i < CONST_maxBananaCount; i++)
                        if (!banana.active[i])
                        {
                            Rand = yLane[0] + rand() % (yLane[3] - yLane[0]);

                            banana.active[i] = 1;
                            banana.spr[i].setPosition(Wx + banana.size.x / 2, Rand);
                            break;
                        }
                }
            }

            for (int i = 0; i < CONST_maxBananaCount; i++)                        // banana processing
                if (banana.active[i])
                {
                    if (!underwaterActive)
                    {
                        if (slowdownTime < 0)
                            banana.spr[i].move(int(-roadSpeed / 2), 0);
                        else
                            banana.spr[i].move(-roadSpeed, 0);
                    }
                    else if (!whale.stop)
                        banana.spr[i].move(int(-3 * roadSpeed / 4), 0);

                    p2 = banana.spr[i].getPosition();
                    if (p2.x + banana.size.x / 2 < 0)
                    {
                        activeBananas--;
                        banana.active[i] = 0;
                    }
                    else if (p1.x + car[playerCar][0].size.x / 2 >= p2.x and p1.x - car[playerCar][0].size.x / 2 <= p2.x + banana.size.x and p1.y + car[playerCar][0].size.y / 2 <= p2.y + banana.size.y and p1.y + car[playerCar][0].size.y / 2 >= p2.y)
                    {
                        bonusScoreValue += 1000;
                        banana.active[i] = 0;
                        bananaTime = 0;
                        bananaed = 1;
                        hudicon[3].time = 18;
                        endscreenBananaCounter++;

                        if (BananaSound.getStatus() == sf::SoundSource::Status::Playing)
                            BananaSound.stop();
                        BananaSound.play();
                    }
                }

            if (quoteTime < 0)
            {
                quoteTime++;
                if (quoteTime == 0)
                {
                    activeQuote = 0;
                    lastQuote = -240;
                }
            }

            if (lastQuote < 0)
                lastQuote++;

            if (!activeQuote and lastQuote >= 0)                           // quote spawn
            {
                Rand = rand() % 1000;
                if (Rand < 7)
                {
                    GenQuote();
                    activeQuote = 1;
                    quoteTime = -300;
                }
            }

            if (activeEvent == 1)                                           // kid spawn
            {
                int l = 0;
                if (!kidWave1 and eventTime >= 1370)
                {
                    kidWave1 = 1;
                    l = 1;
                }
                if (!kidWave2 and eventTime >= 1520)
                {
                    kidWave2 = 1;
                    l = 1;
                }
                if (l)
                {
                    for (l = 0; l < 6; l++)
                        usedLane[l] = 0;

                    for (int ii = 0; ii < 3; ii++)
                    {
                        Rand = rand() % (3 * CONST_kidCount);

                        int i, j;
                        i = Rand / 3;
                        j = Rand % 3;
                        if (!kid[i].active[j])
                        {
                            kid[i].active[j] = 1;
                            Rand = rand() % 6;
                            while (usedLane[Rand])
                                Rand = rand() % 6;
                            usedLane[Rand] = 1;

                            kid[i].spr[j].setPosition(Wx, 300 + 100 * Rand);
                        }
                    }
                }
            }

            for (int i = 0; i < CONST_kidCount; i++)                         // kid processing
                for (int j = 0; j < 3; j++)
                    if (kid[i].active[j])
                    {
                        if (slowdownTime < 0)
                            kid[i].spr[j].move(int(-roadSpeed / 2), 0);
                        else
                            kid[i].spr[j].move(-roadSpeed, 0);

                        p2 = kid[i].spr[j].getPosition();
                        if (p2.x <= p1.x + car[playerCar][0].size.x / 2 and p1.x - car[playerCar][0].size.x / 2 <= p2.x + kid[i].size.x and p2.y + kid[i].size.y / 2 <= p1.y + car[playerCar][0].size.y / 2 and p2.y + 3 * kid[i].size.y / 2 >= p1.y + car[playerCar][0].size.y / 2)
                        {
                            kid[i].active[j] = 0;
                            bonusScoreValue += 1500;

                            int l = 0;
                            while (explosion[l].active)
                                l++;

                            p2 = kid[i].spr[j].getPosition();
                            if (!explosion[l].tex.loadFromFile("Assets/Textures/Effects/Explosion/frame25.png"));
                            //explosion[l].sound.play();
                            explosion[l].spr.setTexture(explosion[l].tex);
                            explosion[l].scale = 3;
                            explosion[l].spr.setScale(explosion[l].scale, explosion[l].scale);
                            explosion[l].spr.setPosition(p2.x - 2 * kid[i].size.x, p2.y - 3 * kid[i].size.y / 2);
                            explosion[l].frame = -1;
                            explosion[l].active = 1;

                            endscreenKidCounter++;
                            BonusSound[kidsRunOver].play();
                            kidsRunOver++;
                            if (kidsRunOver == 5)
                            {
                                JackpotSound.play();
                                jackpotTime = -120;
                                bonusScoreValue += 2500;
                            }
                            else if (kidsRunOver == 6)
                                MaximumComboSound.play();
                        }
                    }

            if (carChaseActive)                                            // car chase processing
            {
                carChaseTime++;
                if (carChaseTime == 60)
                {
                    policeMoveTime = 115;
                    policeMoveX = 3;
                    policeMoveY = 0;
                }

                if (carChaseTime % 240 == 0 and carChaseTime < 960)
                {
                    Rand = rand() % 4;
                    for (int i = 0; Rand == policeCar[0].Lane and i < 10; i++)
                        Rand = rand() % 4;
                    

                    int i = yLane[Rand] - yLane[policeCar[0].Lane];

                    policeMoveX = 0;
                    policeMoveY = 5;
                    if (i < 0)
                    {
                        policeMoveY = -5;
                        i = -i;
                    }

                    policeMoveTime = i / 5;

                    for (int j = 0; j < 3; j++)
                        policeCar[j].Lane = Rand;
                }

                if (carChaseTime == 960)
                {
                    policeMoveTime = 115;
                    policeMoveX = -3;
                    policeMoveY = 0;
                    roadSpeed -= 2;
                }

                if (carChaseTime == 1075)
                {
                    carChaseActive = 0;
                    CurrentSong.setPitch(1.0f);
                    endscreenPoliceCounter++;
                }

                if (policeMoveTime > 0)
                {
                    policeMoveTime--;
                    for (int i = 0; i < 3; i++)
                        policeCar[i].spr.move(policeMoveX, policeMoveY);
                }

                p2 = policeCar[0].spr.getPosition();
                if (p1.x + car[playerCar][0].size.x / 2 >= p2.x + policeCar[0].size.x / 10 and p1.x - car[playerCar][0].size.x / 2 <= p2.x + 9 * policeCar[0].size.x / 10 and p1.y <= p2.y - policeCar[0].size.y / 6 and p1.y >= p2.y - 5 * policeCar[0].size.y / 6)
                    ExplodePlayerCar();

                for (int l = 0; l < 3; l++)
                {
                    int i, j;
                    i = ieCar[l].x;
                    j = ieCar[l].y;
                    sf::Vector2f P1;
                    P1 = enemyCar[i][0].spr[j].getPosition();
                    if (i != -1 and j != -1 and enemyCar[i][0].active[j])
                        if (P1.x + enemyCar[i][0].size.x >= p2.x + policeCar[0].size.x / 10 and P1.x <= p2.x + 9 * policeCar[0].size.x / 10 and P1.y - enemyCar[i][0].size.y / 2 <= p2.y - policeCar[0].size.y / 6 and P1.y - enemyCar[i][0].size.y / 2>= p2.y - 5 * policeCar[0].size.y / 6)
                        {
                            ExplodeEnemyCar(i, j);

                            bonusScoreValue += 300;
                        }
                }

                policeSirenFrame++;
                policeSirenFrame %= 60;
                if (policeSirenFrame == 30)
                    PoliceSirenSpr.setTexture(PoliceSirenTex1);
                else if (policeSirenFrame == 0)
                    PoliceSirenSpr.setTexture(PoliceSirenTex0);
            }

            if (drink == 1 and !shot[0].active and shotTime == 0)                                   // shot spawn
            {
                Rand = rand() % 1000;
                if (Rand < 2)
                {
                    Rand = yLane[0] + rand() % (yLane[3] - yLane[0]);
                    shot[0].active = 1;
                    for (int i = 0; i < 3; i++)
                        shot[i].spr.setPosition(Wx + shot[i].size.x / 2, Rand);
                }
            }

            if (shot[0].active)                                                                // shot processing
            {
                for (int i = 0; i < 3; i++)
                    shot[i].spr.move(-roadSpeed, 0);

                p2 = shot[0].spr.getPosition();
                if (p2.x + shot[0].size.x / 2 < 0)
                    shot[0].active = 0;
                else if (p1.x + car[playerCar][0].size.x / 2 >= p2.x and p1.x - car[playerCar][0].size.x / 2 <= p2.x + shot[0].size.x and p1.y + car[playerCar][0].size.y / 2 <= p2.y + shot[0].size.y and p1.y + car[playerCar][0].size.y / 2 >= p2.y)
                {
                    bonusScoreValue += 3000;
                    shot[0].active = 0;
                    shotTime = -300;
                    hudicon[2].time = -shotTime;
                    endscreenShotCounter++;

                    if (SlurpSound.getStatus() == sf::SoundSource::Status::Playing)
                        SlurpSound.stop();
                    SlurpSound.play();
                }
            }

            if (shotTime < 0)
                shotTime++;

            if (drink == 1 and !rpg[0].active and !rpgEquipped)                                                   // rpg spawn
            {
                Rand = rand() % 10000;
                if (Rand < 4)
                {
                    Rand = yLane[0] + rand() % (yLane[3] - yLane[0]);
                    rpg[0].active = 1;
                    for (int i = 0; i < 3; i++)
                        rpg[i].spr.setPosition(Wx + rpg[i].size.x / 2, Rand);
                }
            }

            if (rpg[0].active)                                                                // rpg pickup
            {
                for (int i = 0; i < 3; i++)
                    rpg[i].spr.move(-roadSpeed, 0);

                p2 = rpg[0].spr.getPosition();
                if (p2.x + rpg[0].size.x / 2 < 0)
                    rpg[0].active = 0;
                else if (p1.x + car[playerCar][0].size.x / 2 >= p2.x and p1.x - car[playerCar][0].size.x / 2 <= p2.x + rpg[0].size.x and p1.y + car[playerCar][0].size.y / 2 <= p2.y + 2 * rpg[0].size.y and p1.y + car[playerCar][0].size.y / 2 >= p2.y - rpg[0].size.y)
                {
                    rpg[0].active = 0;
                    rpgEquipped = 1;
                    rpgSpriteOn = 1;
                    RpgPickupSound.play();
                    endscreenRpgCounter++;
                }
            }

            if (rpgEquipped and xPressed)                                  // rpg shoot
            {
                rpgSpriteOff = 1;
                rpgEquipped = 0;

                rocket.active = 1;
                rocket.spr.setPosition(p1.x + 220, p1.y - 45);
            }

            if (rocket.active)                                                  // rocket processing
            {
                p2 = rocket.spr.getPosition();
                for (int l = 0; l < 3; l++)
                {
                    int i, j;
                    i = ieCar[l].x;
                    j = ieCar[l].y;
                    sf::Vector2f P1;
                    P1 = enemyCar[i][0].spr[j].getPosition();
                    if (i != -1 and j != -1 and enemyCar[i][0].active[j])
                        if (p2.x >= P1.x and p2.x <= P1.x + enemyCar[i][0].size.x and p2.y <= P1.y - enemyCar[i][0].size.y / 6 and p2.y >= P1.y - 5 * enemyCar[i][0].size.y / 6)
                        {
                            ExplodeEnemyCar(i, j);

                            rocket.active = 0;

                            RpgPickupSound.play();

                            bonusScoreValue += 5000;
                        }
                }

                if (p2.x - rocket.size.x / 2 >= Wx)
                    rocket.active = 0;

                rocket.spr.move(50, 0);
            }

            if (akCooldown < 0)
                akCooldown++;

            if (drink == 2 and xPressed and bulletCount > 0 and akCooldown == 0)                                  // AK-47 shoot
            {
                bulletCount--;

                int i;
                for (i = 0; bullet[i].active and i < CONST_maxBullets; i++);
                bullet[i].active = 1;
                bullet[i].spr.setPosition(p1.x + 251, p1.y - 57);
                akCooldown = -7;
                gunflashTime = -5;
                endscreenShotsFired++;
                BulletSound.play();
            }

            for (int i1 = 0; i1 < CONST_maxBullets; i1++)                                      // bullet processing
                if (bullet[i1].active)                                                  
                {
                    p2 = bullet[i1].spr.getPosition();
                    for (int l = 0; l < 3; l++)
                    {
                        int i, j;
                        i = ieCar[l].x;
                        j = ieCar[l].y;
                        sf::Vector2f P1;
                        P1 = enemyCar[i][0].spr[j].getPosition();
                        if (i != -1 and j != -1 and enemyCar[i][0].active[j])
                            if (p2.x >= P1.x and p2.x <= P1.x + enemyCar[i][0].size.x and p2.y <= P1.y - enemyCar[i][0].size.y / 6 and p2.y >= P1.y - 5 * enemyCar[i][0].size.y / 6)
                            {
                                enemyCar[i][0].hp[j]--;
                                BulletHitSound.play();

                                if (enemyCar[i][0].hp[j] == 0)
                                {
                                    ExplodeEnemyCar(i, j);

                                    endscreenCarsDestroyed++;
                                    if (i == 6)
                                        bonusScoreValue += 8888;
                                    else
                                        bonusScoreValue += 4000;
                                }

                                bullet[i1].active = 0;
                            }
                    }

                    if (p2.x - bullet[i1].size.x / 2 >= Wx)
                        bullet[i1].active = 0;
                    if (slowdownTime < 0)
                        bullet[i1].spr.move(17, 0);
                    else
                        bullet[i1].spr.move(35, 0);
                }

            if (drink == 2 and !ammobox[0].active)                                   // ammobox spawn
            {
                Rand = rand() % 10000;
                if (Rand < 14)
                {
                    Rand = yLane[0] + rand() % (yLane[3] - yLane[0]);
                    ammobox[0].active = 1;
                    for (int i = 0; i < 3; i++)
                        ammobox[i].spr.setPosition(Wx + ammobox[i].size.x / 2, Rand);
                }
            }

            if (ammobox[0].active)                                                                // ammobox processing
            {
                for (int i = 0; i < 3; i++)
                    if (slowdownTime < 0)
                        ammobox[i].spr.move(int(-roadSpeed / 2), 0);
                    else
                    ammobox[i].spr.move(-roadSpeed, 0);

                p2 = ammobox[0].spr.getPosition();
                if (p2.x + ammobox[0].size.x / 2 < 0)
                    ammobox[0].active = 0;
                else if (p1.x + car[playerCar][0].size.x / 2 >= p2.x and p1.x - car[playerCar][0].size.x / 2 <= p2.x + ammobox[0].size.x and p1.y + car[playerCar][0].size.y / 2 <= p2.y + ammobox[0].size.y and p1.y + car[playerCar][0].size.y / 2 >= p2.y)
                {
                    bonusScoreValue += 1000;
                    ammobox[0].active = 0;
                    bulletCount += 3;

                    AmmoboxSound.play();
                }
            }

            if (drink == 2 and !slowdown[0].active and !slowdownEquipped and slowdownTime == 0)                                   // slowdown spawn
            {
                Rand = rand() % 10000;
                if (Rand < 10)
                {
                    Rand = yLane[0] + rand() % (yLane[3] - yLane[0]);
                    slowdown[0].active = 1;
                    for (int i = 0; i < 3; i++)
                        slowdown[i].spr.setPosition(Wx + slowdown[i].size.x / 2, Rand);
                }
            }

            if (slowdown[0].active)                                                                // slowdown processing
            {
                for (int i = 0; i < 3; i++)
                    if (slowdownTime < 0)
                        slowdown[i].spr.move(int(-roadSpeed / 2), 0);
                    else
                        slowdown[i].spr.move(-roadSpeed, 0);

                p2 = slowdown[0].spr.getPosition();
                if (p2.x + slowdown[0].size.x / 2 < 0)
                    slowdown[0].active = 0;
                else if (p1.x + car[playerCar][0].size.x / 2 >= p2.x and p1.x - car[playerCar][0].size.x / 2 <= p2.x + slowdown[0].size.x and p1.y + car[playerCar][0].size.y / 2 <= p2.y + slowdown[0].size.y and p1.y + car[playerCar][0].size.y / 2 >= p2.y)
                {
                    bonusScoreValue += 3000;
                    slowdown[0].active = 0;
                    slowdownEquipped = 1;

                    SlowdownEquipSound.play();
                }
            }

            if (slowdownEquipped and zPressed)
            {
                slowdownTime = -150;
                hudicon[4].time = -slowdownTime;
                slowdownEquipped = 0;
                endscreenSlowdownCounter++;
                SlowdownSound.play();
                SlowdownEndSound.play();
                CurrentSong.setPitch(0.8f); 
                ImagineSound.setPitch(0.8f);
                BombAlarm.setPitch(0.5f);
                PlaneSound.setPitch(0.5f);
            }

            if (vendor.active)                                              // vendor processing
            {
                if (!vendor.good and !vendor.bad)
                {
                    vendor.spr.move(-roadSpeed - 3, 0);
                    vendor.itemSpr.move(-roadSpeed - 3, 0);
                    vendor.auraSpr.move(-roadSpeed - 3, 0);

                    vendor.time++;
                    if (vendor.time % 2 == 0)
                    {
                        vendor.frame++;
                        vendor.frame %= 7;
                        vendor.spr.setTexture(VendorTex[vendor.frame]);
                    }
                }
                else if (vendor.bad)
                {
                    vendor.spr.move(-roadSpeed, 0);
                }
                else if (vendor.good)
                {
                    vendor.spr.move(-roadSpeed, 0);

                    vendor.frame++;
                    vendor.frame %= 20;
                    if (vendor.frame == 0)
                        vendor.spr.setTexture(VendorTex[8]);
                    else if (vendor.frame == 5)
                        vendor.spr.setTexture(VendorTex[9]);
                    else if (vendor.frame == 10)
                        vendor.spr.setTexture(VendorTex[10]);
                    else if (vendor.frame == 15)
                        vendor.spr.setTexture(VendorTex[9]);
                }

                if (vendor.spr.getPosition().x + vendor.size.x <= -Wx / 2)
                    vendor.active = 0;
                else
                {
                    int i, j, l;
                    for (l = 0; l < 3; l++)
                    {
                        i = ieCar[l].x;
                        j = ieCar[l].y;
                        if (i != -1 and enemyCar[i][0].active[j])
                        {
                            sf::Vector2f P1, P2;
                            P1 = enemyCar[i][0].spr[j].getPosition();
                            P2 = vendor.spr.getPosition();
                            if (enemyCar[i][0].Lane[j] == vendor.Lane and P1.x <= P2.x + 3 * vendor.size.x / 4 and P1.x + enemyCar[i][0].size.x >= P2.x + vendor.size.x / 4)
                            {
                                ExplodeEnemyCar(i, j);

                                bonusScoreValue += 2500;
                            }
                        }
                    }

                    if (!vendor.good and !vendor.bad)
                    {
                        p2 = vendor.spr.getPosition();
                        if (p1.x + car[playerCar][0].size.x / 2 >= p2.x and p1.x - car[playerCar][0].size.x / 2 <= p2.x + vendor.size.x and p1.y <= p2.y and p1.y + car[playerCar][0].size.y / 2 >= p2.y - vendor.size.y / 2)
                        {
                            if (playerMoney >= vendor.price)
                            {
                                vendor.good = 1;
                                vendor.spr.setTexture(VendorTex[8]);
                                playerMoney -= vendor.price;
                                endscreenMoneySpent += vendor.price;
                                VendorQuoteToText();
                                VendorWindowSpr.setTexture(VendorWindowTexGood);
                                VendorGoodSound.play();

                                if (vendor.item == 0)
                                    cornEquipped = 1;
                                else if (vendor.item == 1)
                                    seashellEquipped = 1;
                                else if (vendor.item == 2)
                                    donutEquipped = 1;
                            }
                            else
                            {
                                vendor.bad = 1;
                                vendor.spr.setTexture(VendorTex[7]);
                                VendorQuoteToText();
                                VendorWindowSpr.setTexture(VendorWindowTexBad);
                                VendorBadSound.play();
                            }
                        }
                    }
                }
            }

            if (drink == 3 and !vendor.active and !underwaterActive and activeEvent != 6)                                                    // vendor spawn
            {
                Rand = rand() % 10000;

                if (Rand < 20 and (!donutEquipped or !cornEquipped or !seashellEquipped))
                {
                    vendor.active = 1;
                    vendor.frame = 0;
                    vendor.iframe = 0;
                    vendor.idir = 1;
                    vendor.time = 0;
                    vendor.bad = vendor.good = vendor.sale = 0;
                    
                    int localVector[CONST_maxVendorItems] = {}, j = 0;

                    if (!cornEquipped)
                        localVector[j++] = 0;
                    if (!seashellEquipped)
                        localVector[j++] = 1;
                    if (!donutEquipped)
                        localVector[j++] = 2;

                    Rand = rand() % j;
                    vendor.item = localVector[Rand];

                    vendor.spr.setTexture(VendorTex[0]);
                    vendor.size = VendorTex[0].getSize();
                    vendor.spr.setOrigin(0, vendor.size.y);

                    vendor.itemSpr.setTexture(VendorItemTex[vendor.item]);
                    vendor.isize = VendorItemTex[vendor.item].getSize();
                    vendor.itemSpr.setOrigin(vendor.isize.x / 2, vendor.isize.y);
                    vendor.auraSpr.setRotation(0);
                    vendor.auraSpr.setOrigin(vendor.isize.x / 2, vendor.isize.y);

                    Rand = rand() % 4;
                    for (int i = 0; i < 10 and lastCar[Rand] < 0; i++)
                        Rand = rand() % 4;

                    for (; lastCar[Rand] < 0; Rand++, Rand %= 4);

                    vendor.Lane = Rand;
                    vendor.spr.setPosition(2 * Wx, yLane[vendor.Lane]);

                    p2 = vendor.spr.getPosition();
                    vendor.itemSpr.setPosition(p2.x + vendor.isize.x / 2 + 5, p2.y - 90);
                    vendor.auraSpr.setPosition(p2.x + vendor.isize.x / 2 + 5, p2.y - 90);

                    vendor.price = CONST_itemPrice[vendor.item];
                    lastCar[vendor.Lane] = -Wx - 120;

                    VendorMoneyToText();
                    VendorQuoteToText();
                    VendorWindowSpr.setTexture(VendorWindowTex);
                    VendorNewSound.play();
                }
            }

            if (drink == 3)                           // tide processing
            {
                int dist = seaLevel - tideLine, dist2 = CONST_defaultSeaLevel - seaLevel;

                if (!whale.stop)
                {
                    SeaSpr.move(-roadSpeed, 0);
                    SeaSpr1.move(-roadSpeed, 0);
                }

                if (activeEvent != 6)
                {
                    if (tideActive and tideCooldown < 0)
                    {
                        seaLevel++;
                    }

                    if (tideActive and tideCooldown == 0)
                    {
                        int i;

                        tideTime++;

                        if (dist >= 400)
                            i = 10;
                        else if (dist >= 300)
                            i = 7;
                        else if (dist >= 200)
                            i = 6;
                        else if (dist >= 150)
                            i = 5;
                        else if (dist >= 100)
                            i = 4;
                        else if (dist >= 60)
                            i = 3;
                        else if (dist >= 30)
                            i = 2;
                        else if (dist > 0)
                            i = 1;
                        else
                        {
                            i = 0;
                            tideActive = 0;
                            tideRest = -300;
                            tideCooldown = -3 * tideTime + tideRest;
                            TideRestSound.play();
                        }

                        seaLevel -= i;
                    }

                    if (tideRest == 0 and !tideActive and dist2 > 0)
                    {
                        int i;

                        if (dist <= 30 or dist2 <= 30)
                            i = 1;
                        else if (dist <= 60 or dist2 <= 60)
                            i = 2;
                        else if (dist <= 100 or dist2 <= 100)
                            i = 3;
                        else if (dist <= 150 or dist2 <= 150)
                            i = 4;
                        else if (dist <= 200 or dist2 <= 200)
                            i = 5;
                        else if (dist <= 300 or dist2 <= 300)
                            i = 6;
                        else if (dist <= 400 or dist2 <= 400)
                            i = 7;
                        else
                            i = 10;

                        seaLevel += i;
                    }
                }
                else
                {
                    if (tideActive)
                    {
                        if (tideCooldown < 0)
                        {
                            tideCooldown++;
                            seaLevel++;
                        }
                        else if (dist > 0)
                        {
                            if (songVolume > 0)
                            {
                                songVolume--;
                                CurrentSong.setVolume(songVolume * musicVolume);
                            }
                            seaLevel -= 50;
                        }
                        else
                        {
                            tideActive = 0;
                            tideCooldown = -120;
                            transitionTime = -255;
                            underwaterActive = 1;
                            hasBeenUnderwater = 1;
                            underwaterTime = 0;
                            whale.stop = whale.active = whale.noReturn = whale.onShore = whale.carDespawn = 0;
                            RoadSpr.setTexture(SearoadTex);
                            RoadSpr1.setTexture(SearoadTex1);

                            int i = 3, j;
                            for (j = 0; j < 3; j++)
                                car[i][j].spr.setTexture(car[i][j].tex2);

                            vendor.active = 0;

                            alternateVolume = 0;
                            AlternateSong.setVolume(alternateVolume * musicVolume);
                            AlternateSong.play();
                            CurrentSong.pause();
                        }
                    }
                    else
                    {
                        if (tideCooldown < 0)
                        {
                            if (alternateVolume < 100)
                            {
                                alternateVolume++;
                                AlternateSong.setVolume(alternateVolume * musicVolume);
                            }
                            tideCooldown++;
                        }
                        else if (transitionTime < 0)
                        {
                            transitionTime++;
                            seaOpacity -= 1;
                            seashineOpacity += 1;
                            SeaSpr.setColor(sf::Color(255, 255, 255, seaOpacity));
                            SeaSpr1.setColor(sf::Color(255, 255, 255, seaOpacity));
                            SeashineSpr.setColor(sf::Color(255, 255, 255, seashineOpacity));
                        }
                    }
                }

                if (!tideActive and tideCooldown == 0 and !underwaterActive)
                {
                    Rand = rand() % 10000;
                    
                    if (activeEvent == 7)
                    {
                        tideActive = 1;
                        tideLine = -5800;
                        tideCooldown = -300;
                        activeEvent = 6;
                        BigTideSound.play();
                    }
                    else if (Rand < 50)
                    {
                        tideActive = 1;
                        tideTime = 0;

                        Rand = 1 + rand() % 2;

                        if (Rand == 0)
                        {
                            int count = 0;
                            for (int l = 0; l < 3; l++)
                            {
                                int i, j;
                                i = ieCar[l].x;
                                j = ieCar[l].y;

                                if (i != -1 and enemyCar[i][0].active[j])
                                {
                                    if (enemyCar[i][0].Lane[j] <= Rand)
                                        count++;

                                    if (count == 1)
                                    {
                                        Rand++;
                                        l = 3;
                                    }
                                }
                            }
                        }

                        if (Rand == 1)
                        {
                            int count = 0;
                            for (int l = 0; l < 3; l++)
                            {
                                int i, j;
                                i = ieCar[l].x;
                                j = ieCar[l].y;

                                if (i != -1 and enemyCar[i][0].active[j])
                                {
                                    if (enemyCar[i][0].Lane[j] <= Rand)
                                        count++;

                                    if (count == 2)
                                    {
                                        Rand++;
                                        l = 3;
                                    }
                                }
                            }
                        }

                        tideLane = Rand;
                        tideLine = yLane[Rand];
                        tideCooldown = -100;

                        TideSound1.play();
                    }
                }

                SeaSpr.setPosition(SeaSpr.getPosition().x, seaLevel);
                SeaSpr1.setPosition(SeaSpr1.getPosition().x, seaLevel);

                if (SeaSpr.getPosition().x <= -Wx)
                    SeaSpr.move(2 * Wx, 0);
                if (SeaSpr1.getPosition().x <= -Wx)
                    SeaSpr1.move(2 * Wx, 0);

                if (tideCooldown < 0)
                    tideCooldown++;

                if (tideRest < 0)
                {
                    tideRest++;

                    if (tideRest == 0)
                        TideSound2.play();
                }

            }

            if (underwaterActive and transitionTime == 0)                                                          
            {
                if (activeFishes < 4 and !whale.active)                                                               // fish spawn
                {
                    Rand = rand() % 10000;
                    if (Rand < 150)
                    {
                        FishSpawn();
                    }
                }

                for (int i = 0; i < CONST_maxFishes; i++)                                              // fish processing
                    for (int j = 0; j < 4; j++)
                        if (fish[i][j].active)
                        {
                            fish[i][j].frame++;
                            fish[i][j].time++;

                            if (i == 0)
                            {
                                fish[i][j].frame %= 50;
                                if (fish[i][j].frame == 0)
                                    fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][0]);
                                else if (fish[i][j].frame == 40)
                                    fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][1]), BiteSound.play();
                            }
                            else if (i == 1)
                            {
                                if (fish[i][j].time >= 90 and fish[i][j].rotates > 0)
                                {
                                    Rand = rand() % 10000;
                                    if (Rand < 70)
                                    {
                                        fish[i][j].dir = !fish[i][j].dir;
                                        fish[i][j].rotates--;
                                        fish[i][j].time = 0;
                                        fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][fish[i][j].nframe]);

                                        fish[i][j].speed0 += 1;
                                        fish[i][j].speed1 += 1;
                                        if (fish[i][j].dir)
                                            fish[i][j].speed = fish[i][j].speed1;
                                        else
                                            fish[i][j].speed = fish[i][j].speed0;

                                        ReturnfishSound.play();
                                    }
                                }

                                fish[i][j].frame %= 120;
                                if (fish[i][j].frame == 0)
                                    fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][0]), fish[i][j].nframe = 0;
                                else if (fish[i][j].frame == 30)
                                    fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][1]), fish[i][j].nframe = 1;
                                else if (fish[i][j].frame == 60)
                                    fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][2]), fish[i][j].nframe = 2;
                                else if (fish[i][j].frame == 90)
                                    fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][1]), fish[i][j].nframe = 1;
                            }
                            else if (i == 2 or i == 3 or i == 5)
                            {
                                fish[i][j].frame %= 120;
                                if (fish[i][j].frame == 0)
                                    fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][0]);
                                else if (fish[i][j].frame == 30)
                                    fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][1]);
                                else if (fish[i][j].frame == 60)
                                    fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][2]);
                                else if (fish[i][j].frame == 90)
                                    fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][1]);
                            }
                            else if (i == 4)
                            {
                                if (fish[i][j].state == 1)
                                {
                                    if (fish[i][j].frame == 5 or fish[i][j].frame == 10)   // shadow
                                    {
                                        fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][1]);
                                        if (fish[i][j].dir == 0)
                                            fish[i][j].shadowSpr.setTexture(FishShadowTex[i][1]);
                                        else
                                            fish[i][j].shadowSpr.setTexture(FishShadowTex[i][3]);
                                    }
                                    else if (fish[i][j].frame == 7 or fish[i][j].frame == 12)  // disappear
                                    {
                                        fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][0]);
                                        if (fish[i][j].dir == 0)
                                            fish[i][j].shadowSpr.setTexture(FishShadowTex[i][0]);
                                        else
                                            fish[i][j].shadowSpr.setTexture(FishShadowTex[i][2]);
                                    }
                                    else if (fish[i][j].frame == 30)    // fully appear
                                    {
                                        fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][2]);
                                        if (fish[i][j].dir == 0)
                                            fish[i][j].shadowSpr.setTexture(FishShadowTex[i][1]);
                                        else
                                            fish[i][j].shadowSpr.setTexture(FishShadowTex[i][3]);

                                        fish[i][j].state = 2;
                                    }
                                }
                            }
                            else if (i == 6)
                            {
                                fish[i][j].frame %= 50;
                                if (fish[i][j].frame == 0)
                                    fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][0]);
                                else if (fish[i][j].frame == 25)
                                    fish[i][j].spr.setTexture(FishTex[i][fish[i][j].dir][1]);
                            }
                            

                            p2 = fish[i][j].spr.getPosition();

                            if (fish[i][j].dir)
                            {
                                fish[i][j].spr.move(fish[i][j].speed, 0);
                                fish[i][j].shadowSpr.move(fish[i][j].speed, 0);
                                
                                if (p2.x >= Wx)
                                {
                                    fish[i][j].active = 0;
                                    activeFishes--;
                                    usedFlane[fish[i][j].Flane] = 0;
                                    continue;
                                }
                            }
                            else
                            {
                                fish[i][j].spr.move(-fish[i][j].speed, 0);
                                fish[i][j].shadowSpr.move(-fish[i][j].speed, 0);

                                if (p2.x + fish[i][j].size.x <= 0)
                                {
                                    fish[i][j].active = 0;
                                    activeFishes--;
                                    usedFlane[fish[i][j].Flane] = 0;
                                    continue;
                                }
                            }

                            sf::Vector2f P1;
                            P1.x = p1.x - car[playerCar][0].size.x / 2;
                            P1.y = p1.y - car[playerCar][0].size.y / 2;

                            if (i == 0 or i == 1)  // 135 mij, 0-120 hitbox pe OY, 402 162 -> 100, 340 pe OX
                            {
                                if (P1.x + 130 < p2.x + fish[i][j].size.x and P1.x + 330 > p2.x and P1.y + 15 < p2.y and P1.y + 110 > p2.y - fish[i][j].size.y and goldfishTime == 0 and immunityTime == 0)
                                {
                                    ExplodePlayerCar();
                                }
                            }
                            else if (i == 2)
                            {
                                if (P1.x + 130 < p2.x + fish[i][j].size.x and P1.x + 330 > p2.x and P1.y + 15 < p2.y and P1.y + 110 > p2.y - fish[i][j].size.y and goldfishTime == 0 and immunityTime == 0)
                                {
                                    fish[i][j].active = 0;
                                    activeFishes--;
                                    usedFlane[fish[i][j].Flane] = 0;

                                    jellyfishTime -= 180;
                                    hudicon[6].time += 180;
                                    bonusScoreValue += 6000;
                                    endscreenJellyfishCounter++;

                                    blood[20].sound.play();
                                }
                            }
                            else if (i == 3)
                            {
                                if (P1.x + 130 < p2.x + fish[i][j].size.x and P1.x + 330 > p2.x and P1.y + 15 < p2.y and P1.y + 110 > p2.y - fish[i][j].size.y)
                                {
                                    fish[i][j].active = 0;
                                    activeFishes--;
                                    usedFlane[fish[i][j].Flane] = 0;

                                    goldfishTime = -420;
                                    hudicon[7].time = -goldfishTime;
                                    bonusScoreValue += 8000;
                                    endscreenGoldfishCounter++;

                                    GoldfishSound.play();
                                }
                            }
                            else if (i == 4)
                            {
                                if (fish[i][j].state == 0)
                                {
                                    if (fish[i][j].dir == 0)
                                    {
                                        p2.y -= 56;

                                        if (P1.x + 130 < p2.x + fish[3][0].size.x and P1.x + 330 > p2.x and P1.y + 15 < p2.y and P1.y + 110 > p2.y - fish[3][0].size.y)
                                        {
                                            fish[i][j].state = 1;
                                            fish[i][j].frame = -1;
                                            AnglerSound.play();
                                        }
                                    }
                                    else
                                    {
                                        p2.x += 215;   
                                        p2.y -= 56;   

                                        if (P1.x + 130 < p2.x + fish[3][0].size.x and P1.x + 330 > p2.x and P1.y + 15 < p2.y and P1.y + 110 > p2.y - fish[3][0].size.y)
                                        {
                                            fish[i][j].state = 1;
                                            fish[i][j].frame = -1;
                                            AnglerSound.play();
                                        }
                                    }
                                }
                                else if (fish[i][j].state == 2 and goldfishTime == 0 and immunityTime == 0)
                                {
                                    if (fish[i][j].dir == 0)
                                    {
                                        if (P1.x + 130 < p2.x + 264 and P1.x + 330 > p2.x + 72 and P1.y + 15 < p2.y and P1.y + 110 > p2.y - fish[i][j].size.y + 38)
                                        {
                                            ExplodePlayerCar();
                                        }
                                    }
                                    else
                                    {
                                        if (P1.x + 130 < p2.x + 204 and P1.x + 330 > p2.x + 14 and P1.y + 15 < p2.y and P1.y + 110 > p2.y - fish[i][j].size.y + 38)
                                        {
                                            ExplodePlayerCar();
                                        }
                                    }
                                }
                            }
                            else if (i == 5)
                            {
                                if (P1.x + 130 < p2.x + fish[i][j].size.x and P1.x + 330 > p2.x and P1.y + 15 < p2.y and P1.y + 110 > p2.y - fish[i][j].size.y)
                                {
                                    fish[i][j].active = 0;
                                    activeFishes--;
                                    usedFlane[fish[i][j].Flane] = 0;

                                    bellfishEquipped = 1;
                                    BellSound.play();
                                    bonusScoreValue += 3000;
                                }
                            }
                            else if (i == 6)
                            {
                                if (P1.x + 130 < p2.x + fish[i][j].size.x and P1.x + 330 > p2.x and P1.y + 15 < p2.y and P1.y + 110 > p2.y - fish[i][j].size.y)
                                {
                                    fish[i][j].active = 0;
                                    activeFishes--;
                                    usedFlane[fish[i][j].Flane] = 0;

                                    Rand = 1 + rand() % 8;
                                    playerMoney += Rand * 10;

                                    MoneySound.play();
                                    bonusScoreValue += 2000;
                                }
                            }
                        }

                underwaterTime++;                             // whale spawn

                if ((underwaterTime >= 1800 or summonWhale) and !whale.active)                 // EDIT before release!!!!!!!
                {
                    Rand = rand() % 10000;
                    //Rand = 0;

                    if (summonWhale)
                        Rand = 0;

                    if (Rand < 4)
                    {
                        whale.active = 1;
                        whale.frame = 0;

                        whale.spr1.setPosition(2 * Wx, 0);
                        whale.spr2.setPosition(2 * Wx, 0);
                        whale.eyeSpr1.setPosition(2 * Wx, 0);
                        whale.eyeSpr2.setPosition(2 * Wx, 0);

                        GenWhaleQuote();
                        whale.quoteLength = whale.fullQuote.length();
                        whale.quoteIndex = 0;
                        whaleQuoteText.setPosition(930, 600);
                        whale.textOpacity = 255;
                        whaleQuoteText.setFillColor(sf::Color(63, 56, 156, whale.textOpacity));
                        whaleQuoteText.setOutlineColor(sf::Color(0, 0, 0, whale.textOpacity));
                    }
                }
                summonWhale = 0;

                if (whale.active)                        // whale processing
                {
                    whale.frame++;
                    
                    p2 = whale.spr1.getPosition();
                    
                    if (!whale.noReturn and p2.x <= 700)
                        whale.noReturn = 1;

                    if (!whale.stop)
                    {
                        if (p2.x <= 200)
                        {
                            whale.stop = 1;
                            whale.frame = 0;
                            hasSeenWhale = 1;
                            endscreenWhaleCounter++;
                        }
                        else
                        {
                            whale.spr1.move(int(-3 * roadSpeed / 4), 0);
                            whale.spr2.move(int(-3 * roadSpeed / 4), 0);
                            whale.eyeSpr1.move(int(-3 * roadSpeed / 4), 0);
                            whale.eyeSpr2.move(int(-3 * roadSpeed / 4), 0);
                        }
                    }
                    else
                    {
                        if (whale.frame >= 75 and whale.frame < 100)
                        {
                            whale.spr1.move(16, 0);
                        }
                        else if (whale.frame == 100)
                        {
                            AlternateSong.pause();
                            EngineSound.pause();
                        }
                        else if (whale.frame >= 160 and whale.frame < 210)
                        {
                            if (whale.frame == 160)
                                whale.carDespawn = 1;

                            whale.eyeSpr1.move(3, 0);
                            whale.eyeSpr2.move(3, 0);
                        }
                        else if (whale.frame >= 230 and whale.frame < 485)
                        {
                            darknessOpacity++;
                            DarknessSpr.setColor(sf::Color(255, 255, 255, darknessOpacity));

                            int i, j;

                            for (i = 0, j = 0; i <= whale.frame - 230; i += 6)
                                j++;

                            whale.spr1.move(-j, 0);
                            whale.spr2.move(-j, 0);
                            whale.eyeSpr1.move(-j, 0);
                            whale.eyeSpr2.move(-j, 0);

                            whale.eyeSpr1.move(1, 0);
                            whale.eyeSpr2.move(1, 0);
                        }
                        else if (whale.frame == 485)
                        {
                            RoadSpr.setTexture(Road2Tex);
                            RoadSpr1.setTexture(Road2Tex1);
                            seashineOpacity = 0;
                            SeashineSpr.setColor(sf::Color(255, 255, 255, seashineOpacity));

                            seaOpacity = 255;
                            SeaSpr.setColor(sf::Color(255, 255, 255, seaOpacity));
                            SeaSpr1.setColor(sf::Color(255, 255, 255, seaOpacity));
                            seaLevel = CONST_defaultSeaLevel;
                            SeaSpr.setPosition(SeaSpr.getPosition().x, seaLevel);

                            whale.onShore = 1;
                            whale.spr3.setPosition(400, 850);
                            whale.spr4.setPosition(400, 850);
                            CapsuleSpr.setPosition(800, 780);

                            for (int i = 0; i < 3; i++)
                            {
                                car[3][i].spr.setTexture(CarAbsentTex);
                                car[3][i].spr.setPosition(930, 500);
                            }
                        }
                        else if (whale.frame > 485 and whale.frame < 613)
                        {
                            darknessOpacity -= 2;
                            DarknessSpr.setColor(sf::Color(255, 255, 255, darknessOpacity));
                        }
                        else if (whale.frame >= 613 and whale.frame < 673)
                        {
                            if (whale.frame == 613)
                                WhalethrowSound.play();

                            CapsuleSpr.move(0, -60);

                            darknessOpacity = 0;
                            DarknessSpr.setColor(sf::Color(255, 255, 255, darknessOpacity));
                        }
                        else if (whale.frame >= 673 and whale.frame < 710)
                        {
                            CapsuleSpr.move(0, 80);
                        }
                        else if (whale.frame == 710)
                        {
                            underwaterActive = 0;
                            whale.active = whale.noReturn = whale.carDespawn = whale.stop = 0;
                            activeEvent = 0;

                            for (int i = 0; i < 3; i++)
                                car[3][i].spr.setTexture(car[3][i].tex);

                            CapsuleBrokenSpr.setPosition(900, 550);

                            songVolume = 100;
                            CurrentSong.setVolume(songVolume * musicVolume);
                            EngineSound.play();
                            GlassSound.play();
                            tsunamiCooldown = -1200;
                        }

                        if (whale.frame >= 145 and whale.quoteIndex < whale.quoteLength)
                        {
                            if (whale.frame % 3 == 0)
                            {
                                whale.quoteIndex++;
                                whale.quote = whale.fullQuote;
                                whale.quote.resize(whale.quoteIndex);
                                whaleQuoteText.setString(whale.quote);
                                WhaleVoiceSound.play();
                            }
                        }

                        if (whale.frame >= 300 and whale.frame < 351)
                        {
                            whale.textOpacity -= 5;
                            whaleQuoteText.setFillColor(sf::Color(63, 56, 156, whale.textOpacity));
                            whaleQuoteText.setOutlineColor(sf::Color(0, 0, 0, whale.textOpacity));
                        }
                    }
                }

            }

            if (goldfishTime < 0)
            {
                CarAuraSpr.setPosition(p1.x, p1.y);
                goldfishTime++;
            }

            if (immunityTime < 0)
            {
                ImmunityAuraSpr.setPosition(p1.x, p1.y);
                immunityTime++;
            }

            if (cornEquipped)
            {
                CornAuraSpr.setPosition(p1.x, p1.y);
            }

            if (whale.onShore and !whale.active)
            {
                whale.spr3.move(-roadSpeed, 0);
                whale.spr4.move(-roadSpeed, 0);
                CapsuleBrokenSpr.move(-roadSpeed, 0);

                p2 = whale.spr3.getPosition();

                if (p2.x + whale.size.x <= 0)
                    whale.onShore = 0;
            }

            for (int i = 0; i < CONST_maxMoneybags; i++)                         // moneybag spawn
            {
                if (drink == 3 and !moneybag[i].active and !underwaterActive)
                {
                    Rand = rand() % 10000;

                    if (Rand < 12)
                    {
                        Rand = rand() % 4;

                        if (tideActive and tideLane < Rand)
                            tideLane = Rand;
                        
                        moneybag[i].active = 1;
                        moneybag[i].frame = 0;
                        moneybag[i].scale = 1;
                        moneybag[i].shadowSpr.setScale(moneybag[i].scale, moneybag[i].scale);
                        moneybag[i].shadowOpacity = 230;
                        moneybag[i].shadowSpr.setColor(sf::Color(255, 255, 255, moneybag[i].shadowOpacity));

                        moneybag[i].spr.setPosition(Wx + moneybag[i].size.x, yLane[Rand]);
                        moneybag[i].shadowSpr.setPosition(Wx + 3 * moneybag[i].size.x / 2, yLane[Rand] - 15);
                    }
                }
            }

            for (int i = 0; i < CONST_maxMoneybags; i++)                         // moneybag processing
            {
                if (moneybag[i].active)
                {
                    if (!underwaterActive)
                    {
                        moneybag[i].spr.move(-roadSpeed, 0);
                        moneybag[i].shadowSpr.move(-roadSpeed, 0);
                    }
                    else
                    {
                        moneybag[i].spr.move(-int(3 * roadSpeed / 4), 0);
                        moneybag[i].shadowSpr.move(-int(3 * roadSpeed / 4), 0);
                    }

                    if (moneybag[i].frame < 35)
                    {
                        moneybag[i].spr.move(0, -1);
                        moneybag[i].shadowOpacity -= 1;
                        moneybag[i].shadowSpr.setColor(sf::Color(255, 255, 255, moneybag[i].shadowOpacity));
                        moneybag[i].scale -= 0.005f;
                        moneybag[i].shadowSpr.setScale(moneybag[i].scale, moneybag[i].scale);
                    }
                    else
                    {
                        moneybag[i].spr.move(0, 1);
                        moneybag[i].shadowOpacity += 1;
                        moneybag[i].shadowSpr.setColor(sf::Color(255, 255, 255, moneybag[i].shadowOpacity));
                        moneybag[i].scale += 0.005f;
                        moneybag[i].shadowSpr.setScale(moneybag[i].scale, moneybag[i].scale);
                    }

                    moneybag[i].frame++;
                    moneybag[i].frame %= 70;

                    p2 = moneybag[i].spr.getPosition();
                    if (p2.x + moneybag[i].size.x < 0)
                        moneybag[i].active = 0;
                    else if (p1.x + car[playerCar][0].size.x / 2 >= p2.x and p1.x - car[playerCar][0].size.x / 2 <= p2.x + moneybag[i].size.x and p1.y + car[playerCar][0].size.y / 2 <= p2.y + moneybag[i].size.y and p1.y + car[playerCar][0].size.y / 2 >= p2.y)
                    {
                        bonusScoreValue += 1000;
                        moneybag[i].active = 0;
                        
                        playerMoney += moneybag[i].value;
                        MoneySound.play();
                    }
                }
            }

            if (bellfishEquipped and cPressed and underwaterActive)         // bellfish processing
            {
                bellfishEquipped = 0;
                summonWhale = 1;
                SeacallSound.play();
                BellfishSound.play();
            }

            if (seashellEquipped and cPressed and !underwaterActive)         // seashell processing
            {
                seashellEquipped = 0;
                activeEvent = 7;
                SeacallSound.play();
            }

            if (donutEquipped and xPressed and !whale.stop)      // donut processing
            {
                donutTime = -20;
                donutScale = 1.0f;
                DonutSpr.setScale(donutScale, donutScale);
                donutOpacity = 255;
                DonutSpr.setColor(sf::Color(255, 255, 255, donutOpacity));
                DonutSpr.setOrigin(DonutTex.getSize().x / 2, DonutTex.getSize().y / 2);
                donutEquipped = 0;
                DonutSound.play();
            }

            if (donutTime < 0)
            {
                donutTime++;
                donutScale += 1.5f;
                DonutSpr.setScale(donutScale, donutScale);
                donutOpacity -= 12;
                DonutSpr.setColor(sf::Color(255, 255, 255, donutOpacity));
                DonutSpr.setPosition(p1);

                for (int l = 0; l < 3; l++)
                {
                    int i, j;
                    i = ieCar[l].x;
                    j = ieCar[l].y;
                    if (i != -1 and j != -1 and enemyCar[i][0].active[j])
                    {
                        sf::Vector2f P1, P2, P3, P4;
                        P1 = P2 = P3 = P4 = enemyCar[i][0].spr[j].getPosition();
                        P2.y -= enemyCar[i][0].size.y;
                        P3.x += enemyCar[i][0].size.x;
                        P3.y -= enemyCar[i][0].size.y;
                        P4.x += enemyCar[i][0].size.x;

                        if (DonutCollision(p1, P1) or DonutCollision(p1, P2) or DonutCollision(p1, P3) or DonutCollision(p1, P4))
                        {
                            ExplodeEnemyCar(i, j);
                            bonusScoreValue += 4000;
                        }
                    }
                }

                for (int i = 0; i < CONST_maxFishes; i++)
                    for (int j = 0; j < 4; j++)
                        if (fish[i][j].active)
                        {
                            sf::Vector2f P1, P2, P3, P4;
                            P1 = P2 = P3 = P4 = fish[i][j].spr.getPosition();
                            P2.y += fish[i][j].size.y;
                            P3.x += fish[i][j].size.x;
                            P3.y += fish[i][j].size.y;
                            P4.x += fish[i][j].size.x;

                            if (DonutCollision(p1, P1) or DonutCollision(p1, P2) or DonutCollision(p1, P3) or DonutCollision(p1, P4))
                            {
                                ExplodeFish(i, j);
                                bonusScoreValue += 3000;
                            }
                        }
            }


            if (CurrentSong.getStatus() == sf::SoundSource::Status::Stopped)             // song processing
            {
                if (!firstTimeSong)
                {
                    GenSong();
                    firstTimeSong = 1;
                    CurrentSong.play();
                }
                else
                {
                    newSongDelay++;
                    if (newSongDelay == 120)
                    {
                        GenSong();
                        newSongDelay = 0;
                        CurrentSong.play();
                    }
                }
            }
            else if (CurrentSong.getStatus() == sf::SoundSource::Status::Paused and !underwaterActive)
                CurrentSong.play();

            if (drink == 3)
            {
                if (AlternateSong.getStatus() == sf::SoundSource::Status::Stopped)
                {
                    GenASong();
                    AlternateSong.play();
                    AlternateSong.pause();
                }
                else if (AlternateSong.getStatus() == sf::SoundSource::Status::Paused and underwaterActive and !whale.stop)
                    AlternateSong.play();
            }

            prevHudicon = -1;
            for (int i = 0; i < CONST_hudiconCount; i++)                 // hudicon processing
            {
                if ((i == 0 and drink == 2) or hudicon[i].time > 1 or (i == 9 and drink == 3 and cornEquipped))
                {
                    hudicon[i].time--;
                    if (prevHudicon != -1)
                    {
                        p2 = hudicon[prevHudicon].spr.getPosition();
                        hudicon[i].spr.setPosition(p2.x - hudicon[i].size.x - 10, p2.y);
                    }
                    else
                        hudicon[i].spr.setPosition(1825, 10);

                    HudiconToText(i);
                    hudicon[i].txt.setOrigin(hudicon[i].txt.getGlobalBounds().getSize().x / 2, 0);
                    p2 = hudicon[i].spr.getPosition();
                    hudicon[i].txt.setPosition(p2.x + hudicon[i].size.x / 2, p2.y + hudicon[i].size.y - 7);
                    prevHudicon = i;
                }
                else
                    hudicon[i].time = 0;
            }

            huditem[0].active = rpgEquipped;
            huditem[1].active = (bulletCount > 0);
            huditem[2].active = slowdownEquipped;
            huditem[5].active = bellfishEquipped and underwaterActive;
            huditem[3].active = seashellEquipped and !underwaterActive;
            huditem[4].active = donutEquipped;

            prevHuditem = -1;
            for (int i = 0; i < CONST_huditemCount; i++)             // huditem processing
            {
                if (huditem[i].active)
                {
                    if (prevHuditem != -1)
                    {
                        p2 = huditem[prevHuditem].spr.getPosition();
                        huditem[i].spr.setPosition(p2.x - huditem[i].size.x - 10, p2.y);
                    }
                    else
                        huditem[i].spr.setPosition(1775, 900);
;
                    prevHuditem = i;
                }
            }

            ScoreToText();                                                 // score processing

            if (bonusScoreValue)
            {
                bonusScoreGain += bonusScoreValue;
                score += bonusScoreValue;
                bonusScoreTime = -120;
                BonusScoreToText();
            }

            PlayerMoneyToText();

            DrawStuff(window);                                                 // drawing stuff time!
            score += 5;

            if (escPressed)
            {
                prevStatus = status;
                status = 6;
                window.setKeyRepeatEnabled(false);
                PauseAllSounds();
            }
        }
        else if (status == 2)                                                          // death !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {
            if (cornEquipped)
            {
                status = 1;
                cornEquipped = 0;
                immunityTime = -150;
                hudicon[8].time = -immunityTime;
                continue;
            }
            
            p1Dead = 1;

            window.setKeyRepeatEnabled(false);

            highscore[drink] = std::max(highscore[drink], score - 5);

            CurrentSong.pause();
            AlternateSong.pause();
            EngineSound.stop();
            SlowdownEndSound.stop();
            TideSound1.stop();
            TideSound2.stop();
            TideRestSound.stop();
            BigTideSound.stop();

            if (deathKeyCooldown < 0)
                deathKeyCooldown++;
            else if (!endscreenOn)
            {
                if (keyPressed)
                {
                    EndscreenSpr.setPosition(Wx / 2, 0);
                    endscreenOn = 1;
                    endscreenMoveTime = 0;

                    for (int i = 0; i < CONST_maxEndscreenText; i++)
                        endscreenText[i].setString("");

                    EndscreenManager();

                    EndscreenSound.play();
                }
            }
            else if (keyPressed)
                status = 3;

            if (endscreenOn)
            {
                if (endscreenMoveTime < 1000)
                {
                    EndscreenSpr.move(0, 200);
                    for (int i = 0; i < CONST_maxEndscreenText; i++)
                        endscreenText[i].move(0, 200);
                    endscreenMoveTime += 200;
                }
                else
                {
                    if (currentEndscreenEvent < maxEndscreenEvents)
                    {
                        if (currentEndscreenEvent >= 0)
                            EndscreenToText(currentEndscreenEvent);
                        if (endscreenEventTimer < 0)
                        {
                            endscreenEventTimer++;
                            endscreenCurrentValue += endscreenIncrement;
                        }
                        else
                        {
                            currentEndscreenEvent++;
                            if (currentEndscreenEvent == 0)
                            {
                                endscreenEventTimer = -20;
                                endscreenIncrement = endscreenValue[currentEndscreenEvent] / 20.0f;
                                endscreenCurrentValue = 0;
                            }
                            else
                            {
                                endscreenEventTimer = -15;
                                endscreenIncrement = endscreenValue[currentEndscreenEvent] / 15.0f;
                                endscreenCurrentValue = 0;
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < CONST_maxExplosions; i++)
                if (explosion[i].active)
                {
                    int j = explosion[i].frame / 4;
                    fileName = "Assets/Textures/Effects/Explosion/frame";
                    fileName += char('0' + j / 10);
                    fileName += char('0' + j % 10);
                    fileName += ".png";
                    if (!explosion[i].tex.loadFromFile(fileName));
                    explosion[i].spr.setTexture(explosion[i].tex);

                    explosion[i].frame++;
                    if (explosion[i].frame == 104)
                        explosion[i].active = 0;
                }

            policeSirenFrame++;
            policeSirenFrame %= 60;
            if (policeSirenFrame == 30)
                PoliceSirenSpr.setTexture(PoliceSirenTex1);
            else if (policeSirenFrame == 0)
                PoliceSirenSpr.setTexture(PoliceSirenTex0);

            DrawStuff(window);

            if (endscreenOn)
            {
                window.draw(EndscreenSpr);
                for (int i = 0; i < CONST_maxEndscreenText; i++)
                    window.draw(endscreenText[i]);
                if (newHighscoreReachedEnd)
                    window.draw(newHighscoreEnd);

            }
        }
        else if (status == 3)                                                            // drinking prep !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {

            OverwriteSave();

            PoliceSiren.stop();
            EndscreenSound.stop();
            ImagineSound.stop();
            BombAlarm.stop();
            PlaneSound.stop();
            BlizzardSound.stop();
            MenuSong.play();

            DrinkSpr[drink].setPosition(950, 640);

            mouseHeld = 0;
            
            status = 5;

            window.draw(MenuSpr);
            window.draw(DrinkSpr[drink]);

            exitButton = new Button;
            exitButton->tex.loadFromFile("Assets/Textures/HUD/exit.png");
            exitButton->spr.setTexture(exitButton->tex);
            exitButton->spr.setPosition(1668, 13);

        }
        else if (status == 4)                                                           // driving prep !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {
            playerCar = drink;
            playerCarFrameCounter = -1;

            for (int i = 0; i < 3; i++)
                car[playerCar][i].spr.setPosition(Wx / 4, Wy / 2); // 1668  13

            if (drink == 2)
            {
                RoadSpr.setTexture(Road1Tex);
                RoadSpr1.setTexture(Road1Tex1);
            }
            else if (drink == 3)
            {
                RoadSpr.setTexture(Road2Tex);
                RoadSpr1.setTexture(Road2Tex1);
            }
            else
            {
                RoadSpr.setTexture(RoadTex);
                RoadSpr1.setTexture(RoadTex1);
            }
            RoadSpr.setPosition(0, 0);
            RoadSpr1.setPosition(Wx, 0);
            roadSpeed = 10;

            activeTrees = 0;

            for (int i = 0; i < 9; i++)
                tree[i].active = 0;

            for (int i = 10 + rand() % 600; i < Wx; i += (140 + rand() % 600))
            {
                activeTrees++;
                int j = activeTrees;
                tree[j].spr.setPosition(i, treeLine);
                tree[j].active = 1;
            }

            activeCars = 0;

            for (int i = 0; i < 4; i++)
            {
                lastCar[i] = 1;
                lastCarRate[i] = 0;
            }

            for (int i = 1; i <= CONST_enemyCarCount; i++)
                for (int j = 0; j < 3; j++)
                    for (int l = 0; l < 5; l++)
                        enemyCar[i][j].active[l] = 0;

            for (int l = 0; l < 3; l++)
                ieCar[l].x = ieCar[l].y = -1;

            activeBananas = 0;
            lastBanana = 1;
            bananaed = 0;

            for (int i = 0; i < CONST_maxBananaCount; i++)
                banana.active[i] = 0;

            for (int i = 0; i < CONST_maxExplosions; i++)
                explosion[i].active = blood[i].active = 0;

            score = 0;

            lastQuote = -120;
            quoteTime = 0;
            activeQuote = 0;

            if (highscore[drink])
                newHighscoreReached = 0;
            else
                newHighscoreReached = 1;
            newHighscoreTime = 0;
            newHighscoreReachedEnd = 0;

            for (int i = 0; i < CONST_kidCount; i++)
                for (int j = 0; j < CONST_maxKidCount; j++)
                    kid[i].active[j] = 0;

            bonusScoreGain = 0;
            bonusScoreTime = 0;

            carVelocityX = carVelocityY = 0;

            activeEvent = 0;
            lastEvent = 0;
            eventTime = 0;

            deathKeyCooldown = -10;
            carChaseActive = 0;

            shot[0].active = 0;
            shotTime = 0;
            ammobox[0].active = 0;

            rpg[0].active = 0;
            rpgEquipped = 0;
            rpgSpriteOn = 0;
            rpgSpriteOff = 1;
            rocket.active = 0;

            newSongDelay = 0;
            
            if (drink != lastDrink and (drink != 0 or lastDrink != 1))
            {
                CurrentSong.stop();
                firstTimeSong = 0;
            }

            CurrentSong.setPitch(1.0f);
            ImagineSound.setPitch(1.0f);
            BombAlarm.setPitch(1.0f);
            BlizzardSound.setPitch(1.0f);
            PlaneSound.setPitch(1.0f);
            EngineSound.play();
            MenuSong.pause();

            jackpotTime = 0;
            imagineTime = 0;

            for (int i = 0; i < CONST_hudiconCount; i++)
                hudicon[i].time = 0;

            endscreenOn = 0;
            timeCounter = endscreenBananaCounter = endscreenKidCounter = endscreenPoliceCounter = endscreenShotCounter = endscreenRpgCounter = endscreenShotsFired = endscreenSlowdownCounter = endscreenCarsDestroyed = 0;
            endscreenAirstrikeCounter = endscreenBlizzardCounter = endscreenMoneySpent = endscreenGoldfishCounter = endscreenWhaleCounter = endscreenBallCounter = endscreenJellyfishCounter = 0;
            hasBeenUnderwater = 0;
            hasSeenWhale = 0;

            for (int i = 0; i < 10; i++)
                speedMilestone[i] = 0;

            if (drink == 2)
                bulletCount = 3;
            else
                bulletCount = 0;

            for (int i = 0; i < CONST_maxBullets; i++)
                bullet[i].active = 0;

            akCooldown = gunflashTime = 0;

            slowdown[0].active = 0;
            slowdownEquipped = 0;
            slowdownTime = 0;

            for (int i = 0; i < CONST_maxDrinks; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    car[i][j].speedX = 10;
                    car[i][j].speedY = 14;
                }
            }

            FogSpr.setPosition(0, 0);
            FogSpr1.setPosition(Wx, 0);
            FogSpr.setColor(sf::Color(255, 255, 255, 0));
            FogSpr1.setColor(sf::Color(255, 255, 255, 0));
            SnowstormSpr.setPosition(-Wx, 0);
            SnowstormSpr1.setPosition(0, -Wy);
            SnowstormSpr.setColor(sf::Color(255, 255, 255, 0));
            SnowstormSpr1.setColor(sf::Color(255, 255, 255, 0));

            bombCount = 0;
            bombAlarmActive = 0;
            bombAlarmDelay = bombAlarmLinger = 0;

            lastDrink = drink;
            if (lastDrink == 0)
                lastDrink = 1;

            if (drink == 2)
                quoteText.setFillColor(sf::Color(205, 233, 228));
            else if (drink == 3)
                quoteText.setFillColor(sf::Color(102, 178, 255));
            else
                quoteText.setFillColor(sf::Color(0, 153, 0));

            for (int i = 0; i < CONST_maxBalls; i++)
                ball[i].active = 0;

            activeBalls = 0;
            balledTime = 0;

            vendor.active = 0;

            seaLevel = CONST_defaultSeaLevel;
            tideTime = tideRest = 0;
            tideActive = 0;
            tideCooldown = -240;

            if (drink == 3)
            {
                SeaSpr.setPosition(0, seaLevel);
                SeaSpr1.setPosition(Wx, seaLevel);
            }

            seaOpacity = 255;
            seashineOpacity = 0;
            SeaSpr.setColor(sf::Color(255, 255, 255, seaOpacity));
            SeaSpr1.setColor(sf::Color(255, 255, 255, seaOpacity));
            SeashineSpr.setColor(sf::Color(255, 255, 255, seashineOpacity));

            underwaterActive = whale.active = whale.carDespawn = whale.noReturn = whale.stop = whale.onShore = 0;
            activeFishes = 0;
            
            for (int i = 0; i < CONST_maxFishes; i++)
                for (int j = 0; j < 4; j++)
                    fish[i][j].active = 0;

            for (int i = 0; i < 6; i++)
                usedFlane[i] = 0;

            jellyfishTime = goldfishTime = underwaterTime = immunityTime = 0;

            darknessOpacity = 0;
            DarknessSpr.setColor(sf::Color(255, 255, 255, darknessOpacity));

            songVolume = 100;
            CurrentSong.setVolume(songVolume * musicVolume);

            donutTime = playerMoney = 0;

            for (int i = 0; i < CONST_maxMoneybags; i++)
                moneybag[i].active = 0;

            summonWhale = bellfishEquipped = donutEquipped = cornEquipped = seashellEquipped = p1Dead = 0;

            tsunamiCooldown = -1200;

            status = 1;

            delete exitButton;
        }
        else if (status == 5)                                                                       // drinking real !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        {
            if (!settingsActive)
            {
                if (!exitActive)
                {
                    DetectSettingsPressed();
                    DetectMenuStuffPressed();
                }
            }

            if (exitActive)
                DetectConfirmExit();
            else
                DetectExitButton();

            if (upPressed or downPressed)
                spacePressed = 1;

            if (flickerTime < 0)
            {
                flickerTime++;
                if (flickerTime == 0)
                    MenuSpr.setTexture(MenuTex);
            }
            else
            {
                Rand = rand() % 1000;
                if (Rand < 40)
                {
                    Rand = rand() % 100;
                    if (Rand < 50)
                        flickerTime = -10;
                    else if (Rand < 75)
                        flickerTime = -15;
                    else if (Rand < 85)
                        flickerTime = -20;
                    else if (Rand < 95)
                        flickerTime = -25;
                    else
                        flickerTime = -50;
                    MenuSpr.setTexture(MenuTex1);
                }
            }

            if (!isDrinkMoving)
            {
                if (rightPressed)
                {
                    drinkOld = drink;
                    drink++;
                    drink %= CONST_maxDrinks;
                    DrinkSpr[drink].setPosition(Wx + DrinkTex[drink].getSize().x / 2, 640);
                    drinkDirection = 0;
                    isDrinkMoving = 1;

                }
                else if (leftPressed)
                {
                    int i = 0;
                    drinkOld = drink;
                    drink--;
                    if (drink < 0)
                        drink = CONST_maxDrinks - 1;
                    else
                        drink %= CONST_maxDrinks;
                    i -= (DrinkTex[drink].getSize().x / 2);
                    DrinkSpr[drink].setPosition(i, 640);
                    drinkDirection = 1;
                    isDrinkMoving = 1;
                }
                else if (spacePressed)
                {
                    status = 4;
                    window.setKeyRepeatEnabled(true);
                }
            }
            else
            {
                if (!drinkDirection)
                {
                    if (DrinkSpr[drink].getPosition().x > 950)
                        DrinkSpr[drink].move(-20, 0);
                    DrinkSpr[drinkOld].move(-20, 0);

                    if (DrinkSpr[drinkOld].getPosition().x + DrinkTex[drinkOld].getSize().x / 2 <= 0 and DrinkSpr[drink].getPosition().x <= 950)
                        isDrinkMoving = 0;
                }
                else
                {
                    if (DrinkSpr[drink].getPosition().x < 950)
                        DrinkSpr[drink].move(20, 0);
                    DrinkSpr[drinkOld].move(20, 0);

                    if (DrinkSpr[drinkOld].getPosition().x - DrinkTex[drinkOld].getSize().x / 2 >= Wx and DrinkSpr[drink].getPosition().x >= 950)
                        isDrinkMoving = 0;
                }
            }

            sf::Text drinkName, drinkDescription;

            drinkName.setFont(typewriter);
            drinkName.setString(drinkNames[drink]);
            drinkName.setCharacterSize(46);
            drinkName.setOrigin(drinkName.getGlobalBounds().getSize().x / 2, drinkName.getGlobalBounds().getSize().y / 2);
            drinkName.setOutlineColor(sf::Color(0, 0, 0, 128));
            drinkName.setOutlineThickness(5);
            drinkName.setPosition(Wx / 2, 995);

            drinkDescription.setFont(typewriter);
            drinkDescription.setString(drinkDesc[drink]);
            drinkDescription.setCharacterSize(30);
            drinkDescription.setOrigin(drinkDescription.getGlobalBounds().getSize().x / 2, drinkDescription.getGlobalBounds().getSize().y / 2);
            drinkDescription.setOutlineColor(sf::Color(0, 0, 0, 128));
            drinkDescription.setOutlineThickness(3);
            drinkDescription.setPosition(Wx / 2, 1045);

            window.draw(MenuSpr);
            window.draw(selectDrink);
            window.draw(drinkName);
            window.draw(drinkDescription);
            window.draw(DrinkSpr[drink]);
            window.draw(DrinkSpr[drinkOld]);

            if (!settingsActive)
            {
                window.draw(SettingsButtonSpr);
                window.draw(ArrowSpr1);
                window.draw(ArrowSpr2); 
                window.draw(DriveSpr);
            }

            HighscoreToText();
            window.draw(highscoreMenu);

            if (settingsActive)
                ProcessSettings(window);

            if (!exitActive)
            {
                window.draw(exitButton->spr);
            }
            else
            {
                window.draw(exitConfirm->spr);
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                mouseHeld = 1;
            else
                mouseHeld = 0;

            CursorSpr.setPosition(MousePosition.x, MousePosition.y);
            window.draw(CursorSpr);
        }
        else if (status == 6)                // pause game
        {
            DrawStuff(window);

            ProcessSettings(window);

            CursorSpr.setPosition(MousePosition.x, MousePosition.y);
            window.draw(CursorSpr);

            if (escPressed)
            {
                status = prevStatus;
                window.setKeyRepeatEnabled(true);
                ResumeAllSounds();
                OverwriteSave();
            }
        }
        else if (status == 0) // title screen
        {
            window.setKeyRepeatEnabled(false);

            if (keyPressed == 1)
            {
                status = 3;
                keyPressed = 0;
            }

            titleFlicker++;
            if (titleFlicker <= 45)
                window.draw(TitleScreenSpr1);
            else
                window.draw(TitleScreenSpr);
            titleFlicker %= 90;

            lastDrink = -1;
        }
        else if (status == -1)
        {
            window.draw(LoadingSpr);
            status = -2;
        }
        else if (status == -2)
        {
            window.draw(LoadingSpr);
            LoadStuff();

            status = 0;
        }

        OyeDetector();

        if (oyeTime < 0)
        {
            if (oyeTime >= -30)
            {
                oyeOpacity -= 8;
                OyeSpr.setColor(sf::Color(255, 255, 255, oyeOpacity));
            }
            window.draw(OyeSpr);

            oyeTime++;
        }

        deltaTime = clock() - oldTime;
        while (deltaTime < 1000.0f / 60.0f)
            deltaTime = clock() - oldTime;
        oldTime = clock();

        window.display();

        if (gameExited)
            return 0;
    }

    return 0;
}