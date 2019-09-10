#include "filemanager.h"
#include "ui_filemanager.h"
#include "markermanager.h"
#include "savereminddialog.h"
#include "file.h"
#include <QFileDialog>
#include <QDir>
#include <QLineEdit>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

FileManager* FileManager::self = nullptr;

FileManager::FileManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileManager)
{
    ui->setupUi(this);

    // 单体模式
    if (self != this){
        delete  self;
        self = this;
    }

    // 初始化菜单
    m_initMenu();
    connect(ui->fileListWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(ShowRMenu(QPoint)));

    m_fileList.clear();
    connect(this,SIGNAL(Update()), this, SLOT(Refresh()));

}

FileManager::~FileManager()
{
    delete ui;
}

void FileManager::InputFiles(){
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            tr("Select files"),
                            ".",
                            "Images (*.png *.jpg);;Bitmap (*.bmp);;GIF (*.gif)");

    for (auto it = files.begin();it != files.end();it++){
        File newFile(*it,FileType::single);
        if (m_FindIndexOfFile(*it) == -1){
            m_fileList.push_back(newFile);
        }
    }

    emit Update();
}

void FileManager::InputFolder(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select CT folder"),
                                                    ".",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    File newFile(dir, FileType::ct);
    if (m_FindIndexOfFile(dir) == -1){
        m_fileList.push_back(newFile);
    }

    /*
    // 文件拓展名过滤
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    QDir d(dir);
    d.setNameFilters(filters);
    */

    emit Update();
}

void FileManager::ClearList(){
    m_fileList.clear();
    m_nowOpenFile = "";
    m_nowOpenMarkerFile = "";
    emit Update();
}

void FileManager::Refresh(){
    ui->fileListWidget->clear();
    for (auto it = m_fileList.begin();it != m_fileList.end();it++){
        ui->fileListWidget->addItem((*it).id());
    }
}

int FileManager::m_FindIndexOfFile(const QString& path, const QString& id) const{
    int i = 0;
    for (auto it = m_fileList.begin();it != m_fileList.end();it++){
        if ((*it).path() == path || (*it).id() == id)
            return i;
        else {
            i++;
        }
    }
    return -1;
}


void FileManager::m_initMenu(){
    m_menu = new QMenu(this);

    QAction *actionRename = new QAction(m_menu);
    actionRename->setText(tr("Rename"));
    //actionRename->setIcon();
    m_menu->addAction(actionRename);
    connect(actionRename,SIGNAL(triggered()), this, SLOT(RMenuRename()));

    QAction *actionDelete = new QAction(m_menu);
    actionDelete->setText(tr("Delete"));
    //actionRename->setIcon();
    m_menu->addAction(actionDelete);
    connect(actionDelete,SIGNAL(triggered()), this, SLOT(RMenuDelete()));
}

void FileManager::ShowRMenu(QPoint p){
    m_menu->exec(this->mapToGlobal(p));
}

void FileManager::RMenuRename(){
    int currentIndex = ui->fileListWidget->currentRow();
    if (currentIndex == -1)
        return;

    QListWidgetItem* currentItem = ui->fileListWidget->currentItem();
    // 改名输入框
    bool ok;
    QString inputText = QInputDialog::getText(this, tr("Rename"),
                                         tr("Input new name:"), QLineEdit::Normal,
                                         currentItem->text(), &ok);
    if (ok && !inputText.isEmpty()){
        // 检测有无重复Id
        for (auto it = m_fileList.begin();it != m_fileList.end();it++){
            if ((*it).id() == inputText){
                QMessageBox::warning(this, "Rename Error", "The name has been existed!");
                return;
            }
        }

        m_fileList[static_cast<size_t>(currentIndex)].setId(inputText);
        emit Update();
    }
}

void FileManager::RMenuDelete(){
    int currentIndex = ui->fileListWidget->currentRow();
    if (currentIndex == -1)
        return;


    auto it = m_fileList.begin();
    for (int i = 0;i < currentIndex;i++)
        it++;

    // 如果删除现在打开的文件，要清空标注列表，还要关闭视图
    if ((*it).path() == m_nowOpenFile){
        if (SaveRemind())
            MarkerManager::Instance()->Clear();
        else return;

        m_nowOpenFile = "";
        m_nowOpenMarkerFile = "";
        emit LoadPictureRequest(nullptr);
    }
    m_fileList.erase(it);

    emit Update();
}

QString FileManager::FindPathOfFile(const QString& id) const{
    int index = m_FindIndexOfFile(nullptr, id);
    return m_fileList[static_cast<size_t>(index)].path();
}

const File* FileManager::FindFile(const QString& id) const{
    int index = m_FindIndexOfFile(nullptr, id);
    return &(m_fileList[static_cast<size_t>(index)]);
}

bool FileManager::SaveRemind()
{
    if (!m_nowOpenFile.isEmpty() && !m_nowOpenMarkerFile.isEmpty()){
        SaveRemindDialog dialog;
        dialog.exec();
        SaveRemindStatus s = dialog.Status();
        switch (s) {
        case SaveRemindStatus::Save:
            MarkerManager::Instance()->Save(m_nowOpenMarkerFile);
            return true;;
        case SaveRemindStatus::SaveTo:
            MarkerManager::Instance()->Save(dialog.Path());
            return true;
        case SaveRemindStatus::Discard:
            return true;
        case SaveRemindStatus::Cancel:
            return false;
        }
    }
}

void FileManager::on_fileListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (SaveRemind())
        emit LoadPictureRequest(item);
}

FileManager* FileManager::Instance(){
    return self;
}

int FileManager::FileSize() const
{
    return m_fileList.size();
}
