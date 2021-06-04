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
            Layout.alignment: Qt.AlignHCenter
            source: "qrc:/Images/Squirrel-Icon.png"
        }

        Label{
            font.pointSize: 12
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 10
            text: "Developed by Veveriza"
        }

        Label{
            font.pointSize: 12
            font.italic: true
            Layout.alignment: Qt.AlignHCenter
            text: "Versione 1.13"
        }
    }
}
