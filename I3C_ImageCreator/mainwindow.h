/*********************************************************
 * MainWindow.h
 * Author: Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include "aboutus.h"

/************************************************************************
 * The purpose of this class is to create the UI of the main window and
 * to make sure all of the actions availlable to the user are dispatched
 * properly.
 * **********************************************************************/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /* Events */
    void closeEvent(QCloseEvent* event);

private:
    /* Initialisation */
    void initMainWindow();

private slots:
    /* Menu File */
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionQuit_triggered();

    /* Menu Edit */
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();

    /* Menu Help */
    void on_actionAbout_Us_triggered();

private:
    Ui::MainWindow *ui;

    bool m_bAllChangesSaved;
};

#endif // MAINWINDOW_H
