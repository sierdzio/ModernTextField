import QtQuick 2.15
import QtQuick.Layouts 1.15

import ModernFields 1.0

ColumnLayout {
    Text {
        text: "Input"
    }

    ModernTextField {
        Layout.fillHeight: true
        Layout.fillWidth: true
    }

    Text {
        text: "Preview"
    }

    ModernTextField {
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
}
