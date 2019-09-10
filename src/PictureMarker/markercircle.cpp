#include "markercircle.h"
#include "graphicscircleitem.h"

#include <QRgb>


using namespace LinkGraphicsItem;

MarkerCircle::MarkerCircle()
{

}

MarkerCircle::MarkerCircle(QPointF p, qreal r)
    : Marker(MarkerType::Circle)
{
    Instantialize();
    GraphicsCircleItem* item = static_cast<GraphicsCircleItem*>(m_item);
    item->SetRadius(r);
    item->setPos(p);
}

MarkerCircle::~MarkerCircle(){
    delete static_cast<GraphicsCircleItem*>(m_item);
}

qreal MarkerCircle::Radius() const{
    GraphicsCircleItem* item = static_cast<GraphicsCircleItem*>(m_item);
    return item->Radius();
}

void MarkerCircle::SetRadius(const qreal r){
    GraphicsCircleItem* item = static_cast<GraphicsCircleItem*>(m_item);
    item->SetRadius(r);
}

void MarkerCircle::Instantialize(){
    delete static_cast<GraphicsCircleItem*>(m_item);
    m_item = new GraphicsCircleItem();
    Marker::Instantialize();
}

QJsonObject MarkerCircle::Serialize(){
    QJsonObject obj;
    GraphicsCircleItem* item = static_cast<GraphicsCircleItem*>(m_item);

    // type
    obj.insert("Type", static_cast<int>(m_type));

    // m_rgb
    QJsonArray arrayRgb;
    arrayRgb.append(Color().red());
    arrayRgb.append(Color().green());
    arrayRgb.append(Color().blue());
    obj.insert("Rgb", arrayRgb);

    // pos
    QJsonArray arrayPos;
    arrayPos.append(Pos().x());
    arrayPos.append(Pos().y());
    obj.insert("Pos", arrayPos);

    //id
    obj.insert("Id", m_id);

    // radius
    obj.insert("Radius", item->Radius());

    return obj;
}

void MarkerCircle::Deserialize(const QJsonObject &obj){
    Instantialize();
    GraphicsCircleItem* item = static_cast<GraphicsCircleItem*>(m_item);

    Marker::Deserialize(obj);

    if (obj.contains("Radius")){
        QJsonValue value = obj.value("Radius");
        if (value.isDouble()){
            item->SetRadius(value.toDouble());
        }
    }
}
