#include "marker3d.h"
#include "graphicsitem.h"

using namespace LinkGraphicsItem;

Marker3D::Marker3D()
{

}

Marker3D::Marker3D(MarkerType type, QPointF pos, qreal posHeight, QColor color)
    :Marker(type), m_pos(pos), m_posHeight(posHeight), m_color(color)
{

}

Marker3D::~Marker3D(){
    delete m_itemTop;
    delete m_itemLeft;
    delete m_itemFront;
}

QColor Marker3D::Color() const
{
    return m_color;
}

QPointF Marker3D::Pos() const
{
    return m_pos;
}

qreal Marker3D::CornerPixel() const
{
    return m_cornerPixel;
}

const LinkGraphicsItem::GraphicsItem *Marker3D::Item(View3DWindows flag) const
{
    switch (flag) {
    case View3DWindows::Top:
        return m_itemTop;
    case View3DWindows::Front:
        return m_itemFront;
    case View3DWindows::Left:
        return m_itemLeft;
    }
}

void Marker3D::SetPosHeight(qreal h){
    m_posHeight = h;
}

void Marker3D::SetPos(QPointF p, qreal h){
    m_pos = p;
    m_posHeight = h;
}

void Marker3D::SetPos(qreal px, qreal py, qreal pz){
    m_pos = QPointF(px, py);
    m_posHeight = pz;
}

void Marker3D::SetLengthIndex(qreal l){
    m_lengthIndex = l;
}

void Marker3D::SetWidthIndex(qreal w){
    m_widthIndex = w;
}

void Marker3D::SetHeightIndex(qreal h){
    m_heightIndex = h;
}

void Marker3D::SetIndexs(qreal l, qreal w, qreal h)
{
    m_lengthIndex = l;
    m_widthIndex = w;
    m_heightIndex = h;
}

void Marker3D::SetColor(const QColor &color){
    m_color = color;
    m_itemTop->SetColor(color);
    m_itemLeft->SetColor(color);
    m_itemFront->SetColor(color);
}

void Marker3D::SetCornerPixel(qreal pixel)
{
    m_cornerPixel = pixel;
}

void Marker3D::AddToItemGroup(QGraphicsItemGroup *itemGroup, View3DWindows view){
    switch (view) {
    case View3DWindows::Top:
        m_itemTop->setParentItem(itemGroup);
        break;
    case View3DWindows::Front:
        m_itemFront->setParentItem(itemGroup);
        break;
    case View3DWindows::Left:
        m_itemLeft->setParentItem(itemGroup);
        break;
    }
}

int Marker3D::IsItemIncluded(LinkGraphicsItem::GraphicsItem *item){
    if (item == m_itemTop)
        return static_cast<int>(View3DWindows::Top);
    else if (item == m_itemFront)
        return static_cast<int>(View3DWindows::Front);
    else if (item == m_itemLeft)
        return static_cast<int>(View3DWindows::Left);
    else
        return -1;
}

void Marker3D::Deserialize(const QJsonObject &obj){
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
            m_color = qRgb(array.at(0).toInt(), array.at(1).toInt(), array.at(2).toInt());
        }
    }
    if (obj.contains("Pos")){
        QJsonValue value = obj.value("Pos");
        if (value.isArray()){
            QJsonArray array = value.toArray();
            m_pos = QPointF(array.at(0).toDouble(), array.at(1).toDouble());
        }
    }
    if (obj.contains("PosHeight")){
        QJsonValue value = obj.value("PosHeight");
        if (value.isDouble()){
            m_posHeight = value.toDouble();
        }
    }
    if (obj.contains("Id")){
        QJsonValue value = obj.value("Id");
        if (value.isString()){
            m_id = value.toString();
        }
    }
}

void Marker3D::UpdateAllItem(GraphicsItem *item)
{
    int rtn = IsItemIncluded(item);
    if (rtn != -1){
        View3DWindows view = static_cast<View3DWindows>(rtn);
        UpdateFromView(view);
        UpdateToView(View3DWindows::Top);
        UpdateToView(View3DWindows::Left);
        UpdateToView(View3DWindows::Front);
    }
}
