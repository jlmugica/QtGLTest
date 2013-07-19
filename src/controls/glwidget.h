#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QVector3D>

// Default ortographic perspective parameters
#define DEFAULT_LEFT -1.0
#define DEFAULT_RIGHT 1.0
#define DEFAULT_TOP 1.0
#define DEFAULT_BOTTOM -1.0
#define DEFAULT_NEAR 4.0
#define DEFAULT_FAR 50.0

// Default camera parameters
#define DEFAULT_X_ROT 20.0
#define DEFAULT_Y_ROT 45.0
#define DEFAULT_Z_POS -10.0

// From http://doc.qt.nokia.com/latest/qwheelevent.html
// "Most mouse types work in steps of 15 degrees, in which case the delta
// value is a multiple of 120; i.e., 120 units * 1/8 = 15 degrees."
#define DELTA_2_DEGREES 1.0 / 8.0;
#define DEGREES_2_STEPS 1.0 / 15.0;

#define MOVE_MOUSE_FACTOR 0.5
#define ROTATE_MOUSE_FACTOR 0.01

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:          

    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void updateViewport(int width, int height);

    // Attributes
    QVector3D getPosition() { return mPosition; }

public slots:
    void setXRotation(double angle);
    void setYRotation(double angle);

    void setXPosition(double xPos);
    void setYPosition(double yPos);
    void setZPosition(double zPos);

    void setNumCubes(int numCubes);
    void forceDraw();

protected:
    bool init();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    bool drawScene();
    void createCubeDisplayList();
    bool drawDisplayListCube(QVector3D position);
    bool drawCube(QVector3D position, float mSide);

private:
    QVector3D mPosition;
    QVector3D mRotation;
    QPoint mLastPosition;

    QColor mClearColor;

    // Smooth zoom
    bool mZoomingIn;
    bool mZoomingOut;
    float mZoomAmount;
    float mZoomSmoothFactor;

    // Ortho parameters
    double mLeft;
    double mRight;
    double mTop;
    double mBottom;
    double mNear;
    double mFar;

    // Light attributes
    QVector3D mLightPosition;

    // Number of cubes
    int mNumCubes;

    // Cube's size
    float mCubeSize;

    // Display list
    GLuint mCubeDisplayList;

    void normalizeAngle(double *angle);
};

#endif // GLWIDGET_H
