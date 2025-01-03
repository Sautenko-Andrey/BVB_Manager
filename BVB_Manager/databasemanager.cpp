#include <QMessageBox>
#include "databasemanager.h"

void DatabaseManager::prepareManager(QWidget *widget){
    m_db = QSqlDatabase::addDatabase("QSQLITE","Connection");
    m_db.setDatabaseName(path_to_db);

    if(!m_db.open()){
        QMessageBox::warning(widget, "Error",
                             "Couldn't load the database. Reopen the app");
        return;
    }
}

DatabaseManager::~DatabaseManager() {
    m_db.close();
}


QSqlDatabase& DatabaseManager::getDatabase() {
    return m_db;
}
