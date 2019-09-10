#ifndef MARKERRECT_H
#define MARKERRECT_H

#include "marker.h"

#include <QWidget>
#include "graphicsrectitem.h"

class MarkerRect : public Marker
{
public:
    MarkerRect();
    MarkerRect(QPointF p, qreal w, qreal h);
    ~MarkerRect();

    qreal Width() const;
    qreal Height() const;

    void SetWidth(qreal w) const;
    void SetHeight(qreal h) const;

    void Instantialize();

    // 序列化
    QJsonObject Serialize();
    void Deserialize(const QJsonObject& obj);

private:

};

#endif // MARKERRECT_H
