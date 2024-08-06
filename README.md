# Gravitational Particle Simulation

This project is a continuation of fluid simulation concepts, extended to simulate gravitational interactions between particles. The simulation calculates gravitational forces between particles and visualizes their dynamic interactions in a 2D space using OpenGL and GLFW.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Code Structure](#code-structure)
- [Acknowledgments](#acknowledgments)
- [License](#license)

## Introduction

This project builds on the principles of fluid simulation by focusing on gravitational interactions between particles. The primary goal is to simulate how particles attract each other based on gravitational forces, similar to celestial bodies.

The project uses Newton's law of universal gravitation, where the force between two particles is inversely proportional to the square of the distance between them. The resulting simulation demonstrates complex and dynamic behavior as particles interact in real time.

## Build

To set up and compile the project on your local machine, follow these steps:

1. **Clone the Repository**: Start by cloning the project repository from GitHub. This will create a local copy of the code on your machine.

   ```bash
   git clone https://github.com/shamykyzer/gravity-simulation.git
   cd gravity-simulation

2. **Navigate to the project directory**: 
    ```bash
    cd gravity-simulation
    ```
3. **Compile the project using the provided 'Makefile'**:
    ```bash
    make
    ```
This command will compile the source files and produce an executable named `gravity_simulation` in the root directory.

4. **Run the executable to start the simulation**:
   ```bash
   ./gravity_simulation
   ```

## Features

- Simulates gravitational forces between particles based on their mass and distance.
- Real-time visualization using OpenGL and GLFW.
- Wrap-around behavior to keep particles within the visible simulation area.
- Customizable particle attributes, such as mass and initial position.

## Installation

To build and run the simulation, ensure you have the necessary dependencies installed:
##Usage
