// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SceneCaptureActor.generated.h"

UCLASS()
class MYPROJECT2_API ASceneCaptureActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASceneCaptureActor();

	/* 録画対象となるSceneCapture2Dから書き出したRenderTargetを接続します */
	UPROPERTY(EditAnywhere)
	UTextureRenderTarget2D *render_tex;

	/* .hdrファイルの書き出し先のパスを設定します、書き出し先はpath%08d.hdrとなります */
	UPROPERTY(EditAnywhere)
	FString path;

	/* 一時的に録画を止める場合はtrueに設定します */
	UPROPERTY(EditAnywhere)
	bool pause=false;

	/* RGBではなくアルファチャンネルをキャプチャして書き出す場合はtrueに設定します */
	UPROPERTY(EditAnywhere)
	bool capture_alpha=false;

	int frame = 0;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
