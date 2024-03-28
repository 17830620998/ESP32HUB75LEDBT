/*
#define R1 21
#define G1 22
#define BL1 27
#define R2 14
#define G2 12
#define BL2 13
#define CH_A 23
#define CH_B 19
#define CH_C 5
#define CH_D 17
#define CH_E -1 // assign to any available pin if using two panels or 64x64 panels with 1/32 scan
#define CLK 16
#define LAT 4
#define OE 15
*/

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>
#include "MyFont.h"
#include "Arduino_GB2312_library.h"
#include "BluetoothA2DPSink.h"

BluetoothA2DPSink a2dp_sink;

boolean isnight = 0;
boolean twopannel = 0;

#define PANEL_WIDTH 64
#define PANEL_HEIGHT 64
#define PANELS_NUMBER 1
#define PANE_WIDTH PANEL_WIDTH *PANELS_NUMBER
#define PANE_HEIGHT PANEL_HEIGHT

MatrixPanel_I2S_DMA *dma_display = nullptr;

//定义显示数组
// uint16_t ledtab[128][64];
uint16_t ledtab[64][64];
int screen_num = 0;  //显示控制，让元素在两个屏幕上跳动
int hou, minu, sec, year1, month1, day1, week, sec_ten, sec_one;

uint16_t color = 0xf0b0;   //时间
uint16_t color2 = 0x780F;  //日期
uint16_t color3 = 0xf000;  //农历
uint16_t color4 = 0xfff0;  //温度
uint16_t color5 = 0xFDA0;
uint16_t colorl = 0xff00;  //滚动条
uint16_t Rs = 0xF800;
uint16_t Ls = 0x07E0;
uint16_t Bs = 0xFFFF;

void connection_state_changed(esp_a2d_connection_state_t state, void *ptr) {
  Serial.println(a2dp_sink.to_str(state));
}

void audio_state_changed(esp_a2d_audio_state_t state, void *ptr) {
  Serial.println(a2dp_sink.to_str(state));
}

void avrc_metadata_callback(uint8_t data1, const uint8_t *data2) {
  if (data1 == 1) {
    Serial.print("歌名：");
    Serial.println(reinterpret_cast<const char *>(data2));
    clearOLED();
    drawHanziS(4, 0, reinterpret_cast<const char *>(data2), Rs);
    fillScreenTab();
    cleanTab();
  }
  if (data1 == 4) {
    Serial.print("歌手：");
    Serial.println(reinterpret_cast<const char *>(data2));
  }
  if (data1 == 2) {
    Serial.print("专辑：");
    Serial.println(reinterpret_cast<const char *>(data2));
  }
}


String getStringFromChars(const char *data, size_t size) {
  String result;
  for (size_t i = 0; i < size; i++) {
    result += data[i];
  }
  return result;
}

void setup() {
  Serial.begin(115200);
  initOLED();
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
    .sample_rate = 44100,                         // corrected by info from bluetooth
    .bits_per_sample = (i2s_bits_per_sample_t)16, /* the DAC module will only take the 8bits from MSB */
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_STAND_MSB,
    .intr_alloc_flags = 0,  // default interrupt priority
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
  };
  a2dp_sink.set_i2s_config(i2s_config);
  a2dp_sink.set_on_connection_state_changed(connection_state_changed);
  a2dp_sink.set_on_audio_state_changed(audio_state_changed);
  a2dp_sink.set_avrc_metadata_attribute_mask(ESP_AVRC_MD_ATTR_TITLE | ESP_AVRC_MD_ATTR_ARTIST | ESP_AVRC_MD_ATTR_ALBUM | ESP_AVRC_MD_ATTR_TRACK_NUM | ESP_AVRC_MD_ATTR_NUM_TRACKS);
  a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
  a2dp_sink.start("MyMusic");
  clearOLED();
  drawText("Hello World", 0, 0);
  delay(2000);
  clearOLED();
}

void loop() {
}
