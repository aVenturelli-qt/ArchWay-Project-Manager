#ifndef VIEWTOGGLESW_H
#define VIEWTOGGLESW_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QCheckBox>
#include <QPen>
#include <QPainter>

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
