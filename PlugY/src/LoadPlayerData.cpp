/*=================================================================
	File created by Yohann NICOLAS.
	Add support 1.13d by L'Autour.
	Add support 1.14d by haxifix.

	Load Player Custom Data.

=================================================================*/

#include "infinityStash.h"
#include "extendedSaveFile.h"
#include "sharedSaveFile.h"
#include "common.h"
#include "d2functions.h"
#include "error.h"
#include "modifMemory.h"
#include <d2constants.h>

namespace PlugY {
/************************************ LOADING ***********************************/

    DWORD __stdcall LoadSPCustomData(Commons::Unit *ptChar) {
        DWORD size = 0;
        BYTE *data;
        log_msg("--- Start LoadSPCustomData ---\n");
        active_sharedStash = active_SharedStashInMultiPlayer > 0;
        log_msg("active_sharedStash = %d : %d\n\n", active_sharedStash);
        if (!ptChar) {
            log_msg("LoadSPCustomData : ptChar == NULL\n");
            return 0x1B;
        }//Unknow failure
        if (ptChar->nUnitType != UNIT_PLAYER) {
            log_msg("LoadSPCustomData : ptChar->nUnitType != UNIT_PLAYER\n");
            return 0x1B;
        }//Unknow failure
        if (!getPlayerData(ptChar)) {
            log_msg("LoadSPCustomData : PCPlayerData == NULL\n");
            return 0x1B;
        }//Unknow failure
        if (!getGame(ptChar)) {
            log_msg("LoadSPCustomData : getGame == NULL\n");
            return 0x1B;
        }//Unknow failure

        DWORD ret = 0;
        if (getGame(ptChar)->isLODGame) {
            log_msg("is LOD Game\n");
            data = readExtendedSaveFile(getPlayerData(ptChar)->name, &size);
            ret = loadExtendedSaveFile(ptChar, data, size);
            D2FogMemDeAlloc(data, __FILE__, __LINE__, 0);
            if (!ret && active_sharedStash) {
                data = readSharedSaveFile(getPlayerData(ptChar)->name, &size);
                ret = loadSharedSaveFile(ptChar, data, size);
                D2FogMemDeAlloc(data, __FILE__, __LINE__, 0);
            }
        } else {
            log_msg("is not LOD Game\n");
        }
        log_msg("End LoadSPCustomData.\n\n");
        return ret;
    }

    __declspec(naked)void caller_LoadSPPlayerCustomData_114() {
        __asm{
        MOV EDI, EAX
        TEST EDI, EDI
        JNZ JMP_LoadSPPlayerCustomData
        PUSH DWORD PTR SS:[ESP+0x1C]
        CALL LoadSPCustomData
        MOV EDI, EAX
        TEST EDI, EDI
        JNZ JMP_LoadSPPlayerCustomData
        RETN
        JMP_LoadSPPlayerCustomData:
        ADD DWORD PTR SS:[ESP], 0x14
        RETN
        }
    }

    __declspec(naked)void caller_LoadSPPlayerCustomData() {
        __asm{
        MOV ESI, EAX
        TEST ESI, ESI
        JNZ JMP_LoadSPPlayerCustomData
        PUSH DWORD PTR SS:[ESP+0x14]
        CALL LoadSPCustomData
        MOV ESI, EAX
        TEST ESI, ESI
        JNZ JMP_LoadSPPlayerCustomData
        RETN
        JMP_LoadSPPlayerCustomData:
        ADD DWORD PTR SS:[ESP], 0x17
        RETN
        }
    }

/*
return value :
0:ok
1:Unable to enter game. Bad character version
2:Unable to enter game. Bad character quest data
3:Unable to enter game. Bad character waypoint data
4:Unable to enter game. Bad character stats data
5:Unable to enter game. Bad character skills data
6:Unable to enter game
7:Unable to enter game. Bad inventory data
8:Unable to enter game. Bad dead bodies
9:Unable to enter game. Bad header
0A:Unable to enter game. bad hireables
0B:Unable to enter game, bad intro data
0C:Unable to enter game. bad item
0D:Unable to enter game, bad dead body item
0E:Unable to enter game, generic bad file
0F:Game is full
10:Version do not match....
11:Unable to enter game,Your character must kill Baal to play nightmare game
12:Unable to enter game,Your character must kill Baal in nightmare difficulty to play hell game
13:Unable to enter game, a normal character cannot join a game create an harcore character.
14:Unable to enter game, an harcore character cannot join a game create by a normal character.
15:A dead character cannot join or create any game
16:unknown failure
17:Unable to enter game. A D2 Character cannot join an expansion character
18:Expansion cannot join D2
19:Failures to join game
1A:Unable to enter game
1B:unknown failure
*/
    enum {
        TS_SAVE_PERSONAL = 0,
        TS_SAVE_SHARED,
        TS_PUTGOLD,
        TS_TAKEGOLD
    };
#pragma pack(1)

    typedef struct {
        BYTE packID;
        BYTE packSize;
        DWORD finalSize: 31;
        DWORD isCustom: 1;
        BYTE type;
        BYTE data[0xFF];
    } t_rcvMsg;

#pragma pack()
    struct s_MPSaveFile {
        DWORD clientID;
        int sizeExtended;
        int curExtended;
        BYTE *dataExtended;
        int sizeShared;
        int curShared;
        BYTE *dataShared;
        s_MPSaveFile *prev;
        s_MPSaveFile *next;
        bool completed;
    };

    static BYTE customPackID = 0x6C;

    static s_MPSaveFile *receivedSaveFiles = NULL;

    void freeCurrentCF(DWORD memoryPool, s_MPSaveFile **curSF) {
        if (*curSF == NULL) return;
        D2FreeMem(memoryPool, (*curSF)->dataExtended, __FILE__, __LINE__, 0);
        D2FreeMem(memoryPool, (*curSF)->dataShared, __FILE__, __LINE__, 0);
        if ((*curSF)->next)
            (*curSF)->next->prev = (*curSF)->prev;
        if ((*curSF)->prev)
            (*curSF)->prev->next = (*curSF)->next;
        else
            receivedSaveFiles = (*curSF)->next;
        D2FreeMem(memoryPool, *curSF, __FILE__, __LINE__, 0);
        *curSF = NULL;
    }

    void sendData(BYTE *data, DWORD size, bool isShared) {
        t_rcvMsg pack;
//	d2_assert(size >= 0x40000000, "size of file > 0x40000000", __FILE__, __LINE__);

        pack.packID = customPackID;
        pack.finalSize = size;
        pack.isCustom = true;
        pack.type = isShared ? TS_SAVE_SHARED : TS_SAVE_PERSONAL;
        DWORD sended = 0;
        while (sended < size) {
            pack.packSize = (BYTE) (size - sended > 0xFE ? 0xFF : size - sended + 1);
            CopyMemory(pack.data, &data[sended], pack.packSize);
            log_msg("Loading Send Packet: \ttype=%X\t finalSize=%X\t packSize=%02X\t data=%08X\n", pack.type, pack.finalSize, pack.packSize, pack.data);
            D2SendToServer(pack.packSize + 7, 0, &pack);
            sended += pack.packSize - 1;
        }
        log_msg("\n");
    }

    void __fastcall SendSaveFiles(char *ptPath, DWORD maxsize, char *name) {
        D2FogGetSavePath(ptPath, maxsize);
        log_msg("\n--- Start SendSaveFiles ---\n");
        active_sharedStash = active_SharedStashInMultiPlayer == 2;
        log_msg("active_sharedStash = %d : %d\n\n", active_sharedStash);

        // Send Extended Save File
        log_msg("Send Extended Save File\n");
        DWORD sizeExtended = 0;
        BYTE *dataExtended = readExtendedSaveFile(name, &sizeExtended);
        sendData(dataExtended, sizeExtended, false);
        D2FogMemDeAlloc(dataExtended, __FILE__, __LINE__, 0);

        // Send Shared Save File
        //if (active_sharedStash)
        {
            log_msg("Send Shared Save File\n");
            DWORD sizeShared = 0;
            BYTE *dataShared = readSharedSaveFile(name, &sizeShared);
            sendData(dataShared, sizeShared, true);
            D2FogMemDeAlloc(dataShared, __FILE__, __LINE__, 0);
        }

        // Ending load
        log_msg("End SendSaveFiles.\n\n");
    }

    DWORD __stdcall ReceiveSaveFiles(DWORD clientID, t_rcvMsg *msg) {
        if ((msg->packID != customPackID) || !msg->isCustom) return msg->packID;
        log_msg("Loading Receive Packet: clientID=%d\t type=%X\t finalSize=%X\t packSize=%02X\t data=%08X\n", clientID, msg->type, msg->finalSize, msg->packSize, msg->data);
        active_sharedStash = active_SharedStashInMultiPlayer == 2;
        log_msg("active_sharedStash = %d : %d\n\n", active_sharedStash);
        bool isShared;
        switch (msg->type) {
            case TS_SAVE_PERSONAL :
                isShared = false;
                break;
            case TS_SAVE_SHARED :
                isShared = true;
                break;
//	case TS_PUTGOLD :		putGold(ptChar, (DWORD)msg->data); return 0;
//	case TS_TAKEGOLD :		takeGold(ptChar, (DWORD)msg->data); return 0;
            default:
                return 0;//return msg->packID;
        }
        NetClient *ptClient = ptClientTable[clientID & 0xFF];
        s_MPSaveFile *curSF = receivedSaveFiles;
        while (curSF && (clientID != curSF->clientID))
            curSF = curSF->next;
        if (curSF && curSF->completed)
            freeCurrentCF(0 * getClientGame(ptClient)->memoryPool, &curSF);
        if (!curSF) {
            curSF = (s_MPSaveFile *) D2AllocMem(0 * getClientGame(ptClient)->memoryPool, sizeof(s_MPSaveFile), __FILE__, __LINE__, 0);
            ZeroMemory(curSF, sizeof(s_MPSaveFile));
            curSF->clientID = clientID;
            curSF->next = receivedSaveFiles;
            if (receivedSaveFiles) receivedSaveFiles->prev = curSF;
            receivedSaveFiles = curSF;
        }
        log_msg("curSF = %08X\tcurSF->completed = %d\n", curSF, curSF->completed);
        DWORD size = msg->packSize - 1;
        if (isShared) {
            if (curSF->sizeShared == 0)
                curSF->sizeShared = msg->finalSize;
            d2_assert(curSF->sizeShared != msg->finalSize, "Size of shared file has change", __FILE__, __LINE__);
            if (!curSF->dataShared)
                curSF->dataShared = (BYTE *) D2AllocMem(0 * getClientGame(ptClient)->memoryPool, curSF->sizeShared, __FILE__, __LINE__, 0);//		d2_assert(!curSF->dataShared, "Can't malloc data", __FILE__, __LINE__);

            CopyMemory(&curSF->dataShared[curSF->curShared], msg->data, size);
            curSF->curShared += size;

        } else {
            if (curSF->sizeExtended == 0)
                curSF->sizeExtended = msg->finalSize;
            d2_assert(curSF->sizeExtended != msg->finalSize, "Size of extented file has change", __FILE__, __LINE__);
            if (!curSF->dataExtended)
                curSF->dataExtended = (BYTE *) D2AllocMem(0 * getClientGame(ptClient)->memoryPool, curSF->sizeExtended, __FILE__, __LINE__, 0);//		d2_assert(!curSF->dataExtended, "Can't malloc data", __FILE__, __LINE__);

            CopyMemory(&curSF->dataExtended[curSF->curExtended], msg->data, size);
            curSF->curExtended += size;
        }
        if (curSF->sizeExtended && (curSF->sizeExtended == curSF->curExtended) && curSF->sizeShared && (curSF->sizeShared == curSF->curShared)) {
            curSF->completed = true;
            log_msg("ReceiveSaveFiles completed.\n");
        }
        log_msg("End Load Receive Packet\n");
        return msg->packID + 10;
    }

/*
typedef int (STDCALL * t_fct_recv)(SOCKET s, char *buf, int len, int flags);
t_fct_recv fct_recv;
int STDCALL ReceiveSaveFiles_9(DWORD clientID, SOCKET s, char *buf, int len, int flags)
{
	t_rcvMsg* msg = (t_rcvMsg*) buf;
	int nb = fct_recv(s,buf,len,flags);
	if ((nb<6) || (msg->packID != customPackID) || !msg->isCustom || (msg->packSize+7 != nb))
		return nb;
	ReceiveSaveFiles(clientID,msg);
	msg->packSize = 1;
	return 7;
}*/

    DWORD __stdcall LoadMPCustomData(Commons::Unit *ptChar) {
        log_msg("Start LoadMPCustomData\n");
        active_sharedStash = active_SharedStashInMultiPlayer == 2;
        log_msg("active_sharedStash = %d : %d\n\n", active_sharedStash);
        if (!ptChar) return NULL;
        if (!ptChar) {
            log_msg("LoadMPCustomData : ptChar == NULL\n");
            return 0x1B;
        }//Unknow failure
        if (ptChar->nUnitType != UNIT_PLAYER) {
            log_msg("LoadMPCustomData : ptChar->nUnitType != UNIT_PLAYER\n");
            return 0x1B;
        }//Unknow failure
        if (!getPlayerData(ptChar)) {
            log_msg("LoadMPCustomData : PCPlayerData == NULL\n");
            return 0x1B;
        }//Unknow failure

        NetClient *ptClient = D2GetClient(ptChar, (char *)__FILE__, __LINE__);
        s_MPSaveFile *curSF = receivedSaveFiles;
        while (curSF && (ptClient->clientID != curSF->clientID))
            curSF = curSF->next;
        DWORD ret = 0;
        if (!curSF) {
            log_msg("Server has received no data from extra save files of character %s\n", getPlayerData(ptChar)->name);
            ret = 0xE;//Unable to enter game, generic bad file
        }
        if (!ret && !curSF->completed) {
            log_msg("Server hasn't receive all data from extra save files of character %s\n", getPlayerData(ptChar)->name);
            ret = 0xE;//Unable to enter game, generic bad file
        }
        if (!ret && !getGame(ptChar)) {
            log_msg("LoadMPCustomData : getGame == NULL\n");
            ret = 0x1B;//Unknow failure
        }
        if (getGame(ptChar)->isLODGame) {
            log_msg("is LOD Game\n");
            if (!ret)
                ret = loadExtendedSaveFile(ptChar, curSF->dataExtended, curSF->sizeExtended);
            if (!ret && active_sharedStash)
                ret = loadSharedSaveFile(ptChar, curSF->dataShared, curSF->sizeShared);
        } else {
            log_msg("is not LOD Game\n");
        }
        freeCurrentCF(getGame(ptChar)->memoryPool, &curSF);
        log_msg("--- End LoadMPCustomData. ---\n\n");
        return ret;
    }

    __declspec(naked)void caller_LoadMPPlayerCustomData_114() {
        __asm{
        PUSH DWORD PTR SS:[EDI]
        CALL LoadMPCustomData
        TEST EAX, EAX
        JNZ JMP_LoadMPlayerCustomData
        CMP DWORD PTR DS:[EDI], 0
        JNZ Continue_LoadMP
        ADD DWORD PTR SS:[ESP], 0x1D
        Continue_LoadMP:
        RETN
        JMP_LoadMPlayerCustomData:
        SUB DWORD PTR SS:[ESP], 0x10
        RETN
        }
    }

    __declspec(naked)void caller_LoadMPPlayerCustomData_111() {
        __asm{
        PUSH DWORD PTR SS:[EBX]
        CALL LoadMPCustomData
        TEST EAX, EAX
        JNZ JMP_LoadMPlayerCustomData
        CMP DWORD PTR DS:[EBX], 0
        JNZ Continue_LoadMP
        ADD DWORD PTR SS:[ESP], 0x21
        Continue_LoadMP:
        RETN
        JMP_LoadMPlayerCustomData:
        SUB DWORD PTR SS:[ESP], 0x12
        RETN
        }
    }

    __declspec(naked)void caller_LoadMPPlayerCustomData() {
        __asm{
        PUSH DWORD PTR SS:[EDI]
        CALL LoadMPCustomData
        TEST EAX, EAX
        JNZ JMP_LoadMPlayerCustomData
        CMP DWORD PTR DS:[EDI], 0
        JNZ Continue_LoadMP
        ADD DWORD PTR SS:[ESP], 0x46
        Continue_LoadMP:
        RETN
        JMP_LoadMPlayerCustomData:
        SUB DWORD PTR SS:[ESP], 0xD
        RETN
        }
    }

    __declspec(naked)void caller_LoadMPPlayerCustomData_9() {
        __asm{
        PUSH DWORD PTR SS:[EDI]
        CALL LoadMPCustomData
        TEST EAX, EAX
        JNZ JMP_LoadMPlayerCustomData
        CMP DWORD PTR DS:[EDI], 0
        JNZ Continue_LoadMP
        ADD DWORD PTR SS:[ESP], 0x1D
        Continue_LoadMP:
        RETN
        JMP_LoadMPlayerCustomData:
        SUB DWORD PTR SS:[ESP], 0x13
        RETN
        }
    }

    __declspec(naked)void caller_SendSaveFiles_111() {
        __asm{
        POP EAX
        PUSH DWORD PTR CS:[EAX+0x01] //"name"
        PUSH EAX
        JMP SendSaveFiles
        }
    }

    __declspec(naked)void caller_SendSaveFiles() {
        __asm{
        POP EAX
        PUSH DWORD PTR CS:[EAX+0x0F] //"name"
        PUSH EAX
        JMP SendSaveFiles
        }
    }

    __declspec(naked)void caller_ReceiveSaveFiles_114() {
        __asm{
        PUSH ECX
        PUSH EDX
        LEA EBX, DWORD PTR DS:[ESI+4]
        PUSH EBX        //Message
        MOV EBX, DWORD PTR SS:[ESI]
        PUSH EBX        //ID client
        CALL ReceiveSaveFiles
        POP EDX
        POP ECX
        RETN
        }
    }

    __declspec(naked)void caller_ReceiveSaveFiles_111() {
        __asm{
        PUSH ECX
        PUSH EDX
        LEA EBX, DWORD PTR DS:[EBP+4]
        PUSH EBX        //Message
        MOV EBX, DWORD PTR SS:[EBP]
        PUSH EBX        //ID client
        CALL ReceiveSaveFiles
        POP EDX
        POP ECX
        RETN
        }
    }

    __declspec(naked)void caller_ReceiveSaveFiles() {
        __asm{
        PUSH ECX
        PUSH EDX
        LEA EDI, DWORD PTR DS:[ESI+4]
        PUSH EDI        //Message
        MOV EDI, DWORD PTR DS:[ESI]
        PUSH EDI        //ID client
        CALL ReceiveSaveFiles
        POP EDX
        POP ECX
        RETN
        }
    }

    __declspec(naked)void caller_BugFix109b() {
        __asm{
        MOV EAX, DWORD PTR DS:[EDI+0xBD0]
        TEST EAX, EAX
        JE go_to_default
        MOV EAX, DWORD PTR SS:[ESP+0x20]
        MOV DWORD PTR SS:[ESP+0x24], EAX
        RETN
        go_to_default:
        SUB DWORD PTR SS:[ESP], 0x4B
        RETN
        }
    }

    __declspec(naked)void caller_BugFix109d() {
        __asm{
        MOV EAX, DWORD PTR DS:[ESI+0xAB0]
        TEST EAX, EAX
        JE go_to_default
        MOV EAX, DWORD PTR SS:[ESP+0x20]
        MOV DWORD PTR SS:[ESP+0x24], EAX
        RETN
        go_to_default:
        SUB DWORD PTR SS:[ESP], 0x4B
        RETN
        }
    }

    void Install_LoadPlayerData() {
        static int isInstalled = false;
        if (isInstalled || !active_PlayerCustomData) return;
        log_msg("Patch D2Game & D2Client for load Player's custom data. (LoadPlayerData)\n");

        // Load SP player custom data.
        mem_seek(
            offset_D2Game + (
                version_D2Game == V114d ? 0x13447A : (
                    version_D2Game == V113d ? 0x3BCCD : (
                        version_D2Game == V113c ? 0x5638D : (
                            version_D2Game == V112 ? 0x465BD : (
                                version_D2Game == V111b ? 0x278CD : (
                                    version_D2Game == V111 ? 0xBB8ED : (version_D2Game == V110 ? 0x5CB0F : (version_D2Game == V109d ? 0x5086F : 0x5046F)))))))));
        memt_byte(0x8B, 0xE8); // CALL caller_LoadSPPlayerCustomData
        MEMT_REF4(version_D2Game >= V114d ? 0x75FF85F8 : 0x75F685F0, version_D2Game >= V114d ? caller_LoadSPPlayerCustomData_114 : caller_LoadSPPlayerCustomData);
        memt_byte(version_D2Game >= V114d ? 0x13 : 0x16, 0x90); // NOP
        //6FC8CB0F   8BF0             MOV ESI,EAX
        //6FC8CB11   85F6             TEST ESI,ESI
        //6FC8CB13   75 16            JNZ SHORT D2Game.6FC8CB2B
        //0203B8ED  |> 8BF0           MOV ESI,EAX
        //0203B8EF  |. 85F6           TEST ESI,ESI
        //0203B8F1  |. 75 16          JNZ SHORT D2Game.0203B909
        //01F978CD  |> 8BF0           MOV ESI,EAX
        //01F978CF  |. 85F6           TEST ESI,ESI
        //01F978D1  |. 75 16          JNZ SHORT D2Game.01F978E9
        //6FC665BD  |> 8BF0           MOV ESI,EAX
        //6FC665BF  |. 85F6           TEST ESI,ESI
        //6FC665C1  |. 75 16          JNZ SHORT D2Game.6FC665D9
        //6FC7638D  |> 8BF0           MOV ESI,EAX
        //6FC7638F  |. 85F6           TEST ESI,ESI
        //6FC76391  |. 75 16          JNZ SHORT D2Game.6FC763A9
        //6FC5BCCD  |> 8BF0           MOV ESI,EAX
        //6FC5BCCF  |. 85F6           TEST ESI,ESI
        //6FC5BCD1  |. 75 16          JNZ SHORT D2Game.6FC5BCE9
        //0053447A  |. 8BF8           MOV EDI,EAX
        //0053447C  |. 85FF           TEST EDI,EDI
        //0053447E  |. 75 13          JNZ SHORT Game.00534493

        // Load MP player custom data.
        mem_seek(
            offset_D2Game + (
                version_D2Game == V114d ? 0x134572 : (
                    version_D2Game == V113d ? 0x3BB57 : (
                        version_D2Game == V113c ? 0x56217 : (
                            version_D2Game == V112 ? 0x46447 : (
                                version_D2Game == V111b ? 0x27757 : (
                                    version_D2Game == V111 ? 0xBB777 : (version_D2Game == V110 ? 0x5CC66 : (version_D2Game == V109d ? 0x50B90 : 0x50790)))))))));
        memt_byte(0x83, 0xE8);
        MEMT_REF4(version_D2Game >= V114d ? 0x1D74003F : version_D2Game >= V111 ? 0x2174003B : version_D2Game == V110 ? 0x4674003F : 0x1D74003F,
            version_D2Game >= V114d ? caller_LoadMPPlayerCustomData_114 : version_D2Game >= V111 ? caller_LoadMPPlayerCustomData_111 : version_D2Game == V110 ? caller_LoadMPPlayerCustomData : caller_LoadMPPlayerCustomData_9);
        //6FC8CC66  . 833F 00         CMP DWORD PTR DS:[EDI],0
        //6FC8CC69  . 74 46           JE SHORT D2Game.6FC8CCB1
        //0203B777  |> 833B 00        CMP DWORD PTR DS:[EBX],0
        //0203B77A  |. 74 21          JE SHORT D2Game.0203B79D
        //01F97757  |> 833B 00        CMP DWORD PTR DS:[EBX],0
        //01F9775A  |. 74 21          JE SHORT D2Game.01F9777D
        //6FC66447  |> 833B 00        CMP DWORD PTR DS:[EBX],0
        //6FC6644A  |. 74 21          JE SHORT D2Game.6FC6646D
        //6FC76217  |> 833B 00        CMP DWORD PTR DS:[EBX],0
        //6FC7621A  |. 74 21          JE SHORT D2Game.6FC7623D
        //6FC5BB57  |> 833B 00        CMP DWORD PTR DS:[EBX],0
        //6FC5BB5A  |. 74 21          JE SHORT D2Game.6FC5BB7D
        //00534572  |> 833F 00        CMP DWORD PTR DS:[EDI],0
        //00534575  |. 74 1D          JE SHORT Game.00534594

        // Send save files to Server.
        mem_seek(
            offset_D2Client + (
                version_D2Client == V114d ? 0x7807E : (
                    version_D2Client == V113d ? 0xB638C : (
                        version_D2Client == V113c ? 0x1457C : (
                            version_D2Client == V112 ? 0x7933C : (
                                version_D2Client == V111b ? 0x5DFDC : (
                                    version_D2Client == V111 ? 0x733FC : (version_D2Client == V110 ? 0xD5A2 : (version_D2Client == V109d ? 0xCF32 : 0xCF42)))))))));
        MEMJ_REF4(D2FogGetSavePath, version_D2Game >= V111 ? caller_SendSaveFiles_111 : caller_SendSaveFiles);
        //6FAAD5A1  |. E8 88D10B00    CALL <JMP.&Fog.#10115>
        //6FB233FB  |. E8 CA8AF9FF    CALL <JMP.&Fog.#10115>
        //6FB0DFDB  |. E8 C6DEFAFF    CALL <JMP.&Fog.#10115>
        //6FB2933B  |. E8 6A2CF9FF    CALL <JMP.&Fog.#10115>
        //6FAC457B  |. E8 187AFFFF    CALL <JMP.&Fog.#10115>
        //6FB6638B  |. E8 2E5BF5FF    CALL <JMP.&Fog.#10115>
        //0047807D  |. E8 CEEFF8FF    CALL Game.00407050

        // Receive save files from client.
        mem_seek(
            offset_D2Game + (
                version_D2Game == V114d ? 0x13F114 : (
                    version_D2Game == V113d ? 0xD53E9 : (
                        version_D2Game == V113c ? 0xCAF39 : (
                            version_D2Game == V112 ? 0x624D9 : (
                                version_D2Game == V111b ? 0x703D9 : (
                                    version_D2Game == V111 ? 0x376E9 : (version_D2Game == V110 ? 0x191A : (version_D2Game == V109d ? 0x183A : 0x183A)))))))));
        memt_byte(0x8B, 0xE8);
        if (version_D2Game >= V111) {
            MEMT_REF4(version_D2Game >= V114d ? 0x46B60F1E : 0xB60F005D, version_D2Game >= V114d ? caller_ReceiveSaveFiles_114 : caller_ReceiveSaveFiles_111);
            if (version_D2Game < V114a) memt_byte(0x45, 0x90);
            memt_byte(0x04, 0x90);
            //01FB76E9  |. 8B5D 00        MOV EBX,DWORD PTR SS:[EBP]
            //01FB76EC  |. 0FB645 04      MOVZX EAX,BYTE PTR SS:[EBP+4]
            //01FE03D9  |. 8B5D 00        MOV EBX,DWORD PTR SS:[EBP]
            //01FE03DC  |. 0FB645 04      MOVZX EAX,BYTE PTR SS:[EBP+4]
            //6FC824D9  |. 8B5D 00        MOV EBX,DWORD PTR SS:[EBP]
            //6FC824DC  |. 0FB645 04      MOVZX EAX,BYTE PTR SS:[EBP+4]
            //6FCEAF39  |. 8B5D 00        MOV EBX,DWORD PTR SS:[EBP]
            //6FCEAF3C  |. 0FB645 04      MOVZX EAX,BYTE PTR SS:[EBP+4]
            //6FCF53E9  |. 8B5D 00        MOV EBX,DWORD PTR SS:[EBP]
            //6FCF53EC  |. 0FB645 04      MOVZX EAX,BYTE PTR SS:[EBP+4]
            //0053F114  |. 8B1E           MOV EBX,DWORD PTR DS:[ESI]
            //0053F116  |. 0FB646 04      MOVZX EAX,BYTE PTR DS:[ESI+4]
        } else {
            MEMT_REF4(0x04468A3E, caller_ReceiveSaveFiles);
            //6FC3191A  |. 8B3E           MOV EDI,DWORD PTR DS:[ESI]
            //6FC3191C  |. 8A46 04        MOV AL,BYTE PTR DS:[ESI+4]
        }
        if (version_Fog <= V109d) {
            mem_seek(
                offset_Fog + (
                    version_Fog == V114d ? 0x0000 : (
                        version_Fog == V113d ? 0x0000 : (
                            version_Fog == V113c ? 0x0000 : (
                                version_Fog == V112 ? 0x0000 : (
                                    version_Fog == V111b ? 0x0000 : (
                                        version_Fog == V111 ? 0x0000 : (
                                            version_Fog == V110 ? 0x0000 : (version_Fog == V109d ? 0x45AE : 0x47DE)))))))));
            memt_byte(0x8B, 0xE8);
            MEMT_REF4(0x891C2444, version_Fog == V109b ? caller_BugFix109b : caller_BugFix109d);
            memt_byte(0x44, 0x90);
            memt_byte(0x24, 0x90);
            memt_byte(0x20, 0x90);
            //6FF545AE  |> 8B4424 1C      |MOV EAX,DWORD PTR SS:[ESP+1C]           ;  Case 2 of switch 6FF5454C
            //6FF545B2  |. 894424 20      |MOV DWORD PTR SS:[ESP+20],EAX
        }
        if (version_D2Game == V109b || version_D2Game == V109d)
            customPackID -= 3;
        else if (version_D2Game == V110)
            customPackID--;
        log_msg("\n");
        isInstalled = true;
    }
}
