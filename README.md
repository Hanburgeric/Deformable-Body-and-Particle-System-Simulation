# Deformable Body and Particle System Simulation

An extension of the previous project, [Pinball](https://github.com/Hanburgeric/Pinball), asdf.

<br>

## Features
### Deformable Body (Cloth)
asdf.<br>
<p align=center>
<img width=400 src="https://i.giphy.com/media/v1.Y2lkPTc5MGI3NjExZmkwM3g2OGlmaTZqamJhb2liaDN4azRlOG4wbXV3YmkzeTljaHRkbyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/fFlgq2DULa0ZIrykQi/giphy.gif"/>
</p><br>

### Particle System (Fluid)
asdf.<br>
<p align=center>
<img width=400 src="https://i.giphy.com/media/v1.Y2lkPTc5MGI3NjExMnJ0cXpmcHF2OWh4dTY1Y25wZWdhMTIya21tOTAyaGR0eG1pdmpreCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/LTixAYGXSXNqvoVDsA/giphy.gif"/>
</p><br>

### Rigid Body (Sphere)
asdf.<br>
<p align=center>
<img width=400 src="https://i.giphy.com/media/v1.Y2lkPTc5MGI3NjExaWJmMWl5dnB0b2VsdDg5OHUzazduaDF3bms0cHg0dTR2dXppMTN2cyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/aJqAZZKFszaogWyO7c/giphy.gif"/>
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

Rigid Body (Sphere)
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
