#ifndef GRAPHICSRECTITEM_H
#define GRAPHICSRECTITEM_H

// graphicsrectitem.h: 用于GraphicsView框架的矩形图元

#include <QWidget>
#include <QGraphicsItem>
#include <QPen>
#include <QEvent>

#include "graphicsitem.h"

namespace LinkGraphicsItem
{


class GraphicsRectItem : public GraphicsItem
{
    Q_OBJECT

public:
    GraphicsRectItem(QGraphicsItem* parent = nullptr);
    GraphicsRectItem(QPointF p, qreal w = 0, qreal h = 0, QColor color = Qt::black,QGraphicsItem* parent = nullptr);

    qreal Width() const;
    qreal Height() const;

    void SetWidth(qreal w);
    void SetHeight(qreal h);

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    /*
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    */

signals:
    // 被点击时传出自己的指针
    void clicked(QGraphicsItem*);

private:
    // 宽度
    qreal m_width;
    // 高度
    qreal m_height;
    // 鼠标按下时的宽度
    qreal m_mousePressWidth;
    // 鼠标按下时的高度
    qreal m_mousePressHeight;

    // 判断鼠标按下时的位置,参数p为item内坐标系
    Direction m_JudgeDirection(QPointF p);
};

}
#endif // GRAPHICSRECTITEM_H
