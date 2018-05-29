#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Model/model.h"



class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *e);




  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransform ();
    void modelTransform2();
    void projectTransform ();
    void viewTransform();
    void calculObjecte();
    void centreRadi(glm::vec3& Pminim, glm::vec3& Pmax, glm::vec3& Centre);
    void iniCamera();


    // attribute locations
    GLuint vertexLoc, colorLoc, projLoc, viewLoc;
    // uniform locations
    GLuint transLoc;
    // VAO i VBO names

    GLuint VAOS[2];
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    float scale;
    float angle = 0;
    glm::vec3 pos;

    // Paràmetres projectTransform
    float FOV;
    float ra;
    float zNear;
    float zFar;
    float d;

    // Paràmetres viewTransform
    glm::vec3 OBS;
    glm::vec3 VRP;
    glm::vec3 UP;

    glm::vec3 PatricioMin;
    glm::vec3 PatricioMax;
    glm::vec3 CentreP;

    glm::vec3 EscenaMin;
    glm::vec3 EscenaMax;
    glm::vec3 CentreE;


    float radiOBJ;
    float radiESC;

    float PosXant;
    float PosYant;

    float rotacioX;
    float rotacioY;

    Model m;
};
