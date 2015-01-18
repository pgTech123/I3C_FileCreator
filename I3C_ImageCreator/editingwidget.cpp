#include "editingwidget.h"
#include "ui_editingwidget.h"

EditingWidget::EditingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditingWidget)
{
    ui->setupUi(this);
    this->setLayout(ui->gridLayout);

    /* Pointer initialisation */
    m_Image = NULL;
    m_PixmapLayerStack = NULL;
}

EditingWidget::~EditingWidget()
{
    delete ui;
    deleteImageAndLS();
}

void EditingWidget::instanciateImageAndLS()
{
    deleteImageAndLS();
    m_Image = new Image();
    m_PixmapLayerStack = new PixmapLayerStack(this);
}

void EditingWidget::deleteImageAndLS()
{
    if(m_Image != NULL){
        delete m_Image;
        m_Image = NULL;
    }
    if(m_PixmapLayerStack != NULL){
        delete m_PixmapLayerStack;
        m_PixmapLayerStack = NULL;
    }
}

void EditingWidget::setHistoryReference(History *history)
{
    m_History = history;
    //TODO: Connections
}

void EditingWidget::newImage(int sideSize)
{
    instanciateImageAndLS();
    //TODO
}

bool EditingWidget::isImage()
{
    if(m_Image != NULL){
        //TODO
    }
}

bool EditingWidget::openImage(QString path)
{
    instanciateImageAndLS();
    //TODO
}

void EditingWidget::setSavingPath(QString path)
{
    //TODO
}

bool EditingWidget::save(QString path)
{
    //TODO
}

bool EditingWidget::save()
{
    //TODO
}


void EditingWidget::selectedColor(int r, int g, int b)
{
    //TODO
}


void EditingWidget::on_pushButtonPrevious_clicked()
{
//    if(m_LayerStack != NULL){
//        m_LayerStack->previousLayer();
//        ui->horizontalSliderLayer->setValue(m_LayerStack->getCurrentLayer());
//        ui->labelLayerValue->setText(QString::number(m_LayerStack->getCurrentLayer()));
//    }
}

void EditingWidget::on_pushButtonNext_clicked()
{
//    if(m_LayerStack != NULL){
//        m_LayerStack->nextLayer();
//        ui->horizontalSliderLayer->setValue(m_LayerStack->getCurrentLayer());
//        ui->labelLayerValue->setText(QString::number(m_LayerStack->getCurrentLayer()));
//    }
}

void EditingWidget::on_horizontalSliderLayer_valueChanged(int value)
{
//    if(m_LayerStack != NULL){
//        m_LayerStack->goToLayer(value);
//    }
}

//void MainWindow::setCurrentLayer(int currentLayer)
//{
//    ui->labelLayerValue->setText(QString::number(currentLayer));
//}


//void MainWindow::connectLayerStack()
//{
//    if(m_LayerStack != NULL){
//        connect(m_LayerStack->getUIQLabel(),SIGNAL(initLayerStackDisplay()),this,SLOT(initDisplayLayerStack()));
//    }
//}
