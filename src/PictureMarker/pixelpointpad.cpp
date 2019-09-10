#include "pixelpointpad.h"
#include <qdebug.h>
using namespace LinkGraphicsItem;

//#define MIN(x, y) ((x < y)? (x) : (y))
//#define MAX(x, y) ((x > y)? (x) : (y))

PixelPointPad::PixelPointPad()
{
    m_width = 0;
    m_height = 0;
    m_rgbs = nullptr;
    SetColor(Qt::black);
}

PixelPointPad::PixelPointPad(int w, int h)
{
    m_width = w;
    m_height = h;
    m_rgbs = new QRgb[w * h];
    InitBox();
    SetColor(Qt::black);
}

PixelPointPad::~PixelPointPad()
{
    delete m_rgbs;
}

int PixelPointPad::LeftBoxIndex() const{
    return m_LeftBoxIndex;
}

int PixelPointPad::TopBoxIndex() const{
    return m_TopBoxIndex;
}

int PixelPointPad::RightBoxIndex() const{
    return m_RightBoxIndex;
}

int PixelPointPad::BottomBoxIndex() const{
    return m_BottomBoxIndex;
}

QColor PixelPointPad::Color() const
{
    return m_color;
}

const QRgb *PixelPointPad::RGBs() const
{
    return m_rgbs;
}

void PixelPointPad::SetSize(int w, int h)
{
    if (m_width != w || m_height != h){
        m_width = w;
        m_height = h;
        delete[] m_rgbs;
        m_rgbs = new QRgb[w * h];
    }

    // 初始化最小包围盒
    InitBox();

    SetColor(Qt::black);
    TransparentAll();
}

void PixelPointPad::SetColor(const QColor &color)
{
    m_color = color;
    for (int h = 0;h < m_height;h++){
        for (int w = 0;w < m_width;w++){
            m_rgbs[w + h * m_width] &= 0xff000000;
            m_rgbs[w + h * m_width] |= (color.rgb() & 0x00ffffff);
        }
    }
}

void PixelPointPad::SetBoxIndex(int l, int t, int r, int b)
{
    m_LeftBoxIndex = l;
    m_TopBoxIndex = t;
    m_RightBoxIndex = r;
    m_BottomBoxIndex = b;
}

void PixelPointPad::InitBox()
{
    // 最小包围盒初始化
    m_TopBoxIndex = m_height;
    m_LeftBoxIndex = m_width;
    m_RightBoxIndex = -1;
    m_BottomBoxIndex = -1;
}

void PixelPointPad::TransparentAll()
{
    for (int h = 0;h < m_height;h++){
        for (int w = 0;w < m_width;w++){
            m_rgbs[w + h * m_width] = (m_rgbs[w + h * m_width] & 0x00ffffff);
        }
    }
}

void PixelPointPad::Clear()
{
    delete[] m_rgbs;
    m_rgbs = nullptr;
}

void PixelPointPad::DrawRect(int lx, int ly, int rx, int ry)
{
    // 边界检测
    if (rx < lx || ry < ly) return;
    if (lx < 0) lx = 0;
    if (ly < 0) ly = 0;
    if (rx >= m_width) rx = m_width - 1;
    if (ry >= m_height) ry = m_height - 1;

    // 最小包围盒边界检测
    if (lx < m_LeftBoxIndex) m_LeftBoxIndex = lx;
    if (ly < m_TopBoxIndex) m_TopBoxIndex = ly;
    if (rx > m_RightBoxIndex) m_RightBoxIndex = rx;
    if (ry > m_BottomBoxIndex) m_BottomBoxIndex = ry;

    for (int h = ly;h <= ry;h++){
        for (int w = lx;w <= rx;w++){
            m_rgbs[w + h * m_width] = m_color.rgb();
            m_rgbs[w + h * m_width] |= 0xff000000;
        }
    }

}

void PixelPointPad::DrawRect(PixelPoint x, PixelPoint y)
{
    DrawRect(x.x, x.y, y.x, y.y);
}

void PixelPointPad::EraseRect(int lx, int ly, int rx, int ry)
{
    if (rx < lx || ry < ly) return;
    if (lx < 0) lx = 0;
    if (ly < 0) ly = 0;
    if (rx >= m_width) rx = m_width - 1;
    if (ry >= m_height) ry = m_height - 1;

    for (int h = ly;h <= ry;h++){
        for (int w = lx;w <= rx;w++){
            m_rgbs[w + h * m_width] &= 0x00ffffff;
        }
    }
}

void PixelPointPad::EraseRect(PixelPoint x, PixelPoint y)
{
    EraseRect(x.x, x.y, y.x, y.y);
}

PixelPointPad& PixelPointPad::operator =(const PixelPointPad &other)
{
    SetSize(other.m_width, other.m_height);
    for (int h = 0;h < m_height;h++){
        for (int w = 0;w < m_width;w++){
            m_rgbs[w + h * m_width] = other.m_rgbs[w + h * m_width];
        }
    }

    m_LeftBoxIndex = other.m_LeftBoxIndex;
    m_RightBoxIndex = other.m_RightBoxIndex;
    m_TopBoxIndex = other.m_TopBoxIndex;
    m_BottomBoxIndex = other.m_BottomBoxIndex;

    m_color = other.m_color;

    return *this;
}

QJsonObject PixelPointPad::Serialize() const
{
    QJsonObject ans;

    // color
    QJsonArray arrayRgb;
    arrayRgb.append(Color().red());
    arrayRgb.append(Color().green());
    arrayRgb.append(Color().blue());
    ans.insert("Rgb", arrayRgb);

    // width
    ans.insert("Width", m_width);

    // height
    ans.insert("Height", m_height);

    // pad
    const QRgb* rgbpad = m_rgbs;
    QJsonArray arrayRows;
    for (int row = TopBoxIndex();row <= BottomBoxIndex();row++){
        QJsonArray arrayRow;
        // 一行的第一个元素插入行的索引号
        arrayRow.append(row);
        bool isBlack = false;
        for (int col = LeftBoxIndex();col <= RightBoxIndex();col++){
            if (!isBlack){
                if (rgbpad[row * (int)m_width + col] & 0xff000000){
                    // 之前未找到黑，但这一个审查为黑
                    isBlack = true;
                    arrayRow.append(col);
                }
                else{
                    // 之前未找到黑，且这一个审查不为黑
                    continue;
                }
            }
            else{
                if (rgbpad[row * (int)m_width + col] & 0xff000000){
                    // 之前找到黑，且这一个审查为黑
                    continue;
                }
                else{
                    // 之前找到黑，但这一个审查不为黑
                    isBlack = false;
                    arrayRow.append(col - 1);
                }
            }
        }
        // 如果黑色的地方到达边界，则加上边界
        if (isBlack){
            arrayRow.append(RightBoxIndex());
        }
        arrayRows.append(arrayRow);
    }
    ans.insert("Pad", arrayRows);

    // 最小包围盒
    QJsonArray arrayBox;
    arrayBox.append(LeftBoxIndex());
    arrayBox.append(TopBoxIndex());
    arrayBox.append(RightBoxIndex());
    arrayBox.append(BottomBoxIndex());
    ans.insert("Box", arrayBox);

    return ans;
}

void PixelPointPad::Deserialize(const QJsonObject &obj)
{
    int tempWidth = 0, tempHeight = 0;

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
    // 设置大小
    SetSize(tempWidth, tempHeight);

    // 画出
    if (obj.contains("Pad")){
        QJsonValue value = obj.value("Pad");
        if (value.isArray()){
            QJsonArray arrayRows = value.toArray();
            for (int i = 0;i < arrayRows.size();i++){
                QJsonArray arrayRow = arrayRows.at(i).toArray();
                int rowIndex = arrayRow.at(0).toInt();
                for (int c = 1; c < arrayRow.size();c = c + 2){
                    DrawRect(arrayRow.at(c).toInt(), rowIndex, arrayRow.at(c + 1).toInt(), rowIndex);
                }
            }
        }
    }

    // 最小包围盒
    if (obj.contains("Box")){
        QJsonValue value = obj.value("Box");
        if (value.isArray()){
            QJsonArray arrayBox = value.toArray();
            SetBoxIndex(arrayBox.at(0).toInt(), arrayBox.at(1).toInt(),
                                 arrayBox.at(2).toInt(), arrayBox.at(3).toInt());
        }
    }

    // color
    if (obj.contains("Rgb")){
        QJsonValue value = obj.value(("Rgb"));
        if (value.isArray()){
            QJsonArray array = value.toArray();
            SetColor(qRgb(array.at(0).toInt(), array.at(1).toInt(), array.at(2).toInt()));
        }
    }
}
