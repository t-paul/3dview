#ifndef _GLWIDGET_H
#define	_GLWIDGET_H

#include <QTimer>
#include <QGLWidget>
#include <QMouseEvent>
#include <QElapsedTimer>
#include <QGLShaderProgram>

#include "Mesh.h"
#include "Transform.h"
#include "ui_GLWidget.h"

class GLWidget : public QGLWidget {
    Q_OBJECT
public:
    GLWidget(QWidget* parent = 0);
    virtual ~GLWidget();
private:
    Ui::GLWidget widget;
    float xRot;
    float yRot;
    float zRot;
    float scale;
    float light_angle;
    float x;
    float y;
    float distance;
    bool autoRotate;
    QPoint lastMousePos;
    QTimer *timer;
    QTimer *repaintTimer;
    QElapsedTimer elapsed;

    GLuint vao;

    Transform transform;
    Mesh *mesh;
    
    QColor colorA;
    QColor colorD;
    QColor colorS;
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
    float specularPower;
    float normalLength;
    
    QString shaderName1;
    QString shaderName2;
    QGLShaderProgram shader1;
    QGLShaderProgram shader2;

private:
    void initView();
    void setAttributes(QGLShaderProgram &);
    void loadShader(QGLShaderProgram &shader, const QGLShader::ShaderType type, const QString name);
    void loadShaders(QGLShaderProgram &shader, const QString name);

private slots:
    void onTimer();

public:
    void postUpdate();
    void setShader(int, QString);
    void setColors(QColor colorA, QColor colorD, QColor colorS);
    void setAutoRotate(bool autoRotate);
    void setNormalLength(float normalLength);
    void setIntensity(float ambientIntensity, float diffuseIntensity, float specularIntensity, float specularPower);
    void loadMesh(std::string filename);
    
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif	/* _GLWIDGET_H */
