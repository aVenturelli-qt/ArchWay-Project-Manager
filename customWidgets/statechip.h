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


#ifndef STATECHIP_H
#define STATECHIP_H

#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QWidget>

class StateChip : public QWidget
{
    Q_OBJECT

    bool m_isValid{ false };

    short PADDING_LEFT_RIGHT = 8;
    short PADDING_TOP_BOTTOM = 4;

    QBrush VALID_COL{ QColor(41, 136, 52) };
    QBrush INVALID_COL{ QColor(165, 62, 62) };

public:
    explicit StateChip(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    void setValidityState( bool isValid );

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void stateChanged(bool state);
};

#endif // STATECHIP_H
