// LightInstance.cpp

#include "LightInstance.h"
#include "LibMath/Vector/Vector3.h"
#include "LibMath/Vector/Vector4.h"
#include "LibMath/Angle/Radian.h"
#include "LibMath/Angle/Degree.h"
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <LibMath/Trigonometry.h>
#include <LibMath/Matrix/Matrix4.h>

// -----------------------------------------------------------------------------
// LightInstance base
// -----------------------------------------------------------------------------

LightInstance::LightInstance(Light* lightResource)
    : m_light(lightResource)
    , m_transform(LibMath::Matrix4::identity())
{}

void LightInstance::setTransform(const LibMath::Matrix4& t) {
    m_transform = t;
}

const LibMath::Matrix4& LightInstance::getTransform() const {
    return m_transform;
}

Light* LightInstance::getLight() const {
    return m_light;
}

// -----------------------------------------------------------------------------
// DirectionalLightInstance
// -----------------------------------------------------------------------------

DirectionalLightInstance::DirectionalLightInstance(Light* lightResource)
    : LightInstance(lightResource)
{}

    std::string LightInstance::stripInvisible(const std::string& in) {
    std::string out;
    out.reserve(in.size());
    for (unsigned char c : in) {
        // keep tabs, spaces, printables, and CR/LF
        if (c == '\t' || c == '\n' || c == '\r' ||
            (c >= 0x20 && c <= 0x7E)) {
            out.push_back(c);
        }
    }
    return out;
}


bool LightInstance::LoadInstances(
    const std::string& filename,
    ResourceManager& manager,
    std::vector<DirectionalLightInstance*>& outDir,
    std::vector<PointLightInstance*>& outPoint,
    std::vector<SpotLightInstance*>& outSpot
) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "LightInstance::LoadInstances: cannot open " << filename << "\n";
        return false;
    }

    auto cleanup = [&]() {
        for (auto* l : outDir)   delete l;
        for (auto* l : outPoint) delete l;
        for (auto* l : outSpot)  delete l;
        outDir.clear();
        outPoint.clear();
        outSpot.clear();
    };

    std::string line;
    int lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;

        line = stripInvisible(line);
        // Trim leading whitespace
        size_t i = 0;
        while (i < line.size() && isspace((unsigned char)line[i])) ++i;
        if (i >= line.size() || line[i] == '#') continue;

        std::cout << "line being parsed " << lineNo << std::endl;
        std::istringstream iss(line);
        std::string type;
        float px, py, pz, dx, dy, dz;
        float ar, ag, ab, aa, dr, dg, db, da, sr, sg, sb, sa;
        float cn = 1.0f, cl = 0.0f, cq = 0.0f;        // attenuation defaults
        float inner = 12.5f, outer = 17.5f;        // spot defaults in degrees

        if (!(iss >> type
            >> px >> py >> pz
            >> dx >> dy >> dz
            >> ar >> ag >> ab >> aa
            >> dr >> dg >> db >> da
            >> sr >> sg >> sb >> sa)) {
            std::cerr << "Line " << lineNo << " parse error in " << filename << "\n";
            cleanup(); return false;
        }

        std::cout << type << " " << px << " " << py << " " << pz << " " << dx << " " << dy << " " << dz << " " << ar << " " << ag << " " << ab << " " << aa << " " << dr <<" " << dg  <<" " << db << " " << da << " " << sr << " " << sg << " "  << sb << " " << sa << " " << std::endl;

        if (type != "directional") {
            if (!(iss >> cn >> cl >> cq)) {
                // nothing to read here, clear the failbit so further >> still works
                iss.clear();
            }
        }

        // Optional spot cutoffs
        if (type == "spot") {
            if (!(iss >> inner >> outer)) {
                // likewise clear if those tokens weren’t there
                iss.clear();
            }
        }

        // Create or retrieve Light resource
        // key by lineNo so each is unique
        std::string key = type + std::to_string(lineNo);
        Light* L = manager.create<Light>(key);
        L->setAmbient({ ar,ag,ab,aa });
        L->setDiffuse({ dr,dg,db,da });
        L->setSpecular({ sr,sg,sb,sa });
        if (type != "directional") {
            L->setAttenuation(cn, cl, cq);
        }
        if (type == "spot") {
            
            L->setSpotCutoff(LibMath::Radian(LibMath::Degree(inner).radian()),
                LibMath::Radian(LibMath::Degree(outer).radian()));
        }

        // Build transform: translate to (px,py,pz)
        LibMath::Matrix4 T = LibMath::Matrix4::createTranslation(LibMath::Vector3(px, py, pz));

        if (type == "directional") {
            // For directional: set orientation so -Z maps to (dx,dy,dz)
            LibMath::Vector3 dir = LibMath::Vector3(dx, dy, dz);
            dir.normalize();
            // Build a lookAt from origin toward dir:
            LibMath::Matrix4 R = LibMath::Matrix4::lookAt(
                LibMath::Vector3::zero(),
                dir,
                LibMath::Vector3::up()
            );
            auto* inst = new DirectionalLightInstance(L);
            inst->setTransform(R);
            outDir.push_back(inst);
            std::cout << "directiona light count " << outDir.size();
        }
        else if (type == "point") {
            auto* inst = new PointLightInstance(L);
            inst->setTransform(T);
            outPoint.push_back(inst);

            std::cout << "pointlight count " << outPoint.size();
        }
        else if (type == "spot") {
            // Normalize the spot direction
            LibMath::Vector3 spotDir{ dx, dy, dz };
            spotDir.normalize();

            // Build a rotation matrix that points local –Z at spotDir
            // (we don’t care about translation here yet)

            LibMath::Matrix4 R;

            if ((std::fabs(spotDir.dot(LibMath::Vector3(0, 1, 0))) < 0.99f))
            {
                    R = LibMath::Matrix4::lookAt(
                    LibMath::Vector3::zero(),   // eye at origin
                    spotDir,           // center = direction
                    LibMath::Vector3::up()      // world-up
                );

                    float dot = std::fabs(spotDir.dot(LibMath::Vector3(0, 1, 0)));
                    std::cout << "not colinear " << dot <<std::endl;

    
            }
            else if(spotDir.dot(LibMath::Vector3(0, 1, 0)) < 0)
            {
                 R = LibMath::Matrix4::createRotationX(LibMath::Degree( 90.0f));
            }
            else
            {
                R = LibMath::Matrix4::createRotationX(LibMath::Degree(-90.0f));
            }




            // Now build the translation to px,py,pz
            LibMath::Matrix4 T = LibMath::Matrix4::createTranslation(LibMath::Vector3{ px, py, pz });

            
            // World transform = T * R
            LibMath::Matrix4 M = T * R;

            std::cout << dx << dy << dz << std::endl;
            M.Print();
            // Create the instance and assign the full transform
            auto* inst = new SpotLightInstance(L);
            inst->setTransform(M);
            outSpot.push_back(inst);

            std::cout << "spotlight count " << outSpot.size();
        }
        else {
            std::cerr << "Unknown light type '" << type << "' at line " << lineNo << "\n";
            cleanup();
            return false;
        }
    }

    return true;
}


void DirectionalLightInstance::uploadUniforms(Shader& shader, int idx) const {
    GLuint pid = shader.getID();
    std::string base = "dirLights[" + std::to_string(idx) + "]";

    // world‐space direction = transform * (0,0,-1,0)
    LibMath::Vector4 d4 = m_transform * LibMath::Vector4(0, 0, -1, 0);
    LibMath::Vector3 dir{ d4.m_x, d4.m_y, d4.m_z };
    dir.normalize();

    GLint loc = glGetUniformLocation(pid, (base + ".direction").c_str());
    glUniform3fv(loc, 1, &dir.m_x);

    // ambient
    const LibMath::Vector4& amb = m_light -> getAmbient();
    loc = glGetUniformLocation(pid, (base + ".ambient").c_str());
    glUniform4f(loc, amb.m_x, amb.m_y, amb.m_z, amb.m_w);

    // diffuse
    const LibMath::Vector4& dif = m_light -> getDiffuse();
    loc = glGetUniformLocation(pid, (base + ".diffuse").c_str());
    glUniform4f(loc, dif.m_x, dif.m_y, dif.m_z, dif.m_w);

    // specular
    const LibMath::Vector4& spec = m_light -> getSpecular();
    loc = glGetUniformLocation(pid, (base + ".specular").c_str());
    glUniform4f(loc, spec.m_x, spec.m_y, spec.m_z, spec.m_w);
}

// -----------------------------------------------------------------------------
// PointLightInstance
// -----------------------------------------------------------------------------

PointLightInstance::PointLightInstance(Light* lightResource)
    : LightInstance(lightResource)
{}

void PointLightInstance::uploadUniforms(Shader& shader, int idx) const {
    GLuint pid = shader.getID();
    std::string base = "pointLights[" + std::to_string(idx) + "]";

    // world‐space m_position = transform * (0,0,0,1)
    LibMath::Vector4 p4 = m_transform * LibMath::Vector4(0, 0, 0, 1);
    LibMath::Vector3 pos{ p4.m_x, p4.m_y, p4.m_z };

    GLint loc = glGetUniformLocation(pid, (base + ".position").c_str());
    glUniform3fv(loc, 1, &pos.m_x);

    // ambient
    const LibMath::Vector4& amb = m_light -> getAmbient();
    loc = glGetUniformLocation(pid, (base + ".ambient").c_str());
    glUniform4f(loc, amb.m_x, amb.m_y, amb.m_z, amb.m_w);

    // diffuse
    const LibMath::Vector4& dif = m_light -> getDiffuse();
    loc = glGetUniformLocation(pid, (base + ".diffuse").c_str());
    glUniform4f(loc, dif.m_x, dif.m_y, dif.m_z, dif.m_w);

    // specular
    const LibMath::Vector4& spec = m_light -> getSpecular();
    loc = glGetUniformLocation(pid, (base + ".specular").c_str());
    glUniform4f(loc, spec.m_x, spec.m_y, spec.m_z, spec.m_w);

    // attenuation
    float c, l, q;
    m_light->getAttenuation(c, l, q);
    loc = glGetUniformLocation(pid, (base + ".constant").c_str());
    glUniform1f(loc, c);
    loc = glGetUniformLocation(pid, (base + ".linear").c_str());
    glUniform1f(loc, l);
    loc = glGetUniformLocation(pid, (base + ".quadratic").c_str());
    glUniform1f(loc, q);
}

// -----------------------------------------------------------------------------
// SpotLightInstance
// -----------------------------------------------------------------------------

SpotLightInstance::SpotLightInstance(Light* lightResource)
    : LightInstance(lightResource)
{}

void SpotLightInstance::uploadUniforms(Shader& shader, int idx) const {
    GLuint pid = shader.getID();
    std::string base = "spotLights[" + std::to_string(idx) + "]";

    // m_position
    LibMath::Vector4 p4 = m_transform * LibMath::Vector4(0, 0, 0, 1);
    LibMath::Vector3 pos{ p4.m_x, p4.m_y, p4.m_z };
    GLint loc = glGetUniformLocation(pid, (base + ".position").c_str());
    glUniform3fv(loc, 1, &pos.m_x);

    // direction
    LibMath::Vector4 d4 = m_transform * LibMath::Vector4(0, 0, -1, 0);
    LibMath::Vector3 dir{ d4.m_x, d4.m_y, d4.m_z };
    dir.normalize();
    loc = glGetUniformLocation(pid, (base + ".direction").c_str());
    glUniform3fv(loc, 1, &dir.m_x);

    // ambient
    const LibMath::Vector4& amb = m_light -> getAmbient();
    loc = glGetUniformLocation(pid, (base + ".ambient").c_str());
    glUniform4f(loc, amb.m_x, amb.m_y, amb.m_z, amb.m_w);

    // diffuse
    const LibMath::Vector4& dif = m_light -> getDiffuse();
    loc = glGetUniformLocation(pid, (base + ".diffuse").c_str());
    glUniform4f(loc, dif.m_x, dif.m_y, dif.m_z, dif.m_w);

    // specular
    const LibMath::Vector4& spec = m_light -> getSpecular();
    loc = glGetUniformLocation(pid, (base + ".specular").c_str());
    glUniform4f(loc, spec.m_x, spec.m_y, spec.m_z, spec.m_w);

    // attenuation
    float c, l, q;
    m_light->getAttenuation(c, l, q);
    loc = glGetUniformLocation(pid, (base + ".constant").c_str());
    glUniform1f(loc, c);
    loc = glGetUniformLocation(pid, (base + ".linear").c_str());
    glUniform1f(loc, l);
    loc = glGetUniformLocation(pid, (base + ".quadratic").c_str());
    glUniform1f(loc, q);

    // spot cutoffs
    LibMath::Radian inner, outer;
    m_light->getSpotCutoff(inner, outer);
    loc = glGetUniformLocation(pid, (base + ".cutOff").c_str());
    glUniform1f(loc, inner.raw());
    loc = glGetUniformLocation(pid, (base + ".outerCutOff").c_str());
    glUniform1f(loc, outer.raw());
}