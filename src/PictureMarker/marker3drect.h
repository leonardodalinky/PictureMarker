#ifndef MARKER3DRECT_H
#define MARKER3DRECT_H

#include <QWidget>
#include "marker3d.h"

class Marker3DRect : public Marker3D
{
public:
    Marker3DRect();
    Marker3DRect(qreal px, qreal py, qreal pz, qreal l, qreal w, qreal h);
    Marker3DRect(QPointF p, qreal pz, qreal l, qreal w, qreal h);
    ~Marker3DRect();

    qreal Length() const;
    qreal Width() const;
    qreal Height() const;

    void SetLength(qreal l);
    void SetWidth(qreal w);
    void SetHeight(qreal h);

    // 从某个视图绑定的item更新当前marker数据
    virtual void UpdateFromView(View3DWindows flag);
    // 将当前marker更新至某个视图绑定的item
    virtual void UpdateToView(View3DWindows flag);
    // 通过marker中的已有信息，实例化m_item
    virtual void Instantialize();

    virtual QJsonObject Serialize();
    // 更新marker3D的反序列化
    virtual void Deserialize(const QJsonObject& obj);

private:
    qreal m_length;
    qreal m_width;
    qreal m_height;
};

#endif // MARKER3DRECT_H
