#pragma once
#include "User.h"

class CAutoReset
{
private: 
	int auto_reset_exp_rate_[MAX_ACCOUNT_LEVEL];
	int auto_reset_per_day_[MAX_ACCOUNT_LEVEL];
public:
	CAutoReset(){}
	void Load(char* path);
}; extern CAutoReset gAutoReset;