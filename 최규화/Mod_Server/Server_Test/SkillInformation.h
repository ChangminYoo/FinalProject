#pragma once

//��� ��ų���� �������� ������ �ִ� ����
struct SKILL_BASIC_DATA
{
	bool   first_op{ true };
	bool   once_flag{ true };
	bool   on_using{ false };
	double op_time{ 0.0 };
	high_resolution_clock::time_point curr_cooltime;
	high_resolution_clock::time_point prev_cooltime;
};

//1.(�ǵ�)��ų�� ���õ� ����
struct SKILL_SHIELD_DATA
{
	SKILL_BASIC_DATA data;
};

//2.(�ĵ�)��ų�� ���õ� ����
struct SKILL_WAVESHOCK_DATA
{
	SKILL_BASIC_DATA data;
	bool operated{ false };
};

//3.(�ֻ����ҷ�)��ų�� ���õ� ����
struct SKILL_DICESTRIKE_DATA
{
	SKILL_BASIC_DATA data;
	bool cooltime_error{ false };
};
