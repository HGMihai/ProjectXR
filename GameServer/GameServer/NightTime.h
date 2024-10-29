#pragma once
#include "Map.h"

enum eeNightTimeState
{
	NIGHT_TIME_STATE_BLANK = 0,
	NIGHT_TIME_STATE_EMPTY = 1,
	NIGHT_TIME_STATE_START = 2,
};

struct NIGHT_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
	int AlarmTime;
	int EventTime;
};

struct NIGHT_MESSAGES
{
	char StartMessage[50];
	char EndMessage[50];
};

class cNightTime
{
public:
	cNightTime();
	~cNightTime();
	void Load(char* path);
	bool Check();
	NIGHT_START_TIME WorkTime;
	NIGHT_MESSAGES Messages;
	int Maps[MAX_MAP];
}; extern cNightTime gNightTime;