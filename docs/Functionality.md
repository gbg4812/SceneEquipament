# Scene Representation Package

## Functionalitay

- Represent poligonal meshes
- Arbitrary number of attributes of a set of types and domains (points,
  vertices, faces)
- Represent Shaders that use the attributes and have uniforms as inputs.
- Represent Materials as concrete assignation of values to the uniforms of the
  shader. They can be values or references as there are resources that may be
  shared like textures.

- Represents Objects in the world (Positioned Things) with a Kind:
  - Model (mesh + material)
  - Light
  - Camera
  - For now this three...

## Dynamic dispatch technique

Options:

1. Variadic types (...almost the same)

   They allocate the size needed to hold their biggest element type. In my
   usecase it isn't a problem as i will be storing std::vectors with diferent
   type.

2. Fat-structs...

## Meshes

A mesh is a collection of n attributes.

I want attributes to be indexed continuous arrays... how to do it...

For attributes i will use a variant of vectors.

### Attributes types

- uint (32bit)
- int (32bit)
- float (32bit)
- vec2 (2 float)
- vec3 (3 float)
- vec4 (4 float)
- mat2 (2x2 float)
- mat3 (3x3 float)
- mat4 (4x4 float)

### Operations over meshes

- add attributes of any kind, domain
- add vertices
- delete attributes
- create face

(for now i don't try to make an efficient edit mesh)

### Data structure

- attributes of any domain are contiguous arrays
- 3 variant of vectors one for each domain
- the faces are defined as two vectors:
  1. Has the same size as the vertices and holds subarrays that define the
     vertices of each face.
  2. Holds for every face the index of the vector 1. where it starts. (Inspired in blender's Mesh)
- vertices have an attribute that saves the index of the point they are connected to.

#### Operation costs

- n -> number of elements
- p -> number of points
- v -> number of vertices
- f -> number of faces

| Operation                  | Runing Asimptotic Time                 |
| -------------------------- | -------------------------------------- |
| Add a point/vertex/face    | O(1) or O(n) if reallocation is needed |
| Remove a point/vertex/face | O(n)                                   |

So it is clear that it is a good structure to save data but not to
modify it. We are not talking of any mesh editing operations.

## Resources

- Meshes, Materials, Shaders, Textures

Són dades que es comparteixen entre diversos objectes, materials... S'han de
alocar i desalocar controladament.

Utilitzarem Handles i Pools per allocar-los i referenciar-los.
