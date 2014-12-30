/*********************************************************
 * ColorPad.cpp
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/

#include "colorpad.h"

ColorPad::ColorPad(QWidget *parent):
    QLabel(parent)
{
    m_red = 0;
    m_green = 0;
    m_blue = 0;

    /* Init UI */
    m_PixmapColor = new QPixmap(8,8);
    m_PixmapColor->fill(Qt::black);
    QLabel::setPixmap(*m_PixmapColor);
}

ColorPad::~ColorPad()
{
    delete m_PixmapColor;
}

void ColorPad::mousePressEvent(QMouseEvent*)
{
    /* Change UI (Blue Contour) */
    drawSelectedContour();

    emit ImSelected(m_red, m_green, m_blue, m_iId);
}

void ColorPad::resizeEvent(QResizeEvent *event)
{
    /* x 0.8 to be able to size down */
    int newHeight = event->size().height()*0.8;
    int newWidth = event->size().width()*0.8;

    if(newWidth >= 8 && newHeight >= 8){
        (*m_PixmapColor) = m_PixmapColor->scaled(newWidth, newHeight);
        QLabel::setPixmap(*m_PixmapColor);
    }
}

void ColorPad::setId(int id)
{
    m_iId = id;
}

void ColorPad::drawSelectedContour()
{
    QPainter painter(m_PixmapColor);
    painter.setPen(QPen(QBrush(QColor(SELECTED_CONTOUR_RED,
                                      SELECTED_CONTOUR_GREEN,
                                      SELECTED_CONTOUR_BLUE)),5));
    painter.drawRect(2,2,this->width(),this->height());
    painter.setPen(QPen(QBrush(QColor(0,0,0)),2));
    painter.drawRect(2,2,this->width(),this->height());
    QLabel::setPixmap(*m_PixmapColor);
}

void ColorPad::setRed(int value)
{
    m_red = value;

    /* Update Displayed Color */
    m_PixmapColor->fill(QColor(m_red, m_green, m_blue));
    QLabel::setPixmap(*m_PixmapColor);
    drawSelectedContour();
    emit ImSelected(m_red, m_green, m_blue, m_iId);
}

void ColorPad::setGreen(int value)
{
    m_green = value;

    /* Update Displayed Color */
    m_PixmapColor->fill(QColor(m_red, m_green, m_blue));
    QLabel::setPixmap(*m_PixmapColor);
    drawSelectedContour();
    emit ImSelected(m_red, m_green, m_blue, m_iId);
}

void ColorPad::setBlue(int value)
{
    m_blue = value;

    /* Update Displayed Color */
    m_PixmapColor->fill(QColor(m_red, m_green, m_blue));
    QLabel::setPixmap(*m_PixmapColor);
    drawSelectedContour();
    emit ImSelected(m_red, m_green, m_blue, m_iId);
}

void ColorPad::unselect()
{
    /* Display only color */
    m_PixmapColor->fill(QColor(m_red, m_green, m_blue));
    QLabel::setPixmap(*m_PixmapColor);
}
