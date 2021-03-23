import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQml
import TableModel

ApplicationWindow {
    id: root
    x: 120; y: 40
    width: 1024; height: 768
    visible: true
    title: qsTr("QGomsil Tools")
    header: MenuBar{
        Menu{
            width: 150
            title: "&File"
            Action{
                id: actionQuit
                text: "&Quit (Ctrl + Q)"
                shortcut: "Ctrl+Q"
                onTriggered: root.close();
            }
        }
        Menu{
            title: "&Azioni"
            width: 250
            Action{
                id: actionConnetti
                text: "&Connetti " + cbxHost.currentText + " (Ctrl + F5)"
                shortcut: "Ctrl+F5"
                onTriggered: {
                    tableModel.setConnection(cbxHost.currentText);
                    cbxTabelle.enabled = true
                    btnUpdate.enabled = true
                    btnAggiorna.enabled = true
                    cbxTabelle.forceActiveFocus();
                }
            }
            Action{
                id: actionAggiornaData
                text: "Aggiorna a &Data Odierna (Ctrl + F6)"
                shortcut: "Ctrl+F6"
                onTriggered: tableModel.initModel(cbxTabelle.currentText)
           }
            Action{
                id: actionAggiorna
                text: "Aggiorna &Righe Tabella (Ctrl + F7)"
                shortcut: "Ctrl+F7"
                onTriggered: tableModel.setAggiornaRighe(chxTutti.checked, spxRighe.value);
            }
            Action{
                id: actionSearch
                text: "A&vvia Ricerca (Ctrl + F8)"
                shortcut: "Ctrl+F8"
                onTriggered: {
                    tableModel.callSearch(txfData.text, cbxOperatori.currentValue);
                    tableView.positionViewAtRow(0, Qt.AlignTop, 0);
                }
            }
        }
    }
    footer: Rectangle{
        height: 25; width: parent.width
        color: "lightgrey"
        Label{
            id: statusBar
            x: 10; y: parent.height /2 - height / 2
            font.pointSize: 10
            text: tableModel.MsgStatusBar
        }
    }
    ColumnLayout{
        id: col1
        y:10
        width: parent.width; height: parent.height
        spacing: 10

        RowLayout{
            id: row1
            height: 25
            Layout.maximumHeight: 25
            width: parent.width
            spacing: 10

            Label{
                y: row1.height / 2 - height / 2
                font.pointSize: 12
                Layout.leftMargin: 5
                text: "Hostname:"
            }
            ComboBox{
                id: cbxHost
                width: 200; height: parent.height
                textRole: "Key"
                valueRole: "Value"
                model: ListModel{
                    ListElement{
                       Key: "192.168.0.31"
                       Value: "192.168.0.31"
                    }
                    ListElement{
                        Key: "gomsil.ddns.net"
                        Value: "gomsil.ddns.net"
                    }
                }
            }
            Button{
                width: 200; height: parent.height
                icon.source: "qrc:/Images/Places-Network-Server-Database-Icon.png"
                text: "Connetti a " + cbxHost.currentText
                onClicked: actionConnetti.trigger();
            }
            ComboBox{
                id: cbxTabelle
                height: parent.height
                Layout.maximumWidth: 50
                enabled: false
                model: tableModel.Tabelle
                onActivated: actionAggiornaData.trigger();
            }
            Button{
                id: btnUpdate
                height: parent.height
                Layout.maximumWidth: 50
                enabled: false
                icon.source: "qrc:/Images/Button-Round-Reload-Icon.png"
                onClicked: actionAggiornaData.trigger();
                ToolTip{
                    visible: btnUpdate.hovered
                    timeout: 2000
                    text: "Aggiorna tabella a Data"
                }
            }
            Rectangle{
                height: parent.height; width: 3
                color: "black"
            }
            Label{
                width: 50; height: parent.height
                text: "Visualizza Record:"
            }
            CheckBox{
                id: chxTutti
                checkState: Qt.Unchecked
                text: "Tutti i Record"
            }
            SpinBox{
                id: spxRighe
                height: parent.height; width: 100
                editable: true
                from: 1
                to: 1000000
                value: 10
            }
            Button{
                id: btnAggiorna
                height: parent.height; width: 100
                enabled: false
                icon.source: "qrc:/Images/Button-Round-Reload-Icon.png"
                ToolTip{
                    visible: btnAggiorna.hovered
                    text: "Aggiorna Righe Tabella"
                    timeout: 2000
                }
                text: "Aggiorna"
                onClicked: actionAggiorna.trigger()
            }
        }

        RowLayout{
            id: row2
            height: 25; width: parent.width
            Layout.maximumHeight: 25
            spacing: 10

            Label{
                font.pointSize: 10
                Layout.leftMargin: 5
                text: "Custom Query:"
            }
            TextField{
                id: txfQuery
                height: parent.height
                Layout.fillWidth: true
                font.pointSize: 10
                rightInset: 5
                placeholderText: "Edit the Update or Delete Query"
                background: Rectangle{
                    color: "white"
                    border.width: 1
                    border.color: "black"
                }
                onEditingFinished: tableModel.setCustomQuery(txfQuery.text);
                onPressed: txfQuery.text = "Update " + cbxTabelle.currentText + " Set "
            }
        }

        RowLayout{
            id:row3
            height: 25; width: parent.width
            Layout.maximumHeight: 25
            spacing: 10

            Label{
                font.pointSize: 10
                Layout.leftMargin: 5
                text: "Search:"
            }
            TextField{
                id: txfData
                height: parent.height
                rightInset: 5
                font.pointSize: 10
                background: Rectangle{
                    implicitWidth: 150
                    border.width: 1
                    border.color: "black"
                }
                text: getData()
            }
            ComboBox{
                id: cbxOperatori
                height: parent.height
                width: 100
                model: tableModel.ListaOperatori
            }
            Button{
                id: btnSearch
                height: parent.height
                width: 100
                Layout.maximumWidth: 100
                icon.source: "qrc:/Images/Search-Icon.png"
                onClicked: actionSearch.trigger();
            }
        }

        HorizontalHeaderView{
            id: horizontalHeader
            syncView: tableView
        }
        TableView{
            id: tableView
            width: root.width
            Layout.fillHeight: true
            Layout.bottomMargin: 10
            columnSpacing: 1
            rowSpacing: 1
            clip: true

            model: TableModel{id: tableModel}
            delegate: Rectangle{
                implicitHeight: 20
                implicitWidth: 150
                border.width: 1
                border.color: "grey"

                Label{
                    anchors.centerIn: parent
                    font.pointSize: 8
                    color: "black"
                    text: model.Display
                }
            }

        }
    }
    function getData(){
        var d = new Date();
        var Mese = 0;
        var i = d.getMonth();
        switch(i){
            case 9:
            case 10:
            case 11:
                Mese = ++i
                break
            default:
                Mese = ++i
                Mese = "0" + Mese
                break
        };
        return d.getDate() + "/" + Mese + "/" + d.getFullYear();
    }
}
