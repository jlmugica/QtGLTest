QT += opengl
DESTDIR = bin
UI_DIR = forms
MOC_DIR = tmp
OBJECTS_DIR = tmp
RCC_DIR = tmp

HEADERS += \
    src/controls/glwidget.h \
    src/forms/formmain.h
SOURCES += \
    src/main.cpp \
    src/controls/glwidget.cpp \
    src/forms/formmain.cpp

FORMS += \
    forms/formmain.ui
