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
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Events */
void MainWindow::closeEvent(QCloseEvent* event)
{
    if(m_bAllChangesSaved){
        qApp->quit();
    }
    else{
        /* Ask if we want to save changes */
        //TODO
    }
}

/* Private Functions */
void MainWindow::initMainWindow()
{
    m_bAllChangesSaved = true;
}

/* Menu File */
void MainWindow::on_actionNew_triggered()
{

}

void MainWindow::on_actionOpen_triggered()
{

}

void MainWindow::on_actionSave_triggered()
{

}

void MainWindow::on_actionSave_As_triggered()
{

}

void MainWindow::on_actionQuit_triggered()
{
    /* Call close to allow prompt "Do you wanna save changes" */
    this->close();
}

/* Menu Edit */
void MainWindow::on_actionUndo_triggered()
{

}

void MainWindow::on_actionRedo_triggered()
{

}

/* Menu Help */
void MainWindow::on_actionAbout_Us_triggered()
{

}
