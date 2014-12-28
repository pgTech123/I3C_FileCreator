#ifndef PAINTINGWINDOW_H
#define PAINTINGWINDOW_H

#include <QDockWidget>
#include <QCloseEvent>

namespace Ui {
class PaintingWindow;
}

class PaintingWindow : public QDockWidget
{
    Q_OBJECT

public:
    explicit PaintingWindow(QWidget *parent = 0);
    ~PaintingWindow();
    void closeEvent(QCloseEvent *closeEvent);

signals:
    void hidden();
    void selectedColor(int r, int g, int b);

private:
    Ui::PaintingWindow *ui;
};

#endif // PAINTINGWINDOW_H
