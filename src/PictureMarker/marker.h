#ifndef MARKER_H
#define MARKER_H

// marker.h: 标注的基类

#include <QWidget>
#include <QGraphicsItemGroup>
#include <QRgb>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QFile>
#include "graphicsitem.h"


enum class MarkerType{
    Rect = 0,
    Circle,
    DrawingPad,
    Rect3D,
    DrawingPad3D
};

class Marker : public QObject
{
    Q_OBJECT

public:
    Marker();
    Marker(MarkerType type);
    virtual ~Marker();
    bool Visible() const;
    virtual QColor Color() const;

    QString Id() const;
    MarkerType Type() const;
    QString TypeName() const;
    // item中实际的pos
    QPointF Pos() const;
    qreal CornerPixel() const;
    LinkGraphicsItem::GraphicsItem* Item() const;
    
    // 通过marker中的已有信息，实例化m_item
    virtual void Instantialize();
    void AddToItemGroup(QGraphicsItemGroup* itemGroup);

    void SetId(const QString& id);
    void SetVisible(const bool b);
    // 不仅改rgb值，还改变绑定对象的颜色
    virtual void SetColor(const QColor& color);
    // 改变绑定对象的位置(2d)
    void SetPos(const QPointF pos);
    virtual void SetCornerPixel(qreal pixel);

    virtual QJsonObject Serialize() = 0;
    virtual void Deserialize(const QJsonObject& obj);

protected:
    QString m_id;
    bool m_visible = false;
    MarkerType m_type;

    // only work in 2D
    LinkGraphicsItem::GraphicsItem* m_item = nullptr;
};

#endif // MARKER_H
