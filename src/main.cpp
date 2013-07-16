#include <QApplication>
#include <QDialog>
#include <QtGui>
#include "forms/formmain.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FormMain * formMain = new FormMain();
    formMain->show();

    return app.exec();
}
