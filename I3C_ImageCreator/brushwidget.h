#ifndef BRUSHWIDGET_H
#define BRUSHWIDGET_H

#include <QDockWidget>
#include <QCloseEvent>

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

signals:
    void hidden();

private:
    Ui::BrushWidget *ui;
};

#endif // BRUSHWIDGET_H
