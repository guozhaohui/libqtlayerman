TARGET   = controller
TEMPLATE = app

HEADERS  = window.h \
           control.h
SOURCES  = main.cpp \
           window.cpp \
           control.cpp

INCLUDEPATH += /usr/local/include/ilm
LIBS        += -lilmClient


