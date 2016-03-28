# Buzz LightShirt

A shirt that monitors your posture and encourages deep breathing exercises. An accelerometer allows for easy anytime feedback on how well you're doing.

[Video on Youtube](https://youtu.be/LyU4IyufY14)

The project has the following features

* Continuously monitor the wearer's posture and inform them using vibrations if corrective action is needed.
* At regular pre-defined intervals, initiate a deep breathing exercise. This can also be manually triggered using a push button.
* Track the performance of the above 2 parameters and communicate this to the user in the form of an LED.

Components used:

* Lilypad mainboard
* RGB LED
* Vibration motor
* Lilypad accelerometer ADXL335

## Repository Structure

The sketch under `buzzlightshirt` is the main program.
`test-sketches` contains code written in the intermediate steps for testing various components.
