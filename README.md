# Nau

## Overview
A real-time GLSL renderer that visualizes a non-rotating black hole, gravitationally warped sky and a glowing accretion disk. This project is an exploration of the physics of black holes, built on the actual geodesic equations of general relativity rather than purely simulating visual effects.

## Exactness 
The light rays are traced with a simple RK4 integrator, based on the equations derived for a Schwarzschild black hole. While the underlying ray-tracing is based on actual physics, this is not a physically rigorous simulation: the RK4 integrator introduces approximation errors, and the accretion disk emission is entirely artistic rather than physically accurate. 

All the calculations can be found [here](CALCULATIONS.md)

## Screenshots
<img width="1659" height="997" alt="Screenshot 2026-02-28 at 19 40 20" src="https://github.com/user-attachments/assets/94f78016-4f80-4dae-b06f-5cec7c0f8820" />

## Technical stack
- C++20
- CMake 3.31
- OpenGL 3.3, glm, GLSL 330, GLFW, stb

## Installation
1. Clone the repository
```
git clone https://github.com/bokuyemskyy/nau
cd nau
```

2. Configure the project with CMake:
```
cmake -B build
```

3. Compile
```
cmake --build build
```

## Credits
- Formulas and calculations are based on [Schwarzschild metric Wikipedia](https://en.wikipedia.org/wiki/Schwarzschild_metric)
- Originally inspired by kavan's [Simulating Black Holes in C++](https://www.youtube.com/watch?v=8-B6ryuBkCM)


