#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  public slots:
    void setValor(int val);

  signals:
    void actualitzarAngle(int ang);

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
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event
    // corresponent de ratolí
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

  private:
    void createBuffersPatricio ();
    void createBuffersVaca ();
    void createBuffersTerraIParet ();
    void carregaShaders ();
    void iniEscena ();
    void iniCamera ();
    void projectTransform ();
    void viewTransform ();
    void modelTransformTerra ();
    void modelTransformPatricio ();
    void modelTransformVaca ();
    void calculaCapsaModelPatricio ();
    void calculaCapsaModelVaca ();


    void posicioFocus();
    void colorFocus();
    void llumAmbient();

    // VAO i VBO names
    GLuint VAO_Patr;
    GLuint VAO_Vaca;
    GLuint VAO_Terra;
    // Program
    QOpenGLShaderProgram *program;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // attribute locations
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;
    GLuint posFocusLoc, colFocusLoc, llumAmbientLoc;
    // model
    Model patr;
    Model vaca;
    // paràmetres calculats a partir de la capsa contenidora del model
    glm::vec3 centrePatr;
    glm::vec3 centreVaca;

    float escalaPat;
    float escalaVaca;
    // radi de l'escena
    float radiEsc;

    typedef  enum {NONE, ROTATE, ZOOM} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float angleY;
    float angleX;
    float angleROT;
    bool perspectiva;

    glm::vec3 EscenaMin;
    glm::vec3 EscenaMax;

    glm::vec3 CentreESC;
    glm::vec3 VRP;
    glm::vec3 UP;


    float FOV;
    float ang_ini;
    float raw;
    float rav;
    float zNear;
    float zFar;
    float d;

    // Iluminacio
    glm::vec3 posF;



};
