#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <QFileInfo>
#include <QTextStream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define BUFFER_OFFSET(offset) ((void *)(offset))

#include "GLWidget.h"

GLWidget::GLWidget(QWidget* parent) : QGLWidget(parent)
{
    widget.setupUi(this);

    initView();

    mesh = NULL;
    timer = new QTimer();
    repaintTimer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    connect(repaintTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
    setFocus();
}

GLWidget::~GLWidget()
{
    timer->stop();
    delete timer;
}

void
GLWidget::initView()
{
    scale = 1;
    xRot = yRot = zRot = 0;
    light_angle = 0;
    x = y = 0;
    distance = 50;
    autoRotate = false;
}

void
GLWidget::onTimer()
{
    updateGL();
}

void
GLWidget::setColor(QColor color)
{
    this->color = color;
}

void
GLWidget::setAutoRotate(bool autoRotate)
{
    timer->stop();
    if (autoRotate) {
	timer->start(20);
    }
    this->autoRotate = autoRotate;
}

void
GLWidget::setIntensity(float ambientIntensity, float diffuseIntensity, float specularIntensity, float specularPower)
{
    this->ambientIntensity = ambientIntensity;
    this->diffuseIntensity = diffuseIntensity;
    this->specularIntensity = specularIntensity;
    this->specularPower = specularPower;
}

void
GLWidget::setNormalLength(float normalLength)
{
    this->normalLength = normalLength;
}

void
GLWidget::postUpdate()
{
    repaintTimer->stop();
    repaintTimer->start(20);
}

void
GLWidget::loadMesh(std::string filename)
{
    timer->stop();
    
    if (mesh != NULL) {
	delete mesh;
    }
    
    std::cout << "loadMesh: file: " << filename << std::endl;
    mesh = new Mesh(filename);
    mesh->bind();
    initView();
    updateGL();
}

void
GLWidget::initializeGL()
{
    // Otherwise glGenVertexArrays crashes on Linux
    glewExperimental = GL_TRUE;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
	/* Problem: glewInit failed, something is seriously wrong. */
	fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	QApplication::quit();
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    const unsigned char *str = glGetString(GL_VERSION);
    fprintf(stdout, "OpenGL Version: %s\n", str);
    const unsigned char *glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);
    fprintf(stdout, "GLSL Version: %s\n", glsl);

    qglClearColor(QColor(0xfd, 0xf6, 0xe3));

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    loadShaders(shader1, "shader");
    loadShaders(shader2, "geometry");

    QStringList args = QCoreApplication::arguments();
    if (args.size() > 1) {
	QFileInfo fileInfo(args.at(1));
	if (fileInfo.isFile()) {
	    loadMesh(fileInfo.absoluteFilePath().toStdString());
	}
    } else {
        loadMesh("/data/OpenSCAD/3dview/resources/meshes/UltimakerRobot_support.stl");
    }
}

void
GLWidget::setAttributes(QGLShaderProgram & shader)
{
    shader.bind();
    
    glm::vec4 light_pos = glm::vec4(glm::rotateZ(glm::vec3(50.0, 50.0, 200.0), light_angle), 1.0);
    
    shader.setUniformValue("AmbientProduct", ambientIntensity * color.redF(), ambientIntensity * color.greenF(), ambientIntensity * color.blueF(), 1.0);
    shader.setUniformValue("DiffuseProduct", diffuseIntensity, diffuseIntensity, diffuseIntensity, 1.0);
    shader.setUniformValue("SpecularProduct", specularIntensity, specularIntensity, specularIntensity, 1.0);
    shader.setUniformValue("LightPosition", light_pos.x, light_pos.y, light_pos.z, light_pos.w);
    shader.setUniformValue("Shininess", specularPower);
    shader.setUniformValue("NormalLength", normalLength);

    shader.setUniformValue("Projection", QMatrix4x4(glm::value_ptr(transform.projection())).transposed());

    glm::mat4 modelview = transform.matrix() * mesh->matrix();
    shader.setUniformValue("ModelView", QMatrix4x4(glm::value_ptr(modelview)).transposed());
}

void
GLWidget::paintGL()
{
    if (autoRotate) {
	yRot += 0.01;
    }
    
    transform.setRotate(glm::vec3(xRot, yRot, zRot));
    transform.setTranslate(glm::vec3(x, y, -distance));
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setAttributes(shader1);
    if (mesh) {
        mesh->draw(shader1.attributeLocation("vPosition"), shader1.attributeLocation("vNormal"));
    }
    
    setAttributes(shader2);
    if (mesh) {
        mesh->draw(shader2.attributeLocation("vPosition"), shader1.attributeLocation("vNormal"));
    }
    
    elapsed.restart();
}

void
GLWidget::resizeGL(int width, int height)
{
    transform.setWindow(width, height);
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void
GLWidget::wheelEvent(QWheelEvent *e)
{
    float factor = e->delta() > 0 ? 0.9 : 1.1;
    distance *= factor;
    updateGL();
}

void
GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePos = event->pos();
}

void
GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    repaintTimer->stop();

    int dx = event->x() - lastMousePos.x();
    int dy = event->y() - lastMousePos.y();

    if (event->buttons() & Qt::LeftButton) {
	yRot += (float)dx / -100.0f;
	xRot += (float)dy / -100.0f;
    } else if (event->buttons() & Qt::RightButton) {
	x += (float)dx / 10.0f;
	y -= (float)dy / 10.0f;
    }

    lastMousePos = event->pos();
    
    if (elapsed.elapsed() > 50) {
	updateGL();
    } else {
        repaintTimer->start(50);
    }
}

void
GLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers().testFlag(Qt::ShiftModifier)) {
	if (event->key() == Qt::Key_Up) {
	    y += 1.0;
	} else if (event->key() == Qt::Key_Down) {
	    y -= 1.0;
	} else if (event->key() == Qt::Key_Left) {
	    x -= 1.0;
	} else if (event->key() == Qt::Key_Right) {
	    x += 1.0;
	}
    } else {
	if (event->key() == Qt::Key_Plus) {
	    distance *= 0.9;
	} else if (event->key() == Qt::Key_Minus) {
	    distance *= 1.1;
	} else if (event->key() == Qt::Key_Up) {
	    xRot += 0.1;
	} else if (event->key() == Qt::Key_Down) {
	    xRot -= 0.1;
	} else if (event->key() == Qt::Key_Left) {
	    yRot += 0.1;
	} else if (event->key() == Qt::Key_Right) {
	    yRot -= 0.1;
	} else if (event->key() == Qt::Key_C) {
	    x = 0;
	    y = 0;
	}
    }
    updateGL();
}

void
GLWidget::loadShader(QGLShaderProgram & shader, const QGLShader::ShaderType type, const QString name)
{
    QFile resource(name);
    if (resource.open(QFile::ReadOnly | QFile::Text)) {
	QTextStream stream(&resource);
	QString shaderSource = stream.readAll().trimmed();
	if (!shaderSource.isEmpty()) {
	    if (shader.addShaderFromSourceCode(type, shaderSource)) {
		std::cout << "Added shader source for type " << type << std::endl;
	    } else {
		std::cout << "Failed to add shader source for type " << type << std::endl;
	    }
	}
    }
}

void
GLWidget::loadShaders(QGLShaderProgram & shader, const QString name)
{
    QString path = QString(":/resources/shader/") + name;
    loadShader(shader, QGLShader::Vertex, path + ".vs");
    loadShader(shader, QGLShader::Fragment, path + ".fs");
    loadShader(shader, QGLShader::Geometry, path + ".gs");
    shader.link();
}