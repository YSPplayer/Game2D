#include "mainwindow.h"
#include <QtWidgets/QApplication>
using namespace Game::Client;
int main(int argc, char *argv[]) {
  
    QApplication a(argc, argv);
    MainWindow* w = new MainWindow;
    w->resize(1920, 1040);
    w->show();
    return a.exec();
}
