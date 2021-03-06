#include <Arduino.h>

//Strings are evil. Especially on an Arduino.
//But if you insist...
//#define USE_STRINGS

//Some comments about using only c-style char array strings
//- Library is only using pointer references, make sure string memory stays valid for mulitple frames (Global buffer)
//- No checks for NULL-pointers or buffer overflows.... Make Sure strings are there and zero-terminated...

// max7219 registers
#define MAX7219_REG_NOOP         0x0
#define MAX7219_REG_DIGIT0       0x1
#define MAX7219_REG_DIGIT1       0x2
#define MAX7219_REG_DIGIT2       0x3
#define MAX7219_REG_DIGIT3       0x4
#define MAX7219_REG_DIGIT4       0x5
#define MAX7219_REG_DIGIT5       0x6
#define MAX7219_REG_DIGIT6       0x7
#define MAX7219_REG_DIGIT7       0x8
#define MAX7219_REG_DECODEMODE   0x9
#define MAX7219_REG_INTENSITY    0xA
#define MAX7219_REG_SCANLIMIT    0xB
#define MAX7219_REG_SHUTDOWN     0xC
#define MAX7219_REG_DISPLAYTEST  0xF

#define TEXT_ALIGN_LEFT          0 // Text is aligned to left side of the display
#define TEXT_ALIGN_LEFT_END      1 // Beginning of text is just outside the right end of the display
#define TEXT_ALIGN_RIGHT         2 // End of text is aligned to the right of the display
#define TEXT_ALIGN_RIGHT_END     3 // End of text is just outside the left side of the display

class LedMatrix {
    
public:
    
    /**
     * Constructor.
     * numberOfDisplays: number of connected devices
     * slaveSelectPin: CS (or SS) pin
     */
    LedMatrix(byte numberOfDisplays, byte slaveSelectPin);
    
    /**
     * Initializes the SPI interface
     */
    void init();
    
    /**
     * Sets the intensity on all devices.
     * intensity: 0-15
     */
    void setIntensity(byte intensity);
    
    /**
     * Sets the width in pixels for one character.
     * Default is 7.
     */
    void setCharWidth(byte charWidth);
    
    /**
     * Sets the text alignment.
     * Default is TEXT_ALIGN_LEFT_END.
     *
     */
    void setTextAlignment(byte textAlignment);
    
    /**
     * Send a byte to a specific device.
     */
    void sendByte (const byte device, const byte reg, const byte data);
    
    /**
     * Send a byte to all devices (convenience method).
     */
    void sendByte (const byte reg, const byte data);
    
    /**
     * Turn on pixel at position (x,y).
     */
    void setPixel(byte x, byte y);

    /**
     * Print a custom 8x8 character at position x. Data has to array of 8 bytes
     */
    void setCustomChar(byte x,byte width,byte *data);
    
    /**
     * Clear the frame buffer.
     */
    void clear();
    
    /**
     * Draw the currently set text at the current offset.
     */
    void drawText();
    
    /**
     * Set the current text.
     */
#ifdef USE_STRINGS    
    void setText(String text);
#else
    void setText(char* text);
#endif
    /**
     * Set the text that will replace the current text after a complete scroll
     * cycle.
     */
#ifdef USE_STRINGS    
    void setNextText(String nextText);
#else
    void setNextText(char*  nextText);
#endif
    /**
     * Set a specific column with a byte value to the framebuffer.
     */
    void setColumn(int column, byte value);
    
    /**
     * Writes the framebuffer to the displays.
     */
    void commit();
    
    /**
     * Scroll the text to the right.
     */
    void scrollTextRight();
    
    /**
     * Scroll the text to the left.
     */
    void scrollTextLeft();
    
    /**
     * Oscilate the text between the two limits.
     */
    void oscillateText();

    /**
     * Set rotated characters (for those displays where the Matrix is rotated by 90 Deg...)
     */
    void setRotate(bool rot);

private:
    byte* cols;
    uint16_t  *spitransfer; //Could save a few bytes here by making this dynamically allocated but that sounds like overkill
#ifdef USE_STRINGS  
    String myText;
    String myNextText;
#else
    char* myText;
    char* myNextText;
#endif
    int myTextOffset = 0;
    int myTextAlignmentOffset = 0;
    int increment = -1;
    byte myNumberOfDevices = 0;
    byte mySlaveSelectPin = 0;
    byte myCharWidth = 8;
    byte myTextAlignment = 1;
    bool rotate = false;
    
    void calculateTextAlignmentOffset();
    void sendAllBytes (void);

#ifdef USE_STRINGS  
    int getTextLength(String txt);
#else
    int getTextLength(char* txt);
#endif
};