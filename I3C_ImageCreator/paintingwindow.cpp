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
    ui->dockWidgetContents->setLayout(ui->verticalLayout);

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
    setPresetColors();
    m_iColorPadSelected = 0;    //Preset first color
    m_ColorPads[m_iColorPadSelected].select();
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

void PaintingWindow::emitSelectedColor()
{
    int r = m_ColorPads[m_iColorPadSelected].getRed();
    int g = m_ColorPads[m_iColorPadSelected].getGreen();
    int b = m_ColorPads[m_iColorPadSelected].getBlue();

    emit selectedColor(r,g,b);
    emit selectedColor();
}

void PaintingWindow::setPresetColors()
{                                                 /*Red, Gre, Blue*/
    int presetColors[NUM_OF_PRESET_COLORS * 3] = {  255, 255, 255,
                                                    255, 0,   0,
                                                    0,   255, 0,
                                                    0,   0,   255,
                                                    255, 255, 0,
                                                    255, 0,   255,
                                                    0,   255, 255,
                                                    128, 128, 128,
                                                    128, 255, 0,
                                                    128, 0,   255,

                                                    255, 128, 128,
                                                    0,   255, 128};

    for(int i = 0; i < NUM_OF_PRESET_COLORS; i++){
        if(i < NUM_OF_COLOR_PADS){
            m_ColorPads[i].setRed(presetColors[i*3]);
            m_ColorPads[i].setGreen(presetColors[i*3 + 1]);
            m_ColorPads[i].setBlue(presetColors[i*3 + 2]);
            m_ColorPads[i].unselect();
        }
    }
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
    emit selectedColor();
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

void PaintingWindow::on_pushButton_ColorPalette_clicked()
{
    QColor currentColor;
    currentColor.setRed(m_ColorPads[m_iColorPadSelected].getRed());
    currentColor.setGreen(m_ColorPads[m_iColorPadSelected].getGreen());
    currentColor.setBlue(m_ColorPads[m_iColorPadSelected].getBlue());

    QColor color = QColorDialog::getColor(currentColor, this);

    if(color.isValid()){
        m_ColorPads[m_iColorPadSelected].setRed(color.red());
        m_ColorPads[m_iColorPadSelected].setGreen(color.green());
        m_ColorPads[m_iColorPadSelected].setBlue(color.blue());
    }
}
