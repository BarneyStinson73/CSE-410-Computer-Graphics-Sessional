#include<bits/stdc++.h>
using namespace std;

#include<GL/glut.h>

int angle = 0;

void draw_circle(int x, int y, double r) {
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < 360; i++) {
        float theta = i * 3.1416 / 180;
        glVertex2f(x + r * cos(theta), y + r * sin(theta));
    }
    glEnd();
}

void draw_rotating_line(double r) {
    glBegin(GL_LINES);
    {
        glVertex2f(0, 0);
        glVertex2f(r * cos(angle * 3.1416 / 180), r * sin(angle * 3.1416 / 180));
        // drawing a sine wave
    

        glVertex2f(r * cos(angle * 3.1416 / 180), r * sin(angle * 3.1416 / 180));
        glVertex2f(0.5,r * sin(angle * 3.1416 / 180));


    // glVertex2f(0, 0);
    // glVertex2f(.5 * cos(3.1416 / 4), .5 * sin(3.1416 / 4));
    }
    glEnd();
}

void draw_sine_wave(double radius) {
    glPushMatrix();
    
        for(int i=0;i<2000;i++){

            glTranslatef(0.001,0,0);
            glBegin(GL_LINE_STRIP);
            {
                glVertex2f(0,radius*sin(-(i-angle)*3.1416/180));
                glVertex2f(0.001,radius*sin(-(i-angle+1)*3.1416/180));
            }
            glEnd();
        }

    
    glPopMatrix();
}

void idle(int time) {
    angle++;
    angle%=360;
    glutPostRedisplay();
    glutTimerFunc(10,idle,0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0);
    glPushMatrix();
    glTranslatef(-0.5, 0, 0);
    draw_circle(0, 0, .3);
    glColor3f(0, 1, 0);
    draw_rotating_line(0.3);
    glPopMatrix();
    draw_sine_wave(0.3);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Circle");
    glutDisplayFunc(display);
    // glutTimerFunc(10,idle,0);
    glutMainLoop();
    return 0;
}