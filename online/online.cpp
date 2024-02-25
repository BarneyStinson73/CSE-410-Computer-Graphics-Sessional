#include<bits/stdc++.h>
using namespace std;

#include<GL/glut.h>

void draw_axis(){
    glBegin(GL_LINES);
    glVertex2f(-0.5,0.0);
    glVertex2f(0.5,0.0);
    glVertex2f(0.0,-.5);
    glVertex2f(0.0,.5);    
    glEnd();
}
double angle_conversion(double a){
    float theta = a * 3.1416 / 180;
    return theta;
}
void draw_circle(int x, int y, double r) {
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < 360; i++) {
        float theta = i * 3.1416 / 180;
        glVertex3f(x + r * cos(theta), y + r * sin(theta),0.0);
    }
    glVertex3f(x+r*cos(angle_conversion(60)),y+r*sin(angle_conversion(60)),0.0);
    glVertex3f(x+r*cos(angle_conversion(60)),y+r*sin(angle_conversion(60)),-.5);
    glVertex3f(x+r*cos(angle_conversion(120)),y+r*sin(angle_conversion(120)),0.0);
    glVertex3f(x+r*cos(angle_conversion(120)),y+r*sin(angle_conversion(120)),-.5);
    glVertex3f(x+r*cos(angle_conversion(180)),y+r*sin(angle_conversion(180)),0);
    glVertex3f(x+r*cos(angle_conversion(180)),y+r*sin(angle_conversion(180)),-0.5);
    glEnd();
}
// void rotate(){
//     glVertex3f(0.0+)
// }
void draw_square(){
    glBegin(GL_POLYGON);
       glVertex2f(0,0);
       glVertex2f(.1,0);
       glVertex2f(.1,.1);
       glVertex2f(0,0.1);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    // glPushMatrix();
    // glTranslatef(-0.5, 0, 0);
    // draw_circle(0, 0, .3);
    // glColor3f(0, 1, 0);
    // draw_rotating_line(0.3);
    // glPopMatrix();
    // draw_sine_wave(0.3);
    draw_axis();
    draw_circle(0,0,.5);
    draw_square();
    glFlush();
}



// void draw_ride(){
//     glVertex3f()
// }
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Merry Go Round");
    glutDisplayFunc(display);
    // glutTimerFunc(10,idle,0);
    glutMainLoop();
    return 0;
}