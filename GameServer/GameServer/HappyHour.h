#pragma once

struct HAPPYHOUR_TIME
{
	int		sIndex;
	int		sHour;
	int		sMin;
	int		sDuration;
};

class CHappyHour
{
public:
	CHappyHour();
	~CHappyHour();
	void Load(char* path);
	void StartEvent(int ID);
	void CloseEvent();
	void TickTime();

	int Enabled;
	int	EventID;
	int AddExpirience[4];
	int AddDrop[4];
	int Tick;

	int DefaultExp[4];
	int DefaultDrop[4];
	std::vector<HAPPYHOUR_TIME> m_HappyHour;
}; extern CHappyHour gHappyHour;