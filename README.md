# Pandora
Birds-eye overview of my hobby game engine. Intended for personal use.

### Table of contents
 - [Root](Root)
   - [Core](Core)
      - [Async](Async)
      - [Data](Data)
      - [Encoding](Encoding)
      - [Input](Input)
      - [IO](IO)
      - [Logging](Logging)
      - [Math](Math)
      - [Resources](Resources)
      - [Time](Time)
      - [Window](Window)
    - [Graphics](Graphics)
      - [Sprite](Sprite)
      - [Model](Model)
      - [Text](Text)
   - [Audio](Audio)
   - [Scene](Scene)
   - [Lua](Lua)
   - [Libs](Libs)
   
# Root
The root Pandora folder contains a few helper files.

There is a header for every category, e.g. `Core.h`, `Graphics.h`, etc...
This header will include every header in that category.

There is also `App.h` and `Entry.h` which act as an optional application starting point.

## Core
The root Core folder contains a few files.
Namely `Types.h` defines all the exact-width types like `u32`, `i16`, `f64` and `Assert.h` defines the `PD_ASSERT(...)`, `PD_ASSERT_D(...)` macros.

### Async
Async contains atomics, mutices, scope locks, threads and signals.

### Data
Data contains the allocators, arrays, strings, dictionaries, references, and memory operations.

### Encoding
Encoding contains a Base64 encoder/decoder, JSON encoder/decoder, compression, encryption and a box loader.
Box is the custom archive format used by Pandora. It compresses all the resources and includes metadata on how it should be loaded.

Encoding also contains `BoxBuilder`, which is used to construct box files. This is disabled by default because it depends on every other
catagory which breaks the "Core should be self-sufficient" mantra.

### Input
Input contains key definitions and a basic input manager which allows querying whether keys are pressed/held.

### IO
IO contains streams, file streams, memory streams, console output, file operations, folder operations, path operations and app storage.

### Logging
Logging contains the logger that Pandora uses.

### Math
Math contains vectors, int vectors, matrices, perlin noise, RNG, lerping and basic math operations.

### Resources
Resources contains the resource catalog and the `BinaryResource` resource.

### Time
Time contains basic time operations, stopwatches and duration operations.

### Window
Window contains a window class.

## Graphics
Graphics contains cameras, constant buffers, layouts for buffers, frame buffers, renderers, line renderers,
shaders, textures, materials, texture packing and a uniform video API.

The implementations can be found in `Graphics/Backend/*`.

### Model
Model contains the mesh resource, models and the model renderer.

### Sprite
Sprite contains sprites, sprite atlasses and the sprite renderer.

### Text
Text contains the font resource and text renderer.

## Audio
Audio contains the uniform audio API, Wave encoder, audio buffers and sound sources.

The implementations can be found in `Audio/Backend/*`.

## Scene
Scene contains very rudimentary scene management tools.
Currently you can switch what code is executed by changing scenes.

## Lua
Lua contains a work-in-progress Lua interface. There are no engine bindings (yet).

## Libs
Libs contains small dependencies that are included into the engine itself.
It currently contains the following dependencies:
  - dxerr
    - Used to get error messages from `HRESULT`s.
  - glad
    - OpenGL loader.
  - Dear ImGui
    - Debug GUI.
  - MeowHash
    - Very fast hashing.
  - stb
    - Image reading/writing, perlin noise, rect packing.
  - tiny-AES
    - AES encryption.
  - utf8
    - Basic UTF-8 utilities.

# TODO
Things that are still to-do in the engine:
  - Proper lighting model
  - Better Lua bindings
  - Optimized model renderer
  - Optimize various other things
  - Proper scene graph
  - GUI editor
  - More probably
