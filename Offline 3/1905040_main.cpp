#include<bits/stdc++.h>
using namespace std;

#include <iostream>
#include <fstream>
#include <sstream>
#include<GL/glut.h>
#include "1905040_classes.h"
#include "bitmap_image.hpp"

// point operator*(double constant, point const& right) {
//     return point(right.x*constant,right.y*constant,right.z*constant);
// }


vector<Object*> objects;
vector<PointLight> pointLights;
vector<SpotLight> spotLights;
cam_position cam;
int recursionLevel,pixelCount,shapeCount;
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
void capture();
double a=0.5;
// 1- look left, 2- look right, 3- look up, 4- look down, 5- tilt anti clockwise, 6- tilt clockwise
// up arrow- move forward, down arrow- move backward, left arrow- move left, right arrow- move right ,page up- move up, page down- move down
void keyboardListener(unsigned char key, int x, int y){
    cout<<key<<endl;
    switch(key){
        case '0':
            capture();
            break;
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

// void drawCheckerBox(double a, int color = 0) {
//   glBegin(GL_QUADS);
//   {
//     if (color == 0) {
//       glColor3f(0.0f, 0.0f, 0.0f); // Black
//     } else {
//       glColor3f(1.0f, 1.0f, 1.0f); // White
//     }
//     glVertex3f(0, 0, 0);
//     glVertex3f(0, a, 0);
//     glVertex3f(a, a, 0);
//     glVertex3f(a, 0, 0);
//   }
//   glEnd();
// }

// void drawCheckers(double a) {
//   for (int i = -40; i < 40; i++) {
//     for (int j = -40; j < 40; j++) {
//       glPushMatrix();
//       glTranslatef(i * a, j * a, 0);
//       drawCheckerBox(a, (i + j) % 2);
//       glPopMatrix();
//     }
//   }
// }

    double windowHeight = 800, windowWidth = 800;
void capture(){
//     initialize bitmap image and set background color
// planeDistance = (windowHeight/2.0) /
// tan(viewAngle/2.0)
// topleft = eye + l*planeDistance - r*windowWidth/2 +
// u*windowHeight/2
// du = windowWidth/imageWidth
// dv = windowHeight/imageHeight
// // Choose middle of the grid cell
// topleft = topleft + r*(0.5*du) - u*(0.5*dv)
// int nearest;
// double t, tMin;
// for i=1:imageWidth
// for j=1:imageHeight
// calculate curPixel using topleft,r,u,i,j,du,dv
// cast ray from eye to (curPixel-eye) direction
// double *color = new double[3]
// for each object, o in objects
// t = o.intersect(ray, dummyColor, 0)
// update t so that it stores min +ve value
// save the nearest object, on
// tmin = on->intersect(ray, color, 1)
// update image pixel (i,j)
// save image // The 1st image you capture after running the program
// should be named Output_11.bmp, the 2nd image you capture should be
// named Output_12.bmp and so on
    bitmap_image image(pixelCount, pixelCount);
    image.set_all_channels(0, 0, 0);
    double planeDistance = (windowHeight/2.0) / tan(DEG2RAD((60/2.0)));
    point up_vector=normalize(cam.up);
    point look_vector = normalize(cam.look - cam.position);
    point right_vector = look_vector.cross_product(up_vector);
    point topleft = cam.position + look_vector*planeDistance - right_vector*windowWidth/2 + up_vector*windowHeight/2;
    double du = 1.0*windowWidth/pixelCount;
    double dv = 1.0*windowHeight/pixelCount;
    topleft = topleft + right_vector*(0.5*du) - up_vector*(0.5*dv);
    for(int i=0;i<pixelCount;i++){
        for(int j=0;j<pixelCount;j++){
            point curPixel = topleft + right_vector*i*du - up_vector*j*dv;
            ray ray(cam.position,curPixel-cam.position);
            double tMin = DBL_MAX;
            int min=-1;
            Object* nearest = NULL;
            for(int k=0;k<objects.size();k++){
                double dummyColor[3];
                double t = objects[k]->intersect(&ray,dummyColor,0);
                if(t>0 && t<tMin){
                    tMin = t;
                    nearest = objects[k];
                    min=k;
                }
            }
            if(nearest!=NULL){
                double color[3];
                tMin = nearest->intersect(&ray,color,1);
                if(color[0]>1) color[0] = 1;
                if(color[1]>1) color[1] = 1;
                if(color[2]>1) color[2] = 1;
                image.set_pixel(i,j,color[0]*255,color[1]*255,color[2]*255);
            }
        }
    }
    image.save_image("Output.bmp");

}
void Display(void)
{

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
    // Floor floor(1000,2);
    // floor.draw();

    a = 1.0;
    for(int i=0;i<objects.size();i++){
        objects[i]->draw();
    }

    glFlush();
    glutSwapBuffers();

}
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1, 1, 5000.0);
}
void idle(){
    glutPostRedisplay();
}
int main(int argc, char** argv)
{
    
    
    std::ifstream inputFile("./scene.txt"); 
    
    if (inputFile.is_open()) {
        inputFile >> recursionLevel >> pixelCount;
        inputFile >> shapeCount;
        // cout<<recursionLevel<<" "<<pixelCount<<endl;
        // cout<<shapeCount<<endl;

        for(int i=0;i<shapeCount;i++){
            std::string shapeType;
            inputFile >> shapeType;
            if(shapeType=="triangle"){
                
                double x1,y1,z1,x2,y2,z2,x3,y3,z3;
                inputFile >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
                double r,g,b;
                inputFile >> r >> g >> b;
                double ambient,diffuse,specular,reflection;
                inputFile >> ambient >> diffuse >> specular >> reflection;
                double shininess;
                inputFile >> shininess;
                Object* triangle = new Triangle(point(x1,y1,z1),point(x2,y2,z2),point(x3,y3,z3));
                triangle->setColor(r,g,b);
                triangle->setCoEfficients(ambient,diffuse,specular,reflection);
                triangle->setShine(shininess);
                objects.push_back(triangle);
                cout<<"triangle in "<<i<<endl;
            }
            else if(shapeType=="sphere"){
                double x,y,z,r;
                inputFile >> x >> y >> z >> r;
                double red,green,blue;
                inputFile >> red >> green >> blue;
                double ambient,diffuse,specular,reflection;
                inputFile >> ambient >> diffuse >> specular >> reflection;
                double shininess;
                inputFile >> shininess;
                Object* sphere = new Sphere(point(x,y,z),r);
                sphere->setColor(red,green,blue);
                sphere->setCoEfficients(ambient,diffuse,specular,reflection);
                sphere->setShine(shininess);
                objects.push_back(sphere);
                cout<<"sphere in "<<i<<endl;
            }
            else if(shapeType=="general"){
                double A,B,C,D,E,F,G,H,I,J;
                inputFile >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
                double x,y,z;
                inputFile >> x >> y >> z;
                point center(x,y,z);
                double length, width, height;
                inputFile >> length >> width >> height;

                double red,green,blue;
                inputFile >> red >> green >> blue;
                double ambient,diffuse,specular,reflection;
                inputFile >> ambient >> diffuse >> specular >> reflection;
                double shininess;
                inputFile >> shininess;
                Object* general = new General(A,B,C,D,E,F,G,H,I,J,center,length,width,height);
                general->setColor(red,green,blue);
                general->setCoEfficients(ambient,diffuse,specular,reflection);
                general->setShine(shininess);
                objects.push_back(general);
                cout<<"general in "<<i<<endl;
            }
        }
        int lightCount;
        inputFile >> lightCount;
        for(int i=0;i<lightCount;i++){
            double x,y,z,r,g,b;
            inputFile >> x >> y >> z >> r >> g >> b;
            PointLight pointLight(point(x,y,z),r,g,b);
            pointLights.push_back(pointLight);
        }
        int spotLightCount;
        inputFile >> spotLightCount;
        for(int i=0;i<spotLightCount;i++){
            double x,y,z,r,g,b,dir_x,dir_y,dir_z,angle;
            inputFile >> x >> y >> z >> r >> g >> b >> dir_x >> dir_y >> dir_z >> angle;
            PointLight pointLight(point(x,y,z),r,g,b);
            SpotLight spotLight(pointLight,point(dir_x,dir_y,dir_z),angle);
            spotLights.push_back(spotLight);
        }
        Object *floor=new Floor(1000,20);
        floor->setCoEfficients(0.4,0.2,0.2,0.2);
        floor->setShine(5);
        objects.push_back(floor);
        cout<<"floor in "<<objects.size()<<endl;

    } 
    else {
        std::cerr << "Error opening file!" << std::endl;
    }

    inputFile.close();
    
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowWidth, windowHeight);
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