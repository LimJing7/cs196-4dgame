#include "comvec3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"



comVec3::comVec3(glm::vec3 value){
    content=value;
}

bool comVec3::operator< (const comVec3& r) const{
    return std::tie(content.x, content.y, content.z)
         < std::tie(r.content.x, r.content.y, r.content.z);}
bool comVec3::operator> (const comVec3& r) const{ return r<*this; }
bool comVec3::operator>=(const comVec3& r) const{ return !(*this<r); }
bool comVec3::operator<=(const comVec3& r) const{ return !(*this>r); }
bool comVec3::operator==(const comVec3& r) const{ return content==r.content; }
