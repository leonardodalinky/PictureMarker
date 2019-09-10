#include "view3d.h"
#include "ui_view3d.h"
#include <QDir>
#include <QScreen>
#include <QGraphicsSceneEvent>
#include "markermanager.h"
#include "graphicspdrawingpaditem.h"
#include "marker3ddrawingpad.h"
#include "graphicspdrawingpaditem.h"
#include <QDebug>

View3D::View3D(QWidget *parent, QLabel* statusLabel) :
    QWidget(parent),
    ui(new Ui::View3D)
{
    ui->setupUi(this);

    // initiate
    m_Initiate();
    m_statusLabel = statusLabel;

    // view允许hover
    ui->viewTop->setMouseTracking(true);
    ui->viewFront->setMouseTracking(true);
    ui->viewLeft->setMouseTracking(true);

    // 工具栏与view的互动
    ui->viewTop->installEventFilter(this);
    ui->viewFront->installEventFilter(this);
    ui->viewLeft->installEventFilter(this);

    // 状态栏与view的互动
    m_sceneTop->installEventFilter(this);
    m_sceneFront->installEventFilter(this);
    m_sceneLeft->installEventFilter(this);

}

View3D::~View3D()
{
    delete ui;
    delete m_rgbTop;
    delete m_rgbFront;
    delete m_rgbLeft;
}

void View3D::SetLensLabel(QLabel *label)
{
    m_lensLabel = label;
}

bool View3D::eventFilter(QObject *object, QEvent *event)
{
    View3DWindows nowView;
    if (object == ui->viewTop || object == m_sceneTop) nowView = View3DWindows::Top;
    else if (object == ui->viewFront || object == m_sceneFront) nowView = View3DWindows::Front;
    else if (object == ui->viewLeft || object == m_sceneLeft) nowView = View3DWindows::Left;

    // 三个主场景的事件过滤
    if (object == m_sceneTop || object == m_sceneFront || object == m_sceneLeft){
        if (event->type() == QEvent::GraphicsSceneMouseMove){
            QGraphicsSceneMouseEvent* mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);
            // 状态栏分三个窗口
            if (object == m_sceneTop){
                m_statusLabel->setText(tr("View:Top X:%1 Y:%2")
                                       .arg(mouseEvent->scenePos().x()).arg(mouseEvent->scenePos().y()));
            }
            else if (object == m_sceneFront){
                m_statusLabel->setText(tr("View:Front X:%1 Z:%2")
                                       .arg(mouseEvent->scenePos().x()).arg(mouseEvent->scenePos().y()));
            }
            else if (object == m_sceneLeft){
                m_statusLabel->setText(tr("View:Left Y:%1 Z:%2")
                                       .arg(mouseEvent->scenePos().x()).arg(mouseEvent->scenePos().y()));
            }

            // 放大镜
            if (ToolManager::CurrentTool() == ToolType::Lens && m_lensLabel != nullptr){
                QScreen* screen = QApplication::primaryScreen();
                int x = QCursor::pos().x();
                int y = QCursor::pos().y();
                int pixel = ToolManager::LensPixel();
                QPixmap map = screen->grabWindow(0, x - pixel, y - pixel, 2 * pixel, 2 * pixel);
                m_lensLabel->setPixmap(map.scaled(m_lensLabel->width(), m_lensLabel->height(), Qt::KeepAspectRatio));
            }
        }
        else if (event->type() == QEvent::GraphicsSceneMousePress){
            // 是否可画
            if (ToolManager::CurrentTool() == ToolType::Draw){
                Marker* mrk = MarkerManager::Instance()->SelectedMarker();
                if (mrk != nullptr && mrk->Type() == MarkerType::DrawingPad3D){
                        static_cast<Marker3DDrawingPad*>(mrk)->SetStatus(LinkGraphicsItem::DrawingPadStatus::Drawing);
                }
            }
            else if (ToolManager::CurrentTool() == ToolType::Erase){
                Marker* mrk = MarkerManager::Instance()->SelectedMarker();
                if (mrk != nullptr && mrk->Type() == MarkerType::DrawingPad3D){
                        static_cast<Marker3DDrawingPad*>(mrk)->SetStatus(LinkGraphicsItem::DrawingPadStatus::Erasing);
                }
            }

            // 分割标注画板
            if (MarkerManager::Instance()->SelectedMarker() != nullptr &&
                    MarkerManager::Instance()->SelectedMarker()->Type() == MarkerType::DrawingPad3D){
                Marker3DDrawingPad* mrk = static_cast<Marker3DDrawingPad*>(MarkerManager::Instance()->SelectedMarker());
                if (mrk->MainView() != nowView) return QWidget::eventFilter(object, event);
                // 主视角的层数
                int nowIndex;
                switch (nowView) {
                case View3DWindows::Top:
                    nowIndex = (int)m_heightIndex;
                    break;
                case View3DWindows::Front:
                    nowIndex = (int)m_widthIndex;
                    break;
                case View3DWindows::Left:
                    nowIndex = (int)m_lengthIndex;
                    break;
                }

                if (mrk->Contains(nowIndex)) return QWidget::eventFilter(object, event);
                else if (!mrk->Contains(nowIndex) && ToolManager::CurrentTool() == ToolType::Draw){
                    mrk->UpdateFromView(nowView);
                    mrk->AddPad(nowIndex);
                    mrk->UpdateToView(nowView);
                }
            }

            // 工具栏交互
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
                            static_cast<MarkerDrawingPad*>(mrk)->SetStatus(LinkGraphicsItem::DrawingPadStatus::Drawing);
                    }
                    break;
                case ToolType::Erase:
                    mrk = MarkerManager::Instance()->SelectedMarker();
                    if (mrk != nullptr && mrk->Type() == MarkerType::DrawingPad){
                            static_cast<MarkerDrawingPad*>(mrk)->SetStatus(LinkGraphicsItem::DrawingPadStatus::Erasing);
                    }
                    break;
                case ToolType::ZoomIn:
                    ZoomIn(nowView);
                    break;
                case ToolType::ZoomOut:
                    ZoomOut(nowView);
                    break;
                }
            }
        }
        // 滚轮放大缩小
        else if (event->type() == QEvent::GraphicsSceneWheel){
            QGraphicsSceneWheelEvent* wheelEvent = static_cast<QGraphicsSceneWheelEvent*>(event);
            if (QApplication::keyboardModifiers() == Qt::ControlModifier){
                if (wheelEvent->delta() > 0){
                    ZoomIn(nowView);
                }
                else{
                    ZoomOut(nowView);
                }
            }
        }
    }

    return QWidget::eventFilter(object, event);
}

void View3D::LoadPicture(const File *file)
{
    // initiate
    for (int i = static_cast<int>(View3DWindows::Top);i <= static_cast<int>(View3DWindows::Left);i++){
        m_InitMarkerGroup(static_cast<View3DWindows>(i));
        m_InitBackgroundGroup(static_cast<View3DWindows>(i));
    }

    // 确保读入文件夹
    if (file->fileType() != FileType::ct)
        return;

    QDir dir(file->path());
    QStringList nameFilter;
    nameFilter << "*.jpg" << "*.png" << ".bmp";
    dir.setNameFilters(nameFilter);
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);

    // 读取图片
    QFileInfoList fileInfoList = dir.entryInfoList();
    //m_images = new QImage [fileInfoList.size()];
    //m_rgbs = new const QRgb* [fileInfoList.size()];
    for (int i = 0;i < fileInfoList.size();i++){
        QImage img(fileInfoList[i].filePath());
        img = img.convertToFormat(QImage::Format_ARGB32);
        m_images.append(img);
        m_rgbs.append((const QRgb*)(m_images[i].constBits()));
    }

    // 更新长宽高
    m_backgroundLength = m_images[0].width();
    m_backgroundWidth = m_images[0].height();
    m_backgroundHeight = fileInfoList.size();
    m_rgbTop = new QRgb [m_backgroundLength * m_backgroundWidth];
    m_rgbFront = new QRgb [m_backgroundLength * m_backgroundHeight];
    m_rgbLeft = new QRgb [m_backgroundWidth * m_backgroundHeight];

    // 更新滑条状态
    ui->viewTopSlider->setRange(0, static_cast<int>(m_backgroundHeight) - 1);
    ui->viewFrontSlider->setRange(0, static_cast<int>(m_backgroundWidth) - 1);
    ui->viewLeftSlider->setRange(0, static_cast<int>(m_backgroundLength) - 1);

    for (int i = static_cast<int>(View3DWindows::Top);i <= static_cast<int>(View3DWindows::Left);i++){
        UpdateViewBackground(static_cast<View3DWindows>(i));
    }
}

void View3D::AddMarker3D(Marker3D *marker, View3DWindows view)
{
    switch (view) {
    case View3DWindows::Top:
        marker->AddToItemGroup(m_markerGroupTop, view);
        break;
    case View3DWindows::Front:
        marker->AddToItemGroup(m_markerGroupFront, view);
        break;
    case View3DWindows::Left:
        marker->AddToItemGroup(m_markerGroupLeft, view);
        break;
    }
}

void View3D::ClearMarker(View3DWindows view)
{
    QList<QGraphicsItem*> itemList;
    switch (view) {
    case View3DWindows::Top:
        itemList.append(m_markerGroupTop->childItems());
        for (auto it = itemList.begin();it != itemList.end();it++){
            m_markerGroupTop->removeFromGroup(*it);
            m_sceneTop->removeItem(*it);
        }
        break;
    case View3DWindows::Front:
        itemList.append(m_markerGroupFront->childItems());
        for (auto it = itemList.begin();it != itemList.end();it++){
            m_markerGroupFront->removeFromGroup(*it);
            m_sceneFront->removeItem(*it);
        }
        break;
    case View3DWindows::Left:
        itemList.append(m_markerGroupLeft->childItems());
        for (auto it = itemList.begin();it != itemList.end();it++){
            m_markerGroupLeft->removeFromGroup(*it);
            m_sceneLeft->removeItem(*it);
        }
        break;
    }
}

void View3D::UpdateViewBackground(View3DWindows view)
{
    // 更新rgb
    switch (view) {
    case View3DWindows::Top:
        for (int i = 0; i < m_backgroundLength * m_backgroundWidth;i++){
            m_rgbTop[i] = m_rgbs[(int)m_heightIndex][i];
        }
        break;
    case View3DWindows::Front:
        for (int h = 0;h < m_backgroundHeight;h++){
            for (int w = 0;w < m_backgroundLength;w++){
                m_rgbFront[w + h * (int)m_backgroundLength] = m_rgbs[h][w + (int)m_widthIndex * (int)m_backgroundLength];
            }
        }
        break;
    case View3DWindows::Left:
        for (int h = 0;h < m_backgroundHeight;h++){
            for (int w = 0;w < m_backgroundWidth;w++){
                m_rgbLeft[w + h * (int)m_backgroundWidth] = m_rgbs[h][(int)m_lengthIndex + w * (int)m_backgroundLength];
            }
        }
        break;
    }

    // 放入view中
    if (view == View3DWindows::Top){
        m_InitBackgroundGroup(View3DWindows::Top);
        QImage image((const uchar*)m_rgbTop, m_backgroundLength, m_backgroundWidth, QImage::Format_ARGB32);
        QGraphicsPixmapItem* picItem = new QGraphicsPixmapItem(QPixmap::fromImage(image), m_backgroundGroupTop);
        picItem->setPos(0,0);
    }
    else if (view == View3DWindows::Front){
        m_InitBackgroundGroup(View3DWindows::Front);
        QImage image((const uchar*)m_rgbFront, m_backgroundLength, m_backgroundHeight, QImage::Format_ARGB32);
        QGraphicsPixmapItem* picItem = new QGraphicsPixmapItem(QPixmap::fromImage(image), m_backgroundGroupFront);
        picItem->setPos(0,0);
    }
    else if (view == View3DWindows::Left){
        m_InitBackgroundGroup(View3DWindows::Left);
        QImage image((const uchar*)m_rgbLeft, m_backgroundWidth, m_backgroundHeight, QImage::Format_ARGB32);
        QGraphicsPixmapItem* picItem = new QGraphicsPixmapItem(QPixmap::fromImage(image), m_backgroundGroupLeft);
        picItem->setPos(0,0);
    }
}

void View3D::m_Initiate()
{
    m_sceneTop = new QGraphicsScene(this);
    m_sceneFront = new QGraphicsScene(this);
    m_sceneLeft = new QGraphicsScene(this);

    m_sceneTop->setBackgroundBrush(Qt::gray);
    m_sceneFront->setBackgroundBrush(Qt::gray);
    m_sceneLeft->setBackgroundBrush(Qt::gray);

    ui->viewTop->setScene(m_sceneTop);
    ui->viewFront->setScene(m_sceneFront);
    ui->viewLeft->setScene(m_sceneLeft);

    m_backgroundGroupTop = new QGraphicsItemGroup();
    m_markerGroupTop = new QGraphicsItemGroup();
    m_sceneTop->addItem(m_backgroundGroupTop);
    m_sceneTop->addItem(m_markerGroupTop);

    m_backgroundGroupFront = new QGraphicsItemGroup();
    m_markerGroupFront = new QGraphicsItemGroup();
    m_sceneFront->addItem(m_backgroundGroupFront);
    m_sceneFront->addItem(m_markerGroupFront);

    m_backgroundGroupLeft = new QGraphicsItemGroup();
    m_markerGroupLeft = new QGraphicsItemGroup();
    m_sceneLeft->addItem(m_backgroundGroupLeft);
    m_sceneLeft->addItem(m_markerGroupLeft);
}

void View3D::m_InitBackgroundGroup(View3DWindows flag)
{
    QList<QGraphicsItem*> itemList;
    switch (flag) {
    case View3DWindows::Top:
        itemList = m_backgroundGroupTop->childItems();
        break;
    case View3DWindows::Front:
        itemList = m_backgroundGroupFront->childItems();
        break;
    case View3DWindows::Left:
        itemList = m_backgroundGroupLeft->childItems();
        break;
    }

    for (auto it = itemList.begin();it != itemList.end();it++){
        delete *it;
    }
}

void View3D::m_InitMarkerGroup(View3DWindows flag)
{
    QList<QGraphicsItem*> itemList;
    switch (flag) {
    case View3DWindows::Top:
        itemList = m_markerGroupTop->childItems();
        break;
    case View3DWindows::Front:
        itemList = m_markerGroupFront->childItems();
        break;
    case View3DWindows::Left:
        itemList = m_markerGroupLeft->childItems();
        break;
    }
    for (auto it = itemList.begin();it != itemList.end();it++){
        delete *it;
    }
}

void View3D::on_viewTopSlider_valueChanged(int value)
{
    // lcd
    ui->lcdNumberTop->display(value);

    m_heightIndex = value;
    UpdateViewBackground(View3DWindows::Top);

    Marker* mrk = MarkerManager::Instance()->SelectedMarker();
    if (mrk == nullptr) return;
    Marker3D* mrk3d = static_cast<Marker3D*>(mrk);

    for (int i = static_cast<int>(View3DWindows::Top);i <= static_cast<int>(View3DWindows::Left);i++){
        mrk3d->UpdateFromView(static_cast<View3DWindows>(i));
    }

    mrk3d->SetIndexs(m_lengthIndex, m_widthIndex, m_heightIndex);

    for (int i = static_cast<int>(View3DWindows::Top);i <= static_cast<int>(View3DWindows::Left);i++){
        mrk3d->UpdateToView(static_cast<View3DWindows>(i));
    }
}

void View3D::on_viewFrontSlider_valueChanged(int value)
{
    // lcd
    ui->lcdNumberFront->display(value);

    m_widthIndex = value;
    UpdateViewBackground(View3DWindows::Front);

    Marker* mrk = MarkerManager::Instance()->SelectedMarker();
    if (mrk == nullptr) return;
    Marker3D* mrk3d = static_cast<Marker3D*>(mrk);

    for (int i = static_cast<int>(View3DWindows::Top);i <= static_cast<int>(View3DWindows::Left);i++){
        mrk3d->UpdateFromView(static_cast<View3DWindows>(i));
    }

    mrk3d->SetIndexs(m_lengthIndex, m_widthIndex, m_heightIndex);


    for (int i = static_cast<int>(View3DWindows::Top);i <= static_cast<int>(View3DWindows::Left);i++){
        mrk3d->UpdateToView(static_cast<View3DWindows>(i));
    }
}

void View3D::on_viewLeftSlider_valueChanged(int value)
{
    //lcd
    ui->lcdNumberLeft->display(value);

    m_lengthIndex = value;
    UpdateViewBackground(View3DWindows::Left);

    Marker* mrk = MarkerManager::Instance()->SelectedMarker();
    if (mrk == nullptr) return;
    Marker3D* mrk3d = static_cast<Marker3D*>(mrk);

    for (int i = static_cast<int>(View3DWindows::Top);i <= static_cast<int>(View3DWindows::Left);i++){
        mrk3d->UpdateFromView(static_cast<View3DWindows>(i));
    }

    mrk3d->SetIndexs(m_lengthIndex, m_widthIndex, m_heightIndex);

    for (int i = static_cast<int>(View3DWindows::Top);i <= static_cast<int>(View3DWindows::Left);i++){
        mrk3d->UpdateFromView(static_cast<View3DWindows>(i));
        mrk3d->UpdateToView(static_cast<View3DWindows>(i));
    }
}

void View3D::ZoomIn(View3DWindows view)
{
    switch (view) {
    case View3DWindows::Top:
        if (m_zoomTop == 12)
            return;
        m_zoomTop++;
        ui->viewTop->scale(1.25, 1.25);
        break;
    case View3DWindows::Front:
        if (m_zoomFront == 12)
            return;
        m_zoomFront++;
        ui->viewFront->scale(1.25, 1.25);
        break;
    case View3DWindows::Left:
        if (m_zoomLeft == 12)
            return;
        m_zoomLeft++;
        ui->viewLeft->scale(1.25, 1.25);
        break;
    }
}

void View3D::ZoomOut(View3DWindows view)
{
    switch (view) {
    case View3DWindows::Top:
        if (m_zoomTop == 0)
            return;
        m_zoomTop--;
        ui->viewTop->scale(0.8, 0.8);
        break;
    case View3DWindows::Front:
        if (m_zoomFront == 0)
            return;
        m_zoomFront--;
        ui->viewFront->scale(0.8, 0.8);
        break;
    case View3DWindows::Left:
        if (m_zoomLeft == 0)
            return;
        m_zoomLeft--;
        ui->viewLeft->scale(0.8, 0.8);
        break;
    }
}
