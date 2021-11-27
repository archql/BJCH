import QtQuick 2.6
import QtQuick.Window 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import controlmodel 1.0


Window {
    id: window
    width: Screen.width
    height: Screen.height
    visible: true

    property string selectedItem: "Air"
    property string currentTask: "task_1"
    property bool admin_access: false


    RadialGradient
        {
            anchors.fill: parent
            gradient: Gradient{
            GradientStop {position: 0.000;color: Qt.rgba(0.9, 0.9, 0.9, 1);}
            GradientStop {position: 0.500;color: Qt.rgba(0.4, 0.4, 0.4, 1);}
            }
    }
    Control {
        id: ctrl
        Component.onCompleted:
        {
            admin_access = requestAdminKey()
            myconsole.message("admin powers? "+ admin_access, "orange")
            myconsole.message("task loaded? "+ ctrl.loadTask(currentTask), "pink")
    // temp
        }
        onMapReady: {
            //lview.model = ctrl
            //repa.model = ctrl
            //scroll.contentWidth = repa.width    // The important part
            //scroll.contentHeight = repa.height
            //console.log(repa.width)

        }
        onDataChanged:
        {
            console.log("Data changed!")
        }

        onCellChanged: {
            if (repa.count == 0)
                return
            repa.itemAt(c.index).texname = Number((c.wstate >> 1) & 0x55);
            console.log("cell changed at ", c.index, " with wallstate ", c.wstate)
        }
    }

    GridLayout {
        id: grid
        anchors.fill: parent
        columnSpacing: 20
        rowSpacing: 20

        rows: 8
        columns: 8

        property double colMulti : grid.width / grid.columns
        property double rowMulti : grid.height / grid.rows
        function prefWidth(item){
            return colMulti * item.Layout.columnSpan
        }
        function prefHeight(item){
            return rowMulti * item.Layout.rowSpan
        }

        Button {
            id: b_question

            text: "?"

            Layout.columnSpan   : 1
            Layout.column       : 0
            Layout.rowSpan      : 1
            Layout.row          : 0

            Layout.preferredWidth  : grid.prefWidth(this)
            Layout.preferredHeight : grid.prefHeight(this)

            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        Rectangle {
            id: tasktextrect
            Layout.columnSpan   : 5
            Layout.column       : 1
            Layout.rowSpan      : 1
            Layout.row          : 0

            Layout.preferredWidth  : grid.prefWidth(this)
            Layout.preferredHeight : grid.prefHeight(this)

            Layout.fillHeight: true
            Layout.fillWidth: true

            color: "#A0A0A0A0"

            ScrollView {
                clip: true
                id: scrollView
                anchors.fill: parent

                contentWidth: tasktext.width    // The important part
                contentHeight: tasktext.height



                Text {
                    id: tasktext
                    text: "<b>Your task is going to appear here</b>"
                    wrapMode: Text.WordWrap
                    font.pointSize: 14
                    width: tasktextrect.width
                }
            }
        }
        Rectangle {

            Text {
                anchors.centerIn: parent
                text: qsTr("00:00:00")
                font.pixelSize: parent.height / 2
            }

            Layout.columnSpan   : 2
            Layout.column       : 6
            Layout.rowSpan      : 1
            Layout.row          : 0

            Layout.preferredWidth  : grid.prefWidth(this)
            Layout.preferredHeight : grid.prefHeight(this)

            Layout.fillHeight: true
            Layout.fillWidth: true

            color: "#A0A0A0A0"
        }
        ColumnLayout {
            Layout.columnSpan   : 1
            Layout.column       : 0
            Layout.rowSpan      : 6
            Layout.row          : 1

            Layout.preferredWidth  : grid.prefWidth(this)
            Layout.preferredHeight : grid.prefHeight(this)

            Layout.fillHeight: true
            Layout.fillWidth: true

            spacing: 10
            Layout.alignment: Qt.AlignTop

            Button {
                text: "Click me!"
                visible: admin_access
                onClicked: {
                    repa.width = 21 * 60;
                    repa.height = 13 * 60;
                    ctrl.gen(21, 13);

                    myconsole.message("gen with w25 h20", "gray")
                }
            }
            Button {
                text: "Update"
                onClicked: {
                    myconsole.message("simulation update emitted, took " + Number(ctrl.update()) + "ms", "gray")
                }
            }
            Button {
                text: "Emitter!"
                visible: admin_access
                onClicked: {
                    selectedItem = "Emitter"
                }
            }
            Button {
                text: "Wall1!"
                onClicked: {
                    selectedItem = "Wall1"
                }
            }
            Button {
                text: "Wall2!"
                onClicked: {
                    selectedItem = "Wall2"
                }
            }
            Button {
                text: "Wall3!"
                onClicked: {
                    selectedItem = "Wall3"
                }
            }
            Button {
                text: "Wall4!"
                onClicked: {
                    selectedItem = "Wall4"
                }
            }
            Button {
                text: "WorkPlace"
                onClicked: {
                    selectedItem = "Workplace"
                }
            }
            Button {
                text: "ToggleLock"
                visible: admin_access
                onClicked: {
                    selectedItem = "LOCKED"
                }
            }

            CheckBox
            {
                id: chooseOverlay
                text: "switch noise overlay"
            }
            CheckBox
            {
                id: chooseLock
                text: "switch lock overlay"
            }

            TextField {
                id: tedit
                width: 300
                height: 30
                placeholderText: "Enter emitter force:"
                visible: admin_access
                validator: IntValidator
                {
                    bottom: 1
                    top: 150
                }
            }

            TextField {
                id: fedit
                width: 300
                height: 30
                placeholderText: "Enter file name:"
                visible: admin_access
            }

            Button {
                visible: admin_access
                text: "Save field as shematic"
                onClicked: {
                    var filename = fedit.text
                    if (filename !== "" || filename !== "true" || filename !== "false")
                        myconsole.message("file saved? " + ctrl.saveToFile(filename), "pink")
                    else
                        myconsole.message("file operation error: bad filename!", "red")
                }
            }
            Button {
                visible: admin_access
                text: "Load field as shematic"
                onClicked: {
                    var filename = fedit.text
                    if (filename !== "" || filename !== "true" || filename !== "false")
                        myconsole.message("file loaded? " + ctrl.ldFromFile(filename), "pink")
                    else
                        myconsole.message("file operation error: bad filename!", "red")
                }
            }
            TextField {
                id: taskedit
                width: 300
                height: 30
                placeholderText: "Enter task formated file name:"
                visible: admin_access
            }
            Button {
                visible: admin_access
                text: "Create new task"
                onClicked: {
                    var filename = taskedit.text
                    if (filename !== "" || filename !== "true" || filename !== "false")
                        myconsole.message("task created? " +ctrl.parseTasks(filename), "pink")
                }
            }
        }

        ScrollView {
            id: scroll
            clip: true
            contentWidth: repa.width    // The important part
            contentHeight: repa.height

            Layout.columnSpan   : 5
            Layout.column       : 1
            Layout.rowSpan      : 6
            Layout.row          : 1

            Layout.preferredWidth  : grid.prefWidth(this)
            Layout.preferredHeight : grid.prefHeight(this)

            Layout.fillHeight: true
            Layout.fillWidth: true


            Repeater {
                id: repa
                //required model
                model: ctrl

                delegate: Image {
                    //required start_x

                    source: "sources/textures/floor_" + texname + ".jpg"
                    cache: true

                    property string texname: Number((model.wstate >> 1) & 0x55)

                    width: 60
                    height: 60
                    x: model.x * width
                    y: model.y * height
                    //color: model.color
                    onSourceChanged:
                    {
                        //myconsole.message("SRC change " + texname + " wallstate " + Number(model.wstate), "gray")
                    }

                    MouseArea {
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        anchors.fill: parent
                        onClicked: {
                            //model.noise = 30
                            //ctrl.update(model.x, model.y, 10, parseInt(tedit.text));
                            if (!admin_access && model.locked)
                            {
                                myconsole.message("cell at " + Number(index) + " is locked!", "red")
                                return
                            }
                            if (selectedItem == "LOCKED")
                            {
                                model.locked ^= 1
                                return
                            }
                            var ifAdd = mouse.button == Qt.LeftButton
                            if (selectedItem == "Emitter")
                                ctrl.resetEmitter(index, ifAdd, parseInt(tedit.text))
                            model.typeOfCell = ifAdd ? selectedItem : "Air"
                            myconsole.message("cell at " + Number(index) + " type changed: " + selectedItem, "gray")
                        }
                    }
                    Image {
                        id: texture_up
                        anchors.fill: parent
                        source: "sources/textures/tex_" + model.typeOfCell + ".png"
                        //visible: model.typeOfCell !== "Air"
                        cache: true
                        onSourceChanged: {
                            //console.log("BBB ")
                        }
                    }
                    Image {
                        id: texture_locked
                        anchors.fill: parent
                        source: "sources/textures/lock.png"
                        visible: model.locked & chooseLock.checked
                        cache: true
                    }
                    Text {
                        id: name
                        font.bold: true

                        font.pixelSize: parent.width * 0.6
                        color: model.color
                        text: qsTr(Math.round(noise).toString())
                        anchors.centerIn: parent
                        //anchors.fill: parent
                        style: Text.Outline; styleColor: "black"

                        onTextChanged: {
                            color =  model.color
                            overlay.color =  model.color
                        }
                        visible: chooseOverlay.checked
                    }
                    Rectangle {
                        id: overlay
                        anchors.fill: parent
                        opacity: 0.35
                        visible: chooseOverlay.checked
                    }



                }
            }


        }

        ListView {
            id: taskview

            clip: true
            Layout.columnSpan   : 2
            Layout.column       : 6
            Layout.rowSpan      : 6
            Layout.row          : 1

            Layout.preferredWidth  : grid.prefWidth(this)
            Layout.preferredHeight : grid.prefHeight(this)

            Layout.fillHeight: true
            Layout.fillWidth: true

            model: 10
            spacing: 10

            delegate: Rectangle
            {
                property string task_text: ""
                property color text_color: "black"
                color: "#A0A0A0A0"
                width: parent.width
                height: 60
                Text {
                    id: el_task_text
                    font.pixelSize: 20
                    anchors.centerIn: parent
                    text: task_text
                    wrapMode: Text.WordWrap
                    color: text_color
                }
            }
        }

        Button {
            id: b_restart

            text: "restart"

            Layout.columnSpan   : 1
            Layout.column       : 0
            Layout.rowSpan      : 1
            Layout.row          : 7

            Layout.preferredWidth  : grid.prefWidth(this)
            Layout.preferredHeight : grid.prefHeight(this)

            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        Console {
            id: myconsole

            Layout.columnSpan   : 5
            Layout.column       : 1
            Layout.rowSpan      : 1
            Layout.row          : 7

            Layout.preferredWidth  : grid.prefWidth(this)
            Layout.preferredHeight : grid.prefHeight(this)

            Layout.fillHeight: true
            Layout.fillWidth: true

            color: "#F0F0F0F0"
        }
        Button {
            id: b_check

            text: "check"

            Layout.columnSpan   : 2
            Layout.column       : 6
            Layout.rowSpan      : 1
            Layout.row          : 7

            Layout.preferredWidth  : grid.prefWidth(this)
            Layout.preferredHeight : grid.prefHeight(this)

            Layout.fillHeight: true
            Layout.fillWidth: true

            onClicked:
            {
                ctrl.update()
                // check conditions
                ctrl.checkCurTasks()
                var tasks = ctrl.tasks
                taskview.model = tasks.length
                for (var i = 0; i < tasks.length; i ++)
                {
                    taskview.itemAtIndex(i).task_text = tasks[i].getDescription()
                    taskview.itemAtIndex(i).text_color = tasks[i].completed() ? "green" : "black"
                }
            }
        }
    }
//    Console
//    {
//        width: scroll.width
//        height: 100
//    }
}
