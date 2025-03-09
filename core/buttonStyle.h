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

#ifndef BUTTONSTYLE_H
#define BUTTONSTYLE_H

#include <QColor>
#include <QPen>
#include <QDebug>

/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


struct ButtonStyle
{
    using FillStrokePair = std::pair<QColor, QColor>;

    enum class ButtonState {
        Default,
        Hover,
        Pressed,
        Disabled
    };

    FillStrokePair FS_disabled{ std::make_pair( Qt::gray, Qt::darkGray) };
    FillStrokePair FS_default { std::make_pair( Qt::white, Qt::black  ) };
    FillStrokePair FS_hover   { std::make_pair( QColor(220, 200, 241), QColor(114, 24, 183) ) };
    FillStrokePair FS_pressed { std::make_pair( QColor(203, 190, 240), Qt::black ) };

    QPen pen_disabled{ QPen( FS_disabled.second, 2.0, Qt::SolidLine ) };
    QPen pen_default { QPen( FS_default.second , 2.0, Qt::SolidLine ) };
    QPen pen_hover   { QPen( FS_hover.second   , 2.0, Qt::SolidLine ) };
    QPen pen_pressed { QPen( FS_pressed.second , 2.0, Qt::SolidLine ) };


    void setFillStrokeColor( QColor fill, QColor stroke, ButtonState target = ButtonState::Default )
    {
        FillStrokePair temp_pair = std::make_pair( fill, stroke );

        switch ( target )
        {
            case ButtonState::Disabled:
                FS_disabled = temp_pair;
                pen_disabled.setBrush( stroke );
                return;

            case ButtonState::Default:
                FS_default = temp_pair;
                pen_default.setBrush( stroke );
                return;

            case ButtonState::Hover:
                FS_hover = temp_pair;
                pen_hover.setBrush( stroke );
                return;

            case ButtonState::Pressed:
                FS_pressed = temp_pair;
                pen_pressed.setBrush( stroke );
                return;

            default:
                qDebug() << "State doesn't not exist..";
        }
    }

    void setPenByState( QPen pen, ButtonState target = ButtonState::Default )
    {
        switch ( target )
        {
            case ButtonState::Default:  pen_default  = pen; return;
            case ButtonState::Hover:    pen_hover    = pen; return;
            case ButtonState::Pressed:  pen_pressed  = pen; return;
            case ButtonState::Disabled: pen_disabled = pen; return;
            default:
                qDebug() << "State doesn't not exist..";
        }
    }

    std::pair<QColor, QPen> fillAndPenStroke( ButtonState state= ButtonState::Default ) const
    {
        switch ( state )
        {
            case ButtonState::Default:  return std::make_pair( FS_default.first, pen_default );
            case ButtonState::Hover:    return std::make_pair( FS_hover.first, pen_hover );
            case ButtonState::Pressed:  return std::make_pair( FS_pressed.first, pen_pressed );
            case ButtonState::Disabled: return std::make_pair( FS_disabled.first, pen_disabled );
            default:
            {
                qDebug() << "State doesn't not exist..";
                return std::make_pair( QColor(), QPen() );
            }
        }
    }
};



#endif // BUTTONSTYLE_H
