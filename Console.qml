import QtQuick 2.0
import QtQuick.Controls 2.2
import Qt.labs.folderlistmodel 2.13

//============================================================
//========THIS IS MY QML CONSOLE IMPLEMENTATION===============
//=================version 2.12==============================
//============================================================

Item {
    // array that hold all messages from command emition and its colors
    property var history: [["#" + getCurrentTime(
                                ), "darkorange"], ["#history", "darkorange"]]
    property var used_commands: []
    property int used_numb: -1 // for saved commands list

    property int predicted_numb: -1
    property string predicted_usertext: ""

    property color color

    // CUT CONSOLE VERSION
    // FUNCS LOCKED
    // -apply:
    // -applyChanges
    //

    //basic message is current time and "history" text

    //============================================================
    //========Functions descr=========================
    //============================================================
    //==============="public"================================
    // message(message,color) - write message in history with specified color (and apply it)
    //==================="private"============================
    // applyChanges() - adopt history array and ListView to new consequences
    // clear() - clears history
    // getCurrentTime() - ret date string
    // used_add(cmd) and used_see(numb) commands for used_commands[] ruling:
    //         used_add - this adds cmd to array and used_see switch text in TextField
    // acceptKey() - decodes keys
    // predicted_see() - ret predict command to current input text
    //=======================================================
    //=======================================================

    //============================================================
    //========Design and text inputer/outputer====================
    //===========(Dont touch this!)===============================
    //text input field
    //===========
    //history field
    Rectangle {
        //history field
        //===========
        width: parent.width
        height: parent.height
        color: parent.color
        ScrollView {
            clip: true
            id: scrollView
            anchors.fill: parent
            ListView {
                id: listofhistory
                verticalLayoutDirection: ListView.BottomToTop
                model: history.length
                delegate: Text {

                    // This (history.length - index - 1) is address of cell, which text will placed in ListView
                    //  history.length-1 is max cells id
                    text: history[history.length - index - 1][0]
                          === null ? "" : history[history.length - index - 1][0]
                    color: history[history.length - index
                                   - 1][1] //This checks history cell empty or not
                           === null ? "" : history[history.length - index - 1][1]
                }
            }
        }
    }


    //============================================================
    //========"Public" Functions=========================
    //============================================================

    function message(message, color) {
        history.push([message, color])
        applyChanges()
    }

    //============================================================
    //========"Private" Functions=========================
    //============================================================

    //applyChanges in console
    function applyChanges() {
        if (history.length >= 100) {
            //if history is too big, clear it
            clear()
        }
        listofhistory.model = history.length //make size of list same as history
        listofhistory.forceLayout() //force redraw all texts
    }

    //clears history
    function clear() {
        history.length = 0
        history.push(["#clear success " + getCurrentTime(), "darkorange"])
    }

    function getCurrentTime() {
        return new Date().toLocaleString(Qt.locale(), "yyyy.MM.dd hh:mm:ss")
    }
} //======================================
