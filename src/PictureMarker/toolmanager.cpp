#include "toolmanager.h"
#include "ui_toolmanager.h"
#include "markermanager.h"
#include <QToolButton>
#include "graphicspdrawingpaditem.h"

using namespace LinkGraphicsItem;

ToolType ToolManager::m_currentTool = ToolType::Mouse;
int ToolManager::m_currentIndex = 0;
ToolManager* ToolManager::self = nullptr;
int ToolManager::m_lensPixel = 20;

ToolManager::ToolManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolManager)
{
    ui->setupUi(this);

    // 单体模式
    if (self != this){
        delete  self;
        self = this;
    }

    ui->Mouse->setChecked(true);

}

ToolManager::~ToolManager()
{
    delete ui;
}

int ToolManager::CurrentIndex(){
    return m_currentIndex;
}

ToolType ToolManager::CurrentTool(){
    return m_currentTool;
}

int ToolManager::LensPixel()
{
    return m_lensPixel;
}

void ToolManager::SetTool(ToolType type){
    m_SetAllOff();
    m_currentTool = type;
    m_currentIndex = static_cast<int>(type);

    QObjectList objList = ui->groupBox->children();
    for (auto it = objList.begin();it != objList.end();it++){
        auto toolbtn = qobject_cast<QToolButton*>(*it);
        if (toolbtn != nullptr && toolbtn->text() == QString(static_cast<int>(type))){
            toolbtn->setChecked(true);
        }
    }
}

void ToolManager::Set3DStyle(bool is3D)
{
    if (!is3D){
        ui->Circle->setEnabled(true);
    }
    else{
        ui->Circle->setEnabled(false);
    }
}

void ToolManager::m_SetAllOff(){
    QObjectList objList = ui->groupBox->children();
    for (auto it = objList.begin();it != objList.end();it++){
        auto toolbtn = qobject_cast<QToolButton*>(*it);
        if (toolbtn != nullptr){
            toolbtn->setChecked(false);
        }
    }
}

void ToolManager::on_Mouse_clicked()
{
    m_currentIndex = static_cast<int>(ToolType::Mouse);
    m_currentTool = ToolType::Mouse;
    m_SetAllOff();
    ui->Mouse->setChecked(true);
}

void ToolManager::on_Square_clicked()
{
    m_currentIndex = static_cast<int>(ToolType::Square);
    m_currentTool = ToolType::Square;
    m_SetAllOff();
    ui->Square->setChecked(true);
}

void ToolManager::on_Brush_clicked()
{
    m_currentIndex = static_cast<int>(ToolType::Brush);
    m_currentTool = ToolType::Brush;
    m_SetAllOff();
    ui->Brush->setChecked(true);
}

void ToolManager::on_Circle_clicked()
{
    m_currentIndex = static_cast<int>(ToolType::Circle);
    m_currentTool = ToolType::Circle;
    m_SetAllOff();
    ui->Circle->setChecked(true);
}

void ToolManager::on_Draw_clicked()
{
    m_currentIndex = static_cast<int>(ToolType::Draw);
    m_currentTool = ToolType::Draw;
    m_SetAllOff();
    ui->Draw->setChecked(true);

    // only for DrawingPad
    /*
    Marker* mrk = MarkerManager::Instance()->SelectedMarker();
    if (mrk != nullptr && mrk->Type() == MarkerType::DrawingPad){
            static_cast<MarkerDrawingPad*>(mrk)->SetStatus(DrawingPadStatus::Drawing);
    }
    */
}

void ToolManager::on_Erase_clicked()
{
    m_currentIndex = static_cast<int>(ToolType::Erase);
    m_currentTool = ToolType::Erase;
    m_SetAllOff();
    ui->Erase->setChecked(true);

    // only for DrawingPad
    /*
    Marker* mrk = MarkerManager::Instance()->SelectedMarker();
    if (mrk != nullptr && mrk->Type() == MarkerType::DrawingPad){
            static_cast<MarkerDrawingPad*>(mrk)->SetStatus(DrawingPadStatus::Erasing);
    }
    */
}

ToolManager* ToolManager::Instance(){
    return self;
}

void ToolManager::on_ZoomIn_clicked()
{
    m_currentIndex = static_cast<int>(ToolType::ZoomIn);
    m_currentTool = ToolType::ZoomIn;
    m_SetAllOff();
    ui->ZoomIn->setChecked(true);
}

void ToolManager::on_ZoomOut_clicked()
{
    m_currentIndex = static_cast<int>(ToolType::ZoomOut);
    m_currentTool = ToolType::ZoomOut;
    m_SetAllOff();
    ui->ZoomOut->setChecked(true);
}

void ToolManager::on_Lens_clicked()
{
    m_currentIndex = static_cast<int>(ToolType::Lens);
    m_currentTool = ToolType::Lens;
    m_SetAllOff();
    ui->Lens->setChecked(true);
}
