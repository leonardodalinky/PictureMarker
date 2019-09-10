#ifndef MARKERDRAWINGPAD_H
#define MARKERDRAWINGPAD_H

#include <QWidget>
#include "graphicsitem.h"
#include "marker.h"
#include "graphicspdrawingpaditem.h"
#include "pixelpointpad.h"


class MarkerDrawingPad : public Marker
{
public:
    MarkerDrawingPad();
    MarkerDrawingPad(qreal w, qreal h);
    ~MarkerDrawingPad();

    qreal Width() const;
    qreal Height() const;
    LinkGraphicsItem::PixelPointPad Pad() const;
    const LinkGraphicsItem::PixelPointPad& PadRef() const;

    void SetPad(const LinkGraphicsItem::PixelPointPad& pad);
    void SetStatus(LinkGraphicsItem::DrawingPadStatus s);
    
    void Instantialize();

    // 序列化
    QJsonObject Serialize();
    void Deserialize(const QJsonObject& obj);

private:

};

#endif // MARKERDRAWINGPAD_H
