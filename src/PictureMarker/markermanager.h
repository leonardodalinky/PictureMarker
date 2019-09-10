#ifndef MARKERMANAGER_H
#define MARKERMANAGER_H

// markermanager.h: 管理标注的控件

#include <QWidget>
#include <QTableWidget>
#include "view2d.h"
#include "view3d.h"
#include "marker.h"
#include "markerrect.h"
#include "markercircle.h"
#include "markerdrawingpad.h"
#include "historymanager.h"


typedef QList<Marker*> MarkerList;

namespace Ui {
class MarkerManager;
}

class MarkerManager : public QWidget
{
    Q_OBJECT

public:
    explicit MarkerManager(QWidget *parent = nullptr);
    ~MarkerManager();

    static MarkerManager* Instance();

    // 返回现在选中的标注
    Marker* SelectedMarker() const;

    // 更新列表框中的显示
    void ListUpdate();
    // 设置所有标注不可见
    void SetAllMarkerInvisible();
    // 将可见的标注显示到view中
    void UpdateToView(View2D* view2d);
    void UpdateToView(View3D* view3d);
    
    // 从路径中读取标注
    void LoadMarker(const QString& markerPath);
    // 在路径中存储标注
    void Save(const QString& markerPath);

    // 创建一个新的标注并设为唯一可见
    void CreateMarker(MarkerType type, qreal w, qreal h, qreal l = 0, View3DWindows mainView = View3DWindows::Top);

public slots:
    // 清空所有标记
    void Clear();
    // 撤销与重做
    void Undo();
    void Redo();

signals:
    void UpdateMarkerToViewRequest();


private slots:
    void RMenuRename();
    void ShowRMenu(QPoint p);
    void RMenuDelete();
    void RMenuRecolor();
    void on_markerTableWidget_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::MarkerManager *ui;

    MarkerList m_markerList;
    // 当前被选中的的索引号
    int m_nowSelectedIndex = -1;
    // 右键菜单
    QMenu* m_menu;
    // 初始化右键菜单
    void m_InitMenu();
    QString m_IdGen();
    // 管理撤销
    HistoryManager m_history;

    static MarkerManager* self;
};

#endif // MARKERMANAGER_H
