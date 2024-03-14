import QtQuick
import QtQuick.Controls

Window {
    visible: true
    title: qsTr("PI Organ")
    color: "black"
    visibility: Window.FullScreen
    
    property real defaultWidth: 800
    property real defaultHeight: 480

    MouseArea {
        anchors.fill: parent
        enabled: false
        cursorShape: Qt.BlankCursor
    }

    SwipeView{

        id: view
        anchors.fill: parent
        currentIndex: 0

        // Scale Window to fullscreen
        transform: Scale { 
            xScale: screenWidth / defaultWidth
            yScale: screenHeight / defaultHeight
        }

        Item{

            id: item1
            Component {
                id: delegateComponent
                Label {
                    text: modelData
                    color: ( Math.abs(Tumbler.displacement)<0.5)?"#7Fff7F":"#ffffFF"
                    font.weight: Font.Bold
                    opacity: 1.0 - Math.abs(Tumbler.displacement) / (Tumbler.tumbler.visibleItemCount / 2)
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 14

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        onClicked: {
                            Tumbler.tumbler.currentIndex = index;
                        }
                    }

                }
            }

            Column {

                width: 225
                height: 400

                Label {
                    width: parent.width
                    height: 20
                    color: "#ffffff"
                    wrapMode: Label.Wrap
                    font.weight: Font.Bold
                    font.pointSize: 14
                    horizontalAlignment: Qt.AlignHCenter
                    text: qsTr("Basspedal:")
                }

                Tumbler {
                    objectName: "tmbInstrument0"
                    width: parent.width
                    height: 300
                    visibleItemCount: 7
                    font.bold: false
                    wheelEnabled: false
                    delegate: delegateComponent
                    model: ["Acoustic Grand Piano", "Bright Acoustic Piano", "Electric Grand Piano", "Honky-tonk Piano", "Electric Piano 1", "Electric Piano 2", "Harpsichord", "Clavi", "Celesta", "Glockenspiel", "Music Box", "Vibraphone", "Marimba", "Xylophone", "Tubular Bells", "Dulcimer", "Drawbar Organ", "Percussive Organ", "Rock Organ", "Church Organ", "Reed Organ", "Accordion", "Harmonica", "Tango Accordion", "Acoustic Guitar (nylon)", "Acoustic Guitar (steel)", "Electric Guitar (jazz)", "Electric Guitar (clean)", "Electric Guitar (muted)", "Overdriven Guitar", "Distortion Guitar", "Guitar harmonics", "Acoustic Bass", "Electric Bass (finger)", "Electric Bass (pick)", "Fretless Bass", "Slap Bass 1", "Slap Bass 2", "Synth Bass 1", "Synth Bass 2", "Violin", "Viola", "Cello", "Contrabass", "Tremolo Strings", "Pizzicato Strings", "Orchestral Harp", "Timpani", "String Ensemble 1", "String Ensemble 2", "SynthStrings 1", "SynthStrings 2", "Choir Aahs", "Voice Oohs", "Synth Voice", "Orchestra Hit", "Trumpet", "Trombone", "Tuba", "Muted Trumpet", "French Horn", "Brass Section", "SynthBrass 1", "SynthBrass 2", "Soprano Sax", "Alto Sax", "Tenor Sax", "Baritone Sax", "Oboe", "English Horn", "Bassoon", "Clarinet", "Piccolo", "Flute", "Recorder", "Pan Flute", "Blown Bottle", "Shakuhachi", "Whistle", "Ocarina", "Lead 1 (square)", "Lead 2 (sawtooth)", "Lead 3 (calliope)", "Lead 4 (chiff)", "Lead 5 (charang)", "Lead 6 (voice)", "Lead 7 (fifths)", "Lead 8 (bass + lead)", "Pad 1 (new age)", "Pad 2 (warm)", "Pad 3 (polysynth)", "Pad 4 (choir)", "Pad 5 (bowed)", "Pad 6 (metallic)", "Pad 7 (halo)", "Pad 8 (sweep)", "FX 1 (rain)", "FX 2 (soundtrack)", "FX 3 (crystal)", "FX 4 (atmosphere)", "FX 5 (brightness)", "FX 6 (goblins)", "FX 7 (echoes)", "FX 8 (sci-fi)", "Sitar", "Banjo", "Shamisen", "Koto", "Kalimba", "Bag pipe", "Fiddle", "Shanai", "Tinkle Bell", "Agogo", "Steel Drums", "Woodblock", "Taiko Drum", "Melodic Tom", "Synth Drum", "Reverse Cymbal", "Guitar Fret Noise", "Breath Noise", "Seashore", "Bird Tweet", "Telephone Ring", "Helicopter", "Applause", "Gunshot"]
                    wrap: false
                    anchors.horizontalCenter: parent.horizontalCenter

                    onCurrentIndexChanged: {
                        _myClass.currentIndexChanged(0, currentIndex)
                    }
                }
            }

            Column {
                x: 265
                y: 0

                width: 225
                height: 400

                Label {
                    width: parent.width
                    height: 20
                    color: "#ffffff"
                    wrapMode: Label.Wrap
                    font.pointSize: 14
                    font.weight: Font.Bold
                    horizontalAlignment: Qt.AlignHCenter
                    text: qsTr("Manual I:")
                }

                Tumbler {
                    objectName: "tmbInstrument1"
                    width: parent.width
                    height: 300
                    visibleItemCount: 7
                    delegate: delegateComponent
                    model: ["Acoustic Grand Piano", "Bright Acoustic Piano", "Electric Grand Piano", "Honky-tonk Piano", "Electric Piano 1", "Electric Piano 2", "Harpsichord", "Clavi", "Celesta", "Glockenspiel", "Music Box", "Vibraphone", "Marimba", "Xylophone", "Tubular Bells", "Dulcimer", "Drawbar Organ", "Percussive Organ", "Rock Organ", "Church Organ", "Reed Organ", "Accordion", "Harmonica", "Tango Accordion", "Acoustic Guitar (nylon)", "Acoustic Guitar (steel)", "Electric Guitar (jazz)", "Electric Guitar (clean)", "Electric Guitar (muted)", "Overdriven Guitar", "Distortion Guitar", "Guitar harmonics", "Acoustic Bass", "Electric Bass (finger)", "Electric Bass (pick)", "Fretless Bass", "Slap Bass 1", "Slap Bass 2", "Synth Bass 1", "Synth Bass 2", "Violin", "Viola", "Cello", "Contrabass", "Tremolo Strings", "Pizzicato Strings", "Orchestral Harp", "Timpani", "String Ensemble 1", "String Ensemble 2", "SynthStrings 1", "SynthStrings 2", "Choir Aahs", "Voice Oohs", "Synth Voice", "Orchestra Hit", "Trumpet", "Trombone", "Tuba", "Muted Trumpet", "French Horn", "Brass Section", "SynthBrass 1", "SynthBrass 2", "Soprano Sax", "Alto Sax", "Tenor Sax", "Baritone Sax", "Oboe", "English Horn", "Bassoon", "Clarinet", "Piccolo", "Flute", "Recorder", "Pan Flute", "Blown Bottle", "Shakuhachi", "Whistle", "Ocarina", "Lead 1 (square)", "Lead 2 (sawtooth)", "Lead 3 (calliope)", "Lead 4 (chiff)", "Lead 5 (charang)", "Lead 6 (voice)", "Lead 7 (fifths)", "Lead 8 (bass + lead)", "Pad 1 (new age)", "Pad 2 (warm)", "Pad 3 (polysynth)", "Pad 4 (choir)", "Pad 5 (bowed)", "Pad 6 (metallic)", "Pad 7 (halo)", "Pad 8 (sweep)", "FX 1 (rain)", "FX 2 (soundtrack)", "FX 3 (crystal)", "FX 4 (atmosphere)", "FX 5 (brightness)", "FX 6 (goblins)", "FX 7 (echoes)", "FX 8 (sci-fi)", "Sitar", "Banjo", "Shamisen", "Koto", "Kalimba", "Bag pipe", "Fiddle", "Shanai", "Tinkle Bell", "Agogo", "Steel Drums", "Woodblock", "Taiko Drum", "Melodic Tom", "Synth Drum", "Reverse Cymbal", "Guitar Fret Noise", "Breath Noise", "Seashore", "Bird Tweet", "Telephone Ring", "Helicopter", "Applause", "Gunshot"]
                    wrap: false
                    anchors.horizontalCenter: parent.horizontalCenter

                    onCurrentIndexChanged: {
                        //I want get the part of the model which belong to currentItem
                        _myClass.currentIndexChanged(1, currentIndex)
                    }
                }
            }
            Column {
                x: 530
                y: 0
                width: 225
                height: 400

                Label {
                    width: parent.width
                    height: 20
                    color: "#ffffff"
                    wrapMode: Label.Wrap
                    font.pointSize: 14
                    font.weight: Font.Bold
                    horizontalAlignment: Qt.AlignHCenter
                    text: qsTr("Manual II:")
                }

                Tumbler {
                    objectName: "tmbInstrument2"
                    width: parent.width
                    height: 300
                    visibleItemCount: 7
                    delegate: delegateComponent
                    model: ["Acoustic Grand Piano", "Bright Acoustic Piano", "Electric Grand Piano", "Honky-tonk Piano", "Electric Piano 1", "Electric Piano 2", "Harpsichord", "Clavi", "Celesta", "Glockenspiel", "Music Box", "Vibraphone", "Marimba", "Xylophone", "Tubular Bells", "Dulcimer", "Drawbar Organ", "Percussive Organ", "Rock Organ", "Church Organ", "Reed Organ", "Accordion", "Harmonica", "Tango Accordion", "Acoustic Guitar (nylon)", "Acoustic Guitar (steel)", "Electric Guitar (jazz)", "Electric Guitar (clean)", "Electric Guitar (muted)", "Overdriven Guitar", "Distortion Guitar", "Guitar harmonics", "Acoustic Bass", "Electric Bass (finger)", "Electric Bass (pick)", "Fretless Bass", "Slap Bass 1", "Slap Bass 2", "Synth Bass 1", "Synth Bass 2", "Violin", "Viola", "Cello", "Contrabass", "Tremolo Strings", "Pizzicato Strings", "Orchestral Harp", "Timpani", "String Ensemble 1", "String Ensemble 2", "SynthStrings 1", "SynthStrings 2", "Choir Aahs", "Voice Oohs", "Synth Voice", "Orchestra Hit", "Trumpet", "Trombone", "Tuba", "Muted Trumpet", "French Horn", "Brass Section", "SynthBrass 1", "SynthBrass 2", "Soprano Sax", "Alto Sax", "Tenor Sax", "Baritone Sax", "Oboe", "English Horn", "Bassoon", "Clarinet", "Piccolo", "Flute", "Recorder", "Pan Flute", "Blown Bottle", "Shakuhachi", "Whistle", "Ocarina", "Lead 1 (square)", "Lead 2 (sawtooth)", "Lead 3 (calliope)", "Lead 4 (chiff)", "Lead 5 (charang)", "Lead 6 (voice)", "Lead 7 (fifths)", "Lead 8 (bass + lead)", "Pad 1 (new age)", "Pad 2 (warm)", "Pad 3 (polysynth)", "Pad 4 (choir)", "Pad 5 (bowed)", "Pad 6 (metallic)", "Pad 7 (halo)", "Pad 8 (sweep)", "FX 1 (rain)", "FX 2 (soundtrack)", "FX 3 (crystal)", "FX 4 (atmosphere)", "FX 5 (brightness)", "FX 6 (goblins)", "FX 7 (echoes)", "FX 8 (sci-fi)", "Sitar", "Banjo", "Shamisen", "Koto", "Kalimba", "Bag pipe", "Fiddle", "Shanai", "Tinkle Bell", "Agogo", "Steel Drums", "Woodblock", "Taiko Drum", "Melodic Tom", "Synth Drum", "Reverse Cymbal", "Guitar Fret Noise", "Breath Noise", "Seashore", "Bird Tweet", "Telephone Ring", "Helicopter", "Applause", "Gunshot"]
                    wrap: false
                    anchors.horizontalCenter: parent.horizontalCenter
                    // onCurrentItemChanged: _myClass.currentItemChanged(0,"-")
                    onCurrentIndexChanged: {
                        //I want get the part of the model which belong to currentItem
                        console.log("Index Changed:" + currentIndex);
                        _myClass.currentIndexChanged(2, currentIndex)
                    }
                }
            }

            PageIndicator {
                id: pageIndicator
            }

            Slider {
                id: slider
                orientation: "Vertical"
                objectName: "sldVolumeInstrument0"
                x: 225
                y: 44
                width: 40
                height: 267
                value: 0.5
                onValueChanged: _myClass.sliderValueChanged(value, 0)
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

            Slider {
                id: slider1
                objectName: "sldVolumeInstrument1"
                x: 490
                y: 44
                width: 40
                height: 267
                value: 0.5
                orientation: "Vertical"
                onValueChanged: _myClass.sliderValueChanged(value, 1)
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

            Slider {
                id: slider2
                objectName: "sldVolumeInstrument2"
                x: 745
                y: 44
                width: 40
                height: 267
                value: 0.5
                orientation: "Vertical"
                onValueChanged: _myClass.sliderValueChanged(value, 2)
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

            Button {
                id: button
                x: 23
                y: 368
                height: 44
                width: 44
                text: qsTr("-")
                font.pointSize: 14
                font.weight: Font.Bold
                onClicked: _myClass.buttonClicked(0, "-")
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

            Button {
                id: button1
                x: 152
                y: 368
                width: 44
                height: 44
                text: qsTr("+")
                font.pointSize: 14
                font.weight: Font.Bold
                onClicked: _myClass.buttonClicked(0, "+")
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

            Button {
                id: button2
                x: 196
                y: 368
                width: 44
                height: 44
                text: qsTr("B")
                onClicked: _myClass.buttonClicked(0, "B")
                font.pointSize: 14
                font.weight: Font.Bold
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

            TextField {
                id: textField
                objectName: "txtOffsetInstrument0"
                x: 66
                y: 368
                width: 88
                height: 44
                horizontalAlignment: "AlignHCenter"
                font.pointSize: 14
                text: qsTr("0")
                font.weight: Font.Bold
                color: "white"
                background: Rectangle {
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                    color: "#303030"
                    border.color: "gray"
                }
            }

            Button {
                id: button3
                x: 290
                y: 368
                width: 44
                height: 44
                text: qsTr("-")
                font.pointSize: 14
                font.weight: Font.Bold
                onClicked: _myClass.buttonClicked(1, "-")
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

            Button {
                id: button4
                x: 419
                y: 368
                width: 44
                height: 44
                text: qsTr("+")
                onClicked: _myClass.buttonClicked(1, "+")
                font.pointSize: 14
                font.weight: Font.Bold
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

            Button {
                id: button5
                x: 463
                y: 368
                width: 44
                height: 44
                text: qsTr("B")
                onClicked: _myClass.buttonClicked(1, "B")
                font.pointSize: 14
                font.weight: Font.Bold
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

            TextField {
                id: textField1
                objectName: "txtOffsetInstrument1"
                x: 333
                y: 368
                width: 88
                height: 44
                horizontalAlignment: "AlignHCenter"
                font.pointSize: 14
                text: qsTr("0")
                font.weight: Font.Bold
                color: "white"
                background: Rectangle {
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                    color: "#303030"
                    border.color: "gray"
                }
                
            }

            Palette {
                id: pltBtnRegular
                objectName: "pltBtnRegular"
                button: "#303030"
                buttonText: "white"
            }

            Palette {
                id: pltBtnHighlight
                objectName: "pltBtnHighlight"
                button: "#7Fff7F"
                buttonText: "#101010"
            }

            Button {
                id: button6
                x: 556
                y: 368
                width: 44
                height: 44
                text: qsTr("-")
                onClicked: _myClass.buttonClicked(2, "-")
                font.pointSize: 14
                font.weight: Font.Bold
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

            Button {
                id: button7
                x: 685
                y: 368
                width: 44
                height: 44
                text: qsTr("+")
                onClicked: _myClass.buttonClicked(2, "+")
                font.pointSize: 14
                font.weight: Font.Bold
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

            Button {
                id: button8
                x: 729
                y: 368
                width: 44
                height: 44
                text: qsTr("B")
                onClicked: _myClass.buttonClicked(2, "B")
                font.pointSize: 14
                font.weight: Font.Bold
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

            TextField {
                id: textField2
                objectName: "txtOffsetInstrument2"
                x: 599
                y: 368
                width: 88
                height: 44
                horizontalAlignment: "AlignHCenter"
                font.pointSize: 14
                font.weight: Font.Bold
                text: qsTr("0")
                color: "white"
                background: Rectangle {
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                    color: "#303030"
                    border.color: "gray"
                }
            }

            Button {
                id: btnSetting0
                objectName: "btnSetting0"
                x: 23
                y: 418
                width: 88
                height: 44
                text: qsTr("1")
                onClicked: _myClass.buttonClicked(0, "P1")
                font.pointSize: 14
                font.weight: Font.Bold
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }
            Button {
                id: btnSetting1
                objectName: "btnSetting1"
                x: 115
                y: 418
                width: 88
                height: 44
                text: qsTr("2")
                onClicked: _myClass.buttonClicked(0, "P2")
                font.pointSize: 14
                font.weight: Font.Bold
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }
            Button {
                id: btnSetting2
                objectName: "btnSetting2"
                x: 207
                y: 418
                width: 88
                height: 44
                text: qsTr("3")
                onClicked: _myClass.buttonClicked(0, "P3")
                font.pointSize: 14
                font.weight: Font.Bold
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }
            Button {
                id: btnSetting3
                objectName: "btnSetting3"
                x: 299
                y: 418
                width: 88
                height: 44
                text: qsTr("4")
                onClicked: _myClass.buttonClicked(0, "P4")
                font.pointSize: 14
                font.weight: Font.Bold
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }
            Button {
                id: btnSetting4
                objectName: "btnSetting4"
                x: 391
                y: 418
                width: 88
                height: 44
                text: qsTr("5")
                onClicked: _myClass.buttonClicked(0, "P5")
                font.pointSize: 14
                font.weight: Font.Bold
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }
            Button {
                id: btnSetting5
                objectName: "btnSetting5"
                x: 483
                y: 418
                width: 88
                height: 44
                text: qsTr("6")
                onClicked: _myClass.buttonClicked(0, "P6")
                font.pointSize: 14
                font.weight: Font.Bold
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }
            Button {
                id: btnSetting6
                objectName: "btnSetting6"
                x: 577
                y: 418
                width: 88
                height: 44
                text: qsTr("7")
                onClicked: _myClass.buttonClicked(0, "P7")
                font.pointSize: 14
                font.weight: Font.Bold
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
            MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }
            Button {
                id: btnSaveSetting
                objectName: "btnSaveSetting"
                x: 684
                y: 418
                width: 88
                height: 44
                text: qsTr("S")
                onClicked: _myClass.buttonClicked(0, "S")
                font.pointSize: 14
                font.weight: Font.Bold
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }
            }
        }

   Item{
        id: item2

 Label {
                    x: 160
                    y: 0
                    width: 100
                    height: 20
                    color: "#ffffff"
                    wrapMode: Label.Wrap
                    font.pointSize: 14
                    font.weight: Font.Bold
                    horizontalAlignment: Qt.AlignHCenter
                    text: qsTr("Hall")
                }

 Label {
                    x: 565
                    y: 0
                    width: 100
                    height: 20
                    color: "#ffffff"
                    wrapMode: Label.Wrap
                    font.pointSize: 14
                    font.weight: Font.Bold
                    horizontalAlignment: Qt.AlignHCenter
                    text: qsTr("Chorus")
                }


  Slider {
                id: sldReverbLevel
                orientation: "Vertical"
                objectName: "sldReverbLevel"
                x: 50
                y: 44
                width: 40
                height: 267
                value: 0.5
                onValueChanged: _myClass.sliderValueChanged(value, 3)
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

  Slider {
                id: sldReverbRoomsize
                orientation: "Vertical"
                objectName: "sldReverbRoomsize"
                x: 150
                y: 44
                width: 40
                height: 267
                value: 0.5
                onValueChanged: _myClass.sliderValueChanged(value, 4)
                MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                }
            }

  Slider {
                id: sldReverbDamp
                orientation: "Vertical"
                objectName: "sldReverbDamp"
                x: 250
                y: 44
                width: 40
                height: 267
                value: 0.5
                onValueChanged: _myClass.sliderValueChanged(value, 5)
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

  Slider {
                id: sldReverbWidth
                orientation: "Vertical"
                objectName: "sldReverbWidth"
                x: 350
                y: 44
                width: 40
                height: 267
                value: 0.5
                onValueChanged: _myClass.sliderValueChanged(value, 6)
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

  Slider {
                id: sldChorusLevel
                orientation: "Vertical"
                objectName: "sldChorusLevel"
                x: 500
                y: 44
                width: 40
                height: 267
                value: 0.5
                onValueChanged: _myClass.sliderValueChanged(value, 7)
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

  Slider {
                id: sldChorusDepth
                orientation: "Vertical"
                objectName: "sldChorusDepth"
                x: 600
                y: 44
                width: 40
                height: 267
                value: 0.5
                onValueChanged: _myClass.sliderValueChanged(value, 8)
                MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                }
            }

  Slider {
                id: sldChorusSpeed
                orientation: "Vertical"
                objectName: "sldChorusSpeed"
                x: 700
                y: 44
                width: 40
                height: 267
                value: 0.5
                onValueChanged: _myClass.sliderValueChanged(value, 9)
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

     Label {
            x: 40; y: 350;
            color: "#ffffff"; font.pointSize: 14
            text: qsTr("Stärke")
            }
     Label {
            x: 150; y: 350;
            color: "#ffffff"; font.pointSize: 14
            text: qsTr("Raum")
            }
     Label {
            x: 225; y: 350;
            color: "#ffffff"; font.pointSize: 14
            text: qsTr("Dämpfung")
            }
     Label {
            x: 350; y: 350;
            color: "#ffffff"; font.pointSize: 14
            text: qsTr("Weite")
            }

     Label {
            x: 495; y: 350;
            color: "#ffffff"; font.pointSize: 14
            text: qsTr("Stärke")
            }
     Label {
            x: 600; y: 350;
            color: "#ffffff"; font.pointSize: 14
            text: qsTr("Tiefe")
            }
     Label {
            x: 690; y: 350;
            color: "#ffffff"; font.pointSize: 14
            text: qsTr("Frequnz")
            }



   }

   Item{

        id: item3
        
        Rectangle {

            anchors.fill: parent

            Rectangle {
                x: 0
                y: 0
                width: 400
                height: parent.height
                color: "black"


                Component {
                    id: nameDelegate
                    Text {
                        readonly property ListView __lv: ListView.view
                        width: parent.width
                        text: modelData;
                        font.pixelSize: 20
                        color: ListView.isCurrentItem ? "#7Fff7F" : "gray"

                        MouseArea {
                            anchors.fill: parent
                            onClicked: __lv.currentIndex = model.index
                        }
                    }
                }
            
                ListView {
                    objectName: "lvSoundFonts"
                    anchors.fill: parent
                    delegate: nameDelegate
                    focus: true
                    clip: true
                    /*
                    header: Rectangle {
                        anchors { left: parent.left; right: parent.right }
                        height: 10
                        color: "pink"
                    }
                    footer: Rectangle {
                        anchors { left: parent.left; right: parent.right }
                        height: 10
                        color: "lightblue"
                    }
                    highlight: Rectangle {
                        anchors { left: parent.left; right: parent.right }
                        color: "#101010"
                    }
                    */

                    onCurrentIndexChanged: {
                        //I want get the part of the model which belong to currentItem
                        if (currentIndex !== -1) {
                            _myClass.soundFontChanged(currentItem.text);
                        }
                        txtStatus.text = currentItem.text
                      
                    }

                }
            //<-- slide
            }
 Rectangle {
                x: 400
                y:0
                width: 400
                height: parent.height
                color: "black"

                Label {
                    x: 20
                    y: 20
                    id: txtStatus
                    width: 100
                    height: 40
                    font.pointSize: 14
                    font.weight: Font.Bold
                    color: "#ffffff"
                    text: qsTr("Soundfont geladen.")
                }


        BusyIndicator {
            id: busyIndicator1
                x: 10
                y: 10
                width: 40
                height: 40
                palette {
                    dark: "red"
                }
            visible: true
            running: false
        }


 Button {
                id: btnLoadSF
                x: 152
                y: 368
                width: 100
                height: 44
                text: qsTr("SF Laden")
                font.pointSize: 14
                font.weight: Font.Bold
                onClicked:{
                    txtStatus.text = qsTr("Lade Soundfont...")
                   Qt.callLater(function() { 
                       _myClass.loadSoundFont();
                        txtStatus.text = qsTr("Soundfont geladen!")
                    });
                } 
                palette {
                    button: "#303030"
                    buttonText: "white"
                }
                        MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.BlankCursor
                        enabled: false
                    }

            }

            }
        }


        }
    }

}
