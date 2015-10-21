#include "lin_interpolation.h"
#include "comvec3.h"
#include <vector>
#include <set>
#include <stack>
#include <iostream>

std::vector<comVec3> lin_interpolation_l2p(glm::vec4 start_ver, glm::vec4 end_ver, float w){
    std::vector<comVec3> points;
    std::vector<comVec3>::iterator it;
    
    if ((w>start_ver.w && w<end_ver.w) || (w<start_ver.w && w>end_ver.w)){
        float rel = (w-start_ver.w)/(end_ver.w-start_ver.w);
        points.insert(it, comVec3(glm::vec3( (end_ver.x - start_ver.x)*rel+start_ver.x, 
                                              (end_ver.y - start_ver.y)*rel+start_ver.y, 
                                              (end_ver.z - start_ver.z)*rel+start_ver.z) ));
    }
    if (start_ver.w == w) points.insert(it, comVec3(glm::vec3(start_ver)));
    if (end_ver.w == w) points.insert(it, comVec3(glm::vec3(end_ver)));
    return points;
}

std::vector<comVec3> lin_interpolation_t2l(glm::vec4 a, glm::vec4 b, glm::vec4 c, float w){
    
    std::vector<comVec3> points;
    std::vector<comVec3>::iterator it;
    it=points.begin();
    
    //2 points returned -> a line is drawn
    std::vector<comVec3> pt1 = lin_interpolation_l2p(a, b, w);
    //std::cout<<pt1.size()<<" ";
    it = points.insert(it, pt1.begin(), pt1.end());
    //std::cout<<points.size()<<std::endl;
    std::vector<comVec3> pt2 = lin_interpolation_l2p(a, c, w);
    //std::cout<<pt2.size()<<" ";
    it = points.insert(it, pt2.begin(), pt2.end());
    std::vector<comVec3> pt3 = lin_interpolation_l2p(b, c, w);
    //std::cout<<pt3.size()<<std::endl;
    it = points.insert(it, pt3.begin(), pt3.end());

    return points;
}

std::vector<comVec3> lin_interpolation_c2t(GLuint indices[], GLfloat vertices[], int start, int size, float w){
    
    std::vector<comVec3> points;
    std::vector<comVec3>::iterator it=points.begin();
    
    for (int i=start; i<start+3*size; i+=3){
        glm::vec4 a = glm::vec4(vertices[indices[i]*4], vertices[indices[i]*4+1], vertices[indices[i]*4+2], vertices[indices[i]*4+3]); 
        glm::vec4 b = glm::vec4(vertices[indices[i+1]*4], vertices[indices[i+1]*4+1], vertices[indices[i+1]*4+2], vertices[indices[i+1]*4+3]); 
        glm::vec4 c = glm::vec4(vertices[indices[i+2]*4], vertices[indices[i+2]*4+1], vertices[indices[i+2]*4+2], vertices[indices[i+2]*4+3]); 
        /*std::cout<<indices[i]<<std::endl;
        std::cout<<glm::to_string(a)<<std::endl;
        std::cout<<glm::to_string(b)<<std::endl;
        std::cout<<glm::to_string(c)<<std::endl;*/

        std::vector<comVec3> line = lin_interpolation_t2l(a, b, c, w);
        std::vector<comVec3>::iterator lit;
        std::cout<<line.size()<<" ";

        /*for (lit=line.begin(); lit!=line.end(); lit++){
            std::cout<<glm::to_string((*lit).content)<<std::endl;
        }
        std::cout<<std::endl;*/
    }
    //std::cout<<std::endl;
    //points.insert(it, vertices_to_draw.begin(), vertices_to_draw.end());
    return points;

}

std::vector<comVec3> cycleSearch( std::vector<comVec3>& lines){
    if (lines.size()==0){
        return lines;
    }
    //2 pts in the lines vector gives an edge
    int leng = lines.size();
    std::vector<comVec3>::iterator lit;

    std::set<comVec3> vertices;
    vertices.insert(lines.begin(), lines.end());
    
    std::vector<comVec3> v_vertices;
    std::vector<comVec3>::iterator vit;
    v_vertices.insert(v_vertices.begin(), vertices.begin(), vertices.end());

    std::vector<int> edges (lines.size(), 0);

    for (int j=0; j<(int)v_vertices.size(); j++){
        for (int i=0; i<(int)lines.size(); i++){
            if (lines[i]==*vit){
                edges[i]=j;
                //std::cout<<"hello";
            }
        }
    } 
    
    std::stack<int> stacky;
    std::stack<int> history;
    stacky.push(edges[0]);
//DFS
    std::vector<bool> visited(vertices.size(), false);

    int previous = -1;
    int current = -1;

    while(stacky.size()!=0){
        int previous = current;
        int current = stacky.pop();
        //stacky.push(current);
        //history.push(current);
        if (visited[current]){
            std::vector<int> cyc= cyc_found();
        }
        else{
            visited[current] = true;
            if (!adjacent(edges, stacky, history, current, previous)){
                int i = history.pop();
                while (i!= -1){
                    visited[i] = false;
                    i = history.pop();
                }
            }
        }
        
    }




    return lines;
}

bool adjacent (std::vector<int>& edges, std::stack<int>& stacky, std::stack<int>& history, int current, int previous){
    std::vector<int>::iterator ed_it;
    bool branch = false;
    for (int i=0; i<edges.size(); i++){
        if (edges[i]==current){
            if (i%2==0 and edges[i+1]!=previous){
                if (flag){
                    history.push(-1);
                } 
                stacky.push(edges[i+1]);
                history.push(edges[i+1]);
                flag = true;
            }
            else if (edges[i-1]!=previous){
                if (flag){
                    history.push(-1);
                } 
                stacky.push(edges[i-1]);
                history.push(edges[i-1]);
                flag = true;
            }
        }
    }
    return flag;
}

std::vector<int> cyc_found (std::stack<int>& stacky, int target){
    std::vector<int> cyc;
    std::stack<int> temp;
    
    int current = stacky.pop();
    temp.push(current);
    while (current!=target){
        cyc.push_back(current);
        current = stacky.pop();
        temp.push(current);
    }
    while (temp.size()!=0){
        stacky.push(temp.pop());
    }
    return cyc;
}


