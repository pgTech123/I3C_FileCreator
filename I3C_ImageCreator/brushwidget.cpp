#include "brushwidget.h"
#include "ui_brushwidget.h"

BrushWidget::BrushWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::BrushWidget)
{
    ui->setupUi(this);
    ui->dockWidgetContents->setLayout(ui->gridLayout);

    m_iBrushSize = 1;
    m_SelectedBrush = Pen;
}

BrushWidget::~BrushWidget()
{
    delete ui;
}

void BrushWidget::closeEvent(QCloseEvent *closeEvent)
{
    closeEvent->ignore();
    this->hide();
    emit hidden();
}

void BrushWidget::on_pushButtonBrush_clicked()
{
    m_SelectedBrush = Pen;
    emit brushSelected(m_iBrushSize);
}

void BrushWidget::on_pushButtonEraser_clicked()
{
    m_SelectedBrush = Eraser;
    emit eraserSelected(m_iBrushSize);
}

void BrushWidget::on_spinBoxPenSize_valueChanged(int value)
{
    m_iBrushSize = value;

    if(m_SelectedBrush == Pen){
        emit brushSelected(m_iBrushSize);
    }
    else if(m_SelectedBrush == Eraser){
        emit eraserSelected(m_iBrushSize);
    }
}
