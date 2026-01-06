# Sync Status Embed

ESP8266を使用したWebサーバーベースの同期ステータス表示システム

## 概要

このプロジェクトは、ESP8266マイコンを使用してWebサーバーを立ち上げ、HTTP APIを通じてLEDの状態を制御できるシステムです。4つのLEDを使用して0-15の数値をバイナリ表示します。

## 機能

- **Webサーバー機能**: ESP8266がWiFi接続してWebサーバーとして動作
- **LED制御**: 4つのLED（ピン12, 13, 14, 15）でバイナリ表示

- HOSTNAME: `syncStatusServer`
- mDNS: `syncStatusServer.local`

## API仕様

### エンドポイント

#### `GET /`

- **説明**: サーバーの生存確認（Hostnameを返す）
- **レスポンス例**: `syncStatusServer`

#### `GET /state`

- **説明**: 現在のstateを返却
- **レスポンス例**: `1`

#### `GET /state/{number}`

- **説明**: LEDの状態を設定（0-15の数値をバイナリ表示）
- **パラメータ**:
  - `number`: 0-15の整数値
- **レスポンス例**:
  - 成功時: `success!`
  - 16以上を指定した場合: `This operation is out of scope.` (HTTP 400)

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
  - LED1: GPIO 12
  - LED2: GPIO 13
  - LED3: GPIO 14
  - LED4: GPIO 15
- **回路図**:
  

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
