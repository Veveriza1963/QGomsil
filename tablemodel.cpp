#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include "tablemodel.h"

QSqlDatabase Db;
QSqlTableModel *ptrTable;
QSqlQueryModel *Qm;
QSqlRecord Record;
QSqlError Error;
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
        //Se tabella Utenti entra qui...
        if(Tabella == "Utenti"){
            switch (section) {
                case 0: return QVariant("Rowid");
                case 1: return QVariant("NumID");
                case 2: return QVariant("NomeIT");
                case 3: return QVariant("NomeBG");
                case 4: return QVariant("Job");
            }
        }
        //Se tabella non Utenti qui
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
    if(okConnesso){
        if(Tabella == "Utenti"){
            return 5;
        }
        return 14;
    }
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

QStringList TableModel::ListaOperatori()
{
    if(!okConnesso) return {};
    QSqlQuery Query(QString("Select Distinct Operatore From %1 Order By Operatore Asc").arg(Tabella));
    QStringList Qsl;
    Qsl.operator<<("Tutti");
    while (Query.next()) {
        Qsl.operator<<(Query.value(0).toString());
    }
    return Qsl;
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
        ptrTable = new QSqlTableModel;
        Qm = ptrTable;
    }
}

void TableModel::setDisconnect()
{
    QSqlDatabase::removeDatabase(Db.connectionName());
    Db.close();
    setMsgStatusBar("Disconnessione Database");
}

void TableModel::initModel(QString T)
{
    Tabella.operator=(T);
    emit layoutAboutToBeChanged();
    //Init con tabella Utenti
    if(Tabella == "Utenti"){
        getNumeroRighe("");
        ptrTable->setTable(Tabella);
        ptrTable->setSort(2,Qt::AscendingOrder);
        ptrTable->select();
        emit layoutChanged();
        setMsgStatusBar(QString("Numero Record Letti %1").arg(numeroRighe));
        return;
    }
    //Se Tabella non Utenti
    Filter = QString("Data = \"%1\"").arg(Data);
    getNumeroRighe(QString("Where %1").arg(Filter));
    ptrTable->setTable(Tabella);
    ptrTable->setSort(0, Qt::DescendingOrder);
    ptrTable->setFilter(Filter);
    if(ptrTable->select()){
        setMsgStatusBar(QString("Numero Record Letti %1").arg(numeroRighe));
    }
    emit ListaOperatoriChanged();
    emit layoutChanged();
}

void TableModel::setAggiornaRighe(bool On, quint32 Righe)
{
    beginResetModel();
    if(On){
        Filter.operator=("1");
        getNumeroRighe(QString("Where %1").arg(Filter));
        ptrTable->setFilter(Filter);
        if(ptrTable->select()){
            setMsgStatusBar(QString("Numero Record Letti %1").arg(numeroRighe));
        }
        endResetModel();
        return;
    }

    Qm->setQuery(QSqlQuery(QString("Select * From %1 Order By Rowid Desc Limit %2").arg(Tabella).arg(Righe)));
    getNumeroRighe(QString("Limit %1").arg(Righe));
    setMsgStatusBar(QString("Numero Record Letti %1").arg(numeroRighe));
    endResetModel();
}

void TableModel::setCustomQuery(QString Query)
{
    Qm->setQuery(Query);
    Error = Qm->lastError();
    if(Error.isValid()){
        QHash<int, QString>Err;
        Err.insert(1, "Errore Connessione a Database");
        Err.insert(2, "Errore Scrittura Query");
        Err.insert(3, "Errore Transazione" );
        Err.insert(4, "Errore Sconosciuto");
        setMsgStatusBar(QString("%1 - %2").arg(Error.text(), Error.type()));
    } else {
        setMsgStatusBar("Query Eseguita Correttamente Aggiornare Tabella");
    }
}

void TableModel::callSearch(QString Data, QString Ope)
{
    QString Operatore = (Ope == "Tutti") ? "%" : Ope;
    QSqlQuery Query;
    Query.prepare(QString("Select * From %1 Where Operatore Like ? And Data = ? Order By Rowid Desc").arg(Tabella));
    Query.bindValue(0, Operatore, QSql::Out);
    Query.bindValue(1, Data, QSql::Out);
    if(Query.exec()){
        beginResetModel();
        numeroRighe = Query.size();
        Qm->setQuery(Query);
        setMsgStatusBar(QString("Numero Record Letti %1").arg(numeroRighe));
        endResetModel();
    }
}

void TableModel::getNumeroRighe(QString Condition)
{
    QSqlQuery Query;
    QString Sql = QString("Select Rowid From %1 %2").arg(Tabella, Condition);
    Query.exec(Sql);
    numeroRighe = Query.size();
}

void TableModel::getData()
{
    QDateTime Dt = QDateTime::currentDateTime();
    QDate d = Dt.date();
    Data.operator=(d.toString("dd/MM/yyyy"));
}
