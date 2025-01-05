**\[Preview\] How to use [_FLOSS_](https://wikipedia.org/wiki/FLOSS) code to produce autonomous [_Arduino_](https://wikipedia.org/wiki/Arduino)/[_Elegoo_](https://www.elegoo.com/blogs/arduino-projects) tools**

This is a work in progress (a rough draft) which (through the next month) shall have much redone/removed/improved.

\[This post [from _SubStack_](https://swudususuwu.substack.com/p/how-to-use-floss-systems-to-produce) allows [_all uses_](https://creativecommons.org/licenses/by/2.0/).\]

Tools compatible with this howto:
- TODO, but some from [this other post](https://swudususuwu.substack.com/p/how-to-mix-blender-with-robotics) have common uses:
  - [Grasshopper](https://wikipedia.org/wiki/Grasshopper_3D)
  - [RoboDK](https://wikipedia.org/wiki/RoboDK)
  - [V-REP/CoppeliaSim](https://wikipedia.org/wiki/CoppeliaSim)
  - [Gazebo](https://wikipedia.org/wiki/Gazebo)
# Table of Contents
- [Howto route](#howto-route)
- [External resources](#external-resources)
# Howto route
Some pseudocode [from _Assistant_](https://poe.com/s/bkHWeb6vWGFf3CdUzIch) (just examples of what this post is about; future versions of this post will include own code. Just this post uses _Assistant_-produced code, all other posts/code was human-produced):
## Abstract robot pseudocode
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
- `velocity` is 2D; _Assistant_ did not use conversation context to deduce that `velocity` must include altitude.
# Wheeled _Arduino_/_Elegoo_ robot code
```
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
# Limbed _Arduino_/_Elegoo_ robot code
```
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
- `inverseKinematics` is 2D; _Assistant_ "left it as an exercise to you" to have `inverseKinematics` include multiple limbs & altitude.
# External resources
- [\[Preview\] Have computers do most of central nervous system, such as thalamus, auditory cortex, visual cortices, homunculus](./CnsCompress.md)
- [SubStack post which has list of related posts at bottom](https://swudususuwu.substack.com/p/how-to-mix-blender-with-robotics)
- [Pseudocode (+ some _Arduino_/_Elegoo_ code) from _Assistant_, which plans tasks and controls motor output](https://poe.com/s/bkHWeb6vWGFf3CdUzIch)

