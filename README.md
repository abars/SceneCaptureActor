# SceneCaptureActor
Export HDR movie from Scene Capture 2D of Unreal Engine 4

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

![Setting/SceneCaptureActorSetting.pngを参照](https://github.com/abars/SceneCaptureActor/blob/master/setting/SceneCaptureActorSetting.png)

(10) SceneCaptureActorのpathに書き出し先のパスを設定

# 書き出しファイル

.hdrファイルの連番になります。

書き出したファイルのサンプルはsampleディレクトリにあります。

# 深度値の書き出し

深度値を書き出したい場合、Post Process MaterialをSceneCapture2Dに適用します。

(1) 新規追加 -> マテリアル -> マテリアル(SceneDepth)を追加

(2) SceneDepthをダブルクリック、Material DomainをPost Processに設定、SceneDepth UVsを適当な定数で割ってエミッションカラーに接続 (Setting/SceneDepthMaterial.pngを参照)

(3) Scene Capture 2DのMiscのBlendablesにSceneDepthを設定 (Setting/SetDepthPostEffect.pngを参照)

(4) Scene Capture 2DのCapture SourceをFinal Color (LDR with Post Process)に設定

# 制約

SceneRenderTargetのサイズは2の乗数かつ正方形である必要があります。

#依存ライブラリ

.hdrファイルの書き出しには、以下のサイトのrgbe.cとrgbe.hを使用させて頂いています。

http://www.graphics.cornell.edu/online/formats/rgbe/
