// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// requires CWinConsole decompilation
// ------------------------------

#include "stdafx.h"
#include "MuLua.h"
//#include "TLog.h"
//#include "WinConsole.h"
//#include "configread.h"
#include "LuaExport.h"

//int IncludeLuaQuest(lua_State* state);

MULua::MULua(bool UseSync): m_luaState(NULL)
{
	this->m_bUseSync = UseSync;
	this->Create();
}


MULua::~MULua()
{
	this->Release();
}

bool MULua::DoFile(const char* szFileName)
{
	if (this->m_bUseSync == true)
	{
		EnterCriticalSection(&this->m_luaCritical);
	}

	int iState = 0;

	if ( luaL_loadfile(m_luaState, szFileName) )
	{		
		//g_Log.MsgBox("Error!!, : %s\n", lua_tolstring(m_luaState, -1, 0) );
		MsgBox("Error!!, : %s\n", lua_tolstring(m_luaState, -1, 0) );
		lua_pop(m_luaState, 1);

		if (this->m_bUseSync == true)
		{
			LeaveCriticalSection(&this->m_luaCritical);
		}

		return false;
	}

    iState = lua_pcall(m_luaState, 0, -1, 0);

	if ( iState )
	{
		MsgBox("Error!!, : %s : State = %d\n", lua_tolstring(m_luaState, -1, 0) , iState);
		lua_pop(m_luaState, 1);

		if (this->m_bUseSync == true)
		{
			LeaveCriticalSection(&this->m_luaCritical);
		}

		return false;
	}

	if (this->m_bUseSync == true)
	{
		LeaveCriticalSection(&this->m_luaCritical);
	}

	return true;	
}

bool MULua::DoFile(char* szBuff, size_t size)
{
	if (this->m_bUseSync == true)
	{
		EnterCriticalSection(&this->m_luaCritical);
	}

	if (luaL_loadbuffer(m_luaState, szBuff, size, "loadfilemem") != 0)
	{
		MsgBox("Error!!, : %s\n", lua_tolstring(m_luaState, -1, 0));
		lua_pop(m_luaState, 1);

		if (this->m_bUseSync == true)
		{
			LeaveCriticalSection(&this->m_luaCritical);
		}

		return false;
	}

	int iState = lua_pcall(m_luaState, 0, -1, 0);

	if (iState)
	{
		MsgBox("Error!!, : %s : State = %d\n", lua_tolstring(m_luaState, -1, 0), iState);
		lua_pop(m_luaState, 1);

		if (this->m_bUseSync == true)
		{
			LeaveCriticalSection(&this->m_luaCritical);
		}

		return false;
	}

	if (this->m_bUseSync == true)
	{
		LeaveCriticalSection(&this->m_luaCritical);
	}

	return true;
}

bool MULua::DoFile(lua_State* L, const char* szFileName)
{
	return true;
}

bool MULua::DoString(std::string kString)
{
	return true;
}

lua_State* MULua::GetLua()
{
	return m_luaState;
}

bool MULua::Create()
{
	if (this->m_bUseSync == true)
	{
		InitializeCriticalSection(&this->m_luaCritical);
	}

	m_luaState = luaL_newstate();

	luaL_openlibs(m_luaState);
	//lua_pushcclosure(m_luaState, IncludeLuaQuest, 0);
	//lua_setglobal(m_luaState, "QuestInclude");
	lua_gc(m_luaState, LUA_GCCOLLECT, 0);

	gLuaExport.AddGlobal(m_luaState);
	gLuaExport.AddTable(m_luaState);

	return true;
}

void MULua::Register(void* pLua)
{
	lua_State* pLuaState = (lua_State*)pLua;
}

void MULua::RegisterData(const luaL_Reg * Table, char * TableName, char * DataName, size_t Size)
{
	lua_newuserdata(this->m_luaState, Size);
	luaL_newmetatable(this->m_luaState, TableName);
	luaL_setfuncs(this->m_luaState, Table, 0); // for lua 5.2
	//luaL_register(this->m_luaState, DataName, Table); not work in Lua 5.2
	lua_setmetatable(this->m_luaState, -2);
	lua_setglobal(this->m_luaState, DataName);
}

bool MULua::Generic_Call(const char* func, const char* sig, ...)
{
	if (this->m_luaState == NULL)
	{
		return false;
	}

	if (this->m_bUseSync == true)
	{
		EnterCriticalSection(&this->m_luaCritical);
	}

	va_list	VA_LIST;
	int nArg = 0;

	va_start(VA_LIST, sig);
	lua_getglobal(this->m_luaState, func);

	while (*sig)
	{
		switch (*sig++)
		{
		case 'd':
		{
			lua_pushnumber(this->m_luaState, va_arg(VA_LIST, double));
		} break;

		case 'i':
		{
			lua_pushnumber(this->m_luaState, va_arg(VA_LIST, int));
		} break;

		case 's':
		{
			char *pszString = va_arg(VA_LIST, char *);
			lua_pushlstring(this->m_luaState, pszString, strlen(pszString));
		} break;

		case 'w':
		{
			lua_pushnumber(this->m_luaState, va_arg(VA_LIST, unsigned int));
		} break;

		case 'l':
		{
			lua_pushnumber(this->m_luaState, va_arg(VA_LIST, unsigned long long));
		} break;

		case '>':
		{
			goto endwhile;
		}

		default:
		{
			//g_Log.Add("luacall_Generic_Call invalid option (%c)", *(sig - 1));

			if (this->m_bUseSync == true)
			{
				LeaveCriticalSection(&this->m_luaCritical);
			}

			return false;
		}
		}

		nArg++;
		luaL_checkstack(this->m_luaState, 1, "too many arguments");

	} endwhile:

	int nRes = strlen(sig);

	if (lua_pcall(this->m_luaState, nArg, nRes, 0) != 0)
	{
		//Add("luacall_Generic_Call error running function '%s': '%s'", func, lua_tolstring(this->m_luaState, -1, 0));

		if (this->m_bUseSync == true)
		{
			LeaveCriticalSection(&this->m_luaCritical);
		}

		return false;
	}

	int nRetValCnt = nRes;
	nRes = -nRes;

	while (*sig)
	{
		switch (*sig++)
		{
		case 'd':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				if (this->m_bUseSync == true)
				{
					LeaveCriticalSection(&this->m_luaCritical);
				}

				return false;
			}
			*va_arg(VA_LIST, double *) = lua_tonumber(this->m_luaState, nRes);

		} break;

		case 'i':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				if (this->m_bUseSync == true)
				{
					LeaveCriticalSection(&this->m_luaCritical);
				}

				return false;
			}

			*va_arg(VA_LIST, int *) = (int)lua_tonumber(this->m_luaState, nRes);
		} break;

		case 's':
		{
			if (!lua_isstring(this->m_luaState, nRes))
			{
				if (this->m_bUseSync == true)
				{
					LeaveCriticalSection(&this->m_luaCritical);
				}

				return false;
			}

			*va_arg(VA_LIST, const char **) = lua_tostring(this->m_luaState, nRes);
		} break;

		case 'w':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				if (this->m_bUseSync == true)
				{
					LeaveCriticalSection(&this->m_luaCritical);
				}

				return false;
			}

			*va_arg(VA_LIST, unsigned int*) = (unsigned int)lua_tonumber(this->m_luaState, nRes);
		} break;

		case 'l':
		{
			if (!lua_isnumber(this->m_luaState, nRes))
			{
				if (this->m_bUseSync == true)
				{
					LeaveCriticalSection(&this->m_luaCritical);
				}

				return false;
			}

			*va_arg(VA_LIST, unsigned long long*) = (unsigned long long)lua_tonumber(this->m_luaState, nRes);
		} break;

		default:
		{
			if (this->m_bUseSync == true)
			{
				LeaveCriticalSection(&this->m_luaCritical);
			}

			return false;
		} break;

		}

		nRes++;
	}

	va_end(VA_LIST);

	if (nRetValCnt)
	{
		lua_pop(this->m_luaState, nRetValCnt);
	}

	if (this->m_bUseSync == true)
	{
		LeaveCriticalSection(&this->m_luaCritical);
	}

	return true;
}

void MULua::Release()
{
	if (this->m_bUseSync == true)
	{
		DeleteCriticalSection(&this->m_luaCritical);
	}

	if(m_luaState)
	{
		lua_close(m_luaState);
		m_luaState = NULL;
		m_bUseSync = false;
	}
}


static const int sentinel_ = 0;
#define sentinel	((void *)&sentinel_)