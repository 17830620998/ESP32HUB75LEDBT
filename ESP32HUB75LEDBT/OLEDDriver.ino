uint16_t myBLACK = dma_display->color565(0, 0, 0);        // 定义黑色
uint16_t myWHITE = dma_display->color565(255, 255, 255);  // 定义白色
uint16_t myRED = dma_display->color565(255, 0, 0);        // 定义红色
uint16_t myGREEN = dma_display->color565(0, 255, 0);      // 定义绿色
uint16_t myBLUE = dma_display->color565(0, 0, 255);       // 定义蓝色
void cleanTab() {                                         // 清空屏幕缓冲区
  for (int i = 0; i < 64; i++) {
    for (int j = 0; j < 64; j++) {
      ledtab[i][j] = 0x0000;
    }
  }
}
void fillTab(int x, int y, uint16_t color) {  // 填充屏幕缓冲区指定位置
  if (!isnight) {                             // 如果不是夜晚模式
    ledtab[x][y] = color;
  } else {                  // 如果是夜晚模式
    ledtab[x][y] = 0x8800;  // 使用较暗的颜色填充
  }
}
void fillScreenTab() {  // 将屏幕缓冲区内容显示到LED面板
  //  for (int i = 0; i < 64; i++) {
  //    ledtab[64][0 + i] = TFT_LIGHTGREY; //分隔线
  //  }
  if (twopannel) {  // 如果是双面板模式
    if (minu % 2 == 0 && !isnight) {
      screen_num = 0;
    } else if (!isnight) {
      screen_num = 64;
    }
  } else {
    screen_num = 0;
  }
  for (int i = 0; i < 64; i++) {
    for (int j = 0; j < 64; j++) {
      if (twopannel) {
        if (i < 64) {
          dma_display->drawPixel(i + screen_num, j, ledtab[i][j]);
        } else {
          dma_display->drawPixel(i - screen_num, j, ledtab[i][j]);
        }
      } else {
        dma_display->drawPixel(i + screen_num, j, ledtab[i][j]);
      }
    }
  }
}
void fillCircle(int x, int y, int r, int color) {  // 绘制实心圆
  for (int i = x - r; i < x + r; i++) {
    for (int j = y - r; j < y + r; j++) {
      if (((i - x) * (i - x) + (j - y) * (j - y)) <= (r * r)) {
        fillTab(i, j, color);
      }
    }
  }
}
void drawBit(int x, int y, const uint8_t *bitmap, int width, int height, uint16_t color) {  // 绘制位图

  int32_t i, j, byteWidth = (width + 7) / 8;

  for (j = 0; j < height; j++) {
    for (i = 0; i < width; i++) {
      if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
        //dma_display->drawPixel(x + i, y + j, color);
        fillTab(x + i, y + j, color);
      }
    }
  }
}
void drawLine(int x0, int y0, int x1, int sec) {  //绘制线段
  for (int i = x0; i < x1; i++) {                 //横向
    colorl = rand() % 0xffff;
    if (!isnight) {
      //dma_display->drawPixel(x0 + i, y0, colorl);
      fillTab(x0 + i, y0, colorl);
    }
  }
}
void drawHLine(int x0, int y0, int y1, int sec) {  //绘制水平线段
  for (int i = 0; i < y1; i++) {                   //横向
    colorl = rand() % 0xffff;
    if (!isnight) {
      //dma_display->drawPixel(x0, y0+i, colorl);
      fillTab(x0, y0 + i, colorl);
    }
  }
}
void drawBit2(int x, int y, const uint8_t *bitmap, int width, int height, uint16_t color) {  // 绘制位图2

  int32_t i, j, byteWidth = (width + 7) / 8;
  int first = 0;
  for (j = 0; j < height; j++) {
    for (i = 0; i < width; i++) {
      if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
        if (first == 0 & !isnight) {
          //dma_display->drawPixel(x + i, y + j, color+TFT_SILVER);
          fillTab(x + i, y + j, color + TFT_SILVER);
        } else {
          //dma_display->drawPixel(x + i, y + j, color);
          fillTab(x + i, y + j, color);
        }
        first++;
      }
    }
    first = 0;
  }
}
void drawSmBit(int x, int y, const uint8_t *bitmap, int width, int height, uint16_t color) {  // 绘制小位图
  int32_t i, j;
  unsigned char a[3];
  unsigned char temp;
  for (j = 0; j < height; j++) {
    temp = bitmap[j];
    for (i = width; i > 0; i--) {
      a[i - 1] = temp & 0x01;
      temp >>= 1;
      if (a[i - 1] == 1) {
        // dma_display->drawPixel(x + i, y + j, color);
        fillTab(x + i, y + j, color);
      }
    }
  }
}
void display30Number(int c, int x, int y, uint16_t color) {  // 显示30像素高的数字
  int hz_width;
  for (int k = 0; k < 10; k++) {
    if (shuzi30[k].Index == c) {
      drawBit(x, y, shuzi30[k].sz30_Id, 15, 30, color);
    }
  }
}
void display30Numbers(int numbers, int x, int y, uint16_t color) {  // 显示数字
  int count = 0;
  int temp = 0;
  char buffer[32];
  if (numbers == 0) {
    display30Number(numbers, x, y, color);
  }
  while (numbers) {
    temp = numbers % 10;
    if (numbers < 10) {
      temp = numbers;
    }

    display30Number(temp, x - (count * 16), y, color);
    numbers = numbers / 10;
    count++;  //count表示val是一个几位数
  }
}
void drawColorBit3(int x, int y, const uint16_t *bitmap, int width, int height) {  // 绘制位图

  int32_t i, j, byteWidth = (width + 7) / 8;

  for (j = 0; j < height; j++) {
    for (i = 0; i < width; i++) {
      if (bitmap[i + j * width] != 0) {
        dma_display->drawPixel(x + i, y + j, bitmap[i + j * width]);
      }
    }
  }
}
void drawColorBit(int x, int y, const uint16_t *bitmap, int width, int height) {  // 绘制位图

  int32_t i, j, byteWidth = (width + 7) / 8;

  for (j = 0; j < height; j++) {
    for (i = 0; i < width; i++) {
      if (bitmap[i + j * width] != 0) {
        fillTab(x + i, y + j, bitmap[i + j * width]);
      }
    }
  }
}
void drawColorBit2(int x, int y, const uint16_t *bitmap, int width, int height) {  // 绘制位图

  int32_t i, j, byteWidth = (width + 7) / 8;

  for (j = 0; j < height; j++) {
    for (i = 0; i < width; i++) {
      if (bitmap[i + j * width] != 0) {
        fillTab(x + i, y + j, bitmap[i + j * width]);
      }
    }
  }
}
void showTQ(int c, int x, int y) {  //显示天气图标
  int hz_width;
  for (int k = 0; k < 61; k++) {
    if (tq20[k].Index == c) {
      drawColorBit2(x, y, tq20[k].tq20_Id, 20, 20);
    }
  }
}
void drawText(String words, int x, int y) {  //绘制文本
  dma_display->setCursor(x, y);
  dma_display->setTextColor(dma_display->color565(255, 255, 0));
  dma_display->print(words);
}
void clearOLED() {  //清除LED屏幕上的内容
  dma_display->clearScreen();
}

void initOLED() {  //初始化

  // Module configuration
  HUB75_I2S_CFG mxconfig(
    PANEL_WIDTH,   // module width
    PANEL_HEIGHT,  // module height
    PANELS_NUMBER  // Chain length
  );
  mxconfig.gpio.e = 18;
  mxconfig.clkphase = false;
  mxconfig.driver = HUB75_I2S_CFG::FM6126A;

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(192);  //0-255
  dma_display->clearScreen();
  dma_display->fillScreen(myBLACK);
}
// void setBrightness(int dianya) {
//   dma_display->setBrightness8(light*dianya); //0-255
// }
void displayNumber(int c, int x, int y, uint16_t color) {  //显示数字或小数字
  int hz_width;
  for (int k = 0; k < 10; k++) {
    if (shuzi24[k].Index == c) {
      drawBit2(x, y, shuzi24[k].hz24_Id, 12, 24, color);
    }
  }
}
void disSmallNumber(int c, int x, int y, uint16_t color) {  //指定位置显示一个小号数字
  int hz_width;
  for (int k = 0; k < 10; k++) {
    if (smsz[k].Index == c) {
      //  Serial.println("run here");
      drawSmBit(x, y, smsz[k].smsz_Id, 3, 5, color);
    }
  }
}
void disSmallNumbers(int numbers, int x, int y, uint16_t color) {  //指定位置连续显示一个多位数
  int count = 0;
  int temp = 0;
  char buffer[32];
  if (numbers == 0) {
    disSmallNumber(numbers, x, y, color);
  }
  while (numbers) {
    temp = numbers % 10;
    if (numbers < 10) {
      temp = numbers;
    }

    disSmallNumber(temp, x - (count * 4), y, color);
    numbers = numbers / 10;
    count++;  //count表示val是一个几位数
  }
}
void dis30Number(int c, int x, int y, uint16_t color) {  //显示一个大号数字
  int hz_width;
  for (int k = 0; k < 10; k++) {
    if (shuzi30[k].Index == c) {
      //  Serial.println("run here");
      drawBit(x, y, shuzi30[k].sz30_Id, 30, 60, color);
    }
  }
}
void dis30Numbers(int numbers, int x, int y, uint16_t color) {  //显示一个多位数 numbers 的大号数字
  int count = 0;
  int temp = 0;
  char buffer[32];
  if (numbers == 0) {
    dis30Number(numbers, x, y, color);
  }
  while (numbers) {
    temp = numbers % 10;
    if (numbers < 10) {
      temp = numbers;
    }

    dis30Number(temp, x - (count * 30), y, color);
    numbers = numbers / 10;
    count++;  //count表示val是一个几位数
  }
}
void displayNumbers(int numbers, int x, int y, uint16_t color) {  //但是用于显示大号数字的另一种实现
  int count = 0;
  int temp = 0;
  char buffer[32];
  if (numbers == 0) {
    displayNumber(numbers, x, y, color);
  }
  while (numbers) {
    temp = numbers % 10;
    if (numbers < 10) {
      temp = numbers;
    }

    displayNumber(temp, x - (count * 12), y, color);
    numbers = numbers / 10;
    count++;  //count表示val是一个几位数
  }
}
void disSmallChar(char c, int x, int y, uint16_t color) {  //显示一个小号字符
  int hz_width;
  for (int k = 0; k < 11; k++) {
    if (smchar[k].Index == c) {
      //Serial.println("run here!DrawChar");
      drawSmBit(x, y, smchar[k].smchar_Id, 3, 5, color);
    }
  }
}
void drawChars(int32_t x, int32_t y, const char str[], uint32_t color) {  //连续显示一个字符串 str 中的字符
  int x0 = x;
  for (int i = 0; i < strlen(str); i++) {
    // Serial.print("当前char:");
    // Serial.println(str[i]);
    disSmallChar(str[i], x0, y, color);
    x0 += 4;
  }
}
void displayNumber2(int c, int x, int y, uint16_t color) {  //显示一个小号数字
  int hz_width;
  for (int k = 0; k < 10; k++) {
    if (shuzi14[k].Index == c) {
      drawBit(x, y, shuzi14[k].hz14_Id, 8, 14, color);
    }
  }
}
void displayNumbers2(int numbers, int x, int y, uint16_t color) {  //显示小号数字的另一种实现
  int count = 0;
  int temp = 0;
  char buffer[32];
  if (numbers == 0) {
    displayNumber2(numbers, x, y, color);
  }
  while (numbers) {
    temp = numbers % 10;
    if (numbers < 10) {
      temp = numbers;
    }

    displayNumber2(temp, x - (count * 7), y, color);
    numbers = numbers / 10;
    count++;  //count表示val是一个几位数
  }
}
void drawHz(int xx, int yy, unsigned char *names, uint32_t color) {  //绘制一个汉字字符
  int a = 24;
  unsigned char buffs[a];
  //建立缓存空间
  getfont(names, sizeof(names), buffs);
  int kj = 0;
  int x = 0;
  int y = 0;
  for (int i = 0; i < a; i++) {
    for (int s = 7; s >= 0; s--) {
      if (buffs[i] & (0x01 << s)) {
        if (7 - s + xx >= 0) {
          if (i % 2 == 0) {
            fillTab(7 - s + xx, y + yy, color);
          } else {
            fillTab(15 - s + xx, y + yy, color);
          }
        }
      }
    }
    if ((i + 1) % 2 == 0) {
      y += 1;
    }
  }
}

void drawHanziS(int32_t x, int32_t y, const char str[], uint32_t color) {
  int str_length = strlen(str);
  int max_chars_per_line = 15;                                                 // 每行最多显示的汉字数量
  int line_height = 12;                                                        // 行高
  int max_lines = (str_length + max_chars_per_line - 1) / max_chars_per_line;  // 总行数

  for (int line = 0; line < max_lines; line++) {
    int x0 = x;
    int line_start = line * max_chars_per_line;
    int line_end = min(line_start + max_chars_per_line, str_length);
    for (int i = line_start; i < line_end; i = i + 3) {
      unsigned char b[3];
      b[0] = str[i];
      b[1] = str[i + 1];
      b[2] = str[i + 2];
      drawHz(x0, y + line * line_height, b, color);
      x0 += 11;
    }
  }
}

