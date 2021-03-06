/*********************************************************
 * NewImageDialog.h
 * Author:      Pascal Gendron
 * Version:     0.1.0
 * *******************************************************/

#ifndef NEWIMAGEDIALOG_H
#define NEWIMAGEDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include "editingwidget.h"

/************************************************************************
 * The purpose of this class is to create the UI to facilitate the creation
 * of a new image. The user is asked to enter the size of one side of the
 * image and is invited to enter a name and a path where to save the image.
 * The image is not saved through this class but the user will have the
 * possibility to skip the "save as..." step later.
 * **********************************************************************/

namespace Ui {
class NewImageDialog;
}

class NewImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewImageDialog(QWidget *parent = 0);
    ~NewImageDialog();

    /* Events */
    void accept();
    void reject();
    void closeEvent(QCloseEvent* event);

    void setEditingWidgetReference(EditingWidget *editingWidget);

private:
    bool SavingInfoValid();

private slots:
    void on_pushButtonPath_clicked();

private:
    Ui::NewImageDialog *ui;
    EditingWidget *m_EditingWidget;
};

#endif // NEWIMAGEDIALOG_H
