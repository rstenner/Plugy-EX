/*=================================================================
	File created by Yohann NICOLAS.
	Add support 1.13d by L'Autour.
	Add support 1.14d by haxifix.

	Updating client.

=================================================================*/

#include "updateClient.h"
#include "infinityStash.h"
#include "commands.h"
#include "common.h"
#include "d2functions.h"
#include "error.h"
#include "modifMemory.h"
#include <updatingConst.h>

namespace PlugY {
    void updateClient(Commons::Unit *ptChar, DWORD mFunc, DWORD p1, DWORD p2, DWORD p3) {
        void *ptNetClient;
        DataPacket packet;

        // Intialize the packet to send to client
        ZeroMemory(&packet, sizeof(DataPacket));
        packet.mType = 0x9D;
        packet.mFunc = (BYTE) mFunc;
        packet.mSize = sizeof(DataPacket);
        packet.mPlayerID = ptChar->nUnitId;
        packet.mParam1 = p1;
        packet.mParam2 = p2;
        packet.mParam3 = p3;
        ptNetClient = D2GetClient(ptChar, (char *)__FILE__, __LINE__);

        // Send packet to client for remove placed skills
        D2SendPacket(ptNetClient, &packet, sizeof(DataPacket));
    }

    void updateClient(Commons::Unit *ptChar, DWORD mFunc, char *msg) {
        void *ptNetClient;
        DataPacket packet;

        // Intialize the packet to send to client
        ZeroMemory(&packet, sizeof(DataPacket));
        packet.mType = 0x9D;
        packet.mFunc = (BYTE) mFunc;
        packet.mSize = sizeof(DataPacket);
        packet.mPlayerID = ptChar->nUnitId;
        if (msg != NULL && strlen(msg) > 20)
            return;
        if (msg != NULL)
            strncpy((char *) &packet.mItemID, msg, 20);
        ptNetClient = D2GetClient(ptChar, (char *)__FILE__, __LINE__);

        // Send packet to client for remove placed skills
        D2SendPacket(ptNetClient, &packet, sizeof(DataPacket));
    }

    DWORD __fastcall handleClientUpdate(DataPacket *packet) {
        log_msg("[CLIENT] Received custom message: %d with param: %08X , %08X , %08X\n", packet->mFunc, packet->mParam1, packet->mParam2, packet->mParam3);
        switch (packet->mFunc) {
            case UC_SELECT_STASH:
                setSelectedStashClient(packet->mParam1, packet->mParam2, packet->mParam3, (packet->mParam2 & 8) == 8);
                return 1;
            case UC_SHARED_GOLD :
                updateSharedGold(packet->mParam1);
                return 1;
            case UC_PAGE_NAME: {
                char pageName[21];
                strncpy(pageName, (char *) &packet->mItemID, 20);
                pageName[20] = NULL;
                renameCurrentStash(D2GetClientPlayer(), pageName);
                return 1;
            }
            default :
                return 0;
        }
    }

    __declspec(naked)void caller_handleClientUpdate_114() {
        __asm{
        LEA ECX, DWORD PTR SS:[ESP]
        CALL handleClientUpdate
        MOV ESP, EBP
        POP EBP
        RETN
        }
    }

    __declspec(naked)void caller_handleClientUpdate_111() {
        __asm{
        LEA ECX, DWORD PTR SS:[ESP+8]
        CALL handleClientUpdate
        POP EDI
        POP ESI
        MOV ESP, EBP
        POP EBP
        RETN
        }
    }

    __declspec(naked)void caller_handleClientUpdate() {
        __asm{
        LEA ECX, DWORD PTR SS:[ESP]
        CALL handleClientUpdate
        ADD ESP, 0x104
        RETN
        }
    }

    void Install_UpdateClient() {
        static int isInstalled = false;
        if (isInstalled) return;
        log_msg("Patch D2Client for received Item packet. (UpdateClient)\n");

        // execute if it's our packet else continue
        mem_seek(offset_D2Client + (version_D2Client == V114d ? 0x5EC99 : (version_D2Client == V113d ? 0x84D96 : (version_D2Client == V113c ? 0xAE896 : (version_D2Client == V112 ? 0x66E06 : (version_D2Client == V111b ? 0xBFE86 : (version_D2Client == V111 ? 0x9C6B6 : (version_D2Client == V110 ? 0x145B6 : (version_D2Client == V109d ? 0x14226 : 0x14236)))))))));
        MEMT_REF4(version_D2Client >= V114d ? 0x000000CE : version_D2Client >= V111 ? 0x000000CF : 0x000000D6, version_D2Client >= V114d ? caller_handleClientUpdate_114 : version_D2Client >= V111 ? caller_handleClientUpdate_111 : caller_handleClientUpdate);
        //6FAB45B4  |. 0F87 D6000000  JA D2Client.6FAB4690
        //6FB4C6B4  |. 0F87 CF000000  JA D2Client.6FB4C789
        //6FB6FE84  |. 0F87 CF000000  JA D2Client.6FB6FF59
        //6FB16E04  |. 0F87 CF000000  JA D2Client.6FB16ED9
        //6FB5E894  |. 0F87 CF000000  JA D2Client.6FB5E969
        //6FB34D94  |. 0F87 CF000000  JA D2Client.6FB34E69
        //0045EC97  |. 0F87 CE000000  JA Game.0045ED6B

        log_msg("\n");
        isInstalled = true;
    }

}