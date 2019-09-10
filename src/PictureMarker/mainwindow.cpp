#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "savereminddialog.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QRegExp>
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>
#include <QPixmap>
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // qss样式表
    QFile qssFile(":/qss/Aqua");
    if (qssFile.open(QFile::ReadOnly)){
        QString qss = qssFile.readAll();
        setStyleSheet(qss);
        qssFile.close();
    }

    // connect 菜单栏
    connect(ui->action_AddFiles, SIGNAL(triggered()), ui->fileBox, SLOT(InputFiles()));
    connect(ui->action_AddFileFolder, SIGNAL(triggered()), ui->fileBox, SLOT(InputFolder()));
    //
    // action_Save函数体自动搜寻
    //
    connect(ui->action_Undo, SIGNAL(triggered()), ui->markerBox, SLOT(Undo()));
    connect(ui->action_Redo, SIGNAL(triggered()), ui->markerBox, SLOT(Redo()));


    // connect 文件列表
    connect(ui->fileBox, SIGNAL(LoadPictureRequest(QListWidgetItem *)),this, SLOT(LoadPicture(QListWidgetItem *)));

    // connect 标注列表
    connect(ui->markerBox, SIGNAL(UpdateMarkerToViewRequest()), this, SLOT(UpdateMarkerToView()));

    // 状态栏
    m_statusLabel = new QLabel(ui->statusBar);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setMinimumSize(m_statusLabel->sizeHint());
    ui->statusBar->addWidget(m_statusLabel);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
bool MainWindow::eventFilter(QObject *object, QEvent *event){
    if (ToolManager::Instance()->CurrentTool() == ToolType::Lens){
        if (event->type() == QEvent::MouseMove){
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            qDebug() << mouseEvent->x();
            QScreen* screen = QApplication::primaryScreen();
            int x, y, w, h;
            int pixel = ToolManager::LensPixel();
            int posx = mouseEvent->x();
            x =
            QPixmap screenMap = screen->grabWindow(winId());
        }
    }

    return QWidget::eventFilter(object, event);
}
*/

void MainWindow::LoadPicture(QListWidgetItem * item){
    // 清空标记
    ui->markerBox->Clear();

    // 清空画图区域
    m_InitView();
    QObjectList childList = ui->rightFrame->children();
    for (auto it = childList.begin();it != childList.end();it++)
        delete (*it);

    // 检测是否有内容
    if (item == nullptr) return;

    QString itemId = item->text();
    const File* file = ui->fileBox->FindFile(itemId);
    // 更改当前打开的文件
    ui->fileBox->m_nowOpenFile = file->path();
    ui->fileBox->m_nowOpenMarkerFile = m_AutoFindMarkerPath(file);
    if (file->fileType() == FileType::single){
        QVBoxLayout* layout = new QVBoxLayout(ui->rightFrame);
        view2D = new View2D(ui->rightFrame, m_statusLabel);
        view2D->SetLensLabel(ui->lensLabel);
        layout->addWidget(view2D);
        ui->rightFrame->setLayout(layout);

        ui->toolBox->Set3DStyle(false);

        view2D->LoadPicture(file);
        ui->markerBox->LoadMarker(ui->fileBox->m_nowOpenMarkerFile);
    }
    else if (file->fileType() == FileType::ct){
        // 3D情况的读取
        QVBoxLayout* layout = new QVBoxLayout(ui->rightFrame);
        view3D = new View3D(ui->rightFrame, m_statusLabel);
        view3D->SetLensLabel(ui->lensLabel);
        layout->addWidget(view3D);
        ui->rightFrame->setLayout(layout);

        ui->toolBox->Set3DStyle(true);

        view3D->LoadPicture(file);
        ui->markerBox->LoadMarker(ui->fileBox->m_nowOpenMarkerFile);
    }
}

void MainWindow::m_InitView(){
    view2D = nullptr;
    view3D = nullptr;
}

QString MainWindow::m_AutoFindMarkerPath(const File* file){
    QString path = file->path();
    QRegExp rx("(.*(/|\\\\))((.*)\\.(jpg|png|bmp))?", Qt::CaseInsensitive);
    rx.indexIn(path);
    QString folder = rx.cap(1);
    QString filename = rx.cap(3);
    if (filename.isEmpty()){
        return path + "/_auto.mrk";
    }
    else{
        return folder + rx.cap(4) + ".mrk";
    }
}

void MainWindow::UpdateMarkerToView(){
    if (view2D != nullptr)
        ui->markerBox->UpdateToView(view2D);
    else if (view3D != nullptr){
        ui->markerBox->UpdateToView(view3D);
    }
}

void MainWindow::on_action_Save_triggered()
{
    if (ui->fileBox->m_nowOpenMarkerFile.isEmpty()) return;
    ui->markerBox->Save(ui->fileBox->m_nowOpenMarkerFile);
}

void MainWindow::on_action_ClearFiles_triggered()
{
    if (ui->fileBox->FileSize() == 0) return;

    if (!ui->fileBox->m_nowOpenFile.isEmpty()){
        SaveRemindDialog saveDialog;
        saveDialog.exec();
        switch (saveDialog.Status()) {
        case SaveRemindStatus::Save:
            MarkerManager::Instance()->Save(ui->fileBox->m_nowOpenFile);
            break;
        case SaveRemindStatus::SaveTo:
            MarkerManager::Instance()->Save(saveDialog.Path());
            break;
        case SaveRemindStatus::Discard:
            break;
        case SaveRemindStatus::Cancel:
            return;
        }
    }

    ui->fileBox->ClearList();
    ui->markerBox->Clear();
    delete view2D;
    delete view3D;
    view2D = nullptr;
    view3D = nullptr;
    QObjectList list = ui->rightFrame->children();
    for (auto it = list.begin();it != list.end();it++)
        delete (*it);
    m_statusLabel->setText("");
}

void MainWindow::on_action_AddMarker_triggered()
{
    if (view2D == nullptr && view3D == nullptr)
        return;
    ui->markerBox->Clear();
    QString markerFile = QFileDialog::getOpenFileName(
                            this,
                            tr("Select marker files"),
                            ".",
                            "Marker Files (*.mrk)");
    ui->markerBox->LoadMarker(markerFile);
    ui->fileBox->m_nowOpenMarkerFile = markerFile;
}

void MainWindow::on_action_SaveTo_triggered()
{
    if (view2D == nullptr && view3D == nullptr)
        return;
    QString markerFile = QFileDialog::getSaveFileName(
                this,
                tr("Save markers to ..."),
                ".",
                "Marker Files (*.mrk)"
                );
    ui->markerBox->Save(markerFile);
    ui->fileBox->m_nowOpenMarkerFile = markerFile;
}

void MainWindow::on_FileDockWidget_visibilityChanged(bool visible)
{
    if (visible){
        ui->action_FileWindow->setChecked(true);
    }
    else{
        ui->action_FileWindow->setChecked(false);
    }
}

void MainWindow::on_MarkerDockWidget_visibilityChanged(bool visible)
{
    if (visible){
        ui->action_MarkerWindow->setChecked(true);
    }
    else{
        ui->action_MarkerWindow->setChecked(false);
    }
}

void MainWindow::on_toolDockWidget_visibilityChanged(bool visible)
{
    if (visible){
        ui->action_ToolWindow->setChecked(true);
    }
    else{
        ui->action_ToolWindow->setChecked(false);
    }
}

void MainWindow::on_lensDockWidget_visibilityChanged(bool visible)
{
    if (visible){
        ui->action_LensWindow->setChecked(true);
    }
    else{
        ui->action_LensWindow->setChecked(false);
    }
}

void MainWindow::on_action_FileWindow_triggered(bool checked)
{
    if (checked){
        ui->FileDockWidget->setVisible(true);
    }
    else{
        ui->FileDockWidget->setVisible(false);
    }
}

void MainWindow::on_action_MarkerWindow_triggered(bool checked)
{
    if (checked){
        ui->MarkerDockWidget->setVisible(true);
    }
    else{
        ui->MarkerDockWidget->setVisible(false);
    }
}

void MainWindow::on_action_ToolWindow_triggered(bool checked)
{
    if (checked){
        ui->toolDockWidget->setVisible(true);
    }
    else{
        ui->toolDockWidget->setVisible(false);
    }
}

void MainWindow::on_action_LensWindow_triggered(bool checked)
{
    if (checked){
        ui->lensDockWidget->setVisible(true);
    }
    else{
        ui->lensDockWidget->setVisible(false);
    }
}

void MainWindow::on_action_Author_triggered()
{
    QMessageBox::information(this, "Author", QString("Made by AyajiLin in THU on 09/09/2019.\n")
                             + QString("If you have any questiones, contact me on:\n")
                             + QString("https://github.com/leonardodalinky or\n")
                             + QString("link18thu@outlook.com"));
}
