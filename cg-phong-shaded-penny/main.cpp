//
//  main.cpp
//  HW4
//
//  Created by Pablo Guerra on 3/10/20.
//  Copyright © 2020 Pablo Guerra. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <math.h>
#include "shading.cpp"
using namespace std;

int displayMode = 3;            // Variable to change display modes
bool removeBackground = true;   // Toggle displaying the background
const int SIZE = 500;           // Size of image
float Px[SIZE][SIZE];           // x points from image
float Py[SIZE][SIZE];           // y points from image
float Pz[SIZE][SIZE];           // x points from image
int point_rVal[SIZE][SIZE];     // red color value from image
int point_gVal[SIZE][SIZE];     // green color value from image
int point_bVal[SIZE][SIZE];     // blue color value from image
float Nx[SIZE][SIZE];           // x normal
float Ny[SIZE][SIZE];           // y normal
float Nz[SIZE][SIZE];           // z normal
float depth_scale = 3000.0;     // value to divide for penny depth
float xangle = -15;             // rotation of image in x
float yangle = -15;             // rotation of image in y
float zangle = 0;               // rotation of image in z


// Initialize OpenGL
void init(){
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float radius = 1.2;
    glOrtho(-radius, radius, -radius, radius, -2, 2);
    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
}


// Access and read input file to obtain point depth
void getPixelDepth(string fileName) {
    float Xmin = -1.0;
    float Xmax = 1.0;
    float Ymin = -1.0;
    float Ymax = 1.0;
    
    fstream file;
    file.open(fileName);

    // Check if file can be accessed
    if(!file.good()){
        cout << "Unable to open penny-depth.txt \n";
        exit(0);
    }
    else{
        for(int x=0; x<500; x++){
            for(int y=0; y<500; y++){
                string data;
                getline(file, data, ' ');
                Px[x][y] = Xmin + x * (Xmax - Xmin) / SIZE;
                Py[x][y] = Ymin + y * (Ymax - Ymin) / SIZE;
                // Store depth value into array
                Pz[x][y] = stoi(data)/depth_scale;
            }
        }
    }
    file.close();
}



// Access and read file to obtain red green blue color values
void getPixelColor(string fileName) {
    fstream file;
    file.open(fileName);
    
    // Check if file can be accessed
    if(!file.good()){
        cout << "Unable to open penny-image.txt \n";
        exit(0);
    }
    else{
        for(int x=0; x<500; x++){
            for(int y=0; y<500; y++){
                string data;
                // Store color data into arrays
                getline(file, data, ' ');
                point_rVal[x][y] = stoi(data);
                
                getline(file, data, ' ');
                point_gVal[x][y] = stoi(data);
                
                getline(file, data, ' ');
                point_bVal[x][y] = stoi(data);
                
                getline(file, data, ' ');
            }
        }
    }
    file.close();
}



// Create mesh version of penny
void mesh(){
    for (int x = 0; x < 500; x = x+2){
        for (int y = 0; y < 500; y= y+2){
                glBegin(GL_LINE_LOOP);
                glColor3f(1.0, 1.0, 1.0);
                glVertex3f(Px[x][y], Py[x][y], Pz[x][y]);
                glVertex3f(Px[x+1][y], Py[x+1][y], Pz[x+1][y]);
                glVertex3f(Px[x+1][y+1], Py[x+1][y+1], Pz[x+1][y+1]);
                glVertex3f(Px[x][y+1], Py[x][y+1], Pz[x][y+1]);
                glEnd();
       }
    }
}



// Create colored version of penny
void color_display(){
    for (int x = 0; x < 500; x++){
        for (int y = 0; y < 500; y++){
            if(point_rVal[x][y] > 235 && point_gVal[x][y] > 235 && point_bVal[x][y] > 235 && removeBackground){
                continue;
            }
                glBegin(GL_POLYGON);
                glColor3f(point_rVal[x][y]/255.0, point_gVal[x][y]/255.0, point_bVal[x][y]/255.0);
                glVertex3f(Px[x][y], Py[x][y], Pz[x][y]);
                
                glColor3f(point_rVal[x+1][y]/255.0, point_gVal[x+1][y]/255.0, point_bVal[x+1][y]/255.0);
                glVertex3f(Px[x+1][y], Py[x+1][y], Pz[x+1][y]);
                
                glColor3f(point_rVal[x][y+1]/255.0, point_gVal[x][y+1]/255.0, point_bVal[x][y+1]/255.0);
                glVertex3f(Px[x+1][y+1], Py[x+1][y+1], Pz[x+1][y+1]);
                
                glColor3f(point_rVal[x+1][y+1]/255.0, point_gVal[x+1][y+1]/255.0, point_bVal[x+1][y+1]/255.0);
                glVertex3f(Px[x][y+1], Py[x][y+1], Pz[x][y+1]);
                
                glEnd();
       }
    }
}



// Create phong shaded version of penny
void phong_display(){
     for (int x = 0; x < 499; x++){
           for (int y = 0; y < 499; y++){
               if(point_rVal[x][y] > 235 && point_gVal[x][y] > 235 && point_bVal[x][y] > 235 && removeBackground){
                   continue;
               }
                   glBegin(GL_POLYGON);
                   
                   glColor3f(point_rVal[x][y]/255.0, point_gVal[x][y]/255.0, point_bVal[x][y]/255.0);
                   glNormal3f(Nx[x][y], Ny[x][y], Nz[x][y]);
                   glVertex3f(Px[x][y], Py[x][y], Pz[x][y]);
                   
                   glColor3f(point_rVal[x+1][y]/255.0, point_gVal[x+1][y]/255.0, point_bVal[x+1][y]/255.0);
                   glNormal3f(Nx[x+1][y], Ny[x+1][y], Nz[x+1][y]);
                   glVertex3f(Px[x+1][y], Py[x+1][y], Pz[x+1][y]);
                   
                   glColor3f(point_rVal[x][y+1]/255.0, point_gVal[x][y+1]/255.0, point_bVal[x][y+1]/255.0);
                   glNormal3f(Nx[x][y+1], Ny[x][y+1], Nz[x][y+1]);
                   glVertex3f(Px[x+1][y+1], Py[x+1][y+1], Pz[x+1][y+1]);
                   
                   glColor3f(point_rVal[x+1][y+1]/255.0, point_gVal[x+1][y+1]/255.0, point_bVal[x+1][y+1]/255.0);
                   glNormal3f(Nx[x][y+1], Ny[x][y+1], Nz[x][y+1]);
                   glVertex3f(Px[x][y+1], Py[x][y+1], Pz[x][y+1]);
                   
                   glEnd();
          }
       }
}



// Initialize surface normals
void init_normals(){
    for (int x=0; x<SIZE-1; x++){
       for (int y=0; y<SIZE-1; y++){
          // Get tangents S and T
          float Sx = Px[x+1][y] - Px[x][y];
          float Sy = Py[x+1][y] - Py[x][y];
          float Sz = Pz[x+1][y] - Pz[x][y];
          float Tx = Px[x][y+1] - Px[x][y];
          float Ty = Py[x][y+1] - Py[x][y];
          float Tz = Pz[x][y+1] - Pz[x][y];
          
          // Calculate N = S cross T
          Nx[x][y] = Sy*Tz - Sz*Ty;
          Ny[x][y] = Sz*Tx - Sx*Tz;
          Nz[x][y] = Sx*Ty - Sy*Tx;

          // Make unit vector
          float length = sqrt(Nx[x][y]*Nx[x][y]
                       + Ny[x][y]*Ny[x][y]
                       + Nz[x][y]*Nz[x][y]);
          if (length > 0){
             Nx[x][y] /= length;
             Ny[x][y] /= length;
             Nz[x][y] /= length;
          }
       }

       // Handle last row and column
       for (int x=0; x<SIZE; x++){
          Nx[x][SIZE-1] = Nx[x][SIZE-2];
          Ny[x][SIZE-1] = Ny[x][SIZE-2];
          Nz[x][SIZE-1] = Nz[x][SIZE-2];
          Nx[SIZE-1][x] = Nx[SIZE-2][x];
          Ny[SIZE-1][x] = Ny[SIZE-2][x];
          Nz[SIZE-1][x] = Nz[SIZE-2][x];
       }
    }
}


// Keyboard callback for OpenGL
void keyboard(unsigned char key, int x, int y)
{
   // Update angles
   if (key == 'q')
       xangle -= 5;
   else if (key == 'w')
       xangle += 5;
   else if (key == 'a')
       yangle -= 5;
   else if (key == 's')
       yangle += 5;
   else if (key == 'z')
       zangle -= 5;
   else if (key == 'x')
       zangle += 5;
   else if (key == 't')
       removeBackground = removeBackground == true ? false : true;
   else if (key == '1')
       displayMode = 1;
   else if (key == '2')
        displayMode = 2;
   else if (key == '3')
        displayMode = 3;

   // Redraw objects
   glutPostRedisplay();
}


// Display callback for OpenGL
void display(){
    #ifdef __APPLE__
        glViewport(0, 0, 1000, 1000);
    #endif
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-90, 0.0, 0.0, 1.0);
    glRotatef(xangle, 1.0, 0.0, 0.0);
    glRotatef(yangle, 0.0, 1.0, 0.0);
    glRotatef(zangle, 0.0, 0.0, 1.0);
    
    if(displayMode == 1){
        glDisable(GL_LIGHTING);
        mesh();
    }
    else if(displayMode == 2){
        glDisable(GL_LIGHTING);
        color_display();
    }
    else if(displayMode == 3){
        init_light(GL_LIGHT0, 0, 1, 1, 0.5, 0.5, 0.5);
        init_light(GL_LIGHT1, 0, 0, 1, 0.5, 0.5, 0.5);
        init_light(GL_LIGHT2, 0, 1, 0, 0.5, 0.5, 0.5);
        init_material(Ka, Kd, Ks, 100 * Kp, 0.8, 0.6, 0.4);
        phong_display();
    }
    glFlush();
}


// Main program
int main(int argc, char *argv[])
{
    getPixelDepth("txt/penny-depth.txt");
    getPixelColor("txt/penny-image.txt");
    init_normals();
    
    cout << endl << "Change modes: [1] Wire mesh [2] Color display [3] Phong Shading" << endl;
    cout << "Use [q] and [w] to rotate around Y axis" << endl;
    cout << "Use [a] and [s] to rotate around X axis" << endl;
    cout << "Use [z] and [x] to rotate around Z axis" << endl;
    cout << "Press [t] to display/hide background" << endl;
    
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(500, 100);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Penny");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
    return 0;
}

