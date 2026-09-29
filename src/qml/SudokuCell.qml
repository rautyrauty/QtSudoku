import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.qmlmodels 1.0

FocusScope {
	id: cell
	readonly property int x_pos: model.blockIndex % 3 * 3 + index % 3;
	readonly property int y_pos: Math.floor(model.blockIndex / 3) * 3 + Math.floor(index / 3);
	property bool is_locked: false
	scale: 0.85
	activeFocusOnTab: true

	function setDigit(digit) {
		sdk.setDigit(x_pos,y_pos,digit)
	}

	function restoreColor() {
		if (model.isLocked) {
			cell_background.color = "#7d411f"
		} else {
			cell_background.color = "#706762"
		}
	}

	Rectangle {
		id: cell_background
		anchors.fill: parent
		radius: width / 100 * 5
		border.color: "#6bbbb8"
		color: {
			if (model.isLocked) {
				return "#7d411f"
			}
			return "#706762"
		}
		border.width: {
			if (cell.activeFocus) {
				return width / 100 * 3
			}
			return 0
		}
		SequentialAnimation on color {
			id: blinkAnimation
			running: Qt.point(cell.x_pos, cell.y_pos) == sudoku.errorCell
			loops: Animation.Infinite
			ColorAnimation { to: "red"; duration: 300 }
			ColorAnimation { to: "#706762"; duration: 300 }
		}
	}

	Text {
		id: cell_text
		anchors.centerIn: parent
		text: {
			if (model.digit === 0){
				return ""
			}
			return model.digit
		}
		//text: "(" + cell.x_pos + "," + cell.y_pos + ")"
		font.family: russoFontLoader.name
		color: "white"
		font.pixelSize: parent.height
	}
	MouseArea {
		anchors.fill: parent
		onClicked: {
			cell.forceActiveFocus()
		}
	}

	Keys.onLeftPressed: {
		let focus_cell = sudoku.getCell(cell.x_pos - 1, cell.y_pos)
		if (focus_cell) {
			focus_cell.forceActiveFocus()
		}
	}
	Keys.onRightPressed: {
		console.error("start pos:" + x + "," + y)
		let focus_cell = sudoku.getCell(cell.x_pos + 1, cell.y_pos)
		if (focus_cell) {
			focus_cell.forceActiveFocus()
		}
	}
	Keys.onUpPressed: {
		let focus_cell = sudoku.getCell(cell.x_pos, cell.y_pos - 1)
		if (focus_cell) {
			focus_cell.forceActiveFocus()
		}
	}
	Keys.onDownPressed: {
		let focus_cell = sudoku.getCell(cell.x_pos, cell.y_pos + 1)
		if (focus_cell) {
			focus_cell.forceActiveFocus()
		}
	}
	Keys.onPressed: {
		// console.error(index + ", key pressed: " + event.key)
		// console.error(is_locked)

		if ((event.key === Qt.Key_M) && (event.modifiers & Qt.ControlModifier)) {
			checkButton.forceActiveFocus()
		}
		if (model.isLocked) {
			return
		}
		if (event.key >= 49 && event.key <= 57) { // 1-9
			cell.setDigit(event.text)
		}
		if (event.key === 48 || event.key === 16777223) { // 0,del
			cell.setDigit("")
		}
	}
}
