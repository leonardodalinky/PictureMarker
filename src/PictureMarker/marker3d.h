#ifndef MARKER3D_H
#define MARKER3D_H

// marker3d.h: 3d标注的基类

#include <QWidget>
#include "marker.h"

enum class View3DWindows{
    Top = 1,
    Front,
    Left
};

class Marker3D : public Marker
{
    Q_OBJECT

public:
    Marker3D();
    Marker3D(MarkerType type, QPointF pos = QPointF(0, 0), qreal posHeight = 0, QColor color = Qt::black);
    virtual ~Marker3D() override;

    QColor Color() const override;
    QPointF Pos() const;
    qreal CornerPixel() const;
    const LinkGraphicsItem::GraphicsItem* Item(View3DWindows flag) const;

    void SetPosHeight(qreal h);
    void SetPos(QPointF p, qreal h);
    void SetPos(qreal l, qreal w, qreal h);
    void SetLengthIndex(qreal l);
    void SetWidthIndex(qreal w);
    void SetHeightIndex(qreal h);
    void SetIndexs(qreal l, qreal w, qreal h);
    // 不仅改rgb值，还改变绑定对象的颜色
    void SetColor(const QColor& color) override;
    void SetCornerPixel(qreal pixel) override;

    // 从某个视图更新当前marker数据
    virtual void UpdateFromView(View3DWindows flag) = 0;
    // 将当前marker更新至某个视图
    virtual void UpdateToView(View3DWindows flag) = 0;
    // 通过marker中的已有信息，实例化m_item
    virtual void Instantialize() = 0;
    // 将某个视图中的标注item添加到group里
    void AddToItemGroup(QGraphicsItemGroup* itemGroup, View3DWindows view);

    // 检测item是否在这个marker3D中，若有，则返回对应的View3DWindows类型的整数，否则返回0
    int IsItemIncluded(LinkGraphicsItem::GraphicsItem* item);

    virtual QJsonObject Serialize() = 0;
    // 更新marker3D的反序列化
    virtual void Deserialize(const QJsonObject& obj) override;

public slots:
    void UpdateAllItem(LinkGraphicsItem::GraphicsItem* item);

protected:
    // 位置的高度坐标(顶部为0)
    qreal m_posHeight;

    // 当前位置
    qreal m_lengthIndex = 0;
    qreal m_widthIndex = 0;
    qreal m_heightIndex = 0;

    QColor m_color;
    QPointF m_pos;
    qreal m_cornerPixel = 4;

    // 三个视图的对象
    LinkGraphicsItem::GraphicsItem* m_itemTop = nullptr;
    LinkGraphicsItem::GraphicsItem* m_itemFront = nullptr;
    LinkGraphicsItem::GraphicsItem* m_itemLeft = nullptr;
};

#endif // MARKER3D_H
