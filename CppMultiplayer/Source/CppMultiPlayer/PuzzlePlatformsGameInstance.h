// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"

#include "Networking.h"
#include "ReceiveThread.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CPPMULTIPLAYER_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	
	UPuzzlePlatformsGameInstance(const FObjectInitializer & FObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	void HostServer(FString ServerName) override;

	UFUNCTION(Exec)
	void Join(uint32 Index) override;

	void StartSession();

	virtual void LoadMainMenu() override;

	void RefreshServerList() override;
	
private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
	
	class UMainMenu* Menu;
	class UMenuWidget* InGameMenu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FString DesiredServerName;

	void CreateSession();

public:
	//创建Socket并连接到服务器（主线程）
	UFUNCTION(BlueprintCallable, Category = "MySocket")
		bool SocketCreate(FString IPStr, int32 port);

	//发消息(主线程)
	UFUNCTION(BlueprintCallable, Category = "MySocket")
		bool SocketSend(FString message);

	//收消息(子线程)
	UFUNCTION(BlueprintCallable, Category = "MySocket")
		bool SocketReceive();

	UFUNCTION(BlueprintCallable, Category = "MySocket")
		bool ThreadEnd();

	FString StringFromBinaryArray(TArray<uint8> BinaryArray);

public:
	FSocket *Host;
	FIPv4Address ip;
	FRunnableThread* m_RecvThread;

};
