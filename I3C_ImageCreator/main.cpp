/*********************************************************
 * I3C File Creator
 *
 * This program has for purpose to create I3C images easily.
 *
 * Author: Pascal Gendron
 *
 * Purpose: Create I3C images easilly
 *
 * Reference:   Use the Qt library (QApplication)
 *
 * File format: The program is design to read and write .I3C
 *              files. The description of the structure of such
 *              file will is written in the I3C_File_Structure
 *              file at https://github.com/pgTech123/GV3DFile.git
 *
 * Version:     0.0.1
 *
 * ***********************************************************/

#include <QApplication>
#include <QIcon>
#include "mainwindow.h"

/* Program Parameters */
#define APPLICATION_NAME    "I3C File Creator"
#define ICON_PATH           "./../ressources/GVisionLogo.ico"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow *window = new MainWindow();
    QIcon *icon = new QIcon(ICON_PATH);

    /* Set Window Parameters */
    window->setGeometry(100,100,720,405);
    window->setWindowTitle(APPLICATION_NAME);
    window->setWindowIcon(*icon);

    /* Display Window */
    window->show();

    return app.exec();
}
