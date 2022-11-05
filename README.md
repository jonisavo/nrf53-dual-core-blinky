# nrf53-dual-core-blinky

A dual-core LED blinker for nrf53, using Zephyr's IPC module.

I wanted to learn embedded software engineering, and what's a better way to
start than building an over-engineered LED blinking application?

It uses nrf53's application and network cores like so:

- Application core
    - Continuously blinks LEDs in a separate thread
    - Listens for IPC messages from the network core
    - When an IPC message of a button press is received, the continuous
    blinking thread is suspended, and the LED corresponding to the button is lit
    up.
    - When an IPC message of a button release is received, the continuous
    blinking thread is resumed. The LED corresponding to the button is no
    longer lit up.
- Network core
    - Listens to button presses
    - If a button is pressed, sends an IPC message to the application core
    - If a button is released, sends an IPC message to the application core.

## Building

Open the folder in VS Code with all necessary things installed and hope
for the best. It should work
