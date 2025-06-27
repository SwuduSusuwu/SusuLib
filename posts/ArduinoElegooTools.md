**\[Preview\] How to use [_FLOSS_](https://wikipedia.org/wiki/FLOSS) code to produce autonomous [_Arduino_](https://wikipedia.org/wiki/Arduino)/[_Elegoo_](https://www.elegoo.com/blogs/arduino-projects) tools**

\[This post [from _SubStack_](https://swudususuwu.substack.com/p/how-to-use-floss-systems-to-produce) allows [_all uses_](https://creativecommons.org/licenses/by/2.0/).\] \[Disclaimer: am not affiliated to _Arduino_, nor to _Elegoo_.\] This is a work in progress (a "rough draft") which (through the next month) shall have much added, removed, or improved.

# Table of Contents
- [Licenses](#licenses) (so permissive that you can ignore this.)
- [Intro](#intro)
- [Howto route](#howto-route)
  - [Utils: Shared Functions for C++ and Python](#utils-shared-functions-for-c-and-python)
  - [Unified Modular Robot Controller Example](#unified-modular-robot-controller-example)
- [IMU (Accelerometer / Gyroscope)](#imu-accelerometer--gyroscope)
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

What follows is [code from _Anthropic_'s _Assistant_](https://poe.com/s/bkHWeb6vWGFf3CdUzIch) (just examples of what this post is about; future versions of this post will include own code, once am more experienced with robotic tools).
- [This code was improved through _GitHub_'s _Copilot_](https://github.com/copilot/share/8217120c-09e0-8482-a813-700120634846).
- Just this post (`posts/ArduinoElegooTools.md`) includes _Assistant_-produced code; all other posts / code was human-produced). All code was human-reviewed.

# Utils: Shared Functions for C++ and Python

## C++ (Arduino-style) Utils

```cpp
/*
 * (C) 2025 Swudu Susuwu
 * Dual licensed: Apache 2.0, GPLv2, and optionally Boost Software License 1.0.
 */
#pragma once
#ifndef UTILS_ROBOT_CONTROL_HXX
#define UTILS_ROBOT_CONTROL_HXX

#include <cmath>
#include <vector>
#include <cstring>

// 3D Vector utility
struct Vector3 {
	float x, y, z;
	Vector3(float x_=0, float y_=0, float z_=0): x(x_), y(y_), z(z_) {}
	Vector3 operator+(const Vector3& v) const { return Vector3(x+v.x, y+y.v, z+v.z); }
	Vector3 operator-(const Vector3& v) const { return Vector3(x-v.x, y-v.y, z-v.z); }
	Vector3 operator*(float s) const { return Vector3(x*s, y*s, z*s); }
	Vector3 operator/(float s) const { return (*this) * (1.0f / s); }
	float norm() const { return std::sqrt(x*x + y*y + z*z); }
	Vector3 normalized() const { return (*this) / norm(); }
};

inline float distance(const Vector3& a, const Vector3& b) {
	return (a - b).norm();
}

enum class Platform { ARDUINO, SIMULATOR, KUKA, FANUC };
enum class MotionType { WHEELED, LIMBED };

// Abstract base controller for all robots
class RobotControllerBase {
protected:
	Platform platform;
	MotionType motionType;
public:
	RobotControllerBase(Platform p, MotionType m) : platform(p), motionType(m) {}
	virtual void moveTo(const Vector3& target) = 0;
	virtual ~RobotControllerBase() {}
	Platform getPlatform() const { return platform; }
	MotionType getMotionType() const { return motionType; }
	void setPlatform(Platform p) { platform = p; }
	void setMotionType(MotionType m) { motionType = m; }
};

// Wheeled robot controller (rolling)
class WheeledRobotController : public RobotControllerBase {
public:
	WheeledRobotController(Platform p) : RobotControllerBase(p, MotionType::WHEELED) {}
	void moveTo(const Vector3& target) override {
		switch(platform) {
		case Platform::ARDUINO:
			// Example: setMotorSpeedsForTarget(target);
			break;
		case Platform::SIMULATOR:
			// Example: sendCommandToSimulator(target);
			break;
		case Platform::KUKA:
			// Example: kukaSendMoveCommand(target);
			break;
		default:
			break;
		}
	}
};

// Limbed robot controller (walking)
class LimbedRobotController : public RobotControllerBase {
public:
	LimbedRobotController(Platform p) : RobotControllerBase(p, MotionType::LIMBED) {}
	void moveTo(const Vector3& target) override {
		switch(platform) {
		case Platform::ARDUINO:
			// Example: computeIKAndMove(target);
			break;
		case Platform::SIMULATOR:
			// Example: sendIKToSimulator(target);
			break;
		case Platform::KUKA:
			// Example: kukaSendIKCommand(target);
			break;
		default:
			break;
		}
	}
};
#endif /* ndef UTILS_ROBOT_CONTROL_HXX */
```

## Python Utils

```python
# (C) 2025 Swudu Susuwu
# Dual licensed: Apache 2.0, GPLv2, optionally Boost Software License 1.0.

import math

class Platform:
	ARDUINO = 'arduino'
	SIMULATOR = 'simulator'
	KUKA = 'kuka'
	FANUC = 'fanuc'

class MotionType:
	WHEELED = 'wheeled'
	LIMBED = 'limbed'

class Vector3:
	def __init__(self, x=0, y=0, z=0):
		self.x, self.y, self.z = x, y, z
	def __add__(self, other):
		return Vector3(self.x+other.x, self.y+other.y, self.z+other.z)
	def __sub__(self, other):
		return Vector3(self.x-other.x, self.y-other.y, self.z-other.z)
	def __mul__(self, scalar):
		return Vector3(self.x*scalar, self.y*scalar, self.z*scalar)
	def __truediv__(self, scalar):
		return self * (1.0/scalar)
	def norm(self):
		return math.sqrt(self.x**2 + self.y**2 + self.z**2)
	def normalized(self):
		return self / self.norm() if self.norm() > 0 else Vector3()
def distance(a, b):
	return (a - b).norm()

class RobotControllerBase:
	def __init__(self, platform, motion_type):
		self.platform = platform
		self.motion_type = motion_type
	def move_to(self, target: Vector3):
		raise NotImplementedError
	def set_platform(self, platform):
		self.platform = platform
	def set_motion_type(self, mtype):
		self.motion_type = mtype

class WheeledRobotController(RobotControllerBase):
	def __init__(self, platform):
		super().__init__(platform, MotionType.WHEELED)
	def move_to(self, target: Vector3):
		if self.platform == Platform.ARDUINO:
			# Example: set_motor_speeds_for_target(target)
			pass
		elif self.platform == Platform.SIMULATOR:
			# Example: send_command_to_simulator(target)
			pass
		elif self.platform == Platform.KUKA:
			# Example: kuka_send_move_command(target)
			pass
		else:
			pass

class LimbedRobotController(RobotControllerBase):
	def __init__(self, platform):
		super().__init__(platform, MotionType.LIMBED)
	def move_to(self, target: Vector3):
		if self.platform == Platform.ARDUINO:
			# Example: compute_ik_and_move(target)
			pass
		elif self.platform == Platform.SIMULATOR:
			# Example: send_ik_to_simulator(target)
			pass
		elif self.platform == Platform.KUKA:
			# Example: kuka_send_ik_command(target)
			pass
		else:
			pass
```

# Unified Modular Robot Controller Example

## C++ Example

```cpp
class Nanobot {
public:
	Vector3 location, target, velocity;
	RobotControllerBase* controller;
	float energy;
	std::vector<Vector3> plannedRoute;
	Nanobot(Vector3 initialLocation, RobotControllerBase* ctrl)
		: location(initialLocation), target(), velocity(), energy(100.0f), controller(ctrl) {}
	void moveTo(const Vector3& tgt) {
		target = tgt;
		controller->moveTo(target);
	}
};

// Usage example:
WheeledRobotController myWheeled(Platform::ARDUINO);
LimbedRobotController myLimbed(Platform::SIMULATOR);

Nanobot wheeledBot(Vector3(0,0,0), &myWheeled);
Nanobot limbedBot(Vector3(1,1,0), &myLimbed);

wheeledBot.moveTo(Vector3(10,0,0)); // Moves using Arduino wheeled backend
limbedBot.moveTo(Vector3(1,1,1));   // Moves using simulator limbed backend
```

## Python Example

```python
class Nanobot:
	def __init__(self, initial_location, controller):
		self.location = initial_location
		self.target = Vector3()
		self.velocity = Vector3()
		self.energy = 100.0
		self.controller = controller
		self.planned_route = []
	def move_to(self, target):
		self.target = target
		self.controller.move_to(target)

my_wheeled = WheeledRobotController(Platform.ARDUINO)
my_limbed = LimbedRobotController(Platform.SIMULATOR)

wheeled_bot = Nanobot(Vector3(0,0,0), my_wheeled)
limbed_bot = Nanobot(Vector3(1,1,0), my_limbed)

wheeled_bot.move_to(Vector3(10,0,0))   # Moves using Arduino wheeled backend
limbed_bot.move_to(Vector3(1,1,1))     # Moves using simulator limbed backend
```

# IMU (Accelerometer / Gyroscope)

## C++ Example (MPU6050, 3D)

```cpp
#include <Wire.h>
#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 mpu;
Vector3 totalRotation(0,0,0);
Vector3 relativeVelocity(0,0,0);
Vector3 relativePosition(0,0,0);
unsigned long previousTimestamp = 0;

void setup() {
	Serial.begin(9600);
	if (!mpu.begin()) {
		Serial.println("Failed to initialize MPU6050 sensor!");
		while (1);
	}
	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);
	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
	previousTimestamp = millis();
}

void loop() {
	sensors_event_t a, g, temp;
	mpu.getEvent(&a, &g, &temp);

	// Integrate gyroscope to accumulate total rotation
	totalRotation.x += g.gyro.x;
	totalRotation.y += g.gyro.y;
	totalRotation.z += g.gyro.z;

	unsigned long currentTimestamp = millis();
	float elapsedTime = (currentTimestamp - previousTimestamp) / 1000.0;
	previousTimestamp = currentTimestamp;

	relativeVelocity.x += a.acceleration.x * elapsedTime;
	relativeVelocity.y += a.acceleration.y * elapsedTime;
	relativeVelocity.z += a.acceleration.z * elapsedTime;

	relativePosition.x += relativeVelocity.x * elapsedTime;
	relativePosition.y += relativeVelocity.y * elapsedTime;
	relativePosition.z += relativeVelocity.z * elapsedTime;

	Serial.print("Relative Position: (");
	Serial.print(relativePosition.x);
	Serial.print(", ");
	Serial.print(relativePosition.y);
	Serial.print(", ");
	Serial.print(relativePosition.z);
	Serial.println(") units");
	delay(100);
}
```


## Python Example (with pyfirmata or other serial comms)

```python
# Pseudocode, as actual Arduino IMU sensor reading from PC/Python would require serial protocol
imu_data = {
	"acceleration": Vector3(0,0,0),
	"gyroscope": Vector3(0,0,0)
}
total_rotation = Vector3(0,0,0)
relative_velocity = Vector3(0,0,0)
relative_position = Vector3(0,0,0)
previous_timestamp = None

def read_imu_data():
	# Replace this with serial read from Arduino
	return {
		"acceleration": Vector3(0.01, 0.0, 0.0),
		"gyroscope": Vector3(0.0, 0.001, 0.0)
	}

import time
while True:
	current_data = read_imu_data()
	now = time.time()
	if previous_timestamp is None:
		previous_timestamp = now
	elapsed_time = now - previous_timestamp
	previous_timestamp = now

	total_rotation.x += current_data["gyroscope"].x
	total_rotation.y += current_data["gyroscope"].y
	total_rotation.z += current_data["gyroscope"].z

	relative_velocity.x += current_data["acceleration"].x * elapsed_time
	relative_velocity.y += current_data["acceleration"].y * elapsed_time
	relative_velocity.z += current_data["acceleration"].z * elapsed_time

	relative_position.x += relative_velocity.x * elapsed_time
	relative_position.y += relative_velocity.y * elapsed_time
	relative_position.z += relative_velocity.z * elapsed_time

	print(f"Relative Position: ({relative_position.x}, {relative_position.y}, {relative_position.z}) units")
	time.sleep(0.1)
```

# Howto produce point clouds

## Depth-image to point cloud (Python)

```python
import numpy as np
import cv2

def create_point_cloud(color_image, depth_image, fx, fy, cx, cy):
	height, width, _ = color_image.shape
	x, y = np.meshgrid(np.arange(width), np.arange(height))
	z = depth_image / 1000.0
	x = (x - cx) * z / fx
	y = (y - cy) * z / fy
	point_cloud = np.stack((x, y, z), axis=-1)
	point_cloud_rgb = np.concatenate((point_cloud.reshape(-1, 3), color_image.reshape(-1, 3) / 255.0), axis=-1)
	return point_cloud_rgb
```

## Optical flow to point cloud (Python)

```python
import cv2
import numpy as np

def calculate_optical_flow(prev_frame, next_frame):
	prev_gray = cv2.cvtColor(prev_frame, cv2.COLOR_BGR2GRAY)
	next_gray = cv2.cvtColor(next_frame, cv2.COLOR_BGR2GRAY)
	flow = cv2.calcOpticalFlowFarneback(prev_gray, next_gray, None, 0.5, 3, 15, 3, 5, 1.2, 0)
	return flow

def estimate_depth_from_motion(flow, known_height):
	h, w = flow.shape[:2]
	depth_map = np.zeros((h, w))
	for y in range(h):
		for x in range(w):
			dx, dy = flow[y, x].astype(float)
			depth = known_height / (1 + np.sqrt(dx**2 + dy**2))
			depth_map[y, x] = max(depth, 0)
	return depth_map

def create_point_cloud(color_frame, flow, depth_map):
	h, w = depth_map.shape
	points = []
	for y in range(h):
		for x in range(w):
			z = depth_map[y, x]
			if z > 0:
				point = (x, y, z)
				color = color_frame[y, x] / 255.0
				points.append((*point, *color))
	return np.array(points)
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
`
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

# Howto produce distortion coefficients

```python
# Pseudocode for Estimating Lens Distortion Coefficients

# Step 1: Capture known points and observed points
known_points = loadKnownPoints()  # Load known accurate points (e.g., from a calibration pattern)
observed_points = captureObservedPoints()  # Capture points from the new sensor

# Step 2: Initialize coefficients
k1, k2, k3 = 0.0, 0.0, 0.0
p1, p2 = 0.0, 0.0

# Step 3: Compute error for each point
errors = []
for i in range(len(known_points)):
	expected_x, expected_y = known_points[i]
	observed_x, observed_y = observed_points[i]
	r = np.sqrt(observed_x**2 + observed_y**2)
	radial_distortion = observed_x * (1 + k1 * r**2 + k2 * r**4 + k3 * r**6)
	tangential_distortion_x = observed_x + (2 * p1 * observed_x * observed_y + p2 * (r**2 + 2 * observed_x**2))
	tangential_distortion_y = observed_y + (p1 * (r**2 + 2 * observed_y**2) + 2 * p2 * observed_x * observed_y)
	error_x = expected_x - tangential_distortion_x
	error_y = expected_y - tangential_distortion_y
	errors.append((error_x, error_y))

# Step 4: Optimize coefficients based on errors
# k1, k2, k3, p1, p2 = optimizeCoefficients(errors)
```

## Howto use coefficients for distortion correction

```python
# Pseudocode for Camera Distortion Correction

def correctDistortion(image, distortion_coefficients, fx, fy, cx, cy):
	corrected_image = np.zeros_like(image)
	k1, k2, p1, p2, k3 = distortion_coefficients
	height, width = image.shape[:2]
	for y in range(height):
		for x in range(width):
			xn = (x - cx) / fx
			yn = (y - cy) / fy
			r2 = xn**2 + yn**2
			radial = 1 + k1*r2 + k2*(r2**2) + k3*(r2**3)
			dx = 2 * p1 * xn * yn + p2 * (r2 + 2 * xn**2)
			dy = p1 * (r2 + 2 * yn**2) + 2 * p2 * xn * yn
			x_corr = xn * radial + dx
			y_corr = yn * radial + dy
			u = int(x_corr * fx + cx)
			v = int(y_corr * fy + cy)
			if 0 <= u < width and 0 <= v < height:
				corrected_image[v, u] = image[y, x]
	return corrected_image
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

