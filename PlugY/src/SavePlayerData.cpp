/*=================================================================
	File created by Yohann NICOLAS.
	Add support 1.13d by L'Autour.

    Save Player Custom Data.

=================================================================*/

#include <time.h>
#include <stdio.h>
#include "infinityStash.h"
#include "extendedSaveFile.h"
#include "sharedSaveFile.h"
#include "common.h"
#include "error.h"
#include "d2functions.h"
#include "modifMemory.h"

namespace PlugY {
    bool active_AutoBackup = false;
    int maxBackupPerCharacter = 20;

    void backupSaveFiles(char *name, int isHardCoreGame) {
        if (!active_AutoBackup)
            return;
        char savePath[MAX_PATH];
        char backupPath[MAX_PATH];
        char szSaveName[MAX_PATH];
        char szBackupName[MAX_PATH];
        D2FogGetSavePath(savePath, MAX_PATH);

        // backupfiles
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        sprintf(backupPath, "%sbackup", savePath);
        CreateDirectory(backupPath, NULL);
        sprintf(backupPath, "%sbackup\\%s_%04d%02d%02d_%02d%02d%02d\\", savePath, name, tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
        CreateDirectory(backupPath, NULL);
        log_msg("backup directory:%s isHardcore:%d\n", backupPath, isHardCoreGame);
        sprintf(szSaveName, "%s%s.d2s", savePath, name);
        sprintf(szBackupName, "%s%s.d2s", backupPath, name);
        CopyFile(szSaveName, szBackupName, true);
        sprintf(szSaveName, "%s%s.d2x", savePath, name);
        sprintf(szBackupName, "%s%s.d2x", backupPath, name);
        CopyFile(szSaveName, szBackupName, true);
        sprintf(szSaveName, "%s%s.ma0", savePath, name);
        sprintf(szBackupName, "%s%s.ma0", backupPath, name);
        CopyFile(szSaveName, szBackupName, true);
        sprintf(szSaveName, "%s%s.ma1", savePath, name);
        sprintf(szBackupName, "%s%s.ma1", backupPath, name);
        CopyFile(szSaveName, szBackupName, true);
        sprintf(szSaveName, "%s%s.ma2", savePath, name);
        sprintf(szBackupName, "%s%s.ma2", backupPath, name);
        CopyFile(szSaveName, szBackupName, true);
        sprintf(szSaveName, "%s%s.ma3", savePath, name);
        sprintf(szBackupName, "%s%s.ma3", backupPath, name);
        CopyFile(szSaveName, szBackupName, true);
        sprintf(szSaveName, "%s%s.map", savePath, name);
        sprintf(szBackupName, "%s%s.map", backupPath, name);
        CopyFile(szSaveName, szBackupName, true);
        if (separateHardSoftStash && isHardCoreGame != 0) {
            sprintf(szSaveName, "%s%s%s.sss", savePath, "_LOD_HC_", sharedStashFilename);
            sprintf(szBackupName, "%s%s%s.sss", backupPath, "_LOD_HC_", sharedStashFilename);
            CopyFile(szSaveName, szBackupName, true);
        }
        if (!separateHardSoftStash || isHardCoreGame != 1) {
            sprintf(szSaveName, "%s%s%s.sss", savePath, separateHardSoftStash && isHardCoreGame != 0 ? "_LOD_HC_" : "_LOD_", sharedStashFilename);
            sprintf(szBackupName, "%s%s%s.sss", backupPath, separateHardSoftStash && isHardCoreGame != 0 ? "_LOD_HC_" : "_LOD_", sharedStashFilename);
            CopyFile(szSaveName, szBackupName, true);
        }

        // delete older backup
        if (maxBackupPerCharacter > 0) {
            sprintf(szBackupName, "%sbackup\\%s_*", savePath, name);
            WIN32_FIND_DATA FindFileData;
            HANDLE hFind = FindFirstFile(szBackupName, &FindFileData);
            if (hFind) {
                // Count nb backup folders
                int nbFolders = (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
                while (FindNextFile(hFind, &FindFileData))
                    nbFolders += (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;

                // delete nbFolders - maxfolder
                log_msg("Backup nbFolders: %d\n", nbFolders);
                nbFolders -= maxBackupPerCharacter;
                if (nbFolders > 0) {
                    hFind = FindFirstFile(szBackupName, &FindFileData);
                    do {
                        if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
                            sprintf(backupPath, "%sbackup\\%s\\", savePath, FindFileData.cFileName);
                            log_msg("Delete backup : %s\n", backupPath);
                            sprintf(szBackupName, "%s%s.d2s", backupPath, name);
                            DeleteFile(szBackupName);
                            sprintf(szBackupName, "%s%s.d2x", backupPath, name);
                            DeleteFile(szBackupName);
                            sprintf(szBackupName, "%s%s.ma0", backupPath, name);
                            DeleteFile(szBackupName);
                            sprintf(szBackupName, "%s%s.ma1", backupPath, name);
                            DeleteFile(szBackupName);
                            sprintf(szBackupName, "%s%s.ma2", backupPath, name);
                            DeleteFile(szBackupName);
                            sprintf(szBackupName, "%s%s.ma3", backupPath, name);
                            DeleteFile(szBackupName);
                            sprintf(szBackupName, "%s%s.map", backupPath, name);
                            DeleteFile(szBackupName);
                            sprintf(szBackupName, "%s_LOD_%s.sss", backupPath, sharedStashFilename);
                            DeleteFile(szBackupName);
                            sprintf(szBackupName, "%s_LOD_HC_%s.sss", backupPath, sharedStashFilename);
                            DeleteFile(szBackupName);
                            RemoveDirectory(backupPath);
                            nbFolders--;
                        }
                        FindNextFile(hFind, &FindFileData);
                    } while (nbFolders > 0);
                }
            }
        }
    }

    void __stdcall SaveSPPlayerCustomData(Commons::Unit *ptChar) {
        if (!D2isLODGame()) return;//D2Game but in SP so np

        log_msg("\n--- Start SaveSPPlayerCustomData ---\n");
        char *file = (char *) __FILE__;
        NetClient *ptClient = D2GetClient(ptChar, file, __LINE__);
        backupSaveFiles(getPlayerData(ptChar)->name, ptClient->isHardCoreGame);
        if (active_PlayerCustomData) {
            if (getPYPlayerData(ptChar)->selfStashIsOpened) {
                DWORD curSizeExt = 0;
                DWORD maxSizeExt = 0x4000;
                BYTE *dataExt = (BYTE *) D2AllocMem(getGame(ptChar)->memoryPool, maxSizeExt, file, __LINE__, 0);
                d2_assert(!dataExt, "Error : Memory allocation Extended SaveFile", file, __LINE__);
                saveExtendedSaveFile(ptChar, &dataExt, &maxSizeExt, &curSizeExt);
                writeExtendedSaveFile(getPlayerData(ptChar)->name, dataExt, curSizeExt);
                D2FreeMem(getGame(ptChar)->memoryPool, dataExt, __FILE__, __LINE__, 0);
            }
            if (active_sharedStash && getPYPlayerData(ptChar)->sharedStashIsOpened) {
                DWORD curSizeShr = 0;
                DWORD maxSizeShr = 0x4000;
                BYTE *dataShr = (BYTE *) D2AllocMem(getGame(ptChar)->memoryPool, maxSizeShr, file, __LINE__, 0);
                d2_assert(!dataShr, "Error : Memory allocation Shared SaveFile", file, __LINE__);
                saveSharedSaveFile(ptChar, &dataShr, &maxSizeShr, &curSizeShr);
                writeSharedSaveFile(getPlayerData(ptChar)->name, dataShr, curSizeShr, ptClient->isHardCoreGame);
                D2FreeMem(getGame(ptChar)->memoryPool, dataShr, file, __LINE__, 0);
            }
        }
        log_msg("End saving.\n\n");
    }

    __declspec(naked)void caller_SaveSPPlayerCustomData_114() {
        __asm{
        MOV EAX, DWORD PTR SS:[ESP+8]
        TEST EAX, EAX
        JE next
        PUSH ECX
        PUSH EDX
        PUSH EDX
        CALL SaveSPPlayerCustomData
        POP EDX
        POP ECX
        next:
        JMP D2SaveSPChar
        }
    }

    __declspec(naked)void caller_SaveSPPlayerCustomData_111() {
        __asm{
        MOV EAX, DWORD PTR SS:[ESP+0x14]
        TEST EAX, EAX
        JE next
        PUSH DWORD PTR SS:[ESP+0x8]
        CALL SaveSPPlayerCustomData
        next:
        JMP D2SaveSPChar
        }
    }

/*FCT_ASM ( caller_SaveSPPlayerCustomData_111 )
	CALL D2FogGetSavePath
	PUSH DWORD PTR SS:[ESP+0x2608]
	CALL SaveSPPlayerCustomData
	RETN
}}*/

    __declspec(naked)void caller_SaveSPPlayerCustomData() {
        __asm{
        CALL D2FogGetSavePath
        PUSH EDI
        CALL SaveSPPlayerCustomData
        RETN
        }
    }

    __declspec(naked)void caller_SaveSPPlayerCustomData_9() {
        __asm{
        CALL D2FogGetSavePath
        PUSH ESI
        CALL SaveSPPlayerCustomData
        RETN
        }
    }


//////////////////////////////////////////////////////////////////////////////////////////////////

#define TC_SAVE_PERSONAL 0
#define TC_SAVE_SHARED 1
#pragma pack(1)
    typedef struct {
        BYTE packID;
        BYTE packSize;
        BYTE init;
        DWORD finalSize: 31;
        DWORD isCustom: 1;
        BYTE type;
        BYTE data[0xFF];
    } t_rcvMsg;
#pragma pack()
    struct s_MPSaveFile {
        int sizeExtended;
        int curExtended;
        BYTE *dataExtended;
        int sizeShared;
        int curShared;
        BYTE *dataShared;
    };
    static s_MPSaveFile receivedSaveFiles;
    static BYTE customPackID = 0xB2;
//static s_MPSaveFile* receivedSaveFiles = NULL;


    struct s_dataToSend {
        s_dataToSend *next;
        DWORD clientID;
        int sizeExtended;
        int curExtended;
        BYTE *dataExtended;
        int sizeShared;
        int curShared;
        BYTE *dataShared;
        bool init;
    };
    s_dataToSend *ptDataToSend = NULL;

/*
void sendDataToSave(DWORD clientID, BYTE* data, DWORD size, bool isShared)
{
	t_rcvMsg pack;
	d2_assert(size >= 0x40000000, "size of file > 0x40000000", __FILE__, __LINE__);

	pack.packID = customPackID;
	pack.init = !isShared;
	pack.finalSize = size;
	pack.type = isShared ? TC_SAVE_SHARED : TC_SAVE_PERSONAL;
	pack.isCustom = true;

	DWORD sended = 0;
	while (sended < size)
	{
		pack.packSize = (BYTE) (size - sended > 0xFE ? 0xFE : size - sended);
		CopyMemory(pack.data,  &data[sended], pack.packSize);
		log_msg("send: ID=%02X\tsize=%02X\tinit=%02X\ttotalsize=%08X\ttype=%d\n", pack.packID, pack.init, pack.packSize, pack.finalSize, pack.type);
		pack.packSize++;
		D2SendToClient(0, clientID, (BYTE*)&pack, pack.packSize+7);
		pack.init = false;
		sended += pack.packSize -1;
	}
	log_msg("\n");
}*/

    void __stdcall SendSaveFilesToSave(Commons::Unit *ptChar) {
//	if (!D2isLODGame()) return;
        log_msg("\n--- Start SendSaveFilesToSave ---\n");
        DWORD curSizeExt = 0;
        BYTE *dataExt = NULL;
        DWORD curSizeShr = 0;
        BYTE *dataShr = NULL;
        char *file = (char *) __FILE__;
        if (getPYPlayerData(ptChar)->selfStashIsOpened) {
            DWORD maxSizeExt = 0x4000;
            dataExt = (BYTE *) D2AllocMem(getGame(ptChar)->memoryPool, maxSizeExt, file, __LINE__, 0);
            d2_assert(!dataExt, "Error : Memory allocation Extended SaveFile", file, __LINE__);
            saveExtendedSaveFile(ptChar, &dataExt, &maxSizeExt, &curSizeExt);
        }
        if (active_sharedStash && getPYPlayerData(ptChar)->sharedStashIsOpened) {
            DWORD maxSizeShr = 0x4000;
            dataShr = (BYTE *) D2AllocMem(getGame(ptChar)->memoryPool, maxSizeShr, file, __LINE__, 0);
            d2_assert(!dataShr, "Error : Memory allocation Shared SaveFile", file, __LINE__);
            saveSharedSaveFile(ptChar, &dataShr, &maxSizeShr, &curSizeShr);
        }
        NetClient *ptClient = D2GetClient(ptChar, file, __LINE__);
        s_dataToSend *dataToSend = ptDataToSend;
        while (dataToSend && (dataToSend->clientID != ptClient->clientID))
            dataToSend = dataToSend->next;
        if (!dataToSend) {
            dataToSend = (s_dataToSend *) D2AllocMem(getGame(ptChar)->memoryPool, sizeof(s_dataToSend), file, __LINE__, 0);
            ZeroMemory(dataToSend, sizeof(s_dataToSend));
            dataToSend->next = ptDataToSend;
            ptDataToSend = dataToSend;
        }

        //Sending savefiles
        dataToSend->clientID = ptClient->clientID;
        dataToSend->init = true;
        dataToSend->sizeExtended = curSizeExt;
        dataToSend->curExtended = 0;
        dataToSend->dataExtended = dataExt;
        dataToSend->sizeShared = curSizeShr;
        dataToSend->curShared = 0;
        dataToSend->dataShared = dataShr;
        log_msg("SendSaveFilesToSave : clientID=%d\t init=%d\t sizeExtended=%d\t curExtended=%d\t dataExtended=%d\t sizeShared=%d\t curShared=%d\t dataShared=%08X\n",
                dataToSend->clientID, dataToSend->init, dataToSend->sizeExtended, dataToSend->curExtended, dataToSend->dataExtended, dataToSend->sizeShared, dataToSend->curShared, dataToSend->dataShared);

/*	NetClient* ptClient = D2GetClient(ptChar,__FILE__,__LINE__);
	if (dataExt)
	{
		sendDataToSave(ptClient->clientID, dataExt, curSizeExt, false);
		D2FreeMem(PCGame(ptChar)->memoryPool, dataExt,__FILE__,__LINE__,0);
	}
	if (dataShr)
	{
		sendDataToSave(ptClient->clientID, dataShr, curSizeShr, true);
		D2FreeMem(getGame(ptChar)->memoryPool, dataShr,__FILE__,__LINE__,0);
	}
*/
        log_msg("SendSaveFilesToSave : End\n\n");
    }

    DWORD __stdcall ManageNextPacketToSend(NetClient *ptClient) {
        log_msg("ManageNextPacketToSend\n");
        s_dataToSend *dataToSend = ptDataToSend;
        while (dataToSend && (dataToSend->clientID != ptClient->clientID))
            dataToSend = dataToSend->next;
        if (!dataToSend) return 1;
        log_msg("ManageNextPacketToSend : clientID=%d\t init=%d\t sizeExtended=%d\t curExtended=%d\t dataExtended=%08X\t sizeShared=%d\t curShared=%d\t dataShared=%08X\n",
                dataToSend->clientID, dataToSend->init, dataToSend->sizeExtended, dataToSend->curExtended, dataToSend->dataExtended, dataToSend->sizeShared, dataToSend->curShared, dataToSend->dataShared);
        if (dataToSend->sizeExtended && dataToSend->dataExtended && (dataToSend->curExtended < dataToSend->sizeExtended)) {
            DWORD remainingData = dataToSend->sizeExtended - dataToSend->curExtended;
            t_rcvMsg *msg = (t_rcvMsg *) D2AllocMem(getClientGame(ptClient)->memoryPool, sizeof(t_rcvMsg), __FILE__, __LINE__, 0);
            msg->packID = customPackID;
            msg->init = dataToSend->init;
            msg->finalSize = dataToSend->sizeExtended;
            msg->type = TC_SAVE_PERSONAL;
            msg->isCustom = true;
            msg->packSize = remainingData > 0xFE ? 0xFF : (BYTE) remainingData + 1;
            CopyMemory(msg->data, &dataToSend->dataExtended[dataToSend->curExtended], msg->packSize);
            log_msg("ManageNextPacketToSend : type=%d\t init=%d\t finalSize=%d\t packSize=%d\t data=%08X\n", msg->type, msg->init, msg->finalSize, msg->packSize, msg->data);
            D2SendToClient(0, dataToSend->clientID, msg, msg->packSize + 7);
            dataToSend->init = false;
            dataToSend->curExtended += msg->packSize - 1;
            D2FreeMem(getClientGame(ptClient)->memoryPool, msg, __FILE__, __LINE__, 0);
            if (dataToSend->curExtended == dataToSend->sizeExtended) {
                dataToSend->sizeExtended = 0;
                dataToSend->curExtended = 0;
                D2FreeMem(getClientGame(ptClient)->memoryPool, dataToSend->dataExtended, __FILE__, __LINE__, 0);
                dataToSend->dataExtended = NULL;
            }
            log_msg("ManageNextPacketToSend : End\n");
            return 0;//ManageNextPacketToSend(ptClient);//return 0;
        }
        if (dataToSend->sizeShared && dataToSend->dataShared && (dataToSend->curShared < dataToSend->sizeShared)) {
            DWORD remainingData = dataToSend->sizeShared - dataToSend->curShared;
            t_rcvMsg *msg = (t_rcvMsg *) D2AllocMem(getClientGame(ptClient)->memoryPool, sizeof(t_rcvMsg), __FILE__, __LINE__, 0);
            msg->packID = customPackID;
            msg->init = dataToSend->init;
            msg->finalSize = dataToSend->sizeShared;
            msg->type = TC_SAVE_SHARED;
            msg->isCustom = true;
            msg->packSize = remainingData > 0xFE ? 0xFF : (BYTE) remainingData + 1;
            CopyMemory(msg->data, &dataToSend->dataShared[dataToSend->curShared], msg->packSize);
            log_msg("ManageNextPacketToSend : type=%d\t init=%d\t finalSize=%d\t packSize=%d\t data=%08X\n", msg->type, msg->init, msg->finalSize, msg->packSize, msg->data);
            D2SendToClient(0, dataToSend->clientID, msg, msg->packSize + 7);
            dataToSend->init = false;
            dataToSend->curShared += msg->packSize - 1;
            D2FreeMem(getClientGame(ptClient)->memoryPool, msg, __FILE__, __LINE__, 0);
            if (dataToSend->curShared == dataToSend->sizeShared) {
                dataToSend->sizeShared = 0;
                dataToSend->curShared = 0;
                D2FreeMem(getClientGame(ptClient)->memoryPool, dataToSend->dataShared, __FILE__, __LINE__, 0);
                dataToSend->dataShared = NULL;
            }
            log_msg("ManageNextPacketToSend : End\n");
            return 0;//ManageNextPacketToSend(ptClient);//return 0;
        }
        log_msg("ManageNextPacketToSend : End\n");
        return 1;
    }

    DWORD __stdcall ReceiveSaveFilesToSave(t_rcvMsg *msg) {
        if ((msg->packID != customPackID) || !msg->isCustom) return 0;
        log_msg("ReceiveSaveFilesToSave : Receive Packet - type=%d\t init=%d\t finalSize=%d\t packSize=%d\t data=%08X\n", msg->type, msg->init, msg->finalSize, msg->packSize, msg->data);
        bool isShared;
        if (msg->type == TC_SAVE_PERSONAL)
            isShared = false;
        else if (msg->type == TC_SAVE_SHARED)
            isShared = true;
        else return 0;
        if (msg->init) {
            D2FogMemDeAlloc(receivedSaveFiles.dataExtended, __FILE__, __LINE__, 0);
            D2FogMemDeAlloc(receivedSaveFiles.dataShared, __FILE__, __LINE__, 0);
            ZeroMemory(&receivedSaveFiles, sizeof(receivedSaveFiles));
        }
        log_msg("ReceiveSaveFilesToSave : sizeExtended=%d\t curExtended=%d\t dataExtended=%08X\t sizeShared=%d\t curShared=%d\t dataShared=%08X\n", receivedSaveFiles.sizeExtended, receivedSaveFiles.curExtended, receivedSaveFiles.dataExtended, receivedSaveFiles.sizeShared, receivedSaveFiles.curShared, receivedSaveFiles.dataShared);
        DWORD size = msg->packSize - 1;
        if (isShared) {
            if (receivedSaveFiles.sizeShared == 0)
                receivedSaveFiles.sizeShared = msg->finalSize;
//		d2_assert(receivedSaveFiles.sizeShared != msg->finalSize, "Size of shared file has change", __FILE__, __LINE__);

            if (!receivedSaveFiles.dataShared)
                receivedSaveFiles.dataShared = (BYTE *) D2FogMemAlloc(receivedSaveFiles.sizeShared, __FILE__, __LINE__, 0);
            CopyMemory(&receivedSaveFiles.dataShared[receivedSaveFiles.curShared], msg->data, size);
            receivedSaveFiles.curShared += size;

        } else {
            if (receivedSaveFiles.sizeExtended == 0)
                receivedSaveFiles.sizeExtended = msg->finalSize;
//		d2_assert(receivedSaveFiles.sizeExtended != msg->finalSize, "Size of extented file has change", __FILE__, __LINE__);

            if (!receivedSaveFiles.dataExtended)
                receivedSaveFiles.dataExtended = (BYTE *) D2FogMemAlloc(receivedSaveFiles.sizeExtended, __FILE__, __LINE__, 0);
            CopyMemory(&receivedSaveFiles.dataExtended[receivedSaveFiles.curExtended], msg->data, size);
            receivedSaveFiles.curExtended += size;
        }
        log_msg("ReceiveSaveFilesToSave : End\n");
        return 1;
    }

    void __stdcall SaveMPPlayerCustomData(BYTE *dataD2Savefile) {
        log_msg("SaveMPPlayerCustomData - Start.\n");
        auto ptChar = D2GetClientPlayer();
        backupSaveFiles(getPlayerData(ptChar)->name, (dataD2Savefile[0x24] & 4) == 4);
        if (active_PlayerCustomData) {
            if (receivedSaveFiles.sizeExtended && (receivedSaveFiles.sizeExtended == receivedSaveFiles.curExtended) && (!active_sharedStash || receivedSaveFiles.sizeShared && (receivedSaveFiles.sizeShared == receivedSaveFiles.curShared))) {
                log_msg("SaveMPPlayerCustomData - Saving can start\n");
                if (ptChar) {
                    writeExtendedSaveFile(getPlayerData(ptChar)->name, receivedSaveFiles.dataExtended, receivedSaveFiles.sizeExtended);
                    D2FogMemDeAlloc(receivedSaveFiles.dataExtended, __FILE__, __LINE__, 0);
                    if (active_sharedStash) {
                        writeSharedSaveFile(getPlayerData(ptChar)->name, receivedSaveFiles.dataShared, receivedSaveFiles.sizeShared, (dataD2Savefile[0x24] & 4) == 4);//6FBAB9A4-6FAA0000
                        D2FogMemDeAlloc(receivedSaveFiles.dataShared, __FILE__, __LINE__, 0);
                    }
                    ZeroMemory(&receivedSaveFiles, sizeof(receivedSaveFiles));
                }
                log_msg("SaveMPPlayerCustomData - Receive Savefiles completed\n\n");
            } else {
                if (ptChar) {
                    log_box("Connection error : save with client data.");
                    SaveSPPlayerCustomData(ptChar);
                }
            }
        }
        log_msg("--- SaveMPPlayerCustomData End ---\n\n");
    }

/*
FCT_ASM ( caller_SendSaveFilesToSave_9 )
	POP ECX
	PUSH EAX
	PUSH ECX
	PUSH EBX
	CALL SendSaveFilesToSave
	MOV EDX,EBX
	MOV ECX,EDI
	RETN
}}

FCT_ASM ( caller_SendSaveFilesToSave )
	POP ECX
	PUSH EAX
	PUSH ECX
	PUSH EBX
	CALL SendSaveFilesToSave
	MOV EDX,EBX
	MOV ECX,EBP
	RETN
}}
*/

    __declspec(naked)void caller_ManageNextPacketToSend() {
        __asm{
        PUSH ESI
        CALL ManageNextPacketToSend
        TEST EAX, EAX
        JNZ end_caller_ManageNextPacketToSend
        XOR ECX, ECX
        RETN
        end_caller_ManageNextPacketToSend:
        MOV ECX, DWORD PTR DS:[ESI+0x17C]
        RETN
        }
    }

    __declspec(naked)void caller_ManageNextPacketToSend_9() {
        __asm{
        PUSH ESI
        CALL ManageNextPacketToSend
        TEST EAX, EAX
        JNZ end_caller_ManageNextPacketToSend_9
        MOV DWORD PTR DS:[ESI+0x14C], 5
        POP ESI
        POP ESI
        RETN
        end_caller_ManageNextPacketToSend_9:
        MOV EAX, DWORD PTR DS:[ESI+0x150]
        RETN
        }
    }

    __declspec(naked)void caller_ManageNextPacketToSend_9d() {
        __asm{
        PUSH ESI
        CALL ManageNextPacketToSend
        TEST EAX, EAX
        JNZ end_caller_ManageNextPacketToSend_9d
        MOV DWORD PTR DS:[ESI+0x170], 5
        POP ESI
        POP ESI
        RETN
        end_caller_ManageNextPacketToSend_9d:
        MOV EAX, DWORD PTR DS:[ESI+0x174]
        RETN
        }
    }

    __declspec(naked)void caller_SendSaveFilesToSave_114() {
        __asm{
        PUSH DWORD PTR SS:[ESP+0x4]
        CALL SendSaveFilesToSave
        MOV EDI, DWORD PTR SS:[EBP-0x2008]
        RETN
        }
    }

    __declspec(naked)void caller_SendSaveFilesToSave_111() {
        __asm{
        PUSH DWORD PTR SS:[ESP+0x2014]
        CALL SendSaveFilesToSave
        MOV EAX, DWORD PTR SS:[ESP+0x8]
        TEST EAX, EAX
        RETN
        }
    }

    __declspec(naked)void caller_SendSaveFilesToSave() {
        __asm{
        PUSH EDI
        CALL SendSaveFilesToSave
        MOV EDI, DWORD PTR SS:[ESP+0x14]
        TEST EDI, EDI
        RETN
        }
    }

    __declspec(naked)void caller_SendSaveFilesToSave_9() {
        __asm{
        PUSH ESI
        CALL SendSaveFilesToSave
        MOV ESI, DWORD PTR SS:[ESP+0x14]
        TEST ESI, ESI
        RETN
        }
    }

    __declspec(naked)void caller_ReceivedSaveFilesToSave_114() {
        __asm{
        CMP EDX, -1
        JE continue_rcvFct
        PUSH ECX
        PUSH EDX
        PUSH ECX
        CALL ReceiveSaveFilesToSave
        POP EDX
        POP ECX
        TEST EAX, EAX
        JE continue_rcvFct
        RETN
        continue_rcvFct:
        JMP D2ReceivePacket
        }
    }

    __declspec(naked)void caller_ReceivedSaveFilesToSave_111() {
        __asm{
        LEA EAX, DWORD PTR SS:[ESP+0x10]
        PUSH EAX
        CALL ReceiveSaveFilesToSave
        TEST EAX, EAX
        JE continue_rcvFct
        ADD DWORD PTR SS:[ESP], 0x3D
        RETN
        continue_rcvFct:
        MOVZX EAX, BYTE PTR SS:[ESP+0x10]
        RETN
        }
    }

    __declspec(naked)void caller_ReceivedSaveFilesToSave() {
        __asm{
        CMP EDX, -1
        JE continue_rcvFct
        PUSH ECX
        PUSH EDX
        PUSH ECX
        CALL ReceiveSaveFilesToSave
        POP EDX
        POP ECX
        TEST EAX, EAX
        JE continue_rcvFct
        XOR EAX, EAX
        ADD ESP, 4
        RETN
        continue_rcvFct:
        POP EAX
        SUB ESP, 0x5F4//5F4
        JMP EAX
//	JMP DWORD PTR SS:[ESP+0x5F0]
        }
    }

    __declspec(naked)void caller_SaveMPPlayerCustomData_111() {
        __asm{
        PUSH EAX
        PUSH ECX
        PUSH EAX
        CALL SaveMPPlayerCustomData
        POP ECX
        POP EAX
        CMP ECX, 0xAA55AA55
        RETN
        }
    }

    __declspec(naked)void caller_SaveMPPlayerCustomData() {
        __asm{
        PUSH ECX
        PUSH ECX
        CALL SaveMPPlayerCustomData
        POP ECX
        MOV EAX, DWORD PTR DS:[ECX]
        MOV EDX, DWORD PTR DS:[ECX+4]
        RETN
        }
    }

    void Install_SavePlayerData() {
        static int isInstalled = false;
        if (isInstalled) return;
        log_msg("Patch D2Game & D2Client for save Player's custom data. (SavePlayerData)\n");

        //Save single player custom data.
        if (version_D2Game >= V111) {
            mem_seek(offset_D2Game + (version_D2Game == V114d ? 0x1324EC : (version_D2Game == V113d ? 0x39CD1 : (version_D2Game == V113c ? 0x543D1 : (version_D2Game == V112 ? 0x44601 : (version_D2Game == V111b ? 0x25911 : (version_D2Game == V111 ? 0xB9801 : (version_D2Game == V110 ? 0x0000 : (version_D2Game == V109d ? 0x0000 : 0x0000)))))))));
            MEMC_REF4(D2SaveSPChar, version_D2Game >= V114a ? caller_SaveSPPlayerCustomData_114 : version_D2Game >= V111 ? caller_SaveSPPlayerCustomData_111 : version_D2Game != V109b ? caller_SaveSPPlayerCustomData : caller_SaveSPPlayerCustomData_9);
            //6FCD9800  |. E8 3BFBFFFF    CALL D2Game.6FCD9340
            //6FC45910  |. E8 3BFBFFFF    CALL D2Game.6FC45450
            //6FC64600  |. E8 3BFBFFFF    CALL D2Game.6FC64140
            //6FC743D0  |. E8 3BFBFFFF    CALL D2Game.6FC73F10
            //6FC59CD0  |. E8 3BFBFFFF    CALL D2Game.6FC59810
            //005324EB  |. E8 50FDFFFF    CALL Game.00532240                       ; \Game.00532240
        } else {
            mem_seek(offset_D2Game + (version_D2Game == V114d ? 0x132276 : (version_D2Game == V113d ? 0x39835 : (version_D2Game == V113c ? 0x53F35 : (version_D2Game == V112 ? 0x44165 : (version_D2Game == V111b ? 0x25475 : (version_D2Game == V111 ? 0xB9365 : (version_D2Game == V110 ? 0x5A624 : (version_D2Game == V109d ? 0x4E304 : 0x4DF04)))))))));
            MEMJ_REF4(D2FogGetSavePath, /*version_D2Game >= V114a ? caller_SaveSPPlayerCustomData : version_D2Game >= V111 ? caller_SaveSPPlayerCustomData_111 :*/ version_D2Game != V109b ? caller_SaveSPPlayerCustomData : caller_SaveSPPlayerCustomData_9);
            //6FC8A623   E8 3E210900      CALL <JMP.&Fog.#10115>
            //02039364  |. E8 3B0FF5FF    CALL <JMP.&Fog.#10115>
            //01F95474  |. E8 C34EFEFF    CALL <JMP.&Fog.#10115>
            //6FC64164  |. E8 EB61FCFF    CALL <JMP.&Fog.#10115>
            //6FC73F34  |. E8 DD63FBFF    CALL <JMP.&Fog.#10115>
            //6FC59834  |. E8 FB0AFDFF    CALL <JMP.&Fog.#10115>
            //00532275  |. E8 D64DEDFF    CALL Game.00407050
        }
        if (active_PlayerCustomData) {
            //Send SaveFiles
            mem_seek(offset_D2Game + (version_D2Game == V114d ? 0x132398 : (version_D2Game == V113d ? 0x397AB : (version_D2Game == V113c ? 0x53EAB : (version_D2Game == V112 ? 0x440DB : (version_D2Game == V111b ? 0x253EB : (version_D2Game == V111 ? 0xB92DB : (version_D2Game == V110 ? 0x5A720 : (version_D2Game == V109d ? 0x4E3FA : 0x4DFFA)))))))));
            memt_byte(0x8B, 0x90); // NOP
            memt_byte(version_D2Game >= V114d ? 0xBD : version_D2Game >= V111 ? 0x44 : version_D2Game != V109b ? 0x7C : 0x74, 0xE8); // CALL
            MEMT_REF4(version_D2Game >= V114d ? 0xFFFFDFF8 : version_D2Game >= V111 ? 0xC0850424 : version_D2Game != V109b ? 0xFF851024 : 0xF6851024, version_D2Game >= V114d ? caller_SendSaveFilesToSave_114 : version_D2Game >= V111 ? caller_SendSaveFilesToSave_111 : version_D2Game != V109b ? caller_SendSaveFilesToSave : caller_SendSaveFilesToSave_9);
            //6FC8A720  |. 8B7C24 10      MOV EDI,DWORD PTR SS:[ESP+10]
            //6FC8A724  |. 85FF           TEST EDI,EDI
            //020392DB  |. 8B4424 04      MOV EAX,DWORD PTR SS:[ESP+4]
            //020392DF  |. 85C0           TEST EAX,EAX
            //01F953EB  |. 8B4424 04      MOV EAX,DWORD PTR SS:[ESP+4]
            //01F953EF  |. 85C0           TEST EAX,EAX
            //6FC640DB  |. 8B4424 04      MOV EAX,DWORD PTR SS:[ESP+4]
            //6FC640DF  |. 85C0           TEST EAX,EAX
            //6FC73EAB  |. 8B4424 04      MOV EAX,DWORD PTR SS:[ESP+4]
            //6FC73EAF  |. 85C0           TEST EAX,EAX
            //6FC597AB  |. 8B4424 04      MOV EAX,DWORD PTR SS:[ESP+4]
            //6FC597AF  |. 85C0           TEST EAX,EAX
            //00532398  |. 8BBD F8DFFFFF  MOV EDI,DWORD PTR SS:[EBP-2008]

            mem_seek(offset_D2Game + (version_D2Game == V114d ? 0x12E13E : (version_D2Game == V113d ? 0xBEDD3 : (version_D2Game == V113c ? 0x2D173 : (version_D2Game == V112 ? 0xA89D3 : (version_D2Game == V111b ? 0xE6D83 : (version_D2Game == V111 ? 0xE2943 : (version_D2Game == V110 ? 0x7BBB : (version_D2Game == V109d ? 0x7A13 : 0x7993)))))))));
            memt_byte(0x8B, 0x90); // NOP
            memt_byte(version_D2Game >= V110 ? 0x8E : 0x86, 0xE8); // CALL
            MEMT_REF4(version_D2Game >= V110 ? 0x0000017C : version_D2Game == V109d ? 0x0000174 : 0x00000150, version_D2Game >= V110 ? caller_ManageNextPacketToSend : version_D2Game == V109d ? caller_ManageNextPacketToSend_9d : caller_ManageNextPacketToSend_9);
            //6FC37BBB  |> 8B8E 7C010000  MOV ECX,DWORD PTR DS:[ESI+17C]
            //02062943  |. 8B8E 7C010000  MOV ECX,DWORD PTR DS:[ESI+17C]
            //02056D83  |. 8B8E 7C010000  MOV ECX,DWORD PTR DS:[ESI+17C]
            //6FCC89D3  |. 8B8E 7C010000  MOV ECX,DWORD PTR DS:[ESI+17C]
            //6FC4D173  |. 8B8E 7C010000  MOV ECX,DWORD PTR DS:[ESI+17C]
            //6FCDEDD3  |. 8B8E 7C010000  MOV ECX,DWORD PTR DS:[ESI+17C]
            //0052E13E  |> 8B8E 7C010000  MOV ECX,DWORD PTR DS:[ESI+17C]

            //	mem_seek R7(D2Game, 7B82, 7C05, 7DCF, 0000, 0000, 0000, 0000);//117FC,	117EC,	11DBC
            //	memt_byte( 0x8B ,0xE8); // CALL
            //	MEMT_REF4( (version_D2Game == V110) ?  0x50CD8BD3 : 0x50CF8BD3, (version_D2Game == V110) ? caller_SendSaveFilesToSave : caller_SendSaveFilesToSave_9);
            //6FC37DCF  |. 8BD3           |MOV EDX,EBX
            //6FC37DD1  |. 8BCD           |MOV ECX,EBP
            //6FC37DD3  |. 50             |PUSH EAX

            //Received SaveFiles
            mem_seek(offset_D2Client + (version_D2Client == V114d ? 0x4C70D : (version_D2Client == V113d ? 0x448E6 : (version_D2Client == V113c ? 0x43946 : (version_D2Client == V112 ? 0x7BCD6 : (version_D2Client == V111b ? 0x32076 : (version_D2Client == V111 ? 0x89246 : (version_D2Client == V110 ? 0x11CB0 : (version_D2Client == V109d ? 0x116E0 : 0x116F0)))))))));
            if (version_D2Game >= V114a) {
                MEMC_REF4(D2ReceivePacket, caller_ReceivedSaveFilesToSave_114);
                //0044C70C  |. E8 3F010100    |CALL Game.0045C850
            } else if (version_D2Game >= V111) {
                memt_byte(0x0F, 0xE8);
                MEMT_REF4(0x0C2444B6, caller_ReceivedSaveFilesToSave_111);
                //6FB39246  |. 0FB64424 0C    |MOVZX EAX,BYTE PTR SS:[ESP+C]
                //6FAE2076  |. 0FB64424 0C    |MOVZX EAX,BYTE PTR SS:[ESP+C]
                //6FB2BCD6  |. 0FB64424 0C    |MOVZX EAX,BYTE PTR SS:[ESP+C]
                //6FAF3946  |. 0FB64424 0C    |MOVZX EAX,BYTE PTR SS:[ESP+C]
                //6FAF48E6  |. 0FB64424 0C    |MOVZX EAX,BYTE PTR SS:[ESP+C]
            } else {
                //Received SaveFiles
                memt_byte(0x81, 0x90); // NOP
                memt_byte(0xEC, 0xE8); // CALL
                MEMT_REF4(0x000005F4, caller_ReceivedSaveFilesToSave);
                //6FAB1CB0  |$ 81EC F4050000  SUB ESP,5F4
            }
        }
        if (version_D2Game >= V111) {
            // Save multiplayer player custom data.
            mem_seek(offset_D2Client + (version_D2Client == V114d ? 0x5C565 : (version_D2Client == V113d ? 0x829C2 : (version_D2Client == V113c ? 0xAC572 : (version_D2Client == V112 ? 0x64A22 : (version_D2Client == V111b ? 0xBD7F2 : (version_D2Client == V111 ? 0x99AE2 : (version_D2Client == V110 ? 0x11DBC : (version_D2Client == V109d ? 0x117EC : 0x117FC)))))))));
            memt_byte(0x81, 0xE8); // CALL
            MEMT_REF4(0x55AA55F9, caller_SaveMPPlayerCustomData_111);
            memt_byte(0xAA, 0x90); // CALL
            //6FB49AE2  |. 81F9 55AA55AA  CMP ECX,AA55AA55
            //6FB6D7F2  |. 81F9 55AA55AA  CMP ECX,AA55AA55
            //6FB14A22  |. 81F9 55AA55AA  CMP ECX,AA55AA55
            //6FB5C572  |. 81F9 55AA55AA  CMP ECX,AA55AA55
            //6FB329C2  |. 81F9 55AA55AA  CMP ECX,AA55AA55
            //0045C565  |. 81F9 55AA55AA  CMP ECX,AA55AA55
        } else {
            // Save multiplayer player custom data.
            mem_seek(offset_D2Client + (version_D2Client == V114d ? 0x0000 : (version_D2Client == V113d ? 0x0000 : (version_D2Client == V113c ? 0x0000 : (version_D2Client == V112 ? 0x64A22 : (version_D2Client == V111b ? 0xBD7F2 : (version_D2Client == V111 ? 0x99AE2 : (version_D2Client == V110 ? 0x11DBC : (version_D2Client == V109d ? 0x117EC : 0x117FC)))))))));
            memt_byte(0x8B, 0xE8); // CALL
            MEMT_REF4(0x04518B01, caller_SaveMPPlayerCustomData);
            //6FAB1DBC  |. 8B01           MOV EAX,DWORD PTR DS:[ECX]
            //6FAB1DBE  |. 8B51 04        MOV EDX,DWORD PTR DS:[ECX+4]
        }
        if (version_D2Game == V109b || version_D2Game == V109d)
            customPackID = 0xAB;
        else if (version_D2Game >= V111)
            customPackID++;
        log_msg("\n");
        isInstalled = true;
    }
}
