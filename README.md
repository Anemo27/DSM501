# DSM501 Arduino Library

![Arduino](https://img.shields.io/badge/Arduino-Compatible-brightgreen.svg)

This Arduino library provides support for working with the DSM501 particulate matter sensor. It allows you to easily interface with the sensor and read particulate matter concentrations.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Example](#example)
- [Contributing](#contributing)
- [License](#license)

## Installation

1. Download this repository as a ZIP file.
2. In the Arduino IDE, go to `Sketch > Include Library > Add .ZIP Library...`.
3. Select the downloaded ZIP file.
4. The library is now installed and ready to use.

## Usage

1. Include the library at the top of your Arduino sketch:

   ```cpp
   #include "DSM501.h"

   ```

2. Create an instance of the DSM501 class and specify the pin number and an interrupt handler function:

   ```cpp
   int sensorPin = 2;  // Replace with your actual sensor pin
   DSM501 dsm501(sensorPin, yourInterruptHandler);

   ```

3. Implement your interrupt handler function. This function will be called on each state change of the sensor's output pin:
   ```cpp
   void yourInterruptHandler() {
   dsm501.handleInterrupt();
   }
   ```
4. In your Arduino setup() function, initialize the sensor:

   ```cpp
   void setup() {
   Serial.begin(9600);
   // Other setup code...
   dsm501.isReady(); // Warm-up the sensor
   }
   ```

5. In your loop, you can read particulate matter concentrations:

   ```cpp
   void loop() {
   float pmConcentration = dsm501.readPM();
   // Do something with the concentration value (e.g., display it or send it over a network)
   delay(1000); // Adjust the delay as needed
   }
   ```

## Example

For a complete example sketch, check the examples folder in this repository.

## Contributing

If you would like to contribute to this library, please create a fork of the repository, make your changes, and submit a pull request. Your contributions are greatly appreciated!

## License

This library is released under the MIT License.
