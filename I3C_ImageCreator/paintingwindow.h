/*********************************************************
 * PaintingWindow.h
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/

#ifndef PAINTINGWINDOW_H
#define PAINTINGWINDOW_H

#include <QDockWidget>
#include <QCloseEvent>
#include <QColorDialog>
#include "colorpad.h"

#define NUM_OF_COLOR_PADS   25
#define COLOR_PAD_ROWS      5
#define COLOR_PAD_COLUMN    5


/************************************************************************
 * The purpose of this class is to create the window that allow the user
 * choose painting options. The first iteration of this class will only
 * allow the user to choose a color. Futur iteration might want to allow
 * his the option th choose brush, color picker, ...
 * **********************************************************************/

namespace Ui {
class PaintingWindow;
}

class PaintingWindow : public QDockWidget
{
    Q_OBJECT

public:
    explicit PaintingWindow(QWidget *parent = 0);
    ~PaintingWindow();
    void closeEvent(QCloseEvent *closeEvent);

private slots:
    void selectedPad(int r, int g, int b, int id);

    /* Color */
    void on_spinBoxRed_valueChanged(int value);
    void on_spinBoxGreen_valueChanged(int value);
    void on_spinBoxBlue_valueChanged(int value);

    void on_pushButton_ColorPalette_clicked();

signals:
    void hidden();
    void selectedColor(int r, int g, int b);

private:
    Ui::PaintingWindow *ui;

    /* Color Pad Variables */
    ColorPad *m_ColorPads;
    int m_iColorPadSelected;
};

#endif // PAINTINGWINDOW_H
