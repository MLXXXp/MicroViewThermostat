# A thermostat using the Sparkfun MicroView and a DS18S20 or DS18B20

By Scott Allen

Based on code from http://bildr.org/2011/07/ds18b20-arduino/

## Description

A simple thermostat using the [SparkFun MicroView](https://www.sparkfun.com/products/12923) as a controller and display. Temperature is read from an attached [DS18S20](https://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/DS18S20.html) or [DS18B20](https://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/DS18B20.html) 1-Wire temperature sensor IC. Heater(s) can be attached to output(s) to maintain a temperature within a specified range.

## Usage

Temperatures and pin assignments are set in values near the start of the sketch.

Two output pins are used to control heater(s). The pins will always be in same state with respect to each other. Two pins are provided for redundancy, allowing two separate circuits in case one fails.

In addition to the heater *on* and *off* temperature settings, separate high and low *alert* temperatures can be set. If the temperature goes outside the range of the *alert* temperatures, the display will flash with a *TOO HIGH!* or *TOO LOW!* message added.

### Note:

The font used to display the temperature doesn't include a minus sign. Only positive temperatures will display properly unless a different font is used.

## Required Libraries

[MicroView.h](https://github.com/sparkfun/SparkFun_MicroView_Arduino_Library/)

[OneWire.h](https://github.com/PaulStoffregen/OneWire)

## License

Copyright (c) 2016 Scott Allen

Copyright (c) 2010 bildr community

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

