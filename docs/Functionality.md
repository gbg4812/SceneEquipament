# Scene Representation Package

## Functionalitay

- Represent poligonal meshes
- Arbitrary number of attributes of a set of types
- Represent Shaders that use the attributes and have uniforms as inputs.
- Represent Materials as concrete assignation of values to the uniforms of the
  shader. They can be values or references as there are resources that may be
  shared like textures.

- Objects as relations between materials and meshes
- Transforms as nodes of a transformation tree. They can have an arbitrary
  number of other objects and other transforms associated. The transformations
  are applied from the root to the children.

## Dynamic dispatch technique

Options:

1. Interfaces-Polymorphism
2. Variadic types

   They allocate the size needed to hold their biggest element type. In my
   usecase it isn't a problem as i will be storing std::vectors with diferent
   type.

## Meshes

A mesh is a collection of n attributes.

I want attributes to be indexed continuous arrays... how to do it...

It also has faces represented as a vector of linked-lists to faces

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

- add attributes
- add vertices
- delete attributes
- delete vertices
- add vertice to face

## Resources

- Meshes, Materials, Shaders, Textures

Són dades que es comparteixen entre diversos objectes, materials... S'han de
alocar i desalocar controladament.

Utilitzarem Handles i Pools per allocar-los i referenciar-los.
