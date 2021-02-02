import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

GridLayout {
    id: channel_view
    columns: 2
    signal sig_connect()
    signal sig_start_measure()
    signal sig_stop_measure()
    signal sig_set_range(int range)
    signal sig_get_status()
    signal sig_get_result()

    Button {
        Layout.row: 0
        Layout.column: 0
        text: "Start measure"
        onClicked: channel_view.sig_start_measure();
    }

    Button {
        Layout.row: 0
        Layout.column: 1
        text: "Stop measure"
        onClicked: channel_view.sig_stop_measure();
    }

    Button {
        Layout.row: 1
        Layout.column: 0
        text: "Set range"
        onClicked: channel_view.sig_set_range(combo_range.currentIndex);
    }

    ComboBox {
        Layout.row: 1
        Layout.column: 1
        id: combo_range
        objectName: "combo_range"
        model: ["range0", "range1", "range2", "range3"]
    }

    Button {
        Layout.row: 2
        Layout.column: 0
        text: "Get status"
        onClicked: channel_view.sig_get_status();
    }

    Text {
        Layout.row: 2
        Layout.column: 1
        objectName: "text_status"
        text: "unknown"
    }

    Button {
        Layout.row: 3
        Layout.column: 0
        text: "Get result"
        onClicked: channel_view.sig_get_result();
    }

    Text {
        Layout.row: 3
        Layout.column: 1
        objectName: "text_result"
        text: "unknown"
    }

    Button {
        Layout.row: 4
        Layout.columnSpan: 2
        objectName: "connect_button"
        Layout.fillWidth: true
        text: "Connect"
        onClicked: channel_view.sig_connect();
    }

    Text {
        Layout.row: 5
        Layout.columnSpan: 2
        objectName: "text_msg"
        Layout.fillWidth: true
        text: "this is channel view"
    }
}


