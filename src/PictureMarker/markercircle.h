#ifndef MARKERCIRCLE_H
#define MARKERCIRCLE_H

#include <QWidget>
#include <QJsonDocument>
#include "graphicsitem.h"
#include "marker.h"

class MarkerCircle : public Marker
{
public:
    MarkerCircle();
    MarkerCircle(QPointF p, qreal r);
    ~MarkerCircle();

    qreal Radius() const;

    void SetRadius(const qreal r);

    void Instantialize();

    // 序列化
    QJsonObject Serialize();
    void Deserialize(const QJsonObject& obj);

private:

};

#endif // MARKERCIRCLE_H
