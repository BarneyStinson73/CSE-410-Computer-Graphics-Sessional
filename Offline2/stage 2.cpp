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
        double t=0.0;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 1; j++){
                if(abs(m[i][j])>=0.0001) cout <<fixed<< setprecision(7)<< m[i][j] << " ";
                else cout<<fixed<<setprecision(7)<<t<<" ";
            }
            
        }
        cout << endl;
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
int main(){
    ifstream in("scene.txt");
    streambuf *cinbuf=cin.rdbuf();
    cin.rdbuf(in.rdbuf());

    ofstream out("stage2.txt");
    streambuf *coutbuf = cout.rdbuf();
    cout.rdbuf(out.rdbuf());
    float points[3][3];
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
                cin >> points[i][j];
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

    string line;
    getline(cin,line);
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
    while(true){
        getline(cin,line);
        if(line == "triangle"){
            float m1[4][1] ,m2[4][1],m3[4][1];
            for(int i=0;i<3;i++){
                cin>>m1[i][0];
            }
            for(int i=0;i<3;i++){
                cin>>m2[i][0];
            }
            for(int i=0;i<3;i++){
                cin>>m3[i][0];
            }
            m1[3][0]=1;
            m2[3][0]=1;
            m3[3][0]=1;
            single_matrix p1 = single_matrix(m1);
            single_matrix p2 = single_matrix(m2);
            single_matrix p3 = single_matrix(m3);
            p1=multiply(currentMatrix,p1);
            p1=multiply(V,p1);
            p1=p1/p1.m[3][0];
            // p2=p2*currentMatrix;
            p2=multiply(currentMatrix,p2);
            p2=multiply(V,p2);
            p2=p2/p2.m[3][0];
            // p3=p3*currentMatrix;
            p3=multiply(currentMatrix,p3);
            p3=multiply(V,p3);
            p3=p3/p3.m[3][0];
            p1.print();
            p2.print();
            p3.print();
            cout<<endl;
            // cout<<"a triangle"<<endl;
           
        }
        else if(line == "translate"){
            float x,y,z;
            cin >> x >> y >> z;
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
            cin >> x >> y >> z;
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
            cin>>angle>>x>>y>>z;
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

    cin.rdbuf(cinbuf);
    cout.rdbuf(coutbuf);
    in.close();
    out.close();
    
}