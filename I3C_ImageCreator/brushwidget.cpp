#include "brushwidget.h"
#include "ui_brushwidget.h"

BrushWidget::BrushWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::BrushWidget)
{
    ui->setupUi(this);
    ui->dockWidgetContents->setLayout(ui->gridLayout);
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

}

void BrushWidget::on_pushButtonEraser_clicked()
{

}
