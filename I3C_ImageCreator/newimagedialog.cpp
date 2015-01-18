/*********************************************************
 * NewImageDialog.cpp
 * Author:      Pascal Gendron
 * Version:     0.1.0
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
    m_EditingWidget->newImage(sideSize);

    if(SavingInfoValid()){
        QString pathQString = ui->lineEditPath->text() + "/" + ui->lineEditFileName->text() + ".i3c";
        m_EditingWidget->setSavingPath(pathQString);
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

void NewImageDialog::setEditingWidgetReference(EditingWidget *editingWidget)
{
    m_EditingWidget = editingWidget;
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
