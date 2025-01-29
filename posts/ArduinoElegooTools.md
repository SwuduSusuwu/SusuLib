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
- [Howto produce point clouds](#howto-produce-point-clouds)
- [Howto use point clouds to route](#howto-use-point-clouds-to-route)
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
# Howto produce point clouds
Those [point clouds](https://wikipedia.org/wiki/Point_cloud) show obstacles which autonomous tools must [route around](https://wikipedia.org/wiki/Obstacle_avoidance).

Some pseudocode [from _Assistant_](https://poe.com/s/sx1MuZzPcLy3p9u0elpq) (just examples of what this post is about; future versions of this post will include own code):
## Depth-image to point cloud
Has the most simple code, but requires [advanced sensors](https://www.amazon.com/Consumer-Depth-Cameras-Computer-Vision/dp/1447146395) (such as [LIDAR](https://wikipedia.org/wiki/Lidar)) to produce images.
```
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
```
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
```
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
```
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
# Howto use point clouds to route
Start with [this code (also from _Assistant_)](https://poe.com/s/p0NTuGJRBDlol01YJY5c), which goes into the code from [Howto route](#howto-route):
```
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
```
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
  - \*since `direction.perpendicular()` can not go over or under obstacles.
```
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

# External resources
- [\[Preview\] Have computers do most of central nervous system, such as thalamus, auditory cortex, visual cortices, homunculus](./CnsCompress.md)
- [SubStack post which has list of related posts at bottom](https://swudususuwu.substack.com/p/how-to-mix-blender-with-robotics)
- [Pseudocode (+ some _Arduino_/_Elegoo_ code) from _Assistant_, which plans tasks and controls motor output](https://poe.com/s/bkHWeb6vWGFf3CdUzIch)
- [_NumPy_ + _CV2_ + _TensorFlow_ code from _Assistant_, which produces point clouds (for obstacle avoidance)](https://poe.com/s/sx1MuZzPcLy3p9u0elpq)
- [_CV2_ `calcOpticalFlowFarneback`](https://docs.opencv.org/4.x/d4/dee/tutorial_optical_flow.html#autotoc_md1162)
- [_MathWorks_ `opticalFlowFarneback`](https://www.mathworks.com/help/vision/ref/opticalflowfarneback.html)
- [_Consumer Depth Cameras for Computer Vision: Research Topics and Applications (Advances in Computer Vision and Pattern Recognition)_](https://www.amazon.com/Consumer-Depth-Cameras-Computer-Vision/dp/1447146395)

