# Character-Animation

Developed in C++ and OpenGL, this real-time character animation system employs advanced skeletal animation techniques using Assimp, featuring keyframe interpolation, hierarchical modeling, and dual quaternion skinning for smooth and realistic deformations. The system supports complex bone hierarchies and utilizes Physically-Based Rendering (PBR) and Image-Based Lighting (IBL) for high visual fidelity, along with an HDR skybox for a dynamic environment. Parallelized using OpenMP, the project delivers high performance by distributing computations across bone tree branches.

## Features

### Graphics and Rendering:

- OpenGL Rendering: The project harnesses OpenGL for real-time rendering, providing advanced visual capabilities.
- Physically-Based Rendering (PBR): Implements realistic shading and material rendering, capturing accurate light interactions on character surfaces.
- Image-Based Lighting (IBL): Enhances scene realism with accurate reflections and illumination based on environment textures.
- Shadow Mapping: Supports dynamic shadows, adding depth and enhancing the 3D perception of character models.
- HDR Skybox: Creates a dynamic, immersive background, contributing to the overall atmosphere of the scene.

### Character Animation:

- Skeletal Animation: Features a robust skeletal animation system with support for complex bone hierarchies and keyframe interpolation.
- Dual Quaternion Skinning: Utilizes advanced skinning techniques for smooth and natural deformations of animated characters.
- Hierarchical Modeling: Ensures that child bones move in sync with parent bones, preserving animation integrity.
- Inverse Kinematics (IK): Provides real-time control over character limbs, enabling natural movement and interaction with the environment.
- Physics-Based Movement: Integrates physics simulations for more realistic character responses during dynamic actions.
- Parallelization with OpenMP: Enhances performance by parallelizing animation computations across bone tree branches.

### Animation and Interaction:

- FBX Model Import: Uses Assimp to import FBX models and animations created in Blender, allowing for flexible and detailed character animations.
- Real-Time Interactions: Supports real-time adjustments and responses, making animations feel responsive and alive.
- Animation State Machine: Manages and blends animations smoothly, ensuring seamless transitions between different actions.

### Performance Optimization:

OpenMP Parallelization: Boosts performance by efficiently distributing animation calculations, reducing processing time and enabling real-time performance even with complex models.
Efficient Bone Parsing and Animation Loading: Optimizes the loading of complex bone structures and animations, ensuring smooth operation without lag.

## License

This project is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
Read the full license here.

## Installation

1. Clone the repository:
```
git clone https://github.com/AmrHMorsy/character_animation-.git
```
2. Navigate to the project directory: 
```
cd character_animation
```
3. Create a build directory: 
```
mkdir build
```
4. Navigate to the build directory: 
```
cd build
```
5. Generate makefiles using cmake: 
```
cmake ../
```
6. compile using makefile: 
```
make
