import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    width: 200; height: 300
    visible: false
    modal: true
    standardButtons: Dialog.Close
    title: "About"

    ColumnLayout{
        width: parent.width
        height: parent.height
        spacing: 10

        Image {
            id: imageSquirrel
            anchors.horizontalCenter:  parent.horizontalCenter
            source: "qrc:/Images/Squirrel-Icon.png"
        }

        Label{
            font.pointSize: 12
            anchors.horizontalCenter: parent.horizontalCenter
            Layout.topMargin: 10
            text: "Developed by Veveriza"
        }

        Label{
            font.pointSize: 12
            font.italic: true
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Versione 1.8"
        }
    }
}
