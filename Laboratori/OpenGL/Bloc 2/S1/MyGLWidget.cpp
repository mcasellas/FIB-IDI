#include "MyGLWidget.h"


#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL) {
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
  scale = 1.0f;
}

MyGLWidget::~MyGLWidget () {
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL() {
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  glEnable (GL_DEPTH_TEST);


  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)

  carregaShaders();
  createBuffers();
}

void MyGLWidget::paintGL() {
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  projectTransform();
  viewTransform();


  glm::mat4 transform (1.0f);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);

  glBindVertexArray (VAOS[1]);
  glDrawArrays(GL_TRIANGLES, 0, 9);

  // Carreguem la transformació de model
  modelTransform();
  glBindVertexArray(VAOS[0]);

  // pintem
  glDrawArrays (GL_TRIANGLES, 0, m.faces().size() * 3);

  glBindVertexArray (0);
}

void MyGLWidget::modelTransform() {
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  transform = glm::rotate(transform, float(M_PI/4*angle) ,glm::vec3(0.0,1.0,0.0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}



void MyGLWidget::resizeGL (int w, int h) {
  glViewport(0, 0, w, h);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      break;
    }
    case Qt::Key_R: { // escalar a més petit
      angle += 1;
      break;
    }

    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::createBuffers() {

  m.load ("./models/HomerProves.obj");


  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(2, VAOS);

  // HOMER
  glBindVertexArray(VAOS[0]);

  GLuint VBO_Homer[2];
  glGenBuffers(2, VBO_Homer);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[0]);
  glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_vertices(), GL_STATIC_DRAW); // posició

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Homer[1]);
  glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * m.faces().size() * 3 * 3, m.VBO_matdiff(), GL_STATIC_DRAW); // color

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // TERRA

  glm::vec3 v_Terra[6] = {
    glm::vec3(-1.0, -1.0, 1.0),
    glm::vec3(-1.0, -1.0, -1.0),
    glm::vec3(1.0,-1.0,-1.0),
    glm::vec3(1.0, -1.0, -1.0),
    glm::vec3(1.0, -1.0, 1.0),
    glm::vec3(-1.0,-1.0,1.0)
  };


  glBindVertexArray(VAOS[1]);

  GLuint VBO_Terra;

  glGenBuffers(1,&VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra);
  glBufferData(GL_ARRAY_BUFFER, sizeof(v_Terra), v_Terra, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindVertexArray (0);
}

void MyGLWidget::projectTransform() {
  glm::mat4 Proj = glm::perspective (FOV, ra, zNear, zFar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {
  glm::mat4 View = glm::lookAt(OBS, VRP, UP);
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::carregaShaders() {
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");

  projLoc = glGetUniformLocation(program->programId(), "proj");

  viewLoc = glGetUniformLocation (program->programId(), "view");

}
