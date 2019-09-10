#include "file.h"

File::File()
    : m_fileType(FileType::single),
      m_path(""),
      m_id("")
{
}

File::File(const QString& path, FileType type)
    : m_fileType(type), m_path(path), m_id(path)
{
}

File::File(const QString& path, FileType type, const QString& id)
    : m_fileType(type), m_path(path), m_id(id)
{

}

FileType File::fileType() const{
    return m_fileType;
}

QString File::path() const{
    return m_path;
}

QString File::id() const{
    return m_id;
}

void File::setFileType(FileType type){
    m_fileType = type;
}

void File::setPath(const QString& path){
    m_path = path;
}

void File::setId(const QString &id){
    m_id = id;
}
