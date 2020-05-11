/***
 Assignment-3: Geometric Modeling of a Scene
 
 Name:  Lindsey, Lauren
 
 Collaborators: N/A
 
 Project Summary: This assignment uses rotation matrices around the x, y, and z axes as well as a scaling and translation matrix to create objects in a scene. This begins with a rendering of a cube, created by making one plane, and duplicating that plane by rotating and translating to get all six cube faces.
 ***/


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

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
using namespace std;

// If a float is < EPSILON or > -EPILSON then it should be 0
float EPSILON = 0.000001;
// theta is the angle to rotate the scene
float THETA = 0.0;
// Vector placeholders for the scene and color array
vector<GLfloat> SCENE;
vector<GLfloat> COLOR;

// Initializes a square plane of unit lengths
vector<GLfloat> init_plane() {
    vector<GLfloat> vertices = {
        +0.5,   +0.5,   +0.0,
        -0.5,   +0.5,   +0.0,
        -0.5,   -0.5,   +0.0,
        +0.5,   -0.5,   +0.0
    };
    return vertices;
}

// Converts degrees to radians for rotation
float deg2rad(float d) {
    return (d*M_PI) / 180.0;
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
vector<GLfloat> to_homogeneous_coord(vector<GLfloat> cartesian_coords) {
    vector<GLfloat> result;
    for (int i = 0; i < cartesian_coords.size(); i++) {
        result.push_back(cartesian_coords[i]);
        if ((i+1) % 3 == 0) {
            result.push_back(1.0);
        }
    }
    return result;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogeneous_coords) {
    vector<GLfloat> result;
    for (int i = 0; i < homogeneous_coords.size(); i++) {
        if ((i+1) % 4 == 0) {
            continue;
        } else {
            result.push_back(homogeneous_coords[i]);
        }
    }
    return result;
}

// Definition of a translation matrix
vector<GLfloat> translation_matrix (float dx, float dy, float dz) {
    vector<GLfloat> translate_mat;
    
    translate_mat = {
        1.0,          0.0,          0.0,     dx,
        0.0,         1.0,           0.0,     dy,
        0.0,         0.0,          1.0,      dz,
        0.0,         0.0,          0.0,     1.0
    };
    return translate_mat;
}

// Definition of a scaling matrix
vector<GLfloat> scaling_matrix (float sx, float sy, float sz) {
    vector<GLfloat> scale_mat;
    
    scale_mat = {
        sx,          0.0,          0.0,     0.0,
        0.0,         sy,           0.0,     0.0,
        0.0,         0.0,          sz,      0.0,
        0.0,         0.0,          0.0,     1.0
    };
    
    return scale_mat;
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

// Definition of a rotation matrix about the y-axis by theta degrees
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

// Definition of a rotation matrix about the z-axis by theta degrees
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
    vector<GLfloat> result;
    for (int i = 0; i < B.size(); i += 4) {
        for (int j = 0; j < A.size(); j += 4) {
            GLfloat dot_product = 0.0;
            for (int k = 0; k < 4; k++){
                dot_product += A[j+k] * B[i+k];
            }
            result.push_back(dot_product);
        }
    }
    return result;
}

// Builds a unit cube centered at the origin
vector<GLfloat> build_cube() {
    vector<GLfloat> plane, result, front, back, right, left, top, bottom;
    
    plane = to_homogeneous_coord(init_plane());
    
    //front plane is centered at 0, 0 ,0.5
    front = mat_mult(translation_matrix(0.0, 0.0, 0.5), plane);
    
    // rotate back plane by 180 degrees on the y axis so the front and back normals both face outwards. back plane is centered at 0, 0, -0.5
    back = mat_mult(translation_matrix(0.0, 0.0, -0.5), mat_mult(rotation_matrix_y(180.0), plane));
    
    // rotate right plane by 90 degrees on the y axis. right plane is centered at 0.5, 0, 0
    right = mat_mult(translation_matrix(0.5, 0.0, 0.0), mat_mult(rotation_matrix_y(90.0), plane));
    
    // rotate left plane by -90 degrees on the y axis. left plane is centered at -0.5, 0, 0
    left = mat_mult(translation_matrix(-0.5, 0.0, 0.0), mat_mult(rotation_matrix_y(-90.0), plane));
    
    // rotate top plane by -90 degrees on the x axis. top plane is centered at 0, 0.5, 0
    top = mat_mult(translation_matrix(0.0, 0.5, 0.0), mat_mult(rotation_matrix_x(-90.0), plane));
    
    // rotate bottom plane by 90 degrees on the x axis. bottom plane is centered at 0, -0.5, 0
    bottom = mat_mult(translation_matrix(0.0, -0.5, 0.0), mat_mult(rotation_matrix_x(90.0), plane));
    
    front = to_cartesian_coord(front);
    left = to_cartesian_coord(left);
    right = to_cartesian_coord(right);
    back = to_cartesian_coord(back);
    top = to_cartesian_coord(top);
    bottom = to_cartesian_coord(bottom);
    
    result.insert(result.end(), front.begin(), front.end());
    result.insert(result.end(), left.begin(), left.end());
    result.insert(result.end(), right.begin(), right.end());
    result.insert(result.end(), back.begin(), back.end());
    result.insert(result.end(), top.begin(), top.end());
    result.insert(result.end(), bottom.begin(), bottom.end());
    
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
    //params for gluPerspective: fovy, aspect, zNear, zFar
    gluPerspective(50.0, 1.0, 2.0, 15.0);
    // Position camera at (2, 3, 5), attention at (0, 0, 0), up at (0, 1, 0)
    gluLookAt(2.0, 6.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Construct the scene using objects built from cubes/prisms
vector<GLfloat> init_scene() {
    vector<GLfloat> scene, base;
    
    //bed
    vector<GLfloat> bed, pillow;
    
    //desk
    vector<GLfloat> desk, desk_leg1, desk_leg2, desk_leg3, desk_leg4;
    
    //shelves
    vector<GLfloat> shelf1, shelf2, shelf3;
    
    //dresser
    vector<GLfloat> dresser;
    
    // bookshelf
    vector<GLfloat> bookshelf_left, bookshelf_right, bs1, bs2, bs3, bs4, bs5;
    
    base = to_homogeneous_coord(build_cube());
    
    // Rotation, Scale, and Translate points
    //bed
    bed = mat_mult(scaling_matrix(1, 1, 2.5), base);
    bed = mat_mult(translation_matrix(-1, -1, 0), bed);
    pillow = mat_mult(scaling_matrix(1, 0.25, 0.5), base);
    pillow = mat_mult(translation_matrix(-1, -0.25, -1), pillow);
    
    //desk
    desk = mat_mult(scaling_matrix(1, 0.25, 1.75), base);
    desk = mat_mult(rotation_matrix_y(90), desk);
    desk = mat_mult(translation_matrix(0.5, 0, -0.5), desk);
    desk_leg1 = mat_mult(scaling_matrix(1.5, 0.1, 0.1), base);
    desk_leg1 = mat_mult(rotation_matrix_z(-90), desk_leg1);
    desk_leg1 = mat_mult(translation_matrix(-0.25, -0.9, -0.9), desk_leg1);
    desk_leg2 = mat_mult(translation_matrix(0, 0, 0.75), desk_leg1);
    desk_leg3 = mat_mult(translation_matrix(1.5, 0, 0.75), desk_leg1);
    desk_leg4 = mat_mult(translation_matrix(1.5, 0, 0), desk_leg1);
    
    //shelves
    shelf1 = mat_mult(rotation_matrix_y(90), desk);
    shelf1 = mat_mult(translation_matrix(4, 1, 0), shelf1);
    shelf1 = mat_mult(scaling_matrix(0.5, 0.25, 0.75), shelf1);
    shelf2 = mat_mult(translation_matrix(0, 0.5, 0), shelf1);
    shelf3 = mat_mult(translation_matrix(0, 0.5, 0), shelf2);
    
    //dresser
    dresser = mat_mult(translation_matrix(4, 0, 1), bed);
    dresser = mat_mult(scaling_matrix(0.6, 1, 0.75), dresser);
    
    //bookshelf
    bookshelf_left = mat_mult(rotation_matrix_x(90), dresser);
    bookshelf_left = mat_mult(translation_matrix(0.5, 0.4, 20), bookshelf_left);
    bookshelf_left = mat_mult(scaling_matrix(0.75, 1.25, 0.1), bookshelf_left);
    bookshelf_right = mat_mult(translation_matrix(0, 0, 1), bookshelf_left);
    bs1 = mat_mult(translation_matrix(0, -1.75, 3.8), shelf1);
    bs1 = mat_mult(scaling_matrix(1, 1, 0.7), bs1);
    bs2 = mat_mult(translation_matrix(0, 0.5, 0), bs1);
    bs3 = mat_mult(translation_matrix(0, 1, 0), bs1);
    bs4 = mat_mult(translation_matrix(0, 1.7, 0), bs1);
    bs5 = mat_mult(translation_matrix(0, 2.2, 0), bs1);
    
    // Convert to cartesian coordinates
    // bed
    bed = to_cartesian_coord(bed);
    pillow = to_cartesian_coord(pillow);
    
    // desk
    desk = to_cartesian_coord(desk);
    desk_leg1 = to_cartesian_coord(desk_leg1);
    desk_leg2 = to_cartesian_coord(desk_leg2);
    desk_leg3 = to_cartesian_coord(desk_leg3);
    desk_leg4 = to_cartesian_coord(desk_leg4);
    
    //shelves
    shelf1 = to_cartesian_coord(shelf1);
    shelf2 = to_cartesian_coord(shelf2);
    shelf3 = to_cartesian_coord(shelf3);
    
    //dresser
    dresser = to_cartesian_coord(dresser);
    
    // bookshelf
    bookshelf_left = to_cartesian_coord(bookshelf_left);
    bookshelf_right = to_cartesian_coord(bookshelf_right);
    bs1 = to_cartesian_coord(bs1);
    bs2 = to_cartesian_coord(bs2);
    bs3 = to_cartesian_coord(bs3);
    bs4 = to_cartesian_coord(bs4);
    bs5 = to_cartesian_coord(bs5);
    
    // Add items to scene
    // bed
    scene.insert(scene.end(), bed.begin(), bed.end());
    scene.insert(scene.end(), pillow.begin(), pillow.end());
    
    // desk
    scene.insert(scene.end(), desk.begin(), desk.end());
    scene.insert(scene.end(), desk_leg1.begin(), desk_leg1.end());
    scene.insert(scene.end(), desk_leg2.begin(), desk_leg2.end());
    scene.insert(scene.end(), desk_leg3.begin(), desk_leg3.end());
    scene.insert(scene.end(), desk_leg4.begin(), desk_leg4.end());
    
    // shelves
    scene.insert(scene.end(), shelf1.begin(), shelf1.end());
    scene.insert(scene.end(), shelf2.begin(), shelf2.end());
    scene.insert(scene.end(), shelf3.begin(), shelf3.end());
    
    // dresser
    scene.insert(scene.end(), dresser.begin(), dresser.end());
    
    // bookshelf
    scene.insert(scene.end(), bookshelf_left.begin(), bookshelf_left.end());
    scene.insert(scene.end(), bookshelf_right.begin(), bookshelf_right.end());
    scene.insert(scene.end(), bs1.begin(), bs1.end());
    scene.insert(scene.end(), bs2.begin(), bs2.end());
    scene.insert(scene.end(), bs3.begin(), bs3.end());
    scene.insert(scene.end(), bs4.begin(), bs4.end());
    scene.insert(scene.end(), bs5.begin(), bs5.end());
    
    return scene;
}

// Construct the color mapping of the scene
vector<GLfloat> init_color(vector<GLfloat> scene) {
    vector<GLfloat> colors;
    for (int i = 0; i < scene.size(); i++) {
        colors.push_back(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    }
    return colors;
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    SCENE = to_cartesian_coord(mat_mult(rotation_matrix_y(THETA), to_homogeneous_coord(init_scene())));
    
    GLfloat* scene_vertices = vector2array(SCENE);
    GLfloat* color_vertices = vector2array(COLOR);
    // Pass the scene vertex pointer
    glVertexPointer(3,                // 3 components (x, y, z)
                       GL_FLOAT,         // Vertex type is GL_FLOAT
                       0,                // Start position in referenced memory
                       scene_vertices);  // Pointer to memory location to read from
       // Pass the color vertex pointer
       glColorPointer(3,                   // 3 components (r, g, b)
                      GL_FLOAT,            // Vertex type is GL_FLOAT
                      0,                   // Start position in referenced memory
                      color_vertices);     // Pointer to memory location to read from
       // Draw quad point planes: each 4 vertices
       glDrawArrays(GL_QUADS, 0, SCENE.size()/3.0);
       glFlush();            //Finish rendering
       glutSwapBuffers();
}

void idle_func() {
    THETA = THETA + 0.3;
    display_func();
}

int main (int argc, char **argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Assignment 3");
    
    setup();
    init_camera();
    // Setting global variables SCENE and COLOR with actual values
    SCENE = init_scene();
    COLOR = init_color(SCENE);
    
    // Set up our display function
    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    // Render our world
    glutMainLoop();
    
    return 0;
}

