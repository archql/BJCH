import QtQuick 2.0
import QtQuick.Window 2.3
import QtQuick.Controls 2.5

import controlmodel 1.0


Window {
    id: window
    width: Screen.width
    height: Screen.height
    visible: true
    Control {
        id: ctrl
        onMapReady: {
            //lview.model = ctrl
            //repa.model = ctrl
            scroll.contentWidth = repa.width    // The important part
            scroll.contentHeight = repa.height
            console.log(repa.width)
        }
    }
    Button {
        text: "Click me!"
        onClicked: {
            repa.width = 40 * 50;
            repa.height = 40 * 50;
            ctrl.gen(40, 40);

            //console.log(ctrl.Cells)
            //console.log(ctrl.Cells.length)
            //console.log(ctrl.Cells[0].start_x)
        }
    }
    Button {
        x: 100
        text: "Click me!"
        onClicked: {
            ctrl.update(5, 5, 10, 100);

            //console.log(ctrl.Cells)
            //console.log(ctrl.Cells.length)
            //console.log(ctrl.Cells[0].start_x)
        }
    }
    Button {
        x: 400
        text: "Emitter!"
        onClicked: {
            ctrl.changeActive1();

            //console.log(ctrl.Cells)
            //console.log(ctrl.Cells.length)
            //console.log(ctrl.Cells[0].start_x)
        }
    }
    Button {
        x: 500
        text: "Wall1!"
        onClicked: {
            ctrl.changeActive2();

            //console.log(ctrl.Cells)
            //console.log(ctrl.Cells.length)
            //console.log(ctrl.Cells[0].start_x)
        }
    }
    Button {
        x: 600
        text: "Wall2!"
        onClicked: {
            ctrl.changeActive3();

            //console.log(ctrl.Cells)
            //console.log(ctrl.Cells.length)
            //console.log(ctrl.Cells[0].start_x)
        }
    }
    Button {
        x: 700
        text: "Wall3!"
        onClicked: {
            ctrl.changeActive4();

            //console.log(ctrl.Cells)
            //console.log(ctrl.Cells.length)
            //console.log(ctrl.Cells[0].start_x)
        }
    }
    ScrollView {
        id: scroll
        x: 100
        y: 100
        width: parent.width - x * 2
        height: parent.height - y * 2
        clip: true
        contentWidth: repa.width    // The important part
        contentHeight: repa.height

        Repeater {
            id: repa
            //required model
            model: ctrl


            delegate: Rectangle {
                //required start_x

                width: 50
                height: 50
                x: model.x * width
                y: model.y * height
                color: model.color
                Text {
                    id: name
                    //onFontSizeModeChanged: 25
                    text: qsTr(Math.round(noise).toString())
                }
                MouseArea {
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    anchors.fill: parent
                    onClicked: {
                        //model.noise = 30
                        ctrl.update(model.x, model.y, 10, 40);
                    }
                }
            }
        }


    }

}
