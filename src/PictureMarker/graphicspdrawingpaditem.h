#ifndef GRAPHICSPDRAWINGPADITEM_H
#define GRAPHICSPDRAWINGPADITEM_H

// graphicsdrawingpaditem.h: 用于GraphicsView框架的分割标注图元

#include <QWidget>

#include "graphicsitem.h"
#include "pixelpointpad.h"

namespace LinkGraphicsItem
{

enum class DrawingPadStatus{
    Normal = 0,
    Drawing,
    Erasing
};

class GraphicspDrawingPadItem : public GraphicsItem
{
    Q_OBJECT

public:
    GraphicspDrawingPadItem(QGraphicsItem* parent = nullptr);
    GraphicspDrawingPadItem(qreal w, qreal h, QGraphicsItem* parent = nullptr);

    qreal Width() const;
    qreal Height() const;
    DrawingPadStatus Status() const;
    qreal BrushPixel() const;
    bool Drawable() const;

    void SetBrushPixel(qreal pixel);
    void SetPad(const PixelPointPad& pad);
    void SetColor(const QColor& color);
    void SetSize(qreal w, qreal h);
    void SetDrawable(bool b);

    void AddArea(const PixelPoint point);
    void DeleteArea(const PixelPoint point);

    // 存储绘画点的坐标
    PixelPointPad m_Pad;

public slots:
    void SetStatus(DrawingPadStatus s);

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
    void ItemMouseMoveChanged(GraphicspDrawingPadItem*);

private:
    // 是否在正常,绘画,擦除中
    DrawingPadStatus m_status = DrawingPadStatus::Normal;
    // 画笔的正方形的边长的一半
    qreal m_brushPixel = 3;
    // 宽度
    qreal m_width;
    // 高度
    qreal m_height;

    // 是否可通过画笔绘画
    bool m_isDrawable = true;

    // 两点之间的Manhattan距离
   inline qreal m_Distance(QPointF p1, QPointF p2);

};

}

#endif // GRAPHICSPDRAWINGPADITEM_H
