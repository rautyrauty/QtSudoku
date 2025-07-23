import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.qmlmodels 1.0

import SudokuModel 1.0

Column {
	anchors.fill: parent
	height: {
		if (parent.height/parent.width > 1.5) {
			return  parent.width
		}
		return parent.height
	}
	width: {
		if (parent.height/parent.width < 0.7) {
			return height
		}
		return parent.width
	}
	Sudoku {
		anchors.horizontalCenter: parent.horizontalCenter
		height: parent.height * 4 / 5
		width: height
		id: sudoku
	}
	
	Rectangle {
		id: menu
		height: parent.height - sudoku.height - parent.spacing
		anchors.left: sudoku.left
		anchors.right: sudoku.right
		color: "#373938"
		radius: height / 100 * 5
		
		RowLayout {
			anchors.fill: parent
			anchors.margins: 10
			spacing: 10
			
			MenuBtn {
				id: checkButton
				text: "Check"
				
				onClicked: {
					var errorPoint = sdk.help();
					if (errorPoint.x === -1 && errorPoint.y === -1) {
						root.fireConfetti()
						root.fireConfetti()
						root.fireConfetti()
					} else {
						// TODO: lose
					}
				}
			}
			
			ColumnLayout {
				Layout.fillWidth: true
				Layout.fillHeight: true
				Layout.preferredWidth: parent.width * 0.5
				spacing: 10

				MenuBtn {
					text: "Help"
					
					onClicked: {
						var errorPoint = sdk.help();
						if (errorPoint.x === -1 && errorPoint.y === -1) {
							root.fireConfetti()
							root.fireConfetti()
							root.fireConfetti()
						} else {
							sudoku.setErrorCell(errorPoint.x, errorPoint.y)
						}
					}
				}
				
				MenuBtn {
					text: "Return to Menu"
					
					onClicked: {
						stackView.pop(StackView.PopTransition)
					}
				}
			}
		}
	}
}
