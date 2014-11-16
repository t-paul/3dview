#ifndef _GLWIDGET_H
#define	_GLWIDGET_H

#include <QTimer>
#include <QGLWidget>
#include <QMouseEvent>

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

    GLuint programId;
    GLuint vao;
    GLuint AmbientProductID, DiffuseProductID, SpecularProductID;
    GLuint ModelViewID;
    GLuint ProjectionID;
    GLuint LightPositionID;
    GLuint ShininessID;

    Transform transform;
    Mesh *mesh;
    
    QColor color;
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
    float specularPower;

private:
    GLint getLocationId(GLuint program, const GLchar *name);
    GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);

private slots:
    void onTimer();

public:
    void setColor(QColor color);
    void setAutoRotate(bool autoRotate);
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
