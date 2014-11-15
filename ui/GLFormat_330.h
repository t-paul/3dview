#pragma once

#include <qgl.h>

class GLFormat : public QGLFormat {
public:
    GLFormat()
    {
        setVersion(3, 2);
        setProfile(QGLFormat::CoreProfile);
        setSampleBuffers(true);
    }
    
    virtual ~GLFormat()
    {
    }
};

struct GLContext : public QGLContext {
    GLContext(const QGLFormat& format) : QGLContext(format)
    {
    }
    
    virtual ~GLContext()
    {
    }
};