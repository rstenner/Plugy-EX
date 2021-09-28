/*=================================================================
	File created by Yohann NICOLAS.
	Add support 1.13d by L'Autour.
	Add support 1.14d by haxifix.

	Change Skill win per level up.

=================================================================*/

#include "skillPerLevelUp.h"
#include "common.h"

bool active_SkillPerLevelUpChange=0;
DWORD skillPerLevelUp=1;

__declspec(naked)void caller_changeSkillPerLevelUp_114() {
    __asm{
	MOV EAX,skillPerLevelUp
	IMUL EAX,EDI
	MOV DWORD PTR SS:[ESP+0xC], EAX
	JMP V2AddPlayerStat
}}


__declspec(naked)void caller_changeSkillPerLevelUp_111() {
    __asm{
	MOV EAX,skillPerLevelUp
	IMUL EAX,EBX
	MOV DWORD PTR SS:[ESP+0xC], EAX
	JMP V2AddPlayerStat
}}


__declspec(naked)void caller_changeSkillPerLevelUp() {
    __asm{
	MOV EAX,skillPerLevelUp
	IMUL EAX,EDI
	MOV DWORD PTR SS:[ESP+0xC], EAX
	JMP V2AddPlayerStat
}}

void Install_SkillPerLevelUp()
{
	static int isInstalled = false;
	if (isInstalled) return;

	log_msg("Patch D2Game for change Skill points win per level up. (SkillPerLevelUp)\n");

	mem_seek R8(D2Game,	42261,	42651,	4ED60, D7AE5, 79695, AA455, EB1E5, EDCA5, 1709D1);
	MEMJ_REF4( V2AddPlayerStat, version_D2Game >= V114d ? caller_changeSkillPerLevelUp_114 : version_D2Game >= V111 ? caller_changeSkillPerLevelUp_111 : caller_changeSkillPerLevelUp);
	//6FC7ED5F  |. E8 56CB0900    CALL <JMP.&D2Common.#10518>
	//02057AE4  |. E8 9F28F3FF    CALL <JMP.&D2Common.#10109>
	//01FE9694  |. E8 1B0DF9FF    CALL <JMP.&D2Common.#10627>
	//6FCCA454  |. E8 5F01F6FF    CALL <JMP.&D2Common.#10762>
	//6FD0B1E4  |. E8 E9F2F1FF    CALL <JMP.&D2Common.#10551>
	//6FD0DCA4  |. E8 FDC6F1FF    CALL <JMP.&D2Common.#10645>
	//005709D0  |. E8 DB680B00    CALL Game.006272B0                       ; \Game.006272B0

	log_msg("\n");

	isInstalled = true;
}

/*================================= END OF FILE =================================*/