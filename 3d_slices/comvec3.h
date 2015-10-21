#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"

#ifndef comVec3_include
#define comVec3_include

class comVec3 {
  public:
    glm::vec3 content;
    comVec3 (glm::vec3);
    bool operator< (const comVec3& r) const;
    inline bool operator> (const comVec3& r) const;
    inline bool operator>=(const comVec3& r) const;
    inline bool operator<=(const comVec3& r) const;
    bool operator == (const comVec3& r) const;

};

#endif
