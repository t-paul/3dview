/* 
 * File:   MainWindow.h
 * Author: tp
 *
 * Created on September 28, 2014, 4:58 PM
 */

#ifndef _MAINWINDOW_H
#define	_MAINWINDOW_H

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();
    
private:
    Ui::MainWindow ui;
    
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
    float specularPower;
    float normalLength;
    QString material;
    QString shader1;
    QString shader2;
    QColor colorA;
    QColor colorD;
    QColor colorS;
    
    void setColor(QLabel *label, const QColor &color, float intensity);
    void applyColor(glm::vec3 color, QColor &qColor);
    void updateGUI();
    void updateGL();
    
private slots:
    void on_actionOpen_triggered(void);
    void on_actionExit_triggered(void);
    void on_sliderRedA_valueChanged(int value);
    void on_sliderGreenA_valueChanged(int value);
    void on_sliderBlueA_valueChanged(int value);
    void on_sliderRedD_valueChanged(int value);
    void on_sliderGreenD_valueChanged(int value);
    void on_sliderBlueD_valueChanged(int value);
    void on_sliderRedS_valueChanged(int value);
    void on_sliderGreenS_valueChanged(int value);
    void on_sliderBlueS_valueChanged(int value);
    void on_sliderAmbientIntensity_valueChanged(int);
    void on_sliderDiffuseIntensity_valueChanged(int);
    void on_sliderSpecularIntensity_valueChanged(int);
    void on_sliderSpecularPower_valueChanged(int);
    void on_sliderNormalLength_valueChanged(int);
    void on_comboBoxMaterial_activated(QString);
    void on_comboBoxShader1_activated(QString);
    void on_comboBoxShader2_activated(QString);
    void on_pushButtonAutoRotate_toggled(void);
};

#endif	/* _MAINWINDOW_H */
