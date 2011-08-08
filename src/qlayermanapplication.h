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

#ifndef QLAYERMANAPPLICATION_H
#define QLAYERMANAPPLICATION_H

#include <QApplication>
#include "ilm_client.h"

class QWidget;

class QLayerManApplication : public QApplication
{
public:
    QLayerManApplication(int &argc, char **argv);
    QLayerManApplication(Display *display, int &argc, char **argv, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0);
    virtual ~QLayerManApplication();

    virtual bool notify(QObject *receiver, QEvent *event);

    void setLayer(t_ilm_layer layer);
    void setSurface(t_ilm_layer surface);
    bool registerWindow(QWidget *win, t_ilm_layer layer = 0, t_ilm_surface surface = 0);

private:
    void parseLayerManArgs();

    bool           lm_initialized;
    t_ilm_layer    layerid;
    t_ilm_surface  surfaceid;
    QWidget       *window;
};

#endif //  QLAYERMANAPPLICATION_H

