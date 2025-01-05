**\[Preview\] How to use [_FLOSS_](https://wikipedia.org/wiki/FLOSS) code to produce autonomous [_Arduino_](https://wikipedia.org/wiki/Arduino) / [_Elegoo_](https://www.elegoo.com/blogs/arduino-projects) tools**

\[[This post](./ArduinoElegooTools.md) is [released through *Creative Commons Generic Attribution 2* (which allows all uses)](https://creativecommons.org/licenses/by/2.0/). **Notice**: not affiliated to _Arduino_, nor to _Elegoo_.\] \[This post is a work-in-progress.\]

# Table of Contents
- [Licenses](#licenses) (permissive enough to ignore.)
- [Intro](#intro)
- [Howto route](#howto-route)
  - [Group route pseudocode](#group-route-pseudocode)
  - [Wheeled _Arduino_/_Elegoo_ robot code](#wheeled-arduinoelegoo-robot-code)
  - [Limbed _Arduino_/_Elegoo_ robot code](#limbed-arduinoelegoo-robot-code)
- [Accelerometer / gyroscope (**IMU**)](#accelerometer--gyroscope-imu)
  - [**IMU** as **GPS** / magnetometer substitute](#imu-as-gps--magnetometer-substitute)
  - [**IMU** as kinematic collision detection](#imu-as-kinematic-collision-detection)
- [Howto produce point clouds](#howto-produce-point-clouds)
  - [Howto use point clouds to route](#howto-use-point-clouds-to-route)
- [Howto produce distortion coefficients](#howto-produce-distortion-coefficients)
  - [Howto use coefficients for distortion correction](#howto-use-coefficients-for-distortion-correction)
- [Source code which uses **ROS**](#source-code-which-uses-ROS)
  - [**ROS** fallover prevention](#ROS-fallover-prevention)
- [Individual components, whole tools](#individual-components-whole-tools)
  - [Stepper motors versus servos](#stepper-motors-versus-servos)
  - [Different types / uses of servos](#different-types--uses-of-servos)
  - [Gear reducers](#gear-reducers)
  - [Circuitboard / CPU lists](#circuitboard--CPU-lists)
- [Synopsis + resources](#synopsis--resources)
  - [Preview](#synopsis-preview)

******************************************
# Licenses
As the author of `./posts/ArduinoElegooTools.md` (henceforth **this document**), release all of **this document** (such as concepts or code) to all humans or computers (henceforth **users**) for all uses; the sole restriction is that all of **this document** which does not now fall under patents, **users** shall not patent those from now on, but shall allow other **users** to use those (without additional restrictions).

Assert that am the sole author (have asked the digital _Assistant_ about ownership, plus [_Assistant_ says that discussions with _Assistant_ do not belong to _Assistant_'s webhost, but belong to who uses _Assistant_ to produce thus](https://poe.com/s/gxaODiT6PRnObHHPXE9n)) of **this document** (so am allowed to have all patent/copyright restrictions canceled, plus to allow all to use,) plus choose do so (to allow all humans to use **this document**).
- **users** who wish to use **this document** as-is, can do so.
- **users** who wish to use **this document** through formal licenses, can choose to use: through [Creative Commons Generic Attribution 2.0](https://creativecommons.org/licenses/by/2.0/), or through [**GPLv2**](https://github.com/SwuduSusuwu/SusuLib/blob/trunk/LICENSE_GPLv2) or through [_Apache 2_](https://github.com/SwuduSusuwu/SusuLib/blob/trunk/LICENSE), or through some combo of thus.

## Disclaimer
Other than documents at `swudususuwu.substack.com`, am not responsible for contents of additional referenced documents -- such as those from other hosts {`wikipedia.org`, `poe.com`, `docs.arduino.cc`, `docs.opencv.com`, `www.mathworks.com`, `www.structuralbasics.com`, `play.google.com`, `micropython.org`, `randomnerdtutorials.com`, `realpython.org`, `amazon.com`, `espressif.com`}.
- Am not connected to those other hosts.
- Am not connected to those other documents' authors.

******************************************
# Intro
This document's topics are difficult (lots of calculus, **C++**, plus tool-specific [serial protocols](https://docs.arduino.cc/learn/communication/wire/)).

Suitable intros for [**C++**](https://wikipedia.org/wiki/C++) on _Arduino_ boards (works similar on _Elegoo_ boards):
- [Learn Arduino Programming](https://play.google.com/store/apps/details?id=com.codeworld.learnarduinoprogramming); the most simple, for those who have not used circuit boards or programmed computers.
- [Arduino Programming Tutorial](https://play.google.com/store/apps/details?id=com.algosoftware.arduinoexamples); for those familiar with computers but who have not used boards such as _Arduino_ or _Elegoo_.
- [Arduino Handbook 2](https://play.google.com/store/apps/details?id=com.oleksandrdovhaliuk.ard_hb); offline resource book for those who have used boards such as _Arduino_ or _Elegoo_.

Am not sure the reason that _Assistant_ suggests so much _Python_ code, but it is possible to use such as-is (do not have to convert such _Python_ code into _C++_ code).

Intros for [**Python**](https://wikipedia.org/wiki/Python_(programming_language)) control of _Arduino_ boards (works similar on _Elegoo_ boards):
- <https://docs.arduino.cc/micropython/>; official tutorial about how to execute **Python** scripts on _Arduino_ boards through [_MicroPython_](https://micropython.org/).
  - Requires 256kB **flash** (disk) plus 16kB **RAM** (core mem) on the _Arduino_ (or _Elegoo_) board to use, but does not rely on personal computers to execute.
- [_Arduino_ With Python: How to Get Started](https://realpython.com/arduino-python/);  simple tutorial about how to use [_pyFirmata_](https://github.com/tino/pyFirmata) scripts on computers to control the [_Firmata_ firmware](https://github.com/firmata/arduino?tab=readme-ov-file#firmata) on _Arduino_ (or _Elegoo_) boards.
  - Requires continuous serial connection (such as [**USB**](https://wikipedia.org/wiki/USB) or [**Ethernet**](https://wikipedia.org/wiki/Ethernet)) to the personal computer, to execute.

If neither _Arduino_ (nor compatible clones such as _Elegoo_) are used, must [setup _Arduino IDE_ (or _Arduino IDE 2_) to support the board used](https://support.arduino.cc/hc/en-us/articles/360016466340-Add-third-party-platforms-to-the-Boards-Manager-in-Arduino-IDE#add-or-remove).
- For example, if the board used is [_Raspberry Pi Pico_, _Pico W_](https://randomnerdtutorials.com/programming-raspberry-pi-pico-w-arduino-ide/), _Adafruit Feather RP2040_ (or other _RP2040_ module), set `Additional Boards Manager URLs` to `https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json`

Tools compatible with this howto:
- TODO, but some from [this other post](https://swudususuwu.substack.com/p/how-to-mix-blender-with-robotics) have robotics uses:
  - [Grasshopper](https://wikipedia.org/wiki/Grasshopper_3D)
  - [RoboDK](https://wikipedia.org/wiki/RoboDK)
  - [V-REP/CoppeliaSim](https://wikipedia.org/wiki/CoppeliaSim)
  - [Gazebo](https://wikipedia.org/wiki/Gazebo)
- ![_Arduino Uno_ circuitboard](https://upload.wikimedia.org/wikipedia/commons/3/38/Arduino_Uno_-_R3.jpg)

******************************************
# Howto route
\[**Notice**\]: _Arduino_ (plus compatible _Elegoo_) circuitboard pins output at 5v with a max of 40milliamps, which must go to the stepper motor's (or servo's) control signal; [if used to power large motors, the circuitboard's internal resistors are damaged, so you must purchase a new circuitboard](https://docs.arduino.cc/learn/microcontrollers/digital-pins/#:~:text=milliamps,maximum).
- For motors without control signals (motors with just 2 pins, "+" plus "-"), attach the circuitboard's output pins to the control "gate"s ("base"s) of power transistors, plus attach those transistors' "emitter"s ("source"s) to "ground" of such motors, with transistors' "collector"s attached to the external power source's "ground".
  - Motors <500,000 volts (plus <200 amps) can use [power **MOSFET**s](https://wikipedia.org/wiki/Power_MOSFET) (which have a minimum gate (base) control voltage of 3v, so you must ensure that the output pin does not power other circuits which cause more than 1 volt droop).
  - Motors >500,000 volts (or >500 amps) must use [insulated-gate bipolar transistors](https://wikipedia.org/wiki/Insulated-gate_bipolar_transistor) (which have a minimum gate control voltage of 4v, so you must ensure that the output pin does not also power status indicators or other circuits which can cause voltage drop).

What follows is [code from _Assistant_](https://poe.com/s/bkHWeb6vWGFf3CdUzIch) (just examples of what this post is about; future versions of this post will include own code, once am more experienced with robotic tools).
- Just **this document** (`./posts/ArduinoElegooTools.md`) includes _Assistant_-produced code; all other [`/SusuLib/posts`](./README.md) / [code](../cxx/README.md) was human-produced). All code was human-reviewed.

## Group route pseudocode
```
// Define the nanobot structure
class Nanobot {
	// Attributes
	location: Coordinate
	target: Coordinate
	energy: Float
	taskQueue: List<Task>
	status: String // "idle", "working", "charging"
	velocity: Vector
	neighbors: List<Nanobot>
	plannedRoute: List<Coordinate>

	// Constructor
	function Nanobot(initialLocation) {
		this.location = initialLocation
		this.target = None
		this.energy = INITIAL_ENERGY
		this.taskQueue = []
		this.status = "idle"
		this.velocity = Vector(0, 0)
		this.neighbors = []
		this.plannedRoute = []
	}

	// Function to move to a target location
	function moveTo(targetLocation) {
		// Set target for the movement
		this.target = targetLocation

		// Calculate direction vector
		direction = this.target - this.location
		direction = direction.normalized() // Normalize the direction vector

		// Calculate desired velocity based on direction and speed
		desiredVelocity = direction * MAX_SPEED

		// Update kinematics with a simple linear model
		this.velocity = desiredVelocity
		this.location += this.velocity * TIME_STEP // Update position based on time step

		// Check for potential collisions
		if detectCollision() {
			// If a collision is detected, adjust the velocity
			adjustVelocityForCollision()
		}

		// Share planned route with neighbors
		sharePlannedRoute()
	}

	// Collision detection function with route checking
	function detectCollision() {
		// Check against planned routes of neighbors
		for neighbor in this.neighbors {
			if isRouteIntersecting(neighbor.plannedRoute) {
				return true
			}
		}
		return false
	}

	// Check if this bot's planned route intersects with another bot's route
	function isRouteIntersecting(otherRoute) {
		for point in otherRoute {
			if distance(point, this.location) < COLLISION_THRESHOLD {
				return true
			}
		}
		return false
	}

	// Adjust velocity to avoid collision by planning alternate routes
	function adjustVelocityForCollision() {
		// Simple avoidance strategy: find an alternative route
		newRoute = computeAlternativeRoute()
		this.plannedRoute = newRoute
		// Adjust the velocity to follow the new route
		this.velocity = (this.plannedRoute[0] - this.location).normalized() * MAX_SPEED
	}

	// Compute an alternative route based on the current position and target
	function computeAlternativeRoute() {
		// Logic to compute a new route avoiding known routes
		// Placeholder for route-planning algorithm
		return calculateRoute(this.location, this.target, avoidNeighbors=true)
	}

	// Share planned route with neighbors
	function sharePlannedRoute() {
		routeData = {location: this.location, plannedRoute: this.plannedRoute}
		transmit("route_update", routeData)
	}

	// Additional methods remain the same...
}

// Main loop for all nanobots
function main() {
	nanobots = initializeNanobots(NUM_NANOBOTS)
	while true {
		for bot in nanobots {
			bot.scanForNeighbors() // Update neighbor list
			if bot.status == "idle" {
				bot.communicate() // Share tasks and resources
				bot.executeTask() // Execute assigned tasks
			}
			// Move towards assigned target for the current task
			if not bot.taskQueue.isEmpty() {
				target = getTargetForCurrentTask(bot.taskQueue.peek())
				bot.moveTo(target)
			}
		}
		wait(SIMULATION_TIME_STEP) // Control loop frequency
	}
}

// Entry point
main()
```
**Errata**
- `velocity` vector is 2-dimensional; _Assistant_ did not use conversation context to deduce that `velocity` must include altitude.

## Wheeled _Arduino_/_Elegoo_ robot code
```cpp
// Constants
const int LEFT_MOTOR_PIN = 9;    // PWM pin for left motor
const int RIGHT_MOTOR_PIN = 10;  // PWM pin for right motor
const int LEFT_ENCODER_PIN = 2;  // Interrupt pin for left encoder
const int RIGHT_ENCODER_PIN = 3; // Interrupt pin for right encoder

// Kinematics parameters
const float WHEEL_RADIUS = 0.02; // Wheel radius in meters
const float WHEEL_BASE = 0.1;    // Distance between wheels in meters
const float MAX_SPEED = 255;     // Max PWM value

// Encoder counts
volatile int leftEncoderCount = 0;
volatile int rightEncoderCount = 0;

// Setup function
void setup() {
	pinMode(LEFT_MOTOR_PIN, OUTPUT);
	pinMode(RIGHT_MOTOR_PIN, OUTPUT);
	pinMode(LEFT_ENCODER_PIN, INPUT_PULLUP);
	pinMode(RIGHT_ENCODER_PIN, INPUT_PULLUP);

	// Attach interrupts for encoder counting
	attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN), leftEncoderISR, RISING);
	attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN), rightEncoderISR, RISING);

	Serial.begin(9600); // For debugging
}

// Interrupt Service Routines for encoders
void leftEncoderISR() {
	leftEncoderCount++;
}

void rightEncoderISR() {
	rightEncoderCount++;
}

// Function to set motor speeds
void setMotorSpeeds(int leftSpeed, int rightSpeed) {
	analogWrite(LEFT_MOTOR_PIN, constrain(leftSpeed, 0, MAX_SPEED));
	analogWrite(RIGHT_MOTOR_PIN, constrain(rightSpeed, 0, MAX_SPEED));
}

// Function to calculate the distance traveled by the wheels
float calculateDistance(int encoderCount) {
	return (encoderCount * 2 * PI * WHEEL_RADIUS) / ENCODER_COUNTS_PER_REV; // Adjust ENCODER_COUNTS_PER_REV
}

// Main loop
void loop() {
	// Example: Move forward
	setMotorSpeeds(200, 200); // Set both motors to move forward at speed 200

	// Simulate some actions
	delay(2000); // Move forward for 2 seconds

	// Stop the motors
	setMotorSpeeds(0, 0);

	// Read encoders for position feedback
	float leftDistance = calculateDistance(leftEncoderCount);
	float rightDistance = calculateDistance(rightEncoderCount);

	// Print distances for debugging
	Serial.print("Left Distance: ");
	Serial.print(leftDistance);
	Serial.print(" m, Right Distance: ");
	Serial.print(rightDistance);
	Serial.println(" m");

	// Reset encoder counts
	leftEncoderCount = 0;
	rightEncoderCount = 0;

	// Additional logic for movement can be added here
}
```
**Errata**
- `calculateDistance` is 2-dimensional; _Assistant_ did not use conversation context to deduce that `calculateDistance` must include altitude.

## Limbed _Arduino_/_Elegoo_ robot code
```cpp
// Constants for motor pins
const int LEFT_JOINT_PIN = 9;   // PWM pin for left joint motor
const int RIGHT_JOINT_PIN = 10; // PWM pin for right joint motor
const int ELBOW_JOINT_PIN = 11; // PWM pin for elbow joint motor
const int BASE_JOINT_PIN = 12;  // PWM pin for base joint motor

// Encoder pins
const int LEFT_ENCODER_PIN = 2;  // Interrupt pin for left joint encoder
const int RIGHT_ENCODER_PIN = 3; // Interrupt pin for right joint encoder
const int ELBOW_ENCODER_PIN = 4; // Interrupt pin for elbow joint encoder
const int BASE_ENCODER_PIN = 5;  // Interrupt pin for base joint encoder

// Inverse kinematics parameters
const float LIMB_LENGTH_1 = 0.1; // Length of first limb in meters
const float LIMB_LENGTH_2 = 0.1; // Length of second limb in meters

// Encoder counts
volatile int leftEncoderCount = 0;
volatile int rightEncoderCount = 0;
volatile int elbowEncoderCount = 0;
volatile int baseEncoderCount = 0;

// Setup function
void setup() {
	pinMode(LEFT_JOINT_PIN, OUTPUT);
	pinMode(RIGHT_JOINT_PIN, OUTPUT);
	pinMode(ELBOW_JOINT_PIN, OUTPUT);
	pinMode(BASE_JOINT_PIN, OUTPUT);

	pinMode(LEFT_ENCODER_PIN, INPUT_PULLUP);
	pinMode(RIGHT_ENCODER_PIN, INPUT_PULLUP);
	pinMode(ELBOW_ENCODER_PIN, INPUT_PULLUP);
	pinMode(BASE_ENCODER_PIN, INPUT_PULLUP);

	// Attach interrupts for encoder counting
	attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN), leftEncoderISR, RISING);
	attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN), rightEncoderISR, RISING);
	attachInterrupt(digitalPinToInterrupt(ELBOW_ENCODER_PIN), elbowEncoderISR, RISING);
	attachInterrupt(digitalPinToInterrupt(BASE_ENCODER_PIN), baseEncoderISR, RISING);

	Serial.begin(9600); // For debugging
}

// Interrupt Service Routines for encoders
void leftEncoderISR() { leftEncoderCount++; }
void rightEncoderISR() { rightEncoderCount++; }
void elbowEncoderISR() { elbowEncoderCount++; }
void baseEncoderISR() { baseEncoderCount++; }

// Function to set joint motor speeds
void setJointSpeeds(int leftSpeed, int rightSpeed, int elbowSpeed, int baseSpeed) {
	analogWrite(LEFT_JOINT_PIN, constrain(leftSpeed, 0, 255));
	analogWrite(RIGHT_JOINT_PIN, constrain(rightSpeed, 0, 255));
	analogWrite(ELBOW_JOINT_PIN, constrain(elbowSpeed, 0, 255));
	analogWrite(BASE_JOINT_PIN, constrain(baseSpeed, 0, 255));
}

// Inverse kinematics to calculate joint angles based on target position
void inverseKinematics(float targetX, float targetY) {
	float distance = sqrt(targetX * targetX + targetY * targetY); // Distance to target

	if (distance > (LIMB_LENGTH_1 + LIMB_LENGTH_2)) {
		// Target unreachable
		Serial.println("Target unreachable");
		return;
	}

	// Calculate angles using the cosine law
	float angle2 = acos((LIMB_LENGTH_1 * LIMB_LENGTH_1 + LIMB_LENGTH_2 * LIMB_LENGTH_2 - distance * distance) /
						(2 * LIMB_LENGTH_1 * LIMB_LENGTH_2));
	float angle1 = atan2(targetY, targetX) - atan2(LIMB_LENGTH_2 * sin(angle2),
													LIMB_LENGTH_1 + LIMB_LENGTH_2 * cos(angle2));

	// Convert to motor PWM values (simplified)
	int leftMotorSpeed = map(angle1 * 180 / PI, -90, 90, 0, 255);
	int rightMotorSpeed = map(angle2 * 180 / PI, -90, 90, 0, 255);

	// Set motor speeds
	setJointSpeeds(leftMotorSpeed, rightMotorSpeed, 0, 0); // Elbow and base not used in this example
}

// Main loop
void loop() {
	// Example target position
	float targetX = 0.1; // Target X position in meters
	float targetY = 0.1; // Target Y position in meters

	inverseKinematics(targetX, targetY); // Calculate joint angles and move

	// Read encoders for position feedback
	Serial.print("Left Encoder: ");
	Serial.print(leftEncoderCount);
	Serial.print(" Right Encoder: ");
	Serial.print(rightEncoderCount);
	Serial.print(" Elbow Encoder: ");
	Serial.print(elbowEncoderCount);
	Serial.print(" Base Encoder: ");
	Serial.println(baseEncoderCount);

	// Reset encoder counts for the next loop
	leftEncoderCount = 0;
	rightEncoderCount = 0;
	elbowEncoderCount = 0;
	baseEncoderCount = 0;

	delay(100); // Adjust delay as needed
}
```
**Errata**
- `inverseKinematics` is 2-dimensional (`targetX` = longitude, `targetY` = lattitude); _Assistant_ "left it as an exercise to you" to have `inverseKinematics` include multiple limbs, plus altitude.

**Solution for altitude** ([_Assistant_ says that my solution will do.](https://poe.com/s/uwjbhSrSWTtenF4HTcAa)):
- For those 2 dimensions above, use `targetX` = distance from base, plus `targetY` = altitude.
- For the third dimension, have a third servo (`targetZ` = radial direction from base) which turns the base around towards the goal.
- This is more simple to compute (versus a normal 3-dimensional solution), which allows use of the $1 _ATmega328P_ **CPU**.

******************************************
# Accelerometer / gyroscope (**IMU**)

## **IMU** as **GPS** / magnetometer substitute
Source code for affordable toolkits (such as [_UNO WiFi Rev2_](https://docs.arduino.cc/hardware/uno-wifi-rev2/)) which have Inertial Management Units (**IMU**'s, such as the [_LSM6DS3TR_ **IMU**](https://www.arduino.cc/reference/en/libraries/arduino_lsm6ds3/)), which can:
- Act as [GPS (map position)](https://wikipedia.org/wiki/GPS) substitutes; accelerometer is sufficient to act as relative GPS (plus, if the position at startup is known, gyroscopes allow to deduce true lattitude / longitude / altitude).
- Act as [magnetometer (compass)](https://wikipedia.org/wiki/Magnetometer) substitutes; gyroscope is sufficient to act as relative compass (plus, if the orientation at startup is known, gyroscopes can substitute true compasses).

As with the other source code in this document, due to lack of personal experience with robotics [this is from a digital assistant (but is not a blind reuse of _Assistant_'s first answer; the discussion shows numerous iterative improvements)](https://poe.com/s/0ZfEWQWdEg6368kfIoTN).
```c++
// Include necessary libraries
#include <Wire.h>
#include <Adafruit_MPU6050.h>

// Create an instance of the MPU6050 sensor
Adafruit_MPU6050 mpu;

// Variables to store the total accumulated rotation, velocity, and position
float totalRotation = 0.0;
float relativeVelocityX = 0.0;
float relativeVelocityY = 0.0;
float relativeVelocityZ = 0.0;
float relativePositionX = 0.0;
float relativePositionY = 0.0;
float relativePositionZ = 0.0;

// Variable to store the previous timestamp
unsigned long previousTimestamp = 0;

void setup() {
	// Initialize serial communication
	Serial.begin(9600);

	// Initialize the MPU6050 sensor
	if (!mpu.begin()) {
		Serial.println("Failed to initialize MPU6050 sensor!");
		while (1);
	}

	// Configure the sensor settings
	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);
	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

	// Get the initial timestamp
	previousTimestamp = millis();
}

void loop() {
	// Read the sensor data
	sensors_event_t a, g, temp;
	mpu.getEvent(&a, &g, &temp);

	// Calculate the change in heading (in radians)
	float headingChange = atan2(g.gyro.y, g.gyro.x);

	// Accumulate the total rotation
	totalRotation += headingChange;

	// Calculate the time elapsed since the previous update
	unsigned long currentTimestamp = millis();
	float elapsedTime = (currentTimestamp - previousTimestamp) / 1000.0; // in seconds
	previousTimestamp = currentTimestamp;

	// Update the relative velocity and position
	relativeVelocityX += a.acceleration.x * elapsedTime;
	relativeVelocityY += a.acceleration.y * elapsedTime;
	relativeVelocityZ += a.acceleration.z * elapsedTime;

	relativePositionX += relativeVelocityX * elapsedTime;
	relativePositionY += relativeVelocityY * elapsedTime;
	relativePositionZ += relativeVelocityZ * elapsedTime;

	// Convert the total rotation to degrees
	float absoluteDirection = (totalRotation * 180.0) / M_PI;

	// Normalize the absolute direction to be between 0 and 360 degrees
	if (absoluteDirection < 0) {
		absoluteDirection += 360.0;
	}

	// Print the absolute compass direction, relative position, and relative velocity
	Serial.print("Absolute Compass Direction: ");
	Serial.print(absoluteDirection);
	Serial.println(" degrees");

	Serial.print("Relative Position: (");
	Serial.print(relativePositionX);
	Serial.print(", ");
	Serial.print(relativePositionY);
	Serial.print(", ");
	Serial.print(relativePositionZ);
	Serial.println(") units");

	Serial.print("Relative Velocity: (");
	Serial.print(relativeVelocityX);
	Serial.print(", ");
	Serial.print(relativeVelocityY);
	Serial.print(", ");
	Serial.print(relativeVelocityZ);
	Serial.println(") units/s");

	// Delay for a short time
	delay(100);
}
```

## **IMU** as kinematic collision detection
Accelerometer / gyroscope output which does not match expected values for the current servo mode, must cause the robot to reverse or power down.
This is my version of [_Assistant_'s code for **IMU**-based collision detection](https://poe.com/s/auXNL8T44ofo3YRekdlv) (after close to 42 iterative improvements through prompts to _Assistant_, rewrote some of the functions on own).
- The referenced discussion also has code (not included below, as the simple `expected{Reverse, Forward}{Acceleration, Gyroscope}` matrices reduce resource use) to produce full kinematics models (positions, plus orientations of all segments/hinges) through calibration with **IMU**s.
```python
"""
This code assumes that `Servo` elements of `servos` represent actuators (such as stepper motors or servos) which have some distal order (sequential -- from most anterior ergo proximal, to most posterior ergo distal -- which the code can use IMU measurements to compute):
The `anchorPointAcceleration` (plus `anchorPointGyroscope`) variables store the cumulative motion of the previous servos, which is then subtracted from the current servo's IMU. This corrected IMU value is used to detect deviation of the current servo's IMU from reference values (such deviation should trigger autonomous stops, or slowdown).
TODO: compute distal sequences for numerous limbs (allow multiple branches of servos).
"""
class ServoDirection(Enum):
	REVERSE = -1 # Set when `servo.write(angle)` with `angle < currentAngle(servo)`.
	HALT = 0 # Set when `servo.write(angle)` has finished (when servo stops).
	FORWARD = 1 # Set when `servo.write(angle)` with `angle > currentAngle(servo)`.

class Servo:
	def __init__(self, pin, minAngle, maxAngle, segmentRadius = 0.0, segmentLength = 0.0, direction = ServoDirection.HALT):
		self.pin = pin # First value to `pinMode` value`.
		self.minAngle = minAngle # Minimum `servo.write` value.
		self.maxAngle = maxAngle # Maximum `servo.write` value.
		self.segmentRadius = segmentLength # Optional (unused for now). Radius on segment which attaches `self` to to posterior `Servo` (or, if `self` is the last of the limb's `Servo`s, distance to end of limb). Boards with good CPUs can use this to compute collision avoidance.
		self.segmentLength = segmentLength # Optional (can compute). Distance from `self` to posterior `Servo` (or, if `self` is the last of the limb's `Servo`s, distance to end of limb). Used for inverse kinematics (to compute angles to
		self.direction = direction # Set to `ServoDirection` when `servo` moves or stops.
kinematicDependence = false # Whether or not `servos` (as passed to `detectDeviationsLoop`) has members whose IMUs are affected by other member's IMUs.

def getServoByPin(servos, servoPin)
	return next((servo for servo in servos if servo.pin == servoPin), None)

def moveServoThroughRangeAndCaptureImu(servo):
	"""
	Moves the servo through its full range of motion and captures the IMU data.

	Args:
		servo (Servo): The servo object to be moved.

	Returns:
		list: A list of dictionaries, where each dictionary contains the IMU data (acceleration and gyroscope) at a specific servo position.
	"""
	imuSamples = []

	# Move the servo from minimum to maximum position
	servo.write(0)
	servo.direction = ServoDirection.REVERSE
	imuSamples.append(readImuData(servo.pin))

	servo.write(180)
	servo.direction = ServoDirection.FORWARD
	imuSamples.append(readImuData(servo.pin))

	# Move the servo from maximum to minimum position
	servo.write(180)
	servo.direction = ServoDirection.HALT
	imuSamples.append(readImuData(servo.pin))

	servo.write(0)
	servo.direction = ServoDirection.REVERSE
	imuSamples.append(readImuData(servo.pin))

	servo.direction = ServoDirection.HALT
	return imuSamples

def detectAffectedImus(imuSamples, accelerometerThreshold, gyroscopeThreshold):
	"""
	Detects which other IMUs are affected by the motion of the current servo.

	Args:
#		servos (list): A list of Servo objects, representing the servos in the robot.
		imuSamples (list): The IMU data captured during the servo's range of motion.
		accelerometerThreshold (float): The threshold for the accelerometer data.
		gyroscopeThreshold (float): The threshold for the gyroscope data.

	Returns:
		list: A list of pin numbers for the IMUs that were affected by the motion of the current servo.
	"""
	affectedImuPins = []
	for i, sample in enumerate(imuSamples):
		if isAccelerationOutOfRange(sample["acceleration"], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], ServoDirection.HALT, anchorPointAcceleration, accelerometerThreshold):
			affectedImuPins.append(i)
 #			servos[i].mobileAnchorPoint = true # Alternative to global `kinematicDependence` variable
		if isGyroscopeOutOfRange(sample["gyroscope"], [0.0, 0.0, 0.0], [0.0, 0.0, 0.0], ServoDirection.HALT, anchorPointGyroscope, gyroscopeThreshold):
			affectedImuPins.append(i)
 #			servos[i].mobileAnchorPoint = true
	return affectedImuPins

def calibrateRobotKinematics(servos, accelerometerThreshold=0.02, gyroscopeThreshold=0.02):
	"""
	Calibrates the robot kinematics by capturing IMU data as the servos move through their full range of motion.
	The function also detects the order of the servos by observing which IMUs are affected by the motion of each servo.

	Args:
		servos (list): A list of Servo objects, representing the servos in the robot.
		accelerometerThreshold (float): The threshold for the accelerometer data (default is 2%).
		gyroscopeThreshold (float): The threshold for the gyroscope data (default is 2%).

	Returns:
		imuLookupTable (dict): A dictionary that maps servo pin numbers to their expected IMU outputs.
		servoOrder (list): A list of servo pin numbers in the order they are connected (from base to tip).
	"""
	imuLookupTable = {}
	servoOrder = []

	for servo in servos:
		# Setup board pinout
		pinMode(servo.pin, OUTPUT);

		# Move the servo through its full range of motion and capture the IMU data
		imuSamples = moveServoThroughRangeAndCaptureImu(servo)

		# Compute the expected acceleration and gyroscope values
		expectedForwardAcceleration = computeExpectedAcceleration(imuSamples, True)
		expectedReverseAcceleration = computeExpectedAcceleration(imuSamples, False)
		expectedForwardGyroscope = computeExpectedGyroscope(imuSamples, True)
		expectedReverseGyroscope = computeExpectedGyroscope(imuSamples, False)

		# Store the expected IMU outputs in the lookup table
		imuLookupTable[servo.pin] = {
			"forwardAcceleration": expectedForwardAcceleration,
			"reverseAcceleration": expectedReverseAcceleration,
			"forwardGyroscope": expectedForwardGyroscope,
			"reverseGyroscope": expectedReverseGyroscope,
			"accelerometerThreshold": accelerometerThreshold,
			"gyroscopeThreshold": gyroscopeThreshold
		}
		# Detect which other IMUs are affected by the motion of this servo, and update the servo order
		anchorPointAcceleration = [0.0, 0.0, 0.0] # Do not subtract this servo's acceleration
		anchorPointGyroscope = [0.0, 0.0, 0.0] # Do not subtract this servo's gyroscope
		affectedImuPins = detectAffectedImus(imuSamples, anchorPointAcceleration, anchorPointGyroscope, accelerometerThreshold, gyroscopeThreshold)
		if 0 != len(affectedImuPins:
			kinematicDependence = true # Enables extra processing in `is*OutOfRange()`.
		servoOrder.insert(servoOrder.index(servo.pin) + 1, *affectedImuPins) # If `servoOrder` has `affectedImuPins`, this should preserve their relative order but move them past the current servo.

	return imuLookupTable, servoOrder

def detectDeviations(servo, imuLookupTable, currentImuData, anchorPointAcceleration, anchorPointGyroscope):
	"""
	Detects deviations in the current IMU data by comparing it to the expected values in the lookup table.

	Args:
		servo (Servo): The instance of the servo being controlled.
		imuLookupTable (dict): The lookup table generated by the `calibrateRobotKinematics` function.
		currentImuData (dict): The current IMU data, containing the acceleration and gyroscope values.
		anchorPointAcceleration (list): The total instant acceleration of the previous servos.
		anchorPointGyroscope (list): The total instant gyroscope of the previous servos.

	Returns:
		bool: True if an deviation is detected, False otherwise.
	"""
	# Check if the current IMU data matches the expected values in the lookup table
	expectedImu = imuLookupTable[servo.pin]
	if servoDirection == ServoDirection.FORWARD:
		expectedAcceleration = expectedImu["forwardAcceleration"]
		expectedGyroscope = expectedImu["forwardGyroscope"]
	elif servoDirection == ServoDirection.REVERSE:
		expectedAcceleration = expectedImu["reverseAcceleration"]
		expectedGyroscope = expectedImu["reverseGyroscope"]
	else: # servoDirection == ServoDirection.HALT:
		expectedAcceleration = [0.0, 0.0, 0.0]
		expectedGyroscope = [0.0, 0.0, 0.0]
	accelerometerThreshold = expectedImu["accelerometerThreshold"]
	gyroscopeThreshold = expectedImu["gyroscopeThreshold"]

	# Implement your deviation detection logic here
	if isAccelerationOutOfRange(currentImuData["acceleration"], expectedAcceleration, anchorPointAcceleration, accelerometerThreshold):
		return True  # Acceleration deviation detected

	if isGyroscopeOutOfRange(currentImuData["gyroscope"], expectedGyroscope, anchorPointGyroscope, gyroscopeThreshold):
		return True  # Gyroscope deviation detected

	return False

def detectDeviationsLoop(servos, servoOrder, imuLookupTable, currentImuData, kinematicDependence):
	"""
	Computes total motion from IMUs and uses `detectDeviations()` on all servos.

	Args:
		servos (list): The list of attached servos.
		servoOrder (list): The order of the servos, from base to tip.
		imuLookupTable (dict): The lookup table generated by the `calibrateRobotKinematics` function.
		currentImuData (dict): The current IMU data, containing the acceleration and gyroscope values.

	Returns:
		list: if deviations detected returns `[Servo.pin ...]`, if none returns `[]`.
	"""
	anchorPointAcceleration = [0.0, 0.0, 0.0] # The total instant acceleration of the previous servos.
	anchorPointGyroscope = [0.0, 0.0, 0.0] # The total instant gyroscope of the previous servos.
	anomalousServos = []
	for i in range(len(servoOrder)):
		servoPin = servoOrder[i]
		servo = getServoByPin(servos, servoPin)
		imuData = readImuData(servoPin)
		if detectDeviations(servo, imuLookupTable, currentImuData, anchorPointAcceleration, anchorPointGyroscope)
			anomalousServos.append(servo.pin) # Deviation detected
		anchorPointAcceleration = currentImuData["acceleration"]
		anchorPointGyroscope = currentImuData["gyroscope"]
	return anomalousServose

def isAccelerationOutOfRange(currentAcceleration, expectedAcceleration, servoDirection, anchorPointAcceleration, accelerometerThreshold):
	"""
	Checks if the current acceleration is out of the expected range.

	Args:
		currentAcceleration (list): The current acceleration values in the X, Y, and Z axes.
		expectedAcceleration (list): The expected acceleration values in the X, Y, and Z axes for ServoDirection motion.
		servoDirection (ServoDirection): The current direction of the servo (reverse, halt, or forward).
		anchorPointAcceleration (list): The instant acceleration of the anchor point.
		accelerometerThreshold (float): The threshold for the accelerometer data.

	Returns:
		bool: True if the acceleration is out of the expected range, False otherwise.
	"""
	if kinematicDependence:
		for i in range(3):
			currentAcceleration[i] -= anchorPointAcceleration[i]
	if sqrt(currentAcceleration[0] ** 2 + currentAcceleration[1] ** 2 + currentAcceleration[2] ** 2) > accelerometerThreshold:
		return True
	return False

def isGyroscopeOutOfRange(currentGyroscope, expectedGyroscope, anchorPointGyroscope, gyroscopeThreshold):
	"""
	Checks if the current gyroscope is out of the expected range.

	Args:
		currentGyroscope (list): The current gyroscope values in the X, Y, and Z axes.
		expectedGyroscope (list): The expected gyroscope values in the X, Y, and Z axes for ServoDirection motion.
		anchorPointGyroscope (list): The instant gyroscope of the anchor point.
		gyroscopeThreshold (float): The threshold for the gyroscope data.

	Returns:
		bool: True if the gyroscope is out of the expected range, False otherwise.
	"""
	if kinematicDependence:
		for i in range(3):
			currentGyroscope[i] -= anchorPointGyroscope[i]
	if sqrt(currentGyroscope[0] ** 2 + currentGyroscope[1] ** 2 + currentGyroscope[2] ** 2) > gyroscopeThreshold:
		return True
	return False
```
**Errata**
- `anchorPointAcceleration` (plus `anchorPointGyroscope`) assume that the anchor-point IMU is close to current servor (thus, IMU at distal tip of segment). If this assumption does not hold, use a new kinematic model (such as the more compute-intensive realistic kinematics model from the referenced discussion with _Assistant_) to compute those.
  - If there is just 1 servo (or if the servos are independent), then `kinematicsDependence = false`, plus the kinematics models (such as the code which uses `anchor*Acceleration`) are redundant (the program will skip those).

******************************************
# Howto produce point clouds
Those [point clouds](https://wikipedia.org/wiki/Point_cloud) show obstacles which autonomous tools must [route around](https://wikipedia.org/wiki/Obstacle_avoidance).

Some pseudocode [from _Assistant_](https://poe.com/s/sx1MuZzPcLy3p9u0elpq) (just examples of what this post is about; future versions of this post will include own code):
## Depth-image to point cloud
Has the most simple code, but requires [advanced sensors](https://www.amazon.com/Consumer-Depth-Cameras-Computer-Vision/dp/1447146395) (such as [LIDAR](https://wikipedia.org/wiki/Lidar)) to produce images.
```python
import numpy as np
import tensorflow as tf
import cv2

def create_point_cloud(color_image, depth_image, fx, fy, cx, cy):
	# Get image dimensions
	height, width, _ = color_image.shape

	# Generate grid of pixel coordinates
	x, y = np.meshgrid(np.arange(width), np.arange(height))

	# Calculate normalized image coordinates
	z = depth_image / 1000.0  # Convert depth to meters
	x = (x - cx) * z / fx
	y = (y - cy) * z / fy

	# Stack to create point cloud
	point_cloud = np.stack((x, y, z), axis=-1)

	# Combine with color information
	point_cloud_rgb = np.concatenate((point_cloud.reshape(-1, 3), color_image.reshape(-1, 3) / 255.0), axis=-1)

	return point_cloud_rgb

# Load your images
color_image = cv2.imread('color_image.png')
depth_image = cv2.imread('depth_image.png', cv2.IMREAD_UNCHANGED)

# Camera parameters (example values)
fx = 525.0  # Focal length in x
fy = 525.0  # Focal length in y
cx = 319.5  # Optical center x
cy = 239.5  # Optical center y

# Create point cloud
point_cloud = create_point_cloud(color_image, depth_image, fx, fy, cx, cy)

# Save or process the point cloud
np.savetxt('point_cloud.txt', point_cloud)
```

## Optical flow processing of visuals into point cloud (limited; static z-axis)
Uses [`cv2.calcOpticalFlowFarneback`](https://docs.opencv.org/4.x/d4/dee/tutorial_optical_flow.html#autotoc_md1162) of [_H264_](https://wikipedia.org/wiki/h264) to produce point cloud (limited; static z-axis)
```python
import cv2
import numpy as np
import tensorflow as tf

def calculate_optical_flow(prev_frame, next_frame):
	# Convert frames to grayscale
	prev_gray = cv2.cvtColor(prev_frame, cv2.COLOR_BGR2GRAY)
	next_gray = cv2.cvtColor(next_frame, cv2.COLOR_BGR2GRAY)

	# Calculate optical flow
	flow = cv2.calcOpticalFlowFarneback(prev_gray, next_gray, None, 0.5, 3, 15, 3, 5, 1.2, 0)

	return flow

def create_point_cloud_from_flow(flow, color_frame):
	h, w = flow.shape[:2]
	points = []

	for y in range(h):
		for x in range(w):
			dx, dy = flow[y, x].astype(int)
			z = 1  # Set constant depth or calculate based on your requirements
			point = (x + dx, y + dy, z)  # Adjust based on motion
			color = color_frame[y, x] / 255.0  # Normalize color

			points.append((*point, *color))

	return np.array(points)

# Load video frames (example using a video file)
cap = cv2.VideoCapture('motion_video.mp4')

# Read the first frame
ret, prev_frame = cap.read()
if not ret:
	print("Error reading video")
	exit()

point_clouds = []

while True:
	ret, next_frame = cap.read()
	if not ret:
		break

	# Calculate optical flow
	flow = calculate_optical_flow(prev_frame, next_frame)

	# Create point cloud from flow and color frame
	point_cloud = create_point_cloud_from_flow(flow, next_frame)
	point_clouds.append(point_cloud)

	# Set current frame as previous frame for the next iteration
	prev_frame = next_frame

cap.release()

# Save or process all point clouds
for i, pc in enumerate(point_clouds):
	np.savetxt(f'point_cloud_{i}.txt', pc)
```

## _TensorFlow_: static image to point clouds
[_TensorFlow_](https://tensorflow.org/) computes z-axis, but limited to how accurate [MiDaS (monocular depth estimation)](https://pytorch.org/hub/intelisl_midas_v2/) is.
```python
import tensorflow as tf
import numpy as np
import cv2

# Load a pre-trained depth estimation model (e.g., MiDaS)
model = tf.keras.models.load_model('path_to_your_depth_model.h5')

def estimate_depth(image):
	# Preprocess the image for the model
	input_image = cv2.resize(image, (384, 384))  # Resize as per model requirements
	input_image = np.expand_dims(input_image, axis=0) / 255.0  # Normalize
	depth_map = model.predict(input_image)
	return depth_map[0, :, :, 0]  # Assuming the model outputs a single channel depth map

def create_point_cloud(color_image, depth_map):
	h, w = depth_map.shape
	points = []

	for y in range(h):
		for x in range(w):
			z = depth_map[y, x]  # Get depth value
			if z > 0:  # Only consider valid depth
				point = (x, y, z)  # (x, y, z)
				color = color_image[y, x] / 255.0  # Normalize color
				points.append((*point, *color))

	return np.array(points)

# Load your color image
color_image = cv2.imread('color_image.png')

# Estimate depth from the color image
depth_map = estimate_depth(color_image)

# Create point cloud
point_cloud = create_point_cloud(color_image, depth_map)

# Save the point cloud
np.savetxt('point_cloud.txt', point_cloud)
```
## Optical flow processing of visuals into point cloud (computes z-axis)
*Grok-2* produced [this simplified `g++` version (just a template of optical flow analysis, not suitable to use, but does not require external libs such as `opencv2`)](https://poe.com/s/X7NcH1RmAxhrJJmghTXF), *Solar-Pro-2* produced [this simple `g++` version which uses `calcOpticalFlowFarneback` from `opencv2`](https://poe.com/s/JBMoZAA5SLfEYppQoiap).

**Notice**: what follows is all `python` code [from *Assistant*](https://poe.com/s/sx1MuZzPcLy3p9u0elpq).

Uses [`cv2.calcOpticalFlowFarneback`](https://docs.opencv.org/4.x/d4/dee/tutorial_optical_flow.html#autotoc_md1162) of [_H264_](https://wikipedia.org/wiki/h264) to produce point cloud (computes z-axis)
```python
import cv2
import numpy as np

def calculate_optical_flow(prev_frame, next_frame):
	prev_gray = cv2.cvtColor(prev_frame, cv2.COLOR_BGR2GRAY)
	next_gray = cv2.cvtColor(next_frame, cv2.COLOR_BGR2GRAY)

	# Calculate optical flow using Farneback method
	flow = cv2.calcOpticalFlowFarneback(prev_gray, next_gray, None, 0.5, 3, 15, 3, 5, 1.2, 0)
	return flow

def estimate_depth_from_motion(flow, known_height):
	h, w = flow.shape[:2]
	depth_map = np.zeros((h, w))

	for y in range(h):
		for x in range(w):
			dx, dy = flow[y, x].astype(float)
			# Assuming a simple linear relationship for depth estimation
			depth = known_height / (1 + np.sqrt(dx**2 + dy**2))
			depth_map[y, x] = max(depth, 0)  # Ensure non-negative depth values

	return depth_map

def create_point_cloud(color_frame, flow, depth_map):
	h, w = depth_map.shape
	points = []

	for y in range(h):
		for x in range(w):
			z = depth_map[y, x]
			if z > 0:  # Only consider valid depth
				point = (x, y, z)  # (x, y, z)
				color = color_frame[y, x] / 255.0  # Normalize color
				points.append((*point, *color))

	return np.array(points)

# Load video frames (example using a video file)
cap = cv2.VideoCapture('motion_video.mp4')

# Read the first frame
ret, prev_frame = cap.read()
if not ret:
	print("Error reading video")
	exit()

# Known height of objects in the scene (or average height)
known_height = 1.5  # Example value in meters

point_clouds = []

while True:
	ret, next_frame = cap.read()
	if not ret:
		break

	# Calculate optical flow
	flow = calculate_optical_flow(prev_frame, next_frame)

	# Estimate depth from motion
	depth_map = estimate_depth_from_motion(flow, known_height)

	# Create point cloud
	point_cloud = create_point_cloud(next_frame, flow, depth_map)
	point_clouds.append(point_cloud)

	# Set current frame as previous frame for the next iteration
	prev_frame = next_frame

cap.release()

# Save or process all point clouds
for i, pc in enumerate(point_clouds):
	np.savetxt(f'point_cloud_{i}.txt', pc)
```

## Howto use point clouds to route
Start with [this code (also from _Assistant_)](https://poe.com/s/p0NTuGJRBDlol01YJY5c), which goes into the code from [Howto route](#howto-route):
```python
function detectObstacles(pointCloud):
	for point in pointCloud:
		if isObstacle(point):
			return true  // Obstacle detected
	return false  // No obstacles

function moveTo(targetLocation):
	pointCloud = generatePointCloud()  // Generate or update point cloud
	if detectObstacles(pointCloud):
		adjustPath()  // Modify path to avoid obstacles
	else:
		proceedTo(targetLocation)  // Move to target location
```
```python
function isObstacle(point):
	// Define a threshold distance for obstacle detection
	OBSTACLE_THRESHOLD = 0.5  // Example value in meters

	// Check if the point's z-coordinate (depth) is below the threshold
	if point.z < OBSTACLE_THRESHOLD:
		return true  // Point is an obstacle
	return false  // Point is not an obstacle
function adjustPath():
	// Get the current location and target location
	currentLocation = getCurrentLocation()
	targetLocation = getTargetLocation()

	// Calculate a new target location that avoids the obstacle
	avoidanceVector = computeAvoidanceVector(currentLocation, targetLocation)

	// Set the new target location
	newTargetLocation = targetLocation + avoidanceVector
	setTargetLocation(newTargetLocation)

function computeAvoidanceVector(currentLocation, targetLocation):
	// Calculate the direction vector from current to target location
	direction = targetLocation - currentLocation

	// Normalize the direction vector
	direction = normalize(direction)

	// Create an avoidance vector
	avoidanceDistance = 0.3  // Distance to move away from the obstacle
	avoidanceVector = direction.perpendicular() * avoidanceDistance  // Move perpendicular to the direction

	return avoidanceVector
```
**Errata**
- `computeAvoidanceVector` is 2-dimensional\*; _Assistant_ did not use conversation context to deduce that `computeAvoidanceVector` must include altitude.
  - \*since `direction.perpendicular()` can not go over nor under obstacles.
```python
function main():
	while true:
		pointCloud = generatePointCloud()  // Update point cloud

		// Check for obstacles
		obstacleDetected = false
		for point in pointCloud:
			if isObstacle(point):
				obstacleDetected = true
				break

		if obstacleDetected:
			adjustPath()  // Modify path to avoid obstacles
		else:
			proceedTo(getTargetLocation())  // Move towards target location

		wait(SIMULATION_TIME_STEP)  // Control loop frequency
```
, plus make [_Howto use point clouds to route_](#howto-use-point-clouds-to-route) continuous — such as how (opposed to the [_A\*_ path formula](https://wikipedia.org/wiki/A*_search_algorithm), which must restart if sensor input is new,) the [_D\*_ path formula](https://wikipedia.org/wiki/D*) allows continuous sensor use — the example code uses stored (not continuous) visuals to produce point clouds.

******************************************
# Howto produce distortion coefficients
[_Assistant_ lists how actual autonomous tools post-process the visuals to do distortion correction.](https://poe.com/s/Sj0OyJq1EGBgSZuV4lgV)
```python
// Pseudocode for Estimating Lens Distortion Coefficients

// Step 1: Capture known points and observed points
known_points = loadKnownPoints()  // Load known accurate points (e.g., from a calibration pattern)
observed_points = captureObservedPoints()  // Capture points from the new sensor

// Step 2: Initialize coefficients
k1, k2, k3 = 0.0, 0.0, 0.0
p1, p2 = 0.0, 0.0

// Step 3: Compute error for each point
errors = []
for i from 0 to length(known_points):
	expected_x, expected_y = known_points[i]
	observed_x, observed_y = observed_points[i]

	// Compute radius
	r = sqrt(observed_x^2 + observed_y^2)

	// Compute radial distortion
	radial_distortion = observed_x * (1 + k1 * r^2 + k2 * r^4 + k3 * r^6)
	tangential_distortion_x = observed_x + (2 * p1 * observed_x * observed_y + p2 * (r^2 + 2 * observed_x^2))
	tangential_distortion_y = observed_y + (p1 * (r^2 + 2 * observed_y^2) + 2 * p2 * observed_x * observed_y)

	// Calculate error
	error_x = expected_x - tangential_distortion_x
	error_y = expected_y - tangential_distortion_y
	errors.append((error_x, error_y))

// Step 4: Optimize coefficients based on errors
k1, k2, k3, p1, p2 = optimizeCoefficients(errors)

// Function to optimize coefficients (simplified)
function optimizeCoefficients(errors):
	// Implement optimization algorithm (e.g., least squares)
	// This is where you would use an optimization technique to minimize
	// the total error based on the initial guesses of k1, k2, k3, p1, p2
	return optimized_k1, optimized_k2, optimized_k3, optimized_p1, optimized_p2

// Step 5: Use the coefficients for correction
distortion_coefficients = [k1, k2, p1, p2, k3]

// Now these coefficients can be used in the distortion correction functions
```

## Howto use coefficients for distortion correction
Those coefficients are from [Howto produce distortion coefficients](#howto-produce-distortion-coefficients).
[_Assistant_ lists how actual autonomous tools post-process the visuals to do distortion correction.](https://poe.com/s/Sj0OyJq1EGBgSZuV4lgV)
```python
// Pseudocode for Camera Distortion Correction

// Step 1: Capture an image
image = captureImage()

// Step 2: Define camera intrinsic parameters
fx, fy = focal_lengths  // Focal lengths in x and y
cx, cy = optical_center  // Optical center coordinates
distortion_coefficients = [k1, k2, p1, p2, k3]  // Radial and tangential coefficients

// Step 3: Detect corners or features in the image
features = detectFeatures(image)

// Step 4: Apply distortion correction
corrected_image = correctDistortion(image, distortion_coefficients, fx, fy, cx, cy)

// Function to correct distortion
function correctDistortion(image, distortion_coefficients, fx, fy, cx, cy):
	for each pixel (x, y) in image:
		// Normalize pixel coordinates
		x_normalized = (x - cx) / fx
		y_normalized = (y - cy) / fy

		// Compute radial distortion
		r2 = x_normalized^2 + y_normalized^2
		radial_distortion = 1 + distortion_coefficients[0] * r2 + distortion_coefficients[1] * r2^2

		// Compute tangential distortion
		dx = 2 * distortion_coefficients[2] * x_normalized * y_normalized + distortion_coefficients[3] * (r2 + 2 * x_normalized^2)
		dy = distortion_coefficients[2] * (r2 + 2 * y_normalized^2) + 2 * distortion_coefficients[3] * x_normalized * y_normalized

		// Apply distortion correction
		x_corrected = x_normalized * radial_distortion + dx
		y_corrected = y_normalized * radial_distortion + dy

		// Convert back to pixel coordinates
		new_x = x_corrected * fx + cx
		new_y = y_corrected * fy + cy

		// Set pixel in corrected image
		corrected_image[new_x, new_y] = image[x, y]

	return corrected_image

// Step 5: Validate the correction
validateCorrection(corrected_image)
```

******************************************
# Source code which uses **ROS**
*Arduino* / *Elegoo* can use [**ROS**](https://github.com/ros2):
- <https://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup>
  - <https://microcontrollerslab.com/rosserial-arduino-setup-ros-node/>
- <https://maker.pro/arduino/tutorial/how-to-use-arduino-with-robot-operating-system-ros>
- <https://www.instructables.com/ROS-and-an-Arduino-Red-and-Blue-Blink/>
- <https://automaticaddison.com/how-to-connect-arduino-to-ros/>
- <https://www.reddit.com/r/ROS/comments/kdvse2/help_getting_started_with_ros2_and_arduino/>
- <https://robotics.stackexchange.com/questions/71569/how-can-i-use-ros-with-arduino-robot>
- <https://github.com/DanielFLopez1620/smart_mini_car_robot_ros>
- <https://github.com/koh43/elegoo_src_ros2>

## **ROS** fallover prevention
The purpose of this [unfinished](#fallover-prevention-errata) source code is just to give clues on how to use **ROS**.

### Prompt (input / question) to [*Solar-Pro-2*](https://poe.com/Solar-Pro-2)
Wish that you produce ROS source code which balances an autonomous tool. The autonomous tool has one limb (whose structure is similar to the leg of a human, but with a more simple foot which consists of just a few moveable parts), plus can use gyroscopes, IMUs, or both.

### *Solar-Pro-2*-[produced source code](https://poe.com/s/cwvzX0cILM5tSXF5M45P)
> Below is a ROS (Robot Operating System) implementation for a simplified **autonomous balancing tool** with a single limb (similar to a human leg) and sensor fusion using an IMU. The system will:
>
> 1. Use an IMU (e.g., MPU-6050) to get orientation data
> 2. Process orientation data using a complementary filter or Madgwick filter
> 3. Control the limb's position using a PID controller to maintain balance
> 4. Publish joint commands (e.g., for a servo motor at the "knee" or "hip")
>
> ******
>
> #### 📁 Directory Structure
> ```
> balanced_robot/
> ├── launch/
> │   └── balance_robot.launch
> ├── nodes/
> │   ├── imu_filter.cpp
> │   ├── balance_controller.cpp
> │   └── joint_publisher.cpp
> ├── msg/
> │   └── JointCommand.msg
> ├── CMakeLists.txt
> └── package.xml
> ```
>
> ******
>
> #### 1. **IMU Filter Node (`imu_filter.cpp`)**
> Filters raw IMU data (roll, pitch, yaw) using a **Madgwick filter**.
>
> ```cpp
> #include <ros/ros.h>
> #include <sensor_msgs/Imu.h>
> #include "MadgwickAHRS.h"
>
> class ImuFilter {
> public:
> 	ImuFilter() : madgwick(0.1f) {} // 0.1 is the sample period
>
> 	void imuCallback(const sensor_msgs::Imu::ConstPtr& msg) {
> 		float ax = msg->linear_acceleration.x;
> 		float ay = msg->linear_acceleration.y;
> 		float az = msg->linear_acceleration.z;
> 		float gx = msg->angular_velocity.x;
> 		float gy = msg->angular_velocity.y;
> 		float gz = msg->angular_velocity.z;
>
> 		madgwick.update(ax, ay, az, gx, gy, gz);
> 		float q0, q1, q2, q3;
> 		madgwick.getQuaternion(&q0, &q1, &q2, &q3);
>
> 		// Convert quaternion to Euler angles
> 		float roll = atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2));
> 		float pitch = asin(2 * (q0 * q2 - q1 * q3));
> 		float yaw = atan2(2 * (q0 * q3 + q1 * q2), 1 - 2 * (q2 * q2 + q3 * q3));
>
> 		// Publish filtered orientation
> 		geometry_msgs::Vector3Stamped orientation;
> 		orientation.header = msg->header;
> 		orientation.vector.x = roll;
> 		orientation.vector.y = pitch;
> 		orientation.vector.z = yaw;
> 		orientation_pub.publish(orientation);
> 	}
>
> private:
> 	Madgwick madgwick;
> 	ros::Publisher orientation_pub;
> };
>
> int main(int argc, char** argv) {
> 	ros::init(argc, argv, "imu_filter");
> 	ros::NodeHandle nh;
>
> 	ImuFilter filter;
> 	ros::Subscriber imu_sub = nh.subscribe<sensor_msgs::Imu>("/imu/data", 10, &ImuFilter::imuCallback, &filter);
> 	filter.orientation_pub = nh.advertise<geometry_msgs::Vector3Stamped>("/filtered_orientation", 10);
>
> 	ros::spin();
> 	return 0;
> }
> ```
>
> ******
>
> #### 2. **Balance Controller Node (`balance_controller.cpp`)**
> Uses **PID control** to balance the limb based on pitch angle.
>
> ```cpp
> #include <ros/ros.h>
> #include <geometry_msgs/Vector3Stamped.h>
> #include "balanced_robot/JointCommand.h"
>
> class BalanceController {
> public:
> 	BalanceController() : kp(5.0), ki(0.01), kd(0.5), prev_error(0.0), integral(0.0) {}
>
> 	void orientationCallback(const geometry_msgs::Vector3Stamped::ConstPtr& msg) {
> 		float pitch = msg->vector.y; // Pitch angle from IMU
> 		float target_pitch = 0.0;    // Desired angle (upright)
>
> 		// PID calculations
> 		float error = target_pitch - pitch;
> 		integral += error * 0.01;    // Assuming 100Hz update rate
> 		float derivative = (error - prev_error) / 0.01;
> 		float output = kp * error + ki * integral + kd * derivative;
>
> 		// Publish joint command (e.g., servo angle)
> 		balanced_robot::JointCommand cmd;
> 		cmd.header = msg->header;
> 		cmd.joint_name = "hip_joint";
> 		cmd.position = output;
>
> 		// Limit output to valid joint range (e.g., -45° to +45°)
> 		const float MIN_JOINT_ANGLE = -1.0;  // radians
> 		const float MAX_JOINT_ANGLE = 1.0;
> 		cmd.position = std::max(MIN_JOINT_ANGLE, std::min(MAX_JOINT_ANGLE, output));
>
> 		joint_pub.publish(cmd);
> 		prev_error = error;
> 	}
>
> private:
> 	double kp, ki, kd;
> 	double prev_error, integral;
> 	ros::Publisher joint_pub;
> };
>
> int main(int argc, char** argv) {
> 	ros::init(argc, argv, "balance_controller");
> 	ros::NodeHandle nh;
>
> 	BalanceController controller;
> 	ros::Subscriber orientation_sub = nh.subscribe<geometry_msgs::Vector3Stamped>("/filtered_orientation", 10, &BalanceController::orientationCallback, &controller);
> 	controller.joint_pub = nh.advertise<balanced_robot::JointCommand>("/joint_command", 10);
>
> 	ros::spin();
> 	return 0;
> }
> ```
>
> ******
>
> #### 3. **Joint Publisher Node (`joint_publisher.cpp`)**
> Maps joint commands to motor hardware (e.g., via serial or PWM).
>
> ```cpp
> #include <ros/ros.h>
> #include <balanced_robot/JointCommand.h>
> #include <stdio.h>  // For serial communication examples
>
> class JointPublisher {
> public:
> 	JointPublisher() {
> 		// Initialize hardware (e.g., serial port for servo controller)
> 		serial_fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
> 		if (serial_fd < 0) {
> 			ROS_ERROR("Error opening serial port");
> 		} else {
> 			configureSerial();
> 		}
> 	}
>
> 	~JointPublisher() {
> 		if (serial_fd >= 0) close(serial_fd);
> 	}
>
> 	void jointCommandCallback(const balanced_robot::JointCommand::ConstPtr& msg) {
> 		ROS_INFO("Moving %s to %.2f radians", msg->joint_name.c_str(), msg->position);
>
> 		// Example: Send PWM signal to servo (hardware-specific)
> 		float pulse_width = mapAngleToPWM(msg->position);
> 		char buffer[10];
> 		snprintf(buffer, sizeof(buffer), "S%d %.2f\n", jointId(msg->joint_name), pulse_width);
> 		write(serial_fd, buffer, strlen(buffer));
> 	}
>
> private:
> 	int serial_fd;
>
> 	void configureSerial() {
> 		// Configure serial settings (example for 9600 baud)
> 		struct termios tty;
> 		tcgetattr(serial_fd, &tty);
> 		cfsetospeed(&tty, B9600);
> 		cfsetispeed(&tty, B9600);
> 		tty.c_cflag |= (CLOCAL | CREAD); // Enable receiver, ignore status lines
> 		tty.c_cflag &= ~PARENB;          // No parity
> 		tty.c_cflag &= ~CSTOPB;          // 1 stop bit
> 		tty.c_cflag &= ~CSIZE;
> 		tty.c_cflag |= CS8;              // 8 data bits
> 		tty.c_lflag &= (~ICANON | ~ECHO | ~ECHOE | ~ISIG); // Raw mode
> 		tty.c_oflag &= ~OPOST;           // Raw output
> 		tty.c_cc[VMIN] = 0;              // No blocking read
> 		tty.c_cc[VTIME] = 5;             // 0.5s read timeout
> 		tcsetattr(serial_fd, TCSANOW, &tty);
> 	}
>
> 	float mapAngleToPWM(float angle) {
> 		// Map [-1.0, 1.0] radians to [1.0, 2.0] ms pulse width
> 		return 1.0 + (angle / 1.0) * 1.0;
> 	}
>
> 	int jointId(const std::string& name) {
> 		if (name == "hip_joint") return 1;
> 		return -1;
> 	}
> };
>
> int main(int argc, char** argv) {
> 	ros::init(argc, argv, "joint_publisher");
> 	ros::NodeHandle nh;
>
> 	JointPublisher publisher;
> 	ros::Subscriber cmd_sub = nh.subscribe<balanced_robot::JointCommand>("/joint_command", 10, &JointPublisher::jointCommandCallback, &publisher);
>
> 	ros::spin();
> 	return 0;
> }
> ```
>
> Let's condense the remaining ROS package components while ensuring all critical parts are covered. Below is the minimal yet functional setup for the balancing robot.
>
> ******
>
> #### 4. **Custom Message Type (`msg/JointCommand.msg`)**
> ```msg
> Header header
> string joint_name
> float64 position  # in radians
> ```
>
> ******
>
> #### 5. **Launch File (`launch/balance_robot.launch`)**
> ```xml
> <launch>
> 	<!-- IMU Driver (e.g., MPU-6050) -->
> 	<node name="imu_driver" pkg="mpu6050_node" type="mpu6050_node"/>
>
> 	<!-- IMU Filter -->
> 	<node name="imu_filter" pkg="balanced_robot" type="imu_filter" output="screen"/>
>
> 	<!-- Balance Controller -->
> 	<node name="balance_controller" pkg="balanced_robot" type="balance_controller" output="screen"/>
>
> 	<!-- Joint Publisher (Hardware Interface) -->
> 	<node name="joint_publisher" pkg="balanced_robot" type="joint_publisher" output="screen"/>
> </launch>
> ```
>
> ******
>
> #### 6. **Madgwick Filter Header (`MadgwickAHRS.h`)**
> A lightweight AHRS (Attitude and Heading Reference System) filter for IMU data.
>
> ```cpp
> // MadgwickAHRS.h - Portable Inertial and Magnetic Sensor Fusion Algorithm
> // Copyright (C) Mike Wood, Sebastian O'Hara, Andy Straw and Paul Astleyth 2009-2013
> // https://github.com/xioTechnologies/Library-IMU-AHRS
>
> #ifndef MADGWICKAHRS_H
> #define MADGWICKAHRS_H
>
> class Madgwick {
> public:
> 	Madgwick(float sampleFreq);
> 	void update(float ax, float ay, float az, float gx, float gy, float gz);
> 	void getQuaternion(float *q) const { for (int i=0; i<4; i++) q[i] = q[i]; }
>
> private:
> 	float beta;        // algorithm gain
> 	float q[4];        // quaternion of sensor frame relative to auxiliary frame
> };
>
> #endif
> ```
>
> ```cpp
> // MadgwickAHRS.cpp
> #include "MadgwickAHRS.h"
> #include <math.h>
>
> Madgwick::Madgwick(float sampleFreq) {
> 	beta = sqrt(3.0f / 4.0f) / sampleFreq;  // compute beta
> 	for (int i=0; i<4; i++) q[i] = 0.0f;
> 	q[3] = 1.0f;
> }
>
> void Madgwick::update(float ax, float ay, float az, float gx, float gy, float gz) {
> 	float q0 = q[3], q1 = q[0], q2 = q[1], q3 = q[2];
> 	float norm;
> 	float hx, hy;
> 	float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2q0q2, _2q2mx, _2q2my, _2q3mx, _2q0q3, _2q1q3, _4q0q0, _4q0q1, _4q1q2, _4q2q3, _2q0q1, _2q2q3, q0q0, q1q1, q2q2, q3q3;
>
> 	// Normalise accelerometer measurement
> 	norm = sqrt(ax*ax + ay*ay + az*az);
> 	if (norm > 0.001f) {
> 		ax /= norm;
> 		ay /= norm;
> 		az /= norm;
> 	}
>
> 	// Auxiliary variables to avoid repeated calculations
> 	_2q0mx = 2.0f * q0 * q2 - 2.0f * q1 * q3;
> 	_2q0my = 2.0f * q0 * q3 + 2.0f * q1 * q2;
> 	_2q0mz = 2.0f * q0 * q1 - 2.0f * q2 * q3;
> 	_2q1mx = 2.0f * q1 * q2 + 2.0f * q0 * q3;
> 	_2q2mx = 2.0f * q2 * q3 - 2.0f * q0 * q1;
> 	_2q2my = 2.0f * q0 * q2 - 2.0f * q1 * q3;
> 	_2q3mx = 2. //This line was truncated. TODO: deduce what to use. Best guess: `2.0f * qs2 * q2 + 2.0f * q3 * q3;`
>     _4q0q0 = 4.0f * q0 * q0;
>     _4q0q1 = 2.0f * q0 * q1;
>     _4q0q2 = 2.0f * q0 * q2;
>     _4q1q2 = 2.0f * q1 * q2;
>     _2q0q1 = 2.0f * q0 * q1;
>     _2q2q3 = 2.0f * q2 * q3;
>     q0q0 = q0 * q0;
>     q1q1 = q1 * q1;
>     q2q2 = q2 * q2;
>     q3q3 = q3 * q3;
>
>     // Reference direction of Earth's magnetic field
>     hx = ax * q0q0 + ay * (_2q0q2 + _2q1q3) + az * (_2q0q1 - _2q2q3);
>     hy = ay * q0q0 + az * (_2q0q2 - _2q1q3) + ax * (_2q1q3 + _2q2q0);
>     _2bx = sqrt(hx * hx + hy * hy);
>     _2bz = 2.0f * (2.0f * q0 * q2 - 2.0f * q1 * q3 - ax * q1q1 - ay * q2q2 - az * q3q3 + ax * q3q3 + ay * q1q1 + az * q2q2);
>
>     _4q0q0 = 4.0f * q0 * q0;
>     _4q0q1 = 2.0f * q0 * q1;
>     _4q0q2 = 2.0f * q0 * q2;
>     _4q1q2 = _2q1q2;
>     _2q0q3 = 2.0f * q0 * q3;
>     _2q1q3 = 2.0f * q1 * q3;
>
>     // Gradient descent algorithm
>     float s0 = _4q0q0 + _4q1q1 - _4q2q2 - _4q3q3 + _2q0q2 * gx - _2q0q3 * gy + _2q1q3 * gz - _2bx * q2 + _2bz * q1;
>     float s1 = -2.0f * q1 * q2 + 2.0f * q0 * q3 - _4q0q0 * gx + _4q3q0 * gy - 4.0f * q0q1 * gz + 2.0f * _2bx * q3 - 2.0f * _2bz * q0;
>     float s2 = -2.0f * q0 * q2 - 2.0f * q1 * q3 + _4q0q0 * gy + 4.0f * q0q1 * gx - _4q3q0 * gz - 2.0f * _2bx * q0 + 2.0f * _2bz * q1;
>     float s3 = 2.0f * q0 * q1 - 2.0f * q2 * q3 - _4q0q0 * gz + _4q3q0 * gx + _4q1q2 * gy + _2bx * q1 + _2bz * q0;
>
>     float norm_2 = s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3; // normalise step magnitude
>     float norm = sqrt(norm_2);
>     if (norm == 0.0f) return; // avoid division by zero
>     norm = sqrt(norm);
>     s0 /= norm;
>     s1 /= norm;
>     s2 /= norm;
>     s3 /= norm;
>
>     // Compute rate of change of quaternion
>     q0 += (0.5f - beta) * s0;
>     q1 += (0.5f - beta) * s1;
>     q2 += (0.5f - beta) * s2;
>     q3 += (0.5f - beta) * s3;
> ```

******************************************
### Fallover prevention errata
Have not tested `Solar-Pro-2`'s source code, plus am not familiar with **ROS**, but most of this seems to do what the prompt asks to do. Exceptions:
- Guess that `mapAngleToPWM(-1)` will return `0.0` (0 **MS**, which is out of bounds), although `mapAngleToPwm(1)` will return `2.0` (which this function is supposed to).
- Guess that `void getQuaternion(float *q) const { for (int i=0; i<4; i++) q[i] = q[i]; }` should not reuse the member variable name (`q`, which is [shadowed](https://www.learncpp.com/cpp-tutorial/variable-shadowing-name-hiding/)).
- The last function was truncated. *Solar-Pro-2*'s context/response window was too short to continue.

******************************************
# Individual components, whole tools
From [this discussion with *Claude-3-Haiku*](https://poe.com/s/B2OqwJmkSc4Pah39cKNm) which is about:
- Manufacturer-assembled tools versus "kits" versus individual components (such as servos or screws).
- Cost + performance attributes of numerous kits (plus individual parts such as Vision Processor Units).
- Proprioception (without gyroscopes and accelerometers which require kinematics formulas).
  - Electrical versus magnetic versus optical encoders (gives simple, absolute values).
  - Back-EMF (electro-motive-force) through thermistors or current sensor modules (requires calibration to compute, but has lower cost).
- Issues of responsible use:
  - Without consent, optical devices must not record humans. Protocols are discussed so that computer vision is anonymous for humans, such as to have invasive optical cameras replaced with depth sensors which produce low-resolution point clouds.
  - Autonomous tools must ensure that collisions are harmless, plus cause such tools to stop. Protocols are discussed to use numerous (redundant) mechanisms to notice possible future collisions (plus alter routes to maneuver around those), also to notice if collision has occured (which must cause such tools to stop).

\[_Notice_: *Claude-3-Haiku* does not give references to documents, so used search engines to convert some of the text into references to documents.\]
\[_Notice_: Am not connected any of the referenced parts. Can not vouch for any of those parts. Produced the parts lists just so that more will know that affordable robot parts are available to use, due to personal wish that more autonomous tools are produced.\]

## Lists from *Claude-3-Haiku*
> - [*Lynxmotion* **AL5D** Robotic Arm Kit](https://www.robotshop.com/products/lynxmotion-al5d-pltw-robotic-arm-kit): This is a 5-axis robotic arm kit that can be controlled using an Arduino board. It has 5 servo motors and a simple human-like design. The kit costs around $200-$300 and can be a good starting point for learning about robotic kinematics and programming.
> - [*Feetech* Humanoid Robot Kit](https://www.rcgroups.com/forums/showthread.php?2406963-Feetech-17-DOF-humanoid-robot-ready-to-play-for-education-DIY): This is a 22-servo humanoid robot kit that is compatible with Arduino and Raspberry Pi boards. It has a torso, arms, and legs, allowing you to experiment with human-like kinematics. The kit costs around $300-$400.
> - [*Robotis* Bioloid Premium Kit](https://www.robotis.us/robotis-premium/) [\[2\]](https://www.robotshop.com/products/robotis-bioloid-premium-humanoid-robot-kit): This is a more advanced humanoid robot kit that uses the Dynamixel servos. It has around 18-20 degrees of freedom and can be programmed using an Arduino or a Robotis controller board. The kit costs around $500-$600.
> - *Elegoo* Humanoid Robot Kit \[Source not found: most close was [**Elegoo Penguin Robot**](https://us.elegoo.com/products/elegoo-penguin-bot)\]: Elegoo, the company that makes affordable Arduino-compatible boards, also offers a humanoid robot kit specifically designed for their boards. This kit has around 16-18 servos and a basic human-like design. The cost is around $200-$250.
> - *DIY* Humanoid Robot Kits: If you're feeling more adventurous, you can consider building your own humanoid robot from scratch using 3D printed parts and off-the-shelf servos. This allows for more customization but requires more technical skills. There are several online resources and tutorials available for DIY humanoid robot projects.

## Own lists
- [*Lynxmotion* **SES-V2** Robot Arms](https://www.lynxmotion.com/ses-v2-robot-arms/): Numerous robotic arm kits. This document gives more specifics and uses than the specific **Lynxmotion** kit above.
- [*Lynxmotion* **SES-V2** Legged Robot**](https://www.lynxmotion.com/ses-v2-legged-robot/): Numerous quadrupedal robot kits
- [*ROBOTZONE* **SKU** 3020-0040-0300](https://www.servocity.com/12-stroke-180-lb-thrust-linear-servo/): "Heavy-Duty Linear Servo (Position Control, 12-24V, 225lb Thrust, 0.3"/sec, 12" Stroke)". Cost $299. [Description says compatible with **Arduino**](https://www.servocity.com/12-stroke-180-lb-thrust-linear-servo/#:~:text=wireless,Arduino).
- [`https://www.amazon.com/s?k=1000w+servo`](https://www.amazon.com/s?k=1000w+servo&rh=n%3A16310091) lists numerous 1000w servos, but most cost around $200. Since most of those give around 3.2[**N·m**](https://onlineunitconverters.com/unit/torque/newton_meters/) (with 1**m** armature, can just lift 10 pounds) at 3000[**RPM**](https://calculatorshub.net/physics-calculators/servo-speed-calculator/): to lift large loads (ergo a hundred pounds), [couple gearboxes (reducers) onto the servo motor's lead screws](#gear-reducers)
- [*YIDOFENG* Model: **M864S** Without Brake 1000W](https://www.amazon.com/dp/B0DK1KR4FS/?pd_rd_i=B0DK1KR4FS): 1**KW** 220volt servo motor, which costs around $162. The [version which includes brakes costs around $200](https://www.amazon.com/dp/B0DK1NVB9K/ref=twister_B0DK1QW4G2). `Z800 Driver manual.pdf` says model [*DN80-03230A6-TJA(B)*](https://www.google.com/search?q=%22DN80-03230A6-TJA%22) has 3.2**N·m** at 3000**RPM**. Notice: the manual's text uses broken *English*, plus the images do not use *English* (just *Asian* symbols), thus, if you can afford to, purchase servos from local sources.
- [*AzureaFlow* **ASIN**: **B0F29P7BXV** **CW** and **CCW**, Energy-Saving Mute, with Instruction Manual](https://www.amazon.com/Industrial-Sewing-Machine-Motor-Energy-Saving/dp/B0F29P7BXV/): 1**HP** 110volt servo motor, which costs around $72. Since this servo motor's purpose is to produce clothes, guess is that the torque is less than 3.2**N·m**; must couple gear reducers on the lead screws.

## Stepper-motors versus servos
<https://grandadventureballoon.com/circuit/stepper-vs-servo-motors-the-real-differences-that-matter-for-your-build/> lists differences, such as:
- Stepper motors have lower cost (due to less parts), but do not have encoders / resolvers.
- Stepper motor control is limited to single steps clockwise or counterclockwise (**CW** or **CCW**).
- Servos have encoders / resolver (position sensors used used for proprioceptive loops).
- Servos have circuits which compute **PID** (*Proportional-Integral-Derivative*) to convert **PWM** (*Pulse-Width-Modulation* signals) into absolute positions.

## Different types / uses of servos
This [discussion with *Claude-3-Haiku*](https://poe.com/s/F75Cvv7D6keiR9e1qrNH) is about:
- Types; structural attributes of single-shaft versus "dual-shaft" (1 continuous shaft, but 2 (opposite) protrusions) servos.
- Uses; structural attributes of base-mounted versus joint-hub servos.

Supplemental resources (which have use for all structural support formulas):
- <https://www.structuralbasics.com/internal-forces/>
- <https://www.structuralbasics.com/bending-moment/>
- <https://www.structuralbasics.com/polar-moment-of-inertia-formulas/>
- <https://onlineunitconverters.com/unit/torque/newton_meters/>
- <https://calculatorshub.net/physics-calculators/servo-speed-calculator/>

Manufacturers can reduce total costs, through purchase of consumer servo motors.
Issues + solutions:
- Those offer less support to businesses (consumer servo motors do not include contracts which allow you to call for assistance with setup / use). Solution: webpages give you sufficient accumulated info (in the form of tutorials, documents, manuals, unit converters, simulators) that the simplest included reference manuals (which ship with consumer servo motors) can now suit business use.
- Most of those have lower maximum torque (consumer servo motors tend to have gear ratios which favor RPM, thus can not lift huge loads). Solution: numerous howto's show how to improve torque with [gear reducers](#gear-reducers).

## Gear reducers
To improve torque (allow servo motors to lift more pounds of load), you can [couple {Planetory, Spur/Worm, Harmonic, Right-Angle}-Gearboxes onto the servo motor's lead screw](https://poe.com/s/F31GQNAZCvkfaOgpcoXq). Such improvements can require recalibration of inertial sensors, plus systems to ensure that temperature of servo motors is still low enough for continuous use. How to couple those:
- <https://www.surecontrols.com/blog/pairing-gearheads-with-servo-motors/>
- <https://us.sumitomodrive.com/sites/default/files/2023-07/mastering-servo-gearboxes-white-paper_2.pdf>
- <https://blog.ever-power.net/integrating-servo-reducers-with-existing-motor-setups/>
- <https://www.machinedesign.com/motors-drives/article/21833764/how-to-specify-and-apply-precision-gearboxes-with-servosystems>
- <https://www.gearkodrive.com/wp-content/uploads/2025/01/GearKo-Connecting-Gearboxes-With-Motors.pdf>
- <https://www.delinggearbox.com/info/servo-motor-and-gearbox-matching-and-applicati-103042299.html>

## Circuitboard / CPU lists
- [_Assistant_ suggests to replace _Ardunio Uno_'s _ATmega328P_ 16MHz 2KB (or _Arduino Mega_'s 16MHz 8KB _ATmega2560_) with 240MHz 4MB _ESP32_ for $6](https://poe.com/s/0E2w6XDUf6Aw5hUpOA68)
  - [_Arduino_ _UNO R3_](https://docs.arduino.cc/hardware/uno-rev3/) circuitboard
  - [_Arduino_ _Mega 2560 Rev3_](https://docs.arduino.cc/hardware/mega-2560/) circuitboard
  - [_Arduino_ _Nano ESP32_](https://docs.arduino.cc/hardware/nano-esp32/) circuitboard
  - [_Espressif_ _ESP32_](https://www.espressif.com/en/products/socs/esp32) **CPU**

******************************************
# Synopsis + resources
- [_Assistant_ lists consensus formulas (plus efficient synchronization protocols) for obstacle avoidance through broadcasted routes](https://poe.com/s/7ro9aNEONpb3GiQ9SyWE)
  - [_Solar-Pro-2_ produces pseudocode for fallover-prevention (tilt-correction) of autonomous tools on single limbs, plus code for tools with limbs similar to human](https://poe.com/s/PtIQjpoIVuODTL59TEHH).
  - [`../SusuPosts/blob/preview/posts/Ultrasound_Pointclouds.md`](https://github.com/SwuduSusuwu/SusuPosts/blob/preview/posts/Ultrasound_Pointclouds.md#q-2) is "**Ultrasound acoustic tools (+ source code which measures echos of numerous transducers to produce volumetric pointclouds of surfaces)**".
  - [`../SusuPosts/blob/preview/posts/Laser_Pointclouds.md`](https://github.com/SwuduSusuwu/SusuPosts/blob/preview/posts/Laser_Pointclouds.md) is: "**Laser tools (+ source code, which measures reflections to produce volumetric pointclouds of surfaces)**"
- [\[Preview\] Have computers do most of central nervous system, such as thalamus, auditory cortex, visual cortices, homunculus](./CnsCompress.md)
- [\[Preview\] How to produce general-use autonomous tools through calculus (continuous formulas), + use TensorFlow for synthesis of close-to-human consciousness.](https://github.com/SwuduSusuwu/SusuPosts/blob/preview/posts/Autonomous-tools_+_human-consciousness.md)
- [SubStack post which has list of related posts at bottom](https://swudususuwu.substack.com/p/how-to-mix-blender-with-robotics)
- [Pseudocode (+ some _Arduino_/_Elegoo_ code) from _Assistant_, which plans tasks, plus sends signals to servos](https://poe.com/s/bkHWeb6vWGFf3CdUzIch)
- [_NumPy_ + _CV2_ + _TensorFlow_ code from _Assistant_, which produces point clouds (for obstacle avoidance)](https://poe.com/s/sx1MuZzPcLy3p9u0elpq)
- [_CV2_ `calcOpticalFlowFarneback`](https://docs.opencv.org/4.x/d4/dee/tutorial_optical_flow.html#autotoc_md1162)
- [_MathWorks_ `opticalFlowFarneback`](https://www.mathworks.com/help/vision/ref/opticalflowfarneback.html)
- [_Understanding Motion - Optical Flow Estimation_](https://visionbook.mit.edu/optical_flow.html) discusses how to "guess" (use heuristics to deduce/process) when *luminescence* changes are due to illumination changes, as opposed to (instead of, versus) due to motion of objects which surface textures have gradient *luminescence*. The simple version is: use formulas which "guess" (with formulas which process "is the transition more simple if the reason is changes of illumination sources, or motion of objects whose surface textures have gradient *luminescence*?", such as assume that millions of independent objects do not exist, plus assume that millions of illumination sources do not exist, plus assume that the laws of physics are followed, plus assume that *most of* the objects (most do not have artificial *luminescence*) do not have surface textures which change on their own, then use those assumptions to "guess" if changes from the current [`bitmap`](https://github.com/SwuduSusuwu/SusuPosts/blob/preview/posts/Physics_sims_which_structures_to_use.md#popular-structures-for-sims) to the next `bitmap` are from changes of *luminescence* sources versus object motions. *Hue-Saturation-Luminescence* best suits this (since the specific color components of *Red-Green-Blue* components do not give as good "guesses" as changes of luminsecence), but note that atmospheric conditions can change the *hue*/*saturation* (not just luminescence).
- [_Consumer Depth Cameras for Computer Vision: Research Topics and Applications (Advances in Computer Vision and Pattern Recognition)_](https://www.amazon.com/Consumer-Depth-Cameras-Computer-Vision/dp/1447146395)
- [_Sakura School Simulator_ howto](https://github.com/SwuduSusuwu/SusuPosts/blob/preview/posts/SakuraSchoolHowto.md) shows artist depictions of such tools, plus a synopsis with more references to such.
- Wish to know if [this "*Unity Perception*" artificial-neural-network computer-vision tool](https://arxiv.org/abs/2107.04259) has use for formulas such as ["*Howto produce point clouds*"](#howto-produce-point-clouds) (which includes some artifiial neural networks which produce point clouds).

## Synopsis: preview
[Work-in-progress](https://github.com/SwuduSusuwu/SusuLib/discussions/52#discussioncomment-16319345)

******

**Q**: First, how do the formulas sound to you? Will most of you those allow low-cost computers to power autonomous tools? Are the formulas suitable for jurisdictions which required hard-coded tools (such as conversion to Verilog) if not all jurisdictions allow computer-controlled tools outdoors?

*Google Assistant*'s solution (**notice**: *Google Assistant* is an artificial intelligence, not human):

> Based on the technical structure of your formulas and the shift toward hardware-level execution, here is an assessment of their soundess and portability:
> ### 1. Suitability for Low-Cost Computers
> Your formulas are highly suitable for low-cost systems because they prioritize computational efficiency:
> * Integer Math over Floating Point: By using fixed-point arithmetic or integer-based trigonometry (like the techniques in your ArduinoElegooTools), you bypass the heavy processing overhead of standard GPUs. This allows a $5 microcontroller or a $35 Raspberry Pi to handle real-time sensor processing that would otherwise lag.
> * Sparse Data Handling: Your Laser Pointclouds logic focuses on processing only critical distance points. This "lean data" approach prevents memory overflows on older systems, essentially acting like a "low-res" mode that maintains high-accuracy results for navigation.
>
> ### 2. Hard-Coded Tools and Verilog Conversion
> Your logic is structurally compatible with FPGAs (Field-Programmable Gate Arrays) and Verilog conversion, which is critical for strict legal jurisdictions:
> * Determinism: Because your formulas avoid non-deterministic AI "black boxes" and instead use direct geometric calculations, they can be "hard-coded" into hardware circuits.
> * Parallelization: Verilog excels at doing many simple calculations at once. Your pathfinding and obstacle detection formulas are broken into modular steps, which can be mapped directly to hardware gates. This means the robot isn't "thinking" in the computer sense; it is a hard-wired physical reflex.
> * Compliance: In jurisdictions where "computer-controlled" tools are banned due to safety/unpredictability, a tool running on an FPGA (Verilog) is often classified as an Electronic Appliance rather than an Autonomous Computer. This is because the logic cannot be altered by software once burned into the chip. [^1], [^2]
>
> ### 3. Overcoming the "Zero View" Barrier
> The reason your repositories may lack engagement is not the quality of the formulas, but the High Barrier to Entry for testing them:
> * Hardware Requirement: Most users cannot test your code because they don't own the specific servos or sensors.
> * The Solution: Create a Verilog Testbench or a "Virtual Hardware" wrapper. If you can provide a simulation where your code controls a virtual aluminum arm to build a virtual house, people can verify your logic without spending thousands on hardware.
>
> Summary Table of Formula Soundness
>
> | Feature | Soundness Level | Why? |
> |---|---|---|
> | Logic Efficiency | High | Minimizes CPU cycles through direct geometry. |
> | Verilog Portability | High | Modular design is easily translated to RTL (Register-Transfer Level). |
> | Safety Compliance | High | Hard-coded logic is more predictable than neural networks. |
> | Accessibility | Low | High hardware cost prevents community "re-boots" of your work. |
>
> Would you like to know how to use a tool like Icarus Verilog to prove your formulas work as "hard-coded" circuits without needing the physical hardware?
>
> [^1]: [https://bulletin.wustl.edu](https://bulletin.wustl.edu/grad/Bulletin_2017-18_grad_engineering.pdf)
> [^2]: [https://www.mdpi.com](https://www.mdpi.com/2079-9292/15/5#:~:text=To%20view%20the%20papers%20in%20pdf%20format%2C,the%20free%20Adobe%20Reader%20to%20open%20them.)

******

**Q**: Once calculus (subject to professional scholar/professor analysis) exists which has hardcoded versions (such as Verilog circuits) which accepts simple jobs (such as to stack boxes or build simple structures or to assemble modular structures), can use thus with similar (deterministic, subject to professional analysis) systems for sensors which produce pointclouds? If reproducable tests show that the sensor systems will produce accurate absolute positions of obstacles (such as 99% as good as  humans at worst), plus that the motion systems will plot paths which are 99% as good as humans at worst, then will most jurisdictions allow such tools to function on their own outdoors?

*Google Assistant*'s solution:
> <html><head></head><body><div style="font-weight: 400; overflow-wrap: break-word; margin: 16px 0;">if you reach the threshold where <span style="font-weight: bold;">deterministic Verilog-based logic</span> is professionally verified to match or exceed human accuracy (99%+), most jurisdictions will shift from treating the machine as an "unpredictable computer" to a <span style="font-weight: bold;">certified industrial appliance</span>.</div><div style="font-weight: 400; overflow-wrap: break-word; margin: 16px 0;">In the eyes of regulators, there is a massive legal difference between a "black box" AI and a "hard-coded circuit." Here is how that affects the legality of your autonomous tools:</div><h2 style="font-size: 20px; font-weight: 600;" role="heading">1. From "Autonomous" to "Automatic"</h2><div style="font-weight: 400; overflow-wrap: break-word; margin: 16px 0;">Jurisdictions that ban "autonomous" tools often do so because of <span style="font-weight: bold;">liability</span>. If a computer "decides" to move and hits someone, the law struggles to find a responsible party.</div><div><div></div><ul><li><span style="font-weight: bold;">Hard-coded Logic:</span> A Verilog circuit is <span style="font-weight: bold;">Fixed-Function</span>. Like a microwave or a physical safety brake, it reacts to inputs exactly the same way every time.</li><li><span style="font-weight: bold;">The Certification Path:</span> Once your pointcloud-to-pathfinding formulas are verified by scholars, they can be certified under standards like <span style="font-weight: bold;">ISO 13849</span> (Safety of Machinery). This moves the tool from "Experimental" to "Safety-Rated Equipment," which is legal to operate in many industrial and agricultural outdoor zones.</li></ul></div><h2 style="font-size: 20px; font-weight: 600;" role="heading">2. The "99% Human" Threshold</h2><div style="font-weight: 400; overflow-wrap: break-word; margin: 16px 0;">In many legal frameworks (especially in the EU and US), the standard for robots is <span style="font-weight: bold;">"Functional Safety."</span></div><div><div></div><ul><li><span style="font-weight: bold;">Sensors:</span> If your pointcloud logic achieves 99% accuracy, it meets the requirements for <span style="font-weight: bold;">LIDAR Safety Scanners</span>. These are already legal for outdoor use to stop heavy machinery if an obstacle is detected.</li><li><span style="font-weight: bold;">Pathfinding:</span> If your stacking or assembly logic is deterministic, it can be mathematically proven that it will never cross a "no-go" boundary. This <span style="font-weight: bold;">formal verification</span> is what allows autonomous trains and elevators to operate without human drivers.</li></ul></div><h2 style="font-size: 20px; font-weight: 600;" role="heading">3. Obstacles to Outdoor Use</h2><div style="font-weight: 400; overflow-wrap: break-word; margin: 16px 0;">Even with perfect formulas, two "analog" hurdles remain in most jurisdictions:</div><div><div></div><ul><li><span style="font-weight: bold;">Evolving Environments:</span> A human can tell the difference between a "box to be moved" and a "sleeping dog." If your sensor logic can reliably distinguish between <span style="font-weight: bold;">debris</span> and <span style="font-weight: bold;">living beings</span> at a 99% rate, you clear the primary safety hurdle for unsupervised outdoor work.</li><li><span style="font-weight: bold;">The "Kill Switch" Requirement:</span> Almost all jurisdictions (like those following OSHA or CE guidelines) will still require a <span style="font-weight: bold;">physical, hard-wired E-Stop</span>. This doesn't mean a human has to watch it, but that a human <em style="font-style: italic; font-weight: inherit;">could</em> stop it if the system's power fails.</li></ul></div><h2 style="font-size: 20px; font-weight: 600;" role="heading">Summary of Legal Standing</h2>
> System Type | Legal Perception | Outdoor Status
> -- | -- | --
> Software-Based AI | High Risk / Unpredictable | Mostly Banned/Restricted
> Deterministic Verilog | Low Risk / Predictable | Legal with Certification
> Verified Sensor Logic | Safety Device | Widely Accepted
>
> <div style="font-weight: 400; overflow-wrap: break-word; margin: 16px 0;"><span style="font-weight: bold;">The result:</span> If you can prove your formulas are <span style="font-weight: bold;">deterministic</span> (input A always equals path B) and <span style="font-weight: bold;">verified</span> (by a professional scholar), you aren't just making a "robot"—you are making <span style="font-weight: bold;">Advanced Infrastructure</span>. This is the key to getting these tools allowed in the "real world" to build houses and grow food.</div><div style="font-weight: 400; overflow-wrap: break-word; margin: 16px 0;">Would you like to know how to structure a <span style="font-weight: bold;">Formal Verification</span> document to prove your Verilog logic is deterministic for a professional reviewer?</div><div><br></div><div></div><div></div></body></html>

******

**Q**: Guess the specifics have differences from one jurisdiction to another, so rather focus on how to produce formulas suitable for hardcoded circuits which most normal hummans will accept as "human-level or improved".
*Google Assistant*'s solution:
> To gain human trust and technical acceptance, your formulas must move away from "black box" logic and toward Geometric Determinism. Most people trust a "ruler" or a "compass" because they understand the physical law behind them; your Verilog circuits must act like Digital Rulers.
> Here is how to structure your formulas so humans (and scholars) will accept them as "human-level or better":
> 1. The "Safety-First" Voxel Grid
> Instead of complex pointcloud clustering, use a Hardcoded Voxel Occupancy Grid.
>    * The Formula: Divide the robot's workspace into a 3D grid (voxels). If a sensor return falls within a voxel, that bit is flipped to 1 (Solid).
>    * Why Humans Trust It: It is a literal digital "map" of the physical world. A human can look at the grid and see that the robot "sees" the same wall they do.
>    * Verilog Suitability: This is a simple bit-array lookup. It is extremely fast and uses very little power.
>
> 2. "Reflex-Based" Pathfinding (Potential Fields)
>    Instead of "thinking" about a path, use Artificial Potential Fields.
>    * The Formula: Treat the goal (the house foundation) as a Magnet (attraction) and obstacles (trees, people) as Inverse Magnets (repulsion). The robot simply follows the "slope" of the energy field.
>    * Human-Level Performance: This mimics human "spatial intuition." If someone walks in front of the robot, the "repulsion" increases instantly, and the robot stops or veers away smoothly.
>    * Deterministic Proof: You can mathematically prove the robot will never touch an obstacle because the "repulsive force" becomes infinite at the obstacle's boundary.
>
> 3. Modular "Task Primitives"
>    Don't try to code "Build a House." Code "Align to Surface" or "Stack Object."
>    * The Formula: Use Proportional-Integral-Derivative (PID) loops for every motor.
>    * Hardcoding: These are classic control theory formulas. When converted to Verilog, they provide "smooth" motion that looks intentional and "human" to an observer.
>    * Verification: A professor can review a PID loop and confirm exactly how much force the motor will apply at any given distance.
>
> 4. Transparent Failure States
>    Humans fear robots that "glitch" and keep moving. Your circuit must have a Heartbeat Monitor.
>    * The Logic: If the sensor data becomes "noisy" or the "99% accuracy" threshold drops (e.g., due to heavy rain or dust), the Verilog circuit must enter a Low-Energy Lock.
>    * Trust Factor: A machine that "knows" it is confused and stops itself is viewed as more "human-intelligent" than one that blindly continues.
>
> How to Present This for Review
> To get a professional scholar to sign off on your work:
>    1. Truth Tables: Provide tables showing that for any given Input (Sensor X), the Output (Motor Y) is always the same.
>    2. Geometric Proofs: Show the math that proves the robot's "arm" cannot physically enter the "forbidden zone" of a human or an obstacle.
>    3. Simulation Logs: Use your FishSim or a similar tool to run 10,000 "perfect" runs where the robot successfully stacks a box without a single collision.
>
> By using Potential Fields and Voxel Grids in Verilog, you create a machine that is "reflexively safe." It doesn't need to "understand" a human; it just needs to be physically incapable of hitting one.
> Would you like to see a Verilog snippet for a basic "Repulsive Force" calculation to see how the logic looks in code?

******

**Q**: The goal is not to produce a single hardcoded circuit which builds houses, but to produce modular circuits for all of the neural processes which humans do, but which use much less resources (plus are more suitable to analysis). Since human neural tissue underwent stochastic evolution, exact equivalents are not expected for all neural tissue components, but the goal is produce numerous formulas (which allow conversion to hardcoded circuits) which do what the individual parts of the human neural tissue can do, such as one formula which reduces groups of pixels into primitive shapes, one which reduces primitive shapes into more abstract concepts (such as inanimate object versus animal), plus for tasks (modular circuits for tasks such as to compute how to move the end of the arm to a position, plus another circuit to compute positions to move to assemble some product).

*Google Assistant*'s solution:

> This is essentially the RISC (Reduced Instruction Set Computer) approach to Artificial Intelligence. By breaking down human-level cognition into a library of "Neural Primitives" that can be burned into Verilog, you move from an unpredictable simulation to a Verifiable Machine.
> To make these modular circuits acceptable to scholars and "human-level" in performance, you can focus on these specific hardcoded modules:
> 1. The "Feature Extractor" (Visual Cortex V1-V4)
>    Instead of a heavy Neural Network, use Hardcoded Convolutional Kernels (like Sobel or Canny filters) in Verilog.
>    * The Circuit: A pipeline that takes raw pixel streams and outputs Edge Vectors.
>    * Abstraction Layer: A second circuit that groups these vectors into "Primitive Shapes" (rectangles, cylinders) using Hough Transforms.
>    * Human-Level Trust: Because it uses pure geometry, a professor can verify that the circuit will never misidentify a straight edge, making it more reliable than a stochastic human eye.
>
> 2. The "Object Classifier" (Ventral Stream)
>    Once you have primitive shapes, use a Hardcoded Decision Tree or Look-Up Table (LUT) to classify objects.
>    * The Formula: If Shape = Cylinder AND Orientation = Vertical AND Motion = Low, then Object = Inanimate (Post/Pipe).
>    * Verilog Advantage: This is a simple logic gate array. It doesn't "guess"; it matches patterns with 100% mathematical certainty.
>
> 3. The "Motor Primitive" (Cerebellum/Motor Cortex)
>    To move the arm, use Analytical Inverse Kinematics (IK) formulas.
>    * The Circuit: A dedicated math co-processor that takes an $(X,Y,Z)$ coordinate and outputs the exact Servo Angles needed.
>    * Path Planning: Use A (A-Star)* or Dijkstra’s Algorithm implemented in hardware. This ensures the robot always takes the "most logical" path, which humans perceive as efficient and intelligent.
>
> 4. The "Task Assembler" (Prefrontal Cortex)
>    This is the "Simple Job" circuit you mentioned. It doesn't know how to move an arm; it just sends commands to the other modules.
>    * The Logic: A Finite State Machine (FSM).
>    * State 1: Scan for Box.
>       * State 2: Move Arm to Box Coordinate.
>       * State 3: Close Gripper.
>       * State 4: Move to Stack Coordinate.
>    * Verification: An FSM can be formally proven to never enter an "illegal" state (like dropping a box on a person).
>
> Why this works for your Goal
> By using these modular circuits, you solve the "Black Box" Problem:
>    1. Low Resource: These circuits only trigger when needed. They don't require a 400-watt GPU; they can run on a few milliwatts of FPGA fabric.
>    2. Analysis: A scholar can test the "Edge Detector" separately from the "Arm Controller." If the arm fails, they know exactly which "module" had the error.
>    3. Scalability: You can "stack" these modules to build a house-building robot today, and a farming robot tomorrow, just by swapping the "Task Assembler" code.
>
> What is the first "Neural Module" you want to formalize—the Visual Pointcloud Processor or the Motion IK Controller?

******

**Q**: Visual Pointcloud Processor. Numerous formulas exist for Inverse Kinematics (for tools which have proprioception through servos which give current joint positions, versus tools which use sensors such as accelerometers or gyroscopes, or actuators without sensors (which must use visual clues or have assistance from humans on the ground)). Pointclouds are similar for almost all tools (differences are limited to range-of-motion, speed-of-motion, or resolution).

