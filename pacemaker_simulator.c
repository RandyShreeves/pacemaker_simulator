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

// 3. Subroutines Section
int main(void){
	// Setup
	port_e_initialization();
	
	// Loop
	while(1){
		set_ventricular_trigger();
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