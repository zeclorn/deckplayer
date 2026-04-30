#include "FileBrowserModel.h"

#include <algorithm>
#include <QCollator>
#include <QFileInfo>
#include <QSet>
#include <QStringList>

namespace {

QSet<QString> supportedExtensions()
{
    return {
        QStringLiteral("mp4"),
        QStringLiteral("m4v"),
        QStringLiteral("mkv"),
        QStringLiteral("avi"),
        QStringLiteral("mov"),
        QStringLiteral("webm"),
        QStringLiteral("mpeg"),
        QStringLiteral("mpg"),
        QStringLiteral("ts"),
        QStringLiteral("m2ts"),
        QStringLiteral("wmv"),
        QStringLiteral("flv")
    };
}

}

FileBrowserModel::FileBrowserModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int FileBrowserModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_entries.size();
}

QVariant FileBrowserModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_entries.size()) {
        return {};
    }

    const Entry &entry = m_entries.at(index.row());
    switch (role) {
    case NameRole:
        return entry.name;
    case PathRole:
        return entry.path;
    case DirectoryRole:
        return entry.isDirectory;
    case PlayableRole:
        return entry.isPlayable;
    case HiddenRole:
        return entry.isHidden;
    default:
        return {};
    }
}

QHash<int, QByteArray> FileBrowserModel::roleNames() const
{
    return {
        {NameRole, "name"},
        {PathRole, "path"},
        {DirectoryRole, "isDirectory"},
        {PlayableRole, "isPlayable"},
        {HiddenRole, "isHidden"}
    };
}

QString FileBrowserModel::currentPath() const
{
    return m_currentPath;
}

bool FileBrowserModel::showHidden() const
{
    return m_showHidden;
}

bool FileBrowserModel::canGoUp() const
{
    return !m_currentPath.isEmpty() && QDir(m_currentPath).cdUp();
}

void FileBrowserModel::setCurrentPath(const QString &path)
{
    QFileInfo info(path);
    if (!info.exists() || !info.isDir()) {
        return;
    }

    const QString normalized = QDir(path).absolutePath();
    if (normalized == m_currentPath) {
        return;
    }

    m_currentPath = normalized;
    reload();
    emit currentPathChanged();
}

void FileBrowserModel::setShowHidden(bool showHidden)
{
    if (m_showHidden == showHidden) {
        return;
    }

    m_showHidden = showHidden;
    reload();
    emit showHiddenChanged();
}

void FileBrowserModel::goUp()
{
    QDir dir(m_currentPath);
    if (!dir.cdUp()) {
        return;
    }

    setCurrentPath(dir.absolutePath());
}

void FileBrowserModel::reload()
{
    beginResetModel();
    m_entries.clear();

    if (!m_currentPath.isEmpty()) {
        QDir dir(m_currentPath);
        QDir::Filters filters = QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Readable;
        if (m_showHidden) {
            filters |= QDir::Hidden;
        }

        const QFileInfoList fileInfos = dir.entryInfoList(filters, QDir::NoSort);
        m_entries.reserve(fileInfos.size());
        for (const QFileInfo &info : fileInfos) {
            const bool hidden = info.fileName().startsWith('.');
            if (hidden && !m_showHidden) {
                continue;
            }

            Entry entry;
            entry.name = info.fileName();
            entry.path = info.absoluteFilePath();
            entry.isDirectory = info.isDir();
            entry.isPlayable = !entry.isDirectory && isLikelyMediaFile(info);
            entry.isHidden = hidden;

            if (entry.isDirectory || entry.isPlayable) {
                m_entries.push_back(entry);
            }
        }

        std::sort(m_entries.begin(), m_entries.end(), entryLessThan);
    }

    endResetModel();
}

bool FileBrowserModel::isLikelyMediaFile(const QFileInfo &fileInfo)
{
    const QString extension = fileInfo.suffix().toLower();
    return supportedExtensions().contains(extension);
}

bool FileBrowserModel::entryLessThan(const Entry &left, const Entry &right)
{
    if (left.isDirectory != right.isDirectory) {
        return left.isDirectory;
    }

    static thread_local QCollator collator;
    collator.setNumericMode(true);
    collator.setCaseSensitivity(Qt::CaseInsensitive);
    return collator.compare(left.name, right.name) < 0;
}
