# Robotic Arm Control System

**Course:** EGR 313  
**Prepared by:** George Henry Williams III  
**Date:** March 6, 2026  

---

## Project Overview
The goal of this project is to develop control software for a robotic arm system that allows a user to intuitively move the arm to desired positions in space. The system translates user commands into robotic motion while respecting mechanical constraints and safety limits.

This portion of the project focuses on:
- Software control
- Mathematical modeling of arm movement

Other team members will provide supporting data such as pressure mapping and actuator behavior, which will be integrated into the system.

---

## Primary Objective
The primary objective is to create a software control system that converts user input into robotic arm movement in a natural and intuitive way.

The system will:
- Accept user commands describing desired movement  
- Convert commands into mathematical representations  
- Use arm geometry and joint angles to determine actuator movement  
- Execute movements safely and accurately  

---

## Coordinate System Approach
To ensure intuitive control, the system will use a **Cartesian coordinate system** rather than a polar system.

### User Inputs:
- Move left  
- Move right  
- Move forward  
- Move backward  
- Move up  
- Move down  

The software converts these Cartesian inputs into joint angles and actuator commands.

---

## Mathematical Modeling
A mathematical model will represent the **geometry and kinematics** of the robotic arm.

The model will:
- Represent physical arm geometry  
- Use sensor inputs for joint angles  
- Convert Cartesian commands into joint movement  
- Enforce mechanical limits  

This enables the system to determine how each joint must move to reach a desired position.

---

## User Interface and Control Method

### Preferred Control Method
A **mobile phone application** connected via Bluetooth.

**Workflow:**
1. App connects to robot via Bluetooth  
2. User inputs directional commands  
3. Commands are sent to control software  
4. Robot executes movement  

**App Features:**
- Directional buttons  
- Basic coordinate display  
- Minimal, simple interface  

**Development Tools:**
- Python  
- Kivy  

---

### Backup Control Method
If a mobile app is not feasible:
- A **physical joystick** will be used as an alternative control interface  

---

## Robotic Arm Movement Behavior
The arm should move in a way that is:

- Predictable  
- Smooth  
- Intuitive  

### Behavior Requirements:
- Follow Cartesian movement directions  
- Stay within physical workspace  
- Maintain controlled motion  

The workspace should feel **rectangular/square**, not circular.

---

## Safety and Constraint Handling

### Joint Limits
- Each joint has a maximum rotation (~270°)  
- Software must prevent exceeding limits  

### Obstacle and Resistance Detection
If resistance is detected:
- Stop movement  
- Prevent further motion in that direction  
- Avoid damage  

### Workspace Protection
- Prevent movement outside safe operating range  

---

## Additional Functional Features

### Suction Mechanism
The robotic arm includes a suction cup system.

**Capabilities:**
- Activate suction  
- Deactivate suction  

This system integrates with sensors provided by teammates.

---

## Key Design Goals

### 1. Intuitive Control
Users should be able to control the arm easily with minimal learning.

### 2. Simplicity
Keep both software and interface simple and efficient.

### 3. Safety
Prevent damage and unsafe operation.

### 4. Compatibility
Ensure integration with team-developed sensors and hardware systems.

---

## Expected Outcome
By the end of the project, the system will include:

- Mathematical model of arm geometry  
- Software translating Cartesian inputs into motion  
- User interface (mobile app or joystick)  
- Safety constraints and motion limits  
- Integration with sensor and pressure systems  

---

## Final Result
A **user-friendly robotic arm control system** capable of accurate, safe, and intuitive movement within its workspace.