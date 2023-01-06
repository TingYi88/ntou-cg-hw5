#define  PI   3.141592653
#define Step  1.5 // walk unit per path
#define MaxSwingDegree 50.0
#define MaxRacingRate 2.0
#define   TSIZE  64   /* define texture dimension */

/*--- Define projection method ---*/
#define  PERSPECTIVE   1
#define  PARALLEL      0
int     project_type = PARALLEL;

void myinit();
void Draw_UnitCube(int index);
void draw_cube();
void draw_floor(int);

void draw_axes();
void display();
void my_quit(unsigned char key, int x, int y);
void idle_func();
void compute_ab_axes(void);
void make_check();

/*-----Define window size----*/
int width = 1000, height = 700, select = 0;
unsigned int TextureID[4];
unsigned char circle[64][64][4];
unsigned char waterwave[64][64][4];
unsigned char star[64][64][4];
unsigned char land[64][64][4];

/*-----Define a unit box--------*/
/* Vertices of the box */
float  points[][3] = {
                      {-0.5, -0.5, -0.5},
                      {0.5, -0.5, -0.5},
                      {0.5, 0.5, -0.5},
                      {-0.5, 0.5, -0.5},
                      {-0.5, -0.5, 0.5},
                      {0.5, -0.5, 0.5},
                      {0.5, 0.5, 0.5},
                      {-0.5, 0.5, 0.5}
};

/* face of box, each face composing of 4 vertices */
int    face[][4] = {
                    {0, 3, 2, 1},
                    {0, 1, 5, 4},
                    {1, 2, 6, 5},
                    {4, 5, 6, 7},
                    {2, 3, 7, 6},
                    {0, 4, 7, 3}
};

//body's color
double  colors[6][3] = {
                     {0.91,  0.91,    0.94},
                     {0.0,  0.0,    0.0},
                     {0.91,  0.91,    0.94},

                     {0.91,  0.91,    0.94},
                     {0.0,  0.0,    0.0},
                     {0.91,  0.91,    0.94},
};

/* index of the box faces */
int    cube[6] = { 0, 1, 2, 3, 4, 5 };

/*----Define normals of faces ----*/
float  normal[][4] = { {0.0, 0.0, -1.0}, {0.0, -1.0, 0.0}, {1.0, 0.0, 0.0},
              {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {-1.0, 0.0, 0.0} };

float vertex[][3] = { { -0.5 ,  -0.5 , -0.5 } , { 0.5 , -0.5 , -0.5 } , { 0.5 , 0.5 , -0.5 } , { -0.5 , 0.5 , -0.5 } , { -0.5 , -0.5 , 0.5 } , { 0.5 , -0.5 , 0.5 } , { 0.5 , 0.5 , 0.5 } , { -0.5 , 0.5 , 0.5 } };

/*----Define material properties for cube -----*/
float  mat_ambient[] = { 0.0, 0.1, 0.06, 1.0 };//cyan plastic
float  mat_diffuse[] = { 0.0, 0.50980392, 0.50980392, 1.0 };
float  mat_specular[] = { 0.50196078, 0.50196078, 0.50196078, 1.0 };
float  mat_shininess = 0.25;
float  flr_diffuse[] = { 0.54, 0.89, 0.63, 1.0 };
float  flr_ambient[] = { 0.635, 0.6225, 0.1575, 1.0 };
float  flr_specular[] = { 0.0, 0.0, 0.0, 0.0 };

/*----Define light source properties -------*/
float material_shininess = 32.0;
float  lit_position[] = { -11.9, 0.0, 4.45, 1.0 };//robot spot light
float  lit_direction[] = { -1.0, -1.0, 1.0, 0.0 };
float  lit_diffuse[] = { 0.8, 0.4, 0.4, 1.0 };
float  lit_specular[] = { 0.8, 0.8, 0.8, 1.0 };
float  lit_cutoff = 90.0;
float  lit_exponent = 8.0;
float  intensity = 0.2;
float  cutoff = 15.0;
double light_attenuation = 0.8;

float  lit1_position[] = { -20.0, 0.0, -3.5, 1.0 };//fixed point light
float  lit1_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
float  lit1_cutoff = 90.0;
float  lit1_exponent = 8.0;
float  lit1_direction[] = { -1.0, -1.0, 1.0, 0.0 };

float  lit2_position[] = { -10, 25.0, 30.0, 1.0 };//directional light
float  lit2_diffuse[] = { 0.7, 0.7, 0.0, 1.0 };

float  global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

/*-Declare GLU quadric objects, sphere, cylinder, and disk --*/
GLUquadricObj* sphere = NULL, * cylind = NULL, * disk;

/*-Declare car position, orientation--*/
float self_ang = -90.0;
float glob_ang = 0.0;
float left_leg_ang = 0;
float right_leg_ang = 0;

float left_arm_ang = 0;
float right_arm_ang = 0;

float left_joint_ang = 0;
float right_joint_ang = 0;

float left_arm_hug_ang = 0;
float right_arm_hug_ang = 0;

/*----Some global variables for transformation------*/
float  lit_angle = 0.0;

float  position[3] = { 0.0, 0.0, 0.0 };
float  p86[3] = { 30.0, 4.0, -15.0 };

int dir = 0;
int obstacle = 0;
int motion = 0;
int jump = 0;
int hug = 0;

float cx = -3.0, cy = 6.0, cz = 6.0;//camera's position
float fx = position[0], fy = position[1], fz = position[2];//camara focus on position
float dx = 0.0, dy = 1.0, dz = 0.0;

/*-----Translation and rotations of eye coordinate system---*/
float   eyeDx = 0.0, eyeDy = 0.0, eyeDz = 0.0;
float   eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
double  Eye[3] = { 0.0, 0.0, 30.0 }, Focus[3] = { 0.0, 0.0, 0.0 },
Vup[3] = { 0.0, 1.0, 0.0 };

float   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };
float   eye[3];
float   cv, sv; /* cos(5.0) and sin(5.0) */

double pitch_ang = 0;   //x
double head_ang = 0;    //y
double roll_ang = 0;    //z

/* gluPerspective(fovy, aspect, zNear, zFar)
fov:檢視角度的欄位，以度為單位，以 Y 方向為單位。
aspect:決定 X 方向檢視欄位的外觀比例。 外觀比例是 x (寬度) 與 y (高度) 的比例。
zNear:從檢視器到接近裁剪平面的距離 (一律為正數) 。
zFar:從檢視器到遠裁剪平面的距離 (一律為正數) 。

glFrustum(left, right, bottom, top, zNear, zFar);
left:左垂直裁剪平面的座標。
right:右垂直裁剪平面的座標。
bottom:底端水準裁剪平面的座標。
top:底端水準裁剪平面的座標。
zNear:同Perspective。
zFar:同Perspective。
*/
double zoom = 0.5;
double othZoom = 0.0;
double fov = 90.0;
double aspectRatio = (double)width / (double)height;
double zNear = 2.5;
double zFar = 50.0;
double rFov = fov * PI / 180.0;

/*-----Drawing style
 0:4-windows, 1:x direction, 2:y direction, 3:z-dirtection, 4:perspective
 */
int style = 0;

/*---- the axes of billboard ----*/
float  a[3], b[3];
float  cube_angle = 0.0;
/*-----Create image space for textures -----*/
unsigned char  checkboard[TSIZE][TSIZE][4];   /* checkboard textures */
unsigned char  brick[TSIZE][TSIZE][4];        /* brick wall textures */
unsigned int   textName[3];                   /* declare two texture maps*/
float    mtx[16];     /* the modelview matrix */

void draw_scene(void);

float norm2(float v[])
{
    double  sum;
    float   temp;

    sum = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    temp = (float)sqrt(sum);
    return temp;
}

void right_palm()
{
    glPushMatrix();
    glTranslatef(3.0, 0.0, 0.6);
    glRotatef(30, 0.0, 1.0, 0.0);
    glScalef(1.0, 2.0, 2.0);    /* draw it */
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4.0, 0.0, 1.6);
    glRotatef(-80, 0.0, 1.0, 0.0);
    glScalef(1.0, 1.9, 2.0);    /* draw it */
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.0, 0.0, -0.6);
    glRotatef(-30, 0.0, 1.0, 0.0);
    glScalef(1.0, 2.0, 2.0);    /* draw it */
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4.0, 0.0, -1.6);
    glRotatef(-80, 0.0, 1.0, 0.0);
    glScalef(1.0, 1.9, 2.0);    /* draw it */
    draw_cube();
    glPopMatrix();
}

void left_palm()
{
    glPushMatrix();
    glTranslatef(-3.0, 0.0, -0.6);
    glRotatef(30, 0.0, 1.0, 0.0);
    glScalef(1.0, 2.0, 2.0);    /* draw it */
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-4.0, 0.0, -1.6);
    glRotatef(-80, 0.0, 1.0, 0.0);
    glScalef(1.0, 1.9, 2.0);    /* draw it */
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3.0, 0.0, 0.6);
    glRotatef(-30, 0.0, 1.0, 0.0);
    glScalef(1.0, 2.0, 2.0);    /* draw it */
    draw_cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-4.0, 0.0, 1.6);
    glRotatef(-80, 0.0, 1.0, 0.0);
    glScalef(1.0, 1.9, 2.0);    /* draw it */
    draw_cube();
    glPopMatrix();
}

void draw_body()
{
    /*-----Define some material properties shared by every one ---*/
    float  body_ambient[] = { 0.25, 0.25, 0.25, 1.0 };//chrome
    float  body_diffuse[] = { 0.4, 0.4, 0.4, 1.0 };
    float  body_specular[] = { 0.774597, 0.774597, 0.774597, 1.0 };
    float  body_shininess = 0.6;
    glMaterialfv(GL_FRONT, GL_AMBIENT, body_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, body_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, body_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, body_shininess);

    /*-------Draw the body which is a cube----*/
    //lift the body(0,20,0);
    glTranslatef(position[0], position[1] + 20, position[2]);
    //after every turn the body can keep up the direction just moved
    if (dir == 1) {
        glRotatef(self_ang, 0.0, 1.0, 0.0);
    }
    else if (dir == 2) {
        glRotatef(2 * self_ang, 0.0, 1.0, 0.0);
    }
    else if (dir == 3) {
        glRotatef(3 * self_ang, 0.0, 1.0, 0.0);
    }

    //(y->x,x->-y,z->z) (y,-x,z);
    glRotatef(self_ang, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(20.0, 4.0, 3.0);    /* draw torso */
    draw_cube();
    glPopMatrix();

    glColor3f(0, 0, 1);

    /*-------Draw the head -----*/
    glPushMatrix();
    //turn back to x,y,z
    glRotatef(90, 0.0, 1.0, 0.0);
    glColor3f(0.87, 0.87, 0.89);
    //lift the head
    glTranslatef(0.0, 0.0, -12.0);
    gluSphere(sphere, 4.0,   /* radius=4.0 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 12 stacks */
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.72, 0.72, 0.75);// draw the sensor(which equals to the robot's eye)
    glTranslatef(-12.0, 0.0, 0.0);
    gluCylinder(cylind, 1, 1, 4.5, 12, 12);
    glPopMatrix();

    /*-------left arm -----*/
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(-5.0, -3.0, 0.0);
    glRotatef(160, 0.0, 1.0, 0.0);
    glRotatef(105, 0.0, 0.0, 1.0);
    glRotatef(left_arm_ang, 1.0, 1.0, 0.0); ///control the swing
    glRotatef(left_arm_hug_ang, 0.0, 1.0, 0.0);//control the hug and the gesture
    glTranslatef(-2.0, 0.0, 0.0);

    glPushMatrix();
    glScalef(5.0, 2.0, 2.0);//draw the left arm
    draw_cube();
    glPopMatrix();

    //left fore arm 
    glTranslatef(-5.0, 0.0, 0.0);
    glRotatef(-80, 0.0, 1.0, 0.0);
    glTranslatef(-2.0, 0.0, -2.0);

    glPushMatrix();
    glScalef(6.0, 2.0, 2.0); //draw the right arm
    draw_cube();
    glPopMatrix();

    //left palm
    left_palm();

    glPopMatrix();

    /*-------Draw the right arm-----*/
    glPushMatrix();
    //y,x,z
    glTranslatef(-5.0, 3.0, 0.0);
    glRotatef(-20, 0.0, 1.0, 0.0);
    glRotatef(100, 0.0, 0.0, 1.0);
    glRotatef(right_arm_ang, 1.0, 1.0, 0.0);///control the swing
    glRotatef(right_arm_hug_ang, 0.0, 1.0, 0.0);//control the hug and the gesture
    glTranslatef(+2.0, 0.0, 0.0);

    glPushMatrix();
    glScalef(5.0, 2.0, 2.0);
    draw_cube();
    glPopMatrix();

    /*-------Draw the right fore arm-----*/
    //y,x,z
    glTranslatef(5.0, 0.0, 0.0);
    glRotatef(-80.0, 0.0, 1.0, 0.0);
    glTranslatef(2.0, 0.0, 2.0);

    glPushMatrix();
    glScalef(6.0, 2.0, 2.0);
    draw_cube();
    glPopMatrix();

    //right palm
    right_palm();

    glPopMatrix();

    /*------Draw left leg ----*/
    glColor3f(0.0, 0.0, 0.0);
    glPushMatrix();
    glTranslatef(8.0, -3.0, 0.0); /* Go to left leg position */
    glRotatef(20, 0.0, 1.0, 0.0);
    glRotatef(left_leg_ang, 0.0, 1.0, 0.0);
    glTranslatef(2.0, 0.0, 0.0); /* back to hip joiont */

    glPushMatrix();
    glScalef(6.8, 2.0, 2.0); //draw it
    draw_cube();
    glPopMatrix();

    //left shinn
    //-y,x,z
    glTranslatef(0.0, 0.0, 0.0);
    glRotatef(150, 0.0, 1.0, 0.0);
    glRotatef(left_joint_ang, 0.0, 1.0, 0.0); //control the movement
    glTranslatef(-4.5, 0.0, 0.0);

    glPushMatrix();
    glScalef(7.0, 1.9, 1.9);    /* draw it */
    draw_cube();
    glPopMatrix();

    //left foot
    glPushMatrix();
    glTranslatef(-3.0, 0.0, -0.6);
    glScalef(1.0, 1.9, 3.4);    /* draw it */
    draw_cube();
    glPopMatrix();

    glPopMatrix();

    /*------Draw right leg ----*/
    glPushMatrix();
    glTranslatef(8.0, 3.0, 0.0);
    glRotatef(20, 0.0, 1.0, 0.0);
    glRotatef(right_leg_ang, 0.0, 1.0, 0.0);
    glTranslatef(2.0, 0.0, 0.0); /* back to hip joiont */

    glPushMatrix();
    glScalef(6.8, 2.0, 2.0);    /* draw it */
    draw_cube();
    glPopMatrix();

    //right shinn
    //-y,x,z
    glTranslatef(0.0, 0.0, 0.0);
    glRotatef(150, 0.0, 1.0, 0.0);

    glTranslatef(-4.5, 0.0, 0.0);

    glRotatef(right_joint_ang, 0.0, 1.0, 0.0);
    glPushMatrix();
    glScalef(7.0, 1.9, 1.9);    /* draw it */
    draw_cube();
    glPopMatrix();

    //right foot
    glPushMatrix();
    glTranslatef(-3.0, 0.0, -0.6);

    glScalef(1.0, 1.9, 3.4);    /* draw it */
    draw_cube();
    glPopMatrix();

    glPopMatrix();
}

void draw_obstacle()
{
    /*-----Define some material properties shared by every one ---*/
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    glPushMatrix();
    glTranslatef(5.0, 0.0, 15.0);
    glPushMatrix();
    glScalef(10.0, 20.0, 10.0);
    draw_cube();
    glPopMatrix();
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureID[1]);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void draw_rock()
{
    /*-----Define some material properties shared by every one ---*/
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glNormal3f(0.0, 1.0, 0.0);

    glPushMatrix();
    glColor3f(0.6, 0.7, 0.6);
    //lift the head
    glTranslatef(0.0, 0.0, -10.0);
    gluSphere(sphere, 2.0,   /* radius=4.0 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 12 stacks */
    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);
}

void draw_86()
{
    glPushMatrix();
    glTranslatef(p86[0], p86[1], p86[2]);

    glPushMatrix();              /* 8 */
    glColor3f(1.0, 1.0, 1.0);
    gluCylinder(cylind, 6, 4, /* radius of top and bottom circle */
        2.0,              /* height of the cylinder */
        18,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();

    glTranslatef(0.0, 10.0, 0.0);
    glPushMatrix();              /* Save M1 coord. sys */
    glColor3f(1.0, 1.0, 1.0);
    gluCylinder(cylind, 5, 3, /* radius of top and bottom circle */
        2.0,              /* height of the cylinder */
        18,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();

    glTranslatef(12.0, -10.0, 0.0); //6
    glPushMatrix();              /* Save M1 coord. sys */
    glColor3f(1.0, 1.0, 1.0);
    gluCylinder(cylind, 6, 4, /* radius of top and bottom circle */
        2.0,              /* height of the cylinder */
        18,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();

    glTranslatef(-3.5, 9.0, 0.0);
    glPushMatrix();
    glScalef(2.0, 11.0, 2.0);    /* draw it*/
    draw_cube();
    glPopMatrix();

    glPopMatrix();
}

void draw_billboard(float x, float z, float w, float h);
void brick_pattern();

/*---------------------------------------------------------
 * Procedure to draw the floor.
 */
void draw_floor(int index)
{
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureID[index]);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glNormal3f(0.0, 1.0, 0.0);

    glPushMatrix();
    glTranslated(0.0, -3.0, 0.0);
    glScalef(100.0, 3.0, 100.0);
    Draw_UnitCube(select);
    glPopMatrix();

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++) {
            if ((i + j) % 2 == 0) glColor3f(1.0, 0.8, 0.8);
            else glColor3f(0.1, 0.1, 0.7);
            //draw the lattice
            glBegin(GL_POLYGON);
            glVertex3f((i - 5.0) * 10.0, -2.5, (j - 5.0) * 10.0);
            glVertex3f((i - 5.0) * 10.0, -2.5, (j - 4.0) * 10.0);
            glVertex3f((i - 4.0) * 10.0, -2.5, (j - 4.0) * 10.0);
            glVertex3f((i - 4.0) * 10.0, -2.5, (j - 5.0) * 10.0);
            glEnd();
        }
    glDisable(GL_COLOR_MATERIAL);
}

/*-------------------------------------------------------
 * Procedure to draw three axes and the orign
 */
void draw_axes()
{
    /*----Draw three axes in colors, yellow, meginta, and cyan--*/
    glPushMatrix();
    glTranslatef(0.0, 0.0, -45.0);
    glPushMatrix();

    glPushMatrix();

    /*----Draw a red sphere to represent the original-----*/
    glColor3f(1.0, 0.0, 0.0);
    gluSphere(sphere, 2.0,   /* radius=2.0 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 8 stacks */

    /* Draw Z axis */
    glColor3f(0.0, 0.0, 1.0);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();

    /* Draw Y axis */
    glColor3f(1.0, 1.0, 1.0);
    /*Rotate x -90, z->y,y->(-z)*/
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();

    /* Draw X axis */
    glColor3f(1.0, 0.0, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);  /*Rotate about y  by 90, x -> z,z->-x,y, */
    gluCylinder(cylind, 0.5, 0.5,   /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();

    /*-- Restore the original modelview matrix --*/
    //glPopMatrix();
}

/*-------------------------------------------------------
 * Procedure to draw view volume
 */
void draw_view_volume(double fov)
{
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(0.5, 0, 0, 0.2);
    double aspectRatio = (double)width / (double)height;
    double zNear = 1.5;
    double zFar = 50.0;

    double rFov = fov * PI / 180.0;
    double near_left = -zNear * tan(rFov / 2.0) * aspectRatio;
    double near_right = zNear * tan(rFov / 2.0) * aspectRatio;
    double near_bottom = -zNear * tan(rFov / 2.0);
    double near_top = zNear * tan(rFov / 2.0);
    double far_left = -zFar * tan(rFov / 2.0) * aspectRatio;
    double far_right = zFar * tan(rFov / 2.0) * aspectRatio;
    double far_bottom = -zFar * tan(rFov / 2.0);
    double far_top = zFar * tan(rFov / 2.0);

    glColor4f(0.5, 0, 0, 0.2);
    glBegin(GL_POLYGON);
    glVertex3f(zNear, near_top, near_left);
    glVertex3f(zNear, near_top, near_right);
    glVertex3f(zNear, near_bottom, near_right);
    glVertex3f(zNear, near_bottom, near_left);
    glEnd();

    glColor4f(0.5, 0, 0, 0.2);
    glBegin(GL_POLYGON);
    glVertex3f(zFar, far_top, far_left);
    glVertex3f(zFar, far_top, far_right);
    glVertex3f(zFar, far_bottom, far_right);
    glVertex3f(zFar, far_bottom, far_left);
    glEnd();

    glColor4f(0.5, 0, 0, 0.2);
    glBegin(GL_POLYGON);
    glVertex3f(zFar, far_bottom, far_left);
    glVertex3f(zFar, far_bottom, far_right);
    glVertex3f(zNear, near_bottom, near_right);
    glVertex3f(zNear, near_bottom, near_left);
    glEnd();

    glColor4f(0.5, 0, 0, 0.2);
    glBegin(GL_POLYGON);
    glVertex3f(zNear, near_top, near_left);
    glVertex3f(zNear, near_top, near_right);
    glVertex3f(zFar, far_top, far_right);
    glVertex3f(zFar, far_top, far_left);
    glEnd();

    glColor4f(0.5, 0, 0, 0.2);
    glBegin(GL_POLYGON);
    glVertex3f(zNear, near_top, near_right);
    glVertex3f(zNear, near_bottom, near_right);
    glVertex3f(zFar, far_bottom, far_right);
    glVertex3f(zFar, far_top, far_right);
    glEnd();

    glColor4f(0.5, 0, 0, 0.2);
    glBegin(GL_POLYGON);
    glVertex3f(zNear, near_top, near_left);
    glVertex3f(zNear, near_bottom, near_left);
    glVertex3f(zFar, far_bottom, far_left);
    glVertex3f(zFar, far_top, far_left);
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glFinish();
}

void draw_view()
{
    int    i=0;
    glMatrixMode(GL_MODELVIEW);

    /*----Draw Eye position-----*/
    glPushMatrix();
    glTranslatef(eye[0], eye[1], eye[2]);
    glColor3f(0.0, 1.0, 0.0);
    glutWireSphere(1.0, 10, 10);

    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glRotatef(roll_ang, 1, 0, 0);
    glRotatef(head_ang, 0, 1, 0);
    glRotatef(pitch_ang, 0, 0, 1);
    draw_view_volume(fov);
    glPopMatrix();

    glPopMatrix();

    /*----Draw eye coord. axes -----*/
    glColor3f(1.0, 1.0, 0.0);           /* Draw Xe */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[0][0], eye[1] + 20.0 * u[0][1], eye[2] + 20.0 * u[0][2]);
    glEnd();

    glColor3f(1.0, 0.0, 1.0);          /* Draw Ye */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[1][0], eye[1] + 20.0 * u[1][1], eye[2] + 20.0 * u[1][2]);
    glEnd();

    glColor3f(0.0, 1.0, 1.0);          /* Draw Ze */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[2][0], eye[1] + 20.0 * u[2][1], eye[2] + 20.0 * u[2][2]);
    glEnd();
}

void BillboardDraw()
{
    glPushMatrix();
    /*-------Draw the billboard ----*/
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, textName[0]);

    draw_billboard(5.0 * 0.0, -10.0, 5.0, 8.0);
    draw_billboard(6.0 * 4.0, 5.0 * 4.0, 5.0, 8.0);
    draw_billboard(3.0 * 4.0, 6.0 * 4.0, 5.0, 8.0);
    draw_billboard(2.0 * 4.0, 7.0 * 4.0, 5.0, 8.0);
    draw_billboard(7.0 * 4.0, 2.0 * 4.0, 5.0, 8.0);

    /*-------Draw the billboard ----*/
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glBindTexture(GL_TEXTURE_2D, textName[1]);

    draw_billboard(5.0 * 4.0 + 5, 3.0 * 4.0 + 5, 5.0, 8.0 + 5);
    draw_billboard(6.0 * 4.0 + 5, 5.0 * 4.0 + 5, 5.0, 8.0 + 5);
    draw_billboard(3.0 * 4.0 + 5, 6.0 * 4.0 + 5, 5.0, 8.0 + 5);
    draw_billboard(2.0 * 4.0 + 5, 7.0 * 4.0 + 5, 5.0, 8.0 + 5);
    draw_billboard(7.0 * 4.0 + 5, 2.0 * 4.0 + 5, 5.0, 8.0 + 5);

    /*-------Draw the billboard ----*/
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glBindTexture(GL_TEXTURE_2D, textName[2]);

    draw_billboard(5.0 * 4.0 - 10, 3.0 * 4.0 - 10, 5.0, 8.0 - 10);
    draw_billboard(6.0 * 4.0 - 10, 5.0 * 4.0 - 10, 5.0, 8.0 - 10);
    draw_billboard(3.0 * 4.0 - 10, 6.0 * 4.0 - 10, 5.0, 8.0 - 10);
    draw_billboard(2.0 * 4.0 - 10, 7.0 * 4.0 - 10, 5.0, 8.0 - 10);
    draw_billboard(7.0 * 4.0 - 10, 2.0 * 4.0 - 10, 5.0, 8.0 - 10);

    glPopMatrix();
}
void draw_scene()
{
    BillboardDraw();
    int index = 0;

    draw_floor(index);
    draw_axes();
    draw_rock();

    /*-------Rotate light position----*/
    glPushMatrix();
    glTranslatef(8.0, 0.0, 8.0);          /* Move to [8, 0, 8] */
    glRotatef(lit_angle, 0.0, 1.0, 0.0);  /* Rotate about y-axis */
    /*----position light2 (fixed) in eye coord sys ---*/
    glLightfv(GL_LIGHT2, GL_POSITION, lit2_position);  /*fixed position in eye space---*/
    /*---draw the light source ---*/
    glTranslatef(lit2_position[0], lit2_position[1], lit2_position[2]);
    //glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glColor3f(0.80, 0.00, 0.80);
    glutSolidSphere(1.5, 12, 12);
    glPopMatrix();
    glEnable(GL_CULL_FACE);

    glPushMatrix();
    glScalef(0.8, 1.1, 0.8);    /* resize the 86 */
    draw_86();
    glPopMatrix();
    draw_obstacle();
    draw_body();
}

void p86_direction(unsigned char key)
{
    if (key == 'v') {//forward
        p86[2] += Step;
    }
    else if (key == 'c') {//turn left
        p86[0] -= Step;
    }
    else if (key == 'f') {//backward
        p86[2] -= Step;
    }
    else if (key == 'b') {//turn right
        p86[0] += Step;
    }
    printf("p86[0]=%f\np86[1]=%f\np86[2]=%f\n", p86[0], p86[1], p86[2]);
}

float p86_diff(float posp, float pos86) {//couputue the diffusion
    return fabs(posp - pos86);
}

void position_direction(unsigned char key) {
    //forward
    if (key == 's') {
        motion = 1;
        dir = 0;
        position[0] += Step * cos(self_ang * PI / 180.0);
        position[2] -= Step * sin(self_ang * PI / 180.0);
    }
    //turn left
    else if (key == 'a') {
        motion = 1;
        dir = 1;
        position[0] += Step * cos(self_ang * PI / 90.0);
        position[2] -= Step * sin(self_ang * PI / 90.0);
    }
    //backward
    else if (key == 'w') {
        motion = 1;
        dir = 2;
        position[0] -= Step * cos(self_ang * PI / 180.0);
        position[2] += Step * sin(self_ang * PI / 180.0);
    }
    //turn right
    else if (key == 'd') {
        motion = 1;
        dir = 3;
        position[0] -= Step * cos(self_ang * PI / 90.0);
        position[2] += Step * sin(self_ang * PI / 90.0);
    }
    //run
    else if (key == 'r') {
        motion = 1;
        if (dir == 0) {
            position[0] += 2 * Step * cos(self_ang * PI / 180.0);
            position[2] -= 2 * Step * sin(self_ang * PI / 180.0);
        }
        else if (dir == 1) {
            position[0] += 2 * Step * cos(self_ang * PI / 90.0);
            position[2] -= 2 * Step * sin(self_ang * PI / 90.0);
        }
        else if (dir == 2) {
            position[0] -= 2 * Step * cos(self_ang * PI / 180.0);
            position[2] += 2 * Step * sin(self_ang * PI / 180.0);
        }
        else if (dir == 3) {
            position[0] -= 2 * Step * cos(self_ang * PI / 90.0);
            position[2] += 2 * Step * sin(self_ang * PI / 90.0);
        }

    }
    /*jump*/
    else if (key == 'j') {
        jump = 1;
        position[1] = -0.5;
        if (position[1] >= 6.0) {
            position[1] -= 6.0;
            jump = 0;
        }
    }
    /*gusture please*/
    else if (key == 'u') {
        hug = 1;

        if (left_arm_hug_ang == 0 && left_arm_hug_ang > -20) {
            position[1] = -1.5;
            left_arm_hug_ang -= 50;
            left_joint_ang -= 50;
            left_leg_ang += 50;
            right_joint_ang -= 50;
            right_leg_ang += 50;
        }
        if (right_arm_hug_ang == 0 && right_arm_hug_ang > -20) {
            right_arm_hug_ang -= 50;
        }
        else {
            left_arm_hug_ang = 0;
            left_joint_ang = 0;
            left_leg_ang = 0;
            right_joint_ang = 0;
            right_leg_ang = 0;
            right_arm_hug_ang = 0;
            position[1] = 0;
        }
    }
    printf("position[0]=%f\nposition[1]=%f\nposition[2]=%f\n", position[0], position[1], position[2]);
}

/*-------------------------------------------------------
 * Make viewing matrix
 */
void make_view(int x)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    switch (x) {
    case 4:       /* Perspective */

        /* In this sample program, eye position and Xe, Ye, Ze are computed
           by ourselves. Therefore, use them directly; no trabsform is
           applied upon eye coordinate system
           */
        gluLookAt(eye[0], eye[1], eye[2],
            eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2],
            u[1][0], u[1][1], u[1][2]);
        break;

    case 1:       /* X direction parallel viewing */
        gluLookAt(30.0, 0.0, 0.0, position[0], position[1], position[2], 0.0, 1.0, 0.0);
        break;
    case 2:       /* Y direction parallel viewing */
        gluLookAt(0.0, 30.0, 0.0, position[0], position[1], position[2], 1.0, 0.0, 0.0);
        break;
    case 3:       /* Z direction parallel viewing */
        gluLookAt(0.0, 0.0, 30.0, position[0], position[1], position[2], 0.0, 1.0, 0.0);
        break;
    case 6:       /*original viewing*/
        gluLookAt(20.0, 30.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        break;
    }
}

/*------------------------------------------------------
 * Procedure to make projection matrix
 */
void make_projection(int x)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (x == 4) {
        gluPerspective(fov, aspectRatio, zNear, zFar);
    }
    else if (x == 6) {
        //glFrustum(left, right, bottom, top, zNear, zFar);
        gluPerspective(fov, aspectRatio, zNear, zFar);
    }
    else {
        if (width > height)
            glOrtho(-40.0 - othZoom, 40.0 + othZoom, -40.0 / aspectRatio - othZoom,
                40.0 / aspectRatio + othZoom,
                -100.0, 100.0);
        else
            glOrtho(-40.0 * aspectRatio - othZoom,
                40.0 * aspectRatio + othZoom, -40.0 - othZoom, 40.0 + othZoom,
                -100.0, 100.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

/*---------------------------------------------
 * Idle event callback function, increase blade and arm
 * rotation angles.
 */
void idle_func()
{
    /*moving*/
    if (motion == 1) {
        if (left_arm_ang < MaxSwingDegree) {//from 723 to 754 control the walking and swing
            left_arm_ang += 0.2;
        }
        else {
            left_arm_ang -= MaxSwingDegree;
            motion = 0;
        }

        if (right_arm_ang > -MaxSwingDegree) {
            right_arm_ang -= 0.2;
        }
        else {
            right_arm_ang += MaxSwingDegree;
            motion = 0;
        }

        if (right_leg_ang < MaxSwingDegree) {
            right_leg_ang += 0.2;
        }
        else {
            right_leg_ang = 0.0;
            motion = 0;
        }

        if (left_leg_ang > -MaxSwingDegree) {
            left_leg_ang -= 0.2;
        }
        else {
            left_leg_ang = 0.0;
            motion = 0;
        }

        int jf = 0;
        if (jump == 1)
        {
            if (position[1] < 20.0 && jf == 0) {//kneel down first
                position[1] += 0.1;
                if (left_joint_ang == 0 && left_joint_ang > -20) {
                    left_joint_ang -= 0.2;
                }
                if (left_leg_ang == 0 && left_leg_ang < 10) {
                    left_leg_ang += 0.2;
                }
                if (right_joint_ang == 0 && right_joint_ang > -20) {
                    right_joint_ang -= 0.2;
                }
                if (right_leg_ang == 0 && right_leg_ang < 10) {
                    right_leg_ang += 0.2;
                }
                if (position[1] >= 10.0)
                    jf = 1;
            }
            if (position[1] >= 20.0) {
                if ((position[0] >= -4.5 && position[0] <= 15 && position[2] >= 9 && position[2] <= 27)
                    || (position[0] >= 0 && position[0] < 6 && position[2] >= 6 && position[2] <= 27)
                    || (position[0] >= 6 && position[0] <= 12 && position[2] >= 6 && position[2] <= 27)) {
                    position[1] = 10;// if bump into obstacle jump upon it
                }
                else {// return to the ground
                    position[1] = 0.0;
                }
                left_leg_ang = 0.0;
                right_leg_ang = 0.0;
                motion = 0;
                jump = 0;
            }
        }
    }
    display(); // show the scene again 
}

void makedelay(int) ///to prevent the explosion ocurs too fast
{
    exit(0);
    glutTimerFunc(1000, makedelay, 1000);
}

void Create_Texture_Circle()
{
    int  y, x, i, temp;
    for (y = 0; y < 64; y++)
    {
        for (x = 0; x < 64; x++)
        {
            circle[y][x][0] = 100;
            circle[y][x][1] = 100;
            circle[y][x][2] = 100;
            circle[y][x][3] = 255;
        }
    }
    for (y = 16, x = 0; y < 48; y++)
    {
        if (y <= 20 && y >= 17)
            x = x + 2;
        else if (y == 22 || y == 25 || y == 31)
            x = x + 1;
        else if (y == 42 || y == 39 || y == 33)
            x = x - 1;
        else if (y <= 47 && y >= 44)
            x = x - 2;
        temp = x;
        for (x = 0; x < temp; x++)
        {
            circle[y][x][0] = 150;
            circle[y][x][1] = 150;
            circle[y][x][2] = 150;
            circle[y][x][3] = 255;
        }
        for (x = 64 - temp; x < 64; x++)
        {
            circle[y][x][0] = 150;
            circle[y][x][1] = 150;
            circle[y][x][2] = 150;
            circle[y][x][3] = 255;
        }
        x = temp;
    }

    for (x = 16, y = 0; x < 48; x++)
    {
        if (x <= 20 && x >= 17)
            y = y + 2;
        else if (x == 22 || x == 25 || x == 31)
            y = y + 1;
        else if (x == 42 || x == 39 || x == 33)
            y = y - 1;
        else if (x <= 47 && x >= 44)
            y = y - 2;
        temp = y;
        for (y = 0; y < temp; y++)
        {
            circle[y][x][0] = 200;
            circle[y][x][1] = 200;
            circle[y][x][2] = 200;
            circle[y][x][3] = 255;
        }
        for (y = 64 - temp; y < 64; y++)
        {
            circle[y][x][0] = 200;
            circle[y][x][1] = 200;
            circle[y][x][2] = 200;
            circle[y][x][3] = 255;
        }
        y = temp;
    }
    for (i = 0; i < 61; i++)
    {
        for (y = i; y < 4 + i; y++)
        {
            for (x = i; x < 4 + i; x++)
            {
                circle[y][x][0] = 255;
                circle[y][x][1] = 255;
                circle[y][x][2] = 255;
                circle[y][x][3] = 255;
            }
        }
    }
    for (i = 0; i < 61; i++)
    {
        for (y = i; y < 4 + i; y++)
        {
            for (x = 60 - i; x < 64 - i; x++)
            {
                circle[y][x][0] = 255;
                circle[y][x][1] = 255;
                circle[y][x][2] = 255;
                circle[y][x][3] = 255;
            }
        }
    }
}

void Create_Texture_Land()
{
    int x, y, i;

    for (y = 0; y < 32; y++)
        for (x = 0; x < 32; x++)
        {
            land[x][y][0] = 240;
            land[x][y][1] = 240;
            land[x][y][2] = 240;
            land[x][y][3] = 255;
        }


    for (x = 24; x < 32; x++)
    {
        land[x][0][0] = 255;
        land[x][0][1] = 200;
        land[x][0][2] = 200;
        land[x][0][3] = 255;

        land[0][x][0] = 255;
        land[0][x][1] = 200;
        land[0][x][2] = 200;
        land[0][x][3] = 255;
    }
    for (x = 26; x < 32; x++)
    {
        land[x][1][0] = 255;
        land[x][1][1] = 200;
        land[x][1][2] = 200;
        land[x][1][3] = 255;

        land[1][x][0] = 255;
        land[1][x][1] = 200;
        land[1][x][2] = 200;
        land[1][x][3] = 255;
    }
    for (x = 27; x < 32; x++)
    {
        for (y = 2; y < 4; y++)
        {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }
    for (x = 28; x < 32; x++)
    {
        for (y = 4; y < 7; y++)
        {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }
    for (x = 29; x < 32; x++)
    {
        for (y = 7; y < 9; y++)
        {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }
    for (x = 30; x < 32; x++)
    {
        for (y = 9; y < 12; y++)
        {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }
    for (x = 31; x < 32; x++)
    {
        for (y = 12; y < 16; y++)
        {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 200;
            land[x][y][3] = 255;

            land[y][x][0] = 255;
            land[y][x][1] = 200;
            land[y][x][2] = 200;
            land[y][x][3] = 255;
        }
    }

    for (y = 16; y < 32; y++)
        for (x = 16; x < 32; x++)
        {
            land[x][y][0] = 200;
            land[x][y][1] = 160;
            land[x][y][2] = 255;
            land[x][y][3] = 255;
        }
    for (y = 20; y < 32; y++)
        for (x = 20; x < 32; x++)
        {
            land[x][y][0] = 255;
            land[x][y][1] = 200;
            land[x][y][2] = 255;
            land[x][y][3] = 255;
        }

    for (x = 63; x > 31; x--)
    {
        for (y = 0; y < 32; y++)
            for (i = 0; i < 4; i++)
                land[x][y][i] = land[63 - x][y][i];
    }

    /* Create the rest half part */
    for (y = 32; y < 64; y++)
        for (x = 0; x < 64; x++)
            for (i = 0; i < 4; i++)
                land[x][y][i] = land[x][63 - y][i];
}

void Create_Texture_Star()
{
    int x, y, i;

    for (x = 0; x < 64; x++)
        for (y = 0; y < 64; y++)
        {
            star[x][y][0] = 255;
            star[x][y][1] = 255;
            star[x][y][2] = 255;
            star[x][y][3] = 255;
        }

    x = 31;	y = 60;
    while (x >= 12)
    {
        for (i = y; i > 4; i--)
        {
            star[x][i][0] = 100;
            star[x][i][1] = 100;
            star[x][i][2] = 255;
            star[x][i][3] = 255;
        }
        y -= 3;
        x--;
    }
    x = 31;	y = 20;
    while (x >= 12)
    {
        for (i = y; i > 4; i--)
        {
            star[x][i][0] = 255;
            star[x][i][1] = 255;
            star[x][i][2] = 255;
            star[x][i][3] = 255;
        }
        y -= 1;
        x--;
    }
    x = 4;	y = 40;
    while (y >= 18)
    {
        for (i = x; i < 32; i++)
        {
            star[i][y][0] = 100;
            star[i][y][1] = 100;
            star[i][y][2] = 255;
            star[i][y][3] = 255;
        }
        y -= 1;
        if ((y % 2) == 1)
            x += 2;
        else
            x++;
    }

    for (x = 63; x > 31; x--)
        for (y = 0; y < 64; y++)
            for (i = 0; i < 4; i++)
                star[x][y][i] = star[63 - x][y][i];
}

void Draw_UnitCube(int index)
{
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureID[index]);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    for (int i = 0; i < 6; i++)
    {
        glNormal3fv(normal[i]);
        glColor3dv(colors[i]);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(vertex[face[i][0]]);
        glTexCoord2f(0.0, 8.0);
        glVertex3fv(vertex[face[i][1]]);
        glTexCoord2f(8.0, 8.0);
        glVertex3fv(vertex[face[i][2]]);
        glTexCoord2f(8.0, 0.0);
        glVertex3fv(vertex[face[i][3]]);
        glEnd();
    }
}