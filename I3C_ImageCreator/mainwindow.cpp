/*********************************************************
 * MainWindow.cpp
 * Author: Pascal Gendron
 * Version:     0.1.0
 * *******************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /* Prepare UI */
    ui->setupUi(this);
    ui->centralwidget->setLayout(ui->gridLayout);

    /* Instanciate Permanent Widgets */
    m_EditingWidget = new EditingWidget();
    m_PaintingWidget = new PaintingWindow();
    m_BrushWidget = new BrushWidget();
    m_AboutUsWindow = new AboutUs();

    /* Set Widget in UI */
    ui->gridLayout->addWidget(m_EditingWidget, 0, 0);
    this->addDockWidget(Qt::RightDockWidgetArea, m_PaintingWidget);
    this->addDockWidget(Qt::RightDockWidgetArea, m_BrushWidget);

    /* Connect Close Call to View Menu */
    connect(m_PaintingWidget, SIGNAL(hidden()), this,SLOT(paintingWindowClosed()));
    connect(m_BrushWidget, SIGNAL(hidden()), this,SLOT(brushWidgetClosed()));

    /* Other Connections */
    connect(m_PaintingWidget, SIGNAL(selectedColor(int,int,int)), m_EditingWidget, SLOT(selectedColor(int,int,int)));
    connect(m_BrushWidget,SIGNAL(brushSelected(int)), m_EditingWidget, SLOT(selectedPen(int)));
    connect(m_BrushWidget, SIGNAL(eraserSelected(int)), m_EditingWidget,SLOT(selectedEraser(int)));

    /* Init Project Status */
    m_bAllChangesSaved = true;
    ui->actionUndo->setDisabled(true);
    ui->actionRedo->setDisabled(true);

    /* History */
    m_History = new History(50);
    m_EditingWidget->setHistoryReference(m_History);
    //Insert all other element that needs to be connected to history here

    connect(m_History, SIGNAL(newElementInHistory()), this, SLOT(modificationUnsaved()));
    connect(m_History, SIGNAL(enableUndoButton(bool)), this, SLOT(enableUndo(bool)));
    connect(m_History, SIGNAL(enableRedoButton(bool)), this, SLOT(enableRedo(bool)));
    connect(this, SIGNAL(undoSignal()), m_History, SLOT(undo()));
    connect(this, SIGNAL(redoSignal()), m_History, SLOT(redo()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_EditingWidget;
    delete m_PaintingWidget;
    delete m_BrushWidget;
    delete m_AboutUsWindow;

    delete m_History;
}

/* Events */
void MainWindow::closeEvent(QCloseEvent* event)
{
    if(m_bAllChangesSaved){
        qApp->exit();
    }
    else{
        if(doYouWantToSaveChanges() == GO_ON){
            qApp->exit();
        }
        else{
            event->ignore();
        }
    }
}


void MainWindow::enableUndo(bool enabled)
{
    ui->actionUndo->setEnabled(enabled);
}

void MainWindow::enableRedo(bool enabled)
{
    ui->actionRedo->setEnabled(enabled);
}

void MainWindow::modificationUnsaved()
{
    m_bAllChangesSaved = false;
}

SavingStatus MainWindow::doYouWantToSaveChanges()
{
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
            return GO_ON;
        }
        else{
            return ABORT;
        }
    }
    else if(answer == QMessageBox::No){
        return GO_ON;
    }
    else{
       return ABORT;
    }

}

/* Menu File */
void MainWindow::on_actionNew_triggered()
{
    if(!m_bAllChangesSaved){
        if(doYouWantToSaveChanges() == ABORT){
            return;
        }
    }
    m_NewImageDialogWindow = new NewImageDialog(this);
    m_NewImageDialogWindow->setEditingWidgetReference(m_EditingWidget);
    m_NewImageDialogWindow->show();     /*NewImageDialog kills himself automatically*/
}


void MainWindow::on_actionOpen_triggered()
{
    if(!m_bAllChangesSaved){
        if(doYouWantToSaveChanges() == ABORT){
            return;
        }
    }
    QString path = QFileDialog::getOpenFileName(this, "Load an Image", QString(), "3D Image(*.i3c)");
    if(path == NULL){   //Cancel Button Pressed
        return;
    }
    if(!m_EditingWidget->openImage(path)){
        QMessageBox::warning(this, "ERROR", "An error orrured while trying to open an this image...");
    }
}

void MainWindow::on_actionSave_triggered()
{
    if(m_EditingWidget->isImage()){
        if(!m_EditingWidget->save()){
            on_actionSave_As_triggered();
        }
        else{
            m_bAllChangesSaved = true;
        }
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    /* Ask where to save and under what name */
    if(m_EditingWidget->isImage()){
        QString path = QFileDialog::getSaveFileName(this, "Save Image As...",
                                                QString(), "3D Image (*.i3c)");
        if(path == NULL){   //Cancel Button Pressed
            return;
        }
        if(!m_EditingWidget->save(path)){
            QMessageBox::warning(this, "ERROR",
                                 tr("An error occured trying to save the image. Make sure the file is not")+
                                 tr(" open in an other programm and try again."));
        }
        else{
            m_bAllChangesSaved = true;
        }
    }
}

void MainWindow::on_actionQuit_triggered()
{
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
        m_PaintingWidget->hide();
    }
    else{
        m_PaintingWidget->show();
    }
}

void MainWindow::on_actionBrush_Window_triggered()
{
    if(!ui->actionBrush_Window->isChecked()){
        m_BrushWidget->hide();
    }
    else{
        m_BrushWidget->show();
    }
}

/* Menu Help */
void MainWindow::on_actionAbout_Us_triggered()
{
    m_AboutUsWindow->show();
}


void MainWindow::paintingWindowClosed()
{
    ui->actionPainting_Window->setChecked(false);
}

void MainWindow::brushWidgetClosed()
{
    ui->actionBrush_Window->setChecked(false);
}
