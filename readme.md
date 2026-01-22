# Sync Status Embed

ESP8266を使用したWebサーバーベースのステータス表示システム

## 概要

このプロジェクトは、ESP8266マイコンを使用してWebサーバーを立ち上げ、HTTP APIを通じてLEDの状態を制御できるシステムです。4つのLEDを使用して0-15の数値をバイナリ表示します。

## 機能

- **Webサーバー機能**: ESP8266がWiFi接続してWebサーバーとして動作
- **LED制御**: 4つのLED（ピン12, 13, 14, 15）でバイナリ表示

- HOSTNAME: `syncStatusServer`
- mDNS: `syncStatusServer.local`

## API仕様

### エンドポイント

詳細なAPI仕様については、[documents/openapi.yaml](documents/openapi.yaml)を参照してください。

#### ローカルでSwagger UIを表示する方法

1. VS Codeの拡張機能「Swagger Viewer」をインストール
   - 拡張機能IDから検索: `Arjun.swagger-viewer`
2. VS Codeの設定で「Preview In Browser」を有効化
   - 設定（`Ctrl+,`）を開く
   - `swagger viewer`で検索
   - 「Preview In Browser」にチェックを入れる
3. Swagger UIでプレビュー
   - `documents/swagger.yaml`を開く
   - コマンドパレット（`Ctrl+Shift+P`）を開く
   - `Preview Swagger`を選択
   - ブラウザでSwagger UIが表示されます

### 使用例

```bash
# LEDを全て消灯 (0000)
curl http://syncStatusServer.local/state/0

# LED1のみ点灯 (0001)
curl http://syncStatusServer.local/state/1

# LED2のみ点灯 (0010)
curl http://syncStatusServer.local/state/2

# 全て点灯 (1111)
curl http://syncStatusServer.local/state/15
```

## ハードウェア構成

- **マイコン**: ESP-WROOM-02(ESP8266)
  - 実装には開発ボードである[ESPr® Developer](https://ssci.to/2500)を使用
- **LED**: 4個
- **抵抗(330Ω~2kΩ)**: 4個
- **接続ピン**:
  - LED1: GPIO 14
  - LED2: GPIO 15
  - LED3: GPIO 12
  - LED4: GPIO 13
- **回路図**:

![回路図](/images/diagram.png "diagram")

## セットアップ

### 1. 必要なライブラリ

1. Arduino IDEでボードのセットアップ
   1. <https://github.com/esp8266/Arduino>を参照
2. ボード選択後、「ツール」を開き以下設定値を変更

  |       項目        |         設定値         |
  | ----------------- | ---------------------- |
  | Crystal Frequency | 26 MHz                 |
  | Flash Size        | 2MB(FS:1MB OTA:~512KB) |
  | Flash Frequency   | 80 MHz                 |
  | Flash Mode        | QIO(fast)              |
  | CPU Frequency     | 160 MHz                |

### 2. WiFi設定

`secrets.h.local`を`secrets.h`にリネームorコピーして`STASSID`, `STAPSK`を自身の環境に合わせて設定

### 3. アップロード

1. ESP8266をPCに接続し、書き込みを行う

## 動作確認

1. シリアルモニター(115200 baud)でIPアドレスを確認
2. ブラウザで`http://[IP_ADDRESS]/`にアクセス
3. `http://[IP_ADDRESS]/actions/5`でテスト（LED1とLED3が点灯）

## 関連
