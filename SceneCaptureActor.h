// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SceneCaptureActor.generated.h"

UCLASS()
class MYPROJECT2_API ASceneCaptureActor : public AActor
{
	GENERATED_BODY()

private:
	// Capture One Frame
	void capture(char *name,FFloat16Color *FormatedImageData,int width,int height,int face_n);

	/* 書き出しフレーム番号 */
	int frame = 0;

public:	
	// Sets default values for this actor's properties
	ASceneCaptureActor();

	/* 録画対象となるSceneCapture2Dから書き出したRenderTargetを接続します */
	UPROPERTY(EditAnywhere)
	UTextureRenderTarget2D *render_tex;

	/* .hdrファイルの書き出し先のパスを設定します、書き出し先はpath%08d.hdrとなります */
	UPROPERTY(EditAnywhere)
	FString path;

	/* 録画対象となるSceneCaptureCubeから書き出したRenderTargetを接続します */
	UPROPERTY(EditAnywhere)
	UTextureRenderTargetCube *render_tex_cube;

	/* .hdrファイルの書き出し先のパスを設定します、書き出し先はpath_cube%08d.hdrとなります */
	UPROPERTY(EditAnywhere)
	FString path_cube;

	/* 一時的に録画を止める場合はtrueに設定します */
	UPROPERTY(EditAnywhere)
	bool pause=false;

	/* RGBではなくアルファチャンネルをキャプチャして書き出す場合はtrueに設定します */
	UPROPERTY(EditAnywhere)
	bool capture_alpha=false;

	/* 画像を書き出す際に削除する左画素数を指定します */
	UPROPERTY(EditAnywhere)
	int32 crop_left=0;

	/* 画像を書き出す際に削除する右画素数を指定します */
	UPROPERTY(EditAnywhere)
	int32 crop_right=0;

	/* 画像を書き出す際に削除する上画素数を指定します */
	UPROPERTY(EditAnywhere)
	int32 crop_top=0;

	/* 画像を書き出す際に削除する下画素数を指定します */
	UPROPERTY(EditAnywhere)
	int32 crop_bottom=0;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
