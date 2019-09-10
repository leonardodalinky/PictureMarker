#include "view2d.h"
#include "ui_view2d.h"

#include <QDebug>

#include <QImage>
#include <QPixmap>
#include <QScreen>
#include <QCursor>

#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>

#include "graphicsrectitem.h"
#include "graphicscircleitem.h"
#include "graphicspdrawingpaditem.h"
using namespace LinkGraphicsItem;
#include "marker.h"
#include "markermanager.h"

View2D::View2D(QWidget *parent, QLabel* statusLabel) :
    QWidget(parent),
    ui(new Ui::View2D)
{
    ui->setupUi(this);


    // initiate
    m_scene = new QGraphicsScene(this);
    m_scene->setBackgroundBrush(Qt::gray);
    ui->graphicsView->setScene(m_scene);
    m_backgroundGroup = new QGraphicsItemGroup();
    m_markerGroup = new QGraphicsItemGroup();
    m_scene->addItem(m_backgroundGroup);
    m_scene->addItem(m_markerGroup);

    // view允许hover
    ui->graphicsView->setMouseTracking(true);

    // 工具箱的eventfilter
    ui->graphicsView->installEventFilter(this);

    // 状态栏
    m_statusLabel = statusLabel;
    m_scene->installEventFilter(this);
}

View2D::~View2D()
{
    delete m_backgroundGroup;
    delete m_markerGroup;
    delete ui;
}

void View2D::SetLensLabel(QLabel *label)
{
    m_lensLabel = label;
}

bool View2D::eventFilter(QObject *object, QEvent *event){
    /*
    if (object == ui->graphicsView){
        if (event->type() == QEvent::MouseButtonPress){
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton){
                qreal m;
                switch (ToolManager::CurrentTool()) {
                case ToolType::Mouse:
                    break;
                case ToolType::Square:
                    MarkerManager::Instance()->CreateMarker(MarkerType::Rect,
                                                            m_backgroundWidth, m_backgroundHeight);
                    ToolManager::Instance()->SetTool(ToolType::Mouse);
                    break;
                case ToolType::Brush:
                    MarkerManager::Instance()->CreateMarker(MarkerType::DrawingPad,
                                                            m_backgroundWidth, m_backgroundHeight);
                    ToolManager::Instance()->SetTool(ToolType::Mouse);
                    break;
                case ToolType::Circle:
                    m = (m_backgroundWidth < m_backgroundHeight)? m_backgroundWidth : m_backgroundHeight;
                    MarkerManager::Instance()->CreateMarker(MarkerType::Circle,
                                                            m_backgroundWidth, m_backgroundHeight);
                    ToolManager::Instance()->SetTool(ToolType::Mouse);
                    break;
                case ToolType::Draw:
                    mrk = MarkerManager::Instance()->SelectedMarker();
                    if (mrk != nullptr && mrk->Type() == MarkerType::DrawingPad){
                            static_cast<MarkerDrawingPad*>(mrk)->SetStatus(DrawingPadStatus::Drawing);
                    }
                    break;
                case ToolType::Erase:
                    mrk = MarkerManager::Instance()->SelectedMarker();
                    if (mrk != nullptr && mrk->Type() == MarkerType::DrawingPad){
                            static_cast<MarkerDrawingPad*>(mrk)->SetStatus(DrawingPadStatus::Erasing);
                    }
                    break;
                case ToolType::ZoomIn:
                    ZoomIn();
                    break;
                case ToolType::ZoomOut:
                    ZoomOut();
                    break;
                }
            }
        }
    }
    */
    if (object == m_scene){
        if (event != nullptr &&
                (event->type() == QEvent::GraphicsSceneMouseMove || event->type() == QEvent::GraphicsSceneMousePress)){
            QGraphicsSceneMouseEvent* mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);

            // 状态栏
            QPointF p = mouseEvent->scenePos();
            m_statusLabel->setText("X:" + QString::number(p.x()) + " Y:" + QString::number(p.y()));

            // 放大镜
            if (ToolManager::CurrentTool() == ToolType::Lens && m_lensLabel != nullptr){
                QScreen* screen = QApplication::primaryScreen();
                int x = QCursor::pos().x();
                int y = QCursor::pos().y();
                int pixel = ToolManager::LensPixel();
                QPixmap map = screen->grabWindow(0, x - pixel, y - pixel, 2 * pixel, 2 * pixel);
                m_lensLabel->setPixmap(map.scaled(m_lensLabel->width(), m_lensLabel->height(), Qt::KeepAspectRatio));
            }

            // 画板
            /*
            if (ToolManager::CurrentTool() == ToolType::Draw){
                Marker* mrk = MarkerManager::Instance()->SelectedMarker();
                if (mrk != nullptr && mrk->Type() == MarkerType::DrawingPad){
                        static_cast<MarkerDrawingPad*>(mrk)->SetStatus(DrawingPadStatus::Drawing);
                }
            }
            else if (ToolManager::CurrentTool() == ToolType::Erase){
                Marker* mrk = MarkerManager::Instance()->SelectedMarker();
                if (mrk != nullptr && mrk->Type() == MarkerType::DrawingPad){
                        static_cast<MarkerDrawingPad*>(mrk)->SetStatus(DrawingPadStatus::Erasing);
                }
            }
            */
        }
        if (event != nullptr && event->type() == QEvent::GraphicsSceneMousePress){
            QGraphicsSceneMouseEvent* mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton){
                qreal m;
                Marker* mrk;
                switch (ToolManager::CurrentTool()) {
                case ToolType::Mouse:
                    break;
                case ToolType::Square:
                    MarkerManager::Instance()->CreateMarker(MarkerType::Rect,
                                                            m_backgroundWidth, m_backgroundHeight);
                    ToolManager::Instance()->SetTool(ToolType::Mouse);
                    break;
                case ToolType::Brush:
                    MarkerManager::Instance()->CreateMarker(MarkerType::DrawingPad,
                                                            m_backgroundWidth, m_backgroundHeight);
                    ToolManager::Instance()->SetTool(ToolType::Mouse);
                    break;
                case ToolType::Circle:
                    m = (m_backgroundWidth < m_backgroundHeight)? m_backgroundWidth : m_backgroundHeight;
                    MarkerManager::Instance()->CreateMarker(MarkerType::Circle,
                                                            m_backgroundWidth, m_backgroundHeight);
                    ToolManager::Instance()->SetTool(ToolType::Mouse);
                    break;
                case ToolType::Draw:
                    mrk = MarkerManager::Instance()->SelectedMarker();
                    if (mrk != nullptr && mrk->Type() == MarkerType::DrawingPad){
                            static_cast<MarkerDrawingPad*>(mrk)->SetStatus(DrawingPadStatus::Drawing);
                    }
                    break;
                case ToolType::Erase:
                    mrk = MarkerManager::Instance()->SelectedMarker();
                    if (mrk != nullptr && mrk->Type() == MarkerType::DrawingPad){
                            static_cast<MarkerDrawingPad*>(mrk)->SetStatus(DrawingPadStatus::Erasing);
                    }
                    break;
                case ToolType::ZoomIn:
                    ZoomIn();
                    break;
                case ToolType::ZoomOut:
                    ZoomOut();
                    break;
                }
            }
        }
        // 滚轮放大缩小
        else if (event->type() == QEvent::GraphicsSceneWheel){
            QGraphicsSceneWheelEvent* wheelEvent = static_cast<QGraphicsSceneWheelEvent*>(event);
            if (QApplication::keyboardModifiers() == Qt::ControlModifier){
                if (wheelEvent->delta() > 0){
                    ZoomIn();
                }
                else{
                    ZoomOut();
                }
            }
        }
    }

    return QWidget::eventFilter(object, event);
}

void View2D::ZoomIn(){
    if (m_zoom == 12)
        return;
    m_zoom++;
    ui->graphicsView->scale(1.25, 1.25);
}

void View2D::ZoomOut(){
    if (m_zoom == 0)
        return;
    m_zoom--;
    ui->graphicsView->scale(0.8, 0.8);
}

void View2D::LoadPicture(const File* file){
    // initiate
    m_InitMarkerGroup();
    m_InitBackgroundGroup();

    // 加载背景图
    QImage img(file->path());
    QGraphicsPixmapItem* picItem = new QGraphicsPixmapItem(QPixmap::fromImage(img), m_backgroundGroup);
    picItem->setPos(0,0);
    m_backgroundWidth = img.width();
    m_backgroundHeight = img.height();
}

void View2D::m_InitMarkerGroup(){
    QList<QGraphicsItem*> itemList = m_markerGroup->childItems();
    for (auto it = itemList.begin();it != itemList.end();it++){
        delete *it;
    }
}

void View2D::m_InitBackgroundGroup(){
    QList<QGraphicsItem*> itemList = m_backgroundGroup->childItems();
    for (auto it = itemList.begin();it != itemList.end();it++){
        delete *it;
    }
}

void View2D::AddMarker(Marker* marker){
    marker->AddToItemGroup(m_markerGroup);
}

void View2D::ClearMarker(){
    QList<QGraphicsItem*> itemList;
    itemList.append(m_markerGroup->childItems());
    for (auto it = itemList.begin();it != itemList.end();it++){
        m_markerGroup->removeFromGroup(*it);
        m_scene->removeItem(*it);
    }
}
