
# Gravitational Particle Simulation

This project is a continuation of fluid simulation concepts, extended to simulate gravitational interactions between particles. The simulation calculates gravitational forces between particles and visualizes their dynamic interactions in a 2D space using OpenGL and GLFW.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Build](#build)
- [Usage](#usage)
- [Code Structure](#code-structure)
- [Acknowledgments](#acknowledgments)
- [License](#license)

## Introduction

This project builds on the principles of fluid simulation by focusing on gravitational interactions between particles. The primary goal is to simulate how particles attract each other based on gravitational forces, similar to celestial bodies.

The project uses Newton's law of universal gravitation, where the force between two particles is inversely proportional to the square of the distance between them. The resulting simulation demonstrates complex and dynamic behavior as particles interact in real time.

## Features

- Simulates gravitational forces between particles based on their mass and distance.
- Real-time visualization using OpenGL and GLFW.
- Wrap-around behavior to keep particles within the visible simulation area.
- Customizable particle attributes, such as mass and initial position.

## Installation

To build and run the simulation, ensure you have the necessary dependencies installed:

### Dependencies

- OpenGL
- GLEW
- GLFW

On Ubuntu, you can install these with:

\`\`\`bash
sudo apt-get update
sudo apt-get install build-essential libgl1-mesa-dev libglew-dev libglfw3-dev
\`\`\`

## Build

To set up and compile the project on your local machine, follow these steps:

1. **Clone the Repository**: Start by cloning the project repository from GitHub. This will create a local copy of the code on your machine.

   \`\`\`bash
   git clone https://github.com/shamykyzer/gravity-simulation.git
   \`\`\`

2. **Navigate to the project directory**:

   \`\`\`bash
   cd gravity-simulation
   \`\`\`

3. **Compile the project using the provided \`Makefile\`**:

   \`\`\`bash
   make
   \`\`\`

   This command will compile the source files and produce an executable named \`gravity_simulation\` in the root directory.

4. **Run the executable to start the simulation**:

   \`\`\`bash
   ./gravity_simulation
   \`\`\`

## Usage

To run the simulation:

1. Execute the compiled binary:

   \`\`\`bash
   ./gravity_simulation
   \`\`\`

2. A window will open displaying particles interacting under gravitational forces. The particles are initially placed randomly within the simulation area and will attract each other according to Newton's law of universal gravitation.

### Simulation Controls

- **Mouse Interaction**: Click and hold the left mouse button to attract particles towards the mouse position. This simulates an additional gravitational pull originating from the mouse location.
- **Wrap-around**: Particles that move beyond the visible boundaries of the simulation will reappear on the opposite side, creating a continuous space effect.

### Observing the Simulation

Watch as the particles dynamically interact, forming clusters and complex orbital paths due to gravitational attraction. The simulation demonstrates both local interactions and larger structures emerging over time.

## Code Structure

- **\`include/particle.h\`**: Defines the \`Particle\` structure, which includes position, velocity, mass, and color components, along with function declarations for initializing and updating particles.
- **\`src/particle.c\`**: Implements particle initialization and gravitational force calculations.
- **\`src/main.c\`**: Sets up the OpenGL context and runs the simulation loop.
- **\`Makefile\`**: Used to compile the project and manage dependencies.

## Acknowledgments

This project was inspired by [Brendan Galea's YouTube video](https://www.youtube.com/watch?v=GjbcvqEOIuE&ab_channel=BrendanGalea) on gravitational systems. Special thanks to Brendan Galea for sharing his insights and sparking new ideas.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
"""

