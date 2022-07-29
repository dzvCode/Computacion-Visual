#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

void init(float red, float green, float blue);
void display(void);
void reshape(int w, int h);
//
void mariposa(void);
void corazon(void);
void rosaPolar(void);
//
void ejes2D(void);
void coordenadasPolares(void);
//
void menuPrincipal(int opc);
void menuColor(int opc);
void menu_grosorL(int opc);
void menu_principal(int opc);
void menuSistemaCoordenado(int opc);
void menuCurvas(int opc);
void patronLinea(int);
void menu_patronLinea(int);

GLint opc=0;
GLint sistema=0;
GLint curva=0;
GLint color=0;
GLint grosor=0;
GLint patron=0;

int main(int argc, char **argv){
    glutInit(&argc, argv);//funcion para inicializar la biblioteca GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(400,100);
    glutCreateWindow("PC-1 | Pairazaman Arias y Zavala Sanchez");

    //Sistema referencial
    int submenuSistemaCoordenado = glutCreateMenu(menuSistemaCoordenado);
        glutAddMenuEntry("Cartesianas",1);
        glutAddMenuEntry("Polares",2);
        glutAddMenuEntry("Ninguno",0);

    //Curvas Polares
    int submenuCurvas = glutCreateMenu(menuCurvas);
        glutAddMenuEntry("Mariposa",1);
        glutAddMenuEntry("Corazon",2);
        glutAddMenuEntry("Rosa polar",3);

    //Grosor de pintado
    int submenu_grosor = glutCreateMenu(menu_grosorL);
        glutAddMenuEntry("1",1);
        glutAddMenuEntry("3",2);
        glutAddMenuEntry("5",3);
        glutAddMenuEntry("7",4);

    //Color
    int submenuColor = glutCreateMenu(menuColor);
        glutAddMenuEntry("Azul",1);
        glutAddMenuEntry("Rojo",2);
        glutAddMenuEntry("Verde",3);

    //Patron linea
    int submenu_patronLinea=glutCreateMenu(menu_patronLinea);
        glutAddMenuEntry("Opcion 1",1);
        glutAddMenuEntry("Opcion 2",2);
        glutAddMenuEntry("Restablecer",3);

    //// MENU ////
    int menu = glutCreateMenu(menuPrincipal);
        glutAddSubMenu("Sistema referencial",submenuSistemaCoordenado);
        glutAddSubMenu("Curvas polares",submenuCurvas);
        glutAddSubMenu("Grosor de pintado",submenu_grosor);
        glutAddSubMenu("Color",submenuColor);
        glutAddSubMenu("Patron de linea",submenu_patronLinea);
        glutAddMenuEntry("Salir",1);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glColor3f(0.0f,0.0f,0.0f);
    init(1.0f,1.0f,1.0f);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}

/*GLUT*/
void init(float red, float green, float blue){
    glClearColor(red, green, blue, 0.0);//color de fondo
    glShadeModel(GL_FLAT);//necesario
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //necesario
    menuPrincipal(opc);
    menuColor(color);
    menu_grosorL(grosor);
    menu_patronLinea(patron);
    menuCurvas(curva);

    menuSistemaCoordenado(sistema);

    menuColor(color);
    menu_grosorL(grosor);
    menu_patronLinea(patron);

    glFlush();//necesario
    glutSwapBuffers();//necesario
}

void reshape(int w, int h){
    //necesario
    glViewport(0,0,(GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //proyeccion ortografica
    glOrtho(-10.0,10.0,-10.0,10,-10.0,10.0);//caja negra
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*FUNCIONES PARA LOS SISTEMAS REFERENCIALES*/
void ejes2D(void){
    glColor3f(0.0f,0.0f,0.0f);
    glLineWidth(1);
    glDisable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex2f(-10,0);
    glVertex2f(10,0);
    glVertex2f(0,10);
    glVertex2f(0,-10);
    glEnd();
}

void coordenadasPolares(void){
    glColor3f(0.0f,0.0f,0.0f);
    glLineWidth(1);
    glDisable(GL_LINE_STIPPLE);
    GLUquadricObj *pt;
    pt=gluNewQuadric() ;
    //Renderizamos con lineas
    gluQuadricDrawStyle(pt,GLU_LINE);
    //Se dibujan los discos
    gluDisk(pt,0,14,50,20);
}

/*FUNCIONES DE LAS CURVAS POLARES*/
void mariposa(void){
    float ang, radio , x, y;

    glBegin(GL_LINE_LOOP);
    for (ang = 0.0; ang < 2 * M_PI; ang += 2*M_PI/50){
        radio = pow(M_E, cos(ang))-2*cos(4*ang)+pow(sin((2*ang-M_PI)/24),2);
        x = radio * cos(ang);
        y = radio * sin(ang);
        glVertex2f(x,y);
    }
    glEnd();
}

void corazon(void){
    float ang, radio , x, y;

    glBegin(GL_LINE_LOOP);
    for (ang = 0.0; ang < 2 * M_PI; ang += 2*M_PI/50){
        radio = sin(ang)*(pow(abs(cos(ang)),0.5)/(sin(ang)+7/5))-2*sin(ang)+2;
        x = radio * cos(ang);
        y = radio * sin(ang);
        glVertex2f(x,y);
    }
    glEnd();
}

void rosaPolar(void){
    float ang, radio , x, y;

    glBegin(GL_LINE_LOOP);
    for (ang = 0.0; ang < 2 * M_PI; ang += 2*M_PI/50){
        radio = 2*cos(4*ang);
        x = radio * cos(ang);
        y = radio * sin(ang);
        glVertex2f(x,y);
    }
    glEnd();
}

/*FUNCION PARA MODIFICAR EL PATRON DE LINEA*/
void patronLinea(int factor){
    glLineStipple(factor,0x00FF);
    glEnable(GL_LINE_STIPPLE);
}

/*----MENUS----*/
void menuSistemaCoordenado(int opc){
    switch(opc){
        case 1: sistema=1; ejes2D(); break;//cartesiano
        case 2: sistema=2; coordenadasPolares(); break;//polares
        case 0: sistema=0; break;
    }
}

void menuColor(int opc){
    switch(opc){
        case 1: color=1; glColor3f(0.0f, 0.0f, 1.0f); break;//azul
        case 2: color=2; glColor3f(1.0f, 0.0f, 0.0f); break;//rojo
        case 3: color=3; glColor3f(0.0f, 1.0f, 0.0f); break;//verde
    }
}

void menu_grosorL(int opc){
    switch(opc){
        case 1: grosor=1; glLineWidth(1); break;
        case 2: grosor=2; glLineWidth(3); break;
        case 3: grosor=3; glLineWidth(5); break;
        case 4: grosor=4; glLineWidth(7); break;
    }
}

void menuCurvas(int opc){
    switch(opc){
        case 1: curva=1; mariposa(); break;
        case 2: curva=2; corazon(); break;
        case 3: curva=3; rosaPolar(); break;
    }
}

void menu_patronLinea(int opc){
    switch(opc){
        case 1: patron=1; patronLinea(1); break;
        case 2: patron=2; patronLinea(2); break;
        case 3: patron=3; glDisable(GL_LINE_STIPPLE); break;
    }
}

void menuPrincipal(int opc){
    switch(opc){
        case 1: exit(-1);//Se sale de la ventana
    }
    glutPostRedisplay();
}
