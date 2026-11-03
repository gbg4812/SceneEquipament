#pragma once
/*
 * 1. Llegir tota la info en 3 vectors
 * 2. Per cada vertex d'una cara buscar si ja existeix i sinó crear-lo.
 * 3. Alhora crear la cara.
 */

#include <sys/types.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

#include "../Mesh.hpp"
#include "../Model.hpp"
#include "../Scene.hpp"
#include "../SceneTree.hpp"
#include "Material.hpp"
#include "MaterialFunctions.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "texLoader.hpp"

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
    gbg::MaterialHandle current_mat = gbg::MaterialHandle();
    std::map<std::string, gbg::MaterialHandle> materials;
    std::map<std::string, gbg::TextureHandle> textures;
    gbg::ShaderHandle base_shader;
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
    static std::regex posnbs(R"(\s+(\d+)/(\d+)/(\d+))");

    face_t face;

    for (std::sregex_iterator it(line.begin(), line.end(), posnbs);
         it != std::sregex_iterator{}; ++it) {
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

// Optimitzar!
inline void parseLine(const std::string& line, Mesh& mesh,
                      _parser_context& context) {
    face_t face;

    char c;
    std::size_t pos_idx1, pos_idx2;
    std::stringstream ss(line);
    ss >> c >> pos_idx1 >> pos_idx2;
    --pos_idx1;
    --pos_idx2;
    size_t vert_idx1 = mesh.addVertex();
    size_t vert_idx2 = mesh.addVertex();
    auto& pos_attr = mesh.getAttribute<AttributeTypes::VEC3_ATTR>(0);
    pos_attr[vert_idx1] = context.positions[pos_idx1];
    pos_attr[vert_idx2] = context.positions[pos_idx2];
    face.push_back(vert_idx1);
    face.push_back(vert_idx2);

    mesh.createFace(face);
}

/**
 * Material parameters:
 * vec3 ambient_color;
 * vec3 diffuse_color;
 * vec3 specular_color;
 * float specular_exponent;
 * texture albedo;
 * texture bump;
 */
inline void parseMaterials(const std::filesystem::path& path,
                           _parser_context& context, gbg::Scene& scene) {
    std::ifstream f(path);

    static const std::regex tex_rg(
        "map_((kd)|(bump))(\\s*-bm\\s([\\d\\.]*))*\\s*([\\w/\\.]+)",
        std::regex::icase);

    std::smatch m;

    std::string line;
    gbg::MaterialHandle mat_h = {};
    while (std::getline(f, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "newmtl") {
            std::string name;
            ss >> name;
            auto& mat = scene.mat_mg.create(name);
            auto& sh = scene.sh_mg.get(context.base_shader);
            gbg::setParametersFromShader(sh, mat);
            context.materials[name] = mat.getHandle();
            mat_h = mat.getHandle();
        } else if (type == "Ka") {
            auto& mat = scene.mat_mg.get(mat_h);
            vec3_t val;
            ss >> val.x >> val.y >> val.z;
            mat.setParameter(0, val);
        } else if (type == "Kd") {
            auto& mat = scene.mat_mg.get(mat_h);
            vec3_t val;
            ss >> val.x >> val.y >> val.z;
            mat.setParameter(1, val);
        } else if (type == "Ks") {
            auto& mat = scene.mat_mg.get(mat_h);
            vec3_t val;
            ss >> val.x >> val.y >> val.z;
            mat.setParameter(2, val);

        } else if (type == "Ns") {
            auto& mat = scene.mat_mg.get(mat_h);
            float val;
            ss >> val;
            mat.setParameter(3, val);
        } else if (std::regex_match(line, m, tex_rg)) {
            auto& mat = scene.mat_mg.get(mat_h);
            std::string _path = m[6].str();

            auto it = context.textures.find(_path);
            if (it == context.textures.end()) {
                auto& tx = scene.tx_mg.create(_path);
                loadTexture((path.parent_path() / _path).string(), tx);
                it = context.textures.insert({_path, tx.getHandle()}).first;
            }
            if (m[2].matched) mat.setParameter(4, it->second);
            if (m[3].matched) mat.setParameter(5, it->second);
        }
    }
}

/* Loads all the models in a .obj file and hangs them from the parent
 * @return a list of handles to all the scene nodes added
 */
inline std::list<SceneTreeHandle> objLoader(
    std::filesystem::path path, Scene* scene, SceneTreeHandle parent,
    ShaderHandle base_shader, MaterialHandle default_mat = MaterialHandle()) {
    std::map<std::string,
             void (*)(const std::string&, Mesh&, _parser_context& context)>
        dispatch;
    dispatch["v"] = parseVertexPos;
    dispatch["vn"] = parseVertexNormal;
    dispatch["vt"] = parseVertexUV;
    dispatch["f"] = parseFace;
    dispatch["l"] = parseLine;

    std::ifstream fs(path);
    if (fs.fail()) return std::list<SceneTreeHandle>();

    auto& md_mg = scene->getModelManager();
    auto& ms_mg = scene->getMeshManager();
    auto& st_mg = scene->getSceneTreeManager();

    MeshHandle ms_h;

    std::list<SceneTreeHandle> added;

    std::string line;
    _parser_context context{};
    context.current_mat = default_mat;
    context.base_shader = base_shader;
    while (std::getline(fs, line)) {
        std::string type;
        std::stringstream ss(line);
        ss >> type;

        if (type == "mtllib") {
            std::string mtllib_pt;
            ss >> mtllib_pt;
            parseMaterials(path.parent_path() / mtllib_pt, context, *scene);

        } else if (type == "o" or type == "usemtl") {
            std::string name;
            ss >> name;
            if (type == "usemtl") {
                context.current_mat = context.materials[name];
            }
            if (type == "o" or not ms_h) {
                auto& md = md_mg.create(name);
                auto& child = st_mg.create(name);
                added.push_back(child.getHandle());
                child.setResource(md.getHandle());
                st_mg.prependChild(parent, child.getHandle());
                auto& ms = ms_mg.create("Mesh0");
                ms_h = ms.getHandle();

                md.setMesh(ms.getHandle());
                md.setMaterial(default_mat);
                ms.createAttribute<AttributeTypes::VEC3_ATTR>(0);  // position
                ms.createAttribute<AttributeTypes::VEC3_ATTR>(1);  // normal
                ms.createAttribute<AttributeTypes::VEC2_ATTR>(2);  // texture

                context.vertices.clear();
            }

        } else if (ms_h) {
            Mesh& ms = ms_mg.get(ms_h);
            auto it = dispatch.find(type);
            if (it != dispatch.end()) {
                (*it).second(line, ms, context);
            }
        }
    }
    return added;
}

}  // namespace gbg
