/*********************************************************
 * AboutUs.h
 * Author: Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/

#ifndef ABOUTUS_H
#define ABOUTUS_H

#include <QWidget>

/***********************************************************************
 * The purpose of this class is to define the UI of the About Us Window.
 * ********************************************************************/
namespace Ui {
class AboutUs;
}

class AboutUs : public QWidget
{
    Q_OBJECT

public:
    explicit AboutUs(QWidget *parent = 0);
    ~AboutUs();

private:
    Ui::AboutUs *ui;
};

#endif // ABOUTUS_H
