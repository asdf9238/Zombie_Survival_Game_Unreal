// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "http.h"
#include "HttpModule.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "NPC_OpenAI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatGPTResponse, FString, ResponseText);

UCLASS()
class UNREALPROJECT_GAMEAI_API ANPC_OpenAI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPC_OpenAI();

	UFUNCTION(BlueprintCallable, Category = "OpenAI")
	void SendMessageToChatGPT(const FString& Message);

	UPROPERTY(BlueprintAssignable, Category = "OpenAI")
	FOnChatGPTResponse OnResponseReceived;

private:
	void OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	FString APIKey;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
