#include "stdafx.h"
#include "VipLotteryEvent.h"

#include "Util.h"
#include "Notice.h"
#include "Message.h"
#include "ScheduleManager.h"

cVipLottery gVipLottery;

cVipLottery::cVipLottery()
{
	for(int n=0;n < MAX_VIP_LOTTERY_DROP;n++)
	{
		CUSTOM_VIP_LOTTERY_INFO* lpInfo = &this->m_CustomVipLotteryInfo[n];

		lpInfo->Index = n;
		lpInfo->State = CUSTOM_VIP_LOTTERY_STATE_BLANK;
		lpInfo->RemainTime = 0;
		lpInfo->TargetTime = 0;
		lpInfo->TickCount = GetTickCount();
		lpInfo->AlarmMinSave = -1;
		lpInfo->AlarmMinLeft = -1;
	}
	this->m_Enable = 0;
}

cVipLottery::~cVipLottery()
{
	
}

void cVipLottery::Init()
{
	for(int n=0;n < MAX_VIP_LOTTERY_DROP;n++)
	{
		if(this->m_Enable == 0)
		{
			this->SetState(&this->m_CustomVipLotteryInfo[n],CUSTOM_VIP_LOTTERY_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_CustomVipLotteryInfo[n],CUSTOM_VIP_LOTTERY_STATE_EMPTY);
		}
	}
}

void cVipLottery::Load(char* path)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(path);

	if(res.status != pugi::status_ok)
	{
		ErrorMessageBox("%s file load fail (%s)", path, res.description());
	}

	for(int n=0;n < MAX_VIP_LOTTERY_DROP;n++)
	{
		this->m_CustomVipLotteryInfo[n].StartTime.clear();
		this->m_CustomVipLotteryInfo[n].RuleInfo.DropItem.clear();
	}

	pugi::xml_node VipLottery = file.child("VipLottery");

	this->m_Enable = VipLottery.attribute("Enable").as_int();

	pugi::xml_node list = VipLottery.child("StartInfo");

	for (pugi::xml_node startinfo = list.child("Time"); startinfo; startinfo = startinfo.next_sibling())
	{
		CUSTOM_VIP_LOTTERY_START_TIME info;

		int index = startinfo.attribute("Index").as_int();

		info.Year = startinfo.attribute("Year").as_int();

		info.Month = startinfo.attribute("Month").as_int();

		info.Day = startinfo.attribute("Day").as_int();

		info.DayOfWeek = startinfo.attribute("DayOfWeek").as_int();

		info.Hour = startinfo.attribute("Hour").as_int();

		info.Minute = startinfo.attribute("Minute").as_int();

		info.Second = startinfo.attribute("Second").as_int();

		this->m_CustomVipLotteryInfo[index].StartTime.push_back(info);
	}

	pugi::xml_node list1 = VipLottery.child("RuleInfo");

	for (pugi::xml_node ruleinfo = list1.child("Info"); ruleinfo; ruleinfo = ruleinfo.next_sibling())
	{
		int index = ruleinfo.attribute("Index").as_int();

		strcpy_s(this->m_CustomVipLotteryInfo[index].RuleInfo.Name,ruleinfo.attribute("Name").as_string());

		this->m_CustomVipLotteryInfo[index].RuleInfo.Map = ruleinfo.attribute("Map").as_int();

		this->m_CustomVipLotteryInfo[index].RuleInfo.MinLevel = ruleinfo.attribute("MinLevel").as_int();

		this->m_CustomVipLotteryInfo[index].RuleInfo.MaxLevel = ruleinfo.attribute("MaxLevel").as_int();

		this->m_CustomVipLotteryInfo[index].RuleInfo.MinReset = ruleinfo.attribute("MinReset").as_int();

		this->m_CustomVipLotteryInfo[index].RuleInfo.MaxReset = ruleinfo.attribute("MaxReset").as_int();

		this->m_CustomVipLotteryInfo[index].RuleInfo.MinGReset = ruleinfo.attribute("MinGReset").as_int();

		this->m_CustomVipLotteryInfo[index].RuleInfo.MaxGReset = ruleinfo.attribute("MaxGReset").as_int();

		this->m_CustomVipLotteryInfo[index].RuleInfo.AlarmTime = ruleinfo.attribute("AlarmTime").as_int();
	}

	pugi::xml_node list2 = VipLottery.child("RewardInfo");

	for (pugi::xml_node rewardinfo = list2.child("Info"); rewardinfo; rewardinfo = rewardinfo.next_sibling())
	{
		CUSTOM_VIP_LOTTERY_ITEM_INFO info;

		int index = rewardinfo.attribute("Index").as_int();

		info.AccountLevel = rewardinfo.attribute("AccountLevel").as_int();

		info.Count = rewardinfo.attribute("Count").as_int();

		info.Rate = rewardinfo.attribute("Rate").as_int();

		this->m_CustomVipLotteryInfo[index].RuleInfo.DropItem.push_back(info);
	}
}

void cVipLottery::MainProc()
{
	for(int n=0;n < MAX_VIP_LOTTERY_DROP;n++)
	{
		CUSTOM_VIP_LOTTERY_INFO* lpInfo = &this->m_CustomVipLotteryInfo[n];

		if((GetTickCount()-lpInfo->TickCount) >= 1000)
		{
			lpInfo->TickCount = GetTickCount();

			lpInfo->RemainTime = (int)difftime(lpInfo->TargetTime,time(0));

			switch(lpInfo->State)
			{
				case CUSTOM_VIP_LOTTERY_STATE_BLANK:
					this->ProcState_BLANK(lpInfo);
					break;
				case CUSTOM_VIP_LOTTERY_STATE_EMPTY:
					this->ProcState_EMPTY(lpInfo);
					break;
				case CUSTOM_VIP_LOTTERY_STATE_START:
					this->ProcState_START(lpInfo);
					break;
			}
		}
	}
}

void cVipLottery::ProcState_BLANK(CUSTOM_VIP_LOTTERY_INFO* lpInfo)
{
	
}

void cVipLottery::ProcState_EMPTY(CUSTOM_VIP_LOTTERY_INFO* lpInfo) // OK
{
	if(lpInfo->RemainTime > 0 && lpInfo->RemainTime <= (lpInfo->RuleInfo.AlarmTime*60))
	{
		if((lpInfo->AlarmMinSave=(((lpInfo->RemainTime%60)==0)?((lpInfo->RemainTime/60)-1):(lpInfo->RemainTime/60))) != lpInfo->AlarmMinLeft)
		{
			lpInfo->AlarmMinLeft = lpInfo->AlarmMinSave;

			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(21),lpInfo->RuleInfo.Name,(lpInfo->AlarmMinLeft+1));
		}
	}

	if(lpInfo->RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(22),lpInfo->RuleInfo.Name);
		this->SetState(lpInfo,CUSTOM_VIP_LOTTERY_STATE_START);
	}
}

void cVipLottery::ProcState_START(CUSTOM_VIP_LOTTERY_INFO* lpInfo) // OK
{
	for(std::vector<CUSTOM_VIP_LOTTERY_ITEM_INFO>::iterator it=lpInfo->RuleInfo.DropItem.begin();it != lpInfo->RuleInfo.DropItem.end();it++)
	{
		/*if(it->DropState == 0 && ((lpInfo->RuleInfo.EventTime*60)-lpInfo->RemainTime) >= it->DropDelay)
		{
			for(int n=0;n < it->DropCount;n++)
			{
				it->DropState = 1;

				int px = lpInfo->RuleInfo.X;
				int py = lpInfo->RuleInfo.Y;

				if(this->GetRandomItemDropLocation(lpInfo->RuleInfo.Map,&px,&py,lpInfo->RuleInfo.Range,lpInfo->RuleInfo.Range,50) == 0)
				{
					px = lpInfo->RuleInfo.X;
					py = lpInfo->RuleInfo.Y;
				}

				this->GCFireworksSendToNearUser(lpInfo->RuleInfo.Map,px,py);

				GDCreateItemSend(this->GetDummyUserIndex(),lpInfo->RuleInfo.Map,px,py,it->ItemIndex,it->ItemLevel,0,0,0,0,-1,0,0,0,0,0,0xFF,0);
			}
		}*/
	}

	if(lpInfo->RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(23),lpInfo->RuleInfo.Name);
		this->SetState(lpInfo,CUSTOM_VIP_LOTTERY_STATE_EMPTY);
	}
}

void cVipLottery::SetState(CUSTOM_VIP_LOTTERY_INFO* lpInfo,int state) // OK
{
	switch((lpInfo->State=state))
	{
		case CUSTOM_VIP_LOTTERY_STATE_BLANK:
			this->SetState_BLANK(lpInfo);
			break;
		case CUSTOM_VIP_LOTTERY_STATE_EMPTY:
			this->SetState_EMPTY(lpInfo);
			break;
		case CUSTOM_VIP_LOTTERY_STATE_START:
			this->SetState_START(lpInfo);
			break;
	}
}

void cVipLottery::SetState_BLANK(CUSTOM_VIP_LOTTERY_INFO* lpInfo)
{
	
}

void cVipLottery::SetState_EMPTY(CUSTOM_VIP_LOTTERY_INFO* lpInfo)
{
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	/*for(std::vector<CUSTOM_EVENT_DROP_ITEM_INFO>::iterator it=lpInfo->RuleInfo.DropItem.begin();it != lpInfo->RuleInfo.DropItem.end();it++)
	{
		it->DropState = 0;
	}*/

	this->CheckSync(lpInfo);
}

void cVipLottery::SetState_START(CUSTOM_VIP_LOTTERY_INFO* lpInfo)
{
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	//lpInfo->RemainTime = lpInfo->RuleInfo.EventTime*60;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void cVipLottery::CheckSync(CUSTOM_VIP_LOTTERY_INFO* lpInfo)
{
	if(lpInfo->StartTime.empty() != 0)
	{
		this->SetState(lpInfo,CUSTOM_VIP_LOTTERY_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<CUSTOM_VIP_LOTTERY_START_TIME>::iterator it=lpInfo->StartTime.begin();it != lpInfo->StartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpInfo,CUSTOM_VIP_LOTTERY_STATE_BLANK);
		return;
	}

	lpInfo->RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	lpInfo->TargetTime = (int)ScheduleTime.GetTime();
}
