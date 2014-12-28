/*********************************************************
 * ColorPad.h
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/

#ifndef COLORPAD_H
#define COLORPAD_H

#include <QLabel>
#include <QPixmap>
#include <QColor>
#include <QPainter>
#include <QResizeEvent>

#define SELECTED_CONTOUR_RED      50
#define SELECTED_CONTOUR_GREEN    250
#define SELECTED_CONTOUR_BLUE     250

/************************************************************************
 * The purpose of this class is to create the UI for a single color pad
 * **********************************************************************/

class ColorPad: public QLabel
{
    Q_OBJECT
public:
    ColorPad(QWidget* parent = 0);
    ~ColorPad();

    /* Events */
    void mousePressEvent(QMouseEvent*);
    void resizeEvent(QResizeEvent *event);

    void setId(int id);

private:
    void drawSelectedContour();

public slots:
    void setRed(int value);
    void setGreen(int value);
    void setBlue(int value);

    void unselect();

signals:
    void ImSelected(int r, int g, int b, int id);

private:
    int m_iId;

    int m_red;
    int m_green;
    int m_blue;

    QPixmap *m_PixmapColor;
};

#endif // COLORPAD_H
