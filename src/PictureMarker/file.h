#ifndef FILE_H
#define FILE_H

// file.h: 代表一个文件的类

#include <QWidget>

enum class FileType{
    single,
    ct
};

class File
{
public:
    File();
    File(const QString& path, FileType type);
    File(const QString& path, FileType type, const QString& id);

    FileType fileType() const;
    QString path() const;
    QString id() const;

    void setFileType(FileType type);
    void setPath(const QString& path);
    void setId(const QString& id);


private:
    // 文件的类型
    FileType m_fileType;
    // 文件或文件夹保留的路径
    QString m_path;
    // 文件的id
    QString m_id;
};

#endif // FILE_H
