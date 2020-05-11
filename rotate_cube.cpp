/**
 Assignment-2: Rotating a Cube in 3-Dimensional Space
 
 Name: Lindsey, Lauren
 
 Collaborators: Nagendran, Shanaya
 
 Project Summary: This assignment is an application of concepts learned in class, specifically how to rotate a set of points around  different axes. This program sets up the rotation matrix for x, y, and z axes in order to rotate a set of points - which forms a cube - in a viewing space. Specific colors are used for cube faces to distinguish them from one another to see the rotation.
 **/

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#pragma GCC diagnostic pop

#include <math.h>
#include <vector>
#include <iostream>
using namespace std;


float theta = 0.0;
vector<GLfloat> final_points, rotated_points;

// Converts degrees to radians for rotation
float deg2rad(float d) {
    return (d*M_PI)/180.0;
}

// Converts a vector to an array
GLfloat* vector2array(vector<GLfloat> vec) {
    GLfloat* arr = new GLfloat[vec.size()];
    for (int i = 0; i < vec.size(); i++) {
        arr[i] = vec[i];
    }
    return arr;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_homogenous_coord(vector<GLfloat> cartesian_coords) {
    vector<GLfloat> result;
    int size = cartesian_coords.size();
    GLfloat* arr = vector2array(cartesian_coords);
    for(int i = 0; i < size; i++) {
        result.push_back(arr[i]);
        if(i != 0 && (i+1) % 3 == 0) {
            result.push_back(1);
        };
    };
    return result;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogenous_coords) {
    vector<GLfloat> result;
    int size = homogenous_coords.size()-1;
    GLfloat* arr = vector2array(homogenous_coords);
    for(int i = 0; i < size; i++) {
        if(!(i != 0 && (i+1) % 4 == 0)) {
            result.push_back(arr[i]);
        }
    };
    return result;
}

// Definition of a rotation matrix about the x-axis theta degrees
vector<GLfloat> rotation_matrix_x (float theta) {
    vector<GLfloat> rotate_mat_x;
    theta = deg2rad(theta);
    
    // Define the rotation matrix about the x-axis
    rotate_mat_x = {
        1.0,          0.0,          0.0,    0.0,
        0.0,   cos(theta),   -sin(theta),   0.0,
        0.0,   sin(theta),    cos(theta),   0.0,
        0.0,          0.0,           0.0,   1.0
    };
    
    return rotate_mat_x;
}

// Definition of a rotation matrix along the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y (float theta) {
    vector<GLfloat> rotate_mat_y;
    theta = deg2rad(theta);
    
    // Define the rotation matrix about the y-axis
    rotate_mat_y = {
        cos(theta),    0.0,   sin(theta),   0.0,
        0.0,           1.0,           0.0,  0.0,
        -sin(theta),   0.0,    cos(theta),  0.0,
        0.0,           0.0,           0.0,  1.0
    };
    
    return rotate_mat_y;
}

// Definition of a rotation matrix along the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z (float theta) {
    vector<GLfloat> rotate_mat_z;
    theta = deg2rad(theta);
    
    //Define the rotation matrix about the z-axis
    rotate_mat_z = {
        cos(theta),   -sin(theta),   0.0,   0.0,
        sin(theta),   cos(theta),    0.0,   0.0,
        0.0,          0.0,           1.0,   0.0,
        0.0,          0.0,           0.0,   1.0
    };
    
    return rotate_mat_z;
}

// Perform matrix multiplication for A B
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
    //A holds transition matrix, B holds point
    vector<GLfloat> result = { 0.0, 0.0, 0.0, 0.0 };
    for(int i = 0; i < 16; i  = i + 4) { //iterate through the rows in A
        int index =  i == 0 ? 0: i / 4 ;
        for(int j = 0; j < 4; j++) { //iterate through the items in A
            result[index] += A[i+ j] * B[j];
        }
    }
    return result;
}

void setup() {
    // Enable the vertex array functionality
    glEnableClientState(GL_VERTEX_ARRAY);
    // Enable the color array functionality (so we can specify a color for each vertex)
    glEnableClientState(GL_COLOR_ARRAY);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Set up some default base color
    glColor3f(0.5, 0.5, 0.5);
    // Set up white background
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

void init_camera() {
    // Camera parameters
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a 50 degree field of view, 1:1 aspect ratio, near and far planes at 3 and 7
    gluPerspective(50.0, 1.0, 2.0, 10.0);
    // Position camera at (2, 3, 5), attention at (0, 0, 0), up at (0, 1, 0)
    gluLookAt(2.0, 6.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // World model parameters
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    vector<GLfloat> points = {
        // Front plane
        +1.0,   +1.0,   +1.0,
        -1.0,   +1.0,   +1.0,
        -1.0,   -1.0,   +1.0,
        +1.0,   -1.0,   +1.0,
        // Back plane
        +1.0,   +1.0,   -1.0,
        -1.0,   +1.0,   -1.0,
        -1.0,   -1.0,   -1.0,
        +1.0,   -1.0,   -1.0,
        // Right
        +1.0,   +1.0,   -1.0,
        +1.0,   +1.0,   +1.0,
        +1.0,   -1.0,   +1.0,
        +1.0,   -1.0,   -1.0,
        // Left
        -1.0,   +1.0,   -1.0,
        -1.0,   +1.0,   +1.0,
        -1.0,   -1.0,   +1.0,
        -1.0,   -1.0,   -1.0,
        // Top
        +1.0,   +1.0,   +1.0,
        -1.0,   +1.0,   +1.0,
        -1.0,   +1.0,   -1.0,
        +1.0,   +1.0,   -1.0,
        // Bottom
        +1.0,   -1.0,   +1.0,
        -1.0,   -1.0,   +1.0,
        -1.0,   -1.0,   -1.0,
        +1.0,   -1.0,   -1.0,
    };
    
    GLfloat colors[] = {
        // Front plane
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        // Back plane
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        // Right
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        // Left
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        // Top
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        // Bottom
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
    };
    
    // Apply rotation(s) to the set of points
    vector<GLfloat> final_points;
    for(int i = 0; i < points.size(); i+=3 ) {
        GLfloat x = points[i];
        GLfloat y = points[i + 1];
        GLfloat z = points[i + 2];
        vector<GLfloat> new_points = {x, y, z};
        
        //convert to homogenous coordinates
        new_points = to_homogenous_coord(new_points);
        
        float radTheta = deg2rad(theta);
        
        // rotate around x axis
        new_points = (mat_mult(rotation_matrix_x(radTheta), new_points));
        
        // rotate around y axis
        new_points = (mat_mult(rotation_matrix_y(radTheta), new_points));

        //rotate around z axis
        new_points = (mat_mult(rotation_matrix_z(radTheta), new_points));
        
        // convert to cartesian coordinates
        new_points = to_cartesian_coord(new_points);
        
        // add new points to the end of final points
        final_points.push_back(GLfloat(new_points[0]));
        final_points.push_back(GLfloat(new_points[1]));
        final_points.push_back(GLfloat(new_points[2]));
    };
    
    GLfloat* vertices = vector2array(final_points);
    
    glVertexPointer(3,          // 3 components (x, y, z)
                    GL_FLOAT,   // Vertex type is GL_FLOAT
                    0,          // Start position in referenced memory
                    vertices);  // Pointer to memory location to read from
    
    //pass the color pointer
    glColorPointer(3,           // 3 components (r, g, b)
                   GL_FLOAT,    // Vertex type is GL_FLOAT
                   0,           // Start position in referenced memory
                   colors);     // Pointer to memory location to read from
    
    // Draw quad point planes: each 4 vertices
    glDrawArrays(GL_QUADS, 0, 4*6);
    
    glFlush();            //Finish rendering
    glutSwapBuffers();
}

void idle_func() {
    theta = theta+45.0;
    display_func();
}

int main (int argc, char **argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Assignment 2");
    
    setup();
    init_camera();
    
    // Set up our display function
    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    // Render our world
    glutMainLoop();
    return 0;
}


