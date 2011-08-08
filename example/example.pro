TARGET       = example
TEMPLATE     = app
QT          += declarative opengl
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/qtlayerman  /usr/local/include/ilm
LIBS        += -lqtlayerman -lilmClient

SOURCES     = main.cpp \

OTHER_FILES = planes.qml

