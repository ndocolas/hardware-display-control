# Hardware Display Control System

This project implements a **low-level hardware control system** in **C**
using memory-mapped registers. It allows users to interact with a
simulated display and LED module by writing and reading values directly
from a binary-mapped file, providing full control over display state,
refresh rate, colors, LED operation, battery status, and other system
parameters.

## Features

-   Direct manipulation of **memory-mapped registers** for
    hardware-level control.
-   Interactive **command-line interface** for managing:
    -   Display state (ON/OFF)
    -   Display modes (static, sliding, blinking, combined)
    -   Refresh rate configuration
    -   LED operational state and RGB LED colors
    -   Battery level monitoring
    -   Temperature readings
    -   Display word customization.
-   Includes functions for both **writing values** and **reading
    real-time hardware state**.

## Project Structure

-   **register.c** → Main entry point, memory mapping setup, and
    initialization of registers.
-   **hardware.c** → Implementation of register manipulation functions
    and user interface menu.
-   **hardware.h** → Header file with function prototypes and register
    mappings.
-   **Makefile** → Automates compilation, binary creation, and library
    generation.

## Compilation & Execution

To compile the project, simply run:

``` bash
make
```

This command automatically compiles the program and generates the
executable:

``` bash
./exe
```

The system provides an intuitive terminal menu for interacting with the
display and LEDs.

## Building Static and Dynamic Libraries

To generate the requested libraries, execute:

``` bash
make lib
```

This will compile and generate both: - **libhardware.a** → Static
library - **libhardware.so** → Shared library

## Cleaning Build Files

To remove the compiled files and reset the build environment:

``` bash
make clean
```

To also remove the generated libraries:

``` bash
make clear
```

## Technologies Used

-   **C (GCC)** for low-level implementation
-   **Memory-mapped file operations** (`mmap`, `munmap`)
-   **Bitwise operations** for register control
-   **Makefile** for build automation

## License
This project is open-source. See the [LICENSE](LICENSE) file for more details.