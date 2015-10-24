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
        //std::cout<<line.size()<<" ";

        it = points.insert(it, line.begin(), line.end());

        /*for (lit=line.begin(); lit!=line.end(); lit++){
            std::cout<<glm::to_string((*lit).content)<<std::endl;
        }
        std::cout<<std::endl;*/
    }
    //std::cout<<"cycle"<<std::endl;
    std::vector<comVec3> cycle = cycleSearch(points);
    std::vector<comVec3>::iterator cit = cycle.begin();
    /*for (cit=cycle.begin(); cit!=cycle.end(); cit++){
        std::cout<<glm::to_string((*cit).content)<<std::endl;
    }*/
    return cycle;

}

std::vector<comVec3> cycleSearch( std::vector<comVec3>& lines){
    if (lines.size()==0){
        return lines;
    }
    //2 pts in the lines vector gives an edge
    std::vector<comVec3>::iterator lit;

    std::vector<comVec3> cycles;

    std::set<comVec3> vertices;
    vertices.insert(lines.begin(), lines.end());
    
    std::vector<comVec3> v_vertices;
    std::vector<comVec3>::iterator vit = v_vertices.begin();
    vit = v_vertices.insert(vit, vertices.begin(), vertices.end());

    std::vector<int> edges (lines.size(), 0);
    std::vector<comVec3> cycle;

    for (int j=0; j<(int)v_vertices.size(); j++){
        for (int i=0; i<(int)lines.size(); i++){
            if (lines[i]==v_vertices[j]){
                edges[i]=j;
                //std::cout<<"hello";
            }
        }
    } 

    /*//cout edges
    for (int i=0; i<(int)lines.size(); i++){
        std::cout<<edges[i]<<" "<<glm::to_string(lines[i].content)<<std::endl;
    }*/
    
    std::stack<int> stacky;
    std::stack<int> history;
    std::stack<int> previouses;
    stacky.push(edges[0]);
    history.push(-1);
    history.push(edges[0]);
//DFS
    std::vector<bool> visited(vertices.size(), false);

    int previous = -1;
    int current = -1;

    while(stacky.size()!=0){
        previous = current;
        current = stacky.top();
        stacky.pop();

        /*//cout stacky and history
        std::cout<<"stacky: ";
        std::stack<int> temp;
        while (stacky.size()!=0){
            std::cout<<stacky.top();
            temp.push(stacky.top());
            stacky.pop();
        }
        while (temp.size()!=0){
            stacky.push(temp.top());
            temp.pop();
        }
        std::cout<<std::endl;
        std::cout<<"history: ";
        while (history.size()!=0){
            std::cout<<history.top();
            temp.push(history.top());
            history.pop();
        }
        while (temp.size()!=0){
            history.push(temp.top());
            temp.pop();
        }
        std::cout<<std::endl;*/

        if (visited[current]){
            std::vector<int> cyc= cyc_found(history, current, previous);
            std::vector<int>::iterator cit;
            int i = history.top();
            history.pop();
            while (i!= -1){
                visited[i] = false;
                i = history.top();
                history.pop();
                previouses.pop();
            }
            for (cit=cyc.begin(); cit!=cyc.end(); cit++){
                cycle.push_back(v_vertices[*cit]);
            }

        }
        else{
            visited[current] = true;
            if (!adjacent(edges, stacky, history, previouses, current, previous)){
                int i = history.top();
                history.pop();
                while (i!= -1){
                    visited[i] = false;
                    i = history.top();
                    history.pop();
                    previouses.pop();
                }
            }
        }
    }
        
    return cycle;
}

bool adjacent (std::vector<int>& edges, std::stack<int>& stacky, std::stack<int>& history, std::stack<int>& previouses,  int current, int previous){
    //std::cout<<"adjacent"<<std::endl;
    //std::cout<<"current: "<<current<<" previous: "<<previous<<std::endl;
    std::vector<int>::iterator ed_it;
    bool flag = false;
    for (int i=0; i<(int)edges.size(); i++){
        if (edges[i]==current){
            if (i%2==0 and edges[i+1]!=previous){
                //std::cout<<i<<": ";
                //std::cout<<edges[i-1]<<" "<<edges[i]<<" '"<<edges[i+1]<<"'"<<std::endl;
                if (flag){
                    history.push(-1);
                    previouses.push(current);
                } 
                stacky.push(edges[i+1]);
                history.push(edges[i+1]);
                previouses.push(current);
                flag = true;
            }
            else if (i%2==1 and edges[i-1]!=previous){
                //std::cout<<i<<": ";
                //std::cout<<"'"<<edges[i-1]<<"' "<<edges[i]<<" "<<edges[i+1]<<std::endl;
                if (flag){
                    history.push(-1);
                    previouses.push(current);
                } 
                stacky.push(edges[i-1]);
                history.push(edges[i-1]);
                previouses.push(current);
                flag = true;
                //std::cout<<edges[i-1]<<" ";
            }
        }
    }
    //std::cout<<std::endl;
    return flag;
}

std::vector<int> cyc_found (std::stack<int>& stacky, int target, int previous){
    std::vector<int> cyc;
    std::stack<int> temp;

    //std::cout<<"cyc_found start\n";
    /*//cout the stack taken in
    while (stacky.size()!=0){
        int cur = stacky.top();
        std::cout<<cur;
        temp.push(cur);
        stacky.pop();
    }
    //std::cout<<"here?";
    while (temp.size()!=0){
        stacky.push(temp.top());
        temp.pop();
    }
    std::cout<<std::endl;*/
    

    int current = stacky.top();
    stacky.pop();
    temp.push(current);
    //cyc.push_back(current);
    current = stacky.top();
    stacky.pop();
    temp.push(current);
    //std::cout<<target<<" "<<previous<<std::endl;
    while (current!=target){
        //std::cout<<"cur: "<<current<<std::endl;
        if (current == -1){
            current = stacky.top();
            stacky.pop();
            temp.push(current);
            
            //std::cout<<current<<std::endl;
            current = stacky.top();
            temp.push(current);
            stacky.pop();
            
        }
        else {
            cyc.push_back(current);
            current = stacky.top();
            stacky.pop();
            temp.push(current);
            
        }
    }
    //std::cout<<"done"<<std::endl;
    //std::cout<<std::endl;
    while (temp.size()!=0){
        stacky.push(temp.top());
        temp.pop();
    }
    if ((int)cyc.size()!=0){
        cyc.insert(cyc.begin(), target);
    }
    
    /*std::cout<<"cyc_beg \n";
    for (std::vector<int>::iterator cit=cyc.begin(); cit!=cyc.end(); cit++){
        std::cout<<*cit<<std::endl;
    }
    std::cout<<"cyc_end \n";*/
    return cyc;
}


