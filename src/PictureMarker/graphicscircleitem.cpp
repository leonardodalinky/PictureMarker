#include "graphicscircleitem.h"
#include <QPainter>
#include <QBrush>

#include <QGraphicsSceneEvent>

using namespace LinkGraphicsItem;

#define SQUARE(x) ((x) * (x))

GraphicsCircleItem::GraphicsCircleItem(QGraphicsItem* parent)
    :GraphicsItem (parent), m_radius(0)
{
    setPos(0, 0);
}

GraphicsCircleItem::GraphicsCircleItem(QPointF p, qreal r, QGraphicsItem* parent)
    : GraphicsItem(parent), m_radius(r)
{
    setPos(p);
}

qreal GraphicsCircleItem::Radius() const{
    return m_radius;
}

void GraphicsCircleItem::SetRadius(const qreal r){
    m_radius = r;
}


QRectF GraphicsCircleItem::boundingRect() const{
    return QRectF(-m_radius - m_cornerPixel, -m_radius - m_cornerPixel,
                  2 * (m_radius + m_cornerPixel), 2 * (m_radius + m_cornerPixel));
}

void GraphicsCircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QRectF rect(-m_radius, -m_radius, 2 * m_radius, 2 * m_radius);

    if (!m_isSeleted){
        m_pen.setWidth(1);
        painter->setPen(m_pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(rect);

        painter->drawRect(-m_cornerPixel, -m_radius - m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
        painter->drawRect(-m_cornerPixel, m_radius - m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
        painter->drawRect(-m_radius - m_cornerPixel, -m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
        painter->drawRect(m_radius - m_cornerPixel, -m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
    }
    else{
        m_pen.setWidth(3);
        painter->setPen(m_pen);
        painter->drawEllipse(rect);

        m_pen.setWidth(2);
        QBrush brush(Color());
        painter->setPen(m_pen);
        painter->setBrush(brush);
        painter->drawRect(-m_cornerPixel, -m_radius - m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
        painter->drawRect(-m_cornerPixel, m_radius - m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
        painter->drawRect(-m_radius - m_cornerPixel, -m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
        painter->drawRect(m_radius - m_cornerPixel, -m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
    }
}

void GraphicsCircleItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
    m_isSeleted = true;

    m_mousePressSelfScenePos = scenePos();
    m_mousePressScenePos = event->scenePos();
    m_mousePressDirection = m_JudgeDirection(event->pos());
    m_mousePressRadius = Radius();

    update();
    emit clicked(static_cast<QGraphicsItem*>(this));
}

void GraphicsCircleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if (m_isSeleted){
        m_isDragging = false;
    }
    else{
        m_isSeleted = true;
    }
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void GraphicsCircleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if (m_isSeleted)
        m_isDragging = true;
    if (m_isDragging){
        qreal nowPosx = event->scenePos().x();
        qreal nowPosy = event->scenePos().y();
        qreal delPosx = nowPosx - m_mousePressScenePos.x();
        qreal delPosy = nowPosy - m_mousePressScenePos.y();
        switch (m_mousePressDirection) {
        case Direction::Inner:
            setPos(m_mousePressSelfScenePos.x() + delPosx, m_mousePressSelfScenePos.y() + delPosy);
            break;
        case Direction::Left:
            m_radius = m_mousePressRadius - delPosx;
            break;
        case Direction::Top:
            m_radius = m_mousePressRadius - delPosy;
            break;
        case Direction::Right:
            m_radius = m_mousePressRadius + delPosx;
            break;
        case Direction::Bottom:
            m_radius = m_mousePressRadius + delPosy;
            break;
        default:
            break;
        }
    }

    prepareGeometryChange();
    update();
    QGraphicsItem::mouseMoveEvent(event);
    //emit ItemMouseMoveChanged(this);
}

Direction GraphicsCircleItem::m_JudgeDirection(QPointF p){
    qreal px = p.x(), py = p.y();
    if (px >= -m_radius - m_cornerPixel && px <= -m_radius + m_cornerPixel
            && py >= -m_cornerPixel && py <= m_cornerPixel)
        return Direction::Left;
    else if (px >= -m_cornerPixel && px <= m_cornerPixel
             && py >= -m_radius - m_cornerPixel && py <= -m_radius + m_cornerPixel)
        return Direction::Top;
    else if (px >= m_radius - m_cornerPixel && px <= m_radius + m_cornerPixel
            && py >= -m_cornerPixel && py <= m_cornerPixel)
        return Direction::Right;
    else if (px >= -m_cornerPixel && px <= m_cornerPixel
             && py >= m_radius - m_cornerPixel && py <= m_radius + m_cornerPixel)
        return Direction::Bottom;
    else if ((SQUARE(px) + SQUARE(py) <= SQUARE(m_radius)))
        return Direction::Inner;
    else
        return Direction::Outer;
}
