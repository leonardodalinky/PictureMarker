#include "graphicspdrawingpaditem.h"
#include <QtMath>
#include <QPainter>
#include <QBrush>
#include <QPixmap>
#include <QDebug>

#include <QGraphicsSceneEvent>

using namespace LinkGraphicsItem;

GraphicspDrawingPadItem::GraphicspDrawingPadItem(QGraphicsItem* parent)
    :GraphicsItem (parent)
{
    setPos(0, 0);
}

GraphicspDrawingPadItem::GraphicspDrawingPadItem(qreal w, qreal h, QGraphicsItem* parent)
    : GraphicsItem(parent), m_width(w), m_height(h)
{
    m_Pad.Clear();
    m_Pad.SetSize(w, h);
    setPos(0,0);
}

qreal GraphicspDrawingPadItem::Width() const
{
    return m_width;
}

qreal GraphicspDrawingPadItem::Height() const
{
    return m_height;
}

DrawingPadStatus GraphicspDrawingPadItem::Status() const{
    return m_status;
}

bool GraphicspDrawingPadItem::Drawable() const
{
    return m_isDrawable;
}

void GraphicspDrawingPadItem::SetPad(const PixelPointPad &pad)
{
    m_Pad = pad;
}

void GraphicspDrawingPadItem::SetColor(const QColor &color)
{
    m_Pad.SetColor(color);
    GraphicsItem::SetColor(color);
}

void GraphicspDrawingPadItem::SetSize(qreal w, qreal h)
{
    m_width = w;
    m_height = h;
    m_Pad.SetSize(w, h);
}

void GraphicspDrawingPadItem::SetDrawable(bool b)
{
    m_isDrawable = b;
}

void GraphicspDrawingPadItem::SetStatus(DrawingPadStatus s){
    m_status = s;
}

void GraphicspDrawingPadItem::AddArea(const PixelPoint point){
    m_Pad.DrawRect(point.x - m_cornerPixel, point.y - m_cornerPixel,
                   point.x + m_cornerPixel, point.y + m_cornerPixel);
}

void GraphicspDrawingPadItem::DeleteArea(const PixelPoint point){
    m_Pad.EraseRect(point.x - m_cornerPixel, point.y - m_cornerPixel,
                   point.x + m_cornerPixel, point.y + m_cornerPixel);
}

QRectF GraphicspDrawingPadItem::boundingRect() const{
    return QRectF(0, 0, m_width,m_height);
}

void GraphicspDrawingPadItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setBrush(QBrush(Color()));
    painter->setPen(Pen());

    QImage image((const uchar*)(m_Pad.RGBs()), m_width, m_height, QImage::Format_ARGB32);
    painter->drawPixmap(boundingRect(), QPixmap::fromImage(image), boundingRect());
    //painter->drawImage(m_width, m_height, image);
}

void GraphicspDrawingPadItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
    //QGraphicsItem::mousePressEvent(event);
}

void GraphicspDrawingPadItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    //QGraphicsItem::mouseReleaseEvent(event);
}

void GraphicspDrawingPadItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if (m_isDrawable && m_status == DrawingPadStatus::Drawing){
        PixelPoint p = PixelPoint((event->pos()).x(), (event->pos()).y());
        AddArea(p);
        emit ItemMouseMoveChanged(this);
    }
    else if (m_isDrawable && m_status == DrawingPadStatus::Erasing){
        PixelPoint p = PixelPoint((event->pos()).x(), (event->pos()).y());
        DeleteArea(p);
        emit ItemMouseMoveChanged(this);
    }
    update();
    //QGraphicsItem::mouseMoveEvent(event);
}

inline qreal GraphicspDrawingPadItem::m_Distance(QPointF p1, QPointF p2){
    return qAbs(p1.x() - p2.x()) + qAbs(p1.y() - p2.y());
}
