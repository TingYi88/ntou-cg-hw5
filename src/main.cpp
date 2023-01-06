/*-----------------------------------------------------------
 * An example program to draw a car
 *   Author: S.K. Ueng
 *   Date:  11/4/2001
 */
#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>
#include <stdbool.h>
#include "define.h"
#include"stb_image.h"

int image_width, image_height, nrChannels;
unsigned char* image_data;

/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);      /*set the background color BLACK */

    /*Clear the Depth & Color Buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */
    glEnable(GL_NORMALIZE);   /*Enable mornalization  */

    glEnable(GL_LIGHTING);    /*Enable lighting effects */
    glEnable(GL_LIGHT0);      /*Turn on light0 */
    glEnable(GL_LIGHT1);      /*Turn on light1 */
    glEnable(GL_LIGHT2);
    /*-----Define light0 ---------*/
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lit_exponent);

    /*-----Define light1 ---------*/
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lit_specular);

    /*-----Define light2 ---------*/
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lit_specular);

    /*-----Define some global lighting status -----*/
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); /* local viewer */
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); /*global ambient*/

    /*-----Create Texture -----*/
    make_check();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenTextures(3, textName);
    glBindTexture(GL_TEXTURE_2D, textName[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, checkboard);

    /*-----Create another texture ----*/
    brick_pattern();
    glBindTexture(GL_TEXTURE_2D, textName[1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, brick);

    /*-----Create another texture ----*/
    Create_Texture_Star();
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
    //glGenTextures(1, TextureID);
    glBindTexture(GL_TEXTURE_2D, textName[2]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, star);

    /*-----Enable face culling -----*/
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    /*---Create quadratic objects---*/
    if (sphere == NULL) {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
        gluQuadricNormals(sphere, GLU_SMOOTH);
    }

    if (cylind == NULL) {
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }

    if (disk == NULL) {
        disk = gluNewQuadric();
        gluQuadricDrawStyle(disk, GLU_FILL);
        gluQuadricNormals(disk, GLU_SMOOTH);
    }

    /*---- Compute cos(5.0) and sin(5.0) ----*/
    cv = cos(5.0 * PI / 180.0);
    sv = sin(5.0 * PI / 180.0);
    /*---- Copy eye position ---*/
    eye[0] = Eye[0];
    eye[1] = Eye[1];
    eye[2] = Eye[2];
}

/*--------------------------------------------------------
 * Procedure to draw a 1x1x1 cube. The cube is within
 * (-0.5,-0.5,-0.5) ~ (0.5,0.5,0.5)
 */
void draw_cube()
{
    int    i;
    for (i = 0; i < 6; i++) {
        glBegin(GL_POLYGON);  /* Draw the face */
        glVertex3fv(points[face[i][0]]);
        glVertex3fv(points[face[i][1]]);
        glVertex3fv(points[face[i][2]]);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
}

/*-------------------------------------------------------
 * Display callback func. This func. draws three
 * cubes at proper places to simulate a solar system.
 */
void display()
{
    /*Clear previous frame and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*----Define the current eye position and the eye-coordinate system---*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2],
        eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2],
        u[1][0], u[1][1], u[1][2]);
    glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
    compute_ab_axes();

    switch (style) {
    case 0:
        make_view(4);//perspective projection
        make_projection(4);
        glViewport(width / 2, 0, width / 2, height / 2);
        draw_scene();
        
        make_view(1);//x axis parallel viewing
        make_projection(1);
        glViewport(0, height / 2, width / 2, height / 2);
        draw_scene();

        make_view(2);//y axis parallel viewing
        glViewport(width / 2, height / 2, width / 2, height / 2);
        draw_scene();

        make_view(3);//z axis parallel viewing
        glViewport(0, 0, width / 2, height / 2);
        draw_scene();
        break;

    case 1:          //perspective projection
        make_view(1);//x axis parallel viewing
        make_projection(1);
        glViewport(0, 0, width, height);
        draw_scene();
        break;

    case 2:
        make_view(2);//y axis parallel viewing
        make_projection(2);
        glViewport(0, 0, width , height);
        draw_scene();
        break;

    case 3:
        make_view(3);//z axis parallel viewing
        make_projection(3);

        glViewport(0, 0, width, height);
        draw_scene();
        break;

    case 4:          //perspective projection
        make_view(4);
        make_projection(4);
        glViewport(0, 0, width, height);
        draw_scene();
        break;

    case 6:
        glViewport(0, 0, width, height);
        make_view(6);
        draw_scene();
        break;
    }
    
    glPushMatrix();
    glLoadIdentity();
    /*----position light1 (fixed) in eye coord sys ---*/
    glLightfv(GL_LIGHT1, GL_POSITION, lit1_position);  /*fixed position in eye space---*/
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lit1_direction);
    glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, &lit1_cutoff);

    /*---draw the light source ---*/
    //glTranslatef(-10.0, 0.0, -3.5);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glColor3f(0.0, 0.0, 0.80);
    glutSolidSphere(1.8, 12, 12);
    glPopMatrix();
    glEnable(GL_CULL_FACE);

    /*-------Draw light source as a red cube----*/
    glPushMatrix();
        glTranslatef(lit_position[0], lit_position[1], lit_position[2]);
        glColor3f(1.0, 0.0, 0.0);
        glutWireSphere(1.0, 12, 12);
    glPopMatrix();

    glEnable(GL_LIGHTING);      /* Turn on lighting */
    /*----Redefine position and direction of light0-----*/
    glLightfv(GL_LIGHT0, GL_POSITION, lit_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lit_direction);
 //   glPopMatrix();


    //glDisable(GL_TEXTURE_2D);

    /*-------Swap the back buffer to the front --------*/
    glutSwapBuffers();
    return;
}

void Reshape(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (double)width / (double)height, 0.5, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*----------------------------------------------------------
 * Procedure to make a checkboard texture.
 */
void make_check()
{
    int   i, j, c;

    for (i = 0; i < TSIZE; i++)
        for (j = 0; j < TSIZE; j++) {
            if (j > i / 2 && j <= 64 - i / 2) c = 255;
            else c = 0;
            checkboard[i][j][0] = c / 8;
            checkboard[i][j][1] = c / 2;
            checkboard[i][j][2] = c / 4;
            if (c == 255) checkboard[i][j][3] = 255;
            else checkboard[i][j][3] = 0;
        }
    //Generate trunk
    for (i = 0; i < TSIZE / 3; i++) {
        for (j = 0; j < TSIZE / 2 - 4; j++) checkboard[i][j][3] = 0;
        for (j = TSIZE / 2 + 4; j < TSIZE; j++) checkboard[i][j][3] = 0;
    }
}

/*----------------------------------------------------------
 * Procedure to maKe a brick wall texture map
 */
void brick_pattern()
{
    int  y, x, i;

    for (y = 0; y < 12; y++) {    /*----create the first row of bricks----*/
        for (x = 0; x < 2; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 2; x < 30; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 30; x < 34; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 34; x < 62; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 62; x < 64; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }
    }

    for (y = 12; y < 16; y++)     /*--create the cement between two breick layers */
        for (x = 0; x < 64; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

    for (y = 16; y < 28; y++) {    /*--create the 2nd layer of bricks---*/
        for (x = 0; x < 14; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 14; x < 18; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 18; x < 46; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 46; x < 50; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 50; x < 64; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = 0;
            brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }
    }

    for (y = 28; y < 32; y++) /*---add another layer of cement----*/
        for (x = 0; x < 64; x++) {
            brick[y][x][0] = 140;
            brick[y][x][1] = 140;
            brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

    for (y = 32; y < 64; y++)
        for (x = 0; x < 64; x++)
            for (i = 0; i < 4; i++) brick[y][x][i] = brick[y - 32][x][i];
}

/*-------------------------------------------------------
 * Procedure to compute the a[] and b[] axes of billboard
 * after eye parameters are specified.
 */
void compute_ab_axes(void)
{
    float  w0, w2;
    double len;

    /*----Get w0 and w2 from the modelview matrix mtx[] ---*/
    w0 = mtx[2]; w2 = mtx[10];

    len = sqrt(w0 * w0 + w2 * w2);
    /*---- Define the a and b axes for billboards ----*/
    b[0] = 0.0; b[1] = 1.0; b[2] = 0.0;
    a[0] = w2 / len; a[1] = 0.0; a[2] = -w0 / len;
}

/*--------------------------------------------------------
 * Procedure to draw a billboard, center=[x,z], width=w,
 * height = h;
 */
void draw_billboard(float x, float z, float w, float h)
{
    float  v0[3], v1[3], v2[3], v3[3];

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    /*----Compute the 4 vertices of the billboard ----*/
    v0[0] = x - (w / 2) * a[0]; v0[1] = 0.0; v0[2] = z - (w / 2) * a[2];
    v1[0] = x + (w / 2) * a[0]; v1[1] = 0.0; v1[2] = z + (w / 2) * a[2];
    v2[0] = x + (w / 2) * a[0]; v2[1] = h; v2[2] = z + (w / 2) * a[2];
    v3[0] = x - (w / 2) * a[0]; v3[1] = h; v3[2] = z - (w / 2) * a[2];

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3fv(v0);
    glTexCoord2f(1.0, 0.0); glVertex3fv(v1);
    glTexCoord2f(1.0, 1.0); glVertex3fv(v2);
    glTexCoord2f(0.0, 1.0); glVertex3fv(v3);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

void Initial()
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glGenTextures(4, TextureID);

    image_data = stbi_load("images/N.bmp", &image_width, &image_height, &nrChannels, 0);
    if (image_data == NULL) {
        fprintf(stderr, "Failed to read the image!");
        exit(EXIT_FAILURE);
    }
    //Create_Texture_Circle();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    }
    /*Create_Texture_Circle();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, circle);*/

    Create_Texture_Star();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, star);

    Create_Texture_Land();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, land);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lit_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lit_direction);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, light_attenuation);
    glMaterialf(GL_FRONT, GL_SHININESS, material_shininess);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_SPECULAR);
    glColor3f(1.0, 1.0, 1.0);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    printf("Press space to change texture map\n");
    printf("Press q to exit program\n");
}

void my_quit(unsigned char key, int ix, int iy)
{
    int    i;
    float  x[3]={0.0}, y[3] = { 0.0 }, z[3] = { 0.0 };

    /*--- Control the robot. ----*/
    //¸I¼²´ú¸Õ
    if (position[0] >= -4.5 && position[0] <= 15 && position[2] >=9 && position[2] <= 27 && position[1] < 9.0) {
        position[0] -= Step;
        position[2] += Step;
    }
    else if (position[0] >= 0 && position[0] < 6 && position[2] >= 6 && position[2] <= 27 && position[1] < 9.0) {
        position[0] += Step;
        position[2] -= Step;
    }
    else if (position[0] >= 6 && position[0] <= 12 && position[2] >= 6 && position[2] <= 27 && position[1] < 9.0) {
        position[0] += Step * 2;
        position[2] -= Step * 2;
    }

    /*control the dead condition*/
    if ((p86_diff(position[0], p86[0])<=20) && (p86_diff(position[2], p86[2]) <= 10) ) 
    {
        printf("You are dead!!!\n");
        fx = position[0]-0, fy = position[1]-0, fz = position[2]-0;
        dx = 0, dy = 2, dz = 0;
        hug = 1;
        if (left_arm_hug_ang == 0 && left_arm_hug_ang > -20) {
            left_arm_hug_ang -= 50;
        }
        if (right_arm_hug_ang == 0 && right_arm_hug_ang > -20) {
            right_arm_hug_ang -= 50;
        }
        glutTimerFunc(1500, makedelay, 1500);
        printf("left_arm_hug_ang= %f, right_arm_hug_ang= %f\n", left_arm_hug_ang, right_arm_hug_ang);
    }

    if (key != 'Q' || key != 'q') {
        position_direction(key);//the robot's motion
        p86_direction(key);//the 86's motion
        display();

        /*------transform the EYE coordinate system ------*/
        if (key == 'P') {
            eyeDy += 0.5;       /* move down */
            for (i = 0; i < 3; i++) eye[i] -= 0.5 * u[1][i];
        }
        else if (key == 'p') {
            eyeDy += -0.5;       /* move up */
            for (i = 0; i < 3; i++) eye[i] += 0.5 * u[1][i];
        }
        else if (key == '7') {
            eyeDx += -0.5;       /* move left */
            for (i = 0; i < 3; i++) eye[i] -= 0.5 * u[0][i];
        }
        else if (key == '9') {
            eyeDx += 0.5;        /* move right */
            for (i = 0; i < 3; i++) eye[i] += 0.5 * u[0][i];
        }
        else if (key == '8') {
            eyeDz += 0.5;        /*move forward */
            for (i = 0; i < 3; i++) eye[i] -= 0.5 * u[2][i];
        }
        else if (key == '5') {
            eyeDz += -0.5;       /* move backward */
            for (i = 0; i < 3; i++) eye[i] += 0.5 * u[2][i];
        }
        else if (key == 'g') {   /* pitching */
            eyeAngx += 5.0;
            pitch_ang -= 5.0;
            if (eyeAngx > 360.0) eyeAngx -= 360.0;
            y[0] = u[1][0] * cv - u[2][0] * sv;
            y[1] = u[1][1] * cv - u[2][1] * sv;
            y[2] = u[1][2] * cv - u[2][2] * sv;

            z[0] = u[2][0] * cv + u[1][0] * sv;
            z[1] = u[2][1] * cv + u[1][1] * sv;
            z[2] = u[2][2] * cv + u[1][2] * sv;

            for (i = 0; i < 3; i++) {
                u[1][i] = y[i];
                u[2][i] = z[i];
            }
        }
        else if (key == 'G') {
            eyeAngx += -5.0;
            pitch_ang += 5.0;
            if (eyeAngx < 0.0) eyeAngx += 360.0;
            y[0] = u[1][0] * cv + u[2][0] * sv;
            y[1] = u[1][1] * cv + u[2][1] * sv;
            y[2] = u[1][2] * cv + u[2][2] * sv;

            z[0] = u[2][0] * cv - u[1][0] * sv;
            z[1] = u[2][1] * cv - u[1][1] * sv;
            z[2] = u[2][2] * cv - u[1][2] * sv;

            for (i = 0; i < 3; i++) {
                u[1][i] = y[i];
                u[2][i] = z[i];
            }
        }
        else if (key == 'h') {            /* heading */
            eyeAngy += 5.0;
            head_ang += 5.0;
            if (eyeAngy > 360.0) eyeAngy -= 360.0;
            for (i = 0; i < 3; i++) {
                x[i] = cv * u[0][i] - sv * u[2][i];
                z[i] = sv * u[0][i] + cv * u[2][i];
            }
            for (i = 0; i < 3; i++) {
                u[0][i] = x[i];
                u[2][i] = z[i];
            }
        }
        else if (key == 'H') {
            eyeAngy += -5.0;
            head_ang -= 5.0;
            if (eyeAngy < 0.0) eyeAngy += 360.0;
            for (i = 0; i < 3; i++) {
                x[i] = cv * u[0][i] + sv * u[2][i];
                z[i] = -sv * u[0][i] + cv * u[2][i];
            }
            for (i = 0; i < 3; i++) {
                u[0][i] = x[i];
                u[2][i] = z[i];
            }
        }
        else if (key == 'o') {            /* rolling */
            eyeAngz += 5.0;
            roll_ang += 5.0;
            if (eyeAngz > 360.0) eyeAngz -= 360.0;
            for (i = 0; i < 3; i++) {
                x[i] = cv * u[0][i] - sv * u[1][i];
                y[i] = sv * u[0][i] + cv * u[1][i];
            }
            for (i = 0; i < 3; i++) {
                u[0][i] = x[i];
                u[1][i] = y[i];
            }
        }
        else if (key == 'O') {
            eyeAngz += -5.0;
            roll_ang -= 5.0;
            if (eyeAngz < 0.0) eyeAngz += 360.0;
            for (i = 0; i < 3; i++) {
                x[i] = cv * u[0][i] + sv * u[1][i];
                y[i] = -sv * u[0][i] + cv * u[1][i];
            }
            for (i = 0; i < 3; i++) {
                u[0][i] = x[i];
                u[1][i] = y[i];
            }
        }
        else if (key == 'e') {
            fov += zoom;
            othZoom += zoom;
        }
        else if (key == 'E') {
            fov -= zoom;
            othZoom -= zoom;
        }
        else if (key == '0') {
            style = 0;
        }
        else if (key == '1') {
            style = 1;
        }
        else if (key == '2') {
            style = 2;
        }
        else if (key == '3') {
            style = 3;
        }

        else if (key == '4') {
            style = 4;
        }
        else if (key == '6') {
            style = 6;
        }
        else if (key == '<') {
            glEnable(GL_LIGHT0);
            glShadeModel(GL_SMOOTH);
        }
        else if (key == '>') 
            glDisable(GL_LIGHT0);
        else if (key == ',') {
            glEnable(GL_LIGHT1); 
            glShadeModel(GL_SMOOTH);
        } 
        else if (key == '.')
            glDisable(GL_LIGHT1);
        else if (key == 'i') {
            glEnable(GL_LIGHT2); 
            glShadeModel(GL_SMOOTH);
        }
        else if (key == 'y') {
            lit_diffuse[0] = 1; lit_diffuse[1] = 0; lit_diffuse[2] = 0;
            lit1_diffuse[0] = 1; lit1_diffuse[1] = 0; lit1_diffuse[2] = 0;
            lit2_diffuse[0] = 1; lit2_diffuse[1] = 0; lit2_diffuse[2] = 0;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
        }
        else if (key == 't') {
            //glDisable(GL_LIGHT0); glDisable(GL_LIGHT1); glDisable(GL_LIGHT2);
            lit_diffuse[0] = 0; lit_diffuse[1] = 0; lit_diffuse[2] = 1;
            lit1_diffuse[0] = 0; lit1_diffuse[1] = 0; lit1_diffuse[2] = 1;
            lit2_diffuse[0] = 0; lit2_diffuse[1] = 0; lit2_diffuse[2] = 1;
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
        }
        else if (key == 'Y') {
        glEnable(GL_LIGHT0); glEnable(GL_LIGHT1); glEnable(GL_LIGHT2);
        lit_diffuse[0] = 1; lit_diffuse[1] = 1; lit_diffuse[2] = 1;
        lit1_diffuse[0] = 1; lit1_diffuse[1] = 1; lit1_diffuse[2] = 1;
        lit2_diffuse[0] = 1; lit2_diffuse[1] = 1; lit2_diffuse[2] = 1;
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
        }
        else if (key == 'I')
            glDisable(GL_LIGHT2);
        else if (key == 'z') {
        if (lit_diffuse[0] > 0) {
                lit_diffuse[0] -= intensity; lit_diffuse[1] -= intensity; lit_diffuse[2] -= intensity;
                lit1_diffuse[0] -= intensity; lit1_diffuse[1] -= intensity; lit1_diffuse[2] -= intensity;
                lit2_diffuse[0] -= intensity; lit2_diffuse[1] -= intensity; lit2_diffuse[2] -= intensity;
            }
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
        }
        else if (key == 'Z') {
        if (lit_diffuse[0] < 1) {
            lit_diffuse[0] += intensity; lit_diffuse[1] += intensity; lit_diffuse[2] += intensity;
            lit1_diffuse[0] += intensity; lit1_diffuse[1] += intensity; lit1_diffuse[2] += intensity;
            lit2_diffuse[0] += intensity; lit2_diffuse[1] += intensity; lit2_diffuse[2] += intensity;
        }
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
        }
        else if (key == 'x') {
            if (lit1_cutoff > 0)  lit1_cutoff -= cutoff;
            else lit1_cutoff -= 0;
        }
        else if (key == 'X') {
            if(lit1_cutoff < 90)  lit1_cutoff += cutoff;
            else lit1_cutoff += 0;
        }
        else if (key == 'A') {
            select = (select + 1) % 3;
        }
        else if (key == 'k') cube_angle -= 10.0;
        else if (key == 'K') cube_angle += 10.0;
        if (cube_angle > 36.0) 
            cube_angle -= 360.0;
        else if (cube_angle < (-360.0)) 
            cube_angle += 360.0;
        /*---Rotate light position by SPACE key ----*/
        if (key == ' ') 
            lit_angle += 5.0;

    }
    display();
}

/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callback func's.
 */
void main(int argc, char** argv)
{
    /*-----Initialize the GLUT environment-------*/
    glutInit(&argc, argv);

    /*-----Depth buffer is used, be careful !!!----*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(width, height);
    glutCreateWindow("robot_hw5");
    Initial();
    myinit();      /*---Initialize other state varibales----*/

    /*----Associate callback func's whith events------*/
    
    glutIdleFunc(idle_func);/* Add idle event */
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(my_quit);
    glutDisplayFunc(display);
    glutMainLoop();
}