/////////////////////////////////////////////////////////////////////////////
/// Name:        main.cpp
/// Purpose:     main class : shows the main numSCAL window
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include <QApplication>
#include "mainwindow.h"

int main(int argc , char *argv[])
{
   QApplication a(argc,argv);

   MainWindow window;
   window.show();

   return a.exec();
}
