#ifndef FILEMANAGER_H
#define FILEMANAGER_H

// filemanager.h: 管理文件的读取存储的控件，并显示在文件区域内

#include "file.h"
#include <QWidget>
#include <QListWidget>
#include <QMenu>

namespace Ui {
class FileManager;
}

class FileManager : public QWidget
{
    Q_OBJECT

public:
    explicit FileManager(QWidget *parent = nullptr);
    ~FileManager();

    static FileManager* Instance();

    // 返回当前打开的文件数量
    int FileSize() const;

    // 当前打开的照片文件
    QString m_nowOpenFile = "";
    // 当前打开的标注文件
    QString m_nowOpenMarkerFile = "";

public slots:
    // 添加文件
    void InputFiles();
    // 添加文件夹下所有图片文件
    void InputFolder();
    // 清空文件列表
    void ClearList();
    // 根据id寻找路径，若无则返回-1
    QString FindPathOfFile(const QString& id) const;
    // 根据id返回文件的指针
    const File* FindFile(const QString& id) const;
    // 保存的提醒，并操作
    bool SaveRemind();

private slots:
    // 刷新目前的文件表
    void Refresh();
    void ShowRMenu(QPoint p);
    void RMenuRename();
    void RMenuDelete();

    void on_fileListWidget_itemDoubleClicked(QListWidgetItem *item);

signals:
    // 刷新目前的文件表
    void Update();
    // 加载图片的请求
    void LoadPictureRequest(QListWidgetItem* item);
    // Tips:内置信号 void	 itemDoubleClicked(QListWidgetItem * item)

    // TODO: 和其他部件的交互

private:
    Ui::FileManager *ui;
    // 记录所要打开的文件的路径
    std::vector<File> m_fileList;
    // 右键菜单
    QMenu* m_menu;
    // 首先根据给定path路径寻找对应文件在m_fileList中的索引号，
    // 再根据id寻找索引号，若无则返回-1
    int m_FindIndexOfFile(const QString& path, const QString& id = nullptr) const;
    // 初始化菜单
    void m_initMenu();


    // 单体模式
    static FileManager* self;
};

#endif // FILEMANAGER_H
