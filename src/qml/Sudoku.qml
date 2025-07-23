import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.qmlmodels 1.0

GridView {
	id: sudoku
	cellHeight: height/3
	cellWidth: width/3
	interactive: false

	model: 9

	function getCell(x,y) {
		if (x<0 || y<0 || x>8 || y>8) {
			return null
		}
		let tmp_square = itemAtIndex(Math.floor(y / 3) * 3 + Math.floor(x / 3)).getGridView()
		let tmp_cell = tmp_square.itemAtIndex(y % 3 * 3 + x % 3)
		return tmp_cell
	}

	property point errorCell: Qt.point(-1, -1)
	Timer {
		id: errorTimer
		interval: 5000
		onTriggered: {
			getCell(errorCell.x,errorCell.y).restoreColor()
			errorCell = Qt.point(-1, -1);
		}
	}
	function setErrorCell(x,y) {
		if (x<0 || y<0 || x>8 || y>8) {
			return null
		}
		errorCell = Qt.point(x, y)
		errorTimer.restart()
	}

	delegate: Rectangle {
		width: GridView.view.cellWidth
		height: GridView.view.cellHeight
		scale: 0.95
		visible: true
		radius: width / 100 * 5
		color: {
			if ( index % 2 == 1) return "#464646"
			return "#373938"
		}

		function getGridView() {
			return square
		}

		GridView {
			id: square
			anchors.fill: parent
			model: sdk.blockModel(index)
			interactive: false
			cellHeight: height/3
			cellWidth: width/3
			delegate: SudokuCell {
				width: GridView.view.cellWidth
				height: GridView.view.cellHeight
			}
		}
	}
}
