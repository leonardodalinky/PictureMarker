#include "markermanager.h"
#include "ui_markermanager.h"
#include <QColorDialog>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QTableWidget>
#include "marker3drect.h"
#include "marker3ddrawingpad.h"
#include <QDebug>

MarkerManager* MarkerManager::self = nullptr;

MarkerManager::MarkerManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarkerManager)
{
    ui->setupUi(this);

    // 初始化菜单
    m_InitMenu();
    connect(ui->markerTableWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(ShowRMenu(QPoint)));

    // 单体模式
    if (self != this){
        delete  self;
        self = this;
    }
}

MarkerManager::~MarkerManager()
{
    delete ui;
}

MarkerManager* MarkerManager::Instance(){
    return self;
}

Marker *MarkerManager::SelectedMarker() const{
    if (m_nowSelectedIndex == -1)
        return nullptr;
    return m_markerList[m_nowSelectedIndex];
}

void MarkerManager::Clear(){
    auto it = m_markerList.begin();
    while (it != m_markerList.end()){
        delete (*it);
        it = m_markerList.erase(it);
    }
    m_markerList.clear();
    m_nowSelectedIndex = -1;

    ListUpdate();
}

void MarkerManager::Undo()
{
    m_history.Save(m_nowSelectedIndex, m_markerList, true);
    m_history.Load(m_nowSelectedIndex, m_markerList, false);

    if (m_nowSelectedIndex != -1){
        SetAllMarkerInvisible();
        m_markerList[m_nowSelectedIndex]->SetVisible(true);
    }

    // 更新工具箱
    ToolManager::Instance()->SetTool(ToolType::Mouse);

    ListUpdate();

    emit UpdateMarkerToViewRequest();
}

void MarkerManager::Redo()
{
    m_history.Load(m_nowSelectedIndex, m_markerList, true);

    if (m_nowSelectedIndex != -1){
        SetAllMarkerInvisible();
        m_markerList[m_nowSelectedIndex]->SetVisible(true);
    }

    // 更新工具箱
    ToolManager::Instance()->SetTool(ToolType::Mouse);

    ListUpdate();

    emit UpdateMarkerToViewRequest();
}

void MarkerManager::RMenuRename(){
    int currentIndex = ui->markerTableWidget->currentRow();
    if (currentIndex == -1)
        return;

    //QTableWidgetItem* currentItem = ui->markerTableWidget->currentItem();
    // 改名输入框
    bool ok;
    QString inputText = QInputDialog::getText(this, tr("Rename"),
                                         tr("Input new name:"), QLineEdit::Normal,
                                         m_markerList[currentIndex]->Id(), &ok);
    if (ok && !inputText.isEmpty()){
        // 检测有无重复Id
        for (auto it = m_markerList.begin();it != m_markerList.end();it++){
            if ((*it)->Id() == inputText){
                QMessageBox::warning(this, "Rename Error", "The name has been existed!");
                return;
            }
        }

        // record
        m_history.Save(m_nowSelectedIndex, m_markerList, false);

        m_markerList[static_cast<int>(currentIndex)]->SetId(inputText);
        ListUpdate();
    }
}

void MarkerManager::ShowRMenu(QPoint p){
    m_menu->exec(this->mapToGlobal(p));
}

void MarkerManager::RMenuDelete(){
    int currentIndex = ui->markerTableWidget->currentRow();
    ui->markerTableWidget->removeRow(currentIndex);
    if (currentIndex == -1)
        return;

    // record
    m_history.Save(m_nowSelectedIndex, m_markerList, false);

    auto it = m_markerList.begin();
    for (int i = 0;i < currentIndex;i++)
        it++;
    delete m_markerList[currentIndex];
    m_markerList.erase(it);
    m_nowSelectedIndex = -1;

    ListUpdate();
    emit UpdateMarkerToViewRequest();
}

void MarkerManager::RMenuRecolor(){
    int currentIndex = ui->markerTableWidget->currentRow();
    QColor color = QColorDialog::getColor(Qt::black, this, "Select Color");

    if (color.isValid()){
        // record
        m_history.Save(m_nowSelectedIndex, m_markerList, false);

        m_markerList[currentIndex]->SetColor(color);
        ListUpdate();
        emit UpdateMarkerToViewRequest();
    }
}

void MarkerManager::ListUpdate(){
    ui->markerTableWidget->clear();

    // 更新头
    QStringList header;
    header << "Id" << "Type";
    ui->markerTableWidget->setHorizontalHeaderLabels(header);

    int i = 0;
    ui->markerTableWidget->setRowCount(m_markerList.size());
    for (auto it = m_markerList.begin();it != m_markerList.end();it++){
        QTableWidgetItem* itemId = new QTableWidgetItem();
        QTableWidgetItem* itemType = new QTableWidgetItem();
        itemId->setText((*it)->Id());
        itemId->setTextColor((*it)->Color());
        itemType->setText((*it)->TypeName());
        itemType->setTextColor((*it)->Color());
        ui->markerTableWidget->setItem(i, 0, itemId);
        ui->markerTableWidget->setItem(i, 1, itemType);
    }
}

void MarkerManager::LoadMarker(const QString& markerPath){
    QFile file(markerPath);
    file.open(QIODevice::ReadOnly);
    // 无法打开文件
    if (file.error() != QFileDevice::NoError) return;
    QByteArray byteArray = file.readAll();
    file.close();

    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(byteArray, &jsonError));

    if (jsonError.error != QJsonParseError::NoError){
        // TODO: 读取出错后
    }

    // 初始化历史表
    m_history.Initiate();

    if (jsonDoc.isArray()){
        QJsonArray jsonArray = jsonDoc.array();
        for (int i = 0;i < jsonArray.size();i++){
            QJsonObject jsonObj = jsonArray.at(i).toObject();
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

            m_markerList.append(newMarker);
        }
    }
    ListUpdate();
}

void MarkerManager::Save(const QString& markerPath){
    QFile file(markerPath);
    file.open(QIODevice::WriteOnly);

    QJsonDocument jsonDoc;
    QJsonArray jsonArray;
    for (auto it = m_markerList.begin();it != m_markerList.end();it++){
        QJsonObject jsonObj = (*it)->Serialize();
        jsonArray.append(jsonObj);
    }
    jsonDoc.setArray(jsonArray);

    file.write(jsonDoc.toJson());
    file.close();
}

void MarkerManager::SetAllMarkerInvisible(){
    for (auto it = m_markerList.begin();it != m_markerList.end();it++){
        (*it)->SetVisible(false);
    }
    emit UpdateMarkerToViewRequest();
}

void MarkerManager::UpdateToView(View2D *view2d){
    view2d->ClearMarker();
    
    for (auto it = m_markerList.begin();it != m_markerList.end();it++){
        if ((*it)->Visible()){
            view2d->AddMarker((*it));
        }
    }
}

void MarkerManager::UpdateToView(View3D *view3d){
    for (int i = static_cast<int>(View3DWindows::Top);i <= static_cast<int>(View3DWindows::Left);i++){
        view3d->ClearMarker(static_cast<View3DWindows>(i));
    }

    for (auto it = m_markerList.begin();it != m_markerList.end();it++){
        if ((*it)->Visible()){
            for (int i = static_cast<int>(View3DWindows::Top);i <= static_cast<int>(View3DWindows::Left);i++){
                view3d->AddMarker3D(static_cast<Marker3D*>((*it)), static_cast<View3DWindows>(i));
            }
        }
    }
}

void MarkerManager::on_markerTableWidget_itemDoubleClicked(QTableWidgetItem *item){
    // 存入历史
    m_history.Save(m_nowSelectedIndex, m_markerList, false);

    m_nowSelectedIndex = ui->markerTableWidget->currentRow();
    SetAllMarkerInvisible();
    m_markerList[m_nowSelectedIndex]->SetVisible(true);

    // 更新工具箱
    ToolManager::Instance()->SetTool(ToolType::Mouse);

    emit UpdateMarkerToViewRequest();
}

void MarkerManager::m_InitMenu(){
    m_menu = new QMenu(this);

    QAction *actionRename = new QAction(m_menu);
    actionRename->setText(tr("Rename"));
    //actionRename->setIcon();
    m_menu->addAction(actionRename);
    connect(actionRename,SIGNAL(triggered()), this, SLOT(RMenuRename()));

    QAction *actionRecolor = new QAction(m_menu);
    actionRecolor->setText(tr("Recolor"));
    //actionRename->setIcon();
    m_menu->addAction(actionRecolor);
    connect(actionRecolor,SIGNAL(triggered()), this, SLOT(RMenuRecolor()));

    QAction *actionDelete = new QAction(m_menu);
    actionDelete->setText(tr("Delete"));
    //actionRename->setIcon();
    m_menu->addAction(actionDelete);
    connect(actionDelete,SIGNAL(triggered()), this, SLOT(RMenuDelete()));
}

QString MarkerManager::m_IdGen(){
    int i = 0;
    bool isFind = false;
    while (!isFind){
        QString str = QString::number(i);
        auto it = m_markerList.begin();
        for ( ;it != m_markerList.end();it++){
            if ((*it)->Id() == str){
                i++;
                break;
            }
        }
        if (it == m_markerList.end()){
            isFind = true;
        }
    }
    return QString::number(i);
}

void MarkerManager::CreateMarker(MarkerType type, qreal w, qreal h, qreal l, View3DWindows mainView){
    // 存入历史
    m_history.Save(m_nowSelectedIndex, m_markerList, false);

    Marker* m;
    switch (type) {
    case MarkerType::Rect:
        m = new MarkerRect(QPointF(0, 0), w / 2, h / 2);
        break;
    case MarkerType::Circle:
        m = new MarkerCircle(QPointF(w / 3, w / 3), w / 5);
        break;
    case MarkerType::DrawingPad:
        m = new MarkerDrawingPad(w, h);
        break;
    case MarkerType::Rect3D:
        m = new Marker3DRect(0, 0, 0, l / 2, w / 2, h / 2);
        break;
    case MarkerType::DrawingPad3D:
        m = new Marker3DDrawingPad(mainView, l, w, h);
    }
    QString idStr = m_IdGen();
    m->SetId(idStr);
    m_markerList.append(m);
    ListUpdate();
}
