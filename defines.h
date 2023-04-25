// SCREEN AND GRID
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 750
#define TILE_SIZE 32 ///Kích thước ô vuông
#define GRID_WIDTH 37 //Số ô chiều ngang
#define GRID_HEIGHT 18 //Số ô chiều dọc

// FRUITS TYPES
#define FRUIT_APPLE 0
#define FRUIT_JAM 1
#define FRUIT_SHIELD 2
#define FRUIT_POOP 3

// COLORS
#define BG_COLOR 0, 0, 0
#define SNAKE_COLOR 255, 30, 60
#define FRUIT_COLOR 0, 140, 140
#define WALL_COLOR 40, 100, 40
#define FLOOR_COLOR1 40, 180, 40
#define FLOOR_COLOR2 40, 200, 40

// SNAKE DIRECTIONS
#define DIR_UP 3
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 0

// SCORE NUMBERS
static const char* zero  = "xxxx xx xx xxxx";
static const char* one   = "  x  x  x  x  x";
static const char* two   = "xxx  xxxxx  xxx";
static const char* three = "xxx  xxxx  xxxx";
static const char* four  = "x xx xxxx  x  x";
static const char* five  = "xxxx  xxx  xxxx";
static const char* six   = "xxxx  xxxx xxxx";
static const char* seven = "xxx  x  x  x  x";
static const char* eight = "xxxx xxxxx xxxx";
static const char* nine  = "xxxx xxxx  xxxx";

static const char** digits[] = { &zero, &one, &two, &three, &four, &five, &six, &seven, &eight, &nine };

static const int DIGIT_PIXEL_SIZE = 5;
