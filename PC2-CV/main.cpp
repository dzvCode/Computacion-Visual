#include <stdlib.h>
#include<conio.h>
#include<stdio.h>
#include<math.h>
#include <gl/glut.h>

void init(void);
void display(void);
void reshape(int,int);
void ejes(int,bool);
void cilindro(float,int,int,GLenum,int);
void esfera(float,float,float,bool);
void color1(int);
void color2(int);
void trigonalPlano();
void tetraedrica();
void bipiramidePentagonal();
void octaedrica();
void bipiramideTrigonal();
void spin();
void rotaCamara();
void moverCamaraY();
void avanza();
void retro();
void rotaciones(bool);
void teclado(unsigned char,int,int);
//Menus
void menuSistemaReferencial(int);
void menuRepresentaciones(int);
void menuJuegoColores(int);
void menuRotaciones(int);
void mainMenu(int);

//Variables Globales para el funcionamiento de los Menus
GLint modelo=0;
GLint sistema=0;
GLint rotacion=0;
GLint color = 1;

//Variables que permiten el funcionamiento de los distintos movimientos
GLdouble angulo=0, incremento_angulo=0.1, anguloVertical = 0;
GLdouble const radio=0.5;
GLfloat px0=0,py0=0,pz0=5;
GLfloat px1=0,py1=0,pz1=4;
static GLfloat theta[] = {0.0,0.0,0.0};
static GLint axis = 2 ;

//Variable booleana que permite cambiar la
//representacion de la esfera (True = glutWireSphere)
bool stateEsfera = true;

//Variables que permitiran cambiar a alambrico o solido el cilindro
//Por defecto estaran alambricos
GLenum modo1=GL_LINE_LOOP;
GLenum modo2=GL_LINES;
int mitad=0;

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,800);
    glutCreateWindow("Practica Califica 2 | Pairazaman & Zavala");
    ////SubMenus////
    int subMenuSistemaReferencial = glutCreateMenu(menuSistemaReferencial);
        glutAddMenuEntry("Activado",1);
        glutAddMenuEntry("Desactivado",2);

    int subMenuRepresentacion = glutCreateMenu(menuRepresentaciones);
        glutAddMenuEntry("Alambrica",1);
        glutAddMenuEntry("Solido",2);

    int subMenuMoleculas = glutCreateMenu(mainMenu);
        glutAddMenuEntry("Trigonal Plano",1);
        glutAddMenuEntry("Tetraedrica",2);
        glutAddMenuEntry("Bipiramide pentagonal",3);
        glutAddMenuEntry("Octaedrica",4);
        glutAddMenuEntry("Bipiramide trigonal",5);

    int subMenuJuegoColores = glutCreateMenu(menuJuegoColores);
        glutAddMenuEntry("Negro-Crema",1);
        glutAddMenuEntry("Verde-Naranja",2);
        glutAddMenuEntry("Rosado-Mostaza",3);

    int subMenuRotaciones = glutCreateMenu(menuRotaciones);
        glutAddMenuEntry("Activado",1);
        glutAddMenuEntry("Desactivado",2);

    ////Main Menu////
    int menu = glutCreateMenu(mainMenu);
        glutAddSubMenu("Sistema Referencial 3D",subMenuSistemaReferencial);
        glutAddSubMenu("Representacion",subMenuRepresentacion);
        glutAddSubMenu("Estructuras Moleculares",subMenuMoleculas);
        glutAddSubMenu("Juego de Colores",subMenuJuegoColores);
        glutAddSubMenu("Rotaciones",subMenuRotaciones);
        glutAddMenuEntry("Esc (Presionar F)",6);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(spin);
    glutKeyboardFunc(teclado);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}

void init(void){
    glClearColor (0.227,0.22,0.271, 0.0);//Color morado pastel
    glShadeModel (GL_FLAT);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // la camara se desplaza sobre el plano xz
    gluLookAt(px0,py0,pz0,px1,py1,pz1,0,1,0);

    menuSistemaReferencial(sistema);
    menuRotaciones(rotacion);
    mainMenu(modelo);

    glFlush();
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h){
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(100, (GLfloat) w/(GLfloat) h,0.1, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//Crea el cilindro donde dependiendo de las variable 'modo1'
//y 'modo2' cambia su representacion (solido o cableado)
//Por otro lado, la variable 'mitad' cambia el juego de colores
void cilindro(float radio, int Nc, int Np, GLenum modo1, GLenum modo2, int mitad){
   float x,y,z,h,ang;
   int i,j;
   h=1.0/float(Nc-1);
   glLineWidth(1);

   if(mitad==0)
    color1(color);
   else
    color2(color);

   //circulosHorizontales
   glBegin(modo1);
   for(j=0;j<Nc;j++){
      for (i=0;i<=Np;i++){
         ang=2*i*M_PI/(Np);
         x=radio*cos(ang);
         y=-0.5+j*h;
         z=radio*sin(ang);
         glVertex3f(x,y,z);
      }
   }
   glEnd();
   //lineasVerticales
  glBegin(modo2);
      for (i=0;i<=Np;i++){
         ang=2*i*M_PI/(Np);
         x=radio*cos(ang);
         z=radio*sin(ang);
         glVertex3f(x,-0.5,z);
         glVertex3f(x,0.5,z);
      }
   glEnd();
}

//Dependiendo el valor de 'state' cambia la representacion
//de la esfera (solido o cableada)
void esfera(float r, float m, float f, bool state){
    if(state)
        glutWireSphere(r, m, f);
    else
        glutSolidSphere(r,m,f);
}

void trigonalPlano(){
    // Esfera central
    color1(color);
    glPushMatrix();
        glTranslatef(0,0,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Tubo cilindrico vertical + esfera
    glPushMatrix();
        glTranslatef(0,0.5,0);
        cilindro(0.3,10,10,modo1,modo2,0);
        glTranslatef(0,1,0);
        cilindro(0.3,10,10,modo1,modo2,1);
        glTranslatef(0,0.5,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Tubo cilindrico inferior izquierdo + esfera
    glPushMatrix();
        glRotatef(-60,0,0,1);
        glTranslatef(0,-0.5,0);
        cilindro(0.3,10,10,modo1,modo2,0);
        glTranslatef(0,-1,0);
        cilindro(0.3,10,10,modo1,modo2,1);
        glTranslatef(0,-0.5,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Tubo cilindrico inferior derecho + esfera
    glPushMatrix();
        glRotatef(60,0,0,1);
        glTranslatef(0,-0.5,0);
        cilindro(0.3,10,10,modo1,modo2,0);
        glTranslatef(0,-1,0);
        cilindro(0.3,10,10,modo1,modo2,1);
        glTranslatef(0,-0.5,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();
}

void tetraedrica(){
 // Esfera central
    color1(color);
    glPushMatrix();
        glTranslatef(0,0,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Tubo cilindrico eje Y Y esfera
    glPushMatrix();
        glTranslatef(0,0.5,0);
        cilindro(0.3,10,10,modo1,modo2,0);
        glTranslatef(0,1,0);
        cilindro(0.3,10,10,modo1,modo2,1);
        glTranslatef(0,0.5,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Tubo cilindrico eje Z- Y- y esfera
    glPushMatrix();
        glRotatef(109.5,1,0,0);
        glTranslatef(0,0.5,0);
        cilindro(0.3,10,10,modo1,modo2,0);
        glTranslatef(0,1,0);
        cilindro(0.3,10,10,modo1,modo2,1);
        glTranslatef(0,0.5,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Tubo cilindrico eje X+ Y- Z+ y esfera
    glPushMatrix();
        glRotatef(109.5,0,1,0);
        glRotatef(109.5,1,0,0);
        glTranslatef(0,0.5,0);
        cilindro(0.3,10,10,modo1,modo2,0);
        glTranslatef(0,1,0);
        cilindro(0.3,10,10,modo1,modo2,1);
        glTranslatef(0,0.5,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

     // Tubo cilindrico eje X- Y- Z+
    glPushMatrix();
        color1(color);
        glRotatef(219,0,1,0);
        glRotatef(109.5,1,0,0);
        glTranslatef(0,0.5,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

     // Tubo cilindrico 2 eje X- Y- Z+
    glPushMatrix();
        color2(color);
        glRotatef(219,0,1,0);
        glRotatef(109.5,1,0,0);
        glTranslatef(0,1.5,0);
        cilindro(0.3,10,10,modo1,modo2,1);
        glTranslatef(0,0.5,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();
}

void bipiramidePentagonal(){
    // Tubo cilindrico eje X+ Z-
    glPushMatrix();
        glRotatef(18,0,1,0);
        glTranslatef(0.5,0,0);
        glRotatef(-90,0,0,1);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo cilindrico eje X- Z-
    glPushMatrix();
        glRotatef(-18,0,1,0);
        glTranslatef(-0.5,0,0);
        glRotatef(90,0,0,1);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo cilindrico eje X- Z+
    glPushMatrix();
        glRotatef(-36,0,1,0);
        glTranslatef(0,0,0.5);
        glRotatef(90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

        // Tubo cilindrico eje X+ Z+
    glPushMatrix();
        glRotatef(36,0,1,0);
        glTranslatef(0,0,0.5);
        glRotatef(90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo cilindrico eje Y positivo
    glPushMatrix();
        glTranslatef(0,0.5,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo cilindrico eje Y negativo
    glPushMatrix();
        glTranslatef(0,-0.5,0);
        glRotatef(180,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo cilindrico eje Z negativo
    glPushMatrix();
        glTranslatef(0,0,-0.5);
        glRotatef(-90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo 2 cilindrico eje X+ Z-
    glPushMatrix();
        glRotatef(18,0,1,0);
        glTranslatef(1.5,0,0);
        glRotatef(-90,0,0,1);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje X- Z-
    glPushMatrix();
        glRotatef(-18,0,1,0);
        glTranslatef(-1.5,0,0);
        glRotatef(90,0,0,1);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje X- Z+
    glPushMatrix();
        glRotatef(-36,0,1,0);
        glTranslatef(0,0,1.5);
        glRotatef(90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje X+ Z+
    glPushMatrix();
        glRotatef(36,0,1,0);
        glTranslatef(0,0,1.5);
        glRotatef(90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje Y positivo
    glPushMatrix();
        glTranslatef(0,1.5,0);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje Y negativo
    glPushMatrix();
        glTranslatef(0,-1.5,0);
        glRotatef(180,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje Z negativo
    glPushMatrix();
        glTranslatef(0,0,-1.5);
        glRotatef(-90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();


    // Esfera central
    glPushMatrix();
        color1(color);
        glTranslatef(0,0,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Esfera eje X+ Z-
    glPushMatrix();
        color2(color);
        glTranslatef(1.902113033,0,-0.6180339887); //glTranslatef(2*cos(18),0,-2*sin(18));
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Esfera eje X- Z-
    glPushMatrix();
        color2(color);
        glTranslatef(-1.902113033,0,-0.6180339887); //glTranslatef(-2*cos(18),0,-2*sin(18));
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Esfera eje X- Z+
    glPushMatrix();
        color2(color);
        glTranslatef(-1.175570505,0,1.618033989); //glTranslatef(-2*sin(36),0,2*cos(36));
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Esfera eje X+ Z+
    glPushMatrix();
        color2(color);
        glTranslatef(1.175570505,0,1.618033989); //glTranslatef(2*sin(36),0,2*cos(36));
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Esfera eje Y positivo
    glPushMatrix();
        color2(color);
        glTranslatef(0,2,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Esfera eje Y negativo
    glPushMatrix();
        color2(color);
        glTranslatef(0,-2,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Esfera eje Z negativo
    glPushMatrix();
        color2(color);
        glTranslatef(0,0,-2);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();
}

void octaedrica(){

     // Tubo cilindrico eje X positivo
    glPushMatrix();
        glTranslatef(0.5,0,0);
        glRotatef(-90,0,0,1);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo cilindrico eje X negativo
    glPushMatrix();
        glTranslatef(-0.5,0,0);
        glRotatef(-90,0,0,1);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo cilindrico eje Y positivo
    glPushMatrix();
        glTranslatef(0,0.5,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo cilindrico eje Y negativo
    glPushMatrix();
        glTranslatef(0,-0.5,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo cilindrico eje Z positivo
    glPushMatrix();
        glTranslatef(0,0,0.5);
        glRotatef(-90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo cilindrico eje Z negativo
    glPushMatrix();
        glTranslatef(0,0,-0.5);
        glRotatef(-90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

     // Tubo 2 cilindrico eje X positivo
    glPushMatrix();
        glTranslatef(1.5,0,0);
        glRotatef(-90,0,0,1);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje X negativo
    glPushMatrix();
        glTranslatef(-1.5,0,0);
        glRotatef(-90,0,0,1);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje Y positivo
    glPushMatrix();
        glTranslatef(0,1.5,0);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje Y negativo
    glPushMatrix();
        glTranslatef(0,-1.5,0);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje Z positivo
    glPushMatrix();
        glTranslatef(0,0,1.5);
        glRotatef(-90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje Z negativo
    glPushMatrix();
        glTranslatef(0,0,-1.5);
        glRotatef(-90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Esfera central
    color1(color);
    glPushMatrix();
        glTranslatef(0,0,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Esfera eje X positivo
    color2(color);
    glPushMatrix();
        glTranslatef(2,0,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

     // Esfera eje X negativo
    glPushMatrix();
        glTranslatef(-2,0,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Esfera eje Y positivo
    glPushMatrix();
        glTranslatef(0,2,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Esfera eje Y negativo
    glPushMatrix();
        glTranslatef(0,-2,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Esfera eje Z positivo
    glPushMatrix();
        glTranslatef(0,0,2);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Esfera eje Z negativo
    glPushMatrix();
        glTranslatef(0,0,-2);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();
}

void bipiramideTrigonal(){
// Tubo cilindrico eje X+ Z+
    glPushMatrix();
        glRotatef(60,0,1,0);
        glTranslatef(0,0,0.5);
        glRotatef(90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo cilindrico eje X- Z+
    glPushMatrix();
        glRotatef(-60,0,1,0);
        glTranslatef(0,0,0.5);
        glRotatef(90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo cilindrico eje Y positivo
    glPushMatrix();
        glTranslatef(0,0.5,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo cilindrico eje Y negativo
    glPushMatrix();
        glTranslatef(0,-0.5,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo cilindrico eje Z negativo
    glPushMatrix();
        glTranslatef(0,0,-0.5);
        glRotatef(90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,0);
    glPopMatrix();

    // Tubo 2 cilindrico eje X+ Z+
    glPushMatrix();
        glRotatef(60,0,1,0);
        glTranslatef(0,0,1.5);
        glRotatef(90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje X- Z+
    glPushMatrix();
        glRotatef(-60,0,1,0);
        glTranslatef(0,0,1.5);
        glRotatef(90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje Y positivo
    glPushMatrix();
        glTranslatef(0,1.5,0);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje Y negativo
    glPushMatrix();
        glTranslatef(0,-1.5,0);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();

    // Tubo 2 cilindrico eje Z negativo
    glPushMatrix();
        glTranslatef(0,0,-1.5);
        glRotatef(90,1,0,0);
        cilindro(0.3,10,10,modo1,modo2,1);
    glPopMatrix();


    // Esfera central
    glPushMatrix();
        color1(color);
        glTranslatef(0,0,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

       // Esfera eje X+ Z+
    glPushMatrix();
        color2(color);
        glTranslatef(sqrt(3),0,1);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

       // Esfera eje X- Z+
    glPushMatrix();
        color2(color);
        glTranslatef(-sqrt(3),0,1);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

        // Esfera eje Y positivo
    glPushMatrix();
        color2(color);
        glTranslatef(0,2,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Esfera eje Y negativo
    glPushMatrix();
        color2(color);
        glTranslatef(0,-2,0);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();

    // Esfera eje Z negativo
    glPushMatrix();
        color2(color);
        glTranslatef(0,0,-2);
        esfera(0.5,20,20,stateEsfera);
    glPopMatrix();
}

//Combinaciones de colores
/*
    Caso 1: Negro - Crema
    Caso 2: Verde - Naranja
    Caso 3: Rosado - Mostaza
*/
void color1(int color){
    switch(color){
        case 1: glColor3f(0.063,0.059,0.059); break;//#100F0F
        case 2: glColor3f(0.58,0.706,0.624); break;//#93B49F
        case 3: glColor3f(0.741,0.259,0.569); break;//#BC4291
    }
}
void color2(int color){
   switch(color){
        case 1: glColor3f(0.863,0.843,0.788); break;//#DCD6C8
        case 2: glColor3f(0.875,0.471,0.38); break;//#DF7860
        case 3: glColor3f(1.,0.773,0.302); break;//#FFC54D
    }
}

//Grafica el sistema referencial 3D
void ejes(int longitud, bool state){
    glLineWidth(2);
    glColor3f (0.804,0.941,0.918);//#CDF0EA
    if(state){
        glBegin(GL_LINES);
            glVertex3d(0,0,longitud);
            glVertex3d(0,0,0);
        glEnd();
        glBegin(GL_LINES);
            glVertex3d(0,0,0);
            glVertex3d(0,longitud,0);
        glEnd();
        glBegin(GL_LINES);
            glVertex3d(0,0,0);
            glVertex3d(longitud,0,0);
        glEnd();
    }
    glLineWidth(1);
}

// Composicion de rotaciones
void rotaciones(bool state){
    if(state){
        glRotatef(theta[0],1.0,0.0,0.0);
        glRotatef(theta[1],0.0,1.0,0.0);
        glRotatef(theta[2],0.0,0.0,1.0);
    }
}

//Rotacion de la figura
void spin(){
    theta[axis] += 0.05;
    if(theta[axis]>360) theta[axis] -= 360.0;
    display();
}

//Movimientos de rotacion y traslacion
void rotaCamara(){
    px1=px0+radio*cos(anguloVertical)*sin(angulo);
    pz1=pz0-radio*cos(anguloVertical)*cos(angulo);
}

void moverCamaraY(){
    px1=px0+radio*cos(anguloVertical)*sin(angulo);
    pz1=pz0-radio*cos(anguloVertical)*cos(angulo);
    py1=py0+radio*sin(anguloVertical);
}

//Escalamientos
void avanza(){
    px0=px1;pz0=pz1, py0 = py1;
    px1=px0+radio*cos(anguloVertical)*sin(angulo);
    pz1=pz0-radio*cos(anguloVertical)*cos(angulo);
    py1=py0+radio*sin(anguloVertical);
}

void retro(){
    px1=px0;pz1=pz0, py1 = py0;
    px0=px1-radio*cos(anguloVertical)*sin(angulo);
    pz0=pz1+radio*cos(anguloVertical)*cos(angulo);
    py0=py1-radio*sin(anguloVertical);
}

void teclado(unsigned char tecla,int x,int y){
    switch(tecla){
        case 'I' : avanza();break;
        case 'M' : retro(); break;
        case 'J' : angulo=angulo+incremento_angulo;rotaCamara(); break;
        case 'K' : angulo=angulo-incremento_angulo;rotaCamara(); break;
        case 'A' : axis = 0; break;
        case 'S' : axis = 1; break;
        case 'D' : axis = 2; break;
        case 'L' : anguloVertical+=incremento_angulo;moverCamaraY(); break;
        case 'P' : anguloVertical-=incremento_angulo;moverCamaraY(); break;
        case 'F' : exit(0); break;
    }
}

//Menus
void menuSistemaReferencial(int opc){
    switch(opc){
        case 1: sistema=1; ejes(5,true); break;
        case 2: sistema=2; ejes(5,false); break;
    }
}

void menuRepresentaciones(int opc){
    switch(opc){
        case 1: modo1=GL_LINE_LOOP; modo2=GL_LINES; stateEsfera=true; break;
        case 2: modo1=GL_POLYGON; modo2=GL_POLYGON; stateEsfera=false; break;
    }
}

void menuJuegoColores(int opc){
    switch(opc){
        case 1: color=1; break;
        case 2: color=2; break;
        case 3: color=3; break;
    }
}

void menuRotaciones(int opc){
    switch(opc){
        case 1: rotacion=1; rotaciones(true); break;
        case 2: rotacion=2; rotaciones(false); break;
    }
}

void mainMenu(int opc){
    switch(opc){
        case 1: modelo=1; trigonalPlano(); break;
        case 2: modelo=2; tetraedrica(); break;
        case 3: modelo=3; bipiramidePentagonal(); break;
        case 4: modelo=4; octaedrica(); break;
        case 5: modelo=5; bipiramideTrigonal(); break;
        case 6: exit(-1);
    }
    glutPostRedisplay();
}
