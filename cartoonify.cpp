/**
 assignment1.cpp
 Assignment-1: Cartoonify

 Name: Lindsey, Lauren

 Collaborators: Nagendran, Shanaya; Sage Strieker, Breelyn Betts

 Project Summary:  I decided to use Bezier's algorithm because I wanted to keep the
 endpoints of my original plotted points in the final curve, as I felt that would help me
 create the most accurate recreation of my image. I did an iterative approach
 because that seemed like the most straightforward option for someone who had just
 been introduced to Bezier's algorithm. For my image I chose to do baby Yoda, since
 I think he is cute and don't mind spending hours plotting points of his face on Desmos.
 */

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <vector>
#include <iostream>
using namespace std;


class Vertex {
    GLfloat x, y;
public:
    Vertex(GLfloat, GLfloat);
    GLfloat get_y() { return y; };
    GLfloat get_x() { return x; };
};

Vertex::Vertex(GLfloat X, GLfloat Y) {
    x = X;
    y = Y;
}

void setup() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

vector <Vertex> generate_points(vector <Vertex> control_points) {
  vector <Vertex> points = control_points;
  vector <Vertex> M, Q1, Q2, Q2_reverse, final_points;

  points = control_points;
  for (int i = 0; i < control_points.size() - 1; i++) {
    for (int j = 0; j < points.size() - 1; j++) {
      GLfloat x_value = 0.5 * (points[j].get_x() + points[j + 1].get_x());
      GLfloat y_value = 0.5 * (points[j].get_y() + points[j + 1].get_y());
      M.push_back(Vertex(x_value, y_value));
    }
    if (M.size() != 1) {
      Q1.push_back(M[0]);
      Q2.push_back(M[M.size() - 1]);
    } else {
      Q1.push_back(M[0]);
    }

    points.clear();
    for (int i = 0; i < M.size(); i++) {
      points.push_back(M[i]);
    }
    M.clear();
  }
    
  //combine points by adding index 0 of control points, Q1, the reverse of Q2, and the last item in control points
  final_points.push_back(control_points[0]);
  for (int i = 0; i < Q1.size(); i++) {
    final_points.push_back(Q1[i]);
  }
  for (int i = int(Q2.size() - 1); i >= 0; i--) {
     final_points.push_back(Q2[i]);
  }
  final_points.push_back(control_points[control_points.size() - 1]);

  return final_points;
}

void draw_curve(vector <Vertex> control_points, int n_iter) {

  for (int i = 1; i <= n_iter; i++) {
    control_points = generate_points(control_points);
  }
    
  glPointSize(0.1f);
  for (int i = 0; i < control_points.size() - 1; i++) {
    //draw points
    glBegin(GL_POINTS);
    glVertex2f(control_points[i].get_x(), control_points[i].get_y());
    glVertex2f(control_points[i + 1].get_x(), control_points[i + 1].get_y());
    glEnd();

    //draw lines
    glBegin(GL_LINES);
    glVertex2f(control_points[i].get_x(), control_points[i].get_y());
    glVertex2f(control_points[i + 1].get_x(), control_points[i + 1].get_y());
    glEnd();
  }
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0f, 1.0f, 0.3f);

    vector<Vertex> points;
    
    Vertex p0 = Vertex(-0.9f, 0.35f); //left side top of head
    Vertex p1 = Vertex(-.38f, 0.51f);
    Vertex p2 = Vertex(-0.067f, 0.682f);
    Vertex p3 = Vertex(0.048f, 0.682f);
    Vertex p4 = Vertex(0.38f, 0.51f);
    Vertex p5 = Vertex(0.82f, 0.42f);
    points.push_back(p0);
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
    points.push_back(p5);
    points = generate_points(points);
    draw_curve(points, 2);
    points.clear();
    
     Vertex p6 = Vertex(-0.9f, 0.35f); //bottom of ears and face
     Vertex p7 = Vertex(-0.594f, 0.272f);
     Vertex p8 = Vertex(-0.326f, 0.23f);
     Vertex p9 = Vertex(-0.09f, 0.089f);
     Vertex p10 = Vertex(0.057f, 0.101f);
     Vertex p11 = Vertex(0.32f, 0.24f);
     Vertex v7 = Vertex(0.56f, 0.27f);
     Vertex v8 = Vertex(0.82f, 0.42f);
     points.push_back(p6);
     points.push_back(p7);
     points.push_back(p8);
     points.push_back(p9);
     points.push_back(p10);
     points.push_back(p11);
     points.push_back(v7);
     points.push_back(v8);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     Vertex q0 = Vertex(-0.326f, 0.54f); //left side face
     Vertex q1 = Vertex(-0.34f, 0.38f);
     Vertex q2 = Vertex(-0.326f, 0.23f);
     points.push_back(q0);
     points.push_back(q1);
     points.push_back(q2);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     Vertex q3 = Vertex(0.32f, 0.54f); //right side face
     Vertex q4 = Vertex(0.34f, 0.362f);
     Vertex q5 = Vertex(0.32f, 0.24f);
     points.push_back(q3);
     points.push_back(q4);
     points.push_back(q5);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     //left eye
     Vertex q6 = Vertex(-0.073f, 0.333f);
     Vertex q7 = Vertex(-0.178f, 0.47f);
     Vertex q8 = Vertex(-0.25f, 0.362f);
     points.push_back(q6);
     points.push_back(q7);
     points.push_back(q8);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     Vertex q9 = Vertex(-0.073f, 0.333f);
     Vertex q10 = Vertex(-0.18f, 0.297f);
     Vertex q11 = Vertex(-0.25f, 0.362f);
     points.push_back(q9);
     points.push_back(q10);
     points.push_back(q11);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     //right eye
      Vertex v0 = Vertex(0.073f, 0.333f);
     Vertex v1 = Vertex(0.178f, 0.47f);
     Vertex v2 = Vertex(0.25f, 0.362f);
     points.push_back(v0);
     points.push_back(v1);
     points.push_back(v2);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     Vertex v3 = Vertex(0.073f, 0.333f);
     Vertex v4 = Vertex(0.18f, 0.297f);
     Vertex v5 = Vertex(0.25f, 0.362f);
     points.push_back(v3);
     points.push_back(v4);
     points.push_back(v5);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     //mouth - I know he's frowning in the picture, but I thought he looked better with a smile :)
     Vertex m0 = Vertex(-0.109f, 0.242f);
     Vertex m1 = Vertex(-0.094f, 0.215f);
     Vertex m2 = Vertex(0.103f, 0.218f);
     Vertex m3 = Vertex(0.109f, 0.242f);
     points.push_back(m0);
     points.push_back(m1);
     points.push_back(m2);
     points.push_back(m3);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     //nose
     Vertex v9 = Vertex(-0.042f, 0.304f);
     Vertex v10 = Vertex(0.0f, 0.337f);
     Vertex v11 = Vertex(0.042f, 0.304f);
     points.push_back(v9);
     points.push_back(v10);
     points.push_back(v11);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     Vertex s0 = Vertex(-0.042f, 0.304f);
     Vertex s1 = Vertex(0.0f, 0.291f);
     Vertex s2 = Vertex(0.042f, 0.304f);
     points.push_back(s0);
     points.push_back(s1);
     points.push_back(s2);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     //turtleneck
     Vertex s3 = Vertex(-0.45f, 0.246f);
     Vertex s4 = Vertex(-0.471f, 0.1f);
     Vertex s5 = Vertex(-0.518f, -0.01f);
     points.push_back(s3);
     points.push_back(s4);
     points.push_back(s5);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     Vertex s6 = Vertex(-0.518f, -0.01f);
     Vertex s7 = Vertex(-0.044f, -0.27f);
     Vertex s8 = Vertex(0.518f, -0.01f);
     points.push_back(s6);
     points.push_back(s7);
     points.push_back(s8);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     Vertex s9 = Vertex(0.45f, 0.246f);
     Vertex s10 = Vertex(0.471f, 0.1f);
     Vertex s11 = Vertex(0.518f, -0.01f);
     points.push_back(s9);
     points.push_back(s10);
     points.push_back(s11);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     //left arm
     glBegin(GL_LINES);
     glVertex2f(-0.518f, -0.01f);
     glVertex2f(-0.756f, -0.284f);
     glEnd();
    
     glBegin(GL_LINES);
     glVertex2f(-0.424f, -0.2f);
     glVertex2f(-0.458f, -0.559f);
     glEnd();
    
     Vertex k0 = Vertex(-0.756f, -0.284f);
     Vertex k1 = Vertex(-0.638, -0.473);
     Vertex k2 = Vertex(-0.458f, -0.559f);
     points.push_back(k0);
     points.push_back(k1);
     points.push_back(k2);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     //right arm
     glBegin(GL_LINES);
     glVertex2f(0.518f, -0.01f);
     glVertex2f(0.756f, -0.284f);
     glEnd();
    
     glBegin(GL_LINES);
     glVertex2f(0.424f, -0.2f);
     glVertex2f(0.458f, -0.559f);
     glEnd();
    
     Vertex k3 = Vertex(0.756f, -0.284f);
     Vertex k4 = Vertex(0.638, -0.473);
     Vertex k5 = Vertex(0.458f, -0.559f);
     points.push_back(k3);
     points.push_back(k4);
     points.push_back(k5);
     points = generate_points(points);
     draw_curve(points, 2);
     points.clear();
    
     //bottom of jacket
     glBegin(GL_LINES);
     glVertex2f(-0.458f, -0.559f);
     glVertex2f(-0.488f, -0.75f);
     glEnd();
    
     glBegin(GL_LINES);
     glVertex2f(0.458f, -0.559f);
     glVertex2f(0.488f, -0.75f);
     glEnd();
    
     glBegin(GL_LINES);
     glVertex2f(-0.072f, -0.184f);
     glVertex2f(-0.125f, -0.75f);
     glEnd();
    
     glBegin(GL_LINES);
     glVertex2f(0.065f, -0.182f);
     glVertex2f(0.017f, -0.75f);
     glEnd();
    
     glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Assignment 1");
    setup();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}


