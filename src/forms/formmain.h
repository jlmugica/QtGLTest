#ifndef FORMMAIN_H
#define FORMMAIN_H

#include <QtGui>
#include <QMainWindow>

#include "../controls/glwidget.h"

namespace Ui {
    class FormMain;
}

class FormMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit FormMain(QWidget *parent = 0);
    ~FormMain();

private slots:
    void updateDraw();
    void updateFPS();
    void setNumCubes(int value);

private:
    Ui::FormMain *ui;
    GLWidget * mGlWindow;

    // Draw Timer
    QTimer * mDrawTimer;
    // FPS timer
    QTimer * mFPSTimer;
    int mNumFrames;
};

#endif // FORMMAIN_H
