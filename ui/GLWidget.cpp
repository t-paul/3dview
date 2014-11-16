#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <GL/glew.h>

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

    scale = 1;
    xRot = yRot = zRot = 0;
    light_angle = 0;
    x = y = 0;
    distance = 50;
    autoRotate = false;

    mesh = NULL;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    setFocus();
}

GLWidget::~GLWidget()
{
    timer->stop();
    delete timer;
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
    updateGL();
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
    updateGL();
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

    float z = mesh->bboxMin().z;
    float midx = mesh->bboxMin().x + (mesh->bboxMax().x - mesh->bboxMin().x) / 2;
    float midy = mesh->bboxMin().y + (mesh->bboxMax().y - mesh->bboxMin().y) / 2;
    
    std::cout << "loadMesh: translate: " << midx << ", " << midy << ", " << z << std::endl;
    x = -midx;
    y = -midy;
    distance = 2 * z;
    
    float camz = 1.5 * mesh->bboxMax().z;
    float lookz = (mesh->bboxMax().z - mesh->bboxMin().z) / 2;
    std::cout << "loadMesh: camera: Z = " << camz << ", look at: " << lookz << std::endl;
    transform.setCamera(glm::vec3(-20.0f, -2 * camz, camz), glm::vec3(0.0f, 0.0f, lookz), glm::vec3(0.0f, 0.0f, 1.0f));
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
    
    programId = loadShaders("resources/shader/shader.vs", "resources/shader/shader.fs");

    AmbientProductID = glGetUniformLocation(programId, "AmbientProduct");
    DiffuseProductID = glGetUniformLocation(programId, "DiffuseProduct");
    SpecularProductID = glGetUniformLocation(programId, "SpecularProduct");
    ModelViewID = glGetUniformLocation(programId, "ModelView");
    ProjectionID = glGetUniformLocation(programId, "Projection");
    LightPositionID = glGetUniformLocation(programId, "LightPosition");
    ShininessID = glGetUniformLocation(programId, "Shininess");

    loadMesh("/data/OpenSCAD/3dview/resources/meshes/UltimakerRobot_support.stl");
}

GLint
GLWidget::getLocationId(GLuint program, const GLchar *name)
{
    GLint id = glGetUniformLocation(program, name);
    if (id < 0) {
	std::cout << "location: " << name << " - " << id << std::endl;
	exit(1);
    }
    return id;
}

void
GLWidget::paintGL()
{
    if (autoRotate) {
	zRot += 0.01;
    }
    
    transform.setRotate(glm::vec3(xRot, yRot, zRot));
    transform.setTranslate(glm::vec3(x, y, -distance));
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glValidateProgram(programId);
    glUseProgram(programId);

    glUniform4f(AmbientProductID, ambientIntensity * color.redF(), ambientIntensity * color.greenF(), ambientIntensity * color.blueF(), 1.0);
    glUniform4f(DiffuseProductID, diffuseIntensity, diffuseIntensity, diffuseIntensity, 1.0);
    glUniform4f(SpecularProductID, specularIntensity, specularIntensity, specularIntensity, 1.0);
    glm::vec4 light_pos = glm::vec4(glm::rotateZ(glm::vec3(50.0, 50.0, 200.0), light_angle), 1.0);
    glUniform4fv(LightPositionID, 1, glm::value_ptr(light_pos));
    glUniform1f(ShininessID, specularPower);

    glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, glm::value_ptr(transform.projection()));
    glUniformMatrix4fv(ModelViewID, 1, GL_FALSE, glm::value_ptr(transform.matrix()));

    GLuint vPosition = glGetAttribLocation(programId, "vPosition");
    GLuint vNormal = glGetAttribLocation(programId, "vNormal");
    if (mesh) {
        mesh->draw(vPosition, vNormal);
    }
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
    int dx = event->x() - lastMousePos.x();
    int dy = event->y() - lastMousePos.y();

    if (event->buttons() & Qt::LeftButton) {
	zRot += (float)dx / -100.0f;
	xRot += (float)dy / -100.0f;
    } else if (event->buttons() & Qt::RightButton) {
	x += (float)dx / 10.0f;
	y -= (float)dy / 10.0f;
    }

    updateGL();
    lastMousePos = event->pos();
}

void
GLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers().testFlag(Qt::ShiftModifier)) {
	if (event->key() == Qt::Key_Up) {
	    yRot += 0.1;
	} else if (event->key() == Qt::Key_Down) {
	    yRot -= 0.1;
	} else if (event->key() == Qt::Key_Left) {
	} else if (event->key() == Qt::Key_Right) {
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
	    zRot += 0.1;
	} else if (event->key() == Qt::Key_Right) {
	    zRot -= 0.1;
	}
    }
    updateGL();
}

GLuint
GLWidget::loadShaders(const char *vertex_file_path, const char *fragment_file_path)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage(std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}