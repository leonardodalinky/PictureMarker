#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

// graphicsitem.h: 重新包装QGraphicsItem类，形成自定义的GraphicsView控件类的基类

#include <QWidget>
#include <QGraphicsItem>
#include <QPen>
#include <QEvent>

namespace LinkGraphicsItem
{

enum class Direction{
    LeftTop = 0,
    RightTop,
    LeftBottom,
    RightBottom,
    Left,
    Top,
    Right,
    Bottom,
    Inner,
    Outer
};

class GraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    GraphicsItem();
    GraphicsItem(QGraphicsItem* parent);
    QPen Pen() const;
    QColor Color() const;
    qreal CornerPixel() const;
    bool IsSelected() const;

    void SetPen(const QPen& pen);
    virtual void SetColor(const QColor& color);
    void SetCornerPixel(const qreal pixel);
    void SetSelected(bool b);

protected:
    // 画笔
    QPen m_pen;
    // 拖动框的大小
    qreal m_cornerPixel = 4;
    // 鼠标按下时的scene中坐标
    QPointF m_mousePressScenePos;
    // 鼠标按下时，整个item的scene坐标
    QPointF m_mousePressSelfScenePos;
    // 鼠标按下时的位置
    Direction m_mousePressDirection;
    bool m_isSeleted = false;
    bool m_isDragging = false;

    bool sceneEvent(QEvent *event);

signals:
    // 被鼠标move事件使得形状等改变
    void ItemMouseMoveChanged(GraphicsItem* item);
};

}
#endif // GRAPHICSITEM_H
