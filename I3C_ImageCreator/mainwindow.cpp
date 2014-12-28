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
    ui->centralwidget->setLayout(ui->gridLayout);
    initMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;

    deleteM_Image();
    deleteM_LayerStack();
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
    m_LayerStack = NULL;

    /* Dock Painting Window */
    this->addDockWidget(Qt::RightDockWidgetArea, &m_PaintingWindow);
    connect(&m_PaintingWindow, SIGNAL(hidden()), this,SLOT(paintingWindowClosed()));
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
    /* Ask if we want to discard changes */
    /* Here, we consider that if there is an image, there is a layerStack */
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
        deleteM_Image();
        deleteM_LayerStack();
    }
    return true;
}

void MainWindow::deleteM_Image()
{
    if(m_Image != NULL){
        delete m_Image;
        m_Image = NULL;
    }
}

void MainWindow::deleteM_LayerStack()
{
    if(m_LayerStack != NULL){
        disconnect(m_LayerStack,SIGNAL(initLayerStackDisplay()),this,SLOT(initDisplayLayerStack()));
        disconnect(ui->horizontalSliderLayer,SIGNAL(valueChanged(int)), this, SLOT(setCurrentLayer(int)));
        disconnect(&m_PaintingWindow,SIGNAL(selectedColor(int,int,int)), m_LayerStack, SLOT(setActiveColor(int,int,int)));

        delete m_LayerStack;
        m_LayerStack = NULL;

        ui->pushButtonPrevious->setEnabled(false);
        ui->pushButtonNext->setEnabled(false);
        ui->horizontalSliderLayer->setEnabled(false);
    }
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

void MainWindow::initDisplayLayerStack()
{
    if(m_LayerStack != NULL){
        /* Show */
        ui->gridLayout->addWidget(m_LayerStack,1,1);

        /* Enable Control Buttons */
        ui->pushButtonPrevious->setEnabled(true);
        ui->pushButtonNext->setEnabled(true);
        ui->horizontalSliderLayer->setEnabled(true);

        /* Connect */
        ui->horizontalSliderLayer->setMaximum(m_LayerStack->getSideSize());
        connect(ui->horizontalSliderLayer,SIGNAL(valueChanged(int)), m_LayerStack, SLOT(goToLayer(int)));
        connect(ui->horizontalSliderLayer,SIGNAL(valueChanged(int)), this, SLOT(setCurrentLayer(int)));
        connect(&m_PaintingWindow,SIGNAL(selectedColor(int,int,int)), m_LayerStack, SLOT(setActiveColor(int,int,int)));
    }
}


/* Private Slots */
/* Menu File */
void MainWindow::on_actionNew_triggered()
{
    if(tryToClearImage()){
        /* Create a new Image */
        m_NewImageDialogWindow = new NewImageDialog(this);
        m_NewImageDialogWindow->setPtrToImage(&m_Image);
        m_NewImageDialogWindow->setPtrToLayerStack(&m_LayerStack);
        connect(m_NewImageDialogWindow, SIGNAL(connectLayerStack()), this,SLOT(connectLayerStack()));
        m_NewImageDialogWindow->show();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if(tryToClearImage())
    {
        QString path = QFileDialog::getOpenFileName(this, "Load an Image",
                                                  QString(), "3D Image(*.i3c)");

        m_Image = new Image(path.toStdString().c_str());

        if(m_Image->open() == NO_ERRORS){
            /* Load image in the layerStack */
            m_LayerStack = new LayerStack(this);
            connect(m_LayerStack,SIGNAL(initLayerStackDisplay()),this,SLOT(initDisplayLayerStack()));
            m_Image->convertImageToLayerStack(&m_LayerStack);
        }
        else{
            /* Display Error */
            QMessageBox::warning(this, "ERROR",
                                 tr("An error orrured while trying to open an this image..."));
            deleteM_Image();
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if(isImage()){
        if(m_Image->isPath()){
            m_Image->convertLayerStackToImage(m_LayerStack);
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
        m_Image->convertLayerStackToImage(m_LayerStack);
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

/* Menu View */
void MainWindow::on_actionPainting_Window_triggered()
{
    if(!ui->actionPainting_Window->isChecked()){
        m_PaintingWindow.hide();
    }
    else{
        m_PaintingWindow.show();
    }
}

/* Menu Help */
void MainWindow::on_actionAbout_Us_triggered()
{
    m_AboutUsWindow.show();
}

/* Buttons */
void MainWindow::on_pushButtonPrevious_clicked()
{
    if(m_LayerStack != NULL){
        m_LayerStack->previousLayer();
        ui->horizontalSliderLayer->setValue(m_LayerStack->getCurrentLayer());
        ui->labelLayerValue->setText(QString::number(m_LayerStack->getCurrentLayer()));
    }
}

void MainWindow::on_pushButtonNext_clicked()
{
    if(m_LayerStack != NULL){
        m_LayerStack->nextLayer();
        ui->horizontalSliderLayer->setValue(m_LayerStack->getCurrentLayer());
        ui->labelLayerValue->setText(QString::number(m_LayerStack->getCurrentLayer()));
    }
}


void MainWindow::setCurrentLayer(int currentLayer)
{
    ui->labelLayerValue->setText(QString::number(currentLayer));
}

/* Other Slots */
void MainWindow::connectLayerStack()
{
    if(m_LayerStack != NULL){
        connect(m_LayerStack,SIGNAL(initLayerStackDisplay()),this,SLOT(initDisplayLayerStack()));
    }
}

void MainWindow::paintingWindowClosed()
{
    ui->actionPainting_Window->setChecked(false);
}
