#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  createBuffersModel();
  createBuffersTerra();

  iniCamera();

}

void MyGLWidget::paintGL ()
{
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar el terra
  glBindVertexArray (VAO_Terra);

  modelTransformTerra ();

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Patr);
  modelTransformPatricio(1, glm::vec3(0.0,0.0,0.0));
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);

  modelTransformPatricio(0, glm::vec3(2.0,0.0,2.0));
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);

  modelTransformPatricio(2, glm::vec3(-2.0,0.0,-2.0));
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);

  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h) {
  glViewport(0, 0, w, h);

  rav = double(width())/double(height()); // Obtenim la relació d'aspecte del viewport
  raw = rav; // Igualem la relació d'aspecte

  if (width() < height()) FOV = 2.0*atan(tan(ang_ini/2)/rav);
  else FOV = ang_ini;

  projectTransform();
}

// Direccio: 0->endavant, 1->dreta, 2->endarrera, 3->esquerra
void MyGLWidget::modelTransformPatricio (int direccio, glm::vec3 posicio) {
  glm::mat4 TG(1.f);  // Matriu de transformació

  TG = glm::translate(TG, posicio);
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::rotate(TG, float(direccio*M_PI/2), glm::vec3(0.0,1.0,0.0));
  TG = glm::translate(TG, -centreBasePatr);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció

  //Proj = glm::perspective(float(M_PI/3.0), 1.0f, 4.0f, 12.0f);
  Proj = glm::perspective(FOV, raw, zNear, zFar);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () {

  glm::mat4 View;  // Matriu de posició i orientació
  //iew = glm::lookAt(glm::vec3(0, 0.5, 8),  glm::vec3(0, 0.5, 0), glm::vec3(0, 1, 0));
  View = glm::lookAt(OBS, VRP, UP);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::iniCamera() {
  PrimeraPersona = false;

  ang_ini = FOV = float(M_PI/3.0);
  raw = 1.0f;
  zNear = 4.0f;
  zFar = 12.0f;

  OBS = glm::vec3(0, 0.5, 8);
  VRP = glm::vec3(0, 0.5, 0);
  UP = glm::vec3(0, 1, 0);

  OBS_guardat = glm::vec3(2.0, 1.0, 2.0);
  VRP_guardat = glm::vec3(0, 1.0, 0);

  projectTransform ();
  viewTransform ();
}

// Canviar a 3a persona
void MyGLWidget::Camera3aPersona () {

  glm::vec3 aux = VRP;
  VRP = VRP_guardat;
  VRP_guardat = aux;

  aux = OBS;
  OBS = OBS_guardat;
  OBS_guardat = aux;

  PrimeraPersona = false;

  ang_ini = FOV = float(M_PI/3.0);

  zNear = 4.0f;
  zFar = 12.0f;

  projectTransform ();
  viewTransform ();
}

// Canviar a 1a persona
void MyGLWidget::Camera1aPersona () {

  glm::vec3 aux = VRP;
  VRP = VRP_guardat;
  VRP_guardat = aux;

  aux = OBS;
  OBS = OBS_guardat;
  OBS_guardat = aux;

  PrimeraPersona = true;

  ang_ini = FOV = float(M_PI/2.0);

  zNear = 0.1f;
  zFar = 12.0f;

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::moure (bool endavant) {

  int direccio = 1;
  if (not endavant) direccio = -1;

  glm::vec3 vector (VRP.x-OBS.x,0.0,VRP.z-OBS.z); // Obtenim el vector director:
  vector = vector/sqrt(vector.x*vector.x+vector.z*vector.z); // Normalitzem

  OBS += direccio*0.1f*vector; // Movem OBS
  VRP += direccio*0.1f*vector; // Movem VRP

  viewTransform ();
}

void MyGLWidget::rotar (bool dreta) {
  int sentit = -1;
  if (not dreta) sentit = 1;

  glm::vec3 vector (VRP.x-OBS.x,0.0,VRP.z-OBS.z); // Obtenim el vector director:
  /*
  vector = glm::vec3(-vector.z,0.0, vector.x); // Obtenim el vector normal
  vector = vector/sqrt(vector.x*vector.x+vector.z*vector.z); // Normalitzem
  */

  vector = glm::rotate(0.1f, glm::vec3(0.0,1.0,0.0));
  std::cout << vector.x << ' ' << vector.z << std::endl;

  float dist = 2*(sin(0.1/2) * glm::distance(OBS,VRP));

  VRP += sentit*dist*vector; // Movem VRP

  viewTransform ();
}


void MyGLWidget::createBuffersModel ()
{
  // Carreguem el model de l'OBJ - Atenció! Abans de crear els buffers!
  patr.load("./models/Patricio.obj");

  // Calculem la capsa contenidora del model
  calculaCapsaModel ();

  // Creació del Vertex Array Object del Patricio
  glGenVertexArrays(1, &VAO_Patr);
  glBindVertexArray(VAO_Patr);

  // Creació dels buffers del model patr
  GLuint VBO_Patr[2];
  // Buffer de posicions
  glGenBuffers(2, VBO_Patr);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_vertices(), GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Buffer de component difusa usada com a color
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Patr[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray(0);
}

void MyGLWidget::createBuffersTerra ()
{
  // Dades del terra
  // VBO amb la posició dels vèrtexs

  glm::vec3 posterra[6] = {
  	glm::vec3(-2.5, 0.0, 2.5),
  	glm::vec3(2.5, 0.0, 2.5),
  	glm::vec3(-2.5, 0.0, -2.5),
  	glm::vec3(-2.5, 0.0, -2.5),
  	glm::vec3(2.5, 0.0, 2.5),
  	glm::vec3(2.5, 0.0, -2.5),
  };

  glm::vec3 color(0.8,0,0.8);

  // Fem que aquest material afecti a tots els vèrtexs per igual
  glm::vec3 colterra[6] = {
	   color, color, color, color, color, color
  };

// Creació del Vertex Array Object del terra
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colterra), colterra, GL_STATIC_DRAW);

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
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

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::calculaCapsaModel ()
{
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = patr.vertices()[0];
  miny = maxy = patr.vertices()[1];
  minz = maxz = patr.vertices()[2];
  for (unsigned int i = 3; i < patr.vertices().size(); i+=3)
  {
    if (patr.vertices()[i+0] < minx)
      minx = patr.vertices()[i+0];
    if (patr.vertices()[i+0] > maxx)
      maxx = patr.vertices()[i+0];
    if (patr.vertices()[i+1] < miny)
      miny = patr.vertices()[i+1];
    if (patr.vertices()[i+1] > maxy)
      maxy = patr.vertices()[i+1];
    if (patr.vertices()[i+2] < minz)
      minz = patr.vertices()[i+2];
    if (patr.vertices()[i+2] > maxz)
      maxz = patr.vertices()[i+2];
  }
  escala = 1.0/(maxy-miny);
  centreBasePatr[0] = (minx+maxx)/2.0; centreBasePatr[1] = miny; centreBasePatr[2] = (minz+maxz)/2.0;
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_C: {
      // aquesta tecla ha de canviar de càmera entre la que hi ha per defecte i la que es demana (1ª persona)
      if (PrimeraPersona) Camera3aPersona();
      else Camera1aPersona();

      break;
    }
    case Qt::Key_Left: {
      // aquesta tecla ha de fer que la càmera giri a la seva esquerra
      if (PrimeraPersona) rotar(true);
      break;
    }
    case Qt::Key_Right: {
      // aquesta tecla ha de fer que la càmera giri a la seva dreta
      if (PrimeraPersona) rotar(false);
      break;
    }
    case Qt::Key_Up: {
      // aquesta tecla ha de fer que la càmera es mogui endavant
      if (PrimeraPersona) moure(true);
      break;
    }
    case Qt::Key_Down: {
      // aquesta tecla ha de fer que la càmera es mogui endarrera
      if (PrimeraPersona) moure(false);
      break;
    }
    default: event->ignore(); break;
  }
  update();
}
