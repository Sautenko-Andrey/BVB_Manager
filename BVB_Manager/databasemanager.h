#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QDir>

class DatabaseManager
{
public:

    DatabaseManager(QWidget *widget = nullptr);

    DatabaseManager(const DatabaseManager &other) = delete;

    DatabaseManager& operator=(const DatabaseManager &other) = delete;

    ~DatabaseManager();

    QSqlDatabase& getDatabase();



private:

    QSqlDatabase m_db;

    inline static const QString path_to_db = QDir::homePath() +
                                             "/BVB_Manager/BVB_Manager/data.db";
};

#endif // DATABASEMANAGER_H
