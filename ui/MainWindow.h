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
    QColor color;
    
    void updateGUI();
    void updateGL();
    
private slots:
    void on_actionOpen_triggered(void);
    void on_actionExit_triggered(void);
    void on_sliderRed_valueChanged(int value);
    void on_sliderGreen_valueChanged(int value);
    void on_sliderBlue_valueChanged(int value);
    void on_sliderAmbientIntensity_valueChanged(int value);
    void on_sliderDiffuseIntensity_valueChanged(int value);
    void on_sliderSpecularIntensity_valueChanged(int value);
    void on_sliderSpecularPower_valueChanged(int value);
};

#endif	/* _MAINWINDOW_H */
