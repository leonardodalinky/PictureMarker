#include "marker3ddrawingpad.h"
#include <QDebug>

using namespace LinkGraphicsItem;

Marker3DDrawingPad::Marker3DDrawingPad()
{

}

Marker3DDrawingPad::Marker3DDrawingPad(View3DWindows view, qreal l, qreal w, qreal h)
    :Marker3D(MarkerType::DrawingPad3D)
{
    m_length = l;
    m_width = w;
    m_height = h;
    m_mainView = view;
    Instantialize();

    GraphicspDrawingPadItem* itemTop = static_cast<GraphicspDrawingPadItem*>(m_itemTop);
    GraphicspDrawingPadItem* itemFront = static_cast<GraphicspDrawingPadItem*>(m_itemFront);
    GraphicspDrawingPadItem* itemLeft = static_cast<GraphicspDrawingPadItem*>(m_itemLeft);

    switch (view) {
    case View3DWindows::Top:
        itemFront->SetDrawable(false);
        itemLeft->SetDrawable(false);
        break;
    case View3DWindows::Front:
        itemTop->SetDrawable(false);
        itemLeft->SetDrawable(false);
        break;
    case View3DWindows::Left:
        itemTop->SetDrawable(false);
        itemFront->SetDrawable(false);
        break;
    }
}

Marker3DDrawingPad::~Marker3DDrawingPad()
{
    for (auto it = m_pixelPointPadList.begin();it != m_pixelPointPadList.end();it++){
        delete (*it);
    }
}

QColor Marker3DDrawingPad::Color() const
{
    return m_color;
}

View3DWindows Marker3DDrawingPad::MainView() const
{
    return m_mainView;
}

void Marker3DDrawingPad::SetColor(const QColor &color)
{
    m_color = color;
    m_itemTop->SetColor(color);
    m_itemFront->SetColor(color);
    m_itemLeft->SetColor(color);
    for (auto it = m_pixelPointPadList.begin();it != m_pixelPointPadList.end();it++){
        (*it)->SetColor(color);
    }
}

void Marker3DDrawingPad::SetStatus(DrawingPadStatus s)
{
    GraphicspDrawingPadItem* itemTop = static_cast<GraphicspDrawingPadItem*>(m_itemTop);
    GraphicspDrawingPadItem* itemFront = static_cast<GraphicspDrawingPadItem*>(m_itemFront);
    GraphicspDrawingPadItem* itemLeft = static_cast<GraphicspDrawingPadItem*>(m_itemLeft);

    itemTop->SetStatus(s);
    itemFront->SetStatus(s);
    itemLeft->SetStatus(s);
}

bool Marker3DDrawingPad::Contains(int index)
{
    return m_pixelPointPadList.contains(index);
}

void Marker3DDrawingPad::AddPad(int index)
{
    if (m_pixelPointPadList.contains(index)) return;
    PixelPointPad* newPad = new PixelPointPad();
    switch (m_mainView) {
    case View3DWindows::Top:
        newPad->SetSize((int)m_length, (int)m_width);
        break;
    case View3DWindows::Front:
        newPad->SetSize((int)m_length, (int)m_height);
        break;
    case View3DWindows::Left:
        newPad->SetSize((int)m_width, (int)m_height);
        break;
    }
    newPad->SetColor(m_color);
    m_pixelPointPadList.insert(index, newPad);
}

// 从某个视图更新当前marker数据
void Marker3DDrawingPad::UpdateFromView(View3DWindows view)
{
    // 仅允许从主视角更新
    if (view != m_mainView) return;

    if (view == View3DWindows::Top){
        if (m_pixelPointPadList.contains((int)m_heightIndex)){
            *(m_pixelPointPadList[(int)m_heightIndex]) = static_cast<GraphicspDrawingPadItem*>(m_itemTop)->m_Pad;
        }
        /*
        else{
            PixelPointPad* newPad = new PixelPointPad();
            *newPad = static_cast<GraphicspDrawingPadItem*>(m_itemTop)->m_Pad;
            m_pixelPointPadList.insert((int)m_heightIndex, newPad);
        }
        */
    }
    else if (view == View3DWindows::Front){
        if (m_pixelPointPadList.contains((int)m_widthIndex)){
            *(m_pixelPointPadList[(int)m_widthIndex]) = static_cast<GraphicspDrawingPadItem*>(m_itemFront)->m_Pad;
        }
        /*
        else{
            PixelPointPad* newPad = new PixelPointPad();
            *newPad = static_cast<GraphicspDrawingPadItem*>(m_itemFront)->m_Pad;
            m_pixelPointPadList.insert((int)m_widthIndex, newPad);
        }
        */
    }
    else if (view == View3DWindows::Left){
        if (m_pixelPointPadList.contains((int)m_lengthIndex)){
            *(m_pixelPointPadList[(int)m_lengthIndex]) = static_cast<GraphicspDrawingPadItem*>(m_itemLeft)->m_Pad;
        }
        /*
        else{
            PixelPointPad* newPad = new PixelPointPad();
            *newPad = static_cast<GraphicspDrawingPadItem*>(m_itemTop)->m_Pad;
            m_pixelPointPadList.insert((int)m_lengthIndex, newPad);
        }
        */
    }
}

// 将当前marker更新至某个视图
void Marker3DDrawingPad::UpdateToView(View3DWindows view)
{
    // 仅允许更新到主视角
    if (view != m_mainView) return;

    GraphicspDrawingPadItem* itemTop = static_cast<GraphicspDrawingPadItem*>(m_itemTop);
    GraphicspDrawingPadItem* itemFront = static_cast<GraphicspDrawingPadItem*>(m_itemFront);
    GraphicspDrawingPadItem* itemLeft = static_cast<GraphicspDrawingPadItem*>(m_itemLeft);

    if (view == View3DWindows::Top){
        if (m_pixelPointPadList.contains((int)m_heightIndex)){
            itemTop->setVisible(true);
            itemTop->m_Pad = *(m_pixelPointPadList[(int)m_heightIndex]);
        }
        else{
            itemTop->setVisible(false);
        }

        itemFront->m_Pad.TransparentAll();
        itemLeft->m_Pad.TransparentAll();

        // front
        for (int x = 0;x < (int)m_length;x++){
            for (int y = 0;y < (int)m_height;y++){
                if (!m_pixelPointPadList.contains(y)) continue;
                QRgb alphaMask = m_pixelPointPadList[y]->m_rgbs[x + (int)m_widthIndex * (int)m_length] & 0xff000000;
                itemFront->m_Pad.m_rgbs[x + y * (int)itemFront->Width()] |= alphaMask;
            }
        }
        // left
        for (int x = 0;x < itemLeft->Width();x++)
            for (int y = 0;y < itemLeft->Height();y++){
                if (!m_pixelPointPadList.contains(y)) continue;
                QRgb alphaMask = m_pixelPointPadList[y]->m_rgbs[(int)m_lengthIndex + x * (int)m_length] & 0xff000000;
                itemLeft->m_Pad.m_rgbs[x + y * (int)itemLeft->Width()] |= alphaMask;
            }
    }
    else if (view == View3DWindows::Front){
        if (m_pixelPointPadList.contains((int)m_widthIndex)){
            itemFront->setVisible(true);
            itemFront->m_Pad = *(m_pixelPointPadList[(int)m_widthIndex]);
        }
        else{
            itemFront->setVisible(false);
        }

        itemTop->m_Pad.TransparentAll();
        itemLeft->m_Pad.TransparentAll();

        // top
        for (int x = 0;x < itemTop->Width();x++)
            for (int y = 0;y < itemTop->Height();y++){
                if (!m_pixelPointPadList.contains(y)) continue;
                QRgb alphaMask = m_pixelPointPadList[y]->m_rgbs[x + (int)m_heightIndex * (int)m_length] & 0xff000000;
                itemTop->m_Pad.m_rgbs[x + y * (int)itemTop->Width()] |= alphaMask;
            }

        // left
        for (int x = 0;x < itemLeft->Width();x++)
            for (int y = 0;y < itemLeft->Height();y++){
                if (!m_pixelPointPadList.contains(x)) continue;
                QRgb alphaMask = m_pixelPointPadList[x]->m_rgbs[(int)m_lengthIndex + y * (int)m_length] & 0xff000000;
                itemLeft->m_Pad.m_rgbs[x + y * (int)itemLeft->Width()] |= alphaMask;
            }

    }
    else if (view == View3DWindows::Left){
        if (m_pixelPointPadList.contains((int)m_lengthIndex)){
            itemLeft->setVisible(true);
            itemLeft->m_Pad = *(m_pixelPointPadList[(int)m_lengthIndex]);
        }
        else{
            itemFront->setVisible(false);
        }

        itemTop->m_Pad.TransparentAll();
        itemFront->m_Pad.TransparentAll();

        // top
        for (int x = 0;x < itemTop->Width();x++)
            for (int y = 0;y < itemTop->Height();y++){
                if (!m_pixelPointPadList.contains(x)) continue;
                QRgb alphaMask = m_pixelPointPadList[x]->m_rgbs[y + (int)m_heightIndex * (int)m_width] & 0xff000000;
                itemTop->m_Pad.m_rgbs[x + y * (int)itemTop->Width()] |= alphaMask;
            }

        // front
        for (int x = 0;x < itemFront->Width();x++)
            for (int y = 0;y < itemFront->Height();y++){
                if (!m_pixelPointPadList.contains(x)) continue;
                QRgb alphaMask = m_pixelPointPadList[x]->m_rgbs[(int)m_widthIndex + y * (int)m_width] & 0xff000000;
                itemFront->m_Pad.m_rgbs[x + y * (int)itemFront->Width()] |= alphaMask;
            }
    }
}

void Marker3DDrawingPad::Instantialize()
{
    delete m_itemTop;
    delete m_itemFront;
    delete m_itemLeft;

    m_itemTop = new GraphicspDrawingPadItem(m_length, m_width);
    m_itemFront = new GraphicspDrawingPadItem(m_length, m_height);
    m_itemLeft = new GraphicspDrawingPadItem(m_width, m_height);

    GraphicspDrawingPadItem* itemTop = static_cast<GraphicspDrawingPadItem*>(m_itemTop);
    GraphicspDrawingPadItem* itemFront = static_cast<GraphicspDrawingPadItem*>(m_itemFront);
    GraphicspDrawingPadItem* itemLeft = static_cast<GraphicspDrawingPadItem*>(m_itemLeft);

    // connect item and marker3d
    connect(itemTop, &GraphicspDrawingPadItem::ItemMouseMoveChanged, this, &Marker3DDrawingPad::UpdateAllItem);
    connect(itemFront, &GraphicspDrawingPadItem::ItemMouseMoveChanged, this, &Marker3DDrawingPad::UpdateAllItem);
    connect(itemLeft, &GraphicspDrawingPadItem::ItemMouseMoveChanged, this, &Marker3DDrawingPad::UpdateAllItem);

    // 刷新初始的显示
    for (int i = static_cast<int>(View3DWindows::Top);i <= static_cast<int>(View3DWindows::Left);i++){
        UpdateToView(static_cast<View3DWindows>(i));
    }
}

QJsonObject Marker3DDrawingPad::Serialize()
{
    QJsonObject obj;

    // type
    obj.insert("Type", static_cast<int>(m_type));

    // m_rgb
    QJsonArray arrayRgb;
    arrayRgb.append(m_color.red());
    arrayRgb.append(m_color.green());
    arrayRgb.append(m_color.blue());
    obj.insert("Rgb", arrayRgb);

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

    // mainview
    obj.insert("MainView", static_cast<int>(m_mainView));

    // pad的序列化
    QJsonArray arrayPads;
    for (auto it = m_pixelPointPadList.begin();it != m_pixelPointPadList.end();it++){
        QJsonArray arrayPad;
        arrayPad.append(it.key());
        arrayPad.append(it.value()->Serialize());
        arrayPads.append(arrayPad);
    }
    obj.insert("Pads", arrayPads);

    return obj;
}

void Marker3DDrawingPad::Deserialize(const QJsonObject &obj){
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
    if (obj.contains("MainView")){
        QJsonValue value = obj.value("MainView");
        if (value.isDouble()){
            m_mainView = static_cast<View3DWindows>(value.toInt());
        }
    }
    // pad的反序列化
    if (obj.contains("Pads")){
        QJsonValue value = obj.value("Pads");
        if (value.isArray()){
            QJsonArray arrayPads = value.toArray();
            for (int i = 0;i < arrayPads.size();i++){
                QJsonArray arrayPad = arrayPads.at(i).toArray();
                int index = arrayPad.at(0).toInt();
                PixelPointPad* newPad = new PixelPointPad();
                newPad->Deserialize(arrayPad.at(1).toObject());

                m_pixelPointPadList.insert(index, newPad);
            }
        }
    }

    Instantialize();
}

void Marker3DDrawingPad::UpdateAllItem(GraphicspDrawingPadItem *item)
{

}
