# Deformable Body and Particle System Simulation

An extension of the previous project, [Pinball](https://github.com/Hanburgeric/Pinball), this time exploring , namely deformable bodies and fluids.

As such, the project abandons Box2D in favor of a proprietary physics engine.

<p align=center>
<img width=720 src="https://i.giphy.com/media/v1.Y2lkPTc5MGI3NjExYTVvOTZwMHA3ejd0d2lzN2h5NHhvdmtwZWlhM3Bxb3o3cHVzN2Y3YiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/At0gGRg8mntzr3S1C8/giphy.gif"/>
</p><br>

<br>

## Features
### Deformable Body
A deformable body in the form of a cloth, modeled using a simple mass-spring system (MSS).

<br>Interacts with the rigid body, but NOT the particle system (yet).<br>
<p align=center>
<img width=720 src="https://i.giphy.com/media/v1.Y2lkPTc5MGI3NjExenV3bWV1MzRvZDZtaGtsZGp0c3UwYzJwcmlzbDU1d29yMWowYmZ6bCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/Or2sPBUWa1bJbqjRKz/giphy.gif"/>
</p><br>

### Particle System
A particle system meant to simulate fluids.

<br>Interacts with the each other and the rigid body, but NOT the deformable body (yet).<br>
<p align=center>
<img width=720 src="https://i.giphy.com/media/v1.Y2lkPTc5MGI3NjExMjI0YXk5djN2eWZ4MHZtbWxrNjJodnlkMHgxdnA2bTlvc2IxYXJkNiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/UajdTZ5v8naqE9vkL9/giphy.gif"/>
</p><br>

### Rigid Body
asdf.

<br>Interacts with both the deformable body and the particle system.<br>
<p align=center>
<img width=720 src="https://i.giphy.com/media/v1.Y2lkPTc5MGI3NjExNzhwMnJocDA4NmlvOWF4NTVtb3VsZXZvbjV4djMwNGZ2OG8yenVjZyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/bMrOlzP3KGUIwKGQK8/giphy.gif"/>
</p><br>

## Build and Deployment
In the case that the .exe does not work (normally due to library linking errors), compile and run the code from Visual Studio by following the following steps:
 1. Set active solution configuration to
    <br>`Release`.
 3. Set active solution platform to
    <br>`x64`.
 5. In project properties, under `Debugging`, set `Environment` to
    <br>`PATH=../dll;%PATH$`.
 6. In project properties, under `C/C++`, set `Additional Include Directories` to
    <br>`../include;%(AdditionalIncludeDirectories)`.
 8. In project properties, under `Linker`, set `Additional Library Directories` to
    <br>`../lib/GL;%(AdditionalLibraryDirectories)`.
 10. In project properties, under `Linker` → `Input`, set `Additional Dependencies` to
     <br>`freeglut.lib;freeglutd.lib;glew32.lib;%(AdditionalDependencies)`.

<br>

## Controls
Simulation
 - `Spacebar` : start/pause the entire simulation.
 - `[` : start/pause the deformable body simulation.
 - `]` : start/pause the particle system simulation.
 - `r` : restart the entire simulation.
<br>

Deformable Body
 - `1` : view deformable body as nodes.
 - `2` : view deformable body as springs.
 - `3` : view deformable body as texture.
<br>

Particle System
 - `F1` : check collisions for every particle.
 - `F2` : check collisions for every particle that has not yet been checked.
 - `F3` : check collisions for particles using a neighbor search with spatial hashing. 
 - `+` : add 100 particles.
 - `-` : subtract 100 particles.
<br>

Rigid Body
 - `w` : move sphere forward.
 - `a` : move sphere left.
 - `s` : move sphere backward.
 - `d` : move sphere right.
<br>

Camera
 - `LMB` + Drag : move camera.
 - `RMB` + Drag : rotate camera.
 - `Scroll` : zoom camera.
<br>

## To-Do
 - Implement collisions between the deformable body and particles.
 - Update code to use modern OpenGL. 
 - Otherwise general refactoring of code to meet current standards.

<br>

## Development Environment
 - IDE : Visual Studio
 - Compiler : MSVC
 - Libraries :
   - [FreeGLUT](https://freeglut.sourceforge.net/)
   - [GLEW](https://glew.sourceforge.net/)
