#ifndef PIXELPOINTPAD_H
#define PIXELPOINTPAD_H

// pixelpointpad.h: 用于GraphicsDrawingPad的画板结构，用于记录并操作像素点

#include <QVector>
#include <QRgb>
#include <QColor>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>

namespace LinkGraphicsItem
{

enum class Pixel : char
{
    On,
    Off
};

// 表示像素点位置的二元组
struct PixelPoint{
    int x;
    int y;
    PixelPoint(int _x, int _y)
        :x(_x), y(_y){}
    PixelPoint()
        :x(0), y(0){}
};


// 像素点画布
class PixelPointPad
{
public:
    PixelPointPad();
    // 初始化长宽高
    PixelPointPad(int w, int h);
    ~PixelPointPad();

    int LeftBoxIndex() const;
    int TopBoxIndex() const;
    int RightBoxIndex() const;
    int BottomBoxIndex() const;
    QColor Color() const;
    const QRgb* RGBs() const;

    void SetSize(int w, int h);
    void SetColor(const QColor& color);
    void SetBoxIndex(int l, int t, int r, int b);
    // 最小包围盒初始化
    void InitBox();
    // 区域全透明
    void TransparentAll();
    // 清除区域
    void Clear();

    // 在两点之间画矩形
    void DrawRect(int lx, int ly, int rx, int ry);
    void DrawRect(PixelPoint x, PixelPoint y);
    // 删除两点之间的矩形
    void EraseRect(int lx, int ly, int rx, int ry);
    void EraseRect(PixelPoint x, PixelPoint y);

    // 重载运算符(深拷贝)
    PixelPointPad& operator = (const PixelPointPad& other);

    QJsonObject Serialize() const;
    void Deserialize(const QJsonObject& obj);

    // 存储图像
    QRgb* m_rgbs;

private:
    // 最小包围盒
    int m_LeftBoxIndex = 0;
    int m_TopBoxIndex = 0;
    int m_RightBoxIndex = 0;
    int m_BottomBoxIndex = 0;

    int m_width = -1;
    int m_height = -1;

    QColor m_color;


};

}
#endif // PIXELPOINTPAD_H
