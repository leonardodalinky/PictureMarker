#ifndef GRAPHICSPDRAWINGPADITEM_H
#define GRAPHICSPDRAWINGPADITEM_H

#include <QWidget>

#include "graphicsitem.h"

namespace LinkGraphicsItem
{

enum class DrawingPadStatus{
    Normal = 0,
    Drawing,
    Erasing
};

struct DrawingPadPoint{
    // 中心点坐标
    QPointF center;
    // 边长
    qreal edge;
    DrawingPadPoint(QPointF p, qreal e)
        :center(p), edge(e){}
};

typedef QList<DrawingPadPoint> DrawingPadPointList;

class GraphicspDrawingPadItem : public GraphicsItem
{
    Q_OBJECT

public:
    GraphicspDrawingPadItem();
    GraphicspDrawingPadItem(qreal w, qreal h, const DrawingPadPointList& pad, QGraphicsItem* parent = nullptr);

    DrawingPadStatus Status() const;
    qreal BrushPixel() const;
    DrawingPadPointList Pad() const;

    void SetBrushPixel(qreal pixel);

    void AddPoint(const DrawingPadPoint& point);
    void AddPoint(const QPointF center, qreal edge);
    void DeleteArea(const DrawingPadPoint& point);
    void DeleteArea(const QPointF center, qreal edge);

public slots:
    void SetStatus(DrawingPadStatus s);

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    // 是否在正常,绘画,擦除中
    DrawingPadStatus m_status = DrawingPadStatus::Normal;
    // 画笔的正方形的边长
    qreal m_brushPixel = 5;
    // 存储绘画点的坐标
    DrawingPadPointList m_Pad;
    // 宽度
    qreal m_width;
    // 高度
    qreal m_height;

    // 两点之间的Manhattan距离
   inline qreal m_Distance(QPointF p1, QPointF p2);

};

}

#endif // GRAPHICSPDRAWINGPADITEM_H
