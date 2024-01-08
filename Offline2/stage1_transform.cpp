// Line 1: eyeX eyeY eyeZ
// Line 2: lookX lookY lookZ
// Line 3: upX upY upZ
// Line 4: fovY aspectRatio near far
// this is the file format,line 1,2,3 for glulookat, line 4 for gluPerspective

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <stack>
using namespace std;

class Point{
public:
    float x,y,z;
    Point(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

class Matrix{
public:
    float m[4][4];
    Matrix(){
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                    m[i][j] = 0.0;
            }
        }
    }
    Matrix(float m[4][4]){
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                    this->m[i][j] = m[i][j];
            }
        }
    }
    Matrix operator*(Matrix m2){
        Matrix m3;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                    for(int k = 0; k < 4; k++){
                        m3.m[i][j] += this->m[i][k] * m2.m[k][j];
                    }
            }
        }
        return m3;
    }
    Matrix operator/(float num){
        Matrix m3;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                    m3.m[i][j] = this->m[i][j]/num;
            }
        }
        return m3;
    }
    // [4][4]*[4][1]
    
    void print(){
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                    cout << m[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

class single_matrix{
    public:
    float m[4][1];

    single_matrix(){
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 1; j++){
                    m[i][j] = 0.0;
            }
        }
    }
    single_matrix(float m[4][1]){
        for(int i = 0; i < 4; i++){
            this->m[i][0] = m[i][0];
        }
    }
    // multiplying a 4*4 matrix from matrix class with a 4*1 matrix from single_matrix class
    single_matrix operator*(Matrix m2){
        single_matrix m3;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 1; j++){
                    for(int k = 0; k < 4; k++){
                        m3.m[i][j] += this->m[i][k] * m2.m[k][j];
                    }
            }
        }
        return m3;
    }
    single_matrix operator/(float num){
        single_matrix m3;
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 1; j++){
                    m3.m[i][j] = this->m[i][j]/num;
            }
        }
        return m3;
    }
    void print(){
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 1; j++){
                    cout << m[i][j] << " ";
            }
            
        }
        cout << endl;
    }
};
int main(){
    // reading file scene.txt for camera position and perspective
    ifstream sceneFile("scene.txt");
    string line;
    vector<float> scene;
    vector<string> lines;
    while(getline(sceneFile,line)){
        stringstream ss(line);
        float num;
        while(ss >> num){
            scene.push_back(num);
            // lines.push_back(line);
        }
        lines.push_back(line);
    }
    sceneFile.close();
    stack<Matrix> S;
    Matrix I;
    float m[4][4] = {{1,0,0,0},
                     {0,1,0,0},
                     {0,0,1,0},
                     {0,0,0,1}};
    I = Matrix(m);
    // printf("I:\n");
    // I.print();
    // S.push(I);
    int i = 0;
    int j=12;
    Matrix currentMatrix=I;
    while (true)
    {
        if(lines.size() == 0){
            break;
        }
        else if(lines[i]=="triangle"){
            // cout<<"current matrix"<<endl;
            // currentMatrix.print();
            float m[4][1] = {{scene[j+1]},
                             {scene[j+2]},
                             {scene[j+3]},
                             {1}};
            single_matrix p1 = single_matrix(m);
            i++;
            j=j+3;
            float m2[4][1] = {{scene[j+4]},
                             {scene[j+5]},
                             {scene[j+6]},
                             {1}};
            single_matrix p2 = single_matrix(m2);
            i++;
            j=j+3;
            float m3[4][1] = {{scene[j+7]},
                             {scene[j+8]},
                             {scene[j+9]},
                             {1}};
            single_matrix p3 = single_matrix(m3);
            i++;
            j=j+3;
            single_matrix p1_new = p1*currentMatrix;
            p1_new = p1_new/p1_new.m[3][0];
            single_matrix p2_new = p2*currentMatrix;
            p2_new = p2_new/p2_new.m[3][0];
            single_matrix p3_new = p3*currentMatrix;
            p3_new = p3_new/p3_new.m[3][0];
            // cout<<"this was supposed to be 1st iteration"<<endl;
            p1_new.print();
            // cout<<"this was supposed to be 2nd iteration"<<endl;
            p2_new.print();
            // cout<<"this was supposed to be 3rd iteration"<<endl;
            p3_new.print();
            cout<<"a triangle"<<endl;
            
        }
        else if(lines[i]=="translate"){
            float x = scene[j+1];
            float y = scene[j+2];
            float z = scene[j+3];
            float m[4][4] = {{1,0,0,x},
                             {0,1,0,y},
                             {0,0,1,z},
                             {0,0,0,1}};
            Matrix T = Matrix(m);
            Matrix M = currentMatrix;
            currentMatrix = M*T;
            // currentMatrix.print();
            // i = i+4;
            j=j+3;
        }
        else if(lines[i]=="scale"){
            float x = scene[j+1];
            float y = scene[j+2];
            float z = scene[j+3];
            float m[4][4] = {{x,0,0,0},
                             {0,y,0,0},
                             {0,0,z,0},
                             {0,0,0,1}};
            Matrix T = Matrix(m);
            Matrix M = currentMatrix;
            currentMatrix = M*T;
            // currentMatrix.print();
            // i = i+4;
            j=j+3;
        }
        else if(lines[i]=="rotate"){
            float x = scene[j+1];
            float y = scene[j+2];
            float z = scene[j+3];
            float angle = scene[j+4];
            float m[4][4] = {{cos(angle)+(1-cos(angle))*x*x, (1-cos(angle))*x*y-sin(angle)*z, (1-cos(angle))*x*z+sin(angle)*y, 0},
                             {(1-cos(angle))*y*x+sin(angle)*z, cos(angle)+(1-cos(angle))*y*y, (1-cos(angle))*y*z-sin(angle)*x, 0},
                             {(1-cos(angle))*z*x-sin(angle)*y, (1-cos(angle))*z*y+sin(angle)*x, cos(angle)+(1-cos(angle))*z*z, 0},
                             {0,0,0,1}};
            Matrix T = Matrix(m);
            Matrix M = currentMatrix;
            currentMatrix = M*T;
            // currentMatrix.print();
            // i = i+5;
            j=j+4;
        }
        else if(lines[i]=="push"){
            // Matrix M = S.top();
            S.push(currentMatrix);
            // i = i+1;
        }
        else if(lines[i]=="pop"){
            S.pop();
            // i = i+1;
        }
        else if(lines[i]=="end"){
            break;
        }
        i++;
    }
}