#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "sound_data.c"

#define SPEED 5

//////////// global variables //////////////
double angle = 3.15;
int x = 0;
int y = 0;
volatile int pixel_buffer_start; // global variable
short int Buffer1[240][512]; // 240 rows, 512 (320 + padding) columns   
short int Buffer2[240][512];
rover_position[4];

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

void project_background(){
    draw_line(0, 200, 60, 180, 0xffff);
	draw_line(60, 180, 70, 180, 0xffff);
	draw_line(70, 180, 110, 220, 0xffff);
	draw_line(110, 220, 125, 220, 0xffff);
	draw_line(125, 220, 145, 200, 0xffff);
	draw_line(145, 200, 155, 200, 0xffff);
	draw_line(155, 200, 170, 215, 0xffff);
	draw_line(170, 215, 176, 215, 0xffff);
	draw_line(176, 215, 185, 222, 0xffff);
	draw_line(185, 222, 208, 203, 0xffff);
	draw_line(208, 203, 250,230,0xffff);
	draw_line(250,230, 285, 195,0xffff);
	draw_line(285,195, 300, 195,0xffff);
	draw_line(300,195, 319, 176, 0xffff);
}


void wait_for_vsync(){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    int status;

    *pixel_ctrl_ptr = 1;
    status = *(pixel_ctrl_ptr + 3);
    while ((status & 0x01) != 0)
    {
        status = *(pixel_ctrl_ptr + 3);
    }
}

void clear_screen(){
	for(int ind1 = 0; ind1<= 319; ind1++){
		for(int ind2 = 0; ind2 <=239; ind2++){
			plot_pixel(ind1,ind2,0);
		}
	}
	project_background();
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

void swap(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

void newLocation(){
    // Update box positions according to their movement speeds
    rover_position[0] += rover_position[2];
    rover_position[1] += rover_position[3];

    // Check for collisions with the screen boundary and reverse direction if needed
    if(rover_position[0] + 1 > 320 || rover_position[0] - 1 < 0) {
        rover_position[2] = -rover_position[2]; // Reverse X direction
        rover_position[0] += rover_position[2]; // Adjust position after direction change
    }
    if(rover_position[1] + 1 > 240 || rover_position[1] - 1 < 0) {
        rover_position[3] = -rover_position[3]; // Reverse Y direction
        rover_position[1] += rover_position[3]; // Adjust position after direction change
    }
}

void draw_line(int x1, int y1, int x2, int y2, short int line_color){
	bool is_steep;
	int deltax = x2 - x1;
	int deltay = abs(y2 - y1);
	int error = -(deltax/2);
	int y = y1;
	int y_step;
	
	if(abs(y2 - y1) > abs(x2 - x1)){
		is_steep = 1;
	}else{
		is_steep = 0;
	}
	if(is_steep == 1){
		swap(&x1,&y1);
		swap(&x2,&y2);
	}
	if(x1 > x2){
		swap(&x1,&x2);
		swap(&y1,&y2);
	}
	if(y1 < y2){
		y_step = 1;
	}else{
		y_step = -1;
	}
	
	for(int x = x1; x <= x2; x++){
		if(is_steep == 1){
			plot_pixel(y,x,line_color);
		}else{
			plot_pixel(x,y,line_color);
		}
		
		error = error + deltay;
		if(error >= 0){
			y = y + y_step;
			error = error - deltax;
		}
	}
}
//////////// MAIN FUNCTION //////////////

int main() {  
	//Initializations
	rover_position[0] = x;
	rover_position[1] = y;
	rover_position[2] = SPEED;
	rover_position[3] = SPEED;

	// Double Buffer Set Up
    volatile int *pixel_ctrl_ptr = (int *)0xFF203020;
    *(pixel_ctrl_ptr + 1) = (int) &Buffer1; 
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen();
	wait_for_vsync();
    *(pixel_ctrl_ptr + 1) = (int) &Buffer2;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); 
    clear_screen(); 

    while (1){
		clear_screen();
		newLocation();
		drawBox(rover_position[0], rover_position[1]);
		
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
		
		wait_for_vsync();
		pixel_buffer_start = *(pixel_ctrl_ptr + 1);
    }

    return 0;
}
