/*
 * 1. Llegir tota la info en 3 vectors
 * 2. Per cada vertex d'una cara buscar si ja existeix i sinó crear-lo.
 * 3. Alhora crear la cara.
 */

#include <sys/types.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

#include "../Model.hpp"
#include "../Scene.hpp"
#include "../SceneTree.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

namespace gbg {

struct _parser_vertex {
    glm::vec3 pos;
    glm::vec3 nrml;
    glm::vec2 tex;

    bool operator==(const _parser_vertex& other) const {
        return (pos == other.pos) and (nrml == other.nrml) and
               (tex == other.tex);
    }
};

inline std::size_t hash_combine(std::size_t h1, std::size_t h2) {
    h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
    return h1;
}

}  // namespace gbg
template <>
struct std::hash<gbg::_parser_vertex> {
    std::size_t operator()(const gbg::_parser_vertex& vert) const noexcept {
        std::size_t h1 = std::hash<glm::vec3>{}(vert.pos);
        std::size_t h2 = std::hash<glm::vec3>{}(vert.nrml);
        std::size_t h3 = std::hash<glm::vec2>{}(vert.tex);

        return gbg::hash_combine(gbg::hash_combine(h1, h2), h3);
    };
};

namespace gbg {

struct _parser_context {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::unordered_map<_parser_vertex, size_t> vertices;
};

inline void parseVertexPos(const std::string& line, Mesh& mesh,
                           _parser_context& context) {
    char c;
    float x, y, z;
    std::stringstream ss(line);
    ss >> c >> x >> y >> z;
    context.positions.push_back(glm::vec3(x, y, z));
}

inline void parseVertexNormal(const std::string& line, Mesh& mesh,
                              _parser_context& context) {
    char c;
    float x, y, z;
    std::stringstream ss(line);
    ss >> c >> c >> x >> y >> z;
    context.normals.push_back(glm::vec3(x, y, z));
}

inline void parseVertexUV(const std::string& line, Mesh& mesh,
                          _parser_context& context) {
    char c;
    float x, y;
    std::stringstream ss(line);
    ss >> c >> c >> x >> y;
    context.uvs.push_back(glm::vec2(x, y));
}

inline void parseFace(const std::string& line, Mesh& mesh,
                      _parser_context& context) {
    std::regex posnbs(R"(\s+(\d+)/(\d+)/(\d+))");

    face_t face;

    for (std::sregex_iterator it(line.begin(), line.end(), posnbs);
         it != std::sregex_iterator{}; ++it) {
        std::cout << (*it)[1] << std::endl;
        std::size_t pos_idx = std::stoi((*it)[1]) - 1;
        std::size_t tex_idx = std::stoi((*it)[2]) - 1;
        std::size_t nrml_idx = std::stoi((*it)[3]) - 1;
        _parser_vertex vert = {context.positions[pos_idx],
                               context.normals[nrml_idx], context.uvs[tex_idx]};

        auto vert_it = context.vertices.find(vert);
        if (vert_it != context.vertices.end()) {
            face.push_back(vert_it->second);
        } else {
            size_t vert_idx = mesh.addVertex();
            context.vertices.insert({vert, vert_idx});

            auto& pos_attr = mesh.getAttribute<AttributeTypes::VEC3_ATTR>(0);
            auto& nrml_attr = mesh.getAttribute<AttributeTypes::VEC3_ATTR>(1);
            auto& uv_attr = mesh.getAttribute<AttributeTypes::VEC2_ATTR>(2);
            *(--pos_attr.end()) = vert.pos;
            *(--nrml_attr.end()) = vert.nrml;
            *(--uv_attr.end()) = vert.tex;

            face.push_back(vert_idx);
        }
    }

    mesh.createFace(face);
}

inline bool objLoader(std::string path, Scene* scene, SceneTree* parent,
                      MaterialHandle default_mat) {
    std::map<std::string,
             void (*)(const std::string&, Mesh&, _parser_context& context)>
        dispatch;
    dispatch["v"] = parseVertexPos;
    dispatch["vn"] = parseVertexNormal;
    dispatch["vt"] = parseVertexUV;
    dispatch["f"] = parseFace;

    std::ifstream fs(path);
    if (fs.fail()) return false;

    auto& md_mg = scene->getModelManager();
    auto& ms_mg = scene->getMeshManager();

    ModelHandle mdh;
    MeshHandle msh;

    std::string line;
    _parser_context context{};
    while (std::getline(fs, line)) {
        std::string type;
        std::stringstream ss(line);
        ss >> type;

        if (type == "o") {
            std::string name;
            ss >> name;
            mdh = md_mg.create(name);
            parent->addChild<gbg::SceneObjectTypes::MODEL>(mdh);
            msh = ms_mg.create("Mesh0");

            md_mg.get(mdh).setMesh(msh);
            md_mg.get(mdh).setMaterial(default_mat);
            Mesh& msh_i = ms_mg.get(msh);
            msh_i.createAttribute<AttributeTypes::VEC3_ATTR>(0);  // position
            msh_i.createAttribute<AttributeTypes::VEC3_ATTR>(1);  // normal
            msh_i.createAttribute<AttributeTypes::VEC2_ATTR>(2);  // normal

            context.vertices.clear();

        } else if (!mdh.empty()) {
            Mesh& msh_i = ms_mg.get(msh);
            auto it = dispatch.find(type);
            if (it != dispatch.end()) {
                (*it).second(line, msh_i, context);
            }
        }
    }
    return true;
}

}  // namespace gbg
