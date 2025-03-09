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

#ifndef TOGGLECHIP_H
#define TOGGLECHIP_H

#include <QWidget>
#include <QCheckBox>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QFontMetrics>
#include <QPainterPath>
#include <QTextOption>


/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/


class ToggleChip : public QCheckBox
{
    Q_OBJECT
public:
    /*!
     * this is a toggleChip that can have 2 state:
     * 0 means left_option_selected
     * 1 means right_option_selected
     *
     * isChecked is inherited from QCheckBox
    */

    explicit ToggleChip(QWidget *parent = nullptr);
    explicit ToggleChip(QString left_option, QString right_option, QWidget *parent = nullptr);
    ~ToggleChip();

    using DoubleStrPair = std::pair<const QString&, const QString&>;

    // getter
    inline const QString& leftText()  const { return m_text_left; }
    inline const QString& rightText() const { return m_text_right; }
    inline const QFont& currentFont() const { return m_font; }
    inline double heightFactor()      const { return HEIGHT_FACTOR; }
    DoubleStrPair leftAndRightText()  const;

    // setter
    inline void setLeftText ( QString str )   { m_text_left  = str; emit leftTextChanged ( m_text_left ); }
    inline void setRightText( QString str )   { m_text_right = str; emit rightTextChanged( m_text_right ); }
    inline void setTextFont ( QFont& font )   { m_font = font; updateGeometry(); update(); }
    inline void setHeightFactor( double fac ) { HEIGHT_FACTOR = fac; }
    void setLeftRightText( QString left, QString right );

    /*!
     *  if isChecked() is 1,
     *  means the right option is selected, so return true
    */
    inline bool indicatorRightChecked() const { return this->isChecked(); }
    QSize sizeHint() const override;

    enum IndicatorPosition {
        Left = 0,
        Right = 1
    };

signals:
    void leftTextChanged ( const QString& str );
    void rightTextChanged( const QString& str );
    void leftAndRightTextChanged( const QString& left, const QString& right );
    void selectionChanged( IndicatorPosition new_position );

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override ;
    void leaveEvent(QEvent* event) override ;

private:
    QString m_text_left { "option 1"};
    QString m_text_right{ "option 2"};
    QFont   m_font{ QFont( "Arial", 14, 600 ) };
    double  HEIGHT_FACTOR{ 0.40 }; // 0.60

    /*   drawing color definition       */
    QBrush FRAME_BG   { QColor( 68, 68, 68) };
    QBrush SLIDER_COL { QColor( 217, 217, 217) };
    QColor TEXT_COL_SELECTED { QColor( 68, 68, 68) };
    QColor TEXT_COL_DEFAULT { QColor( 186, 186, 186) };

};

#endif // TOGGLECHIP_H
