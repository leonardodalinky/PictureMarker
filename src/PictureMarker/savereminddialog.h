#ifndef SAVEREMINDDIALOG_H
#define SAVEREMINDDIALOG_H

// savereminddialog.h: 用于提醒用户保存的对话框

#include <QDialog>

namespace Ui {
class SaveRemindDialog;
}

enum class SaveRemindStatus{
    Save,
    SaveTo,
    Discard,
    Cancel
};

class SaveRemindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveRemindDialog(QWidget *parent = nullptr);
    ~SaveRemindDialog();

    SaveRemindStatus Status() const;
    QString Path() const;

private slots:
    void on_saveButton_clicked();

    void on_saveToButton_clicked();

    void on_discardButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::SaveRemindDialog *ui;

    SaveRemindStatus m_status = SaveRemindStatus::Cancel;
    QString m_path;
};

#endif // SAVEREMINDDIALOG_H
