#pragma once

//모든 스킬들이 공통으로 가지고 있는 정보
struct SKILL_BASIC_DATA
{
	bool   first_op{ true };
	bool   once_flag{ true };
	bool   on_using{ false };
	double op_time{ 0.0 };
	high_resolution_clock::time_point curr_cooltime;
	high_resolution_clock::time_point prev_cooltime;
};

//1.(실드)스킬과 관련된 정보
struct SKILL_SHIELD_DATA
{
	SKILL_BASIC_DATA data;
};

//2.(파동)스킬과 관련된 정보
struct SKILL_WAVESHOCK_DATA
{
	SKILL_BASIC_DATA data;
	bool operated{ false };
};

//3.(주사위불렛)스킬과 관련된 정보
struct SKILL_DICESTRIKE_DATA
{
	SKILL_BASIC_DATA data;
	bool cooltime_error{ false };
};
