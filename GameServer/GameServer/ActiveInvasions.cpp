#include "stdafx.h"
#include "ActiveInvasions.h"
#include "Protection.h"
#include "Util.h"

CActiveInvasions gActiveInvasions;

CActiveInvasions::CActiveInvasions()
{
	this->m_data.clear();
}

void CActiveInvasions::monster_add(int monster_id)
{
	if (!gProtection.GetCustomState(CUSTOM_ACTIVE_INVASIONS))
		return;

	auto it = this->m_data.find(monster_id);

	if(it == this->m_data.end())
	{
		this->m_data.insert(std::pair<int, InvasionsData>(monster_id, InvasionsData(1,1)));
	}
	else
	{
		it->second._count += 1;
		it->second._max_count += 1;
	}
}

void CActiveInvasions::monster_del(int monster_id, bool need_send)
{
	if (!gProtection.GetCustomState(CUSTOM_ACTIVE_INVASIONS))
		return;

	auto it = this->m_data.find(monster_id);

	if (it == this->m_data.end())
	{
		LogAdd(LOG_RED, "[CActiveInvasions] Error! Monster not found, id : %d", monster_id);
	}
	else
	{
		if (it->second._count > 0)
		{
			it->second._count -= 1;

			if(need_send)
				this->update_by_monster_id(monster_id);
		}
		else
			this->m_data.erase(monster_id);

		if(it->second._count == 0)
			this->m_data.erase(monster_id);
	}
}

void CActiveInvasions::update_by_monster_id(int monster_id)
{
	if (!gProtection.GetCustomState(CUSTOM_ACTIVE_INVASIONS))
		return;

	auto it = this->m_data.find(monster_id);

	if (it == this->m_data.end())
		return;

	PMSG_INVASION_MONSTER_UPDATE_SEND pMsg{};

	pMsg.header.set(0xFB, 0x99, sizeof(pMsg));

	pMsg.monster_id = monster_id;

	pMsg.count = it->second._count;

	DataSendAll(reinterpret_cast<BYTE*>(&pMsg), pMsg.header.size);
}


void CActiveInvasions::send_list_to_client() const
{
	if (!gProtection.GetCustomState(CUSTOM_ACTIVE_INVASIONS))
		return;

	BYTE send[8192];

	PMSG_ACTIVE_INVASIONS_SEND pMsg{};

	pMsg.header.set(0xFB, 0x98, 0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_ACTIVE_INVASIONS info{};

	for (auto& item : this->m_data)
	{
		info.monster_id = item.first;
		info.count = item.second;

		memcpy(&send[size], &info, sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send, &pMsg, sizeof(pMsg));
	DataSendAll(send, size);
}
