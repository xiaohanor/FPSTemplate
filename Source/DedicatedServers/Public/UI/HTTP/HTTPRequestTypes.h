#pragma once
#include "DedicatedServers/DedicatedServers.h"

#include "HTTPRequestTypes.generated.h"

USTRUCT()
struct FDMetaData
{
	GENERATED_BODY()

	UPROPERTY()
	int32 httpStatusCode{};

	UPROPERTY()
	FString requestId{};

	UPROPERTY()
	int32 attempts{};

	UPROPERTY()
	double totalRetryDelay{};

	void Dump() const
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("httpStatusCode: %d"), httpStatusCode);
		UE_LOG(LogDedicatedServers, Log, TEXT("requestId: %s"), *requestId);
		UE_LOG(LogDedicatedServers, Log, TEXT("attempts: %d"), attempts);
		UE_LOG(LogDedicatedServers, Log, TEXT("totalRetryDelay: %f"), totalRetryDelay);
	}
};

USTRUCT()
struct FDSListFleetsResponse
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FString> FleetIds;

	UPROPERTY()
	FString NextToken;

	void Dump() const
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("ListFleetsResponse:"));
		for (const FString& FleetId : FleetIds)
		{
			UE_LOG(LogDedicatedServers, Log, TEXT("FleetId: %s"), *FleetId);
		}
		if (!NextToken.IsEmpty())
		{
			UE_LOG(LogDedicatedServers, Log, TEXT("NextToken: %s"), *NextToken);
		}

	}
};
