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
  iniCamera();
}

void MyGLWidget::paintGL() {
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  modelTransformTerra();
  glBindVertexArray (VAOS[1]);
  glDrawArrays(GL_TRIANGLES, 0, 9);

  // Pintem els patricios
  modelTransformPatricio1();
  glBindVertexArray(VAOS[0]);
  glDrawArrays (GL_TRIANGLES, 0, m.faces().size() * 3);

  modelTransformPatricio2();
  glBindVertexArray(VAOS[0]);
  glDrawArrays (GL_TRIANGLES, 0, m.faces().size() * 3);

  modelTransformPatricio3();
  glBindVertexArray(VAOS[0]);
  glDrawArrays (GL_TRIANGLES, 0, m.faces().size() * 3);

  glBindVertexArray (0);
}



void MyGLWidget::resizeGL (int w, int h) {
  glViewport(0, 0, w, h);

  rav = double(width())/double(height()); // Obtenim la relació d'aspecte del viewport
  raw = rav; // igualem la relació d'aspecte
  FOV = ang_ini;
  if (w < h) FOV = 2.0*atan(tan(ang_ini)/rav);
  projectTransform();

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
    case Qt::Key_X: { // zoom out
      zoom(false);
      break;
    }
    case Qt::Key_Z: { // zoom in
      zoom(true);
      break;
    }

    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::zoom(bool ampliar) {

  if (ampliar) {
    if ((FOV - float(M_PI)/20) > 0)
      FOV -= float(M_PI)/20;
  }

  else {
    if ((FOV + float(M_PI)/20) < float(M_PI))
      FOV += float(M_PI)/20;
  }

  ang_ini = FOV;
  projectTransform();
}


void MyGLWidget::createBuffers() {

  m.load ("./models/Patricio.obj");

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(2, VAOS);

  // HOMER
  glBindVertexArray(VAOS[0]);

  calculPatricio();

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
  // Vèrtexs del terra
  glm::vec3 v_Terra[9] = {
    glm::vec3(-2.5, 0, 2.5),
    glm::vec3(-2.5, 0, -2.5),
    glm::vec3(2.5, 0,-2.5),
    glm::vec3(2.5, 0, -2.5),
    glm::vec3(2.5, 0, 2.5),
    glm::vec3(-2.5,0,2.5),

    glm::vec3(-2.5, 0, 2.5),
    glm::vec3(-2.5, 0, -2.5),
    glm::vec3(0.0, d,-0.0)
  };
  // Colors del terra
  glm::vec3 c_Terra[6] = {
    glm::vec3(1,0,0),
    glm::vec3(0,1,0),
    glm::vec3(0,0,1),
    glm::vec3(0,0,1),
    glm::vec3(1,1,0),
    glm::vec3(1,0,0),

  };

  glBindVertexArray(VAOS[1]);

  GLuint VBO_Terra[2];

  // Enviem vèrtexs
  glGenBuffers(1,&VBO_Terra[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(v_Terra), v_Terra, GL_STATIC_DRAW);

  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Enviem colors
  glGenBuffers(1,&VBO_Terra[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * sizeof(c_Terra), c_Terra, GL_STATIC_DRAW); // color

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
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

void MyGLWidget::modelTransformPatricio1() {
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);

  transform = glm::translate(transform,glm::vec3(2.0,0.0,2.0));
  transform = glm::scale(transform, glm::vec3(scale)); // Escalat S D
  transform = glm::rotate(transform, float(M_PI/4*angle) ,glm::vec3(0.0,1.0,0.0)); // Rotació R
  transform = glm::scale(transform, glm::vec3(1/((PatricioMax.y+PatricioMin.y)/2))); // Calculem la mida perquè dongui 1 d'alçada
  glm::vec3 baseOBJ = glm::vec3((PatricioMin.x + PatricioMax.x)/2, PatricioMin.y, (PatricioMin.z + PatricioMax.z)/2);
  transform = glm::translate(transform,-baseOBJ);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);

}

void MyGLWidget::modelTransformPatricio2() {
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);

  transform = glm::scale(transform, glm::vec3(scale)); // Escalat S D
  transform = glm::rotate(transform, float(M_PI/4*angle) ,glm::vec3(0.0,1.0,0.0)); // Rotació R
  transform = glm::rotate(transform, float(M_PI/2) ,glm::vec3(0.0,1.0,0.0)); // Rotació Inicial
  transform = glm::scale(transform, glm::vec3(1/((PatricioMax.y+PatricioMin.y)/2))); // Calculem la mida perquè dongui 1 d'alçada
  glm::vec3 baseOBJ = glm::vec3((PatricioMin.x + PatricioMax.x)/2, PatricioMin.y, (PatricioMin.z + PatricioMax.z)/2);
  transform = glm::translate(transform,-baseOBJ); // portem a l'origen

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);

}

void MyGLWidget::modelTransformPatricio3() {
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);

  transform = glm::translate(transform,glm::vec3(-2.0,0.0,-2.0));
  transform = glm::scale(transform, glm::vec3(scale)); // Escalat S D
  transform = glm::rotate(transform, float(M_PI/4*angle) ,glm::vec3(0.0,1.0,0.0)); // Rotació R
  transform = glm::rotate(transform, float(M_PI) ,glm::vec3(0.0,1.0,0.0)); // Rotació Inicial
  transform = glm::scale(transform, glm::vec3(1/((PatricioMax.y+PatricioMin.y)/2))); // Calculem la mida perquè dongui 1 d'alçada
  glm::vec3 baseOBJ = glm::vec3((PatricioMin.x + PatricioMax.x)/2, PatricioMin.y, (PatricioMin.z + PatricioMax.z)/2);
  transform = glm::translate(transform,-baseOBJ);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);

}

void MyGLWidget::modelTransformTerra() {
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform, float(0) ,glm::vec3(0.0,1.0,0.0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}


void MyGLWidget::calculPatricio() {
  PatricioMin.x = PatricioMax.x =  m.vertices()[0];
  PatricioMin.y = PatricioMax.y =  m.vertices()[1];
  PatricioMin.z = PatricioMax.z =  m.vertices()[2];

  for (unsigned long i = 3; i < m.vertices().size(); i+=3){

    if (m.vertices()[i] < PatricioMin.x) PatricioMin.x = m.vertices()[i];
    if (m.vertices()[i] > PatricioMax.x) PatricioMax.x = m.vertices()[i];

    if (m.vertices()[i+1] < PatricioMin.y) PatricioMin.y = m.vertices()[i+1];
    if (m.vertices()[i+1] > PatricioMax.y) PatricioMax.y = m.vertices()[i+1];

    if (m.vertices()[i+2] < PatricioMin.z) PatricioMin.z = m.vertices()[i+2];
    if (m.vertices()[i+2] > PatricioMax.z) PatricioMax.z = m.vertices()[i+2];
  }

  centreRadi(PatricioMin, PatricioMax, CentrePAT, radiPAT);
}

void MyGLWidget::centreRadi(glm::vec3& Pmin, glm::vec3& Pmax, glm::vec3& Centre, float& Radi){

  Centre = (Pmin+Pmax)/2.0f;
  Radi = glm::distance(Pmax,Pmin)/2.0f;
}

void MyGLWidget::projectTransform() {
  glm::mat4 Proj = glm::mat4(1.0);
  Proj = glm::perspective (FOV, raw, zNear, zFar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {
  //glm::mat4 View = glm::lookAt(OBS, VRP, UP);
  glm::mat4 View = glm::mat4(1.0);

  View = glm::translate(View,glm::vec3(0.0,0.0,-d));
  View = glm::rotate(View, rotacioX, glm::vec3(0.0,1.0,0.0));
  View = glm::rotate(View, rotacioY, glm::vec3(1.0,0.0,0.0));
  View = glm::translate(View, -VRP);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::iniCamera(){

    // Posem la caixa contenidorawde l'escena a mà:
    EscenaMin = glm::vec3(-2.5f,0.0f,-2.5f);
    EscenaMax = glm::vec3(2.5f,1.0f,2.5f);

    // Calculem centre i radi de l'escena
    CentreESC = (EscenaMin+EscenaMax)/2.0f;
    radiESC = glm::distance(EscenaMax,EscenaMin)/2.0f;

    d = radiESC*2;

    ang_ini = asin(radiESC/d);
    FOV = 2*ang_ini;

    raw = 1.0;
    zNear = d-radiESC;
    zFar = d+radiESC;


    VRP = CentreESC;
    //OBS = VRP + d*glm::vec3(0.0f, 1.0f, 1.0f); // Direcció picada

    UP = glm::vec3(0.0f,1.0f,0.0f);

    PosXant = 0.0;
    PosYant = 0.0;
    rotacioX = 0.0;
    rotacioY = 0.0;

    projectTransform();
    viewTransform();
}

// Interacció amb angles d'Euler
void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {

  makeCurrent();

  rotacioX -= ((e->x()-PosXant)/width())*15;
  rotacioY -= ((e->y()-PosYant)/height())*15;

  viewTransform();
  update();

  PosXant = e->x();
  PosYant = e->y();


}
