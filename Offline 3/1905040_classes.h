#include<cmath>
#include<iostream>
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


class ray{
    public:
    point start;
    point dir;
    ray(point a,point b){
        start=a;
        dir=b;
    }
};
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
        point up_vector=normalize(up);
        position+=up_vector * CAM_MOVE_DIST;
        look+=up_vector * CAM_MOVE_DIST;
    }
    void move_down(){  
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
        point look_vector = look - position;
        look = position + rodriguez_formula(look_vector,up,angle);
    }
    void look_right(){
        double angle = DEG2RAD(-CAM_ROTATE_ANGLE);
        point look_vector = look - position;
        look = position + rodriguez_formula(look_vector,up,angle);
    }
    void look_up(){
        double angle = DEG2RAD(CAM_ROTATE_ANGLE) / 10;
        point look_vector = look - position;
        point right_vector = look_vector.cross_product(up);
        look=position+rodriguez_formula(look_vector,right_vector,angle);
        up=rodriguez_formula(up,right_vector,angle);
    }
    void look_down(){
        double angle = DEG2RAD(-CAM_ROTATE_ANGLE)/10;
        point look_vector = look - position;
        point right_vector = look_vector.cross_product(up);
        look=position+rodriguez_formula(look_vector,right_vector,angle);
        up=rodriguez_formula(up,right_vector,angle);
    }
    void tilt_clockwise(){
        double angle = DEG2RAD(CAM_ROTATE_ANGLE)/10;
        point look_vector = look - position;
        point right_vector = look_vector.cross_product(up);
        right=position+rodriguez_formula(right_vector,look_vector,angle);
        up=rodriguez_formula(up,look_vector,angle);
    }
    void tilt_anticlockwise(){
        double angle = DEG2RAD(-CAM_ROTATE_ANGLE)/10;
        point look_vector = look - position;
        point right_vector = look_vector.cross_product(up);
        right=position+rodriguez_formula(right_vector,look_vector,angle);
        up=rodriguez_formula(up,look_vector,angle);
    }
    void print(){
        std::cout<<"up: "<<up.x<<" "<<up.y<<" "<<up.z<<std::endl;
        std::cout<<"look: "<<look.x<<" "<<look.y<<" "<<look.z<<std::endl;
        std::cout<<"right: "<<right.x<<" "<<right.y<<" "<<right.z<<std::endl;
        std::cout<<"position: "<<position.x<<" "<<position.y<<" "<<position.z<<std::endl;
    
    }
};

class Object{
 // should have x, y, z
public:
double height, width, length;
double color[3];
double coEfficients[4]; // ambient, diffuse, specular,reflection coefficients
int shine ;// exponent term of specular component

point reference_point;
virtual void draw(){}
void setColor(double r,double g,double b){
    color[0]=r;
    color[1]=g;
    color[2]=b;
}
void setShine(int s){
    shine=s;
}
void setCoEfficients(double a,double d,double s,double r){
    coEfficients[0]=a;
    coEfficients[1]=d;
    coEfficients[2]=s;
    coEfficients[3]=r;
}
virtual double intersect(ray *r,double *color,int level){
    return -1.0;
    
};
};

class Sphere:public Object{
public:
Sphere(point p,double r){
    reference_point=p;
    length=r;
}
void draw(){
    glPushMatrix();
    {
        glTranslatef(reference_point.x,reference_point.y,reference_point.z);
        glColor3f(color[0],color[1],color[2]);
        glutSolidSphere(length,100,100);
    }
    glPopMatrix();
}

double intersect(ray *r,double *color,int level){

    // ray origin inside or outside or on the sphere
    point oc=r->start-reference_point;
    double r_2=length*length;
    double b=2*oc.dot_product(r->dir);
    double c=oc.dot_product(oc)-r_2;
    int flag=0;
    if(c<0){
        flag=-1;
    }
    else if(c==0){
        flag=0;
    }
    else{
        flag=1;
    }
    double t_p=-oc.dot_product(r->dir);
    if(t_p<0 && flag==-1){
        return -1.0;
    }
    // finding squared distance
    double d_2=oc.dot_product(oc)-t_p*t_p;
    if(d_2>r_2){
        return -1.0;
    }
    double t_2=r_2-d_2;
    double result;
    if(flag==1){
        result=t_p-sqrt(t_2);
    }
    else if(flag==-1){
        result=t_p+sqrt(t_2);
    }
    cout<<"Sphere intersected"<<result<<endl;
    if (level == 0) {
        return result;

    }
    point intersection_point = r->start + r->dir * result;
    for (int i = 0; i < 3; i++) {
        color[i] = this->color[i] * coEfficients[0];
    }
    point normal = intersection_point - reference_point;
    normal = normalize(normal);
    // for each point light pl in pointLights
// cast rayl from pl.light_pos to intersectionPoint
    for(int i=0;i<pointLights.size();i++){
        pointLights[i].lightPos;
        pointLights[i].color[0];
        pointLights[i].color[1];
        pointLights[i].color[2];
        point l = pointLights[i].lightPos - intersection_point;
        double l_len = sqrt(l.dot_product(l));
        l = normalize(l);
        ray shadow_ray = ray(intersection_point, l);
        double t = 0;
        for (int j = 0; j < objects.size(); j++) {
            t = objects[j]->intersect(&shadow_ray, color, 0);
            if (t > 0 && t < l_len) {
                for (int k = 0; k < 3; k++) {
                    color[k] = 0;
                }
                break;
            }
        }
        if (t == -1) {
            double lambert = l.dot_product(normal);
            if (lambert > 0) {
                for (int j = 0; j < 3; j++) {
                    color[j] += lambert * coEfficients[1] * pointLights[i].color[j];
                }
                point v = cam.position - intersection_point;
                v = normalize(v);
                point r = normal * (2 * l.dot_product(normal)) - l;
                double r_dot_v = r.dot_product(v);
                if (r_dot_v > 0) {
                    double specular = pow(r_dot_v, shine);
                    for (int j = 0; j < 3; j++) {
                        color[j] += specular * coEfficients[2] * pointLights[i].color[j];
                    }
                }
            }
        }
    }

    

};

class Triangle:public Object{
public:
point a,b,c;
Triangle(point p1,point p2,point p3){
    a=p1;
    b=p2;
    c=p3;
}
void draw(){
    glPushMatrix();
    {
        glTranslatef(reference_point.x,reference_point.y,reference_point.z);
        glColor3f(color[0],color[1],color[2]);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(a.x,a.y,a.z);
            glVertex3f(b.x,b.y,b.z);
            glVertex3f(c.x,c.y,c.z);
        }
        glEnd();
    }
    glPopMatrix();
}
};

class General:public Object{
public:
double a,b,c,d,e,f,g,h,i,j;
General(double A,double B,double C,double D,double E,double F,double G,double H,double I,double J){
    a=A;
    b=B;
    c=C;
    d=D;
    e=E;
    f=F;
    g=G;
    h=H;
    i=I;
    j=J;
}
};

class PointLight{
public:
point lightPos;
double color[3];
PointLight(){}
PointLight(point p,double r,double g,double b){
    lightPos=p;
    color[0]=r;
    color[1]=g;
    color[2]=b;
}
};

class SpotLight{
public:
PointLight pointLight;
point direction;
double cutoff_angle;
SpotLight(PointLight p,point d,double angle){
    pointLight=p;
    direction=d;
    cutoff_angle=angle;
}
};

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
class Floor:public Object{
public:
// double floorWidth,tileWidth;
Floor(double floorWidth,double tileWidth){
    reference_point=point(-floorWidth/2,-floorWidth/2,0);
    length=floorWidth;
}
void draw(){
 // There will be a floor along the XY-plane
// FloorWidth can be 1000 (from origin 500 across each side)
// Each Tile Width can be 20
// Color should be alternating
// Use GL_QUADS to draw the floor
    glPushMatrix();
    {
        glTranslatef(reference_point.x,reference_point.y,reference_point.z);
        drawCheckers(length/20.0);
    }
    glPopMatrix();
}
};


