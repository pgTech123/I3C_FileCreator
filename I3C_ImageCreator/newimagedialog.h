#ifndef NEWIMAGEDIALOG_H
#define NEWIMAGEDIALOG_H

#include <QDialog>
#include "image.h"

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
    void closeEvent(QCloseEvent* event);

    void setPtrToImage(Image* imgPtr);

private:
    Ui::NewImageDialog *ui;
};

#endif // NEWIMAGEDIALOG_H
