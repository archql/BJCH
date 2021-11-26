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

    property string selectedItem: "Air"

    Control {
        id: ctrl
        onMapReady: {
            //lview.model = ctrl
            //repa.model = ctrl
            scroll.contentWidth = repa.width    // The important part
            scroll.contentHeight = repa.height
            console.log(repa.width)


        }
        onDataChanged:
        {
            // TEMP
            //if (role === Qt.UserRole)
                //for (i = 0; i < repa.count; i++)
//                    repa.itemAt(i).
            console.log("Data changed!")
        }

        onCellChanged: {
            if (repa.count == 0)
                return
            repa.itemAt(c.index).texname = Number((c.wstate >> 1) & 0x55);
            console.log("Changed! ", c.wstate, " index ", c.index)
        }
    }
    RowLayout {
        Button {
            text: "Click me!"
            onClicked: {
                repa.width = 25 * 60;
                repa.height = 20 * 60;
                ctrl.gen(25, 20);

                //console.log(ctrl.Cells)
                //console.log(ctrl.Cells.length)
                //console.log(ctrl.Cells[0].start_x)
            }
        }
        Button {
            text: "Update"
            onClicked: {
                ctrl.update();

                //console.log(ctrl.Cells)
                //console.log(ctrl.Cells.length)
                //console.log(ctrl.Cells[0].start_x)
            }
        }
        Button {
            text: "Emitter!"
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

        CheckBox
        {
            id: chooseOverlay
            text: "switch noise overlay"
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

        TextField {
            id: fedit
            width: 300
            height: 30
            placeholderText: "Enter file name:"
        }

        Button {
            text: "Save field as shematic"
            onClicked: {
                var filename = fedit.text
                if (filename !== "" || filename !== "true" || filename !== "false")
                    fedit.text = ctrl.saveToFile(filename)
            }
        }
        Button {
            text: "Load field as shematic"
            onClicked: {
                var filename = fedit.text
                if (filename !== "" || filename !== "true" || filename !== "false")
                    fedit.text = ctrl.ldFromFile(filename)
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
                    console.log("SRC change ", texname, " wallstate ", model.wstate)
                }

                MouseArea {
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    anchors.fill: parent
                    onClicked: {
                        //model.noise = 30
                        //ctrl.update(model.x, model.y, 10, parseInt(tedit.text));
                        var ifAdd = mouse.button == Qt.LeftButton
                        if (selectedItem == "Emitter")
                            ctrl.resetEmitter(index, ifAdd, parseInt(tedit.text))
                        model.typeOfCell = ifAdd ? selectedItem : "Air"
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

}
