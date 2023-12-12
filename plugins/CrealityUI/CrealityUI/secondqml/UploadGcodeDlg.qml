import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.13
import QtGraphicalEffects 1.12

import "../qml"
import "../secondqml"

BasicDialogV4
{
    id: root
    //color: "transparent"
    width: windowMaxWidth
    height: windowMaxHeight
    //modality: Qt.ApplicationModal
    //flags: Qt.FramelessWindowHint | Qt.Dialog

    signal sigCancelBtnClicked()
    signal sigConfirmBtnClicked()

    //0:Creality Cloud 1: LAN
    property int uploadDlgType: 0
    property int uploadProgress: 0
    property int curWindowState: -1

    property real borderWidth: 1 * screenScaleFactor
    property real shadowWidth: 5 * screenScaleFactor
    property real titleHeight: 30 * screenScaleFactor
    property real borderRadius: 5 * screenScaleFactor

    property real windowMinWidth: 500 * screenScaleFactor
    property real windowMaxWidth: 756 * screenScaleFactor
    property real windowMinHeight: 152 * screenScaleFactor
    property real windowMaxHeight: 600 * screenScaleFactor

    property var curGcodeFileName: ""
    property var previewSource: ""
    property var errorTxt:""
    property var msgImageSource:""
    property var gcodeFileName:""
    //Slice parameter
    property bool isFromFile: kernel_slice_flow.sliceAttain ? kernel_slice_flow.sliceAttain.isFromFile() : false
    property string printTime: kernel_slice_flow.sliceAttain ? kernel_slice_flow.sliceAttain.printing_time() : ""
    property double materialWeight: kernel_slice_flow.sliceAttain ? kernel_slice_flow.sliceAttain.material_weight() : ""

    title: uploadDlgType ? qsTr("Send G-code") : qsTr("Upload G-code")
    property string textColor: Constants.currentTheme ? "#333333" : "#CBCBCC"
    property string shadowColor: Constants.currentTheme ? "#BBBBBB" : "#333333"
    property string borderColor: Constants.currentTheme ? "#D6D6DC" : "#262626"
    property string titleFtColor: Constants.currentTheme ? "#333333" : "#FFFFFF"
    property string titleBgColor: Constants.currentTheme ? "#E9E9EF" : "#6E6E73"
    property string backgroundColor: Constants.currentTheme ? "#FFFFFF" : "#4B4B4D"

    Connections {
        target: cxkernel_cxcloud.sliceService

        onUploadSliceSuccessed: function() {
            root.uploadGcodeSuccess()
        }

        onUploadSliceFailed: function() {
            root.uploadGcodeFailed()
        }

        onUploadSliceProgressUpdated: function(progress) {
            root.uploadGcodeProgress(progress)
        }
    }

    function showUploadDialog(type)
    {
        uploadDlgType = type
        root.show()
    }

    function uploadGcodeProgress(value)
    {
        uploadProgress = value
    }

    function uploadGcodeSuccess()
    {
        errorTxt = qsTr("Uploaded Successfully")
        msgImageSource = "qrc:/UI/photo/upload_success_image.png"
        curWindowState = UploadGcodeDlg.WindowState.State_Message
    }

    function uploadGcodeFailed()
    {
        errorTxt = qsTr("Failed to upload gcode!")
        msgImageSource = "qrc:/UI/photo/upload_msg.png"
        curWindowState = UploadGcodeDlg.WindowState.State_Message
    }

    enum WindowState
    {
        State_Normal,
        State_Progress,
        State_Message
    }

    onVisibleChanged: {
        if(visible)
        {
            curWindowState = UploadGcodeDlg.WindowState.State_Normal

            previewSource = ""
            previewSource = "file:///" + kernel_slice_flow.gcodeThumbnail()

            curGcodeFileName = kernel_slice_flow.getExportName()
            //idGcodeFileName.forceActiveFocus()
        }
    }

    onCurWindowStateChanged: {
        width = curWindowState ? windowMinWidth : windowMaxWidth
        height = curWindowState ? windowMinHeight: windowMaxHeight
    }

    bdContentItem:Rectangle {
        id: rect
        anchors.fill: parent
        //anchors.margins: shadowWidth

        //border.width: borderWidth
        //border.color: borderColor
        color: backgroundColor
        //radius: borderRadius

        Item {
            width: parent.width
            height: parent.height

            //anchors.top: cusTitle.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            MouseArea {
                anchors.fill: parent
                onClicked: forceActiveFocus()
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.topMargin: 20 * screenScaleFactor
                anchors.leftMargin: 40 * screenScaleFactor
                anchors.rightMargin: 40 * screenScaleFactor
                anchors.bottomMargin: 20 * screenScaleFactor

                spacing: 20 * screenScaleFactor
                visible: curWindowState === UploadGcodeDlg.WindowState.State_Normal

                RowLayout {
                    spacing: 8 * screenScaleFactor
                    Layout.alignment: Qt.AlignHCenter

                    Image {
                        Layout.preferredWidth: (uploadDlgType ? 22 : 28) * screenScaleFactor
                        Layout.preferredHeight: (uploadDlgType ? 23 : 26) * screenScaleFactor
                        source: uploadDlgType ? `qrc:/UI/photo/wifiPrint_upload_${Constants.currentTheme ? "light" : "dark"}.svg` : "qrc:/UI/photo/CloudLogo.png"
                    }

                    Text {
                        Layout.preferredWidth: contentWidth * screenScaleFactor
                        Layout.preferredHeight: 26 * screenScaleFactor

                        font.weight: Font.Medium
                        font.family: Constants.mySystemFont.name
                        font.pointSize: Constants.labelFontPointSize_12
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        color: textColor
                        text: uploadDlgType ? qsTr("Local Area Network") : qsTr("Creality Cloud")
                    }
                }

                Rectangle {
                    Layout.preferredWidth: 674 * screenScaleFactor
                    Layout.preferredHeight: 340 * screenScaleFactor

                    color: Constants.currentTheme ? "#FFFFFF" : "#424244"
                    border.color: Constants.currentTheme ? "#DDDDE1" : "transparent"
                    border.width: Constants.currentTheme ? 1 : 0
                    radius: 5

                    Column {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.topMargin: 20 * screenScaleFactor
                        anchors.leftMargin: 20 * screenScaleFactor
                        spacing: 25 * screenScaleFactor

                        Row {
                            spacing: 8 * screenScaleFactor

                            Image {
                                width: 18 * screenScaleFactor
                                height: 16 * screenScaleFactor
                                anchors.verticalCenter: parent.verticalCenter
                                source: Constants.currentTheme ? "qrc:/UI/photo/print_time_light.svg" : "qrc:/UI/photo/print_time_dark.svg"
                            }

                            Text {
                                width: 16 * screenScaleFactor
                                font.weight: Font.Medium
                                font.family: Constants.mySystemFont.name
                                font.pointSize: Constants.labelFontPointSize_9
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                color: textColor
                                text: printTime
                            }
                        }

                        Row {
                            anchors.left: parent.left
                            anchors.leftMargin: 2 * screenScaleFactor
                            spacing: 10 * screenScaleFactor

                            Image {
                                width: 14 * screenScaleFactor
                                height: 14 * screenScaleFactor
                                anchors.verticalCenter: parent.verticalCenter
                                source: Constants.currentTheme ? "qrc:/UI/photo/material_weight_light.svg" : "qrc:/UI/photo/material_weight_dark.svg"
                            }

                            Text {
                                width: 16 * screenScaleFactor
                                font.weight: Font.Medium
                                font.family: Constants.mySystemFont.name
                                font.pointSize: Constants.labelFontPointSize_9
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                color: textColor
                                text: materialWeight + "g"
                            }
                        }
                    }

                    Image {
                        cache: false
                        id: imgPreview
                        anchors.centerIn: parent
                        sourceSize: Qt.size(300, 300)
                        source : root.previewSource
                        property string imgDefault: "qrc:/UI/photo/imgPreview_default.png"

                        onStatusChanged: if(status == Image.Error) root.previewSource = imgDefault
                    }
                }

                RowLayout {
                    spacing: 10 * screenScaleFactor

                    Text {
                        Layout.preferredWidth: contentWidth * screenScaleFactor
                        Layout.preferredHeight: 28 * screenScaleFactor

                        font.weight: Font.Medium
                        font.family: Constants.mySystemFont.name
                        font.pointSize: Constants.labelFontPointSize_9
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        color: textColor
                        text: qsTr("Name")
                    }

                    TextField {
                        id: idGcodeFileName
                        Layout.fillWidth: true
                        Layout.preferredHeight: 28 * screenScaleFactor
                        text:root.curGcodeFileName
                        validator: RegExpValidator { regExp: /^.{94}$/ }
                        maximumLength: 94
                        selectByMouse: true
                        selectionColor: Constants.currentTheme ? "#98DAFF": "#1E9BE2"
                        selectedTextColor: color
                        leftPadding: 10 * screenScaleFactor
                        rightPadding: 10 * screenScaleFactor
                        verticalAlignment: TextInput.AlignVCenter
                        horizontalAlignment: TextInput.AlignLeft
                        color: textColor
                        font.weight: Font.Medium
                        font.family: Constants.mySystemFont.name
                        font.pointSize: Constants.labelFontPointSize_9
                        focus:true
                        background: Rectangle {
                            color: "transparent"
                            border.color: Constants.currentTheme ? "#DDDDE1" : "#6E6E72"
                            border.width: 1
                            radius: 5
                        }
                    }
                }

                RowLayout {
                    spacing: 10 * screenScaleFactor
                    Layout.alignment: Qt.AlignHCenter

                    Repeater {
                        model: ListModel
                        {
                            ListElement { modelData: QT_TR_NOOP("Confirm") }
                            ListElement { modelData: QT_TR_NOOP("Cancel")  }
                        }
                        delegate: ItemDelegate
                        {
                            implicitWidth: 120 * screenScaleFactor
                            implicitHeight: 28 * screenScaleFactor

                            opacity: enabled ? 1.0 : 0.7
                            enabled: index || idGcodeFileName.text.length

                            background: Rectangle {
                                color: parent.hovered ? (Constants.currentTheme ? "#E8E8ED" : "#59595D") : (Constants.currentTheme ? "#FFFFFF" : "#6E6E73")
                                border.color: Constants.currentTheme ? "#DDDDE1" : "transparent"
                                border.width: Constants.currentTheme ? 1 : 0
                                radius: parent.height / 2
                            }

                            contentItem: Text
                            {
                                font.weight: Font.Medium
                                font.family: Constants.mySystemFont.name
                                font.pointSize: Constants.labelFontPointSize_9
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                color: textColor
                                text: qsTr(modelData)
                            }

                            onClicked: {
                                if(!index) gcodeFileName = idGcodeFileName.text
                                index ? sigCancelBtnClicked() : sigConfirmBtnClicked()
                            }
                        }
                    }
                }
            }

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 8 * screenScaleFactor
                visible: curWindowState === UploadGcodeDlg.WindowState.State_Progress

                Text {
                    Layout.preferredWidth: contentWidth * screenScaleFactor
                    Layout.preferredHeight: 28 * screenScaleFactor
                    Layout.alignment: Qt.AlignHCenter

                    font.weight: Font.Medium
                    font.family: Constants.mySystemFont.name
                    font.pointSize: Constants.labelFontPointSize_9
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    color: textColor
                    text: uploadProgress + "%"
                }

                ProgressBar {
                    Layout.preferredWidth: 380 * screenScaleFactor
                    Layout.preferredHeight: 2 * screenScaleFactor

                    from: 0
                    to: 100
                    value: uploadProgress

                    background: Rectangle {
                        color: Constants.currentTheme ? "#D6D6DC" : "#38383B"
                        radius: 1
                    }

                    contentItem: Rectangle {
                        width: parent.visualPosition * parent.width
                        height: parent.height
                        color: Constants.currentTheme ? "#00A3FF" : "#1E9BE2"
                    }
                }
            }

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 20 * screenScaleFactor
                visible: curWindowState === UploadGcodeDlg.WindowState.State_Message

                RowLayout {
                    spacing: 10 * screenScaleFactor
                    Layout.alignment: Qt.AlignHCenter

                    Image {
                        id: idMessageImage
                        sourceSize: Qt.size(24, 24)
                        source: root.msgImageSource
                    }

                    Text {
                        id: idMessageText
                        font.weight: Font.Normal
                        font.family: Constants.mySystemFont.name
                        font.pointSize: Constants.labelFontPointSize_9
                        verticalAlignment: Text.AlignVCenter
                        color: textColor
                        text: root.errorTxt
                    }
                }

                RowLayout {
                    spacing: 10 * screenScaleFactor

                    Repeater {
                        model: ListModel
                        {
                            ListElement { modelData: QT_TR_NOOP("View My Uploads") }
                            ListElement { modelData: QT_TR_NOOP("Cloud Printing")  }
                        }
                        delegate: ItemDelegate
                        {
                            implicitWidth: 120 * screenScaleFactor
                            implicitHeight: 28 * screenScaleFactor

                            background: Rectangle {
                                color: parent.hovered ? (Constants.currentTheme ? "#E8E8ED" : "#59595D") : (Constants.currentTheme ? "#FFFFFF" : "#6E6E73")
                                border.color: Constants.currentTheme ? "#DDDDE1" : "transparent"
                                border.width: Constants.currentTheme ? 1 : 0
                                radius: parent.height / 2
                            }

                            contentItem: Text
                            {
                                font.weight: Font.Medium
                                font.family: Constants.mySystemFont.name
                                font.pointSize: Constants.labelFontPointSize_9
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                color: textColor
                                text: qsTr(modelData)
                            }

                            onClicked:
                            {
                                if(index)
                                {
                                    root.visible = false
                                    cxkernel_cxcloud.printerService.openCloudPrintWebpage()
                                }
                                else {
                                    root.visible = false
                                    kernel_ui.invokeCloudUserInfoFunc("setUserInfoDlgShow", "mySlice")
                                }
                            }
                        }
                    }
                }
            }
        }

        layer.enabled: true

        layer.effect: DropShadow {
            radius: 8
            spread: 0.2
            samples: 17
            color: shadowColor
        }
    }
}
