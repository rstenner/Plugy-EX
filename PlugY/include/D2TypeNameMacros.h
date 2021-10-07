#pragma once

#include <D2UnitStruct.h>
#include "D2TypeDefMacros.h"

namespace PlugY {
/// F8 section
    TD2Common11084 D2Common11084;
    TD2GetLevelID D2GetLevelID;
    TD2GetDropRoom D2GetDropRoom;
    TD2Common10242 D2Common10242;
    TD2CanPutItemInInv D2CanPutItemInInv;
    TD2InvRemoveItem D2InvRemoveItem;
    TD2InvAddItem D2InvAddItem;
    TD2Common10250 D2Common10250;
    TD2Common10273 D2Common10273;
    TD2InventoryGetFirstItem D2InventoryGetFirstItem;
    TD2UnitGetNextItem D2UnitGetNextItem;
    TD2GetRealItem D2GetRealItem;
    TD2GetPosX D2GetPosX;
    TD2GetPosY D2GetPosY;
    TD2GetPosition D2GetPosition;
    TD2GetMaxGoldBank D2GetMaxGoldBank;
    TD2GetRoom D2GetRoom;
    TD2InitPlayerData D2InitPlayerData;
    TD2GetPlayerData D2GetPlayerData;
    TD2GetDefence D2GetDefence;
    TD2GetChanceToBlock D2GetChanceToBlock;
    TD2GetMaxGold D2GetMaxGold;
    TD2Common10572 D2Common10572;
    TD2GetObjectFlags D2GetObjectFlags;
    TD2SetObjectFlags D2SetObjectFlags;
    TD2isInState D2isInState;
    TD2AddPlayerStat D2AddPlayerStat;
    TD2GetPlayerStat D2GetPlayerStat;
    TD2GetPlayerBaseStat D2GetPlayerBaseStat;
    TD2haveDefenceBonus D2haveDefenceBonus;
    TD2haveFireResBonus D2haveFireResBonus;
    TD2haveColdResBonus D2haveColdResBonus;
    TD2haveLightResBonus D2haveLightResBonus;
    TD2havePoisonResBonus D2havePoisonResBonus;
    TD2haveDefenceMalus D2haveDefenceMalus;
    TD2haveFireResMalus D2haveFireResMalus;
    TD2haveColdResMalus D2haveColdResMalus;
    TD2haveLightResMalus D2haveLightResMalus;
    TD2havePoisonResMalus D2havePoisonResMalus;
    TD2CompileTxtFile D2CompileTxtFile;
    TD2GetItemsBIN D2GetItemsBIN;
    TD2GetGemsBIN D2GetGemsBIN;
    TD2GetCubeMainBIN D2GetCubeMainBIN;
    TD2GetNbCubeMainBIN D2GetNbCubeMainBIN;
    TD2GetNextLevelXP D2GetNextLevelXP;
    TD2GetMaxLevel D2GetMaxLevel;
    TD2GetDifficultyLevelsBIN D2GetDifficultyLevelsBIN;
    TD2GetItemQuality D2GetItemQuality;
    TD2TestFlags D2TestFlags;
    TD2GetItemLevel D2GetItemLevel;
    TD2ItemGetPage D2ItemGetPage;
    TD2ItemSetPage D2ItemSetPage;
    TD2CheckItemType D2CheckItemType;
    TD2GetUniqueID D2GetUniqueID;
    TD2SetAnim D2SetAnim;
    TD2GetNbRunesBIN D2GetNbRunesBIN;
    TD2GetRunesBIN D2GetRunesBIN;
    TD2SaveItem D2SaveItem;
    TD2SetSkillBaseLevel D2SetSkillBaseLevel;
    TD2GetSkillLevel D2GetSkillLevel;
    TD2GetSkillCost D2GetSkillCost;
    TD2SetNbPlayers D2SetNbPlayers;
    TD2SendToServer D2SendToServer;
    TD2SendToClient D2SendToClient;
    TD2FogAssertOld D2FogAssertOld;
    TD2FogAssert D2FogAssert;
    TD2FogMemAlloc D2FogMemAlloc;
    TD2FogMemDeAlloc D2FogMemDeAlloc;
    TD2AllocMem D2AllocMem;
    TD2FreeMem D2FreeMem;
    TD2MPQOpenFile D2MPQOpenFile;
    TD2MPQCloseFile D2MPQCloseFile;
    TD2MPQReadFile D2MPQReadFile;
    TD2MPQGetSizeFile D2MPQGetSizeFile;
    TD2FogGetSavePath D2FogGetSavePath;
    TD2FogGetInstallPath D2FogGetInstallPath;
    TD2Fog10212 D2Fog10212;
    TD2GetInstructionPointer D2GetInstructionPointer;
    TD2GetStringFromString D2GetStringFromString;
    TD2GetStringFromIndex D2GetStringFromIndex;
    TD2GetLang D2GetLang;
    TD2PrintBigNumber D2PrintBigNumber;
    TD2StormMPQOpenFile D2StormMPQOpenFile;
    TD2Storm503 D2Storm503;
    TD2FreeWinMessage D2FreeWinMessage;
    TD2GetResolution D2GetResolution;
    TD2FillArea D2FillArea;
    TD2PrintImage D2PrintImage;
    TD2PrintLineOnTextBox D2PrintLineOnTextBox;
    TD2PrintString D2PrintString;
    TD2GetPixelLen D2GetPixelLen;
    TD2SetFont D2SetFont;
    TD2PrintPopup D2PrintPopup;
    TD2GetPixelRect D2GetPixelRect;
    TD2PrintTextPopup D2PrintTextPopup;
    TD2CreateTextBox D2CreateTextBox;
    TD2CMP10014 D2CMP10014;
///A8 section
    TD2GetCharStatsBIN D2GetCharStatsBIN;
    TD2CompileCubeInput D2CompileCubeInput;
    TD2GetItemTypesBIN D2GetItemTypesBIN;
    TD2GetItemStatCostBIN D2GetItemStatCostBIN;
    TD2ReadFile D2ReadFile;
    TD2LoadSuperuniques D2LoadSuperuniques;
    TD2GetInventoryBIN D2GetInventoryBIN;
    TD2LoadImage D2LoadImage;
    TD2FreeImage D2FreeImage;
    TD2SendMsgToAll D2SendMsgToAll;
    TD2GetLastMonsterIDFight D2GetLastMonsterIDFight;
    TD2PrintStatsPage D2PrintStatsPage;
    TD2PrintStat D2PrintStat;
    TD2SetColorPopup D2SetColorPopup;
    TD2PlaySound D2PlaySound;
    TD2SendToServerXX D2SendToServerXX;
    TD2TogglePage D2TogglePage;
    TD2ClickOnStashButton D2ClickOnStashButton;
    TD2LoadBuySelBtn D2LoadBuySelBtn;
    TD2LoadResImage D2LoadResImage;
    TD2FreeResImages D2FreeResImages;
    TD2ReceivePacket D2ReceivePacket;
    TD2PrintInterface D2PrintInterface;
    TD2AddClient D2AddClient;
    TD2GetGameByClientID D2GetGameByClientID;
    TD2BroadcastFunction D2BroadcastFunction;
    TD2SendPacket D2SendPacket;
    TD2SetSkillBaseLevelOnClient D2SetSkillBaseLevelOnClient;
    TD2LinkPortal D2LinkPortal;
    TD2VerifIfNotCarry1 D2VerifIfNotCarry1;
    TD2TestPositionInRoom D2TestPositionInRoom;
    TD2SpawnSuperUnique D2SpawnSuperUnique;
    TD2SpawnMonster D2SpawnMonster;
    TD2Game235C0 D2Game235C0;
    TD2LoadInventory D2LoadInventory;
    TD2GameGetObject D2GameGetObject;
    TD2CreateUnit D2CreateUnit;
    TD2OpenPandPortal D2OpenPandPortal;
    TD2OpenPandFinalPortal D2OpenPandFinalPortal;
    TD2MephAI D2MephAI;
    TD2DiabloAI D2DiabloAI;
    TD2BaalAI D2BaalAI;
    TD2UberBaalAI D2UberBaalAI;
    TD2UberMephAI D2UberMephAI;
    TD2UberDiabloAI D2UberDiabloAI;
    TD2SaveGame D2SaveGame;
    TD2MonsterMove D2MonsterMove;
    TD2MonsterUseSkill D2MonsterUseSkill;
    TD2SaveSPChar D2SaveSPChar;
//ONLY UNTIL 1.10 :
    TD2isLODGame D2isLODGame;
    TD2GetDifficultyLevel D2GetDifficultyLevel;
    TD2GetMouseX D2GetMouseX;
    TD2GetMouseY D2GetMouseY;
    TD2GetClientPlayer D2GetClientPlayer;
    TD2CleanStatMouseUp D2CleanStatMouseUp;
    TD2SendToServer3 D2SendToServer3;
    TD2GetClient D2GetClient;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////// VARIABLES ////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// C8 section
    DWORD *ptResolutionY;
    DWORD *ptResolutionX;
    DWORD *ptNegWindowStartY;
    DWORD *ptWindowStartX;
    NetClient **ptClientTable;
    DWORD *ptIsLodGame;
    BYTE *ptDifficultyLevel;
    DWORD *ptMouseY;
    DWORD *ptMouseX;
    Unit **ptptClientChar;
    DWORD *ptInputCommandLen;
    WCHAR *ptInputCommand;
    DWORD *ptNbStatDesc;
    DWORD *ptStatDescTable;
}