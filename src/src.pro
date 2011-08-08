TEMPLATE     = lib
TARGET       = qtlayerman

INCLUDEPATH +=/usr/local/include/ilm
LIBS        += -lilmClient

SOURCES      = qlayermanapplication.cpp
HEADERS      = qlayermanapplication.h

headers.files = $$HEADERS
isEmpty(INSTALLPREFIX) {
    headers.path = $$[QT_INSTALL_HEADERS]/$$TARGET
    target.path = $$[QT_INSTALL_LIBS]
} else {
    headers.path = $$INSTALLPREFIX/include/$$TARGET
    target.path = $$INSTALLPREFIX/lib/
}
INSTALLS += headers
INSTALLS += target

