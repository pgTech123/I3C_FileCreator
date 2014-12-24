/*********************************************************
 * NewImageDialog.cpp
 * Author: Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/

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
void NewImageDialog::accept()
{
    /* Convert Values */
    int sideSize = ui->comboBoxSize->currentText().toInt();

    /* Create Image */
    *ptrImage = new Image();

    /* Set Image Properties */
    (*ptrImage)->setSideSize(sideSize);
    (*ptrImage)->initializeEmpty();
    if(SavingInfoValid()){
        /* Concat and convert strings */
        QString pathQString = ui->lineEditPath->text() + "/" + ui->lineEditFileName->text() + ".i3c";
        const char* path  = pathQString.toStdString().c_str();
        (*ptrImage)->setPath(path);
    }
    /* Close the window */
    this->close();
}

void NewImageDialog::reject()
{
    this->close();
}

void NewImageDialog::closeEvent(QCloseEvent*)
{
    delete this;
}

/* Public Functions */
void NewImageDialog::setPtrToImage(Image **imgPtr)
{
    ptrImage = imgPtr;
}

bool NewImageDialog::SavingInfoValid()
{
    if(ui->lineEditFileName->text().isEmpty() || ui->lineEditPath->text().isEmpty()){
        return false;
    }
    return true;
}

/* Private slots */
void NewImageDialog::on_pushButtonPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Choose Directory...", QString());
    ui->lineEditPath->setText(path);
}
