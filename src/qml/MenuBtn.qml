import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.qmlmodels 1.0

Button {
	id: checkButton
	activeFocusOnTab: true
	Layout.fillWidth: true
	Layout.fillHeight: true
	Layout.preferredWidth: parent.width * 0.5
	font.family: russoFontLoader.name
	font.pixelSize: Math.min(parent.height * 0.3, parent.width * 0.08)

	focusPolicy: Qt.StrongFocus

	property bool pressedByKeyboard: false

	Keys.onPressed: {
		if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
			pressedByKeyboard = true
			clicked()
			event.accepted = true
		}
	}

	Keys.onReleased: {
		if ((event.key === Qt.Key_Return || event.key === Qt.Key_Enter) && pressedByKeyboard) {
			pressedByKeyboard = false
			event.accepted = true
		}
	}
	
	background: Rectangle {
		color: {
			if (parent.down || parent.pressedByKeyboard) {
				return "#5a524e"
			}
			return "#706762"
		}
		radius: height / 4
		border.width: parent.activeFocus ? 2 : 0
		border.color: "#ffffff"
	}
}
