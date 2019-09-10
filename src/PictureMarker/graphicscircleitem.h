#ifndef GRAPHICSCIRCLEITEM_H
#define GRAPHICSCIRCLEITEM_H

// graphicscircleitem.h: 用于GraphicsView框架的圆形图元

#include <QWidget>
#include <QGraphicsItem>
#include <QPen>
#include <QEvent>

#include "graphicsitem.h"

namespace LinkGraphicsItem
{

class GraphicsCircleItem : public GraphicsItem
{
    Q_OBJECT

public:
    GraphicsCircleItem(QGraphicsItem* parent = nullptr);
    GraphicsCircleItem(QPointF p, qreal r = 0, QGraphicsItem* parent = nullptr);
    qreal Radius() const;

    void SetRadius(const qreal r);

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
    // 被点击时传出自己的指针
    void clicked(QGraphicsItem*);

private:
    // 半径
    qreal m_radius;
    // 鼠标按下时的半径
    qreal m_mousePressRadius;
    // 判断鼠标按下时的位置,参数p为item内坐标系
    Direction m_JudgeDirection(QPointF p);
};

}

#endif // GRAPHICSCIRCLEITEM_H
