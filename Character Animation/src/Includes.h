#include <iostream>
#include <clFFT.h>
#include <fstream>
#include <sstream>
#include <random>
#include <cmath>
#include <map>
#include <chrono>
#include <string>
#include <stdio.h>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <stdlib.h>
#include <queue>
#include <unistd.h>
#include <algorithm>
#include <OpenCL/opencl.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnullability-completeness"
#include <OpenGL/OpenGL.h>
#pragma clang diagnostic pop
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../external/Include/irrKlang.h"

using namespace std ;
using namespace irrklang;






struct Bone {
    Bone * parent ;
    std::string name ;
//    std::vector<bool> flag ;
    std::vector< std::vector<bool> > active ;
    std::vector<Bone*> children ;
    std::vector<glm::mat4> animations ;
    std::vector< std::vector<unsigned int> > affected_vertices ;
    std::vector< std::vector<glm::mat4> > precomputed_animations ;
    glm::mat4 offset, inverse_offset, transformation, local_transformation ;
} ;


struct Mesh {
    glm::vec3 albedo ;
    float metallic, roughness ;
    std::vector<float> vertices ;
    std::vector<unsigned int> indices ;
    std::vector<float> original_vertices ;
    std::vector<std::vector<Bone*>> bones ;
    std::vector<std::vector<float>> weights ;
} ;


struct DecomposedTransformation{
    glm::quat rotate ;
    glm::vec3 translate, scale ;
} ;
