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
            lview.model = ctrl
            //repa.model = ctrl
        }
    }
    Button {
        text: "Click me!"
        onClicked: {
            ctrl.gen(1000, 1000,  4);

            //console.log(ctrl.Cells)
            //console.log(ctrl.Cells.length)
            //console.log(ctrl.Cells[0].start_x)
        }
    }

    ScrollView {
        x: 100
        y: 100
        width: 500
        height: 500
        clip: true

        ListView {
            id: lview
            x: 200
            y: 100
            width: 100; height: 1000
            model: ctrl

            delegate: Rectangle {
                //required start_x
                //required start_y

                height: 25
                width: 100
                Text { text: model.start_x }
            }
        }
    }

//    Repeater {
//        x: 100
//        y: 100
//        id: repa
//        //required model
//        model: ctrl


//        delegate: Rectangle {
//            //required start_x

//            width: 100
//            height: 100
//            color: "red"
//            Text {
//                id: name
//                //text: qsTr(start_x.toString())
//            }
//        }
//    }
}
