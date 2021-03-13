#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDate>
#include "tablemodel.h"

QSqlDatabase Db;
QSqlTableModel *ptrTable;
QSqlRecord Record;
QString Tabella = "";
QString Data;
QString Filter;
bool okConnesso = false;
quint32 numeroRighe = 0;

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    setMsgStatusBar("Connettere Database");
    getData();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    (void) orientation;
    if(role == Qt::DisplayRole){
        switch (section) {
            case 0: return QVariant("RowID");
            case 1: return QVariant("Data");
            case 2: return QVariant("Ora");
            case 3: return QVariant("Stampo");
            case 4: return QVariant("Operatore");
            case 5: return QVariant("Vita Macchina");
            case 6: return QVariant("Cicli");
            case 7: return QVariant("Tempo Ciclo");
            case 8: return QVariant("Vulca");
            case 9: return QVariant("Tempo Iniezione");
            case 10: return QVariant("Tempo Dosatura");
            case 11: return QVariant("Materiale");
            case 12: return QVariant("Temp. Mobile");
            case 13: return QVariant("Temp. Fisso");
        }
    }
    return QVariant();
}

int TableModel::rowCount(const QModelIndex &) const
{
    if(okConnesso) return numeroRighe;

    return 0;
}

int TableModel::columnCount(const QModelIndex &) const
{
    if(okConnesso) return 14;
    return 0;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    int IdxCol = index.column();
    if(role == Qt::DisplayRole){
        switch (IdxCol) {
            case 0:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
                Record = ptrTable->record(index.row());
                return Record.value(IdxCol).toUInt();
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                Record = ptrTable->record(index.row());
                return Record.value(IdxCol).toString();
                break;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> TableModel::roleNames() const
{
    QHash<int, QByteArray> Roles;
    Roles[Qt::DisplayRole] = "Display";
    return Roles;
}

QString TableModel::MsgStatusBar()
{
    return mMsgStatusBar;
}

void TableModel::setMsgStatusBar(QString S)
{
    mMsgStatusBar.operator=(S);
    emit MsgStatusBarChanged();
}

QStringList TableModel::Tabelle()
{
    return mTabelle;
}

void TableModel::setTabelle(QStringList Ls)
{
    mTabelle.operator=(Ls);
    emit TabelleChanged();
}

void TableModel::setConnection(QString Host)
{ 
    Db = QSqlDatabase::addDatabase("QMARIADB");
    Db.setHostName(Host);
    Db.setPort(3306);
    Db.setDatabaseName("GomsilDB");
    okConnesso = Db.open("Gomsil", "Gomsil123");
    if(okConnesso){
        setMsgStatusBar("Connessione con Database OK");
        setTabelle(Db.tables());
    }
}

void TableModel::initModel(QString T)
{
    beginResetModel();
    Tabella.operator=(T);
    ptrTable = new QSqlTableModel();
    ptrTable->setTable(Tabella);
    ptrTable->setSort(0, Qt::DescendingOrder);
    Filter = QString("Data = \"%1\"").arg(Data);
    ptrTable->setFilter(Filter);
    ptrTable->select();
    numeroRighe = getNumeroRighe(Filter);
    setMsgStatusBar("Numero Record Letti " + QString::number(numeroRighe));
    endResetModel();
}

uint32_t TableModel::getNumeroRighe(QString Where)
{
    QString Sql = QString("Select Rowid From %1 Where %2").arg(Tabella).arg(Where);
    QSqlQuery Query;
    Query.exec(Sql);
    return Query.size();
}

void TableModel::getData()
{
    QDateTime Dt = QDateTime::currentDateTime();
    QDate d = Dt.date();
    Data.operator=(d.toString("dd/MM/yyyy"));
}
