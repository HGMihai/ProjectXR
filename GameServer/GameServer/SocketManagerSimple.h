// SocketManager.h: interface for the CSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "CriticalSection.h"
#include "Queue.h"
#include "SocketManager.h"

extern int gClientCount;

class CSocketManagerSimple
{
public:
	CSocketManagerSimple();
	virtual ~CSocketManagerSimple();
	bool Start(WORD port);
	void Clean();
	bool CreateListenSocket();
	bool CreateCompletionPort();
	bool CreateAcceptThread();
	bool CreateWorkerThread();
	bool CreateServerQueue();
	bool DataRecv(int index, IO_MAIN_BUFFER* lpIoBuffer);
	bool DataSend(int index, BYTE* lpMsg, int size);
	void Disconnect(int index);
	void OnRecv(int index, DWORD IoSize, IO_RECV_CONTEXT* lpIoContext);
	void OnSend(int index, DWORD IoSize, IO_SEND_CONTEXT* lpIoContext);
	static int CALLBACK ServerAcceptCondition(IN LPWSABUF lpCallerId, IN LPWSABUF lpCallerData, IN OUT LPQOS lpSQOS, IN OUT LPQOS lpGQOS, IN LPWSABUF lpCalleeId, OUT LPWSABUF lpCalleeData, OUT GROUP FAR* g, CSocketManagerSimple* lpSocketManager);
	static DWORD WINAPI ServerAcceptThread(CSocketManagerSimple* lpSocketManager);
	static DWORD WINAPI ServerWorkerThread(CSocketManagerSimple* lpSocketManager);
	static DWORD WINAPI ServerQueueThread(CSocketManagerSimple* lpSocketManager);
	DWORD GetQueueSize();
private:
	SOCKET m_listen;
	HANDLE m_CompletionPort;
	WORD m_port;
	HANDLE m_ServerAcceptThread;
	HANDLE m_ServerWorkerThread[MAX_SERVER_WORKER_THREAD];
	DWORD m_ServerWorkerThreadCount;
	CQueue m_ServerQueue;
	HANDLE m_ServerQueueSemaphore;
	HANDLE m_ServerQueueThread;
	CCriticalSection m_critical;
};

extern CSocketManagerSimple gSocketManagerSimple;
