/***
   CTIS164 - Template Source Program
----------
STUDENT : Berra Turgut 
SECTION : 04
HOMEWORK: Homework #3 
----------
PROBLEMS: 
----------
ADDITIONAL FEATURES: 
- Speed: press up and down arrow keys to speed up and down 
- When press F1 , the light of the spacehishp turn on/off
***/


#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600


#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532


#define START 0

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height


bool light = false; // the light of the spaceship
int start = 0;
bool space = false;
int speed = 1;
double angle;


typedef struct {
    vec_t pos;
    float angle;
} player_t;

typedef struct {
    float angle;
    vec_t pos;
    vec_t vel;
} alien_t;


player_t P = { 0, 0 };
alien_t  E = { 0, 100 };

bool inside (int x, int y)
{
    return(x < 400 && x > -400 && y > -250 && y < 250);
}

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void vertex(vec_t t, vec_t Tr, double angle) //rotation
{
   double  xp = (t.x * cos(angle) - t.y * sin(angle)) + Tr.x;
   double yp = (t.x * sin(angle) + t.y * cos(angle)) + Tr.y;
   glVertex2d(xp, yp);

}

void background() {
    glColor3ub(5, 68, 53);
    glBegin(GL_POLYGON);
    glVertex2d(-400, 250);
    glVertex2d(400, 250);
    glColor3ub(2, 161, 123);
    glVertex2d(400, -250);
    glVertex2d(-400, -250);
    glEnd();

    glColor3ub(66, 73, 73);
    glRectf(-400, 230, 400, 250);
    glRectf(-400, -250, 400, -230);
    glColor3ub(97, 106, 107);
    glRectf(-400, 210, 400, 230);
}

void drawAlien (alien_t a) {

    double angle = E.angle * D2R;


    double cx = (cos(angle) - 0 * sin(angle)) + a.pos.x;
    double cy = (cos(angle) - 0 * sin(angle)) + a.pos.y;

    glColor3ub(5, 68, 16);
    glBegin(GL_QUADS);
    vertex({ -30 , 30 }, E.pos, angle);
    vertex({ -60 , 30 }, E.pos, angle);
    vertex({ 60 , 30 }, E.pos, angle);
    glEnd();


    glColor3ub(5, 68, 16);
    glBegin(GL_QUADS);
    vertex({ 10 , 0 }, E.pos, angle);
    vertex({ -20 , 30 }, E.pos, angle);
    vertex({ -10 , 0 }, E.pos, angle);
    vertex({ 20 , 30 }, E.pos, angle);
    glEnd();

    glColor3f(0.1, 1, 0.1);
    circle(cx, cy, 20);
    glColor3f(0, 1, 1);
    circle_wire(cx, cy, 20);
    glColor3f(0, 0, 0);
    circle(a.pos.x + 5, a.pos.y + 5, 5);
    circle(a.pos.x - 5, a.pos.y + 5, 5);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    vertex({ 10, -15 }, E.pos, angle);
    vertex({ -10, -15 }, E.pos, angle);

    glEnd();

}


void drawspaceship (player_t p) {


    glColor3f(1, 0.9, 1);
    circle(p.pos.x, 80 + p.pos.y - 50, 18);
    glColor3f(1, 1, 0);


    glColor3f(1, 0.5, 0.5);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(p.pos.x - 20, 20 + p.pos.y);
    glVertex2f(p.pos.x - 50, p.pos.y + 0);
    glVertex2f(p.pos.x + 20, p.pos.y + 20);
    glVertex2f(50 + p.pos.x, p.pos.y + 0);
    glEnd();

    glColor3f(1, 0.5, 0.5);
    glRectf(-20 + p.pos.x, 20 + p.pos.y, 20 + p.pos.x, 30 + p.pos.y);
    glColor3f(1, 1, 0);

    glEnd();
}

void lightofship(player_t p) {

    glColor3ub(180, 168, 145);
    glBegin(GL_QUADS);
    glVertex2f(-30 + p.pos.x, 0 + p.pos.y);
    glVertex2f(50 + p.pos.x, -100 + p.pos.y);

    glVertex2f(30 + p.pos.x, 0 + p.pos.y);
    glVertex2f(-50 + p.pos.x, -100 + p.pos.y);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(-100 + p.pos.x, -100 + p.pos.y);
    glVertex2f(0 + p.pos.x, 50 + p.pos.y);
    glVertex2f(100 + p.pos.x, -100 + p.pos.y);
    glEnd();
}


//
// To display onto window using OpenGL commands
//


void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (start == 0) {

        glColor3f(0, 1, 0); // green
        vprint((winWidth / 2) - 580, -(winHeight / 2) + 300, GLUT_BITMAP_9_BY_15, "<Click To Start>");
    }
    else
    {
        background(); 


        if (!space)
            vprint(-95, -195, GLUT_BITMAP_9_BY_15, "Press <Spacebar> to pause");
        else
            vprint(-95, -195, GLUT_BITMAP_9_BY_15, "Press <Spacebar> to restart");


        if (inside (P.pos.x, P.pos.y))
        {
            glColor3f(1, 1, 1);
            vprint(-400, -245, GLUT_BITMAP_8_BY_13, "Mouse is inside");
        }
        else
        {
            glColor3f(1, 1, 1);
            vprint(-400, -245, GLUT_BITMAP_8_BY_13, "Mouse is outside");
        }


        if (inside(P.pos.x, P.pos.y))
        {

            drawspaceship(P);
            drawAlien(E);


            if (light)
            {
                lightofship(P);
                drawspaceship(P);
                drawAlien(E);
            }
          
        }
        else
        {
            drawAlien(E);
        }

      
        //Name-Surname-Id
        glColor3f(0, 0, 1);
        vprint(-50, 235, GLUT_BITMAP_TIMES_ROMAN_24, "Berra Turgut");
        vprint(-25, 215, GLUT_BITMAP_9_BY_15, "21903363");
        vprint(-380, 215, GLUT_BITMAP_9_BY_15, "Speed = %d", speed);
        //angle 
        vprint(-300, 215, GLUT_BITMAP_8_BY_13, " Angle = %.0f", E.angle);


    }


    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);


    if (key == 32) {

        if (space == 0)
            space = 1;
        else
            space = 0;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.


    if (key == GLUT_KEY_F1) {
        light = !light;
    }

    if (key == GLUT_KEY_UP) {
        if (start == 1)
            if (speed != 5) {
                speed++;
            }

        up = true;
    }

    if (key == GLUT_KEY_DOWN)
        if (start == 1) {

            if (speed != 1) {
                speed--;
            }
            down = true;
        }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.

    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
        if (start == START)
            start = 1;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.

    P.pos.x = x - winWidth / 2;
    P.pos.y = winHeight / 2 - y;


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    //angle = angleV(E.pos)*D2R;

    if (start == 1)
    {

        if (!space)
            E.pos = addV(E.pos, E.vel);

       
        if (inside(P.pos.x, P.pos.y)) {
            E.vel = mulV(speed, unitV(subV(P.pos, E.pos)));
        }
        else
        {
            if (E.pos.x > 400 || E.pos.x < -400)
                E.vel.x = E.vel.x * (-1);

            if (E.pos.y > 250 || E.pos.y < -250)
                E.vel.y = E.vel.y * (-1);
        }

        E.angle = atan2((P.pos.y - E.pos.y), P.pos.x - E.pos.x) / (PI / 180);

        if (E.angle < 0)
        {
            E.angle = E.angle + 360;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("UFO chasing by Berra Turgut");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}