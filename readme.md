# Sync Status Embed

ESP8266を使用したWebサーバーベースのステータス表示システム

## 概要

このプロジェクトは、ESP8266マイコンを使用してWebサーバーを立ち上げ、HTTP APIを通じてOLEDディスプレイの状態を制御できるシステムです。128x64 OLEDディスプレイに4つのボックスを表示し、0-15の数値をバイナリ形式で視覚的に表現します。各ボックスにはカスタマイズ可能なラベルテキストを設定できます。

## 機能

- **Webサーバー機能**: ESP8266がWiFi接続してWebサーバーとして動作
- **OLEDディスプレイ制御**: 128x64 SSD1309 OLEDディスプレイに4つのボックスを表示
- **ラベル機能**: 各ボックスに日本語対応のカスタマイズ可能なラベルを設定
- **設定保存**: ラベル設定はEEPROMに自動保存

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
   - `documents/openapi.yaml`を開く
   - コマンドパレット（`Ctrl+Shift+P`）を開く
   - `Preview Swagger`を選択
   - ブラウザでSwagger UIが表示されます

### 使用例

```bash
# ボックスを全て非表示 (0000)
curl -X PUT http://syncStatusServer.local/state/0

# ボックス1のみ表示 (0001)
curl -X PUT http://syncStatusServer.local/state/1

# ボックス2のみ表示 (0010)
curl -X PUT http://syncStatusServer.local/state/2

# 全て表示 (1111)
curl -X PUT http://syncStatusServer.local/state/15

# 現在のボックス状態を取得
curl http://syncStatusServer.local/state

# ラベル設定を取得
curl http://syncStatusServer.local/labels

# ラベルを設定（URLエンコードが必要）
curl -X PUT "http://syncStatusServer.local/labels?0=ラベル1&1=ラベル2&2=ラベル3&3=ラベル4"
```

## ハードウェア構成

- **マイコン**: ESP-WROOM-02(ESP8266)
  - 実装には開発ボードである[ESPr® Developer](https://ssci.to/2500)を使用
- **ディスプレイ**: SSD1309 128x64 OLED (I2C接続)
- **接続ピン**:
  - SDA: GPIO 5
  - SCL: GPIO 4
  - VCC: 3.3V
  - GND: GND
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
3. 必要なライブラリをインストール
   - **U8g2**: OLEDディスプレイ制御用
     - ライブラリマネージャーで「U8g2」を検索してインストール

### 2. WiFi設定

`secrets.h.local`を`secrets.h`にリネームorコピーして`STASSID`, `STAPSK`を自身の環境に合わせて設定

### 3. アップロード

1. ESP8266をPCに接続し、書き込みを行う

## 動作確認

1. シリアルモニター(115200 baud)でIPアドレスを確認
2. ブラウザで`http://[IP_ADDRESS]/`にアクセス
3. `http://[IP_ADDRESS]/state/5`でテスト（ボックス1とボックス3が表示される）
4. OLEDディスプレイに4つのボックスとラベルが正しく表示されることを確認

## 詳細仕様

### ディスプレイ表示

- **解像度**: 128x64ピクセル
- **フォント**: 日本語対応フォント（u8g2_font_b12_b_t_japanese3）
- **ボックス配置**: 2x2のグリッド配置
  - ボックス0: 左上 (x=2, y=0)
  - ボックス1: 右上 (x=66, y=0)
  - ボックス2: 左下 (x=2, y=34)
  - ボックス3: 右下 (x=66, y=34)
- **ボックスサイズ**: 60x30ピクセル（角丸半径3ピクセル）

### ラベル機能

- **文字数制限**: 日本語5文字まで（UTF-8エンコード）
- **保存方式**: EEPROM自動保存
- **初期値**: "ラベル1"〜"ラベル4"
- **配置**: 各ボックス内の中央

## 関連
