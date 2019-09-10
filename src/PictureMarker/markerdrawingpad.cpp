#include "markerdrawingpad.h"
#include "graphicspdrawingpaditem.h"
#include <QDebug>

using namespace LinkGraphicsItem;

MarkerDrawingPad::MarkerDrawingPad()
    :Marker(MarkerType::DrawingPad)
{

}

MarkerDrawingPad::MarkerDrawingPad(qreal w, qreal h)
    : Marker(MarkerType::DrawingPad)
{
    Instantialize();
    GraphicspDrawingPadItem* item = static_cast<GraphicspDrawingPadItem*>(m_item);
    item->SetSize(w, h);
}

MarkerDrawingPad::~MarkerDrawingPad(){
    delete static_cast<GraphicspDrawingPadItem*>(m_item);
}

qreal MarkerDrawingPad::Width() const
{
    GraphicspDrawingPadItem* item = static_cast<GraphicspDrawingPadItem*>(m_item);
    return item->Width();
}

qreal MarkerDrawingPad::Height() const
{
    GraphicspDrawingPadItem* item = static_cast<GraphicspDrawingPadItem*>(m_item);
    return item->Height();
}

void MarkerDrawingPad::SetPad(const LinkGraphicsItem::PixelPointPad& pad){
    GraphicspDrawingPadItem* item = static_cast<GraphicspDrawingPadItem*>(m_item);
    item->SetPad(pad);
}

void MarkerDrawingPad::SetStatus(DrawingPadStatus s){
    static_cast<GraphicspDrawingPadItem*>(m_item)->SetStatus(s);
}

void MarkerDrawingPad::Instantialize(){
    m_item = new GraphicspDrawingPadItem();
    m_item->SetColor(Qt::black);
}

QJsonObject MarkerDrawingPad::Serialize(){
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

    // width
    obj.insert("Width", Width());

    // height
    obj.insert("Height", Height());

    // id
    obj.insert("Id", m_id);

    // pixelpointpad
    GraphicspDrawingPadItem* item = static_cast<GraphicspDrawingPadItem*>(m_item);
    QJsonObject ppp = item->m_Pad.Serialize();
    obj.insert("PixelPointPad", ppp);

    return obj;
}

void MarkerDrawingPad::Deserialize(const QJsonObject &obj){
    Instantialize();
    GraphicspDrawingPadItem* item = static_cast<GraphicspDrawingPadItem*>(m_item);

    int tempWidth, tempHeight;

    if (obj.contains("Width")){
        QJsonValue value = obj.value("Width");
        if (value.isDouble()){
            tempWidth = value.toDouble();
        }
    }
    if (obj.contains("Height")){
        QJsonValue value = obj.value("Height");
        if (value.isDouble()){
            tempHeight = value.toDouble();
        }
    }
    // 设置大小（舍弃，大小在下面设置）
    item->SetSize(tempWidth, tempHeight);

    // pixelpointpad
    if (obj.contains("PixelPointPad")){
        QJsonValue value = obj.value("PixelPointPad");
        if (value.isObject()){
            item->m_Pad.Deserialize(value.toObject());
        }
    }

    Marker::Deserialize(obj);
}
