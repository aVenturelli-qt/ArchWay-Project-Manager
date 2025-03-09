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
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "pathinfotabledelegate.h"


/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


// static variables init
const QColor PathInfoTableDelegate::VALID_COL   { 79, 119, 60 };
const QColor PathInfoTableDelegate::INVALID_COL { 134, 59, 60  };


PathInfoTableDelegate::PathInfoTableDelegate( QWidget *parent )
    : QStyledItemDelegate{ parent }
{}


void PathInfoTableDelegate::paint(
    QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index
    ) const
{

    // validity chip  - valid / invalid
    if ( index.column() == 2 && index.siblingAtColumn(2).data( Qt::EditRole ).canConvert<bool>() )
    {

        // value of third column
        bool pathIsValid = index.siblingAtColumn(2).data( Qt::EditRole ).toBool();      // with "pathIsValid" = 0  -> background red and text set to Invalid

        painter->save();
        painter->setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing );


        // equivalent to frame_rect - padding
        QRect chip_rect = option.rect.adjusted(
             PADDING_LEFT_RIGHT , PADDING_TOP_BOTTOM,
            -PADDING_LEFT_RIGHT, -PADDING_TOP_BOTTOM
            );

        // move the chip rect to center
        chip_rect.moveCenter( option.rect.center() );

        // draw chip with correct color
        auto chip_bg_col = ( pathIsValid ) ? VALID_COL : INVALID_COL;
        painter->setBrush( chip_bg_col );
        painter->setPen  ( chip_bg_col.darker() );

        int border_rad = chip_rect.height() / 2;
        painter->drawRoundedRect( chip_rect, border_rad, border_rad);

        // draw text centered
        painter->setPen(Qt::white);
        QString chip_text = pathIsValid ? "Valid" : "Invalid";

        QFont font{"Inter", 16, 650};
        painter->setFont( font );
        QFontMetrics fnt_mtrx( font );

        // Calculate text rectangle and center it
        QRect fnt_rect = fnt_mtrx.boundingRect( chip_rect, Qt::AlignCenter, chip_text );
        painter->drawText( fnt_rect, Qt::AlignCenter, chip_text );

        painter->restore();
        return;
    }


    // draw centered the FileType pixmap
    if ( index.column() == 0 && index.siblingAtColumn(0).data( Qt::EditRole ).canConvert<bool>() )
    {
        painter->save();

        QPixmap pix{ INVALID_PIX_PATH };
        pix = pix.scaledToHeight( option.rect.height() * 0.8 , Qt::SmoothTransformation );

        // if third column tells the path is valid
        // set the appropriate icon
        bool pathIsValid = index.siblingAtColumn(2).data( Qt::EditRole ).toBool();
        if (  pathIsValid )
        {

            auto isFile = index.siblingAtColumn(0).data( Qt::EditRole ).toBool();
            pix.load( isFile ? FILE_PIX_PATH : DIR_PIX_PATH );
            pix = pix.scaled( option.rect.height() * 0.8, option.rect.height(),
                             Qt::KeepAspectRatio, Qt::SmoothTransformation );
        }

        QRect targetRect( QPoint(0, 0), pix.size()  );        // Create rect of pixmap size
        targetRect.moveCenter( option.rect.center() );        // Move it to widget center
        painter->drawPixmap( targetRect.topLeft(), pix );
        painter->restore();
        return;
    }


}
