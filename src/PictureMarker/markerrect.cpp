#include "markerrect.h"

using namespace LinkGraphicsItem;

MarkerRect::MarkerRect()
    : Marker(MarkerType::Rect)
{

}

MarkerRect::MarkerRect(QPointF p, qreal w, qreal h)
    : Marker(MarkerType::Rect)
{
    Instantialize();
    GraphicsRectItem* item = static_cast<GraphicsRectItem*>(m_item);
    item->setPos(p);
    item->SetWidth(w);
    item->SetHeight(h);
}

MarkerRect::~MarkerRect(){
    delete static_cast<GraphicsRectItem*>(m_item);
}

qreal MarkerRect::Width() const{
    GraphicsRectItem* item = static_cast<GraphicsRectItem*>(m_item);
    return item->Width();
}

qreal MarkerRect::Height() const{
    GraphicsRectItem* item = static_cast<GraphicsRectItem*>(m_item);
    return item->Height();
}

void MarkerRect::SetWidth(qreal w) const{
    GraphicsRectItem* item = static_cast<GraphicsRectItem*>(m_item);
    item->SetWidth(w);
}

void MarkerRect::SetHeight(qreal h) const{
    GraphicsRectItem* item = static_cast<GraphicsRectItem*>(m_item);
    item->SetHeight(h);
}

void MarkerRect::Instantialize(){
    delete static_cast<GraphicsRectItem*>(m_item);
    m_item = new GraphicsRectItem();
    Marker::Instantialize();
}

QJsonObject MarkerRect::Serialize(){
    QJsonObject obj;

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

    // width
    obj.insert("Width", Width());

    // height
    obj.insert("Height", Height());

    // cornerpixel
    obj.insert("CornerPixel", CornerPixel());

    return obj;
}

void MarkerRect::Deserialize(const QJsonObject &obj){
    Instantialize();
    GraphicsRectItem* item = static_cast<GraphicsRectItem*>(m_item);

    Marker::Deserialize(obj);

    if (obj.contains("Width")){
        QJsonValue value = obj.value("Width");
        if (value.isDouble()){
            item->SetWidth(value.toDouble());
        }
    }
    if (obj.contains("Height")){
        QJsonValue value = obj.value("Height");
        if (value.isDouble()){
            item->SetHeight(value.toDouble());
        }
    }
    if (obj.contains("CornerPixel")){
        QJsonValue value = obj.value("CornerPixel");
        if (value.isDouble()){
           item->SetCornerPixel(value.toDouble());
        }
    }
}
