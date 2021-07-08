# OpenGL_3D_Renderer

A project that shows off a small library I wrote to render 3D objects with OpenGL. This library greatly simplifies the process of rendering a simple 3D scene using OpenGL. It does this by abstracting away the buffers and shaders that OpenGL uses.

This repo contains a project that implements the library (as well as the library itself).


Overview of the library:
- This library has a "SceneCamera3D" class. Each instance of this class represents a 3D scene which can be rendered when desired
- These scenes are made up of a "Camera" object and a list of "Renderable" objects
- The "Camera" obejct can have its position, rotation, field of view, and min and max render distances changed.
- A "Renderable" objects can represent any obejct that you want to render in your scene. It holds the vertex data for the object, as well as its position and rotation.
- You can have as many Scenes, Cameras, and Renderables as you want.


Limitations of the library:
- Textures are not currently supported
- You cannot input custom shaders into the "SceneCamera3D" class. You must use the provided vertex and fragment shaders in the library.
- Having the same instance of the "Renderable" class in multiple scenes is not recommended


External Libraries I used but did not write:
- GLEW
- GLFW
- stb_image
