#include <stdio.h>
#include <math.h>
#include <stdbool.h>

//////////// function declarations //////////////
void drawBox(int x, int y);
void plot_pixel(int x, int y, short int line_color);
void newSpeed(double angle);
int keyboard();
bool gameOver(int x, int y);


//////////// global variables //////////////
int pixel_buffer_start; 
volatile int *pixel_ctrl_ptr = (int *)0xFF203020;
double angle = 180;
int horizontal_speed = 82;
int vertical_speed = 0; //increment by 6 every second


//////////// helper functions //////////////

void drawBox(int x, int y){
    for (int i = (x-1); i < x+2; i++){
        for (int j = (y-1); j < y+2; j++){
            if (i != x && j != y){
                plot_pixel(i,j,0xFFFF);
            }
        }
    }
}

void plot_pixel(int x, int y, short int line_color){
    volatile short int *one_pixel_address;
    one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);
    *one_pixel_address = line_color;
}


void newSpeed(double boxAngle){}

//check if row of pixels below box are white
//restart game?
bool gameOver(int x, int y){
    bool result = false;

    volatile short int *one_pixel_address;
    y += 2;
    one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);
    if (*one_pixel_address = 0xffff){
        result = true;
    }

    return result;
}


int keyboard(){
	unsigned char byte3 = -1;
  	volatile int *PS2_ptr = (int *) 0xFF200100;  // PS/2 port address
    volatile int *RLEDs = (int *) 0x0FF200000;
	int PS2_data, RVALID;

    PS2_data = *(PS2_ptr);	// read the Data register in the PS/2 port
    RVALID = (PS2_data & 0x8000);	// extract the RVALID field
    if (RVALID != 0){
        byte3 = PS2_data & 0xFF;
        switch (byte3) {
            case 0x74:
                *RLEDs = 1;
                return 1;
            case 0x6B:
                *RLEDs = 2;
                return 2;
            case 0x75:
                *RLEDs = 3;
                return 3;						
        }
    }
}

//////////// MAIN FUNCTION //////////////

int main() {    

    while (1){
        int input = keyboard();
        if (gameOver); // game over screen?

        if (input == 1){    
            //up key
            //calculate angle (0 to 180) => starts at 180
            //call newSpeed

        }
        else if (input == 2){       //rotate right

        }
        else if (input == 3){       //rotate left

        }
    }

    return 0;
}





