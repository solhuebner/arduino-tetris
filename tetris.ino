/**
 * http://adafru.it/1484
 */

#include <RGBmatrixPanel.h>

#define CLK 8
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

#define SWITCH A5

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

// colors
#define BLACK  matrix.Color333(0, 0, 0)
#define GRAY   matrix.Color333(1, 1, 1)
#define YELLOW matrix.Color333(3, 2, 0)
#define BLUE   matrix.Color333(0, 0, 1)
#define GREEN  matrix.Color333(0, 1, 0)
#define ORANGE matrix.Color333(3, 1, 0)
#define RED    matrix.Color333(1, 0, 0)
#define PURPLE matrix.Color333(1, 0, 1)

// for positions on the panel
#define SCORE_POINTS 1
#define SCORE_LINES  2
#define SCORE_LEVELS 3

// for collision checking
#define ROTATE 1
#define LEFT   2
#define RIGHT  3
#define FALL   4

int bucket[17];

unsigned long last_interaction = 0;
unsigned long last_tick        = 0;
int           tick_length      = 1000;

// scores
byte levels;
byte lines;
unsigned long points;

// current tetromino
byte     tetr_type, next_tetr_type;
uint16_t tetr_color, next_tetr_color;
byte     tetr_rotation;
int      tetr_offsX;
int      tetr_offsY;

/**
 * those are the 7 different tetris bricks, each placed
 * in a 4x4 square like this:
 * 
 * 16 15 14 13
 * 12 11 10 09
 * 08 07 06 05
 * 04 03 02 01
 * 
 * set bits are the parts where a brick is solid
 * 
 * the up to four quartets represent the four directions a
 * tetromino can rotate
 */

const PROGMEM uint16_t TETROMINOES[28] = {
  // X
  // X
  // X
  // X
  0b0100010001000100, 0b0000000011110000, 0b0100010001000100, 0b0000000011110000,
  
  // XX
  // XX
  0b0000011001100000, 0b0000011001100000, 0b0000011001100000, 0b0000011001100000,
  
  // XX
  //  XX
  0b0000011000110000, 0b0000001001100100, 0b0000011000110000, 0b0000001001100100,
  
  //  XX
  // XX
  0b0000011011000000, 0b0000100011000100, 0b0000011011000000, 0b0000100011000100,
  
  // X
  // X
  // XX
  0b0000011101000000, 0b0100010001100000, 0b0000001011100000, 0b0000011000100010,
  
  //  X
  //  X
  // XX
  0b0000111000100000, 0b0000011001000100, 0b0000010001110000, 0b0010001001100000,
  
  //  X
  // XXX
  0b0000000011100100, 0b0000010011000100, 0b0000010011100000, 0b0000010001100100
};

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
