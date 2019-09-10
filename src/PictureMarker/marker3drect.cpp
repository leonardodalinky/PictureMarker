#include "marker3drect.h"
#include "graphicsrectitem.h"

using namespace LinkGraphicsItem;

Marker3DRect::Marker3DRect()
{

}

Marker3DRect::Marker3DRect(qreal px, qreal py, qreal pz, qreal l, qreal w, qreal h)
    :Marker3D(MarkerType::Rect3D, QPointF(px, py), pz)
{
    m_length = l;
    m_width = w;
    m_height = h;
    Instantialize();
}

Marker3DRect::Marker3DRect(QPointF p, qreal pz, qreal l, qreal w, qreal h)
{
    m_length = l;
    m_width = w;
    m_height = h;
    SetPos(p, pz);
    Instantialize();
}

Marker3DRect::~Marker3DRect(){
    delete m_item;
}

qreal Marker3DRect::Length() const{
    return m_length;
}

qreal Marker3DRect::Width() const{
    return m_width;
}

qreal Marker3DRect::Height() const{
    return m_height;
}

void Marker3DRect::SetWidth(qreal w){
    m_width = w;
}

void Marker3DRect::SetHeight(qreal h){
    m_height = h;
}

void Marker3DRect::SetLength(qreal l){
    m_length = l;
}

void Marker3DRect::UpdateFromView(View3DWindows flag){
    GraphicsRectItem* itemTop = static_cast<GraphicsRectItem*>(m_itemTop);
    GraphicsRectItem* itemFront = static_cast<GraphicsRectItem*>(m_itemFront);
    GraphicsRectItem* itemLeft = static_cast<GraphicsRectItem*>(m_itemLeft);

    if (flag == View3DWindows::Top){
        m_pos = QPointF(itemTop->scenePos());
        m_length = itemTop->Width();
        m_width = itemTop->Height();
    }
    else if (flag == View3DWindows::Front){
        m_pos.setX(itemFront->scenePos().x());
        m_posHeight = itemFront->scenePos().y();
        m_length = itemFront->Width();
        m_height = itemFront->Height();
    }
    else if (flag == View3DWindows::Left){
        m_pos.setY(itemLeft->scenePos().x());
        m_posHeight = itemLeft->scenePos().y();
        m_width = itemLeft->Width();
        m_height = itemLeft->Height();
    }
}

// 将当前marker更新至某个视图绑定的item
void Marker3DRect::UpdateToView(View3DWindows flag){
    GraphicsRectItem* itemTop = static_cast<GraphicsRectItem*>(m_itemTop);
    GraphicsRectItem* itemFront = static_cast<GraphicsRectItem*>(m_itemFront);
    GraphicsRectItem* itemLeft = static_cast<GraphicsRectItem*>(m_itemLeft);

    if (flag == View3DWindows::Top){
        itemTop->setPos(m_pos);
        itemTop->SetWidth(m_length);
        itemTop->SetHeight(m_width);

        // 更改可见的状态
        if (m_heightIndex < m_posHeight || m_heightIndex > m_posHeight + m_height){
            itemTop->setVisible(false);
        }
        else{
            itemTop->setVisible(true);
        }
    }
    else if (flag == View3DWindows::Front){
        itemFront->setPos(QPointF(m_pos.x(), m_posHeight));
        itemFront->SetWidth(m_length);
        itemFront->SetHeight(m_height);

        if (m_widthIndex < m_pos.y() || m_widthIndex > m_pos.y() + m_width){
            itemFront->setVisible(false);
        }
        else {
            itemFront->setVisible(true);
        }
    }
    else if (flag == View3DWindows::Left){
        itemLeft->setPos(QPointF(m_pos.y(), m_posHeight));
        itemLeft->SetWidth(m_width);
        itemLeft->SetHeight(m_height);

        if (m_lengthIndex < m_pos.x() || m_lengthIndex > m_pos.x() + m_length){
            itemLeft->setVisible(false);
        }
        else {
            itemLeft->setVisible(true);
        }
    }
}

void Marker3DRect::Instantialize(){
    delete m_itemTop;
    delete m_itemFront;
    delete m_itemLeft;

    m_itemTop = new GraphicsRectItem(QPointF(Pos()), m_length, m_width, m_color);
    m_itemFront = new GraphicsRectItem(QPointF(Pos().x(), m_posHeight), m_length, m_height, m_color);
    m_itemLeft = new GraphicsRectItem(QPointF(Pos().y(), m_posHeight), m_width, m_height, m_color);

    // connect item and marker3d
    connect(m_itemTop, &GraphicsRectItem::ItemMouseMoveChanged, this, &Marker3DRect::UpdateAllItem);
    connect(m_itemFront, &GraphicsRectItem::ItemMouseMoveChanged, this, &Marker3DRect::UpdateAllItem);
    connect(m_itemLeft, &GraphicsRectItem::ItemMouseMoveChanged, this, &Marker3DRect::UpdateAllItem);

    // 刷新初始的显示
    for (int i = static_cast<int>(View3DWindows::Top);i <= static_cast<int>(View3DWindows::Left);i++){
        UpdateToView(static_cast<View3DWindows>(i));
    }
}

QJsonObject Marker3DRect::Serialize(){
    QJsonObject obj;

    // type
    obj.insert("Type", static_cast<int>(m_type));

    // m_rgb
    QJsonArray arrayRgb;
    arrayRgb.append(m_color.red());
    arrayRgb.append(m_color.green());
    arrayRgb.append(m_color.blue());
    obj.insert("Rgb", arrayRgb);

    // pos
    QJsonArray arrayPos;
    arrayPos.append(m_pos.x());
    arrayPos.append(m_pos.y());
    obj.insert("Pos", arrayPos);

    // posHeight
    obj.insert("PosHeight", m_posHeight);

    // id
    obj.insert("Id", m_id);

    // length
    obj.insert("Length", m_length);

    // width
    obj.insert("Width", m_width);

    // height
    obj.insert("Height", m_height);

    // cornerpixel
    obj.insert("CornerPixel", m_cornerPixel);

    return obj;
}

void Marker3DRect::Deserialize(const QJsonObject &obj){
    Marker3D::Deserialize(obj);

    if (obj.contains("Length")){
        QJsonValue value = obj.value("Length");
        if (value.isDouble()){
            m_length = value.toDouble();
        }
    }
    if (obj.contains("Width")){
        QJsonValue value = obj.value("Width");
        if (value.isDouble()){
            m_width = value.toDouble();
        }
    }
    if (obj.contains("Height")){
        QJsonValue value = obj.value("Height");
        if (value.isDouble()){
            m_height = value.toDouble();
        }
    }
    if (obj.contains("CornerPixel")){
        QJsonValue value = obj.value("CornerPixel");
        if (value.isDouble()){
            m_cornerPixel = value.toDouble();
        }
    }

    Instantialize();
}
