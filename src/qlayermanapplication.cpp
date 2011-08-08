/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the
** Free Software Foundation, Inc., 59 Temple Place - Suite 330,
** Boston, MA 02111-1307, USA.
**
****************************************************************************/

#include <QWidget>
#include <QDebug>

#include "qlayermanapplication.h"


namespace {
    const t_ilm_layer defaultLayer   = 2000;
    const t_ilm_layer defaultSurface =   20;
}


QLayerManApplication::QLayerManApplication(int &argc, char **argv)
    : QApplication(argc, argv), lm_initialized(false), layerid(0), surfaceid(0), window(0)
{
    parseLayerManArgs();
}

QLayerManApplication::QLayerManApplication(Display *display, int &argc, char **argv, Qt::HANDLE visual, Qt::HANDLE colormap)
    : QApplication(display, argc, argv, visual, colormap), lm_initialized(false), layerid(0), surfaceid(0), window(0)
{
    parseLayerManArgs();
}

void QLayerManApplication::parseLayerManArgs()
{
    QStringList cmdline = arguments();

    int id = cmdline.indexOf(QLatin1String("-layer"));
    if(id >= 0 && id < cmdline.length()-1)  {
        int layer = cmdline.at(id+1).toInt();
        if(layer > 0)
            layerid = layer;
    }

    id = cmdline.indexOf(QLatin1String("-surface"));
    if(id >= 0 && id < cmdline.length()-1) {
        int surface = cmdline.at(id+1).toInt();
        if(surface > 0)
            surfaceid = surface;
    }
}

QLayerManApplication::~QLayerManApplication()
{
    if(lm_initialized)
        ilm_destroy();
}

bool QLayerManApplication::notify(QObject *receiver, QEvent *event)
{
    if(event->type() == QEvent::Show && receiver->isWidgetType() && !receiver->parent()) {
        registerWindow(qobject_cast<QWidget*>(receiver));
    } else if(receiver == window && event->type() == QEvent::Move) {
        ilm_surfaceSetDestinationRectangle(surfaceid, window->x(), window->y(), window->width(), window->height());
        ilm_commitChanges();
    }

    return QApplication::notify(receiver, event);
}

void QLayerManApplication::setLayer(t_ilm_layer layer)
{
    layerid = layer;
}

void QLayerManApplication::setSurface(t_ilm_surface surface)
{
    surfaceid = surface;
}

bool QLayerManApplication::registerWindow(QWidget *win, t_ilm_layer layer, t_ilm_surface surface)
{
    if(window || !win)    // only the first window will be regisered
        return false;

    if(!lm_initialized) {
        if(ilm_init() == ILM_SUCCESS) {
            lm_initialized = true;
        } else {
            qWarning("IVI LayerManager client could not be initialized");
            return false;
        }
    }

    if(!layerid)
        layerid = layer ? layer : defaultLayer;
    if(!surfaceid)
        surfaceid = surface ? surface : defaultSurface;

    bool success =       (ilm_surfaceCreate((t_ilm_nativehandle)win->winId(), win->width(), win->height(),
                                            ILM_PIXELFORMAT_RGBA_8888, &surfaceid) == ILM_SUCCESS);
    success = success && (ilm_layerAddSurface(layerid, surfaceid) == ILM_SUCCESS);
    success = success && (ilm_surfaceSetVisibility(surfaceid, ILM_TRUE) == ILM_SUCCESS);
    success = success && (ilm_commitChanges() == ILM_SUCCESS);

    if(success) {
        window = win;
        qDebug() << "Registered window \"" << win << "\" on layer" << layerid << ", surface" << surfaceid;
    } else {
        qWarning() << "Could not register window" << win;
    }

    return success;
}

