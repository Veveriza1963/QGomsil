import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQml 2.12
import TableModel

ApplicationWindow {
    id: root
    x: 120; y: 20
    width: 1024; height: 768
    visible: true
    title: qsTr("QGomsil Tools")
    header: MenuBar{
        Menu{
            title: "&File"
            Action{
                id: actionQuit
                text: "&Quit"
                shortcut: "Ctrl+Q"
                onTriggered: root.close();
            }
        }
    }
    footer: Rectangle{
        height: 25; width: parent.width
        color: "lightgrey"
        Label{
            id: statusBar
            x: 10; y: parent.height /2 - height / 2
            font.pointSize: 12
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

            ComboBox{
                id: cbxHost
                x:10
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
                text: "Connetti a " + cbxHost.currentText
                onClicked: {
                    tableModel.setConnection(cbxHost.currentText);
                    cbxTabelle.forceActiveFocus();
                }
            }

            ComboBox{
                id: cbxTabelle
                width: 100; height: parent.height
                model: tableModel.Tabelle
                onActivated: {
                    tableModel.initModel(cbxTabelle.currentText);
                    tableView.forceLayout();
                }
            }
        }

        TableView{
            id: tableView
            width: root.width
            height: col1.height - row1.height
            Layout.fillHeight: true
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
                    color: "red"
                    text: model.Header
                }

                Label{
                    anchors.centerIn: parent
                    font.pointSize: 8
                    color: "black"
                    text: model.Display
                }
            }

        }
    }
}
