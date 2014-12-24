/*********************************************************
 * MainWindow.cpp
 * Author: Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;

    if(m_Image != NULL){
        delete m_Image;
    }
}

/* Events */
void MainWindow::closeEvent(QCloseEvent* event)
{
    if(m_bAllChangesSaved){
        qApp->exit();
    }
    else{
        /* Ask if we want to save changes */
        int answer = QMessageBox::warning(this, "Modification not saved",
                             tr("The document has been modified.\n"
                             "Do you want to save changes?"),
                             QMessageBox::Save | QMessageBox::No
                             | QMessageBox::Cancel,
                             QMessageBox::Save);

        /* Catch answer and do the action associated to it */
        if(answer == QMessageBox::Save){
            on_actionSave_triggered();
            /* We verify if it worked */
            if(m_bAllChangesSaved){
                qApp->exit();
            }
            else{
                event->ignore();
            }
        }
        else if(answer == QMessageBox::No){
            qApp->exit();
        }
        else{
           event->ignore();
        }
    }
}

/* Private Functions */
void MainWindow::initMainWindow()
{
    m_bAllChangesSaved = true;

    /* Disable Undo&Redo as long as no changes are made */
    ui->actionUndo->setDisabled(true);
    ui->actionRedo->setDisabled(true);

    m_Image = NULL;
}

bool MainWindow::isImage()
{
    if(m_Image == NULL){
        /* Error: No Images */
        QMessageBox::warning(this, "Action Impossible",
                             tr("No images is currently loaded."));
        return false;
    }
    return true;
}

bool MainWindow::tryToClearImage()
{
    /* If there is an image, clear it (or not...) */
    if(m_Image != NULL){
        if(!m_bAllChangesSaved){
            /* Ask if we want to save changes */
            int answer = QMessageBox::warning(this, "Modification not saved",
                                 tr("The document has been modified.\n"
                                 "Do you want to save changes?"),
                                 QMessageBox::Save | QMessageBox::No
                                 | QMessageBox::Cancel,
                                 QMessageBox::Save);

            /* Catch answer and do the action associated to it */
            if(answer == QMessageBox::Save){
                on_actionSave_triggered();
                /* We verify if it worked */
                if(!m_bAllChangesSaved){
                    return false;
                }
            }
            else{
               return false;
            }
        }
        /* Delete it */
        delete m_Image;
        m_Image = NULL;
    }
    return true;
}

/* Public Slots */
void MainWindow::enableUndo(bool enabled)
{
    ui->actionUndo->setEnabled(enabled);
}

void MainWindow::enableRedo(bool enabled)
{
    ui->actionRedo->setEnabled(enabled);
}

void MainWindow::changeRequested()
{
    m_bAllChangesSaved = false;
}

/* Private Slots */
/* Menu File */
void MainWindow::on_actionNew_triggered()
{
    if(tryToClearImage()){
        /* Create a new Image */
        m_NewImageDialogWindow = new NewImageDialog();
        m_NewImageDialogWindow->setPtrToImage(&m_Image);
        m_NewImageDialogWindow->show();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if(tryToClearImage())
    {
        QString path = QFileDialog::getOpenFileName(this, "Load an Image",
                                                  QString(), "3D Image(*.i3c)");
        m_Image->setPath(path.toStdString().c_str());
    }
}

void MainWindow::on_actionSave_triggered()
{
    if(isImage()){
        if(m_Image->isPath()){
            if(!m_Image->save()){
                on_actionSave_As_triggered();
            }
        }
        else{
            on_actionSave_As_triggered();
        }
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    /* Ask where to save and under what name */
    if(isImage()){
        /* Prompt */
        QString path = QFileDialog::getSaveFileName(this, "Save Image As...",
                                                QString(), "3D Image (*.i3c)");
        m_Image->setPath(path.toStdString().c_str());
        m_Image->save();
    }
}

void MainWindow::on_actionQuit_triggered()
{
    /* Call close to allow prompt "Do you wanna save changes" */
    this->close();
}

/* Menu Edit */
void MainWindow::on_actionUndo_triggered()
{
    m_bAllChangesSaved = false;
    emit undoSignal();
}

void MainWindow::on_actionRedo_triggered()
{
    m_bAllChangesSaved = false;
    emit redoSignal();
}

/* Menu Help */
void MainWindow::on_actionAbout_Us_triggered()
{
    m_AboutUsWindow.show();
}
