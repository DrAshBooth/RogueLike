#ifndef WINDOW_H
 #define WINDOW_H

 #include <QWidget>

 class QState;
 class QStateMachine;
 class QTransition;

 #if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
 #define WIDTH 43
 #define HEIGHT 14
 #else
 #define WIDTH 70
 #define HEIGHT 40
 #endif

 class Window : public QWidget
 {
     Q_OBJECT
     Q_PROPERTY(QString status READ status WRITE setStatus)

 public:
     enum Direction { Up, Down, Left, Right };

     Window();

     void movePlayer(Direction direction);
     void setStatus(const QString &status);
     QString status() const;

     QSize sizeHint() const;

 protected:
     void paintEvent(QPaintEvent *event);

 private:
     void buildMachine();
     void setupMap();

     QChar map[WIDTH][HEIGHT];
     int pX, pY;

     QStateMachine *machine;
     QString myStatus;
 };

 #endif
