// 0. Documentation Section
// This program simulates a pacemaker by energizing different LEDs.
// This program uses the TM4C123G microcontroller, a button, and LEDs to simulate a pacemaker.
// A green LED is energized when the system is ready to deliver a simulated shock.
// The button represents an atrial sensor. When it's pushed it simulates sensing a heart's atrial contraction by de-energizing the green LED.
// When the button is released, it represents the completion of the atrial contraction.
// The system waits 250ms for blood to flow from the heart's atria to its ventricles.
// Then a red LED energizes for 250ms, which simulates a shock to the ventricles.
// Once the simulated shock is delivered, the ventricles pump blood to the rest of the body. Then the green LED returns to the ready state.
// Author: Randy Shreeves
// Date: 11/01/2023

// 1. Pre-processor Directives Section
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTE_DATA_R (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_AMSEL_R (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R (*((volatile unsigned long *)0x4002452C))
#define GPIO_PORTE_DIR_R (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R (*((volatile unsigned long *)0x4002451C))
#define NVIC_ST_CTRL_R (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R (*((volatile unsigned long *)0xE000E018))

// 2. Global Declarations Section
void port_e_initialization(void);
void set_ventricular_trigger(void);
void clear_ventricular_trigger(void);
void set_ready_signal(void);
void clear_ready_signal(void);
void sys_tick_initialization(void);
void delay_1ms(unsigned long msec);
void wait_for_atrial_sensor_low(void);
void wait_for_atrial_sensor_high(void);

// 3. Subroutines Section
int main(void){
	// Setup
	port_e_initialization();
	
	// Loop
	while(1){
		set_ready_signal();
		wait_for_atrial_sensor_high();
		clear_ready_signal();
		delay_1ms(10);	// delay 10ms to account for possible button bounce
		wait_for_atrial_sensor_low();
		delay_1ms(250);
		set_ventricular_trigger();
		delay_1ms(250);
		clear_ventricular_trigger();
	}
}// end main()

void port_e_initialization(void){
	unsigned long volatile delay; // delay for 3-5 bus cycles for clock to stabilize
	SYSCTL_RCGC2_R |= 0x10;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTE_AMSEL_R &= ~0x07;
  GPIO_PORTE_PCTL_R &= ~0x000000FF;
	GPIO_PORTE_DIR_R |= 0x06;
	GPIO_PORTE_DIR_R &= ~0x01;
	GPIO_PORTE_AFSEL_R &= ~0x07;
	GPIO_PORTE_DEN_R |= 0x07;
} // end port_e_initialization

void set_ventricular_trigger(void){
	GPIO_PORTE_DATA_R |= 0x02;
}

void clear_ventricular_trigger(void){
	GPIO_PORTE_DATA_R &= ~0x02;
}

void set_ready_signal(void){
	GPIO_PORTE_DATA_R |= 0x04;
}
void clear_ready_signal(void){
	GPIO_PORTE_DATA_R &= ~0x04;
}

void sys_tick_initialization(void){
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 15999;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x00000005;
}//end sys_tick_initialization

void delay_1ms(unsigned long msec){
	unsigned long n = msec;
	while (n > 0){
		sys_tick_initialization();
		while((NVIC_ST_CTRL_R&0x00010000) == 0) {
			// wait for count flag to be set
		}// end inner while loop
		NVIC_ST_CTRL_R = 0;
		n--;
	}// end outer while loop
}// end delay_1ms()

void wait_for_atrial_sensor_low(void){
	while((GPIO_PORTE_DATA_R&0x01) == 0x01){
		// do nothing
	}
	return;
}
void wait_for_atrial_sensor_high(void){
	while((GPIO_PORTE_DATA_R&0x01) == 0x00){
		// do nothing
	}
	return;
}