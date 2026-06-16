# Real-Time Particle Effects Engine

A GPU-instanced particle system written in C++ with OpenGL 3.3. Seven presets, full editor UI, HDR rendering with bloom.

![](screenshots/screenshot.png)

## Build (Windows: Visual Studio 2022)

Dependencies (GLFW, GLM, ImGui) are fetched automatically by CMake. No manual installs needed.

**1. Open the project**
- Open Visual Studio 2022
- File → Open → Folder
- Select the project folder (the one containing `CMakeLists.txt`)
- Wait for VS to finish configuring, you will see "CMake generation finished" in the Output panel

**2. Build**
- Build → Build All (Ctrl+Shift+B)
- Wait for Build All succeeded

**3. Run**
- Open the terminal in Visual Studio (View → Terminal) or a Command Prompt
- cd into the project root, the folder that contains CMakeLists.txt, for example:
  ```
  cd "C:\Users\aryan\Desktop\particle-effects-engine-new"
  ```
- Run the executable:
  ```
  out\build\x64-Debug\bin\ParticleEffectsEngine.exe
  ```
  > Must be launched from the project root so the exe can find the shaders/ folder. Double-clicking it from File Explorer will not work.

## Controls

| Input | Action |
|---|---|
| WASD | Move |
| Q / E | Up / down |
| Hold right mouse | Look around |
| 1 - 7 | Swap preset |
| Space | Burst |
| P / O | Pause / resume |

## Presets

fire, smoke, sparks, rain, magic, embers, snow

## Beyond standard rasterization

- **Curl noise turbulence** (Bridson 2007):  divergence-free 3D noise field perturbs particle velocities so smoke and embers swirl naturally instead of marching along clean trajectories
- **Soft particles**: fragments fade as they approach scene depth, eliminating hard clipping where smoke meets the ground
- **HDR + bloom**: scene renders to an RGBA16F framebuffer; bright pixels are thresholded, blurred with a separable Gaussian (ping-pong FBOs), and composited back with ACES tonemapping
- **Velocity-stretched billboards**: sparks and rain stretch their quads along their motion vector projected into screen space
- **GPU instancing**: one draw call for all live particles, 60K particle pool

## Project layout

```
src/         engine source
shaders/     glsl files loaded at runtime
third_party/ minimal glad loader
```
