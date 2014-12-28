/*********************************************************
 * PaintingWindow.cpp
 * Author:      Pascal Gendron
 * Version:     0.0.1
 * *******************************************************/


#include "paintingwindow.h"
#include "ui_paintingwindow.h"

PaintingWindow::PaintingWindow(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PaintingWindow)
{
    ui->setupUi(this);

    /* Initialize Color Pads */
    m_iColorPadSelected = 0;
    m_ColorPads = new ColorPad[NUM_OF_COLOR_PADS];

    /* Add Color Pads to Layout */
    for(int i = 0; i < COLOR_PAD_ROWS; i++)
    {
        for(int j = 0; j < COLOR_PAD_COLUMN; j++)
        {
            int index = j+ i* COLOR_PAD_COLUMN;
            if(index < NUM_OF_COLOR_PADS){
                ui->gridLayout->addWidget(&m_ColorPads[index],i,j);
                m_ColorPads[index].setId(index);
                connect(&m_ColorPads[index], SIGNAL(ImSelected(int,int,int, int)),this,SLOT(selectedPad(int,int,int,int)));
            }
        }
    }

    ui->dockWidgetContents->setLayout(ui->verticalLayout);
}

PaintingWindow::~PaintingWindow()
{
    delete ui;
    delete[] m_ColorPads;
}

void PaintingWindow::closeEvent(QCloseEvent *closeEvent)
{
    closeEvent->ignore();
    this->hide();
    emit hidden();
}

void PaintingWindow::selectedPad(int r, int g, int b, int id)
{
    m_iColorPadSelected = id;

    /* Unselect other color pads */
    for(int i = 0; i < NUM_OF_COLOR_PADS; i++)
    {
        if(i != id){
            m_ColorPads[i].unselect();
        }
    }
    ui->spinBoxRed->setValue(r);
    ui->spinBoxGreen->setValue(g);
    ui->spinBoxBlue->setValue(b);

    emit selectedColor(r,g,b);
}

void PaintingWindow::on_spinBoxRed_valueChanged(int value)
{
    m_ColorPads[m_iColorPadSelected].setRed(value);
}

void PaintingWindow::on_spinBoxGreen_valueChanged(int value)
{
    m_ColorPads[m_iColorPadSelected].setGreen(value);
}

void PaintingWindow::on_spinBoxBlue_valueChanged(int value)
{
    m_ColorPads[m_iColorPadSelected].setBlue(value);
}

