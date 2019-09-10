#include "historymanager.h"
#include "marker.h"
#include "markerrect.h"
#include "markercircle.h"
#include "markerdrawingpad.h"
#include "marker3drect.h"
#include "marker3ddrawingpad.h"

HistoryManager::HistoryManager()
{

}

bool HistoryManager::CanRedo() const
{
    return m_canRedo;
}

void HistoryManager::Save(const int nowIndex, const MarkerList &list, bool isRedo){
    m_nowSelectedIndex = nowIndex;
    // 可改进
    if (!isRedo){
        m_canRedo = false;
        m_markerRedoList.clear();

        m_markerObjList.clear();
        for (auto it = list.constBegin();it != list.constEnd();it++){
            QJsonObject obj = (*it)->Serialize();
            m_markerObjList.append(obj);
        }
    }
    else{
        m_canRedo = true;
        m_markerRedoList.clear();
        for (auto it = list.constBegin();it != list.constEnd();it++){
            QJsonObject obj = (*it)->Serialize();
            m_markerRedoList.append(obj);
        }
    }
}

void HistoryManager::Initiate()
{
    m_markerObjList.clear();
    m_markerRedoList.clear();
    m_nowSelectedIndex = -1;
    m_canRedo = false;
}

void HistoryManager::Load(int &toIndex, MarkerList &toList, bool isRedo)
{
    // 不可重做时
    if (isRedo && !m_canRedo) return;

    toIndex = m_nowSelectedIndex;
    for (auto it = toList.begin();it != toList.end();it++){
        delete (*it);
    }
    toList.clear();

    if (!isRedo){
        for (auto it = m_markerObjList.constBegin();it != m_markerObjList.constEnd();it++){
            QJsonObject jsonObj = (*it);
            Marker* newMarker;
            switch (static_cast<MarkerType>(jsonObj.value("Type").toInt())) {
            case MarkerType::Rect:
                newMarker = new MarkerRect();
                newMarker->Deserialize(jsonObj);
                break;
            case MarkerType::Circle:
                newMarker = new MarkerCircle();
                newMarker->Deserialize(jsonObj);
                break;
            case MarkerType::DrawingPad:
                newMarker = new MarkerDrawingPad();
                newMarker->Deserialize(jsonObj);
                break;
            case MarkerType::Rect3D:
                newMarker = new Marker3DRect();
                newMarker->Deserialize(jsonObj);
                break;
            case MarkerType::DrawingPad3D:
                newMarker = new Marker3DDrawingPad();
                newMarker->Deserialize(jsonObj);
                break;
            }
            toList.append(newMarker);
        }
    }
    else{
        for (auto it = m_markerRedoList.constBegin();it != m_markerRedoList.constEnd();it++){
            QJsonObject jsonObj = (*it);
            Marker* newMarker;
            switch (static_cast<MarkerType>(jsonObj.value("Type").toInt())) {
            case MarkerType::Rect:
                newMarker = new MarkerRect();
                newMarker->Deserialize(jsonObj);
                break;
            case MarkerType::Circle:
                newMarker = new MarkerCircle();
                newMarker->Deserialize(jsonObj);
                break;
            case MarkerType::DrawingPad:
                newMarker = new MarkerDrawingPad();
                newMarker->Deserialize(jsonObj);
                break;
            case MarkerType::Rect3D:
                newMarker = new Marker3DRect();
                newMarker->Deserialize(jsonObj);
                break;
            case MarkerType::DrawingPad3D:
                newMarker = new Marker3DDrawingPad();
                newMarker->Deserialize(jsonObj);
                break;
            }
            toList.append(newMarker);

            // 重做后不可再重做
            m_canRedo = false;
        }
    }

}
