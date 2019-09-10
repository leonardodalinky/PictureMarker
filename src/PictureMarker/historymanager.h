#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

// historymanager.h: 用于管理撤销与重做的功能

#include <QWidget>
#include "marker.h"


typedef QList<Marker*> MarkerList;

class HistoryManager
{
public:
    HistoryManager();
    bool CanRedo() const;

    // 如果isRedo为true，则存入Redo备选地，否则存入撤销处
    void Save(const int nowIndex, const MarkerList& list, bool isRedo = false);
    void Load(int& toIndex, MarkerList& toList, bool isRedo = false);

    void Initiate();

private:
    QList<QJsonObject> m_markerObjList;
    QList<QJsonObject> m_markerRedoList;
    int m_nowSelectedIndex = -1;
    // 是否可以重做
    bool m_canRedo = false;
};

#endif // HISTORYMANAGER_H
