// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HTTP/HTTPRequestManager.h"

#include "JsonObjectConverter.h"
#include "DedicatedServers/DedicatedServers.h"
#include "UI/HTTP/HTTPRequestTypes.h"

bool UHTTPRequestManager::ContainsError(const TSharedPtr<FJsonObject>& JsonObject) const
{
	if (JsonObject->HasField(TEXT("errorType")) || JsonObject->HasField(TEXT("errorMessage")))
	{
		FString ErrorType = JsonObject->HasField(TEXT("errorType")) ? JsonObject->GetStringField(TEXT("errorType")) : TEXT("未知错误");
		FString ErrorMessage = JsonObject->HasField(TEXT("errorMessage")) ? JsonObject->GetStringField(TEXT("errorMessage")) : TEXT("未知错误信息");
		UE_LOG(LogDedicatedServers, Error, TEXT("错误类型: %s"), *ErrorType);
		UE_LOG(LogDedicatedServers, Error, TEXT("错误信息: %s"), *ErrorMessage);
		return true;
	}
	if (JsonObject->HasField(TEXT("$fault")))
	{
		FString ErrorType = JsonObject->HasField(TEXT("name")) ? JsonObject->GetStringField(TEXT("name")) : TEXT("未知错误");
		UE_LOG(LogDedicatedServers, Error, TEXT("Error Type: %s"), *ErrorType);
		return true;
	}
	return false;
}

void UHTTPRequestManager::DumpMetaData(const TSharedPtr<FJsonObject>& JsonObject) const
{
	if (JsonObject->HasField(TEXT("$metadata")))
	{
		TSharedPtr<FJsonObject> MetaDataJsonObject = JsonObject->GetObjectField(TEXT("$metadata"));
		FDMetaData MetaData;
		FJsonObjectConverter::JsonObjectToUStruct(MetaDataJsonObject.ToSharedRef(), &MetaData, 0, 0);
		MetaData.Dump();
	}
}
