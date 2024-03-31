#include <stdio.h>
#include <math.h>
#include <stdbool.h>

//////////// global variables //////////////
double angle = 3.15;
int x = 0;
int y = 0;
int horizontal_speed = 2;
int vertical_speed = 2; //increment by 6 every second
volatile int pixel_buffer_start; // global variable
short int Buffer1[240][512]; // 240 rows, 512 (320 + padding) columns   
short int Buffer2[240][512];
double box_position[2];
double box_dir[2];

//////////// function declarations //////////////
void wait_for_vsync();
void clear_screen();
void drawBox(int x0, int y0);
void plot_pixel(int x, int y, short int line_color);
double newSpeed(double angle);
int keyboard();
bool gameOver(int x0, int y0);
void drawRover(int x0, int y0);
void drawFlame(double boxAngle, int x0, int y0);

//////////// helper functions //////////////
void wait_for_vsync(){
    volatile int * pixel_ctrl_ptr = (int *) 0xff203020; // base address
    int status;
    *pixel_ctrl_ptr = 1; 
    status = *(pixel_ctrl_ptr + 3);

    while ((status & 0x01) != 0) {
        status = *(pixel_ctrl_ptr + 3);
    } 
}

void clear_screen(){
	for(int ind1 = 0; ind1<= 319; ind1++){
		for(int ind2 = 0; ind2 <=239; ind2++){
			plot_pixel(ind1,ind2,0);
		}
	}
}

void drawBox(int x0, int y0){
	for (int i = (x0-3); i < x0+4; i++){
		for (int j = (y0-3); j < y0+4; j++){
			// i = i*cos(angle) - j*sin(angle);
			// j = i*sin(angle) + j*cos(angle);
			plot_pixel(i,j,0xFFFF);
		}
    }
}

// void drawFlame(double boxAngle, int x, int y){}


// void drawRover(int x, int y){}

void plot_pixel(int x0, int y0, short int line_color){
    volatile short int *one_pixel_address;
    one_pixel_address = pixel_buffer_start + (y0 << 10) + (x0 << 1);
    *one_pixel_address = line_color;
}


// double newSpeed(double boxAngle){}

//check if row of pixels below box are white
// bool gameOver(int x, int y){
//     bool result = false;
//     int count = 0;

//     volatile short int *one_pixel_address;
//     y += 2;
//     x -= 1;
//     for (int i = 0; i < 3; i++){
//         one_pixel_address = pixel_buffer1 + (y << 10) + (x << 1);
//         x++;
//         if (*one_pixel_address = 0xffff){
//             count++;
//             if (count == 3){
//                 result = true;
//             }
//         }
//     }

//     return result;
// }


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
	return -1;
}

void newLocation(int x0, int y0){
	vertical_speed += 6/60;
	horizontal_speed += 5/60;
	y += vertical_speed;
	x += horizontal_speed;
}

//void eraseBox(){}

//////////// MAIN FUNCTION //////////////

int main() {   
    volatile int *pixel_ctrl_ptr = (int *)0xFF203020;
    *(pixel_ctrl_ptr + 1) = (int) &Buffer1; 
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen();
    *(pixel_ctrl_ptr + 1) = (int) &Buffer2;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); 
    clear_screen(); 


    while (1){
		clear_screen();
		newLocation(x, y);
		drawBox(x, y);
		wait_for_vsync();
		
        int input = keyboard();

        //erase box, identify new location, draw box
        //current location plus speed * 1/60 (round to nearest int)
        
        

        if (input == 1); 
            //up key
            //calculate angle (0 to 180) => starts at 180
            //call newSpeed

        
        else if (input == 2){ //rotate right angle - 0.524 rads
			if (angle >= 0.52){
				angle -= 0.52;
			}
		}

    
        else if (input == 3){
			if (angle <= 2.62){
				angle += 0.52;
			}	
		}
		
		pixel_buffer_start = *(pixel_ctrl_ptr + 1);
    }

    return 0;
}