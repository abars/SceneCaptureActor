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

(9) SceneCaptureActorのrender_texにSceneRenderTargetをアタッチ (Setting/SceneCaptureActorSetting.pngを参照)

(10) SceneCaptureActorのpathに書き出し先のパスを設定

# 書き出しファイル

.hdrファイルの連番になります。

書き出したファイルのサンプルはsampleディレクトリにあります。

# 制約

SceneRenderTargetのサイズは2の乗数かつ正方形である必要があります。

#依存ライブラリ

.hdrファイルの書き出しには、以下のサイトのrgbe.cとrgbe.hを使用させて頂いています。

http://www.graphics.cornell.edu/online/formats/rgbe/
