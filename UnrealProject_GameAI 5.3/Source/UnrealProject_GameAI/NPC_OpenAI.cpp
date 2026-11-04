// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_OpenAI.h"
#include "Json.h"
#include "JsonUtilities.h"

// Sets default values
ANPC_OpenAI::ANPC_OpenAI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	APIKey = TEXT("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");
}

// Called when the game starts or when spawned
void ANPC_OpenAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC_OpenAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANPC_OpenAI::SendMessageToChatGPT(const FString& Message) {
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &ANPC_OpenAI::OnResponse);
    Request->SetURL(TEXT("https://api.openai.com/v1/chat/completions"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *APIKey));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    JsonObject->SetStringField(TEXT("model"), TEXT("gpt-3.5-turbo"));

    TArray<TSharedPtr<FJsonValue>> Messages;
    TSharedPtr<FJsonObject> MessageObject = MakeShareable(new FJsonObject());
    MessageObject->SetStringField(TEXT("role"), TEXT("user"));
    MessageObject->SetStringField(TEXT("content"), Message);
    Messages.Add(MakeShareable(new FJsonValueObject(MessageObject)));
    JsonObject->SetArrayField(TEXT("messages"), Messages);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    Request->SetContentAsString(RequestBody);
    Request->ProcessRequest();
}

void ANPC_OpenAI::OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
    if (bWasSuccessful) {
        UE_LOG(LogTemp, Warning, TEXT("bWasSuccessful true"));
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("bWasSuccessful false"));
    }

    if (Response->GetResponseCode() != 200) {
        UE_LOG(LogTemp, Warning, TEXT("Response Code Not 200"));
    }
    if (bWasSuccessful && Response->GetResponseCode() == 200)
    {
        UE_LOG(LogTemp, Warning, TEXT("Received successful response from ChatGPT API."));

        TSharedPtr<FJsonObject> JsonResponse;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

        if (FJsonSerializer::Deserialize(Reader, JsonResponse))
        {
            const TArray<TSharedPtr<FJsonValue>>* Choices;
            if (JsonResponse->TryGetArrayField(TEXT("choices"), Choices) && Choices->Num() > 0)
            {
                FString Content = (*Choices)[0]->AsObject()->GetObjectField("message")->GetStringField("content");
                UE_LOG(LogTemp, Warning, TEXT("Parsed response content: %s"), *Content);
                OnResponseReceived.Broadcast(Content);  // 응답을 브로드캐스트하여 블루프린트에 전달
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("No choices field found in JSON response."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get response from ChatGPT API. StatusCode: %d"), Response->GetResponseCode());
        OnResponseReceived.Broadcast(TEXT("Error: Failed to get response from ChatGPT"));
    }
}
