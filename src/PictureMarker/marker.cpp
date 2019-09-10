#include "marker.h"

using namespace LinkGraphicsItem;

Marker::Marker()
{

}

Marker::~Marker(){
}

Marker::Marker(MarkerType type)
    : m_type(type)
{
    m_id = "NULL";
}

void Marker::SetVisible(const bool b){
    m_visible = b;
}

void Marker::SetColor(const QColor &color){
    m_item->SetColor(color);
}

void Marker::SetPos(const QPointF pos)
{
    m_item->setPos(pos);
}

void Marker::SetCornerPixel(qreal pixel){
    m_item->SetCornerPixel(pixel);
}

bool Marker::Visible() const{
    return m_visible;
}

QColor Marker::Color() const{
    return m_item->Color();
}

QString Marker::Id() const{
    return m_id;
}

MarkerType Marker::Type() const{
    return m_type;
}

QString Marker::TypeName() const
{
    switch (m_type) {
    case MarkerType::Rect:
        return "Rect";
    case MarkerType::Circle:
        return "Circle";
    case MarkerType::DrawingPad:
        return "DrawingPad";
    case MarkerType::Rect3D:
        return "Rect3D";
    case MarkerType::DrawingPad3D:
        return "DrawingPad3D";
    }
}

QPointF Marker::Pos() const{
    return m_item->scenePos();
}

qreal Marker::CornerPixel() const{
    return m_item->CornerPixel();
}

GraphicsItem* Marker::Item() const{
    return m_item;
}

void Marker::Instantialize(){
    m_item->SetColor(qRgb(0, 0, 0));
}

void Marker::AddToItemGroup(QGraphicsItemGroup *itemGroup){
    m_item->setParentItem(itemGroup);
}

void Marker::SetId(const QString &id){
    m_id = id;
}

void Marker::Deserialize(const QJsonObject& obj){
    if (obj.contains("Type")){
        QJsonValue value = obj.value("Type");
        if (value.isDouble()){
            m_type = static_cast<MarkerType>(value.toVariant().toInt());
        }
    }
    if (obj.contains("Rgb")){
        QJsonValue value = obj.value(("Rgb"));
        if (value.isArray()){
            QJsonArray array = value.toArray();
            m_item->SetColor(qRgb(array.at(0).toInt(), array.at(1).toInt(), array.at(2).toInt()));
        }
    }
    if (obj.contains("Pos")){
        QJsonValue value = obj.value("Pos");
        if (value.isArray()){
            QJsonArray array = value.toArray();
            m_item->setPos(QPointF(array.at(0).toDouble(), array.at(1).toDouble()));
        }
    }
    if (obj.contains("Id")){
        QJsonValue value = obj.value("Id");
        if (value.isString()){
            m_id = value.toString();
        }
    }
}
