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
    const t_ilm_layer defaultLayer = 2000;
}

QLayerManApplication::QLayerManApplication(int &argc, char **argv)
    : QApplication(argc, argv), m_initialized(false)
{
    parseLayerManArgs();
}

QLayerManApplication::QLayerManApplication(Display *display, int &argc, char **argv, Qt::HANDLE visual, Qt::HANDLE colormap)
    : QApplication(display, argc, argv, visual, colormap), m_initialized(false)
{
    parseLayerManArgs();
}

void QLayerManApplication::parseLayerManArgs()
{
    QStringList cmdline = arguments();
    m_layerId   = LS_UNDEF;
    m_surfaceId = LS_UNDEF;

    int id = cmdline.indexOf(QLatin1String("-layer"));
    if(id >= 0 && id < cmdline.length()-1)  {
        int layer = cmdline.at(id+1).toInt();
        if(layer > 0)
            m_layerId = layer;
    }

    id = cmdline.indexOf(QLatin1String("-surface"));
    if(id >= 0 && id < cmdline.length()-1) {
        int surface = cmdline.at(id+1).toInt();
        if(surface > 0)
            m_surfaceId = surface;
    }
}

QLayerManApplication::~QLayerManApplication()
{
    if(m_initialized)
        ilm_destroy();
}

bool QLayerManApplication::notify(QObject *receiver, QEvent *event)
{
    if(receiver->isWidgetType() && !receiver->parent()) {
        QWidget *window = qobject_cast<QWidget*>(receiver);
        if (window) {
            if (event->type() == QEvent::Show) {
                registerWindow(window);
            } else if (m_windows.contains(window)) {
                switch(event->type()) {
                case QEvent::Move:
                    t_ilm_uint pos[2];
                    pos[0] = window->x();
                    pos[1] = window->y();
                    ilm_surfaceSetPosition(m_windows[window], pos);
                    ilm_commitChanges();
                    break;
                default:
                    //qDebug() << "~Event:" << event->type();
                    break;
                }
            }
        }
    }

    return QApplication::notify(receiver, event);
}

void QLayerManApplication::setSurface(t_ilm_surface surfaceId)
{
    m_surfaceId = surfaceId;
}

void QLayerManApplication::setLayer(t_ilm_layer layerId)
{
    m_layerId = layerId;
}

bool QLayerManApplication::registerWindow(QWidget *win, t_ilm_surface surfaceId, t_ilm_layer layerId)
{
    if(!win || m_windows.contains(win))
        return false;

    if(!m_initialized) {
        if(ilm_init() == ILM_SUCCESS) {
            m_initialized = true;
        } else {
            qWarning("IVI LayerManager client could not be initialized");
            return false;
        }
    }

    if(layerId == LS_UNDEF)
        layerId = m_layerId != LS_UNDEF ? m_layerId : defaultLayer;

    if(surfaceId == LS_UNDEF)
        surfaceId = m_surfaceId;
    QList<t_ilm_surface> sids = m_windows.values();
    while(sids.contains(surfaceId))
        surfaceId++;

    //qDebug() << "Trying to register window \"" << win << "\" on surface" << surfaceId << ", layer" << layerId;
    bool success = ilm_surfaceCreate((t_ilm_nativehandle)win->winId(), win->width(), win->height(),
                                     ILM_PIXELFORMAT_RGBA_8888, &surfaceId) == ILM_SUCCESS
                && ilm_layerAddSurface(layerId, surfaceId) == ILM_SUCCESS
                && ilm_surfaceSetVisibility(surfaceId, ILM_TRUE) == ILM_SUCCESS
                && ilm_commitChanges() == ILM_SUCCESS;

    if(success) {
        m_windows.insert(win, surfaceId);
        qDebug() << "Registered" << win << "[ Id:" << win->winId() << "]" << "on surface" << surfaceId << ", layer" << layerId;
    } else {
        qWarning() << "Could not register window" << win;
    }

    return success;
}
