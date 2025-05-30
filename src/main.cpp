
#include "time.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Fonts/FreeSans12pt7b.h>

uint16_t default_color = ST77XX_BLACK;
#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

SPIClass mySPI(HSPI);
Adafruit_ST7789 tft = Adafruit_ST7789(&mySPI, TFT_CS, TFT_DC, TFT_RST);
class TextElement
{
private:
  String Value = "";
  uint16_t Color = ST77XX_WHITE;
  uint8_t size = 1;
  int16_t x, y;
  uint16_t w, h;
  int16_t x1, y1;
  int16_t prev_x, prev_y;
  uint16_t prev_w, prev_h;
  bool hasPrevious = false;

public:
  TextElement(String t, uint16_t c, uint8_t s)
  {
    Value = t;
    Color = c;
    size = s;
  }
  void CalculateBounds()
  {
    tft.setFont(&FreeSans12pt7b);
    tft.getTextBounds(Value, 0, 0, &x1, &y1, &w, &h);
  }
  void centerText()
  {
    CalculateBounds();
    x = (320 - w) / 2 - x1;
    y = (170 - h) / 2 - y1;
  }
  void centerVertically()
  {
    CalculateBounds();
    y = (170 - h) / 2 - y1;
  }
  void centerHorizontally()
  {
    CalculateBounds();
    x = (320 - w) / 2 - x1;
  }
  void clear()
  {
    if (hasPrevious)
    {
      tft.fillRect(prev_x - 20, prev_y + y1 - 20, prev_w + 40, prev_h + 40, default_color);
    }
  }
  void draw()
  {
    if (hasPrevious)
    {
      tft.fillRect(prev_x - 20, prev_y + y1 - 20, prev_w + 40, prev_h + 40, default_color);
    }
    prev_x = x;
    prev_y = y;
    prev_w = w;
    prev_h = h;

    hasPrevious = true;

    tft.setTextSize(size);
    tft.setFont(&FreeSans12pt7b);
    tft.setCursor(x, y);
    tft.print(Value);
  }
  void setText(String newt)
  {
    Value = newt;
    CalculateBounds();
  }
  void setSize(uint8_t news)
  {
    size = news;
    CalculateBounds();
  }

  void setPos(int16_t newx, int16_t newy)
  {
    x = newx;
    y = newy;
  }
};

class BoxElement
{
private:
  int16_t x, y;
  uint16_t w, h;
  int16_t x1, y1;
  int16_t prev_x, prev_y;
  uint16_t prev_w, prev_h;
  uint16_t Color = ST77XX_WHITE;
  bool hasPrevious = false;

public:
  BoxElement(uint16_t c, uint16_t newW, uint16_t newH)
  {
    Color = c;
    w = newW;
    h = newH;
  }

  void setPosition(int16_t newX, int16_t newY)
  {
    x = newX;
    y = newY;
  }

  void center()
  {
    x = (320 - w) / 2;
    y = (170 - y) / 2;
  }
  void centerHorizontally()
  {
    x = (320 - w) / 2;
  }
  void centerVertically()
  {
    y = (170 - y) / 2;
  }
  void draw(uint8_t r = 0)
  {
    if (hasPrevious)
    {
      tft.fillRect(prev_x, prev_y, prev_w, prev_h, default_color);
    }

    tft.drawRect(x, y, w, h, Color);

    prev_x = x;
    prev_y = y;
    prev_w = w;
    prev_h = h;
    hasPrevious = true;
  }

  void fill(bool border, uint16_t c, uint8_t r)
  {
    if (hasPrevious)
    {
      tft.fillRect(prev_x, prev_y, prev_w, prev_h, default_color);
    }
    if (r > 0)
    {
      tft.fillRoundRect(x, y, w, h, r, Color);
      prev_x = x;
      prev_y = y;
      prev_w = w;
      prev_h = h;
      hasPrevious = true;
      if (border)
      {
        tft.drawRoundRect(x, y, w, h, r, c);
      }
    }
    else
    {
      tft.fillRect(x, y, w, h, Color);
      prev_x = x;
      prev_y = y;
      prev_w = w;
      prev_h = h;
      hasPrevious = true;
      if (border)
      {
        tft.drawRect(x, y, w, h, c);
      }
    }
  }

  void drawHorizontalLine()
  {
    tft.drawLine(x, y + h / 2, x + w, y + h / 2, Color);
  }

  void drawVerticalLine()
  {
    tft.drawLine(x + w / 2, y, x + w / 2, y + h, Color);
  }

  void clear()
  {
    if (hasPrevious)
    {
      tft.fillRect(prev_x, prev_y, prev_w, prev_h, default_color);
    }
  }

  void setSize(uint16_t newW, uint16_t newH)
  {
    w = newW;
    h = newH;
  }

  void setColor(uint16_t newColor)
  {
    Color = newColor;
  }
};

class CircleElement
{
private:
  int16_t x, y;
  uint8_t r;
   uint8_t prev_r;
  int16_t x1, y1;
  int16_t prev_x, prev_y;

  uint16_t Color = ST77XX_WHITE;
  bool hasPrevious = false;

public:
  CircleElement(uint16_t c, uint8_t newR)
  {
    Color = c;

    r = newR;
  }

  void setPosition(int16_t newX, int16_t newY)
  {
    x = newX;
    y = newY;
  }

  void center()
  {
    x = 320/2;
    y = 170/2;
  }
  void centerHorizontally()
  {
    x = 320/2;
  }
  void centerVertically()
  {
    y = 170/2;
  }
  void draw()
  {
    if (hasPrevious)
    {
      tft.drawCircle(prev_x, prev_y, prev_r, default_color);
    }

    tft.drawCircle(x, y, r, Color);

    prev_x = x;
    prev_y = y;
    prev_r = r;
    hasPrevious = true;
  }

  void fill(bool border, uint16_t c)
  {
    if (hasPrevious)
    {
      tft.fillCircle(prev_x, prev_y, prev_r, default_color);
    }
      tft.fillCircle(x, y, r, Color);
      prev_x = x;
      prev_y = y;
      prev_r = r;
      hasPrevious = true;
      if (border)
      {
        tft.drawCircle(x, y, r , c);
      }

   
  }
  void clear()
  {
    if (hasPrevious)
    {
      tft.fillCircle(prev_x, prev_y,prev_r, default_color);
    }
  }


  void setColor(uint16_t newColor)
  {
    Color = newColor;
  }
  void setRadius(uint8_t newR){
    r = newR;
  }
};


TextElement Text("Hello World!", ST77XX_WHITE, 1);
BoxElement Box(ST77XX_CYAN, 100, 60);
CircleElement Circle(ST77XX_CYAN, 50);
void setup()
{
  mySPI.begin(TFT_SCLK, -1, TFT_MOSI, -1);
  tft.init(170, 320);
  tft.setRotation(1);
  tft.fillScreen(default_color);
  Circle.center();
  Circle.fill(true, ST77XX_WHITE);
}

void loop()
{
}