#include "keyboard.h"


void keyboard(){
    // unsigned char byte1 = 0;
	// unsigned char byte2 = 0;

    
	unsigned char byte3 = 0;
	
  	volatile int * PS2_ptr = (int *) 0xFF200100;  // PS/2 port address

	int PS2_data, RVALID;

	while (1) {
		PS2_data = *(PS2_ptr);	// read the Data register in the PS/2 port
		RVALID = (PS2_data & 0x8000);	// extract the RVALID field
		if (RVALID != 0){

			// byte1 = byte2;
			// byte2 = byte3;
			byte3 = PS2_data & 0xFF;
			if ((byte3 == 0x74) || (byte3 == 0x6B) || (byte3 == 0x75)){
				switch (byte3) {
					case 0x74:
						*RLEDs = 1;
						break;
					case 0x6B:
						*RLEDs = 2;
						break;
					case 0x75:
						*RLEDs = 3;						
						break;
				}
			}
		}
	}
}
