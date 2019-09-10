#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// mainwindow.h: 程序的主窗口

#include "view2d.h"
#include "view3d.h"

#include <QMainWindow>
#include <QListWidget>
#include "file.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 处理放大镜(discard)
    // bool eventFilter(QObject* object, QEvent* event);

public slots:
    void LoadPicture(QListWidgetItem * item);
    void UpdateMarkerToView();

private slots:
    void on_action_Save_triggered();

    void on_action_ClearFiles_triggered();

    void on_action_AddMarker_triggered();

    void on_action_SaveTo_triggered();

    void on_FileDockWidget_visibilityChanged(bool visible);

    void on_action_FileWindow_triggered(bool checked);

    void on_action_MarkerWindow_triggered(bool checked);

    void on_action_ToolWindow_triggered(bool checked);

    void on_action_LensWindow_triggered(bool checked);

    void on_MarkerDockWidget_visibilityChanged(bool visible);

    void on_toolDockWidget_visibilityChanged(bool visible);

    void on_lensDockWidget_visibilityChanged(bool visible);

    void on_action_Author_triggered();

private:
    Ui::MainWindow *ui;
    // view指针初始化
    void m_InitView();
    // 2D视图的指针
    View2D* view2D = nullptr;
    // 3D视图的指针
    View3D* view3D = nullptr;
    // 状态栏
    QLabel* m_statusLabel;
    // 根据传入的file自动寻找同路径下的标注文件，若无则返回新建的路径，若有则返回已存在的路径
    QString m_AutoFindMarkerPath(const File* file);
};

#endif // MAINWINDOW_H
