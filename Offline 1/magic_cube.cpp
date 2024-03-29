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
// #define CAM_INIT_L point(0,0,0)
#define CAM_INIT_R point(-1 / sqrt(2), 1 / sqrt(2), 0)

#define CAM_INIT_POS point(10, 10, 10)
#define CAM_MOVE_DIST 3
#define CAM_ROTATE_ANGLE 0.5
#define AXIS_LEN 1000
#define MAX_TRIANGLE_SIDE 10.0
void keyboardListener(unsigned char key, int x, int y);
void specialKeyListener(int key, int x, int y);
void mouseListener(int button, int state, int x, int y);
void Display(void);
void init(void);
void draw_axis();
void draw_square();
void draw_circle();
void draw_cylinder();

double triangle_side=MAX_TRIANGLE_SIDE;

double maxSphereRadius = MAX_TRIANGLE_SIDE / sqrt(3.0);
double sphere_radius = 0;
int counter=0;
double object_angle=0.0;
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

    point cross_product(point a){
        return point(y*a.z-z*a.y,z*a.x-x*a.z,x*a.y-y*a.x);
    }
    double dot_product(point a){
        return x*a.x+y*a.y+z*a.z;
    }
    // friend point operator*(double constant, point const& right);

};
// point operator*(double constant, point const& right) {
//     return point(right.x*constant,right.y*constant,right.z*constant);
// }
point normalize(point p){
    double t;
    t=p.x * p.x + p.y* p.y + p.z*p.z;
    t=sqrt(t);
    p.x=p.x/t;
    p.y=p.y/t;
    p.z=p.z/t;
    return p;    
}
point look_calculation(point pos,point look){
    point temp;
    temp=look-pos;
    return temp;    
}
point rodriguez_formula(point a,point k,double angle){
    point temp;
    temp=a*cos(angle)+k.cross_product(a)*sin(angle)+k*(k.dot_product(a))*(1-cos(angle));
    return temp;
}
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
        // point up_vector;
        // up_vector= up - position;
        point up_vector=normalize(up);
        position+=up_vector * CAM_MOVE_DIST;
        look+=up_vector * CAM_MOVE_DIST;
    }
    void move_down(){
        // point up_vector;
        // up_vector= up - position;   
        point up_vector=normalize(up);
        position-=up_vector * CAM_MOVE_DIST;
        look-=up_vector * CAM_MOVE_DIST;
    }
    void move_left(){
    
        point up_vector=normalize(up);
        point look_vector = normalize(look - position);
        point right_vector = look_vector.cross_product(up_vector);
        position-=right_vector * CAM_MOVE_DIST;
        look-=right_vector * CAM_MOVE_DIST;
    }
    void move_right(){
        point up_vector=normalize(up);
        point look_vector = normalize(look - position);
        point right_vector = look_vector.cross_product(up_vector);
        position+=right_vector * CAM_MOVE_DIST;
        look+=right_vector * CAM_MOVE_DIST;
    }
    void move_forward(){
        point look_vector;
        look_vector= look - position;
        look_vector=normalize(look_vector);
        position+=look_vector * CAM_MOVE_DIST;
    }
    void move_backward(){
        point look_vector;
        look_vector= look - position;
        look_vector=normalize(look_vector);
        position-=look_vector * CAM_MOVE_DIST;
    }
    void look_left(){
        double angle = DEG2RAD(CAM_ROTATE_ANGLE);
        // look = look * cos(angle) - right * sin(angle);
        // right = look * sin(angle) + right * cos(angle);
        point look_vector = look - position;
        look = position + rodriguez_formula(look_vector,up,angle);
    }
    void look_right(){
        double angle = DEG2RAD(-CAM_ROTATE_ANGLE);
        // angle=angle*(-1.0);
        // look = look * cos(angle) - right * sin(angle);
        // right = look * sin(angle) + right * cos(angle);
        point look_vector = look - position;
        look = position + rodriguez_formula(look_vector,up,angle);
        // right=rodriguez_formula(right,up,angle);
    }
    void look_up(){
        double angle = DEG2RAD(CAM_ROTATE_ANGLE) / 10;
        // look = look * cos(angle) + up * sin(angle);
        // up = look * (-sin(angle)) + up * cos(angle);
        point look_vector = look - position;
        point right_vector = look_vector.cross_product(up);
        look=position+rodriguez_formula(look_vector,right_vector,angle);
        up=rodriguez_formula(up,right_vector,angle);
    }
    void look_down(){
        double angle = DEG2RAD(-CAM_ROTATE_ANGLE)/10;
        // angle=angle*(-1.0);
        // look = look * cos(angle) + up * sin(angle);
        // up = look * (-sin(angle)) + up * cos(angle);
        point look_vector = look - position;
        point right_vector = look_vector.cross_product(up);
        look=position+rodriguez_formula(look_vector,right_vector,angle);
        up=rodriguez_formula(up,right_vector,angle);
    }
    void tilt_clockwise(){
        double angle = DEG2RAD(CAM_ROTATE_ANGLE)/10;
        // right = right * cos(angle) - up * sin(angle);
        // up = right * (sin(angle)) + up * cos(angle);
        point look_vector = look - position;
        point right_vector = look_vector.cross_product(up);
        right=position+rodriguez_formula(right_vector,look_vector,angle);
        up=rodriguez_formula(up,look_vector,angle);
    }
    void tilt_anticlockwise(){
        double angle = DEG2RAD(-CAM_ROTATE_ANGLE)/10;
        // angle=angle*(-1.0);
        // right = right * cos(angle) - up * sin(angle);
        // up = right * (sin(angle)) + up * cos(angle);
        point look_vector = look - position;
        point right_vector = look_vector.cross_product(up);
        right=position+rodriguez_formula(right_vector,look_vector,angle);
        up=rodriguez_formula(up,look_vector,angle);
    }
    void print(){
        cout<<"up: "<<up.x<<" "<<up.y<<" "<<up.z<<endl;
        cout<<"look: "<<look.x<<" "<<look.y<<" "<<look.z<<endl;
        cout<<"right: "<<right.x<<" "<<right.y<<" "<<right.z<<endl;
        cout<<"position: "<<position.x<<" "<<position.y<<" "<<position.z<<endl;
    
    }
};
cam_position cam;
void draw_axis(){
    glPushMatrix();
    glBegin(GL_LINES);
    {
        glColor3f(1, 0, 0);
        glVertex3f(-AXIS_LEN , 0, 0);
        glVertex3f(AXIS_LEN, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, -AXIS_LEN , 0);
        glVertex3f(0, AXIS_LEN, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, -AXIS_LEN );
        glVertex3f(0, 0, AXIS_LEN);
    }
    glEnd();

    glPopMatrix();
}

double a=0.5;
// 1- look left, 2- look right, 3- look up, 4- look down, 5- tilt anti clockwise, 6- tilt clockwise
// up arrow- move forward, down arrow- move backward, left arrow- move left, right arrow- move right ,page up- move up, page down- move down
void keyboardListener(unsigned char key, int x, int y){
    cout<<key<<endl;
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
        case '7':
            cam.print();
            break;
        case ',':
            if(triangle_side>0){
                triangle_side=max(triangle_side-.3,0.0);
                sphere_radius= min(MAX_TRIANGLE_SIDE/sqrt(3),sphere_radius+.3 / sqrt(3.0));
            }
            break;
        case '.':
            if(triangle_side<MAX_TRIANGLE_SIDE){
                triangle_side=min(triangle_side+.3,MAX_TRIANGLE_SIDE);
                sphere_radius= max(0.0,sphere_radius-.3 / sqrt(3.0));
            }
            break;
        case 'a':
            object_angle-=5;
            break;
        case 'd':
            object_angle+=5;
            break;
        default:
            break;
    }
    glutPostRedisplay();
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
    glutPostRedisplay();
}

void drawCheckerBox(double a, int color = 0) {
  glBegin(GL_QUADS);
  {
    if (color == 0) {
      glColor3f(0.0f, 0.0f, 0.0f); // Black
    } else {
      glColor3f(1.0f, 1.0f, 1.0f); // White
    }
    glVertex3f(0, 0, 0);
    glVertex3f(0, a, 0);
    glVertex3f(a, a, 0);
    glVertex3f(a, 0, 0);
  }
  glEnd();
}

void drawCheckers(double a) {
  for (int i = -40; i < 40; i++) {
    for (int j = -40; j < 40; j++) {
      glPushMatrix();
      glTranslatef(i * a, j * a, 0);
      drawCheckerBox(a, (i + j) % 2);
      glPopMatrix();
    }
  }
}

void drawSquare(double a) {
  glBegin(GL_QUADS);
  {
    glVertex3f(0, 0, 0);
    glVertex3f(0, a, 0);
    glVertex3f(a, a, 0);
    glVertex3f(a, 0, 0);
  }
  glEnd();
}
void draw_triangle(){
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(0,triangle_side,0);
        glVertex3f(0,0,triangle_side);
        glVertex3f(triangle_side,0,0);
    }
    glEnd();
}
// drawing a pyramid using the created draw_triangle function along with draw_axis
void draw_pyramid(){
    double difference=MAX_TRIANGLE_SIDE-triangle_side;
    difference=difference/3;
    glColor3f(1.0,0.0,0.0);
    for(int i=0;i<4;i++){
        glPushMatrix();
        // glTranslatef(difference,difference,difference);
        // glRotatef(180.0,1.0,0.0,0.0);
        glRotatef(90.0*i,0.0,0.0,1.0);
        glTranslatef(difference,difference,difference);
        draw_triangle();
        glPopMatrix();
    }
    glRotatef(180.0,1.0,0.0,0.0);
    for(int i=0;i<4;i++){
        glPushMatrix();
        // glTranslatef(difference,difference,difference);
        
        glRotatef(90.0*i,0.0,0.0,1.0);
        glTranslatef(difference,difference,difference);
        draw_triangle();
        glPopMatrix();
    }
}
// void draw_cylinder(){
//     double r=MAX_TRIANGLE_SIDE/2;
//     double h=MAX_TRIANGLE_SIDE;
//     int segments=100;
//     double angle=2*PI/segments;
//     double x,y,z;
//     glColor3f(1.0,1.0,0.0);
//     for(int i=0;i<segments;i++){
//         x=r*cos(angle*i);
//         y=r*sin(angle*i);
//         z=0;
//         glBegin(GL_TRIANGLES);
//         {
//             glVertex3f(0,0,0);
//             glVertex3f(x,y,z);
//             glVertex3f(x,y,z+h);
//         }
//         glEnd();
//     }
//     for(int i=0;i<segments;i++){
//         x=r*cos(angle*i);
//         y=r*sin(angle*i);
//         z=0;
//         glBegin(GL_TRIANGLES);
//         {
//             glVertex3f(0,0,0);
//             glVertex3f(x,y,z);
//             glVertex3f(x,y,z+h);
//         }
//         glEnd();
//     }
//     for(int i=0;i<segments;i++){
//         x=r*cos(angle*i);
//         y=r*sin(angle*i);
//         z=0;
//         glBegin(GL_TRIANGLES);
//         {
//             glVertex3f(0,0,0);
//             glVertex3f(x,y,z);
//             glVertex3f(x,y,z+h);
//         }
//         glEnd();
//     }
//     for(int i=0;i<segments;i++){
//         x=r*cos(angle*i);
//         y=r*sin(angle*i);
//         z=0;
//         glBegin(GL_TRIANGLES);
//         {
//             glVertex3f(0,0,0);
//             glVertex3f(x,y,z);
//             glVertex3f(x,y,z+h);
//         }
//         glEnd();
//     }
// }
void drawCylinder(double h, double r, int segments) {

    vector<point> points;

    double offset = 70.5287794*PI/180.0;

    for (int i = 0; i <= segments+1; i++) {
        double theta = -offset/2 +  i * offset / segments;
        points.push_back(point(r * cos(theta),r * sin(theta),h/2));
        theta = -offset/2 +  (i+1) * offset / segments;
        points.push_back(point(r * cos(theta),r * sin(theta),-h/2));
    }

    glBegin(GL_QUADS);
        for (int i = 0; i < points.size()-2; i+=2) {
            glVertex3f(points[i].x, points[i].y, points[i].z);
            glVertex3f(points[i+1].x, points[i+1].y, points[i+1].z);
            glVertex3f(points[i+3].x, points[i+3].y, points[i+3].z);
            glVertex3f(points[i+2].x, points[i+2].y, points[i+2].z);
        }
    glEnd();
}

// double maxSphereRadius = MAX_TRIANGLE_SIDE / sqrt(3.0);
// double sphere_radius = 1.0;
void drawCylinders(){

    glColor3f(1.0f, 1.0f, 0.0f);

    for(int i=0;i<4;i++){

        glPushMatrix();{
            
            glRotatef(45+i*90,0,1,0);
            glTranslatef(triangle_side/sqrt(2),0,0);
            drawCylinder(triangle_side*sqrt(2),sphere_radius,100);
        }glPopMatrix();
        
    }

    for(int i=0;i<4;i++){

        glPushMatrix();{
            glRotatef(90,1,0,0);
            glRotatef(45+i*90,0,1,0);
            glTranslatef(triangle_side/sqrt(2),0,0);
            drawCylinder(triangle_side*sqrt(2),sphere_radius,100);
        }glPopMatrix();
        
    }

    for(int i=0;i<4;i++){

        glPushMatrix();{
            glRotatef(90,0,0,1);
            glRotatef(45+i*90,0,1,0);
            glTranslatef(triangle_side/sqrt(2),0,0);
            drawCylinder(triangle_side*sqrt(2),sphere_radius,100);
        }glPopMatrix();

    }

}
void drawSphereQuad(double r,int segments)
{
   
    vector<vector<point>> points(segments+1);
	
	double x,y;
	//generate points
	for(int i=0;i<=segments;i++){
		
		for(int j=0;j<=segments;j++){
            x = -1 + (double)i/segments*2;
		    y = -1 + (double)j/segments*2;
            points[i].push_back(normalize(point(x,y,1))*r);
		}
	}
	//draw quads using generated points
	for(int i=0;i<segments;i++)
	{
		for(int j=0;j<segments;j++)
		{
			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawSpheres(){

    for(int i=0;i<4;i++){
        
        glPushMatrix();{
            if(i%2==0)glColor3f(0.0f, 1.0f, 1.0f);
            else glColor3f(1.0f, 0.0f, 1.0f);
            // glColor3f(0.2f, .2f, 0.2f);     
            glRotatef(90*i,0,1,0);
            glTranslatef(0,0,triangle_side);
            drawSphereQuad(sphere_radius,100);

        }glPopMatrix();

    }

    for(int i=0;i<2;i++){
        
        glPushMatrix();{
            if(i%2==0)glColor3f(1.0f, 0.5f, 0.5f);
            else glColor3f(0.5f, 1.0f, 0.5f);
            // glColor3f(0.0f, 0.0f, 1.0f);    
            glRotatef(90+180*i,1,0,0);
            glTranslatef(0,0,triangle_side);
            drawSphereQuad(sphere_radius,100);
        }glPopMatrix();

    }

}

void Display(void)
{
    // // cout<<"Display"<<counter<<endl;
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glClearColor(0,0,0,0);
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cam.position.x, cam.position.y, cam.position.z, cam.look.x, cam.look.y, cam.look.z, cam.up.x, cam.up.y, cam.up.z);


    counter++;
    // cout<<counter<<endl;
    draw_axis();
    glRotatef(object_angle, 0, 0, 1);
    // drawCheckers(2);
    draw_pyramid();
    drawSpheres();
    // glClear(GL_COLOR_BUFFER_BIT);
    // glBegin(GL_LINES){
    //     glVertex2f(0.0, 0.0);
    //     glVertex2f(0.0, 0.5);
    //     glVertex2f(0.5, 0.5);
    //     glVertex2f(0.5, 0.0);
    // };e
    // glBegin(GL_LINES);
    // {
    //     glColor3f(1,1,1);
    //     glVertex3f(cam.position.x,cam.position.y,cam.position.z);
    //     glVertex3f(0,0,0);
    // }glEnd();
    drawCylinders();
    a = 1.0;

    // glColor3f(1.0,1.0,0.0);
    // glPushMatrix();
    // glTranslatef(0.0,0.0,1.0);
    // drawSquare(2);
    // glPopMatrix();

    glFlush();
    glutSwapBuffers();

}
void init(void)
{
    // cout<<"Init"<<counter<<endl;
    // counter++;
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1, 1, 100.0);
}
void idle(){
    glutPostRedisplay();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    
    glutCreateWindow("Task 2");
    init();
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(Display);
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}