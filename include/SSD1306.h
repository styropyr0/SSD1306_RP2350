#ifndef SSD1306
#define SSD1306
#define ADDR 0x3C

/**
 * @author Saurav Sajeev
 */

#include "pio_i2c.h"
#include <stdint.h>
#include <string>
#include <pico/stdlib.h>
#include <cmath>
#include <algorithm>
#include <cstring>
#include "hardware/i2c.h"
#include <pico/stdlib.h>

#ifdef __cplusplus

enum dimensions
{
    WIDTH_128 = 0x80,
    WIDTH_64 = 0x40,
    HEIGHT_64 = 0x40,
    HEIGHT_32 = 0x20
};

enum registryAddress
{
    OLED_OFF = 0xAE,
    OLED_ON = 0xAF,
    INVERT = 0xA7,
    REVERT = 0xA6,
    ENTIRE_DISP_ON = 0xA5,
    RESUME_FROM_VRAM = 0xA4,
    DISP_CLOCK_DIV_RATIO = 0xD5,
    MULTIPLEX = 0xA8,
    CHRG_PUMP = 0x8D,
    DISP_OFFSET = 0xD3,
    MEM_ADDRESS_MODE = 0x20,
    COM_CONFIG = 0xDA,
    CONTRAST = 0x81,
    PRE_CHRG = 0xD9,
    VCOMH_DESEL = 0xDB,
    TURN_ON_ALL_PIXELS = 0xA4,
    SET_DISP_NORMAL = 0xA6,
    SET_COL_ADDR = 0x21,
    SET_PG_ADDR = 0x22
};

enum registryCommands
{
    CLK_RATIO_RST = 0x80,
    CLK_RATIO_HIGH = 0x80,
    CLK_RATIO_MED = 0xB0,
    CLK_RATIO_LOW = 0xF0,
    MULTIPLEX_RATIO_RST = 0x3F,
    OFFSET_NO = 0x00,
    HSCROLL_RIGHT = 0x26,
    HSCROLL_LEFT = 0x27,
    VHSCROLL_RIGHT = 0x29,
    VHSCROLL_LEFT = 0x2A,
    STOP_SCROLL = 0x2E,
    START_SCROLL = 0x2F,
    START_LINE = 0x40,
    CHRG_PUMP_75 = 0x24,
    CHRG_PUMP_85 = 0x27,
    CHRG_PUMP_90 = 0x4F,
    CHRG_PUMP_OFF = 0x10,
    HORIZONTAL = 0x00,
    VERTICAL = 0x01,
    PAGE = 0x02,
    SEG_REMAP_0 = 0xA0,
    SEG_REMAP_127 = 0xA1,
    COM_OUT_SCAN_NORMAL = 0xC0,
    COM_OUT_SCAN_REMAP = 0xC8,
    COM_ALT = 0x12,
    CONTRAST_RST = 0x7F,
    CONTRAST_MAX = 0xFF,
    PRE_CHRG_RST = 0x22,
    PRE_CHRG_MAX = 0xAA,
    VCOMH_65 = 0x00,
    VCOMH_71 = 0x10,
    VCOMH_77 = 0x20,
    VCOMH_83 = 0x30,
    ZERO = 0x00
};

enum powerModes
{
    LOW_POWER_MODE = 0x01,   // Low power consumption, slow display, uses I2C STANDARD MODE (100kHz)
    BALANCED_MODE = 0x02,    // Balanced power consumption, balanced performance, uses I2C STANDARD FAST MODE (200kHz)
    PERFORMANCE_MODE = 0x03, // More power consumption, faster display, enables I2C FAST MODE (400kHz)
    TURBO_MODE = 0x04        // I2C FAST MODE PLUS (1MHz). If your device doesn't support this mode, it will automatically switch to last selected mode.
};

class OLED
{
public:
    /**
     * @brief Constructor which initialize OLED display object. By default, address is 0x3C. If you want to use a different address, please specify it as the third parameter.
     * @param width The width of the OLED display.
     * @param height The height of the OLED display.
     */
    OLED(uint8_t width, uint8_t height);

    /**
     * @brief Constructor which initialize OLED display object, with a custom I2C address.
     * @param width The width of the OLED display.
     * @param height The height of the OLED display.
     * @param address The SSD1306 display's I2C address.
     */
    OLED(uint8_t width, uint8_t height, uint8_t address);

    /**
     * @brief Set a custom font set for the characters. You could also set a custom language character.
     * @param fontArray The array of fonts.
     */
    void setFont(const uint8_t (*fontArray)[5]);

    /**
     * @brief Setup the display. You may call this in the setup function.
     */
    void begin();

    /**
     * @brief Set the font as the default font.
     */
    void clearCustomFont();

    /**
     * @brief Prints the string passed on the display at the specified X, Y coordinates.
     * @param string The string to be printed.
     * @param x The X coordinate.
     * @param y The y coordinate.
     */
    void print(const std::string& string, uint8_t x, uint8_t y);

    /**
     * @brief Prints the string passed on the display at the specified X, Y coordinates as highlighted.
     * @param string The string to be printed.
     * @param x The X coordinate.
     * @param y The y coordinate.
     */
    void printHighlighted(const std::string& string, uint8_t x, uint8_t y);

    /**
     * @brief Prints the string passed on the display at the specified X, Y coordinates with a typewriter animation.
     * @param string The string to be printed.
     * @param x The X coordinate.
     * @param y The y coordinate.
     * @param delay The delay between each character.
     */
    void printAnimated(const std::string& string, uint8_t x, uint8_t y, int delay, bool highlight);
    
    /**
     * @brief Clears the screen and prints the string passed on the display at the specified X, Y coordinates.
     * @param string The string to be printed.
     * @param x The X coordinate.
     * @param y The y coordinate.
     */
    void print_c(const std::string& string, uint8_t x, uint8_t y);

    /**
     * @brief Accepts a dataSet and setup the display with custom preferences.
     * @param dataSet Array of custom preferences.
     */
    void manualSetup(uint8_t *dataSet);

    /**
     * @brief Clears the OLED display.
     */
    void clearScr();

    /**
     * @brief Converts a string to dynamic char* array.
     * @param string Input string.
     * @returns Dynamic char* array of input string.
     */
    char *convertString(const std::string& string);

    /**
     * @brief Displays a progress bar.
     * @param progress Progress to be set.
     * @param x The X coordinate.
     * @param y The Y coordinate.
     * @param style Progress bar variant [USE BETWEEN 1-15 VARIANTS].
     * @note Styles 1-10 are progress bars and 11-15 are loaders.
     */
    void progressBar(uint8_t progress, uint8_t x, uint8_t y, int style);

    /**
     * @brief Sets the display mode to off when clear screen is called.
     * @param mode Set true to turn off the display.
     */
    void turnOffOnClr(bool mode);

    /**
     * @brief Sets the brightness of the pixels in percentage.
     * @param brightness Percentage of brightness.
     */
    void setBrightness(uint8_t brightness);

    /**
     * @brief Draws a bitmap image on the display, respecting the specified coordinates.
     * @param dataSet Bitmap array.
     * @param x The X coordinate.
     * @param y The Y coordinate.
     * @param width Width of the bitmap
     * @param height Height of the bitmap.
     */
    void draw(const uint8_t *dataSet, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

    /**
     * @brief Sets the power mode. There are four available power modes.
     * @param mode The power mode. Use enum values for power mode - (LOW_POWER_MODE or BALANCED_MODE or PERFORMANCE_MODE or TURBO_MODE).
     */
    void setPowerMode(uint8_t mode);

    /**
     * @brief Turns super brightness ON or OFF. Super brightness may be unstable.
     * @param mode True or false to turn it ON or OFF.
     */
    void superBrightness(bool mode);

    /**
     * @brief Inverts the display pixels.
     */
    void invertDisplay();

    /**
     * @brief Turns all the pixels ON.
     */
    void entireDisplayON();

    /**
     * @brief Reverts back to the content.
     */
    void entireDisplayOFF();

    /**
     * @brief Draws a rectangle with the specified dimensions at the specified coordinates.
     * @param startX Top-left X coordinate.
     * @param startY Top-left Y coordinate.
     * @param width Width of the rectangle in pixels
     * @param height Height of the rectangle in pixels.
     * @param cornerRadius Radius of each corner. Use 0 for flat.
     * @param thickness Thickness of the rectangle.
     * @param fill Fill the rectangle with pixels.
     */
    void rectangle(uint8_t startX, uint8_t startY, uint8_t width, uint8_t height, uint8_t cornerRadius, uint8_t thickness, bool fill);

    /**
     * @brief Draws a circle at the specified coordinates.
     * @param centerX X coordinate of center.
     * @param centerY Y coordinate of center.
     * @param radius Radius of the circle.
     * @param thickness Thickness of the circle.
     */
    void circle(uint8_t centerX, uint8_t centerY, uint8_t radius, uint8_t thickness);

    /**
     * @brief Draws a line between the specified coordinates.
     * @param startX Starting X coordinate.
     * @param startY Starting Y coordinate.
     * @param endX Ending X coordinate.
     * @param endY Ending Y coordinate.
     * @param thickness Thickness of the line.
     */
    void line(uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY, uint8_t thickness);

    /**
     * @name Text chaining operator.
     */
    OLED &operator<<(const std::string& string);

    /**
     * @name Bitmap chaining operator.
     */
    OLED &operator[](const uint8_t *bitmap);

    /**
     * @name Coordinates chaining operator.
     */
    OLED &operator<<(int coordinate);

    /**
     * @brief Clears the area between the two specified coordinates.
     * @param startX Starting point for clearing the screen.
     * @param endX The point to which the display shall be cleared.
     * @param page Page number.
     */
    void clearArea(uint8_t startX, uint8_t endX, uint8_t page);

    /**
     * @brief Inflates the drawing canvas. Draws all the items on the display.
     */
    void inflate();

    /**
     * @brief Clears the display buffer.
     * @param refresh If true, it will clear the display immediately, otherwise, waits for an inflate call.
     */
    void clearScr(bool refresh);

    /**
     * @brief Draws a pulse-style waveform plot on the display.
     *
     * This function renders a continuous line representing the variation in values over time,
     * resembling an ECG or heart rate monitor. Each value is scaled vertically according to
     * the provided maximum value and optionally centered around a median.
     *
     * @param x The X coordinate of the plot's top-left corner.
     * @param y The Y coordinate of the plot's top-left corner.
     * @param width The width of the plot area in pixels.
     * @param height The height of the plot area in pixels.
     * @param data Pointer to an array of integer data values.
     * @param size Number of data points in the array.
     * @param maxVal The maximum absolute value in the dataset (used for vertical scaling).
     * @param median The vertical baseline or axis center (default is 0).
     */
    void pulsePlot(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int *data, uint8_t size, int maxVal, int median = 0);

    /**
     * @brief Draws a binary bar plot (bit-style) on the display.
     *
     * Each data point is shown as a vertical bar, with height determined by the data value.
     * Ideal for visualizing binary or thresholded signals (e.g., 0s and 1s, or above/below logic).
     *
     * @param x The X coordinate of the plot's top-left corner.
     * @param y The Y coordinate of the plot's top-left corner.
     * @param width The width of the plot area in pixels.
     * @param height The height of the plot area in pixels.
     * @param data Pointer to an array of integer data values.
     * @param size Number of data points in the array.
     * @param maxVal The maximum value for vertical scaling.
     * @param median The baseline threshold value (default is 0).
     */
    void bitBarPlot(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int *data, uint8_t size, int maxVal, int median = 0);

    /**
     * @brief Draws a scatter plot on the display.
     *
     * Renders a series of discrete pixels representing the data points. Each point's height
     * is scaled and optionally offset by a median, with a horizontal axis drawn at the median.
     *
     * @param x The X coordinate of the plot's top-left corner.
     * @param y The Y coordinate of the plot's top-left corner.
     * @param width The width of the plot area in pixels.
     * @param height The height of the plot area in pixels.
     * @param data Pointer to an array of integer data values.
     * @param size Number of data points in the array.
     * @param maxVal The maximum absolute value used for vertical scaling.
     * @param median The vertical axis baseline (default is 0).
     */
    void scatterPlot(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int *data, uint8_t size, int maxVal, int median = 0);

    /**
     * @brief Draws a histogram plot on the display.
     *
     * Each data point is represented as a vertical bar, creating a visual distribution
     * of the dataset. Useful for viewing frequencies or intensity levels.
     *
     * @param x The X coordinate of the plot's top-left corner.
     * @param y The Y coordinate of the plot's top-left corner.
     * @param width The width of the plot area in pixels.
     * @param height The height of the plot area in pixels.
     * @param data Pointer to an array of integer data values.
     * @param size Number of data points in the array.
     * @param maxVal The maximum value used for scaling bar heights.
     */
    void histogramPlot(uint8_t x, uint8_t y, uint8_t width, uint8_t height, int *data, uint8_t size, int maxVal);

    /**
     * Invert the pixel state of the display
     * @param state Boolean value representing state - ON / OFF
     */
    void invertPixelState(bool state);

private:
    uint8_t HEIGHT = 0, WIDTH = 0, address = 0, charWidth = 0, step = 0, fontWidth = 5, currentPowerMode = BALANCED_MODE, invert = 0, pulsePlotPos = 0;
    std::string stringToPrint;
    uint8_t bitmapCoords[3];
    uint8_t buffer[128 * 64 / 8];
    const uint8_t *imgData;
    char *tempString;
    uint8_t count = 0, outMode = 0;
    const uint8_t (*fontSet)[5];
    bool IS_SETUP = false, clear = false, pixelState = true;
    void autoSetup();
    void execute(uint8_t instruction);
    void getFont(char c, bool highlight, uint8_t y);
    void convert(const std::string& string);
    void sendData(uint8_t data);
    void setPosition(uint8_t x, uint8_t y);
    void lowPowerMode(void);
    void balancedPowerMode(void);
    void performancePowerMode(void);
    void drawPixel(uint8_t x, uint8_t y);
    void displayBuffer(void);
    void drawCircleQuarter(uint8_t centerX, uint8_t centerY, uint8_t radius, uint8_t corner);
    uint8_t checkXBounds(uint8_t x);
    uint8_t checkYBounds(uint8_t y);
    void drawCircleHelper(int x0, int y0, int r, uint8_t cornerName);
    void clearBuffer(void);
    void drawFontPixel(uint8_t data, uint8_t x, uint8_t y);
};

#endif
#endif