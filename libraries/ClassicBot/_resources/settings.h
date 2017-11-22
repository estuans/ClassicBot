
// PINS
// ESP to Arduino
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

// TFT pins
#define TFT_RST    D0
#define TFT_CS     D1
#define TFT_DC     D2
#define TFT_MOSI   D7
#define TFT_SCLK   D5
#define TFT_LITE   D8


// -- COLOURS --
#define BLACK       0x0000      /*   0,   0,   0 */
#define NAVY        0x000F      /*   0,   0, 128 */
#define DARKGREEN   0x03E0      /*   0, 128,   0 */
#define DARKCYAN    0x03EF      /*   0, 128, 128 */
#define MAROON      0x7800      /* 128,   0,   0 */
#define PURPLE      0x780F      /* 128,   0, 128 */
#define OLIVE       0x7BE0      /* 128, 128,   0 */
#define LIGHT_GREY   0xC618      /* 192, 192, 192 */
#define DARK_GREY    0x7BEF      /* 128, 128, 128 */
#define BLUE        0x001F      /*   0,   0, 255 */
#define GREEN       0x07E0      /*   0, 255,   0 */
#define CYAN        0x07FF      /*   0, 255, 255 */
#define RED         0xF800      /* 255,   0,   0 */
#define MAGENTA     0xF81F      /* 255,   0, 255 */
#define YELLOW      0xFFE0      /* 255, 255,   0 */
#define WHITE       0xFFFF      /* 255, 255, 255 */
#define ORANGE      0xFD20      /* 255, 165,   0 */
#define GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define PINK        0xF81F

#define BG_COLOUR DARK_GREY
#define FG_COLOUR CYAN

#define LEFT_EYE_X 20
#define RIGHT_EYE_X 80
#define LEFT_EYE_Y 80
#define RIGHT_EYE_Y 80
#define DEFAULT_EYE_HEIGHT 30
#define DEFAULT_EYE_WIDTH 10

#define CLOCK_X 0
#define CLOCK_Y 0
#define CLOCK_HEIGHT 30
#define CLOCK_WIDTH 128
