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
	
	//for fixed step
	//FApp::SetBenchmarking(true);
	//FApp::SetFixedDeltaTime(1/60);
}

// Capture One Frame
void ASceneCaptureActor::capture(char *name,FFloat16Color *FormatedImageData,int width,int height,int face_n)
{
	//例外処理
	if(width<=0 || height<=0){
		return;
	}

	//画素を取得
	const int RGB_UNIT=3;
	int stride=width*face_n;
	std::vector<float> for_rgbe(stride*height*RGB_UNIT);

	for (int f = 0; f < face_n ; f++){
		for (int y = 0; y < height ; y++){
			for (int x = 0; x < width; x++){
				FFloat16Color c = FormatedImageData[y*width+x+f*width*height];
				int adr=(y*stride+x+width*f)*RGB_UNIT;

				if(capture_alpha){
					for_rgbe[adr + 0] = c.A.GetFloat();
					for_rgbe[adr + 1] = c.A.GetFloat();
					for_rgbe[adr + 2] = c.A.GetFloat();
				}else{
					for_rgbe[adr + 0] = c.R.GetFloat();
					for_rgbe[adr + 1] = c.G.GetFloat();
					for_rgbe[adr + 2] = c.B.GetFloat();
				}
			}
		}
	}

	//サイズ補正
	width*=face_n;

	//クロップ
	int crop_width=width;
	int crop_height=height;
	float *p_rgbe=&for_rgbe[0];
	std::vector<float> crop_rgbe;

	if(face_n==1){
		crop_width=width-crop_left-crop_right;
		crop_height=height-crop_top-crop_bottom;
		if(crop_width<0 || crop_height<0){
			return;
		}

		crop_rgbe.resize(crop_width*crop_height * RGB_UNIT);
		for(int y=0;y<crop_height;y++){
			for(int x=0;x<crop_width;x++){
				int dst_adr=(crop_width*y+x)*RGB_UNIT;
				int src_adr=(stride*(y+crop_top)+(x+crop_left))*RGB_UNIT;
				crop_rgbe[dst_adr + 0] = for_rgbe[src_adr + 0];
				crop_rgbe[dst_adr + 1] = for_rgbe[src_adr + 1];
				crop_rgbe[dst_adr + 2] = for_rgbe[src_adr + 2];
			}
		}

		p_rgbe=&crop_rgbe[0];
	}

	//.hdr出力
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
	RGBE_WriteHeader(f, crop_width, crop_height, &info);
	RGBE_WritePixels_RLE(f, p_rgbe, crop_width, crop_height);
	fclose(f);
}

// Called every frame
void ASceneCaptureActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//録画をポーズする
	if(pause){
		return;
	}

	//2DとCubeを順次書き出し
	for(int is_cube=0;is_cube<2;is_cube++){
		FFloat16Color* FormatedImageData = NULL;
		int width,height,face_n;
		char name[2048];

		if(is_cube==0){
			//RenderTextureからUTexture2Dを生成
			//テクスチャサイズは2の乗数かつ正方形である必要がある
			if(!render_tex){
				continue;
			}
			UTexture2D* Aux2DTex = render_tex->ConstructTexture2D(this, "new_texture_name_2d", EObjectFlags::RF_NoFlags, CTF_DeferCompression);
			if(!Aux2DTex){
				continue;
			}

			//テクスチャ圧縮を無効に設定
			Aux2DTex->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		#if WITH_EDITORONLY_DATA
			Aux2DTex->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
		#endif
			Aux2DTex->SRGB = 0;
			Aux2DTex->UpdateResource();

			//テクスチャからRAWデータを取得
			Aux2DTex->PlatformData->Mips[0].BulkData.GetCopy((void**)&FormatedImageData);

			//テクスチャのサイズを取得
			width = Aux2DTex->GetSurfaceWidth();
			height = Aux2DTex->GetSurfaceHeight();
			face_n = 1;

			//One shot
			sprintf(name, "%s%08d.hdr", TCHAR_TO_ANSI(*path),frame);
			capture(name,FormatedImageData,width,height,face_n);
		}else{
			//RenderTextureCubeからテクスチャの生成
			if(!render_tex_cube){
				continue;
			}
			UTextureCube* AuxCubeTex = render_tex_cube->ConstructTextureCube(this, "new_texture_name_cube", EObjectFlags::RF_NoFlags);
			if(!AuxCubeTex){
				continue;
			}

			//テクスチャからRAWデータを取得
			AuxCubeTex->PlatformData->Mips[0].BulkData.GetCopy((void**)&FormatedImageData);

			//テクスチャのサイズを取得
			width = AuxCubeTex->GetSurfaceWidth();
			height = AuxCubeTex->GetSurfaceHeight();
			face_n = 6;

			//One shot
			sprintf(name, "%s%08d.hdr", TCHAR_TO_ANSI(*path_cube),frame);
			capture(name,FormatedImageData,width,height,face_n);
		}
	}

	//ログ出力
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Output frame %d"), frame));

	//フレームカウントを進める
	frame++;
}

