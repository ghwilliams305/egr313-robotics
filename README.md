Project Objectives Document
Course: EGR 313
Project: Robotic Arm Control System
Prepared by: George Henry Williams III
Date: March 6, 2026

1. Project Overview
The goal of this project is to develop the control software for a robotic arm system that allows a user to intuitively move the arm to desired positions in space. The system will translate user commands into movements of the robotic arm while respecting the mechanical constraints and safety limits of the system.
The primary responsibility of this portion of the project is the software control and mathematical modeling required to translate user commands into robotic arm motion. Other team members will provide supporting data such as pressure mapping and actuator behavior, which will be integrated but are not the primary focus of this work.

2. Primary Objective
The primary objective is to create a software control system that converts user input into robotic arm movement so that the arm moves to the position the user intends in a way that feels natural and intuitive.
The system should:
    • Accept user commands describing desired movement.
    • Convert those commands into mathematical representations of arm motion.
    • Use the geometry and joint angles of the arm to determine the required actuator movement.
    • Execute those movements safely and accurately.

3. Coordinate System Approach
To ensure intuitive control, the system will use a Cartesian coordinate system for user inputs rather than a polar coordinate system.
This means the user will interact with the system using simple directional commands such as:
    • Move left
    • Move right
    • Move forward
    • Move backward
    • Move up
    • Move down
The software will then internally convert these Cartesian movements into the joint angles and actuator commands required to achieve the desired motion of the robotic arm.

4. Mathematical Modeling
A mathematical model will be developed to represent the geometry and kinematics of the robotic arm.
This model will:
    • Represent the physical geometry of the arm.
    • Use sensor inputs that measure the angles of each joint.
    • Convert Cartesian movement commands into appropriate joint movements.
    • Ensure that movements remain within the mechanical limits of the system.
This modeling process will allow the control software to determine how each joint must move to achieve the desired end position of the arm.

5. User Interface and Control Method
The system will prioritize ease of use and accessibility.
Preferred Control Method
The preferred interface is a simple mobile phone application that connects to the robotic arm via Bluetooth.
The proposed system would work as follows:
    • A mobile application connects to the robot using Bluetooth.
    • The user issues simple directional commands through the app.
    • The app sends commands to the robot control software.
    • The robot moves accordingly.
The mobile app will be intentionally simple and may include:
    • Directional movement buttons
    • Basic coordinate information
    • Minimal interface complexity
The app may be developed using Python-compatible tools such as Kivy.
Backup Control Method
If the mobile application is not feasible, a physical joystick control system may be used as a fallback interface.

6. Robotic Arm Movement Behavior
The robotic arm should move in a way that feels predictable and intuitive to the user.
The movement behavior should:
    • Follow direct Cartesian motion where possible.
    • Operate within the physical workspace of the arm.
    • Maintain smooth and controlled movement.
The arm's workspace should resemble a rectangular or square movement region from the user's perspective, rather than circular or polar-style movement.

7. Safety and Constraint Handling
The control system must enforce several safety and operational limits.
Joint Limits
Each joint has a maximum allowable rotation (for example, approximately 270°).
The software must ensure that commands do not cause joints to exceed their mechanical limits.
Obstacle and Resistance Detection
If the arm encounters unexpected resistance (such as hitting a wall or object), the system should:
    • Detect the abnormal resistance
    • Stop further movement in that direction
    • Prevent damage to the arm or surrounding environment
Workspace Protection
The system must ensure the robot does not move outside of its controllable or safe operating range.

8. Additional Functional Features
Suction Mechanism
The robotic arm will include a suction cup mechanism.
The system should allow the user to:
    • Activate suction
    • Deactivate suction
The control logic for suction will integrate with sensor information provided by other team members.

9. Key Design Goals
The project emphasizes the following guiding principles:
1. Intuitive Control
Users should be able to control the robotic arm with minimal learning.
2. Simplicity
Both the software and user interface should remain simple and easy to operate.
3. Safety
The system must prevent mechanical damage and avoid unsafe motions.
4. Compatibility
The control system should work with the sensor inputs and actuator systems developed by other team members.

10. Expected Outcome
By the end of the project, the system should provide:
    • A mathematical model of the robotic arm geometry
    • Software that translates Cartesian user inputs into robotic motion
    • A simple user control interface (mobile app or joystick)
    • Built-in safety constraints and motion limits
    • Integration with team-provided sensor and pressure systems
The final result will be a user-friendly robotic arm control system capable of accurate and safe movement within its workspace.