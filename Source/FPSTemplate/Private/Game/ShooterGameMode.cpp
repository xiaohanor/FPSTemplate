// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ShooterGameMode.h"

DEFINE_LOG_CATEGORY(LogShooterGameMode);

AShooterGameMode::AShooterGameMode()
{
	
}

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();
	
#if WITH_GAMELIFT
	InitGameLift();
#endif
}

void AShooterGameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	//从 “aws gamelift get-compute-auth-token ”API 返回的 AuthToken。请注意，这将在 15 分钟后过期，需要重新调用 API。
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("AUTH_TOKEN: %s"), *OutServerParameters.m_authToken)
	}
	
	//The Host/compute-name of the GameLift Anywhere instance.
	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("HOST_ID: %s"), *OutServerParameters.m_hostId)
	}
	
	//The Anywhere Fleet ID.
	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("FLEET_ID: %s"), *OutServerParameters.m_fleetId)
	}
	
	//The WebSocket URL (GameLiftServiceSdkEndpoint).
	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("WEBSOCKET_URL: %s"), *OutServerParameters.m_webSocketUrl)
	}

	//The PID of the running process
	OutServerParameters.m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId());
	UE_LOG(LogShooterGameMode, Log, TEXT("PID: %s"), *OutServerParameters.m_processId);
}

void AShooterGameMode::ParseCommandLinePort(int32& OutPort)
{
	TArray<FString> CommandLineTokens;
	TArray<FString> CommandLineSwitches;
	FCommandLine::Parse(FCommandLine::Get(), CommandLineTokens, CommandLineSwitches);
	for (const FString& Switch : CommandLineSwitches)
	{
		FString Key;
		FString Value;
		if (Switch.Split("=", &Key, &Value))
		{
			if (Key.Equals(TEXT("port"), ESearchCase::IgnoreCase))
			{
				OutPort = FCString::Atoi(*Value);
				return;
			}
		}
	}
}

void AShooterGameMode::InitGameLift()
{
	UE_LOG(LogShooterGameMode, Log, TEXT("Initializing the GameLift Server"));
	
	FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
	
	//为 GameLift Anywhere 机群定义服务器参数。GameLift 管理的 EC2 机群不需要这些参数。
	FServerParameters ServerParameters;
	SetServerParameters(ServerParameters);

	//InitSDK与GameLift代理建立本地连接，以便进一步通信。
	//对 GameLift Anywhere 机群使用 InitSDK(serverParameters)。
	//对 GameLift 管理的 EC2 机群使用 InitSDK()。
	GameLiftSdkModule->InitSDK(ServerParameters);

	auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
	{
		FString GameSessionId = FString(gameSession.GetGameSessionId());
		UE_LOG(LogShooterGameMode, Log, TEXT("GameSession Initializing: %s"), *GameSessionId);
		GameLiftSdkModule->ActivateGameSession();
	};
	
	ProcessParameters.OnStartGameSession.BindLambda(OnGameSession);

	auto OnProcessTerminate = [=]()
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("Game Server process is terminating."));
		GameLiftSdkModule->ProcessEnding();
	};

	ProcessParameters.OnTerminate.BindLambda(OnProcessTerminate);

	auto OnHealthCheck = []() 
	{
		UE_LOG(LogShooterGameMode, Log, TEXT("Performing Health Check"));
		return true;
	};

	ProcessParameters.OnHealthCheck.BindLambda(OnHealthCheck);

	int32 Port = FURL::UrlConfig.DefaultPort;
	ParseCommandLinePort(Port);

	ProcessParameters.port = Port;

	TArray<FString> LogFiles;
	LogFiles.Add(TEXT("FPSTemplate/Saved/Logs/FPSTemplate.log"));
	ProcessParameters.logParameters = LogFiles;

	UE_LOG(LogShooterGameMode, Log, TEXT("Calling Process Ready."));
	GameLiftSdkModule->ProcessReady(ProcessParameters);
}
