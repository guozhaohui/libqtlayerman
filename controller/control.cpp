/***************************************************************************
**
** This file is part of libqtlayerman controller **
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).*
**
** All rights reserved.
** Contact:  Nokia Corporation (qt-info@nokia.com)
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
**  * Redistributions of source code must retain the above copyright notice,
**    this list of conditions and the following disclaimer.
**  * Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and/or other materials provided with ** the distribution.
**  * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor the
**    names of its contributors may be used to endorse or promote products
**    derived from this software without specific ** prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
** OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**************************************************************************/

#include <QDebug>
#include "control.h"


/* Layers are queried only once at initialization
   Surfaces are queried each time the current layer is changed */

Control::Control() : layerIdx(LS_UNDEF), surfaceIdx(LS_UNDEF)
{
    if(ilm_init() != ILM_SUCCESS)
        qWarning() << "error: ILM init!";

    t_ilm_int length;
    t_ilm_layer *pArray;
    ilm_getLayerIDs(&length, &pArray);
    if(length) {
        for(int i = 0; i < length; i++) {
            struct LayerProperties props;
            props.id = pArray[i];
            ilm_layerGetVisibility(props.id, &props.visibility);
            ilm_layerGetOpacity(props.id, &props.opacity);
            props.zorder = i + 1;
            layerProps.append(props);
        }
        length > 1 ? setLayerIndex(1) : setLayerIndex(0);
    }
}

Control::~Control()
{
    ilm_destroy();
}

void Control::screenResolution(unsigned int *pWidth, unsigned int *pHeight)
{
    ilm_getScreenResolution(0, pWidth, pHeight);
}

int Control::layerIndex() const
{
    return layerIdx;
}

QStringList Control::layerIdsList() const
{
    QStringList list;
    foreach(LayerProperties prop, layerProps) {
        list.append(QString("%1").arg(prop.id));
    }
    return list;
}

int Control::layerVisibility() const
{
    return layerIdx != LS_UNDEF ? layerProps[layerIdx].visibility : false;
}

int Control::layerOpacity() const
{
    return layerIdx != LS_UNDEF ? layerProps[layerIdx].opacity*opacityScale() : 0;
}

int Control::layerZOrder() const
{
    return layerIdx != LS_UNDEF ? layerProps[layerIdx].zorder : 0;
}


int Control::surfaceIndex() const
{
    return surfaceIdx;
}

QStringList Control::surfaceIdsList() const
{
    QStringList list;
    foreach(SurfaceProperties prop, surfaceProps) {
        list.append(QString("%1").arg(prop.id));
    }
    return list;
}

int Control::surfaceVisibility() const
{
    return surfaceIdx != LS_UNDEF ? surfaceProps[surfaceIdx].visibility : false;
}

int Control::surfaceOpacity() const
{
    return surfaceIdx != LS_UNDEF ? surfaceProps[surfaceIdx].opacity*opacityScale() : 0;
}

int Control::surfaceZOrder() const
{
    return surfaceIdx != LS_UNDEF ? surfaceProps[surfaceIdx].zorder : 0;
}


QPointF Control::surfacePos() const
{
    return surfaceIdx != LS_UNDEF ? QPointF(surfaceProps[surfaceIdx].position[0], surfaceProps[surfaceIdx].position[1])
                                  : QPointF(0.0, 0.0);
}


QSizeF Control::surfaceDim() const
{
    return surfaceIdx != LS_UNDEF ? QSizeF(surfaceProps[surfaceIdx].dimension[0], surfaceProps[surfaceIdx].dimension[1])
                                  : QSizeF(0.0, 0.0);
}


void Control::setLayerIndex(int index)
{
    if(index != static_cast<int>(layerIdx)) {
        layerIdx = index;
        t_ilm_int length;
        t_ilm_surface *pArray;
        ilm_getSurfaceIDsOnLayer(layerProps[layerIdx].id, &length, &pArray);
        surfaceProps.clear();
        if(length) {
            for(int i = 0; i < length; i++) {
                struct SurfaceProperties props;
                props.id = pArray[i];
                ilm_surfaceGetVisibility(props.id, &props.visibility);
                ilm_surfaceGetOpacity(props.id, &props.opacity);
                props.zorder = i + 1;
                ilm_surfaceGetPosition(props.id, props.position);
                ilm_surfaceGetDimension(props.id, props.dimension);
                surfaceProps.append(props);
            }
            setSurfaceIndex(0);
        } else {
            setSurfaceIndex(LS_UNDEF);
        }
    }
}

void Control::setLayerOpacity(int value)
{
    if(layerIdx != LS_UNDEF) {
        layerProps[layerIdx].opacity = static_cast<t_ilm_float>(value)/opacityScale();
        ilm_layerSetOpacity(layerProps[layerIdx].id, layerProps[layerIdx].opacity);
        ilm_commitChanges();
    }
}

void Control::setLayerVisibility(int state)
{
    if(layerIdx != LS_UNDEF) {
        layerProps[layerIdx].visibility = (state == Qt::Checked);
        ilm_layerSetVisibility(layerProps[layerIdx].id, layerProps[layerIdx].visibility);
        ilm_commitChanges();
        t_ilm_bool Visibility;
        ilm_layerGetVisibility(layerProps[layerIdx].id, &Visibility);
    }
}

void Control::setLayerZOrder(int value)
{
    if(layerIdx != LS_UNDEF) {
        if(layerProps[layerIdx].zorder != value) {
            layerProps[layerIdx].zorder = value;
            QMap<int, t_ilm_layer> morder;
            for (int i = 0; i < layerProps.size(); ++i)
                morder.insertMulti(layerProps[i].zorder, layerProps[i].id);

            t_ilm_layer *layerz = new t_ilm_layer[layerProps.size()];
            for(int i = 0; i < layerProps.size(); i++)
                layerz[i] = morder.values()[i];

            ilm_displaySetRenderOrder(0, layerz, layerProps.size());
            ilm_commitChanges();

            delete [] layerz;
        }
    }
}


void Control::setSurfaceIndex(int index)
{
    surfaceIdx = index;
}

void Control::setSurfaceOpacity(int value)
{
    if(surfaceIdx != LS_UNDEF) {
        surfaceProps[surfaceIdx].opacity = static_cast<t_ilm_float>(value)/opacityScale();
        ilm_surfaceSetOpacity(surfaceProps[surfaceIdx].id, surfaceProps[surfaceIdx].opacity);
        ilm_commitChanges();
    }
}

void Control::setSurfaceVisibility(int state)
{
    if(surfaceIdx != LS_UNDEF) {
        surfaceProps[surfaceIdx].visibility = (state == Qt::Checked);
        ilm_surfaceSetVisibility(surfaceProps[surfaceIdx].id, surfaceProps[surfaceIdx].visibility);
        ilm_commitChanges();
    }
}

void Control::setSurfaceZOrder(int value)
{
    if(surfaceIdx != LS_UNDEF) {
        if(surfaceProps[surfaceIdx].zorder != value) {
            surfaceProps[surfaceIdx].zorder = value;
            QMap<int, t_ilm_surface> morder;
            for (int i = 0; i < surfaceProps.size(); ++i)
                morder.insertMulti(surfaceProps[i].zorder, surfaceProps[i].id);

            t_ilm_surface *surfacez = new t_ilm_surface[surfaceProps.size()];
            for(int i = 0; i < surfaceProps.size(); i++)
                surfacez[i] = morder.values()[i];

            ilm_layerSetRenderOrder(layerProps[layerIdx].id, surfacez, surfaceProps.size());
            ilm_commitChanges();

            delete [] surfacez;
        }
    }
}

void Control::moveSurface(QPointF pos)
{
    if(surfaceIdx != LS_UNDEF) {
        surfaceProps[surfaceIdx].position[0] = pos.x();
        surfaceProps[surfaceIdx].position[1] = pos.y();
        ilm_surfaceSetPosition(surfaceProps[surfaceIdx].id, surfaceProps[surfaceIdx].position);
        ilm_commitChanges();
    }
}
