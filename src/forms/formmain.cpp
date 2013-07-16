#include "ui_formmain.h"
#include "formmain.h"
#include <QMessageBox>
#include <QDebug>

#define UPDATE_FPS_TIME 1.0*1000.0 // 1 sec
#define UPDATE_DRAW_TIME (1.0/60.0)*1000.0 // Max 60 FPS
#define INITIAL_NUMBER_CUBES 10

FormMain::FormMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::FormMain)
{
    ui->setupUi(this);

    mGlWindow = new GLWidget(this);
    ui->mainLayout->addWidget(mGlWindow);

    // Draw timer
    mDrawTimer = new QTimer();
    connect(mDrawTimer, SIGNAL(timeout()), this, SLOT(updateDraw()));

    mDrawTimer->start(UPDATE_DRAW_TIME);

    mNumFrames = 0;

    // FPS timer
    mFPSTimer = new QTimer();
    connect(mFPSTimer, SIGNAL(timeout()), this, SLOT(updateFPS()));

    // Refresh FPS every sec
    mFPSTimer->start(UPDATE_FPS_TIME);

    // Cube's side slider
    connect(ui->hsNumberCubes, SIGNAL(valueChanged(int)), this, SLOT(setNumCubes(int)));
    ui->hsNumberCubes->setValue(INITIAL_NUMBER_CUBES);
}

FormMain::~FormMain()
{
    delete ui;   
}

void FormMain::updateDraw()
{
    // Repaint
    mGlWindow->forceDraw();

    mNumFrames++;
}

void FormMain::updateFPS()
{
    ui->lFPS->setText("FPS: " + QString::number(mNumFrames));

    mNumFrames = 0;
}

void FormMain::setNumCubes(int value)
{
    mGlWindow->setNumCubes(value);
    ui->lCubeSide->setText("Cube side: " + QString::number(value));
}
