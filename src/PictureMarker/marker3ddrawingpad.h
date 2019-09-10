#ifndef MARKER3DDRAWINGPAD_H
#define MARKER3DDRAWINGPAD_H

#include <QWidget>
#include <QMap>
#include "marker3d.h"
#include "graphicspdrawingpaditem.h"

typedef QMap<int, LinkGraphicsItem::PixelPointPad*> PixelPointPadMap;

class Marker3DDrawingPad : public Marker3D
{
public:
    Marker3DDrawingPad();
    Marker3DDrawingPad(View3DWindows view, qreal l, qreal w, qreal h);
    ~Marker3DDrawingPad() override;

    virtual QColor Color() const override;
    View3DWindows MainView() const;

    // 不仅改rgb值，还改变绑定对象的颜色
    virtual void SetColor(const QColor& color) override;
    void SetStatus(LinkGraphicsItem::DrawingPadStatus s);
    // 查看是否包含某一层
    bool Contains(int index);

    // 在主层的某个位置增加一个画板层,如若已存在，则不做反应
    void AddPad(int index);

    // 从某个视图更新当前marker数据
    void UpdateFromView(View3DWindows view) override;
    // 将当前marker更新至某个视图
    void UpdateToView(View3DWindows view) override;
    // 通过marker中的已有信息，实例化m_item
    virtual void Instantialize() override;

    // 序列化
    virtual QJsonObject Serialize() override;
    // 更新marker3D的反序列化
    virtual void Deserialize(const QJsonObject& obj) override;

public slots:
    void UpdateAllItem(LinkGraphicsItem::GraphicspDrawingPadItem* item);

private:
    View3DWindows m_mainView;

    qreal m_length;
    qreal m_width;
    qreal m_height;

    PixelPointPadMap m_pixelPointPadList;
};

#endif // MARKER3DDRAWINGPAD_H
