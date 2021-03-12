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
QDate Data;
bool okConnesso = false;
quint32 numeroRighe = 0;

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    setMsgStatusBar("Connettere Database");
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
    switch (role) {
        case Qt::DisplayRole:{
            if(okConnesso){
                ptrTable->setTable(Tabella);
                ptrTable->setSort(0, Qt::DescendingOrder);
                //Table.setFilter("Data = " + Data.toString("dd/MM/yyyy"));
                ptrTable->select();
                Record = ptrTable->record(index.row());
                if(Record.isEmpty()) return QString("No Data");
                return Record.value(index.column());
            }
        }break;
        default:
            break;
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
    numeroRighe = getNumeroRighe();
    setMsgStatusBar("Numero Record Letti " + QString::number(numeroRighe));
    ptrTable = new QSqlTableModel();
    endResetModel();
}

uint32_t TableModel::getNumeroRighe()
{
    QString Sql = QString("Select Rowid From %1").arg(Tabella);
    QSqlQuery Query;
    Query.exec(Sql);
    return Query.size();
}
