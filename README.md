# SceneCaptureActor
Export HDR movie from Scene Capture 2D on Unreal Engine 4

# 概要

Unreal Engine 4を使用して各フレームの動画を.hdrファイルに書き出すためのプラグインです。
Unreal Engine 4のMatineではLDR動画しか書き出せませんが、SceneCaptureActorを使用することで、.hdrファイルに書き出すことができます。

# 使用方法

(1) プロジェクトのソースコードにSceneCaptureActorを追加

(2) SceneCaptureActor.hのMYPROJECT2_APIをプロジェクトに応じて書き換え

(3) SceneCaptureActor.cppとrgbe.cppの
    #include "MyProject2.h"
    をプロジェクトに応じて書き換え

(4) コンパイル



(5) 全てのクラス -> SceneCapture2Dを配置

(6) 新規追加 -> マテリアル -> 描画ターゲット(SceneRenderTarget)を追加

(7) SceneCapture2DのTextureTargetにSceneRenderTargetをアタッチ



(8) SceneCaptureActorをシーンに配置

(9) SceneCaptureActorのrender_texにSceneRenderTargetをアタッチ

(10) SceneCaptureActorのpathに書き出し先のパスを設定

![SceneCaptureActor setting](https://github.com/abars/SceneCaptureActor/blob/master/setting/SceneCaptureActorSetting.png)

# 書き出しファイル

.hdrファイルの連番になります。

書き出したファイルのサンプルはsampleディレクトリにあります。

# 深度値の書き出し

深度値を書き出したい場合、Post Process MaterialをSceneCapture2Dに適用します。

(1) 新規追加 -> マテリアル -> マテリアル(SceneDepth)を追加

(2) SceneDepthをダブルクリック、Material DomainをPost Processに設定、SceneDepth UVsを適当な定数で割ってエミッシブカラーに接続

![SceneDepth setting](https://github.com/abars/SceneCaptureActor/blob/master/setting/SceneDepthMaterial.png)

(3) Scene Capture 2DのMiscのBlendablesにSceneDepthを設定

(4) Scene Capture 2DのCapture SourceをFinal Color (LDR with Post Process)に設定

![PostProcess Depth setting](https://github.com/abars/SceneCaptureActor/blob/master/setting/SetDepthPostEffect.png)

# 法線の書き出し

法線を書き出したい場合も、深度値と同様に、Post Process Materialを使用します。

Material EditorでSceneTexture:World Normalをエミッシブカラーに接続して下さい。

![PostProcess Normal setting](https://github.com/abars/SceneCaptureActor/blob/master/setting/SetNormalPostEffect.png)

# 固定フレームレートでの書き出し

Unreal Engineは、BENCHMARKオプションを付けて実行することで、フレームレートを固定することができます。

"UE4Editor.exe" "(.uprojectのフルパス)" -ResX=1920 -ResY=1080 -FPS=60 -BENCHMARK

BENCHMARKオプションの詳細は以下にあります。

"BENCHMARK: Run game at fixed-step in order to process each frame without skipping any frames. This is useful in conjunction with DUMPMOVIE options."

https://docs.unrealengine.com/latest/INT/Programming/Basics/CommandLineArguments/index.html

また、フレームレートの固定は、C++ファイルのBeginPlayで以下のように記述しても行うことができます。

FApp::SetBenchmarking(true);

FApp::SetFixedDeltaTime(1/60);

# 解像度制約

SceneRenderTargetのサイズは2の乗数かつ正方形である必要があります。

また、Unreal Engineの制約から、出力の最大サイズは2048x2048となります。

crop_left , crop_right , crop_top , crop_bottomをpublic変数に追加するかどうか検討中です。

# キューブマップの書き出しについて

UTextureCubeの書き出し方法を調査中です。

# 実行環境制約

SceneCaptureActorが使用している、ConstructTexture2D APIの実装に、#if WITH_EDITORが含まれているため、-gameでは動作しません。

#依存ライブラリ

.hdrファイルの書き出しには、以下のサイトのrgbe.cとrgbe.hを使用させて頂いています。

http://www.graphics.cornell.edu/online/formats/rgbe/
