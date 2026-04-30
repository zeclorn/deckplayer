#pragma once

#include <QAbstractListModel>
#include <QDir>
#include <QString>
#include <QVector>

class FileBrowserModel final : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentPath READ currentPath NOTIFY currentPathChanged)
    Q_PROPERTY(bool showHidden READ showHidden WRITE setShowHidden NOTIFY showHiddenChanged)

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        PathRole,
        DirectoryRole,
        PlayableRole,
        HiddenRole
    };
    Q_ENUM(Roles)

    explicit FileBrowserModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString currentPath() const;
    bool showHidden() const;

    Q_INVOKABLE bool canGoUp() const;

public slots:
    void setCurrentPath(const QString &path);
    void setShowHidden(bool showHidden);
    void goUp();

signals:
    void currentPathChanged();
    void showHiddenChanged();

private:
    struct Entry {
        QString name;
        QString path;
        bool isDirectory = false;
        bool isPlayable = false;
        bool isHidden = false;
    };

    void reload();
    static bool isLikelyMediaFile(const QFileInfo &fileInfo);
    static bool entryLessThan(const Entry &left, const Entry &right);

    QVector<Entry> m_entries;
    QString m_currentPath;
    bool m_showHidden = false;
};
