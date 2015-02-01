#ifndef BRUSHWIDGET_H
#define BRUSHWIDGET_H

#include <QDockWidget>
#include <QCloseEvent>

enum BrushType{
    Pen, Eraser
};

namespace Ui {
class BrushWidget;
}

class BrushWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit BrushWidget(QWidget *parent = 0);
    ~BrushWidget();
    void closeEvent(QCloseEvent *closeEvent);

private slots:
    void on_pushButtonBrush_clicked();
    void on_pushButtonEraser_clicked();
    void on_spinBoxPenSize_valueChanged(int value);

signals:
    void hidden();
    void brushSelected(int brushSize);
    void eraserSelected(int brushSize);

private:
    Ui::BrushWidget *ui;

    int m_iBrushSize;
    BrushType m_SelectedBrush;
};

#endif // BRUSHWIDGET_H
