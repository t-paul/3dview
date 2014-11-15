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

    tick = 0;
    scale = 1;
    xRot = yRot = zRot = 0;
    light_angle = 0;
    distance = 50;

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
    transform.setTranslate(glm::vec3(-midx, -midy, -z));
    
    float camz = 1.5 * mesh->bboxMax().z;
    float lookz = (mesh->bboxMax().z - mesh->bboxMin().z) / 2;
    std::cout << "loadMesh: camera: Z = " << camz << ", look at: " << lookz << std::endl;
    transform.setCamera(glm::vec3(-20.0f, -2 * camz, camz), glm::vec3(0.0f, 0.0f, lookz), glm::vec3(0.0f, 0.0f, 1.0f));
    
    //timer->start(20);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    tick += 0.01;

    glValidateProgram(programId);
    glUseProgram(programId);

    glUniform4f(AmbientProductID, 0.0, 0.0, 0.6, 1.0);
    glUniform4f(DiffuseProductID, 0.4, 0.4, 0.4, 1.0);
    glUniform4f(SpecularProductID, 0.8, 0.8, 0.8, 1.0);
    glm::vec4 light_pos = glm::vec4(glm::rotateZ(glm::vec3(50.0, 50.0, 200.0), light_angle), 1.0);
    glUniform4fv(LightPositionID, 1, glm::value_ptr(light_pos));
    glUniform1f(ShininessID, 64.0);

    float fov = M_PI * 75.0 / 180.0;
    glm::mat4 Projection = glm::perspective(fov, 1.0f, 1.0f, 10000.0f);
    glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, glm::value_ptr(Projection));

    glm::mat4 model = glm::mat4();
    model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
    model = glm::rotate(model, zRot, glm::vec3(0.0, 0.0, 1.0));
    model = glm::rotate(model, yRot, glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, xRot, glm::vec3(1.0, 0.0, 0.0));
    
    glm::mat4 view = glm::mat4();
    view = glm::translate(view, glm::vec3(0.0, -20.0, -distance));
    view = glm::rotate(view, (float)(-M_PI / 2.0f), glm::vec3(1.0, 0.0, 0.0));
    
    glm::mat4 modelview = view * model;
    glUniformMatrix4fv(ModelViewID, 1, GL_FALSE, glm::value_ptr(modelview));

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
    float inc = e->delta() > 0 ? 0.1 : -0.1;
    glm::vec3 look = transform.target() - transform.pos();
    glm::vec3 pos = glm::vec3(glm::translate(inc * look) * glm::vec4(transform.pos(), 1));
    transform.setCamera(pos, transform.target(), transform.up());
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
	glm::vec3 pos = glm::rotateZ(transform.pos(), (float)dx / -50.0f);
	
	glm::vec3 look = transform.target() - pos;
	glm::vec3 axis = glm::cross(look, transform.up());
	//std::cout << "axis: " << axis.x << ", " << axis.y << ", " << axis.z << std::endl;
	glm::vec3 pos2 = glm::rotate(pos, (float)dy / -50.0f, axis);
	transform.setCamera(pos2, transform.target(), transform.up());
    } else if (event->buttons() & Qt::RightButton) {
	//transform.setCamera(transform.pos() + ((transform.pos() - transform.target()) * 0.01f * (float)dy), transform.target(), transform.up());
    }

    updateGL();
    lastMousePos = event->pos();
}

void
GLWidget::keyPressEvent(QKeyEvent *event)
{
    float angle = (float)M_PI/90.0f;
    
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