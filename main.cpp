
#include <QApplication>
#include <QtGui>

 #include "window.h"

 int main(int argv, char **args)
 {
     QApplication app(argv, args);

     Window window;
 #if defined(Q_OS_SYMBIAN)
     window.showMaximized();
 #else
     window.show();
 #endif

     return app.exec();
 }
