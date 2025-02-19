/*=================================================================
	File created by Yohann NICOLAS.
	Add support 1.13d by L'Autour.
	Add support 1.14d and DoNotCloseNihlathakPortal by haxifix.

	More little options.

=================================================================*/
#include "extraOptions.h"
#include <stdio.h>
#include "common.h"
#include "infinityStash.h"
#include "d2functions.h"
#include "globalVariable.h"
#include "LocalizedStrings.h"
#include "parameters.h"
#include "error.h"
#include "modifMemory.h"
#include "statsPoints.h"
#include <d2constants.h>

namespace PlugY {
    bool active_DisplayItemLevel = false;
    DWORD nbPlayersCommandByDefault = 0;
    DWORD nbPlayersCommand = 0;
    bool active_alwaysRegenMapInSP = false;
    bool active_RunLODs = false;
    int active_AlwaysDisplayLifeMana = 0;
    bool active_EnabledTXTFilesWithMSExcel = false;
    bool active_DisplayBaseStatsValue = false;
    bool active_LadderRunewords = false;
    bool active_EnabledCowPortalWhenCowKingWasKill = false;
    bool active_DoNotCloseNihlathakPortal = false;
    bool active_MoveCainNearHarrogathWaypoint = false;
    bool active_RemoveExperienceDiminushingReturn = false;

/****************************************************************************************************/

    void __stdcall displayItemlevel(LPWSTR popup, Commons::Unit *ptItem) {
        if (onRealm && (selectModParam == MOD_NO)) return;
        WCHAR text[0x50];
        _snwprintf(text, 50, L"%s: %u\n", getLocalString(STR_ITEM_LEVEL), D2GetItemLevel(ptItem));
        D2SetColorPopup(text, WHITE);
        wcscat(popup, text);
    }

    __declspec(naked)void caller_displayItemlevel_114() {
        __asm{
        PUSH ECX
        PUSH EAX
        PUSH ECX
        LEA EAX, DWORD PTR SS:[EBP-0x2434]
        PUSH EAX
        CALL displayItemlevel
        POP EAX
        POP ECX
        POP EDX
        PUSH 0x100
        JMP EDX
        }
    }

    __declspec(naked)void caller_displayItemlevel_113() {
        __asm{
        PUSH ECX
        PUSH EAX
        PUSH EAX
        LEA EAX, DWORD PTR SS:[ESP+0x1E70]
        PUSH EAX
        CALL displayItemlevel
        POP EAX
        POP ECX
        POP EDX
        PUSH 0x100
        JMP EDX
        }
    }

    __declspec(naked)void caller_displayItemlevel_111() {
        __asm{
        PUSH ECX
        PUSH EDX
        PUSH ECX
        LEA EAX, DWORD PTR SS:[ESP+0x1E74]
        PUSH EAX
        CALL displayItemlevel
        POP EDX
        POP ECX
        POP EAX
        PUSH 0x100
        JMP EAX
        }
    }

    __declspec(naked)void caller_displayItemlevelSet_114() {
        __asm{
        PUSH ECX
        PUSH EDX
        PUSH ECX
        LEA EAX, DWORD PTR SS:[EBP-0x2138]
        PUSH EAX
        CALL displayItemlevel
        POP EDX
        POP ECX
        POP EAX
        PUSH 0x100
        JMP EAX
        }
    }

    __declspec(naked)void caller_displayItemlevelSet_111() {
        __asm{
        PUSH ECX
        PUSH EDX
        PUSH ECX
        LEA EAX, DWORD PTR SS:[ESP+0x1958]
        PUSH EAX
        CALL displayItemlevel
        POP EDX
        POP ECX
        POP EAX
        PUSH 0x100
        JMP EAX
        }
    }

    __declspec(naked)void caller_displayItemlevel() {
        __asm{
        PUSH ECX
        PUSH EDX
        PUSH ECX
        LEA EAX, DWORD PTR SS:[ESP+0x5018]
        PUSH EAX
        CALL displayItemlevel
        POP EDX
        POP ECX
        POP EAX
        PUSH 0x100
        JMP EAX
        }
    }

    __declspec(naked)void caller_displayItemlevelSet() {
        __asm{
        PUSH ECX
        PUSH EDX
        PUSH ECX
        LEA EAX, DWORD PTR SS:[ESP+0x1220]
        PUSH EAX
        CALL displayItemlevel
        POP EDX
        POP ECX
        POP EAX
        PUSH 0x100
        JMP EAX
        }
    }

    __declspec(naked)void caller_displayItemlevel_9() {
        __asm{
        PUSH ECX
        PUSH EDX
        PUSH ECX
        LEA EAX, DWORD PTR SS:[ESP+0x501C]
        PUSH EAX
        CALL displayItemlevel
        POP EDX
        POP ECX
        POP EAX
        PUSH 0x100
        JMP EAX
        }
    }

    __declspec(naked)void caller_displayItemlevelSet_9() {
        __asm{
        PUSH ECX
        PUSH EDX
        PUSH ECX
        LEA EAX, DWORD PTR SS:[ESP+0x1224]
        PUSH EAX
        CALL displayItemlevel
        POP EDX
        POP ECX
        POP EAX
        PUSH 0x100
        JMP EAX
        }
    }

    void Install_DisplayItemLevel() {
        static int isInstalled = false;
        if (isInstalled) return;
        log_msg("Patch D2Client to display item popup. (DisplayPopup)\n");

        // print the text in the final buffer
        mem_seek(offset_D2Client + (version_D2Client == V114d ? 0x8E983 : (version_D2Client == V113d ? 0x98590 : (version_D2Client == V113c ? 0x941C0 : (version_D2Client == V112 ? 0xAE0AA : (version_D2Client == V111b ? 0x789DA : (version_D2Client == V111 ? 0xADD0A : (version_D2Client == V110 ? 0x438A1 : (version_D2Client == V109d ? 0x3D47C : 0x3D47C)))))))));
        memt_byte(0x68, 0xE8);
        MEMT_REF4(0x100, version_D2Client >= V114d ? caller_displayItemlevel_114 : version_D2Client >= V113c ? caller_displayItemlevel_113 : version_D2Client >= V111 ? caller_displayItemlevel_111 : version_D2Client == V110 ? caller_displayItemlevel : caller_displayItemlevel_9);
        //6FAE38A1   . 68 00010000        PUSH 100
        //6FB5DD0A  |. 68 00010000        PUSH 100
        //6FB289DA  |. 68 00010000        PUSH 100
        //6FB5E0AA  |. 68 00010000    PUSH 100
        //6FB441C0  |. 68 00010000    PUSH 100
        //6FB48590  |. 68 00010000    PUSH 100
        //0048E983  |. 68 00010000    PUSH 100                                 ; /Arg1 = 00000100

        // print the text in the final buffer (for set items)
        mem_seek(offset_D2Client + (version_D2Client == V114d ? 0x8D774 : (version_D2Client == V113d ? 0x973B3 : (version_D2Client == V113c ? 0x92FE3 : (version_D2Client == V112 ? 0xACEB3 : (version_D2Client == V111b ? 0x77773 : (version_D2Client == V111 ? 0xAC773 : (version_D2Client == V110 ? 0x427BE : (version_D2Client == V109d ? 0x3C452 : 0x3C452)))))))));
        memt_byte(0x68, 0xE8);
        MEMT_REF4(0x100, version_D2Client >= V114d ? caller_displayItemlevelSet_114 : version_D2Client >= V111 ? caller_displayItemlevelSet_111 : version_D2Client == V110 ? caller_displayItemlevelSet : caller_displayItemlevelSet_9);
        //6FAE27BE   . 68 00010000        PUSH 100
        //6FB5C773  |. 68 00010000        PUSH 100
        //6FB27773  |. 68 00010000        PUSH 100
        //6FB5CEB3  |. 68 00010000    PUSH 100
        //6FB42FE3  |. 68 00010000    PUSH 100
        //6FB473B3  |. 68 00010000    PUSH 100
        //0048D774  |. 68 00010000    PUSH 100                                 ; /Arg1 = 00000100

        log_msg("\n");
        isInstalled = true;
    }

/****************************************************************************************************/

    auto *msgNBPlayersString = "players %u";
    DWORD *infoEnabledSendPlayersCommand;

    void SendPlayersCommand() {
        if (!needToInit || onRealm) return;
        needToInit = false;
#pragma pack(1)
        struct {
            BYTE displayType;//0x15 main msg;  0x14: char popup
            BYTE un;
            BYTE zero;
            char string[0xFF];
            char null;
            char targetName[0x10];
            char null2;
        } data;
#pragma pack()
        DWORD info = *infoEnabledSendPlayersCommand;
        if ((info != 0) && (info != 1) && (info != 6) && (info != 8)) return;
        if (!active_multiPageStash)
            nbPlayersCommand = nbPlayersCommandByDefault;

        //if (nbPlayersCommand < 2)
        //	return;

        log_msg("SendPlayersCommand %u\n", nbPlayersCommand);
        D2SetNbPlayers(nbPlayersCommand);
        memset(&data, 0, sizeof(data));
        data.displayType = 0x15;
        data.un = 1;
        data.zero = 0;//*(BYTE*)(offset_D2Client+0x112CFC); in 1.10
        sprintf(data.string, msgNBPlayersString, nbPlayersCommand);
        data.null = NULL;
        D2SendMsgToAll((BYTE *) &data);
    }

//FCT_ASM ( caller_SendPlayersCommand_old )
//	CALL SendPlayersCommand
//	JMP D2GetResolution
//}}

    __declspec(naked)void caller_SendPlayersCommand() {
        __asm{
        PUSH ECX
        CALL SendPlayersCommand
        POP ECX
        JMP D2PrintInterface
        }
    }

    void Install_SendPlayersCommand() {
        static int isInstalled = false;
        if (isInstalled) return;
        log_msg("Patch D2Client for init default nb /players. (SendPlayersCommand)\n");
        infoEnabledSendPlayersCommand = (DWORD *) (offset_D2Client + (version_D2Client == V114d ? 0x3A0610 : (version_D2Client == V113d ? 0x11D1DC : (version_D2Client == V113c ? 0x11C394 : (version_D2Client == V112 ? 0x11BFF8 : (version_D2Client == V111b ? 0x11C2AC : (version_D2Client == V111 ? 0x11BFBC : (version_D2Client == V110 ? 0x107960 : (version_D2Client == V109d ? 0x110BC0 : 0x111D60)))))))));
        if (version_D2Client >= V110)
            msgNBPlayersString = (char *) (offset_D2Client + (version_D2Client == V114d ? 0x2D7374 : (version_D2Client == V113d ? 0xD470C : (version_D2Client == V113c ? 0xD4E00 : (version_D2Client == V112 ? 0xD4680 : (version_D2Client == V111b ? 0xD4748 : (version_D2Client == V111 ? 0xD06A8 : (version_D2Client == V110 ? 0xD8448 : (version_D2Client == V109d ? 0x0000 : 0x0000)))))))));

        // Set nb Player to default
//	mem_seek R8(D2Client,	8723B, 865BB, 81B8B, A3602, 66A02, 90162, C39F2, 1D3F2, 56EF9);//6FB1C7B7-6FAA0000
//	MEMJ_REF4( D2GetResolution, caller_SendPlayersCommand_old);
        //6FB2723A  |. E8 7D8E0300    CALL <JMP.&D2gfx.#10005>
        //6FB265BA  |. E8 7D8E0300    CALL <JMP.&D2gfx.#10005>
        //6FB21B8A   . E8 4F9C0400    CALL <JMP.&D2gfx.#10005>
        //6FB53601  |. E8 E099F6FF    CALL <JMP.&D2gfx.#10000>
        //6FB16A01  |. E8 AA65FAFF    CALL <JMP.&D2gfx.#10063>
        //6FB40161  |. E8 1ECFF7FF    CALL <JMP.&D2gfx.#10043>
        //6FB739F1  |. E8 B896F4FF    CALL <JMP.&D2gfx.#10031>
        //6FACD3F1  |. E8 EAFBFEFF    CALL <JMP.&D2gfx.#10012>
        //00456EF8  |. E8 63E20900    CALL Game.004F5160

        // Set nb Player to default
        mem_seek(offset_D2Client + (version_D2Client == V114d ? 0x4CB04 : (version_D2Client == V113d ? 0x453CC : (version_D2Client == V113c ? 0x4437C : (version_D2Client == V112 ? 0x7CB5C : (version_D2Client == V111b ? 0x32C8C : (version_D2Client == V111 ? 0x88E4C : (version_D2Client == V110 ? 0x97DA : (version_D2Client == V109d ? 0x90AA : 0x90BA)))))))));//6FB1C7B7-6FAA0000
        MEMC_REF4(D2PrintInterface, caller_SendPlayersCommand);
        //6FAA90B9   . E8 62E10700    CALL D2Client.6FB27220
        //6FAA90A9   . E8 F2D40700    CALL D2Client.6FB265A0
        //6FAA97D9   . E8 92830700    CALL D2Client.6FB21B70
        //6FB38E4B   . E8 A0A70100    CALL D2Client.6FB535F0
        //6FAE2C8B   . E8 603D0300    CALL D2Client.6FB169F0
        //6FB2CB5B   . E8 F0350100    CALL D2Client.6FB40150
        //6FAF437B   . E8 60F60700    CALL D2Client.6FB739E0
        //6FAF53CB   . E8 1080FDFF    CALL D2Client.6FACD3E0
        //0044CB03  |. E8 D8A30000    CALL Game.00456EE0

        log_msg("\n");
        isInstalled = true;
    }

/****************************************************************************************************/


    void Install_AlwaysRegenMapInSP() {
        static int isInstalled = false;
        if (isInstalled) return;
        log_msg("Patch D2Game for always regenerate maps in Single player. (AlwaysRegenMapInSP)\n");

        // Always regenerate maps in Single player
        mem_seek(offset_D2Game + (version_D2Game == V114d ? 0x16A20F : (version_D2Game == V113d ? 0x26D7E : (version_D2Game == V113c ? 0xED3BE : (version_D2Game == V112 ? 0x1E1FE : (version_D2Game == V111b ? 0x2966D : (version_D2Game == V111 ? 0x6235D : (version_D2Game == V110 ? 0x5DE88 : (version_D2Game == V109d ? 0x51BFA : 0x517ED)))))))));
        memt_byte(0x74, 0xEB);        // JMP SHORT D2Game.01FE2368
        //6FC8DE88  |. 74 09          JE SHORT D2Game.6FC8DE93
        //01FE235D  |. 74 09          JE SHORT D2Game.01FE2368
        //01F9966D  |. 74 09          JE SHORT D2Game.01F99678
        //6FC3E1FE  |. 74 09          JE SHORT D2Game.6FC3E209
        //6FD0D3BE  |. 74 09          JE SHORT D2Game.6FD0D3C9
        //6FC46D7E  |. 74 09          JE SHORT D2Game.6FC46D89
        //0056A20F  |. 74 09          JE SHORT Game.0056A21A

        log_msg("\n");
        isInstalled = true;
    }

/****************************************************************************************************/

    void Install_RunLODs() {
        static int isInstalled = false;
        if (isInstalled) return;
        log_msg("Patch D2gfx for launch any number of Diablo II game in the same computer. (RunLODs)\n");

        // execute if it's our packet else continue
        mem_seek(offset_D2gfx + (version_D2gfx == V114d ? 0xF562B : (version_D2gfx == V113d ? 0xB6B0 : (version_D2gfx == V113c ? 0x85BF : (version_D2gfx == V112 ? 0x894F : (version_D2gfx == V111b ? 0x84AF : (version_D2gfx == V111 ? 0x84CF : (version_D2gfx == V110 ? 0x446A : (version_D2gfx == V109d ? 0x447C : 0x447C)))))))));
        memt_byte(0x74, 0xEB);    // JMP SHORT D2gfx.6FA884F6
        //6FA884AF  |. 74 45          JE SHORT D2gfx.6FA884F6
        //6FA8894F  |. 74 45          JE SHORT D2gfx.6FA88996
        //6FA885BF  |. 74 45          JE SHORT D2gfx.6FA88606
        //6FA8B6B0  |. 74 45          JE SHORT D2gfx.6FA8B6F7
        //004F562B  |. 74 45          JE SHORT Game.004F5672

        log_msg("\n");
        isInstalled = true;
    }

/****************************************************************************************************/

    __declspec(naked)void caller_AlwaysDisplayLife_114() {
        __asm{
        JNZ displayLife
        CMP onRealm, 0
        JNZ normalDisplayLife
        CMP active_AlwaysDisplayLifeMana, 0
        JE normalDisplayLife
        displayLife:
        POP EAX
        ADD EAX, 0x24
        JMP EAX
        normalDisplayLife:
        LEA EAX, DWORD PTR DS:[ESI-0x1E]
        RETN
        }
    }

    __declspec(naked)void caller_AlwaysDisplayLife_113() {
        __asm{
        POP EAX
        CMP onRealm, 0
        JNZ normalDisplayLife
        CMP active_AlwaysDisplayLifeMana, 0
        JE normalDisplayLife
        ADD EAX, 0x28
        JMP EAX
        normalDisplayLife:
        CMP EDX, 0x1E
        JGE cont
        ADD EAX, 0xBC
        cont:
        JMP EAX
        }
    }

    __declspec(naked)void caller_AlwaysDisplayLife_111() {
        __asm{
        CMP onRealm, 0
        JNZ normalDisplayLife
        CMP active_AlwaysDisplayLifeMana, 0
        JE normalDisplayLife
        POP EAX
        ADD EAX, 0x25
        JMP EAX
        normalDisplayLife:
        MOV EAX, ptResolutionY
        MOV EAX, DWORD PTR DS:[EAX]
        RETN
        }
    }

    __declspec(naked)void caller_AlwaysDisplayLife() {
        __asm{
        CMP onRealm, 0
        JNZ normalDisplayLife
        CMP active_AlwaysDisplayLifeMana, 0
        JE normalDisplayLife
        POP EAX
        ADD EAX, 0x28
        JMP EAX
        normalDisplayLife:
        MOV EAX, ptResolutionY
        MOV EAX, DWORD PTR DS:[EAX]
        RETN
        }
    }

    __declspec(naked)void caller_AlwaysDisplayMana_114() {
        __asm{
        CMP onRealm, 0
        JNZ normalDisplayMana
        CMP active_AlwaysDisplayLifeMana, 0
        JE normalDisplayMana
        POP EAX
        ADD EAX, 0x23
        JMP EAX
        normalDisplayMana:
        LEA EDI, DWORD PTR DS:[ECX-0x6F]
        CMP ESI, EDI
        RETN
        }
    }

    __declspec(naked)void caller_AlwaysDisplayMana_113() {
        __asm{
        MOV EAX, DWORD PTR DS:[ptResolutionY]
        MOV EAX, DWORD PTR DS:[EAX]
        CMP onRealm, 0
        JNZ normalDisplayMana
        CMP active_AlwaysDisplayLifeMana, 0
        JE normalDisplayMana
        POP EAX
        ADD EAX, 0x3C
        JMP EAX
        normalDisplayMana:
        RETN
        }
    }

    __declspec(naked)void caller_AlwaysDisplayMana() {
        __asm{
        CMP onRealm, 0
        JNZ normalDisplayMana
        CMP active_AlwaysDisplayLifeMana, 0
        JE normalDisplayMana
        POP EAX
        MOV WORD PTR SS:[ESP+0x14], 0
        ADD EAX, 0x38
        JMP EAX
        normalDisplayMana:
        POP EAX
        POP EDI
        POP ESI
        POP EBP
        POP EBX
        ADD ESP, 0x25C
        RETN
        }
    }

    __declspec(naked)void caller_AlwaysDisplayMana_9() {
        __asm{
        CMP onRealm, 0
        JNZ normalDisplayMana
        CMP active_AlwaysDisplayLifeMana, 0
        JE normalDisplayMana
        ADD DWORD PTR SS:[ESP], 0x32
        LEA ECX, DWORD PTR SS:[ESP+0x24]
        PUSH DWORD PTR SS:[ESP+0xC]
        PUSH DWORD PTR SS:[ESP+0xC]
        PUSH DWORD PTR SS:[ESP+0xC]
        CALL D2PrintString
        MOV WORD PTR SS:[ESP+0x24], 0
        RETN 0xC
        normalDisplayMana:
        POP EAX
        ADD EAX, 0xC2
        JMP EAX
        }
    }

    void Install_AlwaysDisplayLifeMana() {
        static int isInstalled = false;
        if (isInstalled) return;
        log_msg("Patch D2Client for always display life and mana. (ALwaysPrintLifeMana)\n");
        if (version_D2Client >= V113c) {
            mem_seek(offset_D2Client + (version_D2Client == V114d ? 0x98183 : (version_D2Client == V113d ? 0x6D6FA : (version_D2Client == V113c ? 0x2764A : (version_D2Client == V112 ? 0x0000 : (version_D2Client == V111b ? 0x0000 : (version_D2Client == V111 ? 0x0000 : (version_D2Client == V110 ? 0x0000 : (version_D2Client == V109d ? 0x0000 : 0x0000)))))))));
            if (version_D2Client < V114d) memt_byte(0x0F, 0x90);
            memt_byte(version_D2Client >= V114d ? 0x75 : 0x8C, 0xE8);
            MEMT_REF4(version_D2Client >= V114d ? 0xE2468D27 : 0x000000BC, version_D2Client >= V114d ? caller_AlwaysDisplayLife_114 : caller_AlwaysDisplayLife_113);
            //6FAD764A  |. 0F8C BC000000  JL D2Client.6FAD770C
            //6FAD7650  |. 83FA 6E        CMP EDX,6E
            //6FAD7653  |. 0F8F B3000000  JG D2Client.6FAD770C
            //6FAD7659  |. A1 4CBCB86F    MOV EAX,DWORD PTR DS:[6FB8BC4C]

            //6FAD7667  |. 0F8C A4000000  JL D2Client.6FAD7711
            //6FB1D717  |. 0F8C A4000000  JL D2Client.6FB1D7C1
            //6FB1D6FA  |. 0F8C BC000000  JL D2Client.6FB1D7BC
            //00498183  |. 75 27          JNZ SHORT Game.004981AC
            //00498185  |. 8D46 E2        LEA EAX,DWORD PTR DS:[ESI-1E]
        } else {
            // Always display life.
            mem_seek(offset_D2Client + (version_D2Client == V114d ? 0x0000 : (version_D2Client == V113d ? 0x0000 : (version_D2Client == V113c ? 0x0000 : (version_D2Client == V112 ? 0x81733 : (version_D2Client == V111b ? 0xB5DF3 : (version_D2Client == V111 ? 0x2D713 : (version_D2Client == V110 ? 0x5F102 : (version_D2Client == V109d ? 0x58B32 : 0x58B32)))))))));
            memt_byte(0xA1, 0xE8);
            MEMT_REF4(ptResolutionY, version_D2Client >= V111 ? caller_AlwaysDisplayLife_111 : caller_AlwaysDisplayLife);
            //6FADD713  |. A1 605CBA6F    MOV EAX,DWORD PTR DS:[6FBA5C60]
            //6FB65DF3  |. A1 C84FBA6F    MOV EAX,DWORD PTR DS:[6FBA4FC8]
            //6FB31733  |. A1 E4C6B86F    MOV EAX,DWORD PTR DS:[6FB8C6E4]
        }


        // Always display mana.
        if (version_D2Client >= V113c) {
            //mem_seek R7(D2Client, 0000, 0000, 0000, 0000, 0000, 0000, 27711);
            //memt_byte( 0x8B , 0x90);
            //memt_byte( 0x0D , 0xE8);
            //MEMT_REF4( offset_D2Client + 0x11C4B4 , caller_AlwaysDisplayMana_113);
            mem_seek(offset_D2Client + (version_D2Client == V114d ? 0x9825D : (version_D2Client == V113d ? 0x6D7BC : (version_D2Client == V113c ? 0x2770C : (version_D2Client == V112 ? 0x0000 : (version_D2Client == V111b ? 0x0000 : (version_D2Client == V111 ? 0x0000 : (version_D2Client == V110 ? 0x0000 : (version_D2Client == V109d ? 0x0000 : 0x0000)))))))));
            memt_byte(version_D2Client >= V114d ? 0x8D : 0xA1, 0xE8);
            MEMT_REF4(version_D2Client >= V114d ? 0xF73B9179 : (DWORD) ptResolutionY, version_D2Client >= V114d ? caller_AlwaysDisplayMana_114 : caller_AlwaysDisplayMana_113);
            //6FAD770C  |> A1 4CBCB86F    MOV EAX,DWORD PTR DS:[6FB8BC4C]
            //6FAD7711  |> 8B0D B4C4BC6F  MOV ECX,DWORD PTR DS:[6FBCC4B4]

            //6FAD7708  |. 8B5424 10      MOV EDX,DWORD PTR SS:[ESP+10]

            //6FAD77C7  |> 5F             POP EDI
            //6FAD77C8  |. 5E             POP ESI
            //6FAD77C9  |. 5D             POP EBP
            //6FAD77CA  |. 5B             POP EBX
            //6FAD77CB  |. 81C4 28030000  ADD ESP,328
            //6FAD77D1  \. C3             RETN
            //6FB1D7BC  |> A1 3870BA6F    MOV EAX,DWORD PTR DS:[6FB8BC4C]
            //0049825D  |. 8D79 91        LEA EDI,DWORD PTR DS:[ECX-6F]
            //00498260  |. 3BF7           CMP ESI,EDI
        } else if (version_D2Client >= V110) {
            mem_seek(offset_D2Client + (version_D2Client == V114d ? 0x0000 : (version_D2Client == V113d ? 0x0000 : (version_D2Client == V113c ? 0x0000 : (version_D2Client == V112 ? 0x8181B : (version_D2Client == V111b ? 0xB5EDB : (version_D2Client == V111 ? 0x2D7FB : (version_D2Client == V110 ? 0x5F1E6 : (version_D2Client == V109d ? 0x0000 : 0x0000)))))))));
            memt_byte(0x5F, 0xE8);
            MEMT_REF4(0x815B5D5E, caller_AlwaysDisplayMana);
            memt_byte(0xC4, 0x90);    // NOP
            memt_byte(0x5C, 0x90);    // NOP
            memt_byte(0x02, 0x90);    // NOP
            memt_byte(0x00, 0x90);    // NOP
            memt_byte(0x00, 0x90);    // NOP
            //6FADD7FB  |. 5F             POP EDI
            //6FADD7FC  |. 5E             POP ESI
            //6FADD7FD  |. 5D             POP EBP
            //6FADD7FE  |. 5B             POP EBX
            //6FADD7FF  |. 81C4 5C020000  ADD ESP,25C
            //6FB65EDB  |. 5F             POP EDI
            //6FB65EDC  |. 5E             POP ESI
            //6FB65EDD  |. 5D             POP EBP
            //6FB65EDE  |. 5B             POP EBX
            //6FB65EDF  |. 81C4 5C020000  ADD ESP,25C
            //6FB3181B  |. 5F             POP EDI
            //6FB3181C  |. 5E             POP ESI
            //6FB3181D  |. 5D             POP EBP
            //6FB3181E  |. 5B             POP EBX
            //6FB3181F  |. 81C4 5C020000  ADD ESP,25C
            //6FB31825  |. C3             RETN
        } else {
            mem_seek(offset_D2Client + (version_D2Client == V114d ? 0x0000 : (version_D2Client == V113d ? 0x0000 : (version_D2Client == V113c ? 0x0000 : (version_D2Client == V112 ? 0x0000 : (version_D2Client == V111b ? 0x0000 : (version_D2Client == V111 ? 0x0000 : (version_D2Client == V110 ? 0x0000 : (version_D2Client == V109d ? 0x58C09 : 0x58C09)))))))));
            memt_byte(0xE9, 0xE8);
            MEMT_REF4(0x000000C2, caller_AlwaysDisplayMana_9);
            //6FAF8C09   . E9 C2000000    JMP D2Client.6FAF8CD0
        }
        if (active_AlwaysDisplayLifeMana == 2)
            active_AlwaysDisplayLifeMana = 0;
        log_msg("\n");
        isInstalled = true;
    }

/****************************************************************************************************/

    void Install_EnabledTXTFilesWithMSExcel() {
        static int isInstalled = false;
        if (isInstalled) return;
        log_msg("Patch D2Client for enabled the opening of files already opened by MS Excel. (EnabledTXTFilesWithMSExcel)\n");
        mem_seek((DWORD) D2StormMPQOpenFile + (version_Storm >= V114d ? 0xB4 : version_Storm >= V111 ? 0x12A : 0xFF));
        memt_byte(0x01, 0x03);    //; |ShareMode = FILE_SHARE_READ|FILE_SHARE_WRITE
        //6FC1C969  |. 6A 01          PUSH 1        ; |ShareMode = FILE_SHARE_READ

        log_msg("\n");
        isInstalled = true;
    }

/****************************************************************************************************/

    void __stdcall printDisplayBaseStatsValue(WORD statID, sDrawImageInfo *data, DWORD x, DWORD y, DWORD p4, DWORD p5, DWORD p6) {
        if (onRealm || !D2isLODGame()) {
            D2PrintImage(data, x, y, p4, p5, p6);
            return;
        }
        auto ptChar = D2GetClientPlayer();
        CharStatsBIN *charStats = D2GetCharStatsBIN(ptChar->nPlayerClass);
        int minValue = 1;
        switch (statID) {
            case STATS_STRENGTH:
                minValue = charStats->baseSTR;
                break;
            case STATS_DEXTERITY:
                minValue = charStats->baseDEX;
                break;
            case STATS_VITALITY:
                minValue = charStats->baseVIT;
                break;
            case STATS_ENERGY:
                minValue = charStats->baseENE;
                break;
        }
        int statValue = D2GetPlayerBaseStat(ptChar, statID, 0);
        if (isOnRect(D2GetMouseX(), D2GetMouseY(), x + 5, y + 5, 32, 32)) {
            WCHAR text[100];
            _snwprintf(text, 100, getLocalString(STR_STATS_BASE_MIN), statValue, minValue);
            D2SetFont(1);
            D2PrintPopup(text, x + 18, y - 32, WHITE, 1);
        }
        if (D2GetPlayerBaseStat(ptChar, 4, 0) <= 0)
            setFrame(data, 2);
        D2PrintImage(data, x, y, p4, p5, p6);
    }

    __declspec(naked)void caller_displayBaseStatsValue() {
        __asm{
        POP EAX
        XOR ECX, ECX
        MOV CX, WORD PTR DS:[ESI+8]
        PUSH ECX
        PUSH EAX
        JMP printDisplayBaseStatsValue
        }
    }

    void Install_DisplayBaseStatsValue() {
        static int isInstalled = false;
        if (isInstalled) return;
        log_msg("Patch D2Client for display base stats value. (DisplayBaseStatsValue)\n");

        // Always print stat button images.
        mem_seek(offset_D2Client + (version_D2Client == V114d ? 0xA7FFB : (version_D2Client == V113d ? 0xBF955 : (version_D2Client == V113c ? 0xBD1B5 : (version_D2Client == V112 ? 0x6B59A : (version_D2Client == V111b ? 0x8963A : (version_D2Client == V111 ? 0x82BBA : (version_D2Client == V110 ? 0x30073 : (version_D2Client == V109d ? 0x29B02 : 0x29B12)))))))));
        memt_byte(0x8B, 0xEB);    // JMP SHORT D2Client.6FAD0088
        memt_byte(version_D2Client == V114d ? 0x4D : 0x4C, getAddressOfVersion(version_D2Client, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x12));
        memt_byte(version_D2Client == V114d ? 0xF8 : 0x24, 0x90);    // NOP
        if (version_D2Client < V114d) memt_byte(getAddressOfVersion(version_D2Client, 0x20, 0x20, 0x14, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x53), 0x90);            // NOP (V109d:0x20 , V110:0x14
        //6FAD0073     8B4C24 14         MOV ECX,DWORD PTR SS:[ESP+14]
        //6FB32BBA   > 8B4C24 1C         MOV ECX,DWORD PTR SS:[ESP+1C]
        //6FB3963A   > 8B4C24 1C         MOV ECX,DWORD PTR SS:[ESP+1C]
        //6FB1B59A   > 8B4C24 1C      MOV ECX,DWORD PTR SS:[ESP+1C]
        //6FB6D1B5  |> 8B4C24 1C      MOV ECX,DWORD PTR SS:[ESP+1C]
        //6FB6F955  |> 8B4C24 1C      MOV ECX,DWORD PTR SS:[ESP+1C]
        //004A7FFB   > 8B4D F8        MOV ECX,DWORD PTR SS:[EBP-8]
        //004A7FFE   . 53             PUSH EBX                                 ; /Arg3

        mem_seek(offset_D2Client + (version_D2Client == V114d ? 0xA808C : (version_D2Client == V113d ? 0xBF9DE : (version_D2Client == V113c ? 0xBD23E : (version_D2Client == V112 ? 0x6B637 : (version_D2Client == V111b ? 0x896D4 : (version_D2Client == V111 ? 0x82C54 : (version_D2Client == V110 ? 0x300FD : (version_D2Client == V109d ? 0x29B8D : 0x29B9D)))))))));
        MEMJ_REF4(D2PrintImage, caller_displayBaseStatsValue);
        //6FB32C53   . E8 82A3F8FF    CALL <JMP.&D2gfx.#10047>
        //6FB396D3   . E8 D238F8FF    CALL <JMP.&D2gfx.#10044>
        //6FB1B636   . E8 431AFAFF    CALL <JMP.&D2gfx.#10024>
        //6FB6D23D  |. E8 54FEF4FF    |CALL <JMP.&D2gfx.#10041>
        //6FB6F9DD  |. E8 54FEF4FF    |CALL <JMP.&D2gfx.#10042>
        //004A808B   . E8 F0E30400    CALL Game.004F6480                       ; \Game.004F6480

        log_msg("\n");
        isInstalled = true;
    }

/****************************************************************************************************/

    RunesBIN *__stdcall compileRunesTxt(DWORD unused, const char *filename, BINField *ptFields, DWORD *ptRecordCount, DWORD recordLength) {
        RunesBIN *ptRunesBin = (RunesBIN *) D2CompileTxtFile(unused, filename, ptFields, ptRecordCount, recordLength);
        for (DWORD i = 0; i < *ptRecordCount; i++)
            ptRunesBin[i].Server = 0;
        return ptRunesBin;
    }

    void Install_LadderRunewords() {
        static int isInstalled = false;
        if (isInstalled) return;
        if (version_D2Common < V110)
            return;
        log_msg("Patch D2Common for enabled the ladder only runewords. (LadderRunewords)\n");
        mem_seek(offset_D2Common + (version_D2Common == V114d ? 0x239C40 : (version_D2Common == V113d ? 0x63782 : (version_D2Common == V113c ? 0x724B2 : (version_D2Common == V112 ? 0x5D492 : (version_D2Common == V111b ? 0x43A72 : (version_D2Common == V111 ? 0x61762 : (version_D2Common == V110 ? 0x1E965 : (version_D2Common == V109d ? 0x0000 : 0x0000)))))))));
        MEMC_REF4(D2CompileTxtFile, compileRunesTxt);
        //01B6E964  |. E8 0714FFFF    CALL D2Common.#10578                     ; \#10578
        //6FDB1761  |. E8 FAA8FCFF    CALL D2Common.#10653                     ; \#10653
        //6FD93A71  |. E8 EAADFCFF    CALL D2Common.#10496                     ; \#10496
        //6FDAD491  |. E8 BA49FEFF    CALL D2Common.#10244                     ; \#10244
        //6FDC24B1  |. E8 8ACAFEFF    CALL D2Common.#10849                     ; \#10849
        //6FDB3781  |. E8 5A93FAFF    CALL D2Common.#10037                     ; \#10037
        //00639C3F  |. E8 AC86FDFF    CALL Game.006122F0                       ; \Game.006122F0

        log_msg("\n");
        isInstalled = true;
    }

/****************************************************************************************************/

    void Install_EnabledCowPortalWhenCowKingWasKill() {
        static int isInstalled = false;
        if (isInstalled) return;
        log_msg("Patch D2Game for enabled the opening of Cow Portal when player have already kill the cow king in that difficulty. (EnabledCowPortalWhenCowKingWasKill)\n");
        mem_seek(offset_D2Game + (version_D2Game == V114d ? 0x194179 : (version_D2Game == V113d ? 0xEBE8 : (version_D2Game == V113c ? 0x67508 : (version_D2Game == V112 ? 0x75C68 : (version_D2Game == V111b ? 0x5DB68 : (version_D2Game == V111 ? 0xB1278 : (version_D2Game == V110 ? 0x6C5E7 : (version_D2Game == V109d ? 0x5E457 : 0x5DFF7)))))))));
        memt_byte(0x85, 0x33); // XOR EAX,EAX
        //6FC8DFF7   . 85C0           TEST EAX,EAX
        //6FC8E457   . 85C0           TEST EAX,EAX
        //6FC9C5E7   . 85C0           TEST EAX,EAX
        //02061278  |. 85C0           TEST EAX,EAX
        //01FEDB68  |. 85C0           TEST EAX,EAX
        //6FC95C68  |. 85C0           TEST EAX,EAX
        //6FC87508  |. 85C0           TEST EAX,EAX
        //6FC2EBE8  |. 85C0           TEST EAX,EAX
        //00594179  |. 85C0           TEST EAX,EAX

        log_msg("\n");
        isInstalled = true;
    }

/****************************************************************************************************/

    void Install_DoNotCloseNihlathakPortal() {
        static int isInstalled = false;
        if (isInstalled) return;
        log_msg("Patch D2Game to not close Nihlathak's portal. (DoNotCloseNihlathakPortal)\n");
        mem_seek(offset_D2Game + (version_D2Game == V114d ? 0x18B9D7 : (version_D2Game == V113d ? 0x1FF27 : (version_D2Game == V113c ? 0x5AE67 : (version_D2Game == V112 ? 0x26B67 : (version_D2Game == V111b ? 0x51987 : (version_D2Game == V111 ? 0x107C7 : (version_D2Game == V110 ? 0x86679 : (version_D2Game == V109d ? 0x77019 : 0x76B19)))))))));
        memt_byte(0x0F, 0x90);
        memt_byte(0x85, 0x90);
        memt_dword(version_D2Game >= V114d ? 0x000000C5 : version_D2Game >= V111 ? 0x000000BF : 0x000000C1, 0x90909090);
        //6FCA6B19   . 0F85 C1000000  JNZ D2Game.6FCA6BE0
        //6FCA7019   . 0F85 C1000000  JNZ D2Game.6FCA70E0
        //6FCB6679   . 0F85 C1000000  JNZ D2Game.6FCB6740
        //6FC307C7   . 0F85 BF000000  JNZ D2Game.6FC3088C
        //6FC71987   . 0F85 BF000000  JNZ D2Game.6FC71A4C
        //6FC46B67   . 0F85 BF000000  JNZ D2Game.6FC46C2C
        //6FC7AE67   . 0F85 BF000000  JNZ D2Game.6FC7AF2C
        //6FC3FF27   . 0F85 BF000000  JNZ D2Game.6FC3FFEC
        //0058B9D7  |. 0F85 C5000000  JNZ Game.0058BAA2

        log_msg("\n");
        isInstalled = true;
    }

/****************************************************************************************************/
/* townWest.ds1:
	129A6 : 4D->76
	129AA : 56->3A
	12ACA : 05->04
	+4    : 4D->76
	+4    : 56->3A
	+4    : 4E->76
	+4    : 58->3A
	+4    : 01->01
	+4    : 4C->7B
	+4    : 57->50
	+4    : 02->02
	+4    : 4D->7B
	+4    : 5D->41
	+4    : 01->01
	+4    : 49->7D
	+4    : 5C->3E
	+4    : 02->02
	+4    : 4F->
	+4    : 5B->
	+4    : 02->
*/

/*char* FASTCALL usePlugYTownWest(char* filename)
{
	if (onRealm)
		return filename;

	if (!strcmp(filename, "DATA\\GLOBAL\\TILES\\Expansion\\Town\\townWest.ds1"))
		return "PlugY\\townWest.ds1";

	return filename;
}

FCT_ASM ( caller_usePlugYTownWest )
	LEA EAX,DWORD PTR SS:[EBP-0x40]
	MOV ECX,EAX
	CALL usePlugYTownWest
	MOV ECX,EAX
	RETN
}}

FCT_ASM ( caller_usePlugYTownWest_113d )
	LEA EAX,DWORD PTR SS:[ESP+0x14]
	MOV ECX,EAX
	CALL usePlugYTownWest
	MOV EDX,ESI
	RETN
}}*/

/****************************************************************************************************/

    void *__fastcall updateHarrogath(DWORD unused, char *filename, DWORD *size, const char *__CallFile, DWORD __CallLine) {
        if (onRealm)
            return D2ReadFile(unused, filename, size, __CallFile, __CallLine);
        if (strcmp(filename, "DATA\\GLOBAL\\TILES\\Expansion\\Town\\townWest.ds1"))
            return D2ReadFile(unused, filename, size, __CallFile, __CallLine);
        DWORD fileSize = 0;
        if (size == NULL)
            size = &fileSize;
        char *data = (char *) D2ReadFile(unused, filename, size, __FILE__, __LINE__);
        if (*size > 400 &&
            *(DWORD *) (data + *size - 0x16C) == 0x0000004D &&
            *(DWORD *) (data + *size - 0x168) == 0x00000056 &&
            *(DWORD *) (data + *size - 0x48) == 0x00000005 &&
            *(DWORD *) (data + *size - 0x44) == 0x0000004D &&
            *(DWORD *) (data + *size - 0x40) == 0x00000056 &&
            *(DWORD *) (data + *size - 0x3C) == 0x0000004E &&
            *(DWORD *) (data + *size - 0x38) == 0x00000058 &&
            *(DWORD *) (data + *size - 0x34) == 0x00000001 &&
            *(DWORD *) (data + *size - 0x30) == 0x0000004C &&
            *(DWORD *) (data + *size - 0x2C) == 0x00000057 &&
            *(DWORD *) (data + *size - 0x28) == 0x00000002 &&
            *(DWORD *) (data + *size - 0x24) == 0x0000004D &&
            *(DWORD *) (data + *size - 0x20) == 0x0000005D &&
            *(DWORD *) (data + *size - 0x1C) == 0x00000001 &&
            *(DWORD *) (data + *size - 0x18) == 0x00000049 &&
            *(DWORD *) (data + *size - 0x14) == 0x0000005C &&
            *(DWORD *) (data + *size - 0x10) == 0x00000002 &&
            *(DWORD *) (data + *size - 0x0C) == 0x0000004F &&
            *(DWORD *) (data + *size - 0x08) == 0x0000005B &&
            *(DWORD *) (data + *size - 0x04) == 0x00000002) {
            log_msg("update TownWest to Move Cain near Harrogath waypoint\n");
            *(DWORD *) (data + *size - 0x16C) = 0x00000076;
            *(DWORD *) (data + *size - 0x168) = 0x0000003A;
            *(DWORD *) (data + *size - 0x48) = 0x00000004;
            *(DWORD *) (data + *size - 0x44) = 0x00000076;
            *(DWORD *) (data + *size - 0x40) = 0x0000003A;
            *(DWORD *) (data + *size - 0x3C) = 0x00000076;
            *(DWORD *) (data + *size - 0x38) = 0x0000003A;
            *(DWORD *) (data + *size - 0x34) = 0x00000001;
            *(DWORD *) (data + *size - 0x30) = 0x0000007B;
            *(DWORD *) (data + *size - 0x2C) = 0x00000050;
            *(DWORD *) (data + *size - 0x28) = 0x00000002;
            *(DWORD *) (data + *size - 0x24) = 0x0000007B;
            *(DWORD *) (data + *size - 0x20) = 0x00000041;
            *(DWORD *) (data + *size - 0x1C) = 0x00000001;
            *(DWORD *) (data + *size - 0x18) = 0x0000007D;
            *(DWORD *) (data + *size - 0x14) = 0x0000003E;
            *(DWORD *) (data + *size - 0x10) = 0x00000002;
            *(DWORD *) (data + *size - 0x0C) = 0x00000000;
            *(DWORD *) (data + *size - 0x08) = 0x00000000;
            *(DWORD *) (data + *size - 0x04) = 0x00000000;
        } else
            log_msg("ERROR : Failed to update TownWest to Move Cain near Harrogath waypoint\n");
        return data;
    }

    __declspec(naked)void caller_updateHarrogath() {
        __asm{
        MOV ECX, DWORD PTR SS:[ESP+4]
        PUSH 0
        PUSH 0
        PUSH DWORD PTR SS:[ESP+0x10]
        MOV EDX, EAX
        CALL updateHarrogath
        RETN 8
        }
    }

    void Install_MoveCainNearHarrogathWaypoint() {
        static int isInstalled = false;
        if (isInstalled) return;
        log_msg("Patch D2Common to move Deckard Cain near Waypoint in Harrogath. (MoveCainNearHarrogathWaypoint)\n");
        mem_seek(offset_D2Common + (version_D2Common == V114d ? 0x265972 : (version_D2Common == V113d ? 0x54033 : (version_D2Common == V113c ? 0xB573 : (version_D2Common == V112 ? 0x222A3 : (version_D2Common == V111b ? 0x39593 : (version_D2Common == V111 ? 0x82233 : (version_D2Common == V110 ? 0x45A2E : (version_D2Common == V109d ? 0x3654E : 0x3653E)))))))));
        MEMC_REF4(V2ReadFile, version_D2Common >= V111 && version_D2Common <= V113d ? (TD2ReadFile) caller_updateHarrogath : updateHarrogath);
        //6FD7653D  |. E8 62D30300    CALL D2Common.6FDB38A4
        //6FD7654D  |. E8 62D40300    CALL D2Common.6FDB39B4
        //6FD85A2D  |. E8 36E80300    CALL D2Common.6FDC4268
        //6FDD2232   . E8 A974F8FF    CALL D2Common.6FD596E0
        //6FD89592   . E8 3903FDFF    CALL D2Common.6FD598D0
        //6FD722A2   . E8 5976FEFF    CALL D2Common.6FD59900
        //6FD5B572   . E8 89E3FFFF    CALL D2Common.6FD59900
        //6FDA4032   . E8 A975FBFF    CALL D2Common.6FD5B5E0
        //00665971  |. E8 0317EBFF    CALL Game.00517079

        /*
        mem_seek R8(D2Common, 0000, 0000, 0000 , 0000, 0000, 734AA, 6C75A, 308FA, 21E4F7);
        memt_byte( 0x8D, 0xE8);
        MEMT_REF4( version_D2Common >= V114d ? 0xC88BC045 : 0x8B102444, version_D2Common >= V114d ? caller_usePlugYTownWest : caller_usePlugYTownWest_113d);
        if (version_D2Common < V114d) memt_byte( 0xD6, 0x90);
        //6FDC34AA  |. 8D4424 10      LEA EAX,DWORD PTR SS:[ESP+10]
        //6FDC34AE  |. 8BD6           MOV EDX,ESI
        //6FDBC75A  |. 8D4424 10      LEA EAX,DWORD PTR SS:[ESP+10]
        //6FDBC75E  |. 8BD6           MOV EDX,ESI
        //6FD808FA  |. 8D4424 10      LEA EAX,DWORD PTR SS:[ESP+10]
        //6FD808FE  |. 8BD6           MOV EDX,ESI
        //0061E4F7  |. 8D45 C0        LEA EAX,DWORD PTR SS:[EBP-40]
        //0061E4FA  |. 8BC8           MOV ECX,EAX
        */
        log_msg("\n");
        isInstalled = true;
    }

    ExperienceBIN *__stdcall modifExperienceBin(ExperienceBIN *ptExperienceBIN) {
        for (int i = 71; i < 101; i++)
            ptExperienceBIN[i].ExpRatio = 1024;
        return ptExperienceBIN;
    }

    __declspec(naked)void caller_modifExperienceBin() {
        __asm{
        PUSH DWORD PTR SS:[ESP+0x14]
        PUSH DWORD PTR SS:[ESP+0x14]
        PUSH DWORD PTR SS:[ESP+0x14]
        PUSH DWORD PTR SS:[ESP+0x14]
        PUSH DWORD PTR SS:[ESP+0x14]
        CALL D2CompileTxtFile
        PUSH EAX
        CALL modifExperienceBin
        RETN 0x14
        }
    }

    void Install_RemoveExperienceDiminushingReturn() {
        static int isInstalled = false;
        if (isInstalled) return;
        log_msg("Patch D2Common to remove experience diminushing return. (RemoveExperienceDiminushingReturn)\n");

        // modification of experienceBIN
        mem_seek(offset_D2Common + (version_D2Common == V114d ? 0x213E42 : (version_D2Common == V113d ? 0x00000 : (version_D2Common == V113c ? 0x00000 : (version_D2Common == V112 ? 0x00000 : (version_D2Common == V111b ? 0x00000 : (version_D2Common == V111 ? 0x00000 : (version_D2Common == V110 ? 0x00000 : (version_D2Common == V109d ? 0x00000 : 0x00000)))))))));
        MEMC_REF4(D2CompileTxtFile, caller_modifExperienceBin);
        //00613E41  |. E8 AAE4FFFF    CALL Game.006122F0                       ; \Game.006122F0

        log_msg("\n");
        isInstalled = true;
    }
}
