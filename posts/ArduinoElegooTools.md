**\[Preview\] How to use [_FLOSS_](https://wikipedia.org/wiki/FLOSS) code to produce autonomous [_Arduino_](https://wikipedia.org/wiki/Arduino)/[_Elegoo_](https://www.elegoo.com/blogs/arduino-projects) tools**

\[This post [from _SubStack_](https://swudususuwu.substack.com/p/how-to-use-floss-systems-to-produce) allows [_all uses_](https://creativecommons.org/licenses/by/2.0/).\] \[Disclaimer: am not affiliated to _Arduino_, nor to _Elegoo_.\] This is a work in progress (a "rough draft") which (through the next month) shall have much added, removed, or improved.

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
- [Individual components, whole tools](#individual-components-whole-tools)
  - [Stepper motors versus servos](#stepper-motors-versus-servos)
  - [Different types / uses of servos](#different-types--uses-of-servos)
  - [Gear reducers](#gear-reducers)
  - [Circuitboard / CPU lists](#circuitboard--CPU-lists)
- [Synopsis + resources](#synopsis--resources)

# Licenses
Am the sole author of `ArduinoElegooTools.md` (henceforth **this document**), plus release all of **this document** (such as concepts or code) to all humans or computers (henceforth **users**) for all uses; the sole restriction is that all of **this document** which does not now fall under patents, **users** shall not patent those from now on, but shall but allow other **users** to use those (without additional restrictions).

Assert that am the sole author (have asked the digital _Assistant_ about ownership, plus [_Assistant_ says that discussions with _Assistant_ do not belong to _Assistant_'s webhost, but belong to who used _Assistant_ to produce those](https://poe.com/s/gxaODiT6PRnObHHPXE9n)) of **this document** (so am allowed to waive all patent/copyright rights plus allow all to use,) plus choose to waive all patent/copyright rights plus allow all to use.
- **users** who wish to use **this document** as-is, can do so.
- **users** who wish to use **this document** through formal licenses, can choose to use through: [Creative Commons Generic Attribution 2.0](https://creativecommons.org/licenses/by/2.0/), [_GPLv2_](https://github.com/SwuduSusuwu/SusuLib/blob/trunk/LICENSE_GPLv2) or [_Apache 2_](https://github.com/SwuduSusuwu/SusuLib/blob/trunk/LICENSE).

## Disclaimer
Other than documents at `swudususuwu.substack.com`, am not responsible for contents of additional referenced documents -- such as those from other hosts {`wikipedia.org`, `poe.com`, `docs.arduino.cc`, `docs.opencv.com`, `www.mathworks.com`, `www.structuralbasics.com`, `play.google.com`, `micropython.org`, `randomnerdtutorials.com`, `realpython.org`, `amazon.com`, `espressif.com`}.
- Am not connected to those other hosts.
- Am not connected to those other documents' authors.

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

# Howto route
\[**Notice**\]: _Arduino_ (plus compatible _Elegoo_) circuitboard pins output at 5v with a max of 40milliamps, which must go to the stepper motor (or servo)'s control signal; [if used to power large motors, the circuitboard's internal resistors are damaged, so you must purchase a new circuitboard](https://docs.arduino.cc/learn/microcontrollers/digital-pins/#:~:text=milliamps,maximum).
- For motors without control signals (motors with just 2 pins, "+" plus "-"), attach the circuitboard's output pins the control gates power transistors, plus have those transistors power such motors..
  - Motors <500,000 volts (plus <200 amps) can use [power **MOSFET**s](https://wikipedia.org/wiki/Power_MOSFET) (which have a minimum gate control voltage of 3v, so you must ensure that the output pin does not power other circuits which cause more than 1 volt droop).
  - Motors >500,000 volts (or >500 amps) must use [insulated-gate bipolar transistors](https://wikipedia.org/wiki/Insulated-gate_bipolar_transistor) (which have a minimum gate control voltage of 4v, so you must ensure that the output pin does not also power status indicators or other circuits which can cause voltage drop).

What follows is [code from _Assistant_](https://poe.com/s/bkHWeb6vWGFf3CdUzIch) (just examples of what this post is about; future versions of this post will include own code, once am more experienced with robotic tools).
- Just this post (`posts/ArduinoElegooTools.md`) includes _Assistant_-produced code; all other posts / code was human-produced). All code was human-reviewed.

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
- `velocity` vector is 2D; _Assistant_ did not use conversation context to deduce that `velocity` must include altitude.

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
- `calculateDistance` is 2D; _Assistant_ did not use conversation context to deduce that `calculateDistance` must include altitude.

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
- `inverseKinematics` is 2D (`targetX` = longitude, `targetY` = lattitude); _Assistant_ "left it as an exercise to you" to have `inverseKinematics` include multiple limbs, plus altitude.

**Solution for altitude** ([_Assistant_ says that my solution will do.](https://poe.com/s/uwjbhSrSWTtenF4HTcAa)):
- For those 2 dimensions above, use `targetX` = distance from base, plus `targetY` = altitude.
- For the third dimension, have a third servo (`targetZ` = radial direction from base) which turns the base around towards the goal.
- This is more simple to compute (versus a normal 3-dimensional solution), which allows use of the $1 _ATmega328P_ **CPU**.

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
- `computeAvoidanceVector` is 2D\*; _Assistant_ did not use conversation context to deduce that `computeAvoidanceVector` must include altitude.
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
- [*YIDOFENG* Model: **M864S** Without Brake 1000W](https://www.amazon.com/dp/B0DK1KR4FS/?pd_rd_i=B0DK1KR4FS): 1**KW** 220volt servo motor, which costs around $162. The [version which includes brakes costs around $200](https://www.amazon.com/dp/B0DK1NVB9K/ref=twister_B0DK1QW4G2). `Z800 Driver manual.pdf` says model [*DN80-03230A6-TJA(B)*](https://www.google.com/search?q=%22DN80-03230A6-TJA%22) has 3.2[**N·m**] at 3000**RPM**. Notice: the manual's text uses broken *English*, plus the images do not use *English* (just *Asian* symbols), thus, if you can afford to, purchase servos from local sources.
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

# Synopsis + resources
- [_Assistant_ lists consensus formulas (plus efficient synchronization protocols) for obstacle avoidance through broadcasted routes](https://poe.com/s/7ro9aNEONpb3GiQ9SyWE)
  - [_Solar-Pro-2_ produces pseudocode for fallover-prevention (tilt-correction) of autonomous tools on single limbs, plus code for tools with limbs similar to human](https://poe.com/s/PtIQjpoIVuODTL59TEHH).
  - [`../SusuPosts/blob/preview/posts/Ultrasound_Pointclouds.md`](https://github.com/SwuduSusuwu/SusuPosts/blob/preview/posts/Ultrasound_Pointclouds.md#q-2) is "**Ultrasound acoustic tools (+ source code which measures echos of numerous transducers to produce volumetric pointclouds of surfaces)**".
  - [`../SusuPosts/blob/preview/posts/Laser_Pointclouds.md`](https://github.com/SwuduSusuwu/SusuPosts/blob/preview/posts/Laser_Pointclouds.md) is: "**Laser tools (+ source code, which measures reflections to produce volumetric pointclouds of surfaces)**"
- [\[Preview\] Have computers do most of central nervous system, such as thalamus, auditory cortex, visual cortices, homunculus](./CnsCompress.md)
- [SubStack post which has list of related posts at bottom](https://swudususuwu.substack.com/p/how-to-mix-blender-with-robotics)
- [Pseudocode (+ some _Arduino_/_Elegoo_ code) from _Assistant_, which plans tasks, plus sends signals to servos](https://poe.com/s/bkHWeb6vWGFf3CdUzIch)
- [_NumPy_ + _CV2_ + _TensorFlow_ code from _Assistant_, which produces point clouds (for obstacle avoidance)](https://poe.com/s/sx1MuZzPcLy3p9u0elpq)
- [_CV2_ `calcOpticalFlowFarneback`](https://docs.opencv.org/4.x/d4/dee/tutorial_optical_flow.html#autotoc_md1162)
- [_MathWorks_ `opticalFlowFarneback`](https://www.mathworks.com/help/vision/ref/opticalflowfarneback.html)
- [_Consumer Depth Cameras for Computer Vision: Research Topics and Applications (Advances in Computer Vision and Pattern Recognition)_](https://www.amazon.com/Consumer-Depth-Cameras-Computer-Vision/dp/1447146395)

