/*==============================================
	File created by Yohann NICOLAS.
	Add support 1.13d by L'Autour.

	this file implements some common and useful
	function related to some Diablo II mechanisms.

/*============================================*/
#pragma once

#include <Windows.h>
namespace PlugY {
    void loadLocalizedStrings(int language);

    LPCWSTR getLocalString(int stringId);

    LPCWSTR getLocalTypeString(DWORD code);

//    LPCWSTR StripGender(LPWSTR text);

    LPCWSTR StripGender(LPCWSTR text);

    enum eStringList {
        STR_STATS_UNASSIGN_WITH_LIMIT = 0,
        STR_STATS_UNASSIGN_WITHOUT_LIMIT,
        STR_STATS_BASE_MIN,
        STR_SKILLS_UNASSIGN,
        STR_STASH_PREVIOUS_PAGE,
        STR_STASH_NEXT_PAGE,
        STR_TOGGLE_TO_PERSONAL,
        STR_TOGGLE_TO_SHARED,
        STR_TOGGLE_MULTI_DISABLED,
        STR_STASH_PREVIOUS_INDEX,
        STR_STASH_NEXT_INDEX,
        STR_PUT_GOLD,
        STR_TAKE_GOLD,
        STR_PERSONAL_PAGE_NUMBER,
        STR_SHARED_PAGE_NUMBER,
        STR_NO_SELECTED_PAGE,
        STR_SHARED_GOLD_QUANTITY,
        STR_PREVIOUS_PAGE,
        STR_NEXT_PAGE,
        STR_ITEM_LEVEL,
        STR_PAGE_TYPE_CHANGE,
        //
        STR_COW_PORTAL,
        STR_PANDEMONIUM_PORTAL,
        STR_PANDEMONIUM_FINAL_PORTAL,
        STR_FULL,
        STR_REPAIR,
        STR_AND,
        STR_RECHARGE,
        STR_DESTROY_FILLERS,
        STR_REMOVE_FILLERS,
        STR_REGENERATE,
        STR_UPGRADE_TO_EXCEPTIONAL,
        STR_UPGRADE_TO_ELITE,
        STR_ETHERAL,
        STR_NOT_ETHERAL,
        STR_NOT_RUNEWORD,
        STR_BASIC,
        STR_EXCEPTIONAL,
        STR_ELITE,
        STR_CRACKED,
        STR_NORMAL,
        STR_SUPERIOR,
        STR_MAGIC,
        STR_SET,
        STR_RARE,
        STR_UNIQUE,
        STR_CRAFTED,
        STR_TEMPERED,
        STR_ITEM,
        STR_ITEM_SAME_TYPE,
        STR_INCLUDE_UPGRADED,
        STR_WITHOUT_SOCKET,
        STR_WITH_SOCKETS,
        STR_WITH_N_SOCKETS,
        STR_ONLY_N_H,
        STR_ONLY_HELL,
        STR_ONLY_CLASS,

        STR_MERCENARIES,
        STR_MERC_ACT_1,
        STR_MERC_ACT_2,
        STR_MERC_ACT_3,
        STR_MERC_ACT_5,
        STR_BREAKPOINT,
        STR_BREAKPOINTS,
        STR_BLOCK_FRAMES,
        STR_CASTING_FRAMES,
        STR_HIT_RECOVERY_FRAMES,
        STR_1H_SWINGING_WEAPON,
        STR_OTHER_WEAPONS,
        STR_HUMAN_FORM,
        STR_BEAR_FORM,
        STR_WOLF_FORM,
        STR_VAMPIRE_FORM,
        STR_SPEARS_AND_STAVES,
        STR_LIGHTNING_CHAIN_LIGHTNING,
        STR_OTHER_SPELLS
    };

}