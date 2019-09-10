#include "graphicsitem.h"

#include <QGraphicsSceneEvent>
#include <qdebug.h>

using namespace LinkGraphicsItem;

GraphicsItem::GraphicsItem()
{

}

GraphicsItem::GraphicsItem(QGraphicsItem* parent){
    setParentItem(parent);
}

QPen GraphicsItem::Pen() const{
    return m_pen;
}

QColor GraphicsItem::Color() const{
    return m_pen.color();
}

qreal GraphicsItem::CornerPixel() const{
    return m_cornerPixel;
}

bool GraphicsItem::IsSelected() const{
    return m_isSeleted;
}

void GraphicsItem::SetPen(const QPen &pen){
    m_pen = pen;
    update();
}

void GraphicsItem::SetColor(const QColor &color){
    m_pen.setColor(color);
    update();
}

void GraphicsItem::SetCornerPixel(const qreal pixel){
    m_cornerPixel = pixel;
    update();
}

void GraphicsItem::SetSelected(bool b){
    m_isSeleted = b;
}

bool GraphicsItem::sceneEvent(QEvent *event){
    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress:
        mousePressEvent(static_cast<QGraphicsSceneMouseEvent*>(event));
        return true;
    case QEvent::GraphicsSceneMouseMove:
        mouseMoveEvent(static_cast<QGraphicsSceneMouseEvent*>(event));
        return true;
    case QEvent::GraphicsSceneMouseRelease:
        mouseReleaseEvent(static_cast<QGraphicsSceneMouseEvent*>(event));
        return true;
    case QEvent::GraphicsSceneHoverEnter:
        hoverEnterEvent(static_cast<QGraphicsSceneHoverEvent*>(event));
        return true;
    case QEvent::GraphicsSceneHoverMove:
        hoverMoveEvent(static_cast<QGraphicsSceneHoverEvent*>(event));
        return true;
    case QEvent::GraphicsSceneHoverLeave:
        hoverLeaveEvent(static_cast<QGraphicsSceneHoverEvent*>(event));
        return true;
    default:
        break;
    }

    return false;
}
