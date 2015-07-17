// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject2.h"	//Fix Your Project Name
#include "SceneCaptureActor.h"
#include "rgbe.h"

#include "Engine/Canvas.h"
#include "Engine/TextureRenderTarget2D.h"

#include <string>
#include <vector>

#include <algorithm>

// Sets default values
ASceneCaptureActor::ASceneCaptureActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASceneCaptureActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASceneCaptureActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//RenderTextureからUTexture2Dを生成
	//テクスチャサイズは2の乗数かつ正方形である必要がある
	UTexture2D* Aux2DTex = render_tex->ConstructTexture2D(this, "new_texture_name", EObjectFlags::RF_NoFlags, CTF_DeferCompression);

	//テクスチャ圧縮を無効に設定
	Aux2DTex->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
#if WITH_EDITORONLY_DATA
	Aux2DTex->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
#endif
	Aux2DTex->SRGB = 0;
	Aux2DTex->UpdateResource();

	//テクスチャからRAWデータを取得
	FFloat16Color* FormatedImageData = NULL;
	Aux2DTex->PlatformData->Mips[0].BulkData.GetCopy((void**)&FormatedImageData);

	//テクスチャのデータを取得
	int width = Aux2DTex->GetSurfaceWidth();
	int height = Aux2DTex->GetSurfaceHeight();

	std::vector<float> for_rgbe(width*height * 3);

	for (int i = 0; i < width * height; i++){

		FFloat16Color c = FormatedImageData[i];

		for_rgbe[i * 3 + 0] = c.R.GetFloat();
		for_rgbe[i * 3 + 1] = c.G.GetFloat();
		for_rgbe[i * 3 + 2] = c.B.GetFloat();
	}

	//.hdr出力
	char name[2048];
	sprintf(name, "%s%08d.hdr", TCHAR_TO_ANSI(*path),frame);

	rgbe_header_info info;
	info.valid = 1;
	strcpy(info.programtype,"RADIANCE");
	info.gamma = 1.0f;
	info.exposure = 1.0f;

	FILE *f = fopen(name, "wb");
	if(f==NULL){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Output file open failed %s"), name));
		return;
	}
	RGBE_WriteHeader(f, width, height, &info);
	RGBE_WritePixels_RLE(f, &for_rgbe[0], width, height);
	fclose(f);

	//ログ出力
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Output frame %d %dx%d"), frame, width, height));

	//フレームカウントを進める
	frame++;
}

