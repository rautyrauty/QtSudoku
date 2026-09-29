import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.qmlmodels 1.0

import SudokuModel 1.0

ApplicationWindow {
	width: 800
	height: 800
	visible: true
	title: qsTr("QtSudoku")
	id: root

	palette {
		window: "#2a2e32"
		windowText: "#ffffff"
		base: "#373938"
		text: "#ffffff"
		button: "#706762"
		buttonText: "#ffffff"
		placeholderText: "#a09a96"
		highlight: "#6bbbb8"
		highlightedText: "#2a2e32"
	}

	FontLoader {
		id: russoFontLoader
		source: "qrc:/fonts/RussoOne-Regular.ttf"
		onStatusChanged: {
			if (status == FontLoader.Error) {
				console.error("Font loading error:", source);
			}
		}
	}

	StackView {
		id: stackView
		initialItem: startScreen
		anchors.fill: parent
	}

	SudokuModel {
		id: sdk
	}

	function fireConfetti() {
	  const count = 200;

	  const params = {
		particles: 100,
		spread: 70,
		origin: {
			  x: Math.random(),
			  // since particles fall down, skew start toward the top
			  y: Math.random() - 0.2
		},
	  };
	  confetti.fire(params);
	}

	ConfettiCanvas {
		id: confetti;
		anchors.fill: parent
	}

	Component {
		id: startScreen
		Item {
			anchors.fill: parent


			Item {
				id: contentContainer
				anchors.centerIn: parent
				width: Math.min(parent.width, parent.height * 1.5) * 0.8
				height: childrenRect.height

				property real minScale: 0.7
				property real targetScale: 1.0

				function updateScale() {
					var maxHeight = parent.height * 0.9;
					if (height > maxHeight) {
						targetScale = maxHeight / height;
						if (targetScale < minScale) targetScale = minScale;
					} else {
						targetScale = 1.0;
					}
				}

				onHeightChanged: updateScale()
				Component.onCompleted: updateScale()

				transformOrigin: Item.Center
				scale: targetScale

				Column {
					width: parent.width
					spacing: 20

					TextField {
						id: lockedCellsInput
						placeholderText: "0-81"
						validator: IntValidator {
							bottom: 0
							top: 81
						}
						text: "40"
						maximumLength: 2
						inputMethodHints: Qt.ImhDigitsOnly
						horizontalAlignment: TextInput.AlignHCenter
						anchors.horizontalCenter: parent.horizontalCenter

						activeFocusOnTab: true
						font.family: russoFontLoader.name
						font.pixelSize: Math.min(40, parent.width * 0.1)
						height: Math.max(40, font.pixelSize * 1.8)

						width: font.pixelSize * 2.2

						background: Rectangle {
							color: "#373938"
							radius: height / 4
							border.width: parent.activeFocus ? 2 : 0
							border.color: "#ffffff"
						}
					}

					MenuBtn {
						anchors.horizontalCenter: parent.horizontalCenter
						text: "Play!"
						width: parent.width * 0.6

						onClicked: {
							sdk.generate(lockedCellsInput.text)
							stackView.push(sudokuScreen, StackView.PushTransition)
						}
					}

					Column {
						width: parent.width
						spacing: 10
						anchors.horizontalCenter: parent.horizontalCenter

						MenuBtn {
							anchors.horizontalCenter: parent.horizontalCenter
							text: "Options"
							width: parent.width * 0.6

							onClicked: {
								console.log("Options button clicked")
							}
						}
						MenuBtn {
							anchors.horizontalCenter: parent.horizontalCenter
							text: "exit :<"
							width: parent.width * 0.6

							onClicked: {
								Qt.quit()
							}
						}
					}
				}
			}
		}
	}
	Component {
		id: sudokuScreen
		SudokuPage {
			scale: 0.95
			spacing: 10
		}
	}
}
