#include <QtGui>
#include <QKeyEventTransition>

 #include "window.h"
 #include "movementtransition.h"

 Window::Window()
 {
     pX = 5;
     pY = 5;

     QFontDatabase database;
     QFont font;
     if (database.families().contains("Monospace")) {
         font = QFont("Monospace");
     }
     else {
         foreach (QString family, database.families()) {
             if (database.isFixedPitch(family)) {
                 font = QFont(family);
                 break;
             }
         }
     }
 #if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
     font.setPointSize(5);
 #else
     font.setPointSize(12);
 #endif
     setFont(font);

     setupMap();
     buildMachine();
 }

 void Window::setStatus(const QString &status)
 {
     myStatus = status;
     repaint();
 }

 QString Window::status() const
 {
     return myStatus;
 }

 void Window::paintEvent(QPaintEvent * /* event */)
 {
     QFontMetrics metrics(font());
     QPainter painter(this);
     int fontHeight = metrics.height();
     int fontWidth = metrics.width('X');
     int yPos = fontHeight;
     int xPos;

     painter.fillRect(rect(), Qt::black);
     painter.setPen(Qt::white);

     painter.drawText(QPoint(0, yPos), status());

     for (int y = 0; y < HEIGHT; ++y) {
         yPos += fontHeight;
         xPos = 0;

         for (int x = 0; x < WIDTH; ++x) {
             if (y == pY && x == pX) {
                 xPos += fontWidth;
                 continue;
             }

             painter.drawText(QPoint(xPos, yPos), map[x][y]);
             xPos += fontWidth;
         }
     }
     painter.drawText(QPoint(pX * fontWidth, (pY + 2) * fontHeight), QChar('@'));
 }

 QSize Window::sizeHint() const
 {
     QFontMetrics metrics(font());

     return QSize(metrics.width('X') * WIDTH, metrics.height() * (HEIGHT + 1));
 }

 void Window::buildMachine()
 {
     machine = new QStateMachine;

     QState *inputState = new QState(machine);
     inputState->assignProperty(this, "status", "Move the rogue with 2, 4, 6, and 8");

     MovementTransition *transition = new MovementTransition(this);
     inputState->addTransition(transition);

     QState *quitState = new QState(machine);
     quitState->assignProperty(this, "status", "Really quit(y/n)?");

     QKeyEventTransition *yesTransition = new
         QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_Y);
     yesTransition->setTargetState(new QFinalState(machine));
     quitState->addTransition(yesTransition);

     QKeyEventTransition *noTransition =
         new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_N);
     noTransition->setTargetState(inputState);
     quitState->addTransition(noTransition);

     QKeyEventTransition *quitTransition =
         new QKeyEventTransition(this, QEvent::KeyPress, Qt::Key_Q);
     quitTransition->setTargetState(quitState);
     inputState->addTransition(quitTransition);

     machine->setInitialState(inputState);

     connect(machine, SIGNAL(finished()), qApp, SLOT(quit()));

     machine->start();
 }

 void Window::movePlayer(Direction direction)
 {
     switch (direction) {
         case Left:
             if (map[pX - 1][pY] != '#')
                 --pX;
             break;
         case Right:
             if (map[pX + 1][pY] != '#')
                 ++pX;
             break;
         case Up:
             if (map[pX][pY - 1] != '#')
                 --pY;
             break;
         case Down:
             if (map[pX][pY + 1] != '#')
                 ++pY;
             break;
     }
     repaint();
 }

 void Window::setupMap()
 {
     qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

     for (int x = 0; x < WIDTH; ++x)
         for (int y = 0; y < HEIGHT; ++y) {
         if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1 || qrand() % 40 == 0)
             map[x][y] = '#';
         else
             map[x][y] = '.';
     }
 }
