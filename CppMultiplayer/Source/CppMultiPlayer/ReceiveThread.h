// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
#include "Runnable.h"
#include "ThreadingBase.h"
#include "Networking.h"

class CPPMULTIPLAYER_API FReceiveThread : public FRunnable
{
public:
	FReceiveThread(FSocket* client) : m_Client(client)
	{}
	~FReceiveThread()
	{
		stopping = true;
	}

	virtual bool Init() override
	{
		stopping = false;
		return true;
	}

	virtual uint32 Run() override
	{
		if (!m_Client)
		{
			return 0;
		}

		TArray<uint8> ReceiveData;
		uint8 element = 0;
		//�������ݰ�
		while (!stopping)   //�̼߳���������
		{
			ReceiveData.Init(element, 1024u);
			int32 read = 0;
			m_Client->Recv(ReceiveData.GetData(), ReceiveData.Num(), read);
			const FString ReceivedUE4String = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceiveData.GetData())));
			FString log = "Server:" + ReceivedUE4String;
			UE_LOG(LogTemp, Warning, TEXT("*** %s"), *log);

			FPlatformProcess::Sleep(0.01f);
		}


		return 1;
	}

	virtual void Stop() override
	{
		stopping = true;    //������-1
	}

private:
	FSocket* m_Client;  //�ͻ����׽���
	bool stopping;      //ѭ������
	FThreadSafeCounter m_StopTaskCounter;   //�߳����ü�����
};