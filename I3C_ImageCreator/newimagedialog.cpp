/*********************************************************
 * NewImageDialog.cpp
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/

#include "newimagedialog.h"
#include "ui_newimagedialog.h"

NewImageDialog::NewImageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewImageDialog)
{
    ui->setupUi(this);
    m_parent = parent;
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
    *m_ptrImage = new Image();

    /* Set Image Properties */
    (*m_ptrImage)->setSideSize(sideSize);
    if(SavingInfoValid()){
        /* Concat and convert strings */
        QString pathQString = ui->lineEditPath->text() + "/" + ui->lineEditFileName->text() + ".i3c";
        const char* path  = pathQString.toStdString().c_str();
        (*m_ptrImage)->save(path);
    }

    /* Create Layer Stack */
    *m_ptrLayerStack = new PixmapLayerStack();
    emit connectLayerStack();
    (*m_ptrLayerStack)->setSideSize(sideSize);

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
    m_ptrImage = imgPtr;
}

void NewImageDialog::setPtrToLayerStack(PixmapLayerStack** layerStackPtr)
{
    m_ptrLayerStack = layerStackPtr;
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
