#include "graphicsrectitem.h"
#include <QPainter>
#include <QBrush>

#include <QGraphicsSceneEvent>

using namespace LinkGraphicsItem;

GraphicsRectItem::GraphicsRectItem(QGraphicsItem* parent)
    :GraphicsItem (parent), m_width(0), m_height(0)
{
    setPos(0, 0);
}

GraphicsRectItem::GraphicsRectItem(QPointF p, qreal w, qreal h, QColor color, QGraphicsItem* parent)
    : GraphicsItem(parent), m_width(w), m_height(h)
{
    //setAcceptedMouseButtons(Qt::LeftButton);
    //setAcceptHoverEvents(true);
    setPos(p);
    SetColor(color);
}


qreal GraphicsRectItem::Width() const{
    return m_width;
}

qreal GraphicsRectItem::Height() const{
    return m_height;
}

void GraphicsRectItem::SetWidth(qreal w){
    m_width = w;
    update();
}

void GraphicsRectItem::SetHeight(qreal h){
    m_height = h;
    update();
}


QRectF GraphicsRectItem::boundingRect() const{
    return QRectF(-m_cornerPixel, -m_cornerPixel,
                  m_width + 2 * m_cornerPixel, m_height + 2 * m_cornerPixel);
}

void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QRectF rect = boundingRect();

    painter->setPen(m_pen);
    painter->drawRect(0, 0, m_width, m_height);

    if (!m_isSeleted){
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(-m_cornerPixel, -m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
        painter->drawRect(m_width - m_cornerPixel, -m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
        painter->drawRect(-m_cornerPixel, m_height - m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
        painter->drawRect(m_width - m_cornerPixel, m_height - m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
    }
    else{
        QBrush brush(Color());
        painter->setBrush(brush);
        painter->drawRect(-m_cornerPixel, -m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
        painter->drawRect(m_width - m_cornerPixel, -m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
        painter->drawRect(-m_cornerPixel, m_height - m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
        painter->drawRect(m_width - m_cornerPixel, m_height - m_cornerPixel, 2 * m_cornerPixel, 2 * m_cornerPixel);
    }
}

void GraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
    m_isSeleted = true;

    m_mousePressSelfScenePos = scenePos();
    m_mousePressScenePos = event->scenePos();
    m_mousePressDirection = m_JudgeDirection(event->pos());
    m_mousePressWidth = m_width;
    m_mousePressHeight = m_height;

    update();
    // 下面这行必须去掉，不然release只有双击之后才有
    // QGraphicsItem::mousePressEvent(event);
    emit clicked(static_cast<QGraphicsItem*>(this));
}

void GraphicsRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if (m_isSeleted){
        m_isDragging = false;
    }
    else{
        m_isSeleted = true;
    }
    update();
    QGraphicsItem::mouseReleaseEvent(event);

}

void GraphicsRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
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
        case Direction::LeftTop:
            m_width = m_mousePressWidth - delPosx;
            m_height = m_mousePressHeight - delPosy;
            setPos(m_mousePressSelfScenePos.x() + delPosx, m_mousePressSelfScenePos.y() + delPosy);
            break;
        case Direction::RightTop:
            m_width = m_mousePressWidth + delPosx;
            m_height = m_mousePressHeight - delPosy;
            setPos(m_mousePressSelfScenePos.x(), m_mousePressSelfScenePos.y() + delPosy);
            break;
        case Direction::LeftBottom:
            m_width = m_mousePressWidth - delPosx;
            m_height = m_mousePressHeight + delPosy;
            setPos(m_mousePressSelfScenePos.x() + delPosx, m_mousePressSelfScenePos.y());
            break;
        case Direction::RightBottom:
            m_width = m_mousePressWidth + delPosx;
            m_height = m_mousePressHeight + delPosy;
            break;
        default:
            break;
        }

        prepareGeometryChange();
    }
    update();
    QGraphicsItem::mouseMoveEvent(event);
    emit ItemMouseMoveChanged(this);
}
/*
void GraphicsRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    //m_isSeleted = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void GraphicsRectItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event){

}

void GraphicsRectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    //m_isSeleted = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}
*/


Direction GraphicsRectItem::m_JudgeDirection(QPointF p){
    qreal px = p.x(), py = p.y();
    if (px >= -m_cornerPixel && px <= m_cornerPixel
            && py >= -m_cornerPixel && py <= m_cornerPixel)
        return Direction::LeftTop;
    else if (px >= m_width - m_cornerPixel && px <= m_width + m_cornerPixel
             && py >= -m_cornerPixel && py <= m_cornerPixel)
        return Direction::RightTop;
    else if (px >= -m_cornerPixel && px <= m_cornerPixel
             && py >= m_height - m_cornerPixel && py <= m_height + m_cornerPixel)
        return Direction::LeftBottom;
    else if (px >= m_width - m_cornerPixel && px <= m_width + m_cornerPixel
             && py >= m_height - m_cornerPixel && py <= m_height + m_cornerPixel)
        return Direction::RightBottom;
    else if (px >= -m_cornerPixel && px <= m_width + m_cornerPixel
             && py >= -m_cornerPixel && py <= m_height + m_cornerPixel)
        return Direction::Inner;
    else
        return Direction::Outer;
}
