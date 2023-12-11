#include<bits/stdc++.h>
using namespace std;

#include<GL/glut.h>

int counter=0;
class point{
    public:
    double x;
    double y;
    double z;
    point(double a,double b,double c){
        x=a;
        y=b;
        z=c;
    }
    //operator overloading
    point operator+(point a){
        return point(x+a.x,y+a.y,z+a.z);
    }
    point operator-(point a){
        return point(x-a.x,y-a.y,z-a.z);
    }
    point operator*(double a){
        return point(x*a,y*a,z*a);
    }
    point operator/(double a){
        return point(x/a,y/a,z/a);
    }
    point operator=(point a){
        x=a.x;
        y=a.y;
        z=a.z;
        return *this;
    }
    point operator+=(point a){
        x+=a.x;
        y+=a.y;
        z+=a.z;
        return *this;
    }
    point operator-=(point a){
        x-=a.x;
        y-=a.y;
        z-=a.z;
        return *this;
    }
    point operator-(){
        return point(-x,-y,-z);
    }
    point operator*(point a){
        return point(a.x*x,a.y*y,a.z*z);
    }
    friend point operator*(double constant, point const& rhs);

};
inline point operator*(double constant, point const& rhs) {
    return point(rhs.x*constant,rhs.y*constant,rhs.z*constant);
}
class cam_position{
    public:
    point up;
    point left;
    point right;
    void set_cam(point a,point b,point c){
        up=a;
        left=b;
        right=c;
    }
    
};

void Display(void)
{
    cout<<"Display"<<counter<<endl;
    counter++;
    glClear(GL_COLOR_BUFFER_BIT);
    // glBegin(GL_LINES){
    //     glVertex2f(0.0, 0.0);
    //     glVertex2f(0.0, 0.5);
    //     glVertex2f(0.5, 0.5);
    //     glVertex2f(0.5, 0.0);
    // };
    glFlush();

}
void init(void)
{
    // cout<<"Init"<<counter<<endl;
    // counter++;
    glClearColor(0.0, 0.0, 1.0, 0.0);
}
int main(int argc, char** argv)
{
    cout<<"Hello World"<<counter<<endl;
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    
    glutCreateWindow("Task 1");
    init();
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;
}