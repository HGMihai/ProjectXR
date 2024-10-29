#include "stdafx.h"
#include "AutoReset.h"

CAutoReset gAutoReset;

void CAutoReset::Load(char* path)
{
	this->auto_reset_exp_rate_[0] = GetPrivateProfileInt("Common", "AutoResetExperienceRate_AL0", 0, path);
	this->auto_reset_exp_rate_[1] = GetPrivateProfileInt("Common", "AutoResetExperienceRate_AL1", 0, path);
	this->auto_reset_exp_rate_[2] = GetPrivateProfileInt("Common", "AutoResetExperienceRate_AL2", 0, path);
	this->auto_reset_exp_rate_[3] = GetPrivateProfileInt("Common", "AutoResetExperienceRate_AL3", 0, path);

	this->auto_reset_per_day_[0] = GetPrivateProfileInt("Common", "AutoResetPerDay_AL0", 0, path);
	this->auto_reset_per_day_[1] = GetPrivateProfileInt("Common", "AutoResetPerDay_AL1", 0, path);
	this->auto_reset_per_day_[2] = GetPrivateProfileInt("Common", "AutoResetPerDay_AL2", 0, path);
	this->auto_reset_per_day_[3] = GetPrivateProfileInt("Common", "AutoResetPerDay_AL3", 0, path);
}

