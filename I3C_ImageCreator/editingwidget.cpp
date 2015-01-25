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

    /* Make connections */
    connect(m_History, SIGNAL(undoCall(HistoryElement)),
            m_PixmapLayerStack->getUIQLabel(), SLOT(historyUndoCall(DrawingHistory)));
    connect(m_History, SIGNAL(redoCall(HistoryElement)),
            m_PixmapLayerStack->getUIQLabel(), SLOT(historyRedoCall(DrawingHistory)));
    connect(m_PixmapLayerStack->getUIQLabel(), SIGNAL(modificationMade(HistoryElement)),
            m_History, SLOT(appendHistoryElement(HistoryElement)));
}

void EditingWidget::deleteImageAndLS()
{
    if(m_Image != NULL){
        delete m_Image;
        m_Image = NULL;
    }
    if(m_PixmapLayerStack != NULL){
        /* Clear connections */
        disconnect(m_History, SIGNAL(undoCall(HistoryElement)),
                   m_PixmapLayerStack->getUIQLabel(), SLOT(historyUndoCall(DrawingHistory)));
        disconnect(m_History, SIGNAL(redoCall(HistoryElement)),
                   m_PixmapLayerStack->getUIQLabel(), SLOT(historyRedoCall(DrawingHistory)));
        disconnect(m_PixmapLayerStack->getUIQLabel(), SIGNAL(modificationMade(HistoryElement)),
                m_History, SLOT(appendHistoryElement(HistoryElement)));

        delete m_PixmapLayerStack;
        m_PixmapLayerStack = NULL;
    }
}

void EditingWidget::setHistoryReference(History *history)
{
    m_History = history;
}

void EditingWidget::newImage(int sideSize)
{
    instanciateImageAndLS();
    m_QStringPath.clear();
    m_Image->setSideSize(sideSize);
    m_PixmapLayerStack->setSideSize(sideSize);

    initDisplayLayerStack();
}

bool EditingWidget::isImage()
{
    if(m_Image != NULL || m_PixmapLayerStack != NULL){
        return true;
    }
    return false;
}

bool EditingWidget::openImage(QString path)
{
    instanciateImageAndLS();
    bool error = m_Image->open(path.toStdString().c_str());
    if(error == NO_ERRORS){
        m_Image->convertImageToLayerStack(m_PixmapLayerStack);
        initDisplayLayerStack();
        m_PixmapLayerStack->goToLayer(0);
        m_QStringPath = path;
        return true;
    }
    else{
        return false;
    }
}

void EditingWidget::setSavingPath(QString path)
{
    m_QStringPath = path;
}

bool EditingWidget::save(QString path)
{
    m_QStringPath = path;
    return save();
}

bool EditingWidget::save()
{
    if(m_QStringPath.isEmpty()){
        return false;
    }
    m_Image->convertLayerStackToImage((LayerStack*) m_PixmapLayerStack);
    return m_Image->save(m_QStringPath.toStdString().c_str());
}


void EditingWidget::selectedColor(int r, int g, int b)
{
    if(m_PixmapLayerStack != NULL){
        m_PixmapLayerStack->getUIQLabel()->setActiveColor(r, g, b);
    }
}


void EditingWidget::on_pushButtonPrevious_clicked()
{
    if(m_PixmapLayerStack != NULL){
        m_PixmapLayerStack->previousLayer();
        ui->horizontalSliderLayer->setValue(m_PixmapLayerStack->getCurrentLayer());
        ui->labelLayerValue->setText(QString::number(m_PixmapLayerStack->getCurrentLayer()));
    }
}

void EditingWidget::on_pushButtonNext_clicked()
{
    if(m_PixmapLayerStack != NULL){
        m_PixmapLayerStack->nextLayer();
        ui->horizontalSliderLayer->setValue(m_PixmapLayerStack->getCurrentLayer());
        ui->labelLayerValue->setText(QString::number(m_PixmapLayerStack->getCurrentLayer()));
    }
}

void EditingWidget::on_horizontalSliderLayer_valueChanged(int value)
{
    if(m_PixmapLayerStack != NULL){
        m_PixmapLayerStack->goToLayer(value);
    }
}

void EditingWidget::setCurrentLayer(int currentLayer)
{
    ui->labelLayerValue->setText(QString::number(currentLayer));
}

void EditingWidget::initDisplayLayerStack()
{
    if(m_PixmapLayerStack != NULL){
        /* Show */
        ui->gridLayout->addWidget(m_PixmapLayerStack->getUIQLabel(),1,1);

        /* Enable Control Buttons */
        ui->pushButtonPrevious->setEnabled(true);
        ui->pushButtonNext->setEnabled(true);
        ui->horizontalSliderLayer->setEnabled(true);

        /* Connect */
        ui->horizontalSliderLayer->setMaximum(m_PixmapLayerStack->getSideSize()-1);
        connect(ui->horizontalSliderLayer,SIGNAL(valueChanged(int)), this, SLOT(setCurrentLayer(int)));
    }
}
