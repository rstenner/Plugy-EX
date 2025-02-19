/*=================================================================
	File created by Yohann NICOLAS.
	Add support 1.13d by L'Autour.
	Add support 1.14d by haxifix.

	World Event Management.

=================================================================*/

#include "worldEvent.h"
#include "common.h"
#include "d2functions.h"
#include <d2constants.h>
#include "modifMemory.h"
#include "error.h"
#include <string>

namespace PlugY {
#pragma pack(1)
    struct s_WEdata {
        BYTE type;
        BYTE fct;
        BYTE id;
        DWORD param;
        WORD z;
        BYTE uk[31];
    };
#pragma pack()
    bool active_WorldEvent = false;
    DWORD WEactive = 0;
    DWORD DCloneSpawned = 0;
    DWORD nbSOJSold = 0;
    DWORD nbNeedSOJSold = 0;
    DWORD nbManagedSOJSold = 0;
    DWORD nbTicksForNextSOJSold = 0;
    DWORD prevTicks = 0;
    DWORD showSOJSoldCounterInAllDiff = 0;
    std::string itemsToSell = "The Stone of Jordan";
    DWORD worldEventmonsterID = 333;
    DWORD valueOfOwnSOJSold = 100;
    DWORD valueInitSOJSoldMin = 200;
    DWORD valueInitSOJSoldDelta = 2000;
    DWORD triggerAtSolJSoldMin = 75;
    DWORD triggerAtSolJSoldDelta = 51;
    bool active_AutomaticSell = true;
    DWORD timeBeforeAutoSellMin = 00000;
    DWORD timeBeforeAutoSellDelta = 120000;
    CubeInput itemNeeded;

    DWORD getTicksForNextSOJSold() {
        return ((DWORD) (rand() / (RAND_MAX + 1.0) * timeBeforeAutoSellDelta) + timeBeforeAutoSellMin);//average of +100 in 25hours max 41h40
    }

    void __fastcall sendDataToClient(void *ptclient, DWORD *param) {
        D2SendPacket(ptclient, (void *) param[0], (DWORD) param[1]);
    }

    void __stdcall worldEventBroadcast(Game *ptGame, DWORD activeWE, DWORD nbSOJSold) {
        void *param[2];
        s_WEdata data;
        data.type = 0x5A;
        data.fct = activeWE ? 0x12 : 0x11;
        data.id = 4;
        data.param = nbSOJSold;
        data.z = 0;
        param[0] = &data;
        param[1] = (void *) sizeof(data);
        D2BroadcastFunction(ptGame, &sendDataToClient, param);
    }

    Game *__stdcall WEManagement(DWORD clientID) {
        Game *ptGame = D2GetGameByClientID(clientID);
        if (!ptGame) return ptGame;
        if (active_AutomaticSell) {
            while (GetTickCount() - prevTicks >= nbTicksForNextSOJSold) {
                nbSOJSold++;
                prevTicks = prevTicks + nbTicksForNextSOJSold;
                nbTicksForNextSOJSold = getTicksForNextSOJSold();
            }
        }
        if (nbSOJSold == nbManagedSOJSold)
            return ptGame;
        DWORD newWE;
        if (!WEactive && (ptGame->difficultyLevel == D2DM_HELL) && (nbSOJSold >= nbNeedSOJSold)) {
            newWE = 1;
            WEactive = 1;
        } else
            newWE = 0;
        if (showSOJSoldCounterInAllDiff || (ptGame->difficultyLevel == D2DM_HELL))
            worldEventBroadcast(ptGame, newWE, nbSOJSold);
        nbManagedSOJSold = nbSOJSold;
        return ptGame;
    }

    DWORD __fastcall spawnDClone(Game *ptGame, Room *ptRoom, DWORD p3, DWORD p4, DWORD p5, DWORD p6, DWORD monsterID, DWORD p8) {
        if (WEactive && (ptGame->difficultyLevel == D2DM_HELL) && !DCloneSpawned) {
            DCloneSpawned = 1;
            D2SpawnSuperUnique(ptGame, ptRoom, p3, p4, p5, p6, worldEventmonsterID, p8);
            return 0;//no minions
        } else
            return D2SpawnSuperUnique(ptGame, ptRoom, p3, p4, p5, p6, monsterID, p8);
    }

    DWORD __stdcall verifIfWEItem(Commons::Unit *ptItem, DWORD flags, DWORD line, const char *filename) {
        ItemsBIN *ptItemStats = D2GetItemsBIN(ptItem->nTxtFileNo);
        ItemsBIN *ptWantedItemStats = D2GetItemsBIN(itemNeeded.ID);
        if ((itemNeeded.byItemTypeID && D2CheckItemType(ptItem, itemNeeded.ID))
            || (itemNeeded.byItemID && (itemNeeded.ID == 0xFFFF))
            || (itemNeeded.byItemID && !itemNeeded.includeUpgradedVersions && ((DWORD) itemNeeded.ID == ptItem->nTxtFileNo))
            || (itemNeeded.byItemID && itemNeeded.includeUpgradedVersions) && (
                (ptItemStats->ItemCode == ptItemStats->NormCode) && (ptItemStats->ItemCode == ptWantedItemStats->NormCode)
                || (ptItemStats->ItemCode == ptItemStats->UberCode) && (ptItemStats->ItemCode == ptWantedItemStats->NormCode)
                || (ptItemStats->ItemCode == ptItemStats->UberCode) && (ptItemStats->ItemCode == ptWantedItemStats->UberCode)
                || (ptItemStats->ItemCode == ptItemStats->HellCode) && (ptItemStats->ItemCode == ptWantedItemStats->NormCode)
                || (ptItemStats->ItemCode == ptItemStats->HellCode) && (ptItemStats->ItemCode == ptWantedItemStats->UberCode)
                || (ptItemStats->ItemCode == ptItemStats->HellCode) && (ptItemStats->ItemCode == ptWantedItemStats->HellCode)
        ))
            if (!(itemNeeded.isSpecificItem && (D2GetUniqueID(ptItem) != itemNeeded.specificID - 1))
                && !(itemNeeded.haveNoSocket && (D2GetPlayerStat(ptItem, STATS_ITEM_NUMSOCKETS, 0) > 0))
                && !(itemNeeded.haveSockets && (D2GetPlayerStat(ptItem, STATS_ITEM_NUMSOCKETS, 0) == 0))
                && !(itemNeeded.isNotEthereal && ptItem->ptItemData->isEtheral)
                && !(itemNeeded.isEthereal && !ptItem->ptItemData->isEtheral)
                && !(itemNeeded.isBasic && (ptItemStats->ItemCode != ptWantedItemStats->NormCode))
                && !(itemNeeded.isExceptional && (ptItemStats->ItemCode != ptWantedItemStats->UberCode))
                && !(itemNeeded.isElite && (ptItemStats->ItemCode != ptWantedItemStats->HellCode))
                && !(itemNeeded.isNotRuneword && ptItem->ptItemData->isRuneword)) {
                nbSOJSold += valueOfOwnSOJSold;
                return 1;// Can't re-buy the item.
            }
        return D2TestFlags(ptItem, flags, line, filename);
    }

    void initWorldEventVariables() {
        char buf[50];
        memset(&itemNeeded, 0, sizeof(itemNeeded));
        strncpy(buf, itemsToSell.c_str(), 50);
        D2CompileCubeInput(&itemNeeded, buf, 0, 0);
        nbManagedSOJSold = 0;
        DCloneSpawned = 0;
        WEactive = 0;
        while (nbSOJSold >= nbNeedSOJSold)
            nbNeedSOJSold += (DWORD) (rand() / (RAND_MAX + 1.0) * triggerAtSolJSoldDelta + triggerAtSolJSoldMin);
        log_msg("initWorldEventVariables - nbSOJSold = %d, nbNeedSOJSold = %d\n", nbSOJSold, nbNeedSOJSold);
    }

    __declspec(naked)void caller_WEManagement_114() {
        __asm{
        PUSH ECX
        CALL WEManagement
        RETN
        }
    }

    __declspec(naked)void caller_WEManagement_1XX() {
        __asm{
        POP EAX
        PUSH ECX
        PUSH EAX
        JMP WEManagement
        }
    }

    __declspec(naked)void caller_spawnDClone_111() {
        __asm{
        PUSH DWORD PTR SS:[ESP+0x14]
        PUSH EAX
        PUSH DWORD PTR SS:[ESP+0x18]
        PUSH DWORD PTR SS:[ESP+0x18]
        PUSH DWORD PTR SS:[ESP+0x18]
        PUSH ECX
        MOV ECX, DWORD PTR SS:[ESP+0x1C]
        CALL spawnDClone
        RETN 0x14
        }
    }

    __declspec(naked)void caller_spawnDClone_111b() {
        __asm{
        PUSH EDX
        PUSH ECX
        PUSH DWORD PTR SS:[ESP+0x1C]
        PUSH DWORD PTR SS:[ESP+0x1C]
        PUSH DWORD PTR SS:[ESP+0x1C]
        PUSH EAX
        MOV EDX, DWORD PTR SS:[ESP+0x20]
        MOV ECX, DWORD PTR SS:[ESP+0x1C]
        CALL spawnDClone
        RETN 0x14
        }
    }

    __declspec(naked)void caller_spawnDClone_114() {
        __asm{
        PUSH EBX
        PUSH ECX
        PUSH EDX
        PUSH EDI
        PUSH ESI

        PUSH 0
        PUSH EBX
        PUSH 0xFFFFFFFF
        PUSH DWORD PTR SS :[ESP + 0x30]
        PUSH DWORD PTR SS :[ESP + 0x30]
        PUSH DWORD PTR SS :[ESP + 0x30]
        MOV ECX, EDI
        MOV EDX, EAX
        CALL spawnDClone

        POP ESI
        POP EDI
        POP EDX
        POP ECX
        POP EBX
        RETN 0x18
        }
    }

    __declspec(naked)void caller_addClientForWE_111() {
        __asm{
        PUSH EAX
        CALL initWorldEventVariables
        POP EAX
        JMP D2AddClient
        }
    }

    __declspec(naked)void caller_addClientForWE() {
        __asm{
        PUSH ECX
        CALL initWorldEventVariables
        POP ECX
        JMP D2AddClient
        }
    }

    void Install_WorldEvent() {
        static int isInstalled = false;
        if (isInstalled) return;
        if (version_D2Game < V110)
            return;
        nbSOJSold = (DWORD) (rand() / (RAND_MAX + 1.0) * valueInitSOJSoldDelta + valueInitSOJSoldMin);
        if (active_AutomaticSell) {
            prevTicks = GetTickCount();
            nbTicksForNextSOJSold = (DWORD) (rand() / (RAND_MAX + 1.0) * (timeBeforeAutoSellDelta + timeBeforeAutoSellMin));
        }
        log_msg("Patch D2Game for active World Event. (WorldEvent)\n");

        // spawn DClone
        mem_seek(offset_D2Game + (version_D2Game == V114d ? 0x1A4A4F : (version_D2Game == V113d ? 0xCFBD0 : (version_D2Game == V113c ? 0x25280 : (version_D2Game == V112 ? 0x41570 : (version_D2Game == V111b ? 0xECF10 : (version_D2Game == V111 ? 0x4BCB1 : (version_D2Game == V110 ? 0x3F720 : (version_D2Game == V109d ? 0x0000 : 0x0000)))))))));
        MEMC_REF4(V2SpawnSuperUnique, version_D2Game >= V114a ? (DWORD) caller_spawnDClone_114 : version_D2Game >= V111b ? (DWORD) caller_spawnDClone_111b : version_D2Game == V111 ? (DWORD) caller_spawnDClone_111 : (DWORD) spawnDClone);
        //6FC6F71F  |. E8 FCFAFFFF    CALL D2Game.6FC6F220
        //01FCBCB0  |. E8 2BEFFFFF    CALL D2Game.01FCABE0                     ; \D2Game.01FCABE0
        //0205CF0F  |. E8 CCF8FFFF    CALL D2Game.0205C7E0                     ; \D2Game.0205C7E0
        //6FC6156F  |. E8 1CF6FFFF    CALL D2Game.6FC60B90                     ; \D2Game.6FC60B90
        //6FC4527F  |. E8 CCF6FFFF    CALL D2Game.6FC44950                     ; \D2Game.6FC44950
        //6FCEFBCF  |. E8 4CE2FFFF    CALL D2Game.6FCEDE20                     ; \D2Game.6FCEDE20
        //005A4A4E  |. E8 8DBFFFFF    CALL Game.005A09E0                       ; \Game.005A09E0

        // verify if the item sold is a trigger of WE
        mem_seek(offset_D2Game + (version_D2Game == V114d ? 0x179667 : (version_D2Game == V113d ? 0x72BE9 : (version_D2Game == V113c ? 0xBFB29 : (version_D2Game == V112 ? 0x84499 : (version_D2Game == V111b ? 0x92859 : (version_D2Game == V111 ? 0x8E799 : (version_D2Game == V110 ? 0x977D0 : (version_D2Game == V109d ? 0x0000 : 0x0000)))))))));
        MEMJ_REF4(D2TestFlags, verifIfWEItem);
        //6FCC77CF  |. E8 32400500    CALL <JMP.&D2Common.#10707>
        //0200E798  |. E8 E9BDF7FF    CALL <JMP.&D2Common.#10911>
        //02002858  |. E8 E57DF7FF    CALL <JMP.&D2Common.#10303>
        //6FCA4498  |. E8 3B62F8FF    CALL <JMP.&D2Common.#10989>
        //6FCDFB28  |. E8 77ADF4FF    CALL <JMP.&D2Common.#10202>
        //6FC92BE8  |. E8 DD7AF9FF    CALL <JMP.&D2Common.#10458>
        //00579666  |. E8 35EA0A00    CALL Game.006280A0                       ; \Game.006280A0

        // management of the WorldEvent
        mem_seek(offset_D2Game + (version_D2Game == V114d ? 0x12FEED : (version_D2Game == V113d ? 0xE5F51 : (version_D2Game == V113c ? 0x4A791 : (version_D2Game == V112 ? 0xEBF41 : (version_D2Game == V111b ? 0xC5681 : (version_D2Game == V111 ? 0x51F01 : (version_D2Game == V110 ? 0x3CE0 : (version_D2Game == V109d ? 0x0000 : 0x0000)))))))));
        MEMC_REF4(V2GetGameByClientID, version_D2Game >= V114a ? (DWORD) caller_WEManagement_114 : version_D2Game >= V111 ? (DWORD) WEManagement : (DWORD) caller_WEManagement_1XX);
        //6FC33CDF   . E8 FC570000    CALL D2Game.6FC394E0
        //01FD1F00  |. E8 1BE60800    CALL D2Game.02060520
        //02035680  |. E8 1BF30100    CALL D2Game.020549A0
        //6FD0BF40  |. E8 1BA4FBFF    CALL D2Game.6FCC6360
        //6FC6A790  |. E8 4B03FEFF    CALL D2Game.6FC4AAE0
        //6FD05F50  |. E8 AB67FDFF    CALL D2Game.6FCDC700
        //0052FEEC  |. E8 6FE9FFFF    CALL Game.0052E860
        //to check : 005389B0  |. E8 2B75FFFF    CALL Game.0052FEE0

        // add client for the WorldEvent
        mem_seek(offset_D2Game + (version_D2Game == V114d ? 0x13F2D2 : (version_D2Game == V113d ? 0xD556F : (version_D2Game == V113c ? 0xCB0BF : (version_D2Game == V112 ? 0x6265F : (version_D2Game == V111b ? 0x7055A : (version_D2Game == V111 ? 0x3786A : (version_D2Game == V110 ? 0x1AEF : (version_D2Game == V109d ? 0x0000 : 0x0000)))))))));
        MEMC_REF4(D2AddClient, version_D2Game >= V111 && version_D2Game <= V113d ? caller_addClientForWE_111 : caller_addClientForWE);
        //6FC31AEE  |. E8 6D510000    CALL D2Game.6FC36C60
        //01FB7869  |. E8 32C50A00    CALL D2Game.02063DA0
        //01FE0559  |. E8 B27C0700    CALL D2Game.02058210
        //6FC8265E  |. E8 FD890800    CALL D2Game.6FD0B060
        //6FCEB0BE  |. E8 6DE8F7FF    CALL D2Game.6FC69930
        //6FCF556E  |. E8 FDFA0000    CALL D2Game.6FD05070
        //0053F2D1  |. E8 7AD2FEFF    CALL Game.0052C550

        log_msg("\n");
        isInstalled = true;
    }

}