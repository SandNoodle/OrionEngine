# Project structure

Orion Engine is split into several modules with each one having its own CMakeLists.txt defining its own namespace. This
results in following directory structure:
```
Engine
\__ src
|   \__ assets
|   \__ audio
|   \__ core
|   \__ physics
|   \__ platform
|   \__ render
|   \__ scene
|   \__ script
\__ lib
\__ doc
\__ test
\__ CMakeLists.txt
\__ README.md
\__ LICENSE.md
```

Important details:
* base namespace for all the modules is `orion`.
* `core` module does not define its own namespace (uses `namespace orion { ... }`).
* any subdirectories created in a module do not define its own sub-namespace.

## Modules

### Platform

Contains and abstracts platform specific code.

Directory structure:
```
Platform
\__ filesystem - low level file system calls (ex. file watching, loading files).
\__ thread - platform threading abstraction.
\__ timer - platform high resolution clock abstraction.
\__ graphics - wrapper for rendering apis.
|   \__ opengl
|   |   \__ opengl_buffer.cpp
|   |   \__ ...
|   \__ vulkan
|   |   \__ vulkan_buffer.cpp
|   |   \__ ...
|   \__ device.h - used for creating buffers, shaders, etc.
|   \__ buffer.h
|   \__ shader.h
|   \__ texture.h
|   \__ framebuffer.h
|   \__ graphics_context.h
|   \__ compute_context.h
|   \__ pipeline.h
\__ memory - platform specific memory management
|   \__ memory_win32.cpp
|   \__ memory_linux.cpp
|   \__ memory.h
\__ platform.h - OS platform detection. Included in every other file.
```

### Core

Built upon `Platform` module.

Contains all the core, generic functionality used elsewhere by the other modules.

Directory structure:
```
Core
\__ defines
|   \__ type_defines.h - defines atomic data types (i32, u32, etc.).
|   \__ build_defines.h - contain build specific settings (logging level etc.).
|   \__ math_defines.h - defines math data types (vec2, mat4, etc.).
|   \__ defines.h - includes all the defines from above.
\__ memory - memory allocation.
\__ string - custom string implementation.
\__ config - runtime engine configuration (de)serialization.
\__ random - random number generators.
\__ math - vector, quaternion, etc. math.
\__ threads - high level job based threading implementation using platform independece layer.
\__ filesystem - async high-level file system implementation using platform independence layer.
\__ engine_config.h - compile time `#define`s guarded with `#ifndef`, `#endif`.
\__ assertion.h
\__ logger.h
\__ profiler.h
```

### Assets

Built upon `Core` module.

Contains all the asset management functionality, ex. compiling, loading, caching.

Directory structure:
```
Assets
\__ texture_asset
\__ level_asset
\__ font_asset
\__ static_mesh_asset
\__ skeletal_mesh_asset
\__ sound_mesh_asset
\__ ...
\__ asset_cache.h - returns loaded asset (or loads it beforehand if not contained in the cache).
\__ asset_loader.h - loads asset from custom binary format into corresponding assets struct inside the engine.
\__ asset_compiler.h - compiles asset in a popular format (ex. OBJ) into engine's custom binary format.
```

### Low Level Renderer

Built upon `Platform` and `Core` modules.

Operates on graphics api concepts abstracted away in `Platform` module.
Introduces a bit higher (but still relatively low) level concepts used for rendering a scene, ex.
* Materials - collections of texture views and shaders.
* Lights
* Cameras

Following rednderes are introduced here:
 - Debug - drawing of debug lines, boxes, etc.
 - Texts & Fonts
 - Meshes (Static, Skeletal)

Directory structure:
```
Low Level Renderer
\__ material
\__ light
|    \__ area_light.h
|    \__ directional_light.h
|    \__ point_light.h
|    \__ spot_light.h
\__ camera
\__ debug_renderer.h/cpp - renderer that can display debug information (lines, spheres etc.)
\__ static_mesh_renderer.h/cpp - renderer cappable of rendering static meshes.
\__ ...
\__ deferred_renderer.h/cpp - deferred render using all the above renderers.
```

### Physics

Built upon `Core` module.

Directory structure:
```
Physics
\__
TODO
```

### Scene

Built upon `Core`, `Physics` and `Low Level Renderer` modules.

Represents a game scene. Contains and manages entities, runs scripts, and performs all high level culling optimizations,
ex. spatial subdivision, oclussion culling, lod selection.

Directory structure:
```
Scene
\__ 
\__ entity.h - 
\__ scene_renderer.h/cpp - 
\__ scene_graph.h -
\__ scene.h/cpp - 
```

### Script

Directory structure:
```
Gameplay
\__
TODO
```
