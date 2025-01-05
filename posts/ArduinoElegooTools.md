**\[Preview\] How to use [_FLOSS_](https://wikipedia.org/wiki/FLOSS) code to produce autonomous [_Arduino_](https://wikipedia.org/wiki/Arduino)/[_Elegoo_](https://www.elegoo.com/blogs/arduino-projects) tools**

\[This post [from _SubStack_](https://swudususuwu.substack.com/p/how-to-use-floss-systems-to-produce) allows [_all uses_](https://creativecommons.org/licenses/by/2.0/).\] \[Disclaimer: am not affiliated to _Arduino_, nor to _Elegoo_.\] This is a work in progress (a "rough draft") which (through the next month) shall have much added, removed, or improved.

# Table of Contents
- [Licenses](#licenses) (permissive enough to ignore.)
- [Intro](#intro)
- [Howto route](#howto-route)
  - [Group route pseudocode](#group-route-pseudocode)
  - [Wheeled _Arduino_/_Elegoo_ robot code](#wheeled-arduinoelegoo-robot-code)
  - [Limbed _Arduino_/_Elegoo_ robot code](#limbed-arduinoelegoo-robot-code)
- [External resources](#external-resources)

# Licenses
Am the sole author of `ArduinoElegooTools.md` (henceforth **this document**), and release all of **this document** (such as concepts or code) to all humans or computers (henceforth **users**) for all uses; the sole restriction is that all of **this document** which does not now fall under patents, **users** shall not patent those from now on, but shall but allow other **users** to use those (without additional restrictions).

Assert that am the sole author (have asked the digital _Assistant_ about ownership, and [_Assistant_ says that discussions with _Assistant_ do not belong to _Assistant_'s webhost, but belong to who used _Assistant_ to produce those](https://poe.com/s/gxaODiT6PRnObHHPXE9n)) of **this document** (and thus am allowed to waive all patent/copyright rights and allow all to use,) and choose to waive all patent/copyright rights and allow all to use.
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
  - Requires 256kB **flash** (disk) and 16kB **RAM** (core mem) on the _Arduino_ (or _Elegoo_) board to use, but does not rely on personal computers to execute.
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
\[**Notice**\]: _Arduino_ (and counterpart _Elegoo_) circuitboard pins output at 5v with a max of 40milliamps, which must go to the stepper motor (or servo)'s control signal; [if used to power large motors, the circuitboard's internal resistors are damaged and you must purchase a new circuitboard](https://docs.arduino.cc/learn/microcontrollers/digital-pins/#:~:text=milliamps,maximum).
- For motors without control signals (motors with just 2 pins, "+" and "-"), attach the circuitboard's output pins the control gates power transistors, and have those transistors power such motors..
  - Motors <500,000 volts and <200 amps can use [power **MOSFET**s](https://wikipedia.org/wiki/Power_MOSFET) (which have a minimum gate control voltage of 3v, so you must ensure that the output pin does not power other circuits which cause more than 1 volt droop).
  - Motors >500,000 volts and / or >500 amps must use [insulated-gate bipolar transistors](https://wikipedia.org/wiki/Insulated-gate_bipolar_transistor) (which have a minimum gate control voltage of 4v, so you must ensure that the output pin does not also power status indicators or other circuits which can cause voltage drop).

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
- `inverseKinematics` is 2D (`targetX` = longitude, `targetY` = lattitude); _Assistant_ "left it as an exercise to you" to have `inverseKinematics` include multiple limbs & altitude.

**Solution for altitude** ([_Assistant_ says that my solution will do.](https://poe.com/s/uwjbhSrSWTtenF4HTcAa)):
- For those 2 dimensions above, use `targetX` = distance from base, and `targetY` = altitude.
- For the third dimension, have a third servo (`targetZ` = radial direction from base) which turns the base around towards the goal.
- This is more simple to compute (versus a normal 3-dimensional solution), which allows use of the $1 _ATmega328P_ **CPU**.

# External resources
- [_Assistant_ suggests to replace _Ardunio Uno_'s _ATmega328P_ 16MHz 2KB (or _Arduino Mega_'s 16MHz 8KB _ATmega2560_) with 240MHz 4MB _ESP32_ for $6](https://poe.com/s/0E2w6XDUf6Aw5hUpOA68)
  - [_Arduino_ _UNO R3_](https://docs.arduino.cc/hardware/uno-rev3/) circuitboard
  - [_Arduino_ _Mega 2560 Rev3_](https://docs.arduino.cc/hardware/mega-2560/) circuitboard
  - [_Arduino_ _Nano ESP32_](https://docs.arduino.cc/hardware/nano-esp32/) circuitboard
  - [_Espressif_ _ESP32_](https://www.espressif.com/en/products/socs/esp32) **CPU**
- [_Assistant_ lists consensus formulas and efficient synchronization protocols for obstacle avoidance through planned routes](https://poe.com/s/7ro9aNEONpb3GiQ9SyWE)
- [\[Preview\] Have computers do most of central nervous system, such as thalamus, auditory cortex, visual cortices, homunculus](./CnsCompress.md)
- [SubStack post which has list of related posts at bottom](https://swudususuwu.substack.com/p/how-to-mix-blender-with-robotics)
- [Pseudocode (+ some _Arduino_/_Elegoo_ code) from _Assistant_, which plans tasks and controls motor output](https://poe.com/s/bkHWeb6vWGFf3CdUzIch)

