#include "savereminddialog.h"
#include "ui_savereminddialog.h"
#include <QStyle>
#include <QFileDialog>

SaveRemindDialog::SaveRemindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveRemindDialog)
{
    ui->setupUi(this);

    ui->imageLabel->setPixmap(QApplication::style()->standardPixmap(static_cast<QStyle::StandardPixmap>(12))
                              .scaled(ui->imageLabel->width(), ui->imageLabel->height(), Qt::KeepAspectRatio));
}

SaveRemindDialog::~SaveRemindDialog()
{
    delete ui;
}

SaveRemindStatus SaveRemindDialog::Status() const
{
    return m_status;
}

QString SaveRemindDialog::Path() const
{
    return m_path;
}

void SaveRemindDialog::on_saveButton_clicked()
{
    m_status = SaveRemindStatus::Save;
    accept();
}

void SaveRemindDialog::on_saveToButton_clicked()
{
    m_path = QFileDialog::getSaveFileName(
                this,
                tr("Save markers to ..."),
                ".",
                "Marker Files (*.mrk)"
                );
    if (m_path.isEmpty())
        m_status = SaveRemindStatus::Cancel;
    else
        m_status = SaveRemindStatus::SaveTo;

    accept();
}

void SaveRemindDialog::on_discardButton_clicked()
{
    m_status = SaveRemindStatus::Discard;
    accept();
}

void SaveRemindDialog::on_cancelButton_clicked()
{
    m_status = SaveRemindStatus::Cancel;
    accept();
}
