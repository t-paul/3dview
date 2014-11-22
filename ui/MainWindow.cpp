#include <iostream>

#include <QDir>
#include <QFileDialog>
#include <QStringList>

#include "MainWindow.h"
#include "Material.h"

MainWindow::MainWindow()
{
    ui.setupUi(this);
    
    normalLength = 0.4;
    shader1 = "phong";
    material = "Gold";
    texture = "Metal";
    
    QDir shader(":/resources/shader");
    QStringList filter("*.vs");
    QFileInfoList entries = shader.entryInfoList(filter, QDir::Files, QDir::Name);
    ui.comboBoxShader1->addItem("<none>");
    ui.comboBoxShader2->addItem("<none>");
    foreach (QFileInfo entry, entries) {
	ui.comboBoxShader1->addItem(entry.baseName());
	ui.comboBoxShader2->addItem(entry.baseName());
    }
    ui.gl->setShader(0, shader1);
    
    on_comboBoxMaterial_activated(material);
    updateGUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::applyColor(glm::vec3 color, QColor &qColor)
{
    qColor.setRedF(color.r);
    qColor.setGreenF(color.g);
    qColor.setBlueF(color.b);
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
MainWindow::on_sliderRedA_valueChanged(int value)
{
    colorA.setRed(value);
    updateGL();
}

void
MainWindow::on_sliderGreenA_valueChanged(int value)
{
    colorA.setGreen(value);
    updateGL();
}

void
MainWindow::on_sliderBlueA_valueChanged(int value)
{
    colorA.setBlue(value);
    updateGL();
}

void
MainWindow::on_sliderRedD_valueChanged(int value)
{
    colorD.setRed(value);
    updateGL();
}

void
MainWindow::on_sliderGreenD_valueChanged(int value)
{
    colorD.setGreen(value);
    updateGL();
}

void
MainWindow::on_sliderBlueD_valueChanged(int value)
{
    colorD.setBlue(value);
    updateGL();
}

void
MainWindow::on_sliderRedS_valueChanged(int value)
{
    colorS.setRed(value);
    updateGL();
}

void
MainWindow::on_sliderGreenS_valueChanged(int value)
{
    colorS.setGreen(value);
    updateGL();
}

void
MainWindow::on_sliderBlueS_valueChanged(int value)
{
    colorS.setBlue(value);
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
MainWindow::on_sliderNormalLength_valueChanged(int value)
{
    normalLength = std::exp((float)value / 100.0f) - 1;
    updateGL();
}

void
MainWindow::on_comboBoxMaterial_activated(QString value)
{
    material = value;
    const Material m = MaterialLibrary::inst()->find(material.toStdString());
    applyColor(m.ambientColor(), colorA);
    applyColor(m.diffuseColor(), colorD);
    applyColor(m.specularColor(), colorS);
    specularPower = m.shininess() * 128;
    ambientIntensity = 1.0;
    diffuseIntensity = 1.0;
    specularIntensity = 1.0;
    updateGUI();
    updateGL();
}

void
MainWindow::on_comboBoxTexture_activated(QString value)
{
    texture = value;
    updateGL();
}

void
MainWindow::on_comboBoxShader1_activated(QString value)
{
    this->shader1 = value;
    ui.gl->setShader(0, shader1);
    ui.gl->postUpdate();
}

void
MainWindow::on_comboBoxShader2_activated(QString value)
{
    this->shader2 = value;
    ui.gl->setShader(1, shader2);
    ui.gl->postUpdate();
}

void
MainWindow::on_pushButtonAutoRotate_toggled(void)
{
    ui.gl->setAutoRotate(ui.pushButtonAutoRotate->isChecked());
}

void
MainWindow::updateGUI()
{
    ui.sliderRedA->setValue(colorA.red());
    ui.sliderGreenA->setValue(colorA.green());
    ui.sliderBlueA->setValue(colorA.blue());
    ui.sliderRedD->setValue(colorD.red());
    ui.sliderGreenD->setValue(colorD.green());
    ui.sliderBlueD->setValue(colorD.blue());
    ui.sliderRedS->setValue(colorS.red());
    ui.sliderGreenS->setValue(colorS.green());
    ui.sliderBlueS->setValue(colorS.blue());
    ui.sliderAmbientIntensity->setValue(ambientIntensity * 100);
    ui.sliderDiffuseIntensity->setValue(diffuseIntensity * 100);
    ui.sliderSpecularIntensity->setValue(specularIntensity * 100);
    ui.sliderSpecularPower->setValue(specularPower);
    ui.sliderNormalLength->setValue(std::log(normalLength + 1) * 100);
    
    int idx = ui.comboBoxShader1->findText(shader1);
    if (idx >= 0) {
	ui.comboBoxShader1->setCurrentText(shader1);
    }
    
    int matIdx = ui.comboBoxMaterial->findText(material);
    if (matIdx >= 0) {
	ui.comboBoxMaterial->setCurrentText(material);
    }
}

void
MainWindow::setColor(QLabel *label, const QColor &color, float intensity)
{
    QColor col;
    float red = std::min((float)color.redF() * intensity, 1.0f);
    float green = std::min((float)color.greenF() * intensity, 1.0f);
    float blue = std::min((float)color.blueF() * intensity, 1.0f);
    col.setRgbF(red, green, blue);

    QPalette palette = label->palette();
    palette.setColor(label->backgroundRole(), col);
    label->setPalette(palette);
}

void
MainWindow::updateGL()
{
    ui.labelRedADisplay->setText(QString::number(colorA.redF(), 'f', 2));
    ui.labelGreenADisplay->setText(QString::number(colorA.greenF(), 'f', 2));
    ui.labelBlueADisplay->setText(QString::number(colorA.blueF(), 'f', 2));
    ui.labelAmbientIntensityDisplay->setText(QString::number(ambientIntensity, 'f', 2));
    ui.labelRedDDisplay->setText(QString::number(colorD.redF(), 'f', 2));
    ui.labelGreenDDisplay->setText(QString::number(colorD.greenF(), 'f', 2));
    ui.labelBlueDDisplay->setText(QString::number(colorD.blueF(), 'f', 2));
    ui.labelDiffuseIntensityDisplay->setText(QString::number(diffuseIntensity, 'f', 2));
    ui.labelRedSDisplay->setText(QString::number(colorS.redF(), 'f', 2));
    ui.labelGreenSDisplay->setText(QString::number(colorS.greenF(), 'f', 2));
    ui.labelBlueSDisplay->setText(QString::number(colorS.blueF(), 'f', 2));
    ui.labelSpecularIntensityDisplay->setText(QString::number(specularIntensity, 'f', 2));
    ui.labelSpecularPowerDisplay->setText(QString::number(specularPower, 'f', 0));
    ui.labelNormalLengthDisplay->setText(QString::number(normalLength, 'f', 2));
    
    setColor(ui.labelColorADisplay, colorA, 1.0f);
    setColor(ui.labelColorADisplayI, colorA, ambientIntensity);
    setColor(ui.labelColorDDisplay, colorD, 1.0f);
    setColor(ui.labelColorDDisplayI, colorD, diffuseIntensity);
    setColor(ui.labelColorSDisplay, colorS, 1.0f);
    setColor(ui.labelColorSDisplayI, colorS, specularIntensity);
    
    ui.gl->setTextureName(texture);
    ui.gl->setColors(colorA, colorD, colorS);
    ui.gl->setNormalLength(normalLength);
    ui.gl->setIntensity(ambientIntensity, diffuseIntensity, specularIntensity, specularPower);
    ui.gl->postUpdate();
}
