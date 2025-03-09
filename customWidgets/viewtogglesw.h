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

#ifndef VIEWTOGGLESW_H
#define VIEWTOGGLESW_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QCheckBox>
#include <QPen>
#include <QPainter>

/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/

class ViewToggleSw : public QCheckBox
{
    Q_OBJECT

    using PenBrushStyle = std::pair<QPen, QBrush>;

    int PEN_WIDTH { 3 };

    // unchecked state
    QBrush BG_COL_UNCHECK   { QColor(207, 207, 207) };
    QBrush BORDER_COL_UNCHECK { QColor(113, 113, 113) };

    //  checked state
    QBrush BG_COL_CHECK   { QColor(255, 255, 255) };
    QBrush BORDER_COL_CHECK { QColor(152, 73, 249) };

    // handle unchecked
    QBrush HANDLE_COL_UNCHECK  { QColor(132, 131, 151) };
    QBrush BORDER_HANDLE_COL     { QColor(81, 80, 102) };

    // handle checked
    QBrush HANDLE_COL_CHECK { QColor(161, 86, 253) };

public:
    explicit ViewToggleSw(QWidget *parent = nullptr);
    ~ViewToggleSw() override = default;

    // copy constructor
    ViewToggleSw( const ViewToggleSw& other);
    // copy assignment
    ViewToggleSw& operator= (const ViewToggleSw& other);

    QSize sizeHint()    const override;

protected:
    bool hitButton( const QPoint& pos ) const override;  // CheckBox method re-implementation
    void paintEvent( QPaintEvent* event) override;

private:
    void setPainterBrushPen(QPainter* p, QPen& pen, QBrush& brush) const;
};

#endif // VIEWTOGGLESW_H
