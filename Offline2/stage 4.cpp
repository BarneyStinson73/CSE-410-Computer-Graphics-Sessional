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
#include <cmath>
#include <stack>
#include<iomanip>
#include "bitmap_image.hpp"
using namespace std;

#define PI 3.14159265358979323846
class Point{
public:
    float x,y,z;
    Point(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Point operator+(Point p2){
        Point p = Point(this->x+p2.x,this->y+p2.y,this->z+p2.z);
        return p;
    }
    Point operator-(Point p2){
        Point p = Point(this->x-p2.x,this->y-p2.y,this->z-p2.z);
        return p;
    }
    Point operator*(float num){
        Point p = Point(this->x*num,this->y*num,this->z*num);
        return p;
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
                    std::cout << m[i][j] << " ";
            }
            std::cout << endl;
        }
        std::cout << endl;
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
                   if(abs(m[i][j])>=0.0001) std::cout << setprecision(7)<< m[i][j] << " ";
                   else std::cout<<"0.00 ";
            }
            
        }
        std::cout << endl;
    }
};
Point normalize(float x,float y,float z){
    float length = sqrt(x*x+y*y+z*z);
    x = x/length;
    y = y/length;
    z = z/length;
    Point p = Point(x,y,z);
    return p;
}
float dot(Point p1,Point p2){
    return p1.x*p2.x+p1.y*p2.y+p1.z*p2.z;
}
Point cross(Point p1,Point p2){
    float x = p1.y*p2.z-p1.z*p2.y;
    float y = p1.z*p2.x-p1.x*p2.z;
    float z = p1.x*p2.y-p1.y*p2.x;
    Point p = Point(x,y,z);
    return p;
}
Point rodriguez(Point p,Point a,float angle){
    Point p1 = p*cos(angle);
    Point p2 = cross(a,p)*sin(angle);
    Point p3 = a*dot(a,p)*(1-cos(angle));
    Point p4 = p1+p2+p3;
    return p4;
}
single_matrix multiply(Matrix m1,single_matrix m2){
    single_matrix m3;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 1; j++){
                for(int k = 0; k < 4; k++){
                    m3.m[i][j] += m1.m[i][k] * m2.m[k][j];
                }
        }
    }
    return m3;
}
static unsigned long int g_seed = 1;
inline int random_col()
{
g_seed = (214013 * g_seed + 2531011);
return (g_seed >> 16) & 0x7FFF;
}
class triangle{
    public:
    single_matrix p1,p2,p3;
    int red,green,blue;
    float top_scaline,bottom_scaline;
    triangle(single_matrix p1,single_matrix p2,single_matrix p3){
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
    }
    void print(){
        p1.print();
        p2.print();
        p3.print();
    }
    void set_color(){
        red = random_col()%256;
        green = random_col()%256;
        blue = random_col()%256;
    }
};
float max(float a,float b,float c){
    if(a>=b && a>=c) return a;
    else if(b>=a && b>=c) return b;
    else return c;
}
float min(float a,float b,float c){
    if(a<=b && a<=c) return a;
    else if(b<=a && b<=c) return b;
    else return c;
}
int main(){
    std::cout<<M_PI<<endl;
    ifstream in("scene.txt");
    streambuf *cinbuf=std::cin.rdbuf();
    std::cin.rdbuf(in.rdbuf());

    ofstream out("stage4.txt");
    streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());
    float points[3][3];
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
                std::cin >>setprecision(5)>> points[i][j];
        }
    }
    Point eye = Point(points[0][0],points[0][1],points[0][2]);
    Point look = Point(points[1][0],points[1][1],points[1][2]);
    Point up = Point(points[2][0],points[2][1],points[2][2]);
    
    Point l=look-eye;
    l=normalize(l.x,l.y,l.z);
    Point r=cross(l,up);
    r=normalize(r.x,r.y,r.z);
    Point u=cross(r,l);
    float m[4][4] = {{r.x,r.y,r.z,0},
                     {u.x,u.y,u.z,0},
                     {-l.x,-l.y,-l.z,0},
                     {0,0,0,1}};
    Matrix R = Matrix(m);
    float m1[4][4] = {{1,0,0,-eye.x},
                      {0,1,0,-eye.y},
                      {0,0,1,-eye.z},
                      {0,0,0,1}};
    Matrix T = Matrix(m1);
    Matrix V = R*T;
    

    float fovY, aspectRatio, near, far;
    std::cin >> fovY >> aspectRatio >> near >> far;

    float fovX = fovY*aspectRatio;
    float t = near*tan(fovY*PI/(2*180.0));
    float r1 = near*tan(fovX*PI/(2*180.0));
    float m2[4][4] = {{near/r1,0,0,0},
                      {0,near/t,0,0},
                      {0,0,-(far+near)/(far-near),-(2*far*near)/(far-near)},
                      {0,0,-1,0}};
    Matrix q=Matrix(m2);
    string line;
    getline(std::cin,line);
    // getline(cin,line);
    // cout << line << endl;
    stack<Matrix> S;
    
    Matrix I;
    float mm[4][4] = {{1,0,0,0},
                     {0,1,0,0},
                     {0,0,1,0},
                     {0,0,0,1}};
    I = Matrix(mm);
    Matrix currentMatrix=I;
    vector<triangle> triangles;
    while(true){
        getline(std::cin,line);
        if(line == "triangle"){
            float m1[4][1] ,m2[4][1],m3[4][1];
            for(int i=0;i<3;i++){
                std::cin>>m1[i][0];
            }
            for(int i=0;i<3;i++){
                std::cin>>m2[i][0];
            }
            for(int i=0;i<3;i++){
                std::cin>>m3[i][0];
            }
            m1[3][0]=1;
            m2[3][0]=1;
            m3[3][0]=1;
            single_matrix p1 = single_matrix(m1);
            single_matrix p2 = single_matrix(m2);
            single_matrix p3 = single_matrix(m3);
            p1=multiply(currentMatrix,p1);
            p1=multiply(V,p1);
            p1=multiply(q,p1);
            p1=p1/p1.m[3][0];
            // p2=p2*currentMatrix;
            p2=multiply(currentMatrix,p2);
            p2=multiply(V,p2);
            p2=multiply(q,p2);
            p2=p2/p2.m[3][0];
            // p3=p3*currentMatrix;
            p3=multiply(currentMatrix,p3);
            p3=multiply(V,p3);
            p3=multiply(q,p3);
            p3=p3/p3.m[3][0];
            triangle t = triangle(p1,p2,p3);
            triangles.push_back(t);
            // p1.print();
            // p2.print();
            // p3.print();
            // cout<<"a triangle"<<endl;
           
        }
        else if(line == "translate"){
            float x,y,z;
            std::cin >> x >> y >> z;
            float m[4][4] = {{1,0,0,x},
                             {0,1,0,y},
                             {0,0,1,z},
                             {0,0,0,1}};
            Matrix T = Matrix(m);
            Matrix M = currentMatrix;
            currentMatrix = M*T;
        }
        else if(line == "scale"){
            float x,y,z;
            std::cin >> x >> y >> z;
            float m[4][4] = {{x,0,0,0},
                             {0,y,0,0},
                             {0,0,z,0},
                             {0,0,0,1}};
            Matrix T = Matrix(m);
            Matrix M = currentMatrix;
            currentMatrix = M*T;
        }
        else if(line == "rotate"){
            float x,y,z,angle;
            std::cin>>angle>>x>>y>>z;
            angle = angle*PI/180;
            // normalize the vector (x,y,z)
            Point p = normalize(x,y,z);
            //using rodrigues formula with the angle taken as input
            Point x_value=Point(1,0,0);
            Point y_value=Point(0,1,0);
            Point z_value=Point(0,0,1);
            Point p1 = rodriguez(x_value,p,angle);
            Point p2 = rodriguez(y_value,p,angle);
            Point p3 = rodriguez(z_value,p,angle);
            float m[4][4] = {{p1.x,p2.x,p3.x,0},
                             {p1.y,p2.y,p3.y,0},
                             {p1.z,p2.z,p3.z,0},
                             {0,0,0,1}};
            Matrix T = Matrix(m);
            Matrix M = currentMatrix;
            currentMatrix = M*T;
        }
        else if(line == "push"){
            S.push(currentMatrix);

        }
        else if(line == "pop"){
            currentMatrix = S.top();
            S.pop();
        }
        else if(line == "end"){
            break;
        }
    }

    // std::cin.rdbuf(cinbuf);
    // std::cout.rdbuf(coutbuf);


    // read data from config.txt
    ifstream inn("config.txt");
    streambuf *cinbuff=std::cin.rdbuf();
    std::cin.rdbuf(inn.rdbuf());
    
    int width,height;
    std::cin>>width>>height;
    std::cout<<width<<" "<<height<<endl;

    // initialize the z-buffer and frame buffer
    float right_limit,left_limit,top_limit,bottom_limit;
    right_limit=1;
    left_limit=-1;
    top_limit=1;
    bottom_limit=-1;
    float dx=(right_limit-left_limit)/width;
    float dy=(top_limit-bottom_limit)/height;
    float top_y=top_limit - dy/2;
    float bottom_y=bottom_limit + dy/2;
    float left_x=left_limit + dx/2;
    float right_x=right_limit - dx/2;
    // float z_buffer[width][height];
    // dynamic allocation of z-buffer
    float **z_buffer;
    z_buffer = new float*[width];
    for(int i=0;i<width;i++){
        z_buffer[i]=new float[height];
    }
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            z_buffer[i][j]=1.0;
        }
    }
    bitmap_image image(width,height);
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            image.set_pixel(i,j,0,0,0);
        }
    }

    // apply procedure for each triangle
    for(int i=0;i<triangles.size();i++){
        float temp=max(triangles[i].p1.m[1][0],triangles[i].p2.m[1][0],triangles[i].p3.m[1][0]);
        if(temp>top_y) temp=top_y;
        triangles[i].top_scaline=temp;
        temp=min(triangles[i].p1.m[1][0],triangles[i].p2.m[1][0],triangles[i].p3.m[1][0]);
        if(temp<bottom_limit) temp=bottom_limit;
        triangles[i].bottom_scaline=temp;
        // determining left intersecting column and right intersecting column
        int top_intersecting_row=0,bottom_intersecting_row;
        while(true){
            if(top_y-top_intersecting_row*dy<=triangles[i].top_scaline) break;
            top_intersecting_row++;
        }
        bottom_intersecting_row=top_intersecting_row;
        while(true){
            if(top_y-bottom_intersecting_row*dy<triangles[i].bottom_scaline) break;
            bottom_intersecting_row++;
        }
        bottom_intersecting_row++;
        
    }

}