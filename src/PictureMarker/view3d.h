#ifndef VIEW3D_H
#define VIEW3D_H

// view3d.h: 3d视图

#include "file.h"

#include <QWidget>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QMouseEvent>
#include <QList>
#include "toolmanager.h"
#include "marker3d.h"

namespace Ui {
class View3D;
}

class View3D : public QWidget
{
    Q_OBJECT

public:
    // 父对象和状态栏指针
    explicit View3D(QWidget *parent = nullptr, QLabel* statusLabel = nullptr);
    ~View3D() override;

    void SetLensLabel(QLabel* label);

    // 在scene实现添加等功能
    bool eventFilter(QObject *object, QEvent *event) override;

    // 放大至下一阶段
    void ZoomIn(View3DWindows view);
    // 缩小至上一阶段
    void ZoomOut(View3DWindows view);

public slots:
    // 读取CT文件夹中的所有
    void LoadPicture(const File* file);
    // 将现有的marker3D加载到场景中，并在堆中分配新内存
    void AddMarker3D(Marker3D *marker, View3DWindows view);
    // 清空场景中的marker3d，但是不影响其本体的内存空间
    void ClearMarker(View3DWindows view);
    // 根据现有的Index,更新不同场景下的背景rgb，并放到view中
    void UpdateViewBackground(View3DWindows view);

private slots:
    void on_viewTopSlider_valueChanged(int value);

    void on_viewFrontSlider_valueChanged(int value);

    void on_viewLeftSlider_valueChanged(int value);

private:
    Ui::View3D *ui;

    //状态栏标签的指针
    QLabel* m_statusLabel;
    // 放大镜显示的标签
    QLabel* m_lensLabel;

    // view3D的初始化
    void m_Initiate();

    // initiate, clear the items' memory
    void m_InitBackgroundGroup(View3DWindows flag);
    void m_InitMarkerGroup(View3DWindows flag);

    // 控制三个graphicsview窗口中的图元
    QGraphicsScene* m_sceneTop;
    QGraphicsItemGroup* m_backgroundGroupTop;
    QGraphicsItemGroup* m_markerGroupTop;
    QGraphicsScene* m_sceneFront;
    QGraphicsItemGroup* m_backgroundGroupFront;
    QGraphicsItemGroup* m_markerGroupFront;
    QGraphicsScene* m_sceneLeft;
    QGraphicsItemGroup* m_backgroundGroupLeft;
    QGraphicsItemGroup* m_markerGroupLeft;

    // 从俯视图看到的长宽高
    qreal m_backgroundLength = 0;
    qreal m_backgroundWidth = 0;
    qreal m_backgroundHeight = 0;

    // 三个graphicsview窗口平面的位置
    qreal m_lengthIndex = 0;
    qreal m_widthIndex = 0;
    qreal m_heightIndex = 0;

    // 照片读取列
    QList<QImage> m_images;
    QList<const QRgb*> m_rgbs;
    QRgb* m_rgbTop = nullptr;
    QRgb* m_rgbFront = nullptr;
    QRgb* m_rgbLeft = nullptr;

    // 缩放的倍数的阶段（0 ~ 12）
    int m_zoomTop = 6;
    int m_zoomFront = 6;
    int m_zoomLeft = 6;
};

#endif // VIEW3D_H
