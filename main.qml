import QtQuick 2.6
import QtQuick.Window 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

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
    RowLayout {
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
            text: "Click me!"
            onClicked: {
                ctrl.update(5, 5, 10, 100);

                //console.log(ctrl.Cells)
                //console.log(ctrl.Cells.length)
                //console.log(ctrl.Cells[0].start_x)
            }
        }
        Button {
            text: "Emitter!"
            onClicked: {
                ctrl.changeActive1();
                //console.log(ctrl.Cells)
                //console.log(ctrl.Cells.length)
                //console.log(ctrl.Cells[0].start_x)
            }
        }
        Button {
            text: "Wall1!"
            onClicked: {
                ctrl.changeActive2();

                //console.log(ctrl.Cells)
                //console.log(ctrl.Cells.length)
                //console.log(ctrl.Cells[0].start_x)
            }
        }
        Button {
            text: "Wall2!"
            onClicked: {
                ctrl.changeActive3();

                //console.log(ctrl.Cells)
                //console.log(ctrl.Cells.length)
                //console.log(ctrl.Cells[0].start_x)
            }
        }
        Button {
            text: "Wall3!"
            onClicked: {
                ctrl.changeActive4();

                //console.log(ctrl.Cells)
                //console.log(ctrl.Cells.length)
                //console.log(ctrl.Cells[0].start_x)
            }
        }

        TextField {
            id: tedit
            width: 300
            height: 30
            placeholderText: "Enter emitter force:"
            validator: IntValidator
            {
                bottom: 1
                top: 150
            }
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
                        ctrl.update(model.x, model.y, 10, parseInt(tedit.text));
                    }
                }
            }
        }


    }

}
