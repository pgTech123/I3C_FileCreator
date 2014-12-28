/*********************************************************
 * MainWindow.h
 * Author: Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include "newimagedialog.h"
#include "layerstack.h"
#include "image.h"
#include "paintingwindow.h"
#include "aboutus.h"

/*DEBUG PURPOSE*/
#include <iostream>
using namespace std;
/*END DEBUG*/

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

    bool isImage();
    bool tryToClearImage();

    void deleteM_Image();
    void deleteM_LayerStack();


public slots:
    void enableUndo(bool enabled);
    void enableRedo(bool enabled);
    void changeRequested();

    void initDisplayLayerStack();

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

    /* Menu View */
    void on_actionPainting_Window_triggered();

    /* Menu Help */
    void on_actionAbout_Us_triggered();

    /* Buttons */
    void on_pushButtonPrevious_clicked();
    void on_pushButtonNext_clicked();
    void setCurrentLayer(int currentLayer);

    /* Other Slots */
    void connectLayerStack();

    void paintingWindowClosed();

signals:
    void undoSignal();
    void redoSignal();

private:
    Ui::MainWindow *ui;

    /* Status Variable */
    bool m_bAllChangesSaved;

    /* Data Variables */
    Image *m_Image;
    LayerStack *m_LayerStack;

    /* Subwindow */
    NewImageDialog *m_NewImageDialogWindow;
    PaintingWindow m_PaintingWindow;
    AboutUs m_AboutUsWindow;
};

#endif // MAINWINDOW_H
