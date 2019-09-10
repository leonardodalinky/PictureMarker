#ifndef VIEW2D_H
#define VIEW2D_H

// view2d.h: 2d的视图

#include "file.h"

#include <QWidget>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QMouseEvent>
#include "marker.h"
#include "toolmanager.h"


namespace Ui {
class View2D;
}

class View2D : public QWidget
{
    Q_OBJECT

public:
    // 父对象和状态栏指针
    explicit View2D(QWidget *parent = nullptr, QLabel* statusLabel = nullptr);
    ~View2D();

    void SetLensLabel(QLabel* label);

    // 在scene实现添加等功能
    bool eventFilter(QObject *object, QEvent *event);
    // 放大至下一阶段
    void ZoomIn();
    // 缩小至上一阶段
    void ZoomOut();

public slots:
    void LoadPicture(const File* file);
    void AddMarker(Marker* marker);
    void ClearMarker();

private:
    Ui::View2D *ui;

    // 状态栏
    QLabel* m_statusLabel;
    // 放大镜图片显示的标签
    QLabel* m_lensLabel = nullptr;

    // initiate
    void m_InitBackgroundGroup();
    void m_InitMarkerGroup();

    QGraphicsScene* m_scene;
    QGraphicsItemGroup* m_backgroundGroup;
    QGraphicsItemGroup* m_markerGroup;

    qreal m_backgroundWidth;
    qreal m_backgroundHeight;

    // 缩放的倍数的阶段（0 ~ 12）
    int m_zoom = 6;
};

#endif // VIEW2D_H
