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

	UPROPERTY(EditAnywhere)
	UTextureRenderTarget2D *render_tex;

	UPROPERTY(EditAnywhere)
	FString path;

	int frame = 0;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
