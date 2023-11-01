# pacemaker_simulator
Program that simulates a pacemaker by energizing different LEDs.

This system uses the TM4C123G microcontroller, a button, and LEDs to simulate a pacemaker.
A green LED is energized when the system is ready to deliver a simulated shock.
The button represents an atrial sensor. When it's pushed it simulates sensing a heart's atrial contraction by de-energizing the green LED.
When the button is released, it represents the completion of the atrial contraction.
The system waits 250ms for blood to flow from the heart's atria to its ventricles.
Then a red LED energizes for 250ms, which simulates a shock to the ventricles.
Once the simulated shock is delivered, the ventricles pump blood to the rest of the body. Then the green LED returns to the ready state.
