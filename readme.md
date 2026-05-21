# sync_status_embed

同期ステータスを表示する ESP8266 ファームウェアです。  
出力デバイスの種類によって **2つのブランチ** に分かれており、それぞれ動作が異なります。

---

## ブランチ構成

| ブランチ            | 対象ハードウェア    | 概要                                |
| ------------------- | ------------------- | ----------------------------------- |
| [`LED版`](#led版)   | LED × 4             | LEDの点灯パターンでバイナリ値を表示 |
| [`OLED版`](#oled版) | SSD1309 128x64 OLED | OLEDにボックスとラベルを表示        |

各ブランチの詳細（セットアップ手順・API仕様・回路図など）はそれぞれのブランチの README を参照してください。

## LED版

**完成品のイメージ**

**詳細**
[`variant/bit-led` ブランチの README](#)

## OLED版

**完成品のイメージ**

**詳細**
[`variant/smart-oled` ブランチの README](#)

---

## 共通事項

- ESP8266 を使用した Webサーバーベースのファームウェア
- HTTP API (`PUT /state/{0-15}`) で出力デバイスの状態を制御
- HOSTNAME: `syncStatusServer` / mDNS: `syncStatusServer.local`

Arduino IDE のセットアップについては [esp8266/Arduino](https://github.com/esp8266/Arduino) を参照してください。
