#include <LOLIN_EPD.h>
#include <Adafruit_GFX.h>

/*D32 Pro*/
#define EPD_CS 14
#define EPD_DC 27
#define EPD_RST 33  // can set to -1 and share with microcontroller Reset!
#define EPD_BUSY -1 // can set to -1 to not use a pin (will wait a fixed delay)

LOLIN_IL3897 EPD(250, 122, EPD_DC, EPD_RST, EPD_CS, EPD_BUSY); //hardware SPI

void setupEPD(void) 
{
  EPD.begin();
  EPD.clearBuffer();
  EPD.fillScreen(EPD_WHITE);
}
 
void epdDisplay()
{
  EPD.display();
}

void epdDrawLines()
{
  EPD.drawFastHLine(  1,  25, 250, EPD_BLACK);
  EPD.drawFastHLine(  1,  26, 250, EPD_BLACK);
  EPD.drawFastVLine( 82,  27,  80, EPD_BLACK);
  EPD.drawFastVLine( 83,  27,  80, EPD_BLACK);
  EPD.drawFastVLine(167,  27,  80, EPD_BLACK);
  EPD.drawFastVLine(168,  27,  80, EPD_BLACK);
  EPD.drawFastHLine(  1, 107, 250, EPD_BLACK);
  EPD.drawFastHLine(  1, 108, 250, EPD_BLACK);
}

void epdSetDate(String dateString)
{
  EPD.setTextColor(EPD_BLACK);

  EPD.setTextSize(2);
  EPD.setCursor(125 - (6*dateString.length()), 3);
  EPD.println(dateString);

}

void epdDayStatistics(float current, float previous, int delta)
{
  epdDrawValues(1, current, previous, delta);
}

void epdMonthStatistics(float current, float previous, int delta)
{
  epdDrawValues(2, current, previous, delta);
}

void epdYearStatistics(float current, float previous, int delta)
{
  epdDrawValues(3, current, previous, delta);
}

void epdDrawValues( int box, float current, float previous, int delta)
{
  int x = 0;
  int y = 62;
  int accuracy = 0;
  
  if ( box == 1 ) {
    x = 40;
    if ( getOrder(current) < 3 )
      accuracy = 1;
  } else if ( box == 2 ) {
    x = 125;
  } else if ( box = 3 ) {
    x = 210;
  }
  
  EPD.setTextColor(EPD_BLACK);

  int fontSize = 3;
  EPD.setTextSize(fontSize);
  EPD.setCursor(x - (6*fontSize*(getOrder(current)+2*accuracy)-fontSize)/2, y);
  EPD.println(current, accuracy);

  fontSize = 1;
  EPD.setTextSize(fontSize);
  EPD.setCursor(x - 39, y - 16);
  EPD.println(previous, accuracy);
  
  EPD.setTextSize(fontSize);
  EPD.setCursor(x + 39 - (fontSize*6*getDeltaSize(delta)-fontSize) , y - 16);
  EPD.print(delta);
  EPD.println("%");
}

void epdGenerating(bool generating)
{
  if ( generating ) 
  {
    EPD.setTextColor(EPD_BLACK);
    EPD.setTextSize(1);
    EPD.setCursor(95, 113);
    EPD.print("GENERATING");
  }
}

void epdPrintUpdateTime(String time)
{
  EPD.setTextColor(EPD_BLACK);
  EPD.setTextSize(1);
  EPD.setCursor(218, 113);
  EPD.println(time);
}

void epdPrintBatteryState(int percentage, float voltage)
{
  EPD.setTextColor(EPD_BLACK);
  EPD.setTextSize(1);
  EPD.setCursor(1, 113);
  EPD.print(percentage);
  EPD.print("%, ");
  EPD.print(voltage,1);
  EPD.println(" V");
}

void epdSleep()
{
  Serial.println("Put EPD display in sleep");
  EPD.deepSleep();
}

///////////////////////////////////////////////////////
// Helpers

int getOrder(float value)
{
  // returns 'size' of value, i.e. the order of magnitude
  return floor(log10(value+0.5));
}

int getDeltaSize(int delta)
{
  int order = getOrder(abs(delta));
  int sign = 0;
  if ( delta < 0 )
    sign = 1;

  // optional negative sign + delta size is the order of the delta + %
  return sign + order + 1;
}
