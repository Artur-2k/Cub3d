# cub3D

## Demo

![demo](Assets/demo.gif)


## Description

Cub3D is a project from 42 where you transform a 2D tiled map into a 3D environment, inspired by classic games like Wolfenstein.
The project relies on the MiniLibX graphics library, an API designed to facilitate the creation of basic graphical applications and interaction with windowing systems.
It starts by parsing the map to ensure it follows the correct structure - valid file format, textures and map itself - before moving on to the 3D rendering. For this, we used raycasting with the DDA algorithm to simulate the player’s viewpoint and generate a 3D environment by calculating distances to walls. It’s a hands-on project that teaches graphics programming, raycasting, and algorithm optimization.

## Features

- File Validation: Ensures correct map extension, valid file format, valid textures, and proper map formatting.
- Window Management: Handled using the MiniLibX graphics library.
- 3D Rendering: Utilizes raycasting and the DDA algorithm to generate a 3D environment.
- Resource Management: Focuses on making the program error-free and leak-free.
- Player Movement: Movement of the player with WADS, left and right arrow keys

## Getting Started

### Dependencies

- **Operating System**: Unix-based OS (Linux or macOS)
- **Libraries**: libX11, libXext, libbsd and libm for rendering and mathematical computations.

#### Installation on Linux/macOS:

Ensure you have the necessary dependencies installed before proceeding.

- **Debian/Ubuntu**: ```sudo apt install libx11-dev libxext-dev libbsd-dev libm-dev```

For MacOS you'll have to switch the minilibx distro on code/lib

### Installing

1. **Clone the Repository**

Download the project by running:

```
https://github.com/Artur-2k/cub3d
```

2. **Navigate to the Project Directory**

```
cd cub3d
```

3. **Set Up the Project** 
    
Build or prepare the project with:

```
make all
```

## Usage

Run the application with the following command:

```
./cub3D Assets/Maps/Valid/<map-of-choice>
```

Move around with WASD keys and rotate the camare with left and right arrow keys

Example:

```
./cub3D Assets/Maps/Valid/library.cub
```

## Authors

- [Artur Carvalho](https://github.com/Artur-2k)
 
- [Davi Melo](https://github.com/Davi0805)



