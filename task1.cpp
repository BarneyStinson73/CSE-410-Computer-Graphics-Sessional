#include<bits/stdc++.h>
using namespace std;

#include<GL/glut.h>

#define PI acos(-1.0)
#define DEG2RAD(deg) (deg * PI / 180)

#define X_AXIS point(1,0,0)
#define Y_AXIS point(0,1,0)
#define Z_AXIS point(0,0,1)
#define ORIGIN point(0,0,0)
#define CAM_INIT_UP point(0,0,1)
#define CAM_INIT_L point(-1 / sqrt(2), -1 / sqrt(2), 0)
#define CAM_INIT_R point(-1 / sqrt(2), 1 / sqrt(2), 0)
#define CAM_INIT_POS point(100, 100, 100)
#define CAM_MOVE_DIST 3
#define CAM_ROTATE_ANGLE 5
#define AXIS_LEN 1000
void keyboardListener(unsigned char key, int x, int y);
void specialKeyListener(int key, int x, int y);
void mouseListener(int button, int state, int x, int y);
void Display(void);
void init(void);
void draw_axis();
void draw_square();
void draw_circle();
void draw_cylinder();


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
    point(){
        x=0;
        y=0;
        z=0;
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
    // friend point operator*(double constant, point const& right);

};
// point operator*(double constant, point const& right) {
//     return point(right.x*constant,right.y*constant,right.z*constant);
// }
class cam_position{
    public:
    point up;
    point look;
    point right;
    point position;
    cam_position(){
        up=CAM_INIT_UP;
        look=CAM_INIT_L;
        right=CAM_INIT_R;
        position=CAM_INIT_POS;
    }
    void set_cam(point a,point b,point c){
        up=a;
        look=b;
        right=c;
    }
    void move_up(){
        position+up * CAM_MOVE_DIST;
    }
    void move_down(){
        position-up * CAM_MOVE_DIST;
    }
    void move_left(){
        position-right * CAM_MOVE_DIST;
    }
    void move_right(){
        position+right * CAM_MOVE_DIST;
    }
    void move_forward(){
        position+look * CAM_MOVE_DIST;
    }
    void move_backward(){
        position-look * CAM_MOVE_DIST;
    }
    void look_left(){
        double angle = DEG2RAD(CAM_ROTATE_ANGLE);
        look = look * cos(angle) - right * sin(angle);
        right = look * sin(angle) + right * cos(angle);
    }
    void look_right(){
        double angle = DEG2RAD(-CAM_ROTATE_ANGLE);
        // angle=angle*(-1.0);
        look = look * cos(angle) - right * sin(angle);
        right = look * sin(angle) + right * cos(angle);
    }
    void look_up(){
        double angle = DEG2RAD(CAM_ROTATE_ANGLE);
        look = look * cos(angle) + up * sin(angle);
        up = look * (-sin(angle)) + up * cos(angle);
    }
    void look_down(){
        double angle = DEG2RAD(-CAM_ROTATE_ANGLE);
        // angle=angle*(-1.0);
        look = look * cos(angle) + up * sin(angle);
        up = look * (-sin(angle)) + up * cos(angle);
    }
    void tilt_clockwise(){
        double angle = DEG2RAD(CAM_ROTATE_ANGLE);
        right = right * cos(angle) - up * sin(angle);
        up = right * (sin(angle)) + up * cos(angle);
    }
    void tilt_anticlockwise(){
        double angle = DEG2RAD(-CAM_ROTATE_ANGLE);
        // angle=angle*(-1.0);
        right = right * cos(angle) - up * sin(angle);
        up = right * (-sin(angle)) + up * cos(angle);
    }
};
cam_position cam;
void draw_axis(){
    glBegin(GL_LINES);
    {
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(AXIS_LEN, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, AXIS_LEN, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, AXIS_LEN);
    }
    glEnd();
}

double a=0.5;
// 1- look left, 2- look right, 3- look up, 4- look down, 5- tilt anti clockwise, 6- tilt clockwise
// up arrow- move forward, down arrow- move backward, left arrow- move left, right arrow- move right ,page up- move up, page down- move down
void keyboardListener(unsigned char key, int x, int y){
    switch(key){
        case '1':
            cam.look_left();
            break;
        case '2':
            cam.look_right();
            break;
        case '3':
            cam.look_up();
            break;
        case '4':
            cam.look_down();
            break;
        case '5':
            cam.tilt_anticlockwise();
            break;
        case '6':
            cam.tilt_clockwise();
            break;
        default:
            break;
    }
    
}
void specialKeyListener(int key, int x, int y){
    switch(key){
        case GLUT_KEY_UP:
            cam.move_forward();
            break;
        case GLUT_KEY_DOWN:
            cam.move_backward();
            break;
        case GLUT_KEY_LEFT:
            cam.move_left();
            break;
        case GLUT_KEY_RIGHT:
            cam.move_right();
            break;
        case GLUT_KEY_PAGE_UP:
            cam.move_up();
            break;
        case GLUT_KEY_PAGE_DOWN:
            cam.move_down();
            break;
        default:
            break;
    }
}

void Display(void)
{
    // cout<<"Display"<<counter<<endl;
    counter++;
    glClear(GL_COLOR_BUFFER_BIT);
    // glBegin(GL_LINES){
    //     glVertex2f(0.0, 0.0);
    //     glVertex2f(0.0, 0.5);
    //     glVertex2f(0.5, 0.5);
    //     glVertex2f(0.5, 0.0);
    // };
    glBegin(GL_QUADS);
	{
		glVertex3f(a, a, 0);
		glVertex3f(a, -a, 0);
		glVertex3f(-a, -a, 0);
		glVertex3f(-a, a, 0);
	}
	glEnd();
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
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGB);
    
    glutCreateWindow("Task 1");
    init();
    glutDisplayFunc(Display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    glutMainLoop();
    return 0;
}