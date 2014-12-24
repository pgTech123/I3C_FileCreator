#include "newimagedialog.h"
#include "ui_newimagedialog.h"

NewImageDialog::NewImageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewImageDialog)
{
    ui->setupUi(this);
}

NewImageDialog::~NewImageDialog()
{
    delete ui;
}

/* Events */
void NewImageDialog::closeEvent(QCloseEvent*)
{
    delete this;
}

void NewImageDialog::setPtrToImage(Image *imgPtr)
{

}
