#include <vector>
#include <set>
#include <stack>
#include "comvec3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#ifndef LIN_INTERPOLATION_INCLUDE
#define LIN_INTERPOLATION_INCLUDE

std::vector<comVec3> lin_interpolation_l2p(glm::vec4 start_ver, glm::vec4 end_ver, float w);
std::vector<comVec3> lin_interpolation_t2l(glm::vec4 a, glm::vec4 b, glm::vec4 c, float w);

//size is the number of triangles for each 3d object
std::vector<comVec3> lin_interpolation_c2t(GLuint indices[], GLfloat vertices[], int start, int size, float w);

std::vector<comVec3> cycleSearch( std::vector<comVec3>& lines);
bool adjacent (std::vector<int>& edges, std::stack<int>& stacky, std::stack<int>& history, std::stack<int>& previouses, int current, int previous);
std::vector<int> cyc_found (std::stack<int>& stacky, int target, int previous);

#endif
