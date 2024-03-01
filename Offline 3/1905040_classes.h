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
        dir=normalize(dir);
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
class colour{
    public:
    double r,g,b;
    colour(double a,double d,double c){
        r=a;
        g=d;
        b=c;
    }
    colour(){ r=0; g=0; b=0;}
};


class Object;
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
    direction=normalize(direction);
}
};
extern vector<Object*> objects;
extern vector<PointLight> pointLights;
extern vector<SpotLight> spotLights;
extern int recursionLevel;
class Object{
 // should have x, y, z
public:
double height, width, length;
double color[3];
double coEfficients[4]; // ambient, diffuse, specular,reflection coefficients
int shine ;// exponent term of specular component
point normal;

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
virtual double intersect_helper(ray *r)=0;
virtual double intersect(ray *r,double *color,int level){
    double result= intersect_helper(r);
    if(result<0) return -1.0;
    if (level == 0) {
        return result;
    }
    point intersection_point = r->start + r->dir * result;
    colour p= getColorAt(intersection_point);
    // for (int i = 0; i < 3; i++) {
    //     color[i] = this->color[i] * coEfficients[0];
    // }
    color[0]= p.r * coEfficients[0];
    color[1]= p.g * coEfficients[0];
    color[2]= p.b * coEfficients[0];
    // point normal = intersection_point - reference_point;
    // normal = normalize(normal);
    point normal=calculateNormal(intersection_point);
    if(normal.dot_product(r->dir) > 0){
        normal = -normal;
    }
    
    // for each point light pl in pointLights
    // cast rayl from pl.light_pos to intersectionPoint
    for(int i=0;i<pointLights.size();i++){
        
        point l = -(pointLights[i].lightPos - intersection_point);
        double l_len = sqrt(l.dot_product(l));
        if(l_len<1e-5){
            continue;
        }
        l = normalize(l);
        ray shadow_ray = ray(pointLights[i].lightPos, l);
        double t = 0;
        bool inShadow = false;
        for (int j = 0; j < objects.size(); j++) {
            t = objects[j]->intersect(&shadow_ray, color, 0);
            if (t > 0 && t+1e-5 < l_len) {
                inShadow = true;
                break;
            }
        }
        if (!inShadow) {
            double lambert = -l.dot_product(normal);
            if (lambert > 0) {
                // for (int j = 0; j < 3; j++) {
                //     color[j] += lambert * coEfficients[1] * pointLights[i].color[j];
                // }
                color[0] += lambert * coEfficients[1] * pointLights[i].color[0] * p.r;
                color[1] += lambert * coEfficients[1] * pointLights[i].color[1] * p.g;
                color[2] += lambert * coEfficients[1] * pointLights[i].color[2] * p.b;
            }
            point reflect = l - normal * (2 * l.dot_product(normal));
            reflect = normalize(reflect);
            double phong = -(reflect.dot_product(r->dir));
            if (phong > 0) {
                double specular = pow(phong, shine);
                color[0] += specular * coEfficients[2] * pointLights[i].color[0] * p.r;
                color[1] += specular * coEfficients[2] * pointLights[i].color[1] * p.g;
                color[2] += lambert * coEfficients[2] * pointLights[i].color[2] * p.b;
            }
        }
    }
    for(int i=0;i<spotLights.size();i++){
        
        point l = -(spotLights[i].pointLight.lightPos - intersection_point);
        double l_len = sqrt(l.dot_product(l));
        if(l_len<1e-5){
            continue;
        }
        l = normalize(l);
        double angle = acos(l.dot_product(spotLights[i].direction))*180/3.1416;
        if(angle>spotLights[i].cutoff_angle){
            continue;
        }
        ray shadow_ray = ray(spotLights[i].pointLight.lightPos, l);
        double t = 0;
        bool inShadow = false;
        for (int j = 0; j < objects.size(); j++) {
            t = objects[j]->intersect(&shadow_ray, color, 0);
            if (t > 0 && t+1e-5 < l_len) {
                inShadow = true;
                break;
            }
        }
        if (!inShadow) {
            double lambert = -l.dot_product(normal);
            if (lambert > 0) {
                // for (int j = 0; j < 3; j++) {
                //     color[j] += lambert * coEfficients[1] * pointLights[i].color[j];
                // }
                color[0] += lambert * coEfficients[1] * pointLights[i].color[0] * p.r;
                color[1] += lambert * coEfficients[1] * pointLights[i].color[1] * p.g;
                color[2] += lambert * coEfficients[1] * pointLights[i].color[2] * p.b;
            }
            point reflect = l - normal * (2 * l.dot_product(normal));
            reflect = normalize(reflect);
            double phong = -(reflect.dot_product(r->dir));
            if (phong > 0) {
                double specular = pow(phong, shine);
                color[0] += specular * coEfficients[2] * pointLights[i].color[0] * p.r;
                color[1] += specular * coEfficients[2] * pointLights[i].color[1] * p.g;
                color[2] += lambert * coEfficients[2] * pointLights[i].color[2] * p.b;
            }
        }
    }

    if(level<recursionLevel){
        point reflect = r->dir - normal * (2 * r->dir.dot_product(normal));
        reflect = normalize(reflect);
        ray reflected_ray = ray(intersection_point+reflect*1e-5, reflect);
        int index = -1;
        double t=DBL_MAX;
        for(int kk=0;kk<objects.size();kk++){
            double temp = objects[kk]->intersect(&reflected_ray, color, 0);
            if(temp>0 && temp<t){
                t=temp;
                index=kk;
            }
            
        }
        if (index!=-1) {
            double temp_color[3] = {0, 0, 0};
            t = objects[index]->intersect(&reflected_ray, temp_color, level + 1);
            for (int j = 0; j < 3; j++) {
                color[j] += coEfficients[3] * temp_color[j];
            }
        }
    }
    return result;
}

virtual colour getColorAt(point p){
    colour c;
    c.r=color[0];
    c.g=color[1];
    c.b=color[2];
    return c;
}
virtual point calculateNormal(point p){
    return normal;
    
}
};

double determinant(double ara[3][3]){
	double v1 = ara[0][0] * (ara[1][1] * ara[2][2] - ara[1][2] * ara[2][1]);
	double v2 = ara[0][1] * (ara[1][0] * ara[2][2] - ara[1][2] * ara[2][0]);
	double v3 = ara[0][2] * (ara[1][0] * ara[2][1] - ara[1][1] * ara[2][0]);
	return v1 - v2 + v3;
}

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
double intersect_helper(ray *r){
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
    if(t_p<0 && flag==1){
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
    else{
        result=t_p+sqrt(t_2);
    }
    return result;
}

point calculateNormal(point p){
    point normal = p - reference_point;
    normal = normalize(normal);
    return normal;
}

};

class Triangle:public Object{
public:
point a,b,c;
double ara[3][3];
double det;
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
double intersect_helper(ray *r){
    double A[3][3] = {
        {a.x - b.x, a.x - c.x, r->dir.x},
        {a.y - b.y, a.y - c.y, r->dir.y},
        {a.z - b.z, a.z - c.z, r->dir.z}
    };
    double B[3][3] = {
        {a.x - r->start.x, a.x - c.x, r->dir.x},
        {a.y - r->start.y, a.y - c.y, r->dir.y},
        {a.z - r->start.z, a.z - c.z, r->dir.z}
    };
    double C[3][3] = {
        {a.x - b.x, a.x - r->start.x, r->dir.x},
        {a.y - b.y, a.y - r->start.y, r->dir.y},
        {a.z - b.z, a.z - r->start.z, r->dir.z}
    };
    double D[3][3] = {
        {a.x - b.x, a.x - c.x, a.x - r->start.x},
        {a.y - b.y, a.y - c.y, a.y - r->start.y},
        {a.z - b.z, a.z - c.z, a.z - r->start.z}
    };
    det= determinant(A);
    double beta = determinant(B) / det;
    double gamma = determinant(C) / det;
    double t = determinant(D) / det;
    if(beta>0 && gamma>0 && beta+gamma<1 && t>0){
        return t;
    }
    else{
        return -1.0;
    }
}
point calculateNormal(point p){
    point normal = (b - a).cross_product(c - a);
    normal = normalize(normal);
    return normal;
}
};

class General:public Object{
public:
double a,b,c,d,e,f,g,h,i,j;
General(double A,double B,double C,double D,double E,double F,double G,double H,double I,double J, point center, double x,double y,double z){
    a=A;
    b=B;
    c=C;
    d=D;
    e=F;
    f=E;
    g=G;
    h=H;
    i=I;
    j=J;
    reference_point=center;
    length=x;
    width=y;
    height=z;
}
bool insideGeneral(point p){
    double x = p.x - reference_point.x;
    double y = p.y - reference_point.y;
    double z = p.z - reference_point.z;
    if(fabs(length)>1e-5 && (x < 0 || x > length ))
        return false;
    if(fabs(width)>1e-5 && (y < 0 || y > width ))
        return false;
    if(fabs(height)>1e-5 && (z < 0 || z > height ))
        return false;
    return true;
}
double intersect_helper(ray *r){

    point oc = r->start - reference_point;
    double aa = this->a * r->dir.x * r->dir.x + this->b * r->dir.y * r->dir.y + this->c * r->dir.z * r->dir.z + this->d * r->dir.x * r->dir.y + this->e * r->dir.y * r->dir.z + this->f * r->dir.z * r->dir.x;
    double bb = 2 * (this->a * r->dir.x * oc.x + this->b * r->dir.y * oc.y + this->c * r->dir.z * oc.z) + this->d * (r->dir.x * oc.y + r->dir.y * oc.x) + this->e * (r->dir.y * oc.z + r->dir.z * oc.y) + this->f * (r->dir.z * oc.x + r->dir.x * oc.z) + this->g * r->dir.x + this->h * r->dir.y + this->i * r->dir.z;
    double cc = this->a * oc.x * oc.x + this->b * oc.y * oc.y + this->c * oc.z * oc.z + this->d * oc.x * oc.y + this->e * oc.y * oc.z + this->f * oc.z * oc.x + this->g * oc.x + this->h * oc.y + this->i * oc.z + this->j;
    double determinant = bb * bb - 4 * aa * cc;
    if (determinant < 0) {
        return -1.0;
    }
    double t1 = (-bb + sqrt(determinant)) / (2 * aa);
    double t2 = (-bb - sqrt(determinant)) / (2 * aa);
    double tMin=t1<t2?t1:t2;
    double tMax=t1>t2?t1:t2;
    if(tMin>0 && insideGeneral(r->start + r->dir * tMin)){
        return tMin;
    }
    else if(tMax>0 && insideGeneral(r->start + r->dir * tMax)){
        return tMax;
    }
    return -1;
}
point calculateNormal(point p){
    point normal;
    normal.x = 2 * this->a * p.x + this->d * p.y + this->f * p.z + this->g;
    normal.y = 2 * this->b * p.y + this->d * p.x + this->e * p.z + this->h;
    normal.z = 2 * this->c * p.z + this->e * p.y + this->f * p.x + this->i;
    normal = normalize(normal);
    return normal;
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
class Floor:public Object{

void drawCheckers() {
    for (double i = reference_point.x; i < -reference_point.x; i+=length) {
        for (double j = reference_point.y; j < -reference_point.y; j+=length) {
            int x = (i - reference_point.x) / length;
            int y = (j - reference_point.y) / length;
            glPushMatrix();
            {
                glTranslatef(i, j, 0);
                drawCheckerBox(length, (x + y) % 2);
            }
            glPopMatrix();
        }
    }
}
public:
// double floorWidth,tileWidth;
Floor(double floorWidth,double tileWidth){
    reference_point=point(-floorWidth/2,-floorWidth/2,0);
    length=tileWidth;
}
void draw(){
    glPushMatrix();
    {
        drawCheckers();
    }
    glPopMatrix();
}
double intersect_helper(ray *r){
    
    if(r->dir.z==0){
        return -1.0;
    }
    double t=-r->start.z/r->dir.z;
    if(t<0){
        return -1.0;
    }
    if(r->start.x+t*r->dir.x<reference_point.x || r->start.x+t*r->dir.x>-reference_point.x|| r->start.y+t*r->dir.y<reference_point.y || r->start.y+t*r->dir.y>-reference_point.y){
        return -1.0;
    }
    return t;
}
point calculateNormal(point p){
    return point(0,0,1);
}

//overriding getColorAt
colour getColorAt(point p){
    colour c;
    int x = (p.x - reference_point.x) / length;
    int y = (p.y - reference_point.y) / length;
    if ((x + y) % 2 == 0) {
        c.r = 0;
        c.g = 0;
        c.b = 0;
        return c;
    } else {
        c.r = 1;
        c.g = 1;
        c.b = 1;
        return c;
    }
}
};

