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



volatile int pixel_buffer_start; // global variable



#include <stdlib.h>
#include <stdio.h>
#include "drawing.c"


//Prototypes
void wait_for_vsync();
void swap(int *a1, int *a2);

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
    //Draw the board on both buffers
    drawRectangle(0, 0, RESOLUTION_X, RESOLUTION_Y, 0x0);
    drawRectangle(20, 20, RESOLUTION_X-40, RESOLUTION_Y-40, CYAN);
    fillCircle(20, 20, 10, WHITE);


    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    //Draw the board on both buffers
    drawRectangle(0, 0, RESOLUTION_X, RESOLUTION_Y, 0x0);
    drawRectangle(20, 20, RESOLUTION_X-40, RESOLUTION_Y-40, CYAN);
	  fillCircle(20, 20, 10, WHITE);

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


void wait_for_vsync() {
  volatile int * pixel_ctrl_ptr = 0xFF203020;
  register int status;

  *pixel_ctrl_ptr = 1;

  status = *(pixel_ctrl_ptr+3);
  while ( (status & 0x01) != 0 ) {
    status = *(pixel_ctrl_ptr+3);
  }
}

void swap(int *a1, int *a2) {
  int tmp = *a1;
  *a1 = *a2;
  *a2 = tmp;
}
