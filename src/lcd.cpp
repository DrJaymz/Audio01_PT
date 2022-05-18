#include <Arduino.h>
#include "global.h"
#include <TFT_eSPI.h>

#define TFT_RESERVED_LINES 1
#define TFT_TEXT_SIZE 2

// void TFT_powerUp() {
//   Serial.println("Powering up TFT");
//   tft.init();
//   // clear screen:
//   tft.fillScreen(TFT_BLACK);
//   //tft.setRotation(0); // porttrait 0, landscape: 1
// }

// void TFT_sleep() {
//   // clear screen:
//   tft.fillScreen(TFT_BLACK);
//   //
//   Serial.println("Setting TFT (again) to deep-sleep ");
//   tft.writecommand(0x10); // SLEEP (backlight still on ...)
//   digitalWrite(TFT_BL, LOW); // backlight OFF
//   delay(5); // needed!
// }

// void TFT_wake() {
//   tft.writecommand(0x11); // WAKEUP
//   delay(120); // needed! PWR neeeds to stabilize!
//   digitalWrite(TFT_BL, HIGH); // backlight ON
// }

void TFT_printLineAt(String line, int position)
{
    tft.setTextSize(TFT_TEXT_SIZE);
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.setTextWrap(false);
    tft.setCursor(0, position * 16);
    tft.println(line);
}

void TFT_printLine(String line, bool clearScreen = false)
{
    // const int stringSize = 2;
    tft.setTextSize(TFT_TEXT_SIZE);
    const int height = 16 / TFT_TEXT_SIZE;
    const int width = 40 / TFT_TEXT_SIZE;
    static String lines[height];
    static bool init = false;
    static int current_line = 0;

    if(line.length() == 0) return;

    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.setTextWrap(false);

    if (clearScreen)
    {
        init = false;
        current_line = -1 + TFT_RESERVED_LINES;
    }

    if (!init)
    {
        for (size_t i = 0; i < height; i++)
        {

            lines[i].reserve(width);
            lines[i] = "";
        }
        init = true;
        tft.fillScreen(TFT_BLUE);
    }

    // line too long
    if (line.length() > width)
    {
        int position = 0;
        while (position < line.length())
        {
            TFT_printLine(line.substring(position, position + width));
            position += width;
        }

        return;
    }

    Serial.print("TFT:");
    Serial.println(line);
    tft.setCursor(0, 0);

    // if we are starting with an empty screen we just need to tell it to print on the next line
    if (current_line < (height - 1))
    {
        current_line++;
    }
    else
    {
        // if we're at the bottom of the screen scroll lines up
        for (size_t i = TFT_RESERVED_LINES; i < height - 1; i++)
        {
            lines[i] = lines[i + 1];
        }
    }

    // pad the string
    while (line.length() < width)
    {
        line += " ";
    }

    lines[current_line] = line;

    // lines[height - 1] = line;

    for (size_t i = 0; i < height; i++)
    {
        tft.println(lines[i]);
    }
}

void TFT_clear()
{
    tft.fillScreen(TFT_BLUE);
}

void printScreenLine()
{
    TFT_printLine("", false);
}