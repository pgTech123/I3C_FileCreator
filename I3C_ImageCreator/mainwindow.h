/*********************************************************
 * MainWindow.h
 * Author: Pascal Gendron
 * Version:     0.1.0
 * *******************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>

#include "newimagedialog.h"
#include "editingwidget.h"
#include "paintingwindow.h"
#include "aboutus.h"

#include "history.h"

/****************************************************************************
 * The purpose of this class is to create the UI of the main window and
 * to make sure all of the actions availlable to the user are dispatched
 * properly among child windows. It also manage the saving process: deleting
 * instruction sent to child are considered to be absolute. Children will not
 * verify if data will be lost.
 * **************************************************************************/

#define HISTORY_LENGHT      50

enum SavingStatus{
    GO_ON, ABORT
};

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

public slots:
    void enableUndo(bool enabled);
    void enableRedo(bool enabled);
    void modificationUnsaved();

private:
    SavingStatus doYouWantToSaveChanges();

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

    /* Close Calls from Child */
    void paintingWindowClosed();

signals:
    void undoSignal();
    void redoSignal();

private:
    Ui::MainWindow *ui;

    /* Status Variable */
    bool m_bAllChangesSaved;

    /* Child Widgets */
    NewImageDialog *m_NewImageDialogWindow;
    EditingWidget *m_EditingWidget;
    PaintingWindow *m_PaintingWidget;
    AboutUs *m_AboutUsWindow;

    History *m_History;
};

#endif // MAINWINDOW_H
