/* This files provides address values that exist in the system */

#define SDRAM_BASE            0xC0000000
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_CHAR_BASE        0xC9000000

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define TIMER_BASE            0xFF202000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030

/* VGA colors */
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00
#define BLACK  0x0

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

/* Constants for animation */
#define BOX_LEN 2
#define NUM_BOXES 8

#define FALSE 0
#define TRUE 1

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//Prototypes
void drawRectangle(int x, int y, int width, int height, short int color);
void draw_line(int x1, int y1, int x2, int y2, short int line_color);
void plot_pixel(int x, int y, short int line_color);
void clear_screen();
void wait_for_vsync();

void swap(int *a1, int *a2) {
  int tmp = *a1;
  *a1 = *a2;
  *a2 = tmp;
}


volatile int pixel_buffer_start; // global variable

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables
    int twoBeforeX[8] = {0,50,100,150,200,250,300,319};
    int twoBeforeY[8] = {0,50,100,150,200,230,0,150};
    int prevX[8] = {0,50,100,150,200,250,300,319};
    int prevY[8] = {0,50,100,150,200,230,0,150};
    int RECT_W = 3;
    int RECT_H = 3;
    // initialize location and direction of rectangles(not shown)
    int x[8] = {0,50,100,150,200,250,300,319};
    int y[8] = {0,50,100,150,200,230,0,150};
    int xStep[8] = {1,1,1,1,1,1,1,1};
    int yStep[8] = {1,1,1,1,1,1,1,1};

    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen(); // clear backbuffer

    //Draw the board

    //Set up inputs and interupts (or we could poll within the main loop)

    while (true) {
        /* Erase any objects and lines that were drawn in the last iteration */


        /* Drawing any objects and lines */


        /*  Update the locations of object and lines */
        //Include collisions, drag, Checking if balls are in the pocket & Game Logic

        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}

//subroutines

void drawRectangle(int x, int y, int width, int height, short int color) {
  for (int xi = x; xi <= x+width; xi++) {
      draw_line(xi, y, xi, y+height, color);
  }
}

void wait_for_vsync() {
  volatile int * pixel_ctrl_ptr = 0xFF203020;
  register int status;

  *pixel_ctrl_ptr = 1;

  status = *(pixel_ctrl_ptr+3);
  while ( (status & 0x01) != 0 ) {
    status = *(pixel_ctrl_ptr+3);
  }
}

void clear_screen() {
    for (int x = 0; x < RESOLUTION_X; x++) {
      for (int y = 0; y < RESOLUTION_Y; y++) {
        plot_pixel(x, y, 0x0000);
      }
    }
}

void plot_pixel(int x, int y, short int line_color) {
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}


void draw_line(int x1, int y1, int x2, int y2, short int line_color) {

  //If a vertical / horizontal line => Draw using convential method
  if (x1 == x2) {
    for (int y = y1; y <= y2; y++) {
      plot_pixel(x1, y, line_color);
    }
    return;
  }else if (y1 == y2) {
    for (int x = x1; x <= x2; x++) {
      plot_pixel(x, y1, line_color);
    }
    return;
  }

  //Otherwise, Use Breseham's Algorithm to draw a line

  bool isSteep = abs(y2-y1) > abs(x2 - x1);
  if (isSteep) {
    //Swap x1, y1 & x2, y2
    swap(&x1, &y1);
    swap(&x2, &y2);
  }
  if (x1 > x2) {
    //Swap x1, x2 & y1, y2
    swap(&x1, &x2);
    swap(&y1, &y2);
  }

  int deltaY = abs(y2 - y1);
  int deltaX = x2 - x1;
  int error = -1 * deltaX / 2;

  int y = y1;
  int y_step = 0;
  if (y1 < y2)
    y_step = 1;
  else
    y_step = -1;

  for (int x = x1; x <= x2; x++) {
    if (isSteep) {
      plot_pixel(y, x, line_color);
    }else {
      plot_pixel(x, y, line_color);
    }

    error += deltaY;
    if (error >= 0) {
      y += y_step;
      error -= deltaX;
    }

  }

}
