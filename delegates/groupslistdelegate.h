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

#ifndef GROUPSLISTDELEGATE_H
#define GROUPSLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QPainter>
#include <QPainterpath>
#include <QEvent>
#include <QMouseEvent>
#include <QListView>
#include <QConicalGradient>

#include "core/buttonStyle.h"

/*!----------------------------------------------------+
 *                                                     |
 *      @author: Andrea Venturelli                     |
 *      @mail  : aventurelli.github@gmail.com          |
 *                                                     |
 *-----------------------------------------------------+
*/

class GroupsListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    using btnState = ButtonStyle::ButtonState;

    explicit GroupsListDelegate(QWidget *parent = nullptr);

    // Setter/getter for ButtonStyle
    inline void setTopButtonsStyle   (ButtonStyle btnStyle) { m_top_btn_style = btnStyle; }
    inline void setBottomButtonsStyle(ButtonStyle btnStyle) { m_bottom_btn_style = btnStyle; }

    inline void setButtonsStyle(ButtonStyle top_btn, ButtonStyle bottom_btn) {
        m_top_btn_style    = top_btn;
        m_bottom_btn_style = bottom_btn;
    }

    inline std::pair<ButtonStyle, ButtonStyle> buttonsStyle() {
        return std::make_pair( m_top_btn_style, m_bottom_btn_style );
    }

    void paint( QPainter* p, const QStyleOptionViewItem& option, const QModelIndex& index ) const override;
    QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const override;

signals:
    void editButtonClickedGroup   ( const QString group );
    void openAllButtonClickedGroup( const QString group );

    void editButtonClicked   ( const QModelIndex& modelIndex );
    void openAllButtonClicked( const QModelIndex& modelIndex );

protected:
    bool editorEvent(
        QEvent* event,
        QAbstractItemModel* model,
        const QStyleOptionViewItem& option,
        const QModelIndex& index
        ) override;

private:
    QListView* m_view{};
    ButtonStyle m_top_btn_style{};
    ButtonStyle m_bottom_btn_style{};

    /* HELPER METHODS */
    QRect _rectFromFontMetrics(const QFont& font, const QString& text, QPainter* painter = nullptr) const;

    void _drawButton(QRect button_rect, QString text, QPainter* p,  btnState state, bool isTopBtn = true) const;

    QRect _offsetXYRect(int x_offset, int y_offset, const QRect& originalRect) const;

    bool _isMouseEvent(QEvent* event) const;

    bool _updateStateButton(QPoint mousePos, QRect btnRect, btnState currState, btnState newState, const QModelIndex& index, bool isTopButton = true );

    /* static drawing const */
    static constexpr int PADDING{16};
    static constexpr int TEXT_SPACING{12};
    static constexpr int BUTTONS_SPACING{8};
    static constexpr int BUTTON1_HEIGHT{32};
    static constexpr int BUTTON2_HEIGHT{40};
    static constexpr int BUTTONS_WIDTH{100};
    static constexpr int BG_BORDER_RAD{8};

    static inline QFont GROUP_TEXT_FONT{"Arial", 24, 650};
    static inline QFont SUB_TEXT_FONT{"Arial", 12, 500};
    static inline QFont BTN_FONT{"Arial", 12, 500};
    static inline QColor BG_COLOR{255, 255, 255};
    static inline QColor BG_COLOR_SELECTED{80, 160, 180};

    static QConicalGradient _createConicalGradient(QPoint center) {
        QConicalGradient con_grad(center, 0);
        con_grad.setColorAt(0.1, QColor(250, 102, 192));
        con_grad.setColorAt(0.3, QColor(251, 173, 56));
        con_grad.setColorAt(0.55, QColor(218, 152, 251));
        con_grad.setColorAt(0.88, QColor(91, 183, 244));
        con_grad.setColorAt(0.98, QColor(250, 102, 192));
        return con_grad;
    }
};

#endif // GROUPSLISTDELEGATE_H
