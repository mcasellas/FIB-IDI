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

    // Genèriques:
    void createBuffers ();
    void carregaShaders ();
    void projectTransform ();
    void viewTransform();
    void iniCamera();

    // Model Transform:
    void modelTransformPatricio1();
    void modelTransformPatricio2();
    void modelTransformPatricio3();
    void modelTransformTerra();

    // Altres Funcions:
    void calculPatricio();
    void centreRadi(glm::vec3& Pminim, glm::vec3& Pmax, glm::vec3& Centre, float& radi);
    void zoom(bool ampliar);


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

    // Paràmetres projectTransform
    float FOV;
    float ang_ini;
    float raw;
    float rav;
    float zNear;
    float zFar;
    float d;

    // Paràmetres viewTransform
    glm::vec3 OBS;
    glm::vec3 VRP;
    glm::vec3 UP;

    // Paràmetres Patricio
    glm::vec3 PatricioMin;
    glm::vec3 PatricioMax;
    glm::vec3 CentrePAT;
    float radiPAT;

    // Paràmetres Escena
    glm::vec3 EscenaMin;
    glm::vec3 EscenaMax;
    glm::vec3 CentreESC;
    float radiESC;


    // Angles d'Euler
    float PosXant;
    float PosYant;
    float rotacioX;
    float rotacioY;

    Model m;
};
