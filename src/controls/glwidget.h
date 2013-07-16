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
#define DEFAULT_X_ROT 0.0
#define DEFAULT_Y_ROT 0.0
#define DEFAULT_Z_ROT 0.0
#define DEFAULT_X_POS 0.0
#define DEFAULT_Y_POS 0.0
#define DEFAULT_Z_POS -10.0

enum renderingMode { FILL, WIREFRAME, DOTS };
enum projectionType { PERSPECTIVE, ORTOGRAPHIC };

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

    void setNumCubes(int numCubes);
    void forceDraw();

signals:
    void xRotationChanged(double angle);
    void yRotationChanged(double angle);
    void xPositionChanged(double position);
    void yPositionChanged(double position);

protected:
    bool init();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    bool drawCubes();
    void createCubeDisplayList();
    bool drawCube(QVector3D position, float mSide);
    bool drawCube(QVector3D position);


private:
    QVector3D mPosition;
    QVector3D mRotation;
    QPoint mLastPosition;

    QColor mClearColor;

    renderingMode mRenderingMode;
    projectionType mProjectionType;

    // Smooth zoom
    bool mZoomingIn;
    bool mZoomingOut;
    float mZoomAmount;

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
