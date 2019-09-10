#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H

// toolmanager.h: 负责管理工具的选取的控件

#include <QWidget>

enum class ToolType{
    Mouse = 0,
    Square,
    Brush,
    Circle,
    Draw,
    Erase = 5,
    ZoomIn,
    ZoomOut,
    Lens
};

namespace Ui {
class ToolManager;
}

class ToolManager : public QWidget
{
    Q_OBJECT

public:
    explicit ToolManager(QWidget *parent = nullptr);
    ~ToolManager();
    static int CurrentIndex();
    static ToolType CurrentTool();
    static int LensPixel() ;
    void SetTool(ToolType type);

    void Set3DStyle(bool is3D);

    static ToolManager* Instance();

private slots:
    void on_Mouse_clicked();

    void on_Square_clicked();

    void on_Brush_clicked();

    void on_Circle_clicked();

    void on_Draw_clicked();

    void on_Erase_clicked();

    void on_ZoomIn_clicked();

    void on_ZoomOut_clicked();

    void on_Lens_clicked();

private:
    Ui::ToolManager *ui;
    // 使所有按钮弹起
    void m_SetAllOff();
    // 当前选中的工具索引号
    static int m_currentIndex;
    // 当前选中的工具
    static ToolType m_currentTool;
    // 放大镜矩形边长一半
    static int m_lensPixel;

    static ToolManager* self;
};

#endif // TOOLMANAGER_H
