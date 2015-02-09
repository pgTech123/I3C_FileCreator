#ifndef EDITINGWIDGET_H
#define EDITINGWIDGET_H

#include <QWidget>
#include <QString>

#include "history.h"
#include "image.h"
#include "pixmaplayerstack.h"

#include "iostream"
using namespace std;

namespace Ui {
class EditingWidget;
}

class EditingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditingWidget(QWidget *parent = 0);
    ~EditingWidget();

    void setHistoryReference(History *history);

    void newImage(int sideSize);
    bool isImage();
    bool openImage(QString path);
    void setSavingPath(QString path);
    bool save(QString path);
    bool save();

public slots:
    /* Buttons & Widget Elements */
    void on_pushButtonPrevious_clicked();
    void on_pushButtonNext_clicked();
    void on_horizontalSliderLayer_valueChanged(int value);

    void selectedColor(int r, int g, int b);
    void selectedEraser(int eraserSize);
    void selectedPen(int penSize);

private slots:
    void setCurrentLayer(int currentLayer);

private:
    void instanciateImageAndLS();
    void deleteImageAndLS();

    void initDisplayLayerStack();

signals:
    void pingSelectedColor();

private:
    Ui::EditingWidget *ui;

    QString m_QStringPath;

    /* Internal memory management */
    Image *m_Image;
    PixmapLayerStack *m_PixmapLayerStack;

    /* Memory management somewhere else */
    History *m_History;
};

#endif // EDITINGWIDGET_H
