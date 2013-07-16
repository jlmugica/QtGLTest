#include <QtGui>
#include <QtOpenGL>

#include <math.h>
#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    mPosition.setX(DEFAULT_X_POS);
    mPosition.setY(DEFAULT_Y_POS);
    mPosition.setZ(DEFAULT_Z_POS);

    mRotation.setX(DEFAULT_X_ROT);
    mRotation.setY(DEFAULT_Y_ROT);
    mRotation.setZ(DEFAULT_Z_ROT);

    mLightPosition.setX(DEFAULT_X_POS);
    mLightPosition.setY(DEFAULT_Y_POS);
    mLightPosition.setZ(DEFAULT_Z_POS);

    mClearColor = Qt::black;

    mLeft = DEFAULT_LEFT;
    mRight = DEFAULT_RIGHT;
    mTop = DEFAULT_TOP;
    mBottom = DEFAULT_BOTTOM;
    mNear = DEFAULT_NEAR;
    mFar = DEFAULT_FAR;

    mRenderingMode = FILL;
    mProjectionType = PERSPECTIVE;

    mZoomingIn = false;
    mZoomingOut = false;
    mZoomAmount = 0.0;

    mNumCubes = 0;
    mCubeSize = 0;
}

GLWidget::~GLWidget()
{
    glDeleteLists(mCubeDisplayList, 1);
    makeCurrent();
}

void GLWidget::updateViewport(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	glFrustum(mLeft, mRight, mBottom, mTop, mNear, mFar);

    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::setXRotation(double angle)
{
    normalizeAngle(&angle);
    if (angle != mRotation.x()) {
        mRotation.setX(angle);
        emit(xRotationChanged(angle));
        updateGL();
    }
}

void GLWidget::setYRotation(double angle)
{
    normalizeAngle(&angle);
    if (angle != mRotation.y()) {
        mRotation.setY(angle);
        emit(yRotationChanged(angle));
        updateGL();
    }
}

void GLWidget::setXPosition(double xPos)
{
    if(xPos != mPosition.x())
    {
        mPosition.setX(xPos);
        emit(xPositionChanged(xPos));
        updateGL();
    }
}

void GLWidget::setYPosition(double yPos)
{
    if(yPos != mPosition.y())
    {
        mPosition.setY(yPos);
        emit(yPositionChanged(yPos));
        updateGL();
    }
}

void GLWidget::initializeGL()
{
    qglClearColor(mClearColor);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);        
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Lighting
    glEnable (GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    // Create display list
    createCubeDisplayList();
}

void GLWidget::setNumCubes(int numCubes)
{
    mNumCubes = numCubes;
    mCubeSize = 1.0f / (float)numCubes;
}

void GLWidget::forceDraw()
{
    // Paint the scene
    updateGL();
}

void GLWidget::paintGL()
{
    // Update zoom
    if(mZoomingIn)
    {
        mZoomAmount--;
        mPosition.setZ(mPosition.z() + (1.0/10.0));

        if(mZoomAmount == 0)
            mZoomingIn = false;
    }
    else if(mZoomingOut)
    {
        mZoomAmount--;
        mPosition.setZ(mPosition.z() - (1.0/10.0));

        if(mZoomAmount == 0)
            mZoomingOut = false;
    }

    // Lighting
    GLfloat ambientColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Translation & rotation
    glTranslated(mPosition.x(), mPosition.y(), mPosition.z());
    glRotated(mRotation.x(), 1.0, 0.0, 0.0);
    glRotated(mRotation.y(), 0.0, 1.0, 0.0);
    glRotated(mRotation.z(), 0.0, 0.0, 1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Draw cubes
    drawCubes();
}

bool GLWidget::drawCubes()
{
    bool result = true;

    for(int i=-mNumCubes/2; i<=mNumCubes/2; ++i)
    {
        for(int j=-mNumCubes/2; j<=mNumCubes/2; ++j)
        {
            for(int k=-mNumCubes/2; k<=mNumCubes/2; ++k)
            {
                result &= drawCube(QVector3D(i*mCubeSize, j*mCubeSize, k*mCubeSize));
            }
        }
    }

    return result;
}

void GLWidget::createCubeDisplayList()
{
    // Create one display list
    mCubeDisplayList = glGenLists(1);

    if(mCubeDisplayList <= 0)
    {
        qDebug("ERROR: Creating display list");
        return;
    }

    // Compile the display list
    glNewList(mCubeDisplayList, GL_COMPILE);
    {
        drawCube(QVector3D(0,0,0),1.f);
    }
    glEndList();

    GLenum error = glGetError();

    if(error != 0)
    {
        qDebug() << "GL Error:" << error;
        return;
    }
}

bool GLWidget::drawCube(QVector3D position, float side)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Set position
    glTranslated(position.x(), position.y(), position.z());

    glBegin(GL_QUADS);

    QColor faceColor = Qt::red;
    glColor3f(faceColor.redF(), faceColor.greenF(), faceColor.blueF());

    // Front
    glTexCoord2f(0, 0);
    glNormal3f(0.f, 0.f, 1.f);
    glVertex3f(-side / 2.0, -side / 2.0, side / 2.0);
    glTexCoord2f(1, 0);
    glVertex3f(side / 2.0, -side / 2.0, side / 2.0);
    glTexCoord2f(1, 1);
    glVertex3f(side / 2.0, side / 2.0, side / 2.0);
    glTexCoord2f(0, 1);
    glVertex3f(-side / 2.0, side / 2.0, side / 2.0);

    faceColor = Qt::blue;
    glColor3f(faceColor.redF(), faceColor.greenF(), faceColor.blueF());

    // Left
    glTexCoord2f(0, 0);
    glVertex3f(-side / 2.0, -side / 2.0, -side / 2.0);
    glTexCoord2f(1, 0);
    glVertex3f(-side / 2.0, -side / 2.0, side / 2.0);
    glTexCoord2f(1, 1);
    glVertex3f(-side / 2.0, side / 2.0, side / 2.0);
    glTexCoord2f(0, 1);
    glVertex3f(-side / 2.0, side / 2.0, -side / 2.0);

    faceColor = Qt::green;
    glColor3f(faceColor.redF(), faceColor.greenF(), faceColor.blueF());

    // Right
    glTexCoord2f(0, 0);
    glVertex3f(side / 2.0, -side / 2.0, side / 2.0);
    glTexCoord2f(1, 0);
    glVertex3f(side / 2.0, -side / 2.0, -side / 2.0);
    glTexCoord2f(1, 1);
    glVertex3f(side / 2.0, side / 2.0, -side / 2.0);
    glTexCoord2f(0, 1);
    glVertex3f(side / 2.0, side / 2.0, side / 2.0);

    faceColor = Qt::yellow;
    glColor3f(faceColor.redF(), faceColor.greenF(), faceColor.blueF());

    // Back
    glTexCoord2f(0, 0);
    glVertex3f(side / 2.0, -side / 2.0, -side / 2.0);
    glTexCoord2f(0, 1);
    glVertex3f(-side / 2.0, -side / 2.0, -side / 2.0);
    glTexCoord2f(1, 1);
    glVertex3f(-side / 2.0, side / 2.0, -side / 2.0);
    glTexCoord2f(1, 0);
    glVertex3f(side / 2.0, side / 2.0, -side / 2.0);

    faceColor = Qt::cyan;
    glColor3f(faceColor.redF(), faceColor.greenF(), faceColor.blueF());

    // Top
    glVertex3f(-side / 2.0, side / 2.0, side / 2.0);
    glTexCoord2f(1, 0);
    glVertex3f(side / 2.0, side / 2.0, side / 2.0);
    glTexCoord2f(1, 1);
    glVertex3f(side / 2.0, side / 2.0, -side / 2.0);
    glTexCoord2f(0, 1);
    glVertex3f(-side / 2.0, side / 2.0, -side / 2.0);

    faceColor = Qt::magenta;
    glColor3f(faceColor.redF(), faceColor.greenF(), faceColor.blueF());

    // Bottom
    glTexCoord2f(0, 0);
    glVertex3f(-side / 2.0, -side / 2.0, -side / 2.0);
    glTexCoord2f(1, 0);
    glVertex3f(side / 2.0, -side / 2.0, -side / 2.0);
    glTexCoord2f(1, 1);
    glVertex3f(side / 2.0, -side / 2.0, side / 2.0);
    glTexCoord2f(0, 1);
    glVertex3f(-side / 2.0, -side / 2.0, side / 2.0);

    glEnd();

    glPopMatrix();

    GLenum error = glGetError();

    if(error != 0)
    {
        qDebug() << "GL Error:" << error;
        return false;
    }

    return true;
}

bool GLWidget::drawCube(QVector3D position)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslated(position.x(), position.y(), position.z());
    // Draw the cube a little bit smaller, so there is a space between cubes
    glScaled(mCubeSize*0.9, mCubeSize*0.9, mCubeSize*0.9);

    glCallList(mCubeDisplayList);

    glPopMatrix();

    GLenum error = glGetError();

    if(error != 0)
    {
        qDebug() << "GL Error:" << error;
        return false;
    }

    return true;
}

void GLWidget::resizeGL(int width, int height)
{
    updateViewport(width, height);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    mLastPosition = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - mLastPosition.x();
    int dy = event->y() - mLastPosition.y();

    if(event->buttons() & Qt::LeftButton)
    {
        setXRotation(mRotation.x() + (dy * 0.5));
        setYRotation(mRotation.y() + (dx * 0.5));
    }
    else if(event->buttons() & Qt::MidButton)
    {
        setXPosition(mPosition.x() + (dx * 0.01));
        setYPosition(mPosition.y() + (-dy * 0.01));
    }

    mLastPosition = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    // From http://doc.qt.nokia.com/latest/qwheelevent.html
    // "Most mouse types work in steps of 15 degrees, in which case the delta
    // value is a multiple of 120; i.e., 120 units * 1/8 = 15 degrees."
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    if(numSteps > 0)
        mZoomingIn = true;
    else
        mZoomingOut = true;

    // Smooth zoom multiplying by 10
    mZoomAmount = abs(numSteps * 10.0);
}

void GLWidget::normalizeAngle(double *angle)
{
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}
