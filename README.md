# SceneEquipament

SceneEquipament is a 3D scene representation library. It has types to represent
meshes, shaders, materials, models... One of the features I like is that it has
support for _any number of attributes_ of different types to descrive the
vertices.

## Current Features

- Representation of meshes with any number of point attributes.
- Representation of shaders with dynamic number of parameters and input
  attributes.
- Representation of Models and Scene Trees.
- An incomplete .obj loader.

## Technology note

It relays on `std::variant` to implement the mesh attributes system and the
material parameter system. Also, [glm](https://github.com/g-truc/glm.git) is
used as math library.

## Code example

```cpp
    gbg::Mesh mesh;
    using AT = gbg::AttributeTypes;
    mesh.createAttribute<AT::FLOAT_ATTR>(1);
    mesh.createAttribute<AT::VEC2_ATTR>(2);
    mesh.createAttribute<AT::VEC3_ATTR>(3);

    for (int i = 0; i < 100; i++) {
        mesh.addVertex();
    }

    {
        auto& vf = mesh.getAttribute<AT::FLOAT_ATTR>(1);
        auto& v2 = mesh.getAttribute<AT::VEC2_ATTR>(2);
        auto& v3 = mesh.getAttribute<AT::VEC3_ATTR>(3);

        vf[99] = 3.0;

        v2[0] = glm::vec2(1, 2);
    }

    {
        auto& vf = mesh.getAttribute<AT::FLOAT_ATTR>(1);
        auto& v2 = mesh.getAttribute<AT::VEC2_ATTR>(2);
        auto& v3 = mesh.getAttribute<AT::VEC3_ATTR>(3);

    }

```

Example of creation of a mesh with 3 attributes.
