#include "graphicspdrawingpaditem.h"
#include <QtMath>
#include <QPainter>
#include <QBrush>
#include <QDebug>

#include <QGraphicsSceneEvent>

using namespace LinkGraphicsItem;

GraphicspDrawingPadItem::GraphicspDrawingPadItem()
{

}

GraphicspDrawingPadItem::GraphicspDrawingPadItem(qreal w, qreal h, const DrawingPadPointList& pad, QGraphicsItem* parent)
    : GraphicsItem(parent), m_width(w), m_height(h)
{
    m_Pad.clear();
    m_Pad.append(pad);
    setPos(0,0);
}

DrawingPadStatus GraphicspDrawingPadItem::Status() const{
    return m_status;
}

DrawingPadPointList GraphicspDrawingPadItem::Pad() const{
    return m_Pad;
}

void GraphicspDrawingPadItem::SetStatus(DrawingPadStatus s){
    m_status = s;
}

void GraphicspDrawingPadItem::AddPoint(const DrawingPadPoint& point){
    m_Pad.push_back(point);
}

void GraphicspDrawingPadItem::AddPoint(const QPointF center, qreal edge){
    m_Pad.push_back(DrawingPadPoint(center,edge));
}

void GraphicspDrawingPadItem::DeleteArea(const DrawingPadPoint& point){
    auto it = m_Pad.begin();
    while(it != m_Pad.end()){
        // 判断是否在删除区域
        if (m_Distance((*it).center, point.center) <= (*it).edge + point.edge){
            // 被选中
            it = m_Pad.erase(it);
        }
        else{
            // 未选中
            it++;
        }
    }
}

void GraphicspDrawingPadItem::DeleteArea(const QPointF center, qreal edge){
    DeleteArea(DrawingPadPoint(center,edge));
}

QRectF GraphicspDrawingPadItem::boundingRect() const{
    return QRectF(0, 0, m_width,m_height);
}

void GraphicspDrawingPadItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setBrush(QBrush(Color()));
    painter->setPen(Pen());
    qreal tempx, tempy, tempe;

    for (auto it = m_Pad.begin();it != m_Pad.end();it++){
        tempx = (*it).center.x(); tempy = (*it).center.y();
        tempe = (*it).edge;
        painter->drawRect(QRectF(tempx - tempe / 2, tempy - tempe / 2, tempe, tempe));
    }
}

void GraphicspDrawingPadItem::mousePressEvent(QGraphicsSceneMouseEvent *event){

}

void GraphicspDrawingPadItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){

}

void GraphicspDrawingPadItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if (m_status == DrawingPadStatus::Drawing){
        AddPoint(QPointF((event->pos()).x(), (event->pos()).y()), m_brushPixel);
    }
    else if (m_status == DrawingPadStatus::Erasing){
        DeleteArea(QPointF((event->pos()).x(), (event->pos()).y()), m_brushPixel);
    }
    update();
}

inline qreal GraphicspDrawingPadItem::m_Distance(QPointF p1, QPointF p2){
    return qAbs(p1.x() - p2.x()) + qAbs(p1.y() - p2.y());
}
