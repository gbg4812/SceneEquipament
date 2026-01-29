
#include <sys/types.h>

#include <fstream>
#include <iostream>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>

#include "../Model.hpp"
#include "../Scene.hpp"
#include "../SceneTree.hpp"

namespace gbg {

inline void parseVertexPos(const std::string& line, Mesh& mesh) {
    char c;
    float x, y, z;
    std::stringstream ss(line);
    ss >> c >> x >> y >> z;
    mesh.addVertex();
    auto& attr = mesh.getAttribute<AttributeTypes::VEC3_ATTR>(0);
    attr.back() = glm::vec3(x, y, z);
}

inline void parseFace(const std::string& line, Mesh& mesh) {
    std::regex posnbs(R"(\s+(\d+)/)");

    face_t face;

    for (std::sregex_iterator it(line.begin(), line.end(), posnbs);
         it != std::sregex_iterator{}; ++it) {
        std::cout << (*it)[1] << std::endl;
        face.push_back(std::stoul((*it)[1]) - 1);
    }

    mesh.createFace(face);
}

inline bool objLoader(std::string path, Scene& scene, SceneTree* parent) {
    std::map<std::string, void (*)(const std::string&, Mesh&)> dispatch;
    dispatch["v"] = parseVertexPos;
    dispatch["f"] = parseFace;

    std::ifstream fs(path);
    if (fs.fail()) return false;

    auto& md_mg = scene.getModelManager();
    auto& ms_mg = scene.getMeshManager();

    ModelHandle mdh;
    MeshHandle msh;

    std::string line;
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
            Mesh& msh_i = ms_mg.get(msh);
            msh_i.createAttribute<AttributeTypes::VEC3_ATTR>(0);  // position

        } else if (!mdh.empty()) {
            Mesh& msh_i = ms_mg.get(msh);
            auto it = dispatch.find(type);
            if (it != dispatch.end()) {
                (*it).second(line, msh_i);
            }
        }
    }
    return true;
}

}  // namespace gbg
