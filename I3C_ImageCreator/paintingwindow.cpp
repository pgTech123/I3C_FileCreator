#include "paintingwindow.h"
#include "ui_paintingwindow.h"

PaintingWindow::PaintingWindow(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PaintingWindow)
{
    ui->setupUi(this);
    ui->dockWidgetContents->setLayout(ui->verticalLayout);
}

PaintingWindow::~PaintingWindow()
{
    delete ui;
}

void PaintingWindow::closeEvent(QCloseEvent *closeEvent)
{
    closeEvent->ignore();
    this->hide();
    emit hidden();
}
