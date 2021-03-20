#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QString MsgStatusBar READ MsgStatusBar WRITE setMsgStatusBar NOTIFY MsgStatusBarChanged)
    Q_PROPERTY(QStringList Tabelle READ Tabelle WRITE setTabelle NOTIFY TabelleChanged)

public:
    explicit TableModel(QObject *parent = nullptr);

    // Basic functionality:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    //Property Function
    QString MsgStatusBar();
    void setMsgStatusBar(QString S);
    QStringList Tabelle();
    void setTabelle(QStringList Ls);

public slots:
    void setConnection(QString Host);
    void setDisconnect();
    void initModel(QString T);
    void setAggiornaRighe(bool On, quint32 Righe);
    void setCustomQuery(QString Query);

signals:
    void MsgStatusBarChanged();
    void TabelleChanged();

private:
    QString mMsgStatusBar;
    QStringList mTabelle;
    void getNumeroRighe(QString Condition);
    void getData();

};

#endif // TABLEMODEL_H
