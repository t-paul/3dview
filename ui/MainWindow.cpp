#include <QFileDialog>

#include "MainWindow.h"

MainWindow::MainWindow()
{
    ui.setupUi(this);
    
    color = QColor(200, 20, 20);
    ambientIntensity = 0.3;
    diffuseIntensity = 0.5;
    specularIntensity = 0.8;
    specularPower = 32;
    updateGUI();
}

MainWindow::~MainWindow()
{
}

void
MainWindow::on_actionExit_triggered(void)
{
    QApplication::quit();
}

void
MainWindow::on_actionOpen_triggered(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 "/data/OpenSCAD/3dview/resources/meshes",
                                                 tr("Mesh files (*.stl)"));
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists()) {
        this->ui.gl->loadMesh(fileName.toStdString());
	setWindowFilePath(fileInfo.absoluteFilePath());
    } else {
	setWindowFilePath("");
    }
}

void
MainWindow::on_sliderRed_valueChanged(int value)
{
    color.setRed(value);
    updateGL();
}

void
MainWindow::on_sliderGreen_valueChanged(int value)
{
    color.setGreen(value);
    updateGL();
}

void
MainWindow::on_sliderBlue_valueChanged(int value)
{
    color.setBlue(value);
    updateGL();
}

void
MainWindow::on_sliderAmbientIntensity_valueChanged(int value)
{
    ambientIntensity = (float)value / 100.0f;
    updateGL();
}

void
MainWindow::on_sliderDiffuseIntensity_valueChanged(int value)
{
    diffuseIntensity = (float)value / 100.0f;
    updateGL();
}

void
MainWindow::on_sliderSpecularIntensity_valueChanged(int value)
{
    specularIntensity = (float)value / 100.0f;
    updateGL();
}

void
MainWindow::on_sliderSpecularPower_valueChanged(int value)
{
    specularPower = (float)value;
    updateGL();
}

void
MainWindow::updateGUI()
{
    ui.sliderRed->setValue(color.red());
    ui.sliderGreen->setValue(color.green());
    ui.sliderBlue->setValue(color.blue());
    ui.sliderAmbientIntensity->setValue(ambientIntensity * 100);
    ui.sliderDiffuseIntensity->setValue(diffuseIntensity * 100);
    ui.sliderSpecularIntensity->setValue(specularIntensity * 100);
    ui.sliderSpecularPower->setValue(specularPower);
}

void
MainWindow::updateGL()
{
    ui.labelRedDisplay->setText(QString::number(color.redF(), 'f', 2));
    ui.labelGreenDisplay->setText(QString::number(color.greenF(), 'f', 2));
    ui.labelBlueDisplay->setText(QString::number(color.blueF(), 'f', 2));
    ui.labelAmbientIntensityDisplay->setText(QString::number(ambientIntensity, 'f', 2));
    ui.labelDiffuseIntensityDisplay->setText(QString::number(diffuseIntensity, 'f', 2));
    ui.labelSpecularIntensityDisplay->setText(QString::number(specularIntensity, 'f', 2));
    ui.labelSpecularPowerDisplay->setText(QString::number(specularPower, 'f', 0));
    
    QPalette palette = ui.labelColorDisplay->palette();
    palette.setColor(ui.labelColorDisplay->backgroundRole(), color);
    ui.labelColorDisplay->setPalette(palette);
    ui.gl->setColor(color);
    ui.gl->setIntensity(ambientIntensity, diffuseIntensity, specularIntensity, specularPower);
}
