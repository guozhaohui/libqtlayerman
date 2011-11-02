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

#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QGraphicsView>
#include "window.h"
#include "control.h"


RectItem::RectItem(Window *win, QRectF rect) : QGraphicsRectItem(rect), window(win)
{}

void RectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressPos = event->pos();
}

void RectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->scenePos()-pressPos;
    setPos(pos);
    window->moveSurface(pos);
}

Window::Window()
{
    setWindowTitle(tr("LayerManager Controler"));

    control = new Control;

    QLabel *layerIdLabel = new QLabel("ID:");
    layerIdsComboBox = new QComboBox();
    layerIdsComboBox->addItems(control->layerIdsList());
    layerIdsComboBox->setCurrentIndex(control->layerIndex());
    QVBoxLayout *layerIdLayout = new QVBoxLayout();
    layerIdLayout->addWidget(layerIdLabel);
    layerIdLayout->addWidget(layerIdsComboBox);

    layerVisibilityComboBox = new QCheckBox("Visible");
    layerVisibilityComboBox->setChecked(control->layerVisibility());

    QLabel *layerOpacityLabel = new QLabel("Opacity:");
    layerOpacitySlider = new QSlider(Qt::Horizontal);
    layerOpacitySlider->setMinimum(0);
    layerOpacitySlider->setMaximum(control->opacityScale());
    layerOpacitySlider->setValue(control->layerOpacity());
    QVBoxLayout *layerOpacityLayout = new QVBoxLayout();
    layerOpacityLayout->addWidget(layerOpacityLabel);
    layerOpacityLayout->addWidget(layerOpacitySlider);

    QLabel *layerZLabel = new QLabel("Z-Order:");
    layerZOrderSpinBox = new QSpinBox();
    layerZOrderSpinBox->setMinimum(0);
    layerZOrderSpinBox->setMaximum(10);
    layerZOrderSpinBox->setValue(control->layerZOrder());
    QVBoxLayout *layerZLayout = new QVBoxLayout();
    layerZLayout->addWidget(layerZLabel);
    layerZLayout->addWidget(layerZOrderSpinBox);

    QVBoxLayout *layerLayout = new QVBoxLayout();
    layerLayout->addLayout(layerIdLayout);
    layerLayout->addWidget(layerVisibilityComboBox);
    layerLayout->addLayout(layerOpacityLayout);
    layerLayout->addLayout(layerZLayout);
    layerLayout->addStretch(1);

    QGroupBox *layerGroupBox = new QGroupBox("Layer");
    layerGroupBox->setLayout(layerLayout);

    connect(layerIdsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLayerIndex(int)));
    connect(layerOpacitySlider, SIGNAL(valueChanged(int)), control, SLOT(setLayerOpacity(int)));
    connect(layerVisibilityComboBox, SIGNAL(stateChanged (int)), control, SLOT(setLayerVisibility(int)));
    connect(layerZOrderSpinBox, SIGNAL(valueChanged(int)), control, SLOT(setLayerZOrder(int)));


    QLabel *surfaceIdLabel = new QLabel("ID:");
    surfaceIdsComboBox = new QComboBox();
    surfaceIdsComboBox->addItems(control->surfaceIdsList());
    surfaceIdsComboBox->setCurrentIndex(control->surfaceIndex());
    QVBoxLayout *surfaceIdLayout = new QVBoxLayout();
    surfaceIdLayout->addWidget(surfaceIdLabel);
    surfaceIdLayout->addWidget(surfaceIdsComboBox);

    surfaceVisibilityComboBox = new QCheckBox("Visible");
    surfaceVisibilityComboBox->setChecked(control->surfaceVisibility());

    QLabel *surfaceOpacityLabel = new QLabel("Opacity:");
    surfaceOpacitySlider = new QSlider(Qt::Horizontal);
    surfaceOpacitySlider->setMinimum(0);
    surfaceOpacitySlider->setMaximum(control->opacityScale());
    surfaceOpacitySlider->setValue(control->surfaceOpacity());
    QVBoxLayout *surfaceOpacityLayout = new QVBoxLayout();
    surfaceOpacityLayout->addWidget(surfaceOpacityLabel);
    surfaceOpacityLayout->addWidget(surfaceOpacitySlider);

    QLabel *surfaceZLabel = new QLabel("Z-Order:");
    surfaceZOrderSpinBox = new QSpinBox();
    surfaceZOrderSpinBox->setMinimum(0);
    surfaceZOrderSpinBox->setMaximum(10);
    surfaceZOrderSpinBox->setValue(control->surfaceZOrder());
    QVBoxLayout *surfaceZLayout = new QVBoxLayout();
    surfaceZLayout->addWidget(surfaceZLabel);
    surfaceZLayout->addWidget(surfaceZOrderSpinBox);

    QVBoxLayout *surfaceLayout = new QVBoxLayout();
    surfaceLayout->addLayout(surfaceIdLayout);
    surfaceLayout->addWidget(surfaceVisibilityComboBox);
    surfaceLayout->addLayout(surfaceOpacityLayout);
    surfaceLayout->addLayout(surfaceZLayout);
    surfaceLayout->addStretch(1);

    QGroupBox *surfaceGroupBox = new QGroupBox("Surface");
    surfaceGroupBox->setLayout(surfaceLayout);

    connect(surfaceIdsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSurfaceIndex(int)));
    connect(surfaceOpacitySlider, SIGNAL(valueChanged(int)), control, SLOT(setSurfaceOpacity(int)));
    connect(surfaceVisibilityComboBox, SIGNAL(stateChanged (int)), control, SLOT(setSurfaceVisibility(int)));
    connect(surfaceZOrderSpinBox, SIGNAL(valueChanged(int)), control, SLOT(setSurfaceZOrder(int)));

    unsigned int width, height;
    control->screenResolution(&width, &height);
    qreal rateX = 800.0/width;
    qreal rateY = 480.0/height;
    qreal scale = qMin(rateX, rateY);
    width = static_cast<qreal>(width) * scale;
    height = static_cast<qreal>(height) * scale;
    surfaceRectScale = scale;

    QGraphicsScene *gscene = new QGraphicsScene;
    gscene->setSceneRect(0, 0, width, height);
    gscene->setItemIndexMethod(QGraphicsScene::NoIndex);

    QRectF surfaceRect(QPointF(0.0,0.0), control->surfaceDim()*surfaceRectScale);
    surfaceRectItem = new RectItem(this, surfaceRect);
    surfaceRectItem->setBrush(QColor(QLatin1String("#D0E2FA")));
    gscene->addItem(surfaceRectItem);
    surfaceRectItem->setPos(control->surfacePos()*surfaceRectScale);
    QGraphicsView *gview = new QGraphicsView;
    gview->setFixedSize(width, height);
    gview->setScene(gscene);
    gview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(layerGroupBox);
    mainLayout->addWidget(surfaceGroupBox);
    mainLayout->addWidget(gview);
}

Window::~Window()
{
    delete control;
}


void Window::setLayerIndex(int index)
{
    control->setLayerIndex(index);

    layerVisibilityComboBox->setChecked(control->layerVisibility());
    layerOpacitySlider->setValue(control->layerOpacity());
    layerZOrderSpinBox->setValue(control->layerZOrder());

    disconnect(surfaceIdsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSurfaceIndex(int)));
    surfaceIdsComboBox->clear();
    surfaceIdsComboBox->addItems(control->surfaceIdsList());
    connect(surfaceIdsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSurfaceIndex(int)));

    control->surfaceIdsList().isEmpty() ? setSurfaceIndex(LS_UNDEF) : setSurfaceIndex(0);
}


void Window::setSurfaceIndex(int index)
{
    control->setSurfaceIndex(index);

    surfaceVisibilityComboBox->setChecked(control->surfaceVisibility());
    surfaceOpacitySlider->setValue(control->surfaceOpacity());
    surfaceZOrderSpinBox->setValue(control->surfaceZOrder());
    QRectF surfaceRect(QPointF(0.0,0.0), control->surfaceDim()*surfaceRectScale);
    surfaceRectItem->setRect(surfaceRect);
    surfaceRectItem->setPos(control->surfacePos()*surfaceRectScale);
}

void Window::moveSurface(QPointF pos)
{
    control->moveSurface(pos/surfaceRectScale);
}
