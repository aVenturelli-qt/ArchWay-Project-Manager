/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtSql module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include "pathtypeselectordialog.h"
#include "ui_pathtypeselectordialog.h"


/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


PathTypeSelectorDialog::PathTypeSelectorDialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PathTypeSelectorDialog)
{
    ui->setupUi(this);
    this->setAttribute( Qt::WA_TranslucentBackground, true );
    this->setWindowFlags( Qt::FramelessWindowHint );

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(100);  // Softness of the shadow
    shadowEffect->setXOffset(0);      // Horizontal shadow offset
    shadowEffect->setYOffset(0);      // Vertical shadow offset
    shadowEffect->setColor(QColor(0, 20, 130, 120)); // Black shadow with transparency

    this->setGraphicsEffect(shadowEffect);

    QString left ( "Add File");
    QString right( "Add Dir" );

    tgg_chip = new ToggleChip( left, right );

    auto internal_layout = static_cast<QVBoxLayout*>( ui->container_wdg->layout() );
    internal_layout->insertWidget(0, tgg_chip );
}

PathTypeSelectorDialog::~PathTypeSelectorDialog()
{
    delete ui;
}


void PathTypeSelectorDialog::paintEvent( QPaintEvent* event )
{
    QWidget::paintEvent( event );

    QPainter p( this );
    p.setRenderHint( QPainter::Antialiasing );
    p.fillRect( rect(), Qt::transparent );

    QPainterPath polygon = pointyArrow( rect() );
    p.fillPath( polygon, Qt::white );
    p.drawPath( polygon );

    p.end();
}


QPainterPath PathTypeSelectorDialog::pointyArrow(const QRectF& rect) {
    QPainterPath path;

    qreal boxWidth   = rect.width();
    qreal boxHeight  = rect.height() * 0.9;   // 90% of the total height

    qreal triangleWidth  = 60;
    qreal triangleHeight = rect.height() * 0.1;
    qreal triangleOffset = boxWidth * 0.2;          // left triangle's corner start at 20% of the width

    // draw the box rectangle
    path.addRoundedRect(
        rect.left(), rect.top() + triangleHeight,
        boxWidth, boxHeight, BOX_ROUNDED_RAD, BOX_ROUNDED_RAD);

    // triangle points
    QPointF bottLeft ( triangleOffset, triangleHeight );
    QPointF topCenter( triangleOffset + ( triangleWidth / 2 ), 0 );
    QPointF bottRight( triangleOffset + triangleWidth, triangleHeight );

    path.moveTo( bottLeft  );
    path.lineTo( topCenter );
    path.lineTo( bottRight );
    path.closeSubpath();

    return path;
}


void PathTypeSelectorDialog::on_selectPath_btn_clicked()
{
    PathType tempType;

    if ( tgg_chip->indicatorRightChecked() )
    {
        emit selectDirPathRequested();
        tempType = PathType::Dir;
    } else {
        emit selectFilePathRequested();
        tempType = PathType::File;
    }

    emit pathTypeRequested( tempType );

    this->hide();
}

