#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <glm/glm.hpp>

#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLWidget.h"

GLWidget::GLWidget(QWidget* parent) : QGLWidget(parent)
{
    widget.setupUi(this);

    tick = 0;
    scale = 1;
    xRot = yRot = zRot = 0;

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

    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    //glEnable(GL_CULL_FACE);

    qglClearColor(QColor(0xfd, 0xf6, 0xe3));
    programId = loadShaders("resources/shader/tutorial.vs", "resources/shader/tutorial.fs");

//    programId = loadShaders("resources/shader/basic.vs", "resources/shader/basic.fs");
    //programId = loadShaders("resources/shader/simple.vs", "resources/shader/simple.fs");
    //programId = loadShaders("resources/shader/vertex-lighting.vs", "resources/shader/vertex-lighting.fs");

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
    glLoadIdentity();

    glValidateProgram(programId);
    glUseProgram(programId);

    tick += 0.01;
    
    //transform.setTranslate(glm::vec3(0, 0, 0));
    //transform.setUniformScale(sinf(tick));
    //transform.setRotate(glm::vec3(-M_PI/2, 0, M_PI*cosf(tick)));
    GLint gWorld = getLocationId(programId, "gWorld");
    glm::mat4 matrix = transform.matrix();
    glUniformMatrix4fv(gWorld, 1, GL_FALSE, &matrix[0][0]);

    GLint dirLightColorLocationId = getLocationId(programId, "gDirectionalLight.Color");
    GLint dirLightAmbientIntensityLocationId = getLocationId(programId, "gDirectionalLight.AmbientIntensity");
    GLint dirLightDiffuseIntensityLocationId = getLocationId(programId, "gDirectionalLight.DiffuseIntensity");
    GLint dirLightDirectionLocationId = getLocationId(programId, "gDirectionalLight.Direction");
    glUniform3f(dirLightColorLocationId, color.redF(), color.greenF(), color.blueF());
    glUniform1f(dirLightAmbientIntensityLocationId, ambientIntensity);
    glUniform1f(dirLightDiffuseIntensityLocationId, diffuseIntensity);
    
    glm::vec3 direction(0.0f, 0.5f, 0.8f);
    glUniform3f(dirLightDirectionLocationId, direction.x, direction.y, direction.z);

    GLint eyeWorldPosLocationId = getLocationId(programId, "gEyeWorldPos");
    GLint matSpecularIntensityLocationId = getLocationId(programId, "gMatSpecularIntensity");
    GLint specularPowerLocationId = getLocationId(programId, "gSpecularPower");
    glUniform3f(eyeWorldPosLocationId, transform.pos().x, transform.pos().y, transform.pos().z);
    glUniform1f(matSpecularIntensityLocationId, specularIntensity);
    glUniform1f(specularPowerLocationId, specularPower);

    if (mesh) {
        mesh->draw();
    }

    glColor3d(0.5, 0.5, 0.5);
    glLoadIdentity();
    glBegin(GL_LINES);
    double l = 1000.0;
    glVertex3d(-l, 0, 0);
    glVertex3d(+l, 0, 0);
    glVertex3d(0, -l, 0);
    glVertex3d(0, +l, 0);
    glVertex3d(0, 0, -l);
    glVertex3d(0, 0, +l);
    glEnd();
}

void
GLWidget::resizeGL(int width, int height)
{
    transform.setWindow(width, height);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
    glOrtho(-1, +1, -1, +1, 1.0, 25.0);
#endif
    glMatrixMode(GL_MODELVIEW);
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
	    transform.setCamera(transform.pos() + glm::vec3(0, 0, 1), transform.target(), transform.up());
	} else if (event->key() == Qt::Key_Down) {
	    transform.setCamera(transform.pos() + glm::vec3(0, 0, -1), transform.target(), transform.up());
	} else if (event->key() == Qt::Key_Left) {
	    transform.setCamera(transform.pos() + glm::vec3(-1, 0, 0), transform.target(), transform.up());
	} else if (event->key() == Qt::Key_Right) {
	    transform.setCamera(transform.pos() + glm::vec3(1, 0, 0), transform.target(), transform.up());
	}
    } else {
	if (event->key() == Qt::Key_Plus) {
	    glm::vec3 look = transform.target() - transform.pos();
	    glm::vec3 pos = glm::vec3(glm::translate(0.1f * look) * glm::vec4(transform.pos(), 1));
	    transform.setCamera(pos, transform.target(), transform.up());
	} else if (event->key() == Qt::Key_Minus) {
	    glm::vec3 look = transform.target() - transform.pos();
	    glm::vec3 pos = glm::vec3(glm::translate(-0.1f * look) * glm::vec4(transform.pos(), 1));
	    transform.setCamera(pos, transform.target(), transform.up());
	} else if (event->key() == Qt::Key_Up) {
	    glm::vec3 look = transform.target() - transform.pos();
	    glm::vec3 axis = glm::cross(look, transform.up());
	    glm::vec3 pos = glm::rotate(transform.pos(), -angle, axis);
	    transform.setCamera(pos, transform.target(), transform.up());
	} else if (event->key() == Qt::Key_Down) {
	    glm::vec3 look = transform.target() - transform.pos();
	    glm::vec3 axis = glm::cross(look, transform.up());
	    glm::vec3 pos = glm::rotate(transform.pos(), angle, axis);
	    transform.setCamera(pos, transform.target(), transform.up());
	} else if (event->key() == Qt::Key_Left) {
	    glm::vec3 pos = glm::rotateZ(transform.pos(), angle);
	    transform.setCamera(pos, transform.target(), transform.up());
	} else if (event->key() == Qt::Key_Right) {
	    glm::vec3 pos = glm::rotateZ(transform.pos(), -angle);
	    transform.setCamera(pos, transform.target(), transform.up());
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

    glDetachShader(programId, VertexShaderID);
    glDetachShader(programId, FragmentShaderID);
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}