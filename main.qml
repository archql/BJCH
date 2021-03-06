import QtQuick 2.6
import QtQuick.Window 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import controlmodel 1.0


ApplicationWindow  {
    id: window
    width: Screen.width
    height: Screen.height
    visible: true

    visibility: "Maximized"

    property int rect_width: 60

    property int font_size: 20

    property string selectedItem: "Air"

    property bool admin_access: false
    property bool all_completed: false
    property bool help_enabled: true
    property bool tsk_text_enabled: false
    
    property var arrayOfTasks: ["task_1", "task_2", "task_3"]
    property int curTask: 0

    function qmlSaveTaskResult()
    {
        ctrl.saveTaskCompletion(arrayOfTasks[curTask])
    }
    function qmlLoadTask()
    {
        if (curTask >= arrayOfTasks.length) // check if all completed
        {
            all_completed = true
            tasktext.text = "Все задачи выполнены! Поздравляем! <br> Можете возвращаться к тесту."
        }
        else
        {

            myconsole.message("loading task no " + Number(curTask), "deeppink");
            myconsole.message("task loaded? "+ ctrl.loadTask(arrayOfTasks[curTask]), "deeppink")
            tasktext.text = ctrl.getTaskDescr(arrayOfTasks[curTask])
        }
    }
    function qmlUpdateTasksView()
    {
        myconsole.message("sim run, took  "+ Number(ctrl.update()) +"ms", "deeppink");
        myconsole.message("checking task... ", "deeppink");
        ctrl.checkCurTasks()
        var tasks = ctrl.tasks
        taskview.model = tasks.length

        var result = tasks.length !== 0;
        for (var i = 0; i < tasks.length; i ++)
        {
            var completed = tasks[i].completed()
            result &= completed

            myconsole.message("subtask " + Number(i) + " completed? " + completed, "deeppink");
            taskview.itemAtIndex(i).task_text = tasks[i].getDescription()
            taskview.itemAtIndex(i).text_color = completed ? "green" : "black"
        }
        return result
    }
    function qmlClearTasksCompletion()
    {
        for (var i = 0; i < arrayOfTasks.length; i ++)
            ctrl.deleteTaskComletion(arrayOfTasks[i])
    }

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
            myconsole.message("ld user id? " + requestUserId(), "darkorange")
            admin_access = requestAdminKey()
            myconsole.message("admin powers? "+ admin_access, "darkorange")

            qmlClearTasksCompletion()


            qmlLoadTask()
            qmlUpdateTasksView()
        }
        onMapReady: { 
            console.log("onMapReady")
            repa.width = ctrl.getWidth() * rect_width
            repa.height = ctrl.getHeight() * rect_width
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

        visible: !help_enabled

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

        onHeightChanged: { font_size = height / 8 / 6; }

        Button {
            id: b_question

            //text: "?"

            Layout.columnSpan   : 1
            Layout.column       : 0
            Layout.rowSpan      : 1
            Layout.row          : 0

            Layout.preferredWidth  : grid.prefWidth(this)
            Layout.preferredHeight : grid.prefHeight(this)

            Layout.fillHeight: true
            Layout.fillWidth: true

            //font.pixelSize: font_size

            onClicked: help_enabled = !help_enabled

            icon.source: "sources/textures/question.png"
            icon.color: "transparent"
            icon.width: font_size * 4
            icon.height: font_size * 4
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

                ScrollBar.vertical: ScrollBar {
                        parent: scrollView
                        active: true
                        policy: ScrollBar.AlwaysOn
                        x: scrollView.mirrored ? 0 : scrollView.width - width
                        height: scrollView.availableHeight
                        contentItem: Rectangle {
                                implicitWidth: 12
                                color: "black"
                            }

                }

                Text {
                    id: tasktext
                    text: "<b>Ваше задание будет здесь!</b>"
                    wrapMode: Text.WordWrap
                    font.pixelSize: font_size
                    width: tasktextrect.width

                    MouseArea {
                        anchors.fill: parent
                        onDoubleClicked: {
                            tsk_text_enabled = true
                        }
                    }
                }
            }
        }
        Rectangle {

            Text {
                anchors.centerIn: parent
                text: "Прогресс: " + Number(curTask) + "/" + Number(arrayOfTasks.length)
                font.pixelSize: font_size * 2
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
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pixelSize: font_size
                onClicked: {
                    ctrl.gen(15, 15);

                    myconsole.message("gen with w25 h20", "gray")
                }
            }
            Button {
                text: "Update"
                visible: admin_access
                Layout.fillWidth: true
                font.pixelSize: font_size
                Layout.fillHeight: true
                onClicked: {
                    myconsole.message("simulation update emitted, took " + Number(ctrl.update()) + "ms", "gray")
                }
            }
            Button {
                text: "Emitter!"
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pixelSize: font_size
                visible: admin_access
                onClicked: {
                    selectedItem = "Emitter"
                }
                icon.source: "sources/textures/tex_Emitter.png"
                icon.color: "transparent"
                icon.width: font_size * 2
                icon.height: font_size * 2
            }
            Button {
                text: "Стеклянная панель"
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pixelSize: font_size
                onClicked: {
                    selectedItem = "Wall1"
                }
                icon.source: "sources/textures/tex_Wall1.png"
                icon.color: "transparent"
                icon.width: font_size * 2
                icon.height: font_size * 2
            }
            Button {
                text: "Пористый блок"
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pixelSize: font_size
                onClicked: {
                    selectedItem = "Wall2"
                }
                icon.source: "sources/textures/tex_Wall2.png"
                icon.color: "transparent"
                icon.width: font_size * 2
                icon.height: font_size * 2
            }
            Button {
                text: "Дерево"
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pixelSize: font_size
                onClicked: {
                    selectedItem = "Wall3"
                }
                icon.source: "sources/textures/tex_Wall3.png"
                icon.color: "transparent"
                icon.width: font_size * 2
                icon.height: font_size * 2
            }
            Button {
                text: "Бетонный блок"
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pixelSize: font_size
                onClicked: {
                    selectedItem = "Wall4"
                }
                icon.source: "sources/textures/tex_Wall4.png"
                icon.color: "transparent"
                icon.width: font_size * 2
                icon.height: font_size * 2
            }
            Button {
                text: "Рабочее место"
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pixelSize: font_size
                onClicked: {
                    selectedItem = "Workplace"
                }
                icon.source: "sources/textures/tex_Workplace.png"
                icon.color: "transparent"
                icon.width: font_size * 2
                icon.height: font_size * 2
            }
            Button {
                text: "ToggleLock"
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pixelSize: font_size
                visible: admin_access
                onClicked: {
                    selectedItem = "LOCKED"
                }
                icon.source: "sources/textures/lock.png"
                icon.color: "transparent"
                icon.width: font_size * 2
                icon.height: font_size * 2
            }

            CheckBox
            {
                id: chooseOverlay
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pixelSize: font_size
                text: "просмотр шумового загрязнения"

                contentItem: Text {
                        text: parent.text
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: parent.indicator.width + parent.spacing
                        wrapMode: Text.WordWrap
                        font.pixelSize: font_size
                    }
            }
            CheckBox
            {
                id: chooseLock
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pixelSize: font_size
                text: "просмотр заблокированных клеток"

                contentItem: Text {
                        text: parent.text
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: parent.indicator.width + parent.spacing
                        wrapMode: Text.WordWrap
                        font.pixelSize: font_size
                    }
            }

            TextField {
                id: tedit
                width: 300
                height: 30
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pixelSize: font_size
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
                Layout.fillWidth: true
                Layout.fillHeight: true
                font.pixelSize: font_size
                placeholderText: "Enter file name:"
                visible: admin_access
            }

            Button {
                visible: admin_access
                Layout.fillHeight: true
                Layout.fillWidth: true
                font.pixelSize: font_size
                text: "Save field as shematic"
                onClicked: {
                    var filename = fedit.text
                    if (filename !== "" || filename !== "true" || filename !== "false")
                        myconsole.message("file saved? " + ctrl.saveToFile(filename), "deeppink")
                    else
                        myconsole.message("file operation error: bad filename!", "red")
                }
            }
            Button {
                visible: admin_access
                Layout.fillHeight: true
                Layout.fillWidth: true
                font.pixelSize: font_size
                text: "Load field as shematic"
                onClicked: {
                    var filename = fedit.text
                    if (filename !== "" || filename !== "true" || filename !== "false")
                        myconsole.message("file loaded? " + ctrl.ldFromFile(filename), "deeppink")
                    else
                        myconsole.message("file operation error: bad filename!", "red")
                }
            }
            TextField {
                id: taskedit
                Layout.fillHeight: true
                Layout.fillWidth: true
                width: 300
                height: 30
                placeholderText: "Enter task formated file name:"
                visible: admin_access
            }
            Button {
                visible: admin_access
                Layout.fillHeight: true
                Layout.fillWidth: true
                font.pixelSize: font_size
                text: "Create new task"
                onClicked: {
                    var filename = taskedit.text
                    if (filename !== "" || filename !== "true" || filename !== "false")
                        myconsole.message("task created? " +ctrl.parseTasks(filename), "deeppink")
                }
            }
        }

        ScrollView {
            id: scroll
            clip: true
            contentWidth: repa.width    // The important part
            contentHeight: repa.height

            onContentWidthChanged:
            {
                console.log("Content width ", contentWidth)
            }

            Layout.columnSpan   : 5
            Layout.column       : 1
            Layout.rowSpan      : 6
            Layout.row          : 1

            Layout.preferredWidth  : grid.prefWidth(this)
            Layout.preferredHeight : grid.prefHeight(this)

            Layout.fillHeight: true
            Layout.fillWidth: true

            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOn


            Repeater {
                id: repa
                //required model
                model: ctrl

                onWidthChanged:
                {
                    console.log("repa width ", width)
                }


                delegate: Image {
                    //required start_x

                    source: "sources/textures/floor_" + texname + ".jpg"
                    cache: true

                    property string texname: Number((model.wstate >> 1) & 0x55)

                    width: rect_width
                    height: rect_width
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
                            if (model.typeOfCell === "Emitter" || selectedItem == "Emitter")
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

            ScrollBar.vertical: ScrollBar {
                    active: true
                    policy: ScrollBar.AlwaysOn
            }

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
                height: 110
                Text {
                    x: 10
                    id: el_task_text
                    //font.pixelSize: 20
                    width: parent.width - 2*x
                    text: task_text
                    wrapMode: Text.WordWrap
                    color: text_color

                    font.pixelSize: font_size
                }
            }
        }

        Button {
            id: b_restart

            text: "Начать задание сначала"

            enabled: !all_completed

            Layout.columnSpan   : 1
            Layout.column       : 0
            Layout.rowSpan      : 1
            Layout.row          : 7

            Layout.preferredWidth  : grid.prefWidth(this)
            Layout.preferredHeight : grid.prefHeight(this)

            Layout.fillHeight: true
            Layout.fillWidth: true

            font.pixelSize: font_size

            onClicked: { qmlLoadTask() ; qmlUpdateTasksView() }
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

            property bool completed: false

            text: completed ? "Перейти к следующему заданию" : "Проверить выполнение"

            Layout.columnSpan   : 2
            Layout.column       : 6
            Layout.rowSpan      : 1
            Layout.row          : 7

            font.pixelSize: font_size

            Layout.preferredWidth  : grid.prefWidth(this)
            Layout.preferredHeight : grid.prefHeight(this)

            Layout.fillHeight: true
            Layout.fillWidth: true

            enabled: !all_completed

            onClicked:
            {
                if (completed)
                {
                    scrollView.ScrollBar.vertical.position = 0

                    qmlSaveTaskResult()
                    curTask ++
                    qmlLoadTask() 
                }
                if (!all_completed)
                {
                    completed = qmlUpdateTasksView()
                }

            }
        }
    }

    property var help_text_array: [
        "<b>Руководство состоит из 5-и частей</b><br>
        <b>Часть 1. Общая информация.</b><br>  Данное программное средство представляет из себя симулятор распространения звука в помещении.
            Интерфейс программы состоит из кнопки помощи (слева вверху), описания текущего задания (вверху), общего прогресса выполнения (справа вверху),
            панель редактирования (слева), панель просмотра плана помещения (в центре), панель списка задач (справа), консоль (снизу), и две кнопки управления. Кнопка помощи вызовет эту панель.
            Консоль предназначена для просмотра состояния программы. Поля с текстом в программе прокручиваются.
            Для проверки результатов выполнения, в основной программе (той, которая вызвала этот процесс, предназначена для выполнения тестовых заданий) нажмите конпку 'проверить выполнение'",
        "<b>Часть 2. Информация об интерфейсе.</b><br> Основное задание всегда расположено сверху. Двойное нажатие по нему откроет его в полный экран. Панель просмотра плана помещения (<i>план-схема</i>)
            состоит из отдельных <i>клеток</i> -- элементов план-схемы. Взаимодействие с ними осуществляется с помощью мыши. Нажатие левой кнопкой на клетке установит выбранный <i>блок</i>, правой -- уберет его.
            Выбор текущего блока осуществляется на панели выбора слева.
            Клетка может быть занята <i>блоком</i>, <i>рабочим местом</i> (РМ), <i>источником шума</i>, или быть свободной. Клетка может быть заблокирована. Для просмотра доступных клеток, выберете пункт 'просмотр заблокированных клеток'.
            Для просмотра измерений, сделанных с помощью прибора Экофизика 110А, выберете пункт 'просмотр шумового загрязнения'.
            Уровень шумового загрязнения отображается на план-схеме в каждой клетке с помощью цветовой палитры и значений в дБ.
            ",
        "<b>Часть 3. Информация об задачах.</b><br> В приложении создано 3 задачи. Каждая задача состоит из отдельных условий, находящихся на панели справа.
            Для успешного выполнения задачи, комбинируйте блоки и добейтесь выполнения всех условий в списке на панели справа (должны загореться зеленым).
            Прогресс выполнения условия обозначен ввиде записи: \"текущее значение параметра (А)\"/\"требуемое значение параметра (Б)\".
            Условий существует несколько типов. <i>Точное выполнение</i> (точно) -- означает, что для выполнения А должно быть равно Б.
            <i>Условное выполнение</i> (или более)/(или менее) -- означает, что для выполнения А может быть равно или больше/меньше Б.
            Вид параметра задаётся заданием. Пример: 'на РМ уровень шума меньше чем', параметр -- количество РМ, расположенных на план-схеме.
            ",
        "<b>Часть 4. Управление.</b><br>   Основная информация по управлению находится на анимированных картинках. <i>Каждый блок обладает уникальными свойствами</i>. Стеклянная панель отлично пропускает
            и отражает звук, пористый блок хорошо пропускает звук, но очень плохо его отражает. Дерево обладает средними свойствами. Блок бетона очень плохо пропускает звук, но отлично его отражает.
            Источник звука на карте обозначен оранжевым кругом с динамиком внутри.
            Рабочее место обозначено коричневым кругом с надписью РМ.",
        "<b>Часть 5. Выполнение.</b><br>  Чтобы приступить к работе, нажмите кнопку 'Перейти к выполнению'. Располагая блоки на план-схеме, добейтесь выполнения всех условий.
            Комбинируйте блоки для достижения найлучшего эффекта, постарайтесь отследить, как распростаняется звук.
            Дополнительную помощь можно найти в папке help, которая находится в папке с программой. После окончания выполнения задач, кнопки управления отключатся, и будет предложено перейти к основной программе для выполнения финального теста.
            До нажатия кнопки 'проверить выполнение' не завершайте работу программы. Если программа будет перезапущена, вы будете вынуждены начинать работу сначала."]

//    Если основная программа не распознаёт результаты, воспользуйтесь дополнительной утилитой в папке программы.
//                Она подтвердит Ваш результат для преподавателя.
    Pane {
        id: tsk_text_pane
        anchors.fill: parent
        //color: "#A0A0A0A0"
        ColumnLayout {
            anchors.fill: parent
            Text {
                text: tasktext.text
                wrapMode: Text.WordWrap
                font.pixelSize: font_size

                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.margins: 20
            }
            Button
            {
                text: "Вернуться к выполнению"

                Layout.preferredHeight: 100

                font.pixelSize: font_size

                onClicked: tsk_text_enabled = !tsk_text_enabled
            }
        }

        visible: tsk_text_enabled
    }

    Pane {
        id: hlp_pane
        visible: help_enabled
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent
            ListView {
                model: help_text_array.length
                clip: true
                Layout.fillWidth: true
                Layout.fillHeight: true
                ScrollBar.vertical: ScrollBar {
                        active: true
                        policy: ScrollBar.AlwaysOn
                        contentItem: Rectangle {
                                implicitWidth: 12
                                color: "black"
                            }

                }

                spacing: 30

                delegate: Rectangle {
                    RowLayout {
                        width: parent.width
                        spacing: 30
                        Text {
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                            Layout.leftMargin: 20
                            font.pixelSize: font_size * 0.9
                            //Layout.preferredWidth: hlp_pane.width / 3
                            text: help_text_array[index]
                        }
                        AnimatedImage
                        {
                            id: animation;
                            cache: true;
                            Layout.alignment: Qt.AlignRight
                            Layout.rightMargin: 60
                            //Layout.preferredWidth: width
                            source: "sources/anim/exp_gif_" + Number(index+1) + ".gif"
                        }
                    }
                    width: hlp_pane.width
                    height: animation.height
                    color: "#A0A0A0A0"
                }
            }
            Button
            {
                text: "Перейти к выполнению"
                //anchors.bottom: parent.bottom
                //anchors.horizontalCenter: parent.horizontalCenter

                //Layout.fillHeight: true
                Layout.alignment: Layout.Center

                Layout.preferredHeight: 100

                font.pixelSize: font_size

                onClicked: help_enabled = !help_enabled
            }
        }

//        SwipeView {
//            id: view

//            currentIndex: 1
//            anchors.fill: parent

//            Item {
//                id: firstPage
//            }
//            Item {
//                id: secondPage
//            }
//            Item {
//                id: thirdPage
//            }
//        }

//        PageIndicator {
//            id: indicator

//            count: view.count
//            currentIndex: view.currentIndex

//            anchors.bottom: view.bottom
//            anchors.horizontalCenter: parent.horizontalCenter
//        }

    }

}
