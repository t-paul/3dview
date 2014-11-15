#include <QApplication>

#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("OpenSCAD");
    QCoreApplication::setOrganizationDomain("openscad.org");
    QCoreApplication::setApplicationName("3dview");
    QCoreApplication::setApplicationVersion("3dview 0.1");

    QGLFormat fmt;
    fmt.setProfile(QGLFormat::CoreProfile);
    fmt.setVersion(3,2);
    QGLFormat::setDefaultFormat(fmt);

    // create and show your widgets here
    MainWindow main;
    main.show();

    return app.exec();
}
