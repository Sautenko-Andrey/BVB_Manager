#include "utils.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QHeaderView>


const QString BACKGROUND_COLOR = "background-color:#09beb8;";

const QStringList training_types{
    "Warm up", "Gym", "Cardio", "Tactical-technical"
};


/*
    Function creates a players table
*/
void createPlayersTable(QStandardItemModel *model, QTableView *tableView,
                        QSqlDatabase *db, QVector<Player> &all_players,
                        QDialog *dialog)
{

    if(model && tableView && db && dialog){
        // adding header to a table
        model = new QStandardItemModel();

        // allow table header occupy full line
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        const QStringList labels{
            "ID", "First name", "Last name", "Age", "Gender", "Height", "Hometown", "Phone"
        };

        // add header labels for the table
        model->setHorizontalHeaderLabels(labels);
        tableView->setModel(model);

        // adding players data to the table
        QSqlQuery query(*db);

        if(!query.exec("SELECT id, first_name, last_name, age, sex, height, hometown, phone"
                        " FROM Players;")){
            QMessageBox::warning(dialog, "Database error",
                                 "Couldn't load players data. Try one more time.");
            return;
        }

        // filling the container
        while(query.next()){

            all_players.emplace_back(Player(query.value(0).toInt(), query.value(1).toString(),
                                            query.value(2).toString(),query.value(3).toInt(),
                                            query.value(4).toInt(),query.value(5).toInt(),
                                            query.value(6).toString(),query.value(7).toString()));
        }

        // filling the table

        // counter
        int i = 0;

        for(auto const &player : all_players){
            auto id = new QStandardItem(QString::number(player.id));
            auto name = new QStandardItem(player.first_name);
            auto surname = new QStandardItem(player.last_name);
            auto age = new QStandardItem(QString::number(player.age));
            auto gender = new QStandardItem( player.gender ? "male" : "female" );
            auto height = new QStandardItem(QString::number(player.height));
            auto hometown = new QStandardItem(player.hometown);
            auto phone = new QStandardItem(player.phone);

            model->setItem(i, 0, id);
            model->setItem(i, 1, name);
            model->setItem(i, 2, surname);
            model->setItem(i, 3, age);
            model->setItem(i, 4, gender);
            model->setItem(i, 5, height);
            model->setItem(i, 6, hometown);
            model->setItem(i, 7, phone);

            ++i;
        }
    }
}


/*
    Function automatically changes a font of a btn if it's neccessary
*/
void fontAdapter(QPushButton *btn){

    if(btn){
        if(constexpr int max_symbols_amount{24};
            btn->text().size() >= max_symbols_amount)
        {
            btn->setFont(QFont("Ubuntu", 6));
        }
        else if(btn->text().size() > max_symbols_amount / 2 + 1 &&
                 btn->text().size() < max_symbols_amount){
            btn->setFont(QFont("Ubuntu", 9));
        }
    }
}


/*
    Function changes stylesheet of table's items
*/
void changeTableItemStyle(QTableWidget *table, const QColor color,
                          bool is_bold){

    // make  aqua color and bold text of horizontal headers
    for(int i{0}; i < table->columnCount(); ++i){
        if(auto header_item = table->horizontalHeaderItem(i); header_item){
            // change color
            header_item->setForeground(color);
            // make text bold
            if(is_bold){
                auto font = header_item->font();
                font.setBold(true);
                header_item->setFont(font);
            }
        }
    }
}



// Styles----------------------------------------------------------------------------

const QString game_result_style = "QPushButton {"
                                  "background-color: #2e2e2e;"   // Dark grey color
                                  "color: white;"                // text for contrast
                                  "padding: 15px 5px;"      // Add padding to the button
                                  "border: 2px solid #1a1a1a;"   // Slightly darker grey border
                                  "border-radius: 8px;"          // Rounded corners
                                  "font-size: 16px;"             // Font size
                                  "font-weight: bold;"           // Bold text
                                  "text-align: center;"          // Center the text
                                  "}";

const QString game_result_style_2 = "QPushButton {"
                                    "background-color: #2e2e2e;"   // Dark grey color
                                    "color: DeepPink;"                // text for contrast
                                    "padding: 15px 5px;"          // Add padding to the button
                                    "border: 2px solid #1a1a1a;"   // Slightly darker grey border
                                    "border-radius: 8px;"          // Rounded corners
                                    "font-size: 16px;"             // Font size
                                    "font-weight: bold;"           // Bold text
                                    "text-align: center;"          // Center the text
                                    "}";

const QString game_result_spinbox_style = "QSpinBox {"
                                          "font-size: 20px;"    // Make the text bigger
                                          "font-weight: bold;"
                                          "min-width: 100px;"   // Minimum width of the spinbox
                                          "min-height: 40px;"   // Minimum height of the spinbox
                                          "padding: 5px;"       // Padding around the content
                                          "}";

const QString ops_btn_style = "QPushButton {"
                              "background-color: transparent;"            // Transparent background
                              "color: black;"                           // Text color
                              "font-weight: bold;"
                              /*"border: 1px solid black;"*/                // Border with custom color
                              "border: none;"
                              "border-radius: 5px;"                       // Rounded corners
                              "padding: 5px 3px;"                       // Padding
                              "font-size: 16px;"
                              "}"

                              "QPushButton:hover {"
                              "background-color: azure;"                // Background fills on hover
                              "color: DeepPink;"                             // Text turns white on hover
                              "}"

                              "QPushButton:pressed {"
                              "color: Aqua;"
                              "background-color: azure;"                // Darker background when pressed
                              "}";

const QString team_btn_style = "QPushButton {"
                               "background-color: DarkSlateGray;" // Normal background color
                               "color: Aqua;"
                               "font-weight: bold;"
                               "border-radius: 10px;"
                               "padding: 20px 5px;"
                               "}"
                               "QPushButton:hover {"
                               "background-color: black;" // Background color when hovered
                               "color: DeepPink;"
                               "}"
                               "QPushButton:pressed {"
                               "background-color: #1abc9c;"  // Background color when pressed
                               "}";

const QString team_btn_style_for_none = "QPushButton {"
                                        "background-color: DarkSlateGray;" // Normal background color
                                        "color: red;"
                                        "font-weight: bold;"
                                        "border-radius: 10px;"
                                        "padding: 20px 5px;"
                                        "}"
                                        "QPushButton:hover {"
                                        "background-color: black;" // Background color when hovered
                                        "color: DeepPink;"
                                        "}"
                                        "QPushButton:pressed {"
                                        "background-color: #1abc9c;"  // Background color when pressed
                                        "}";

const QString team_btn_pulsing_style = "QPushButton {"
                                       "background-color: DarkSlateGray;" // Normal background color
                                       "color: Aqua;"
                                       "font-weight: bold;"
                                       "border-radius: 10px;"
                                       // "padding: 20px 5px;"
                                       "padding: 30px 5px;"
                                       "}"
                                       "QPushButton:hover {"
                                       "background-color: black;" // Background color when hovered
                                       "color: DeepPink;"
                                       "}"
                                       "QPushButton:pressed {"
                                       "background-color: #1abc9c;"  // Background color when pressed
                                       "}";


const QString ok_button_style = "QPushButton {"
                                "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                                "stop:0 rgba(50, 50, 50, 255), stop:1 rgba(10, 10, 10, 255));"
                                "border: 2px solid #555;"
                                "border-radius: 10px;"
                                "color: Aqua;"
                                "padding: 5px 10px;"
                                "font-size: 16px;"
                                "}"
                                "QPushButton:hover {"
                                "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                                "stop:0 rgba(70, 70, 70, 255), stop:1 rgba(20, 20, 20, 255));"
                                "}"
                                "QPushButton:pressed {"
                                "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, "
                                "stop:0 rgba(30, 30, 30, 255), stop:1 rgba(5, 5, 5, 255));"
                                "}";

const QString team_label_style = "QLabel {"
                                 "font-size: 16px;"      // Large font size for modern look
                                 "font-weight: bold;"
                                 "color: Aqua;"         // Aqua text color
                                 "}"
                                 "QLabel:hover {"      // Optional: Add hover effect for modern interactivity
                                 "color: DeepPink;"  // Change text color on hover
                                 "}";

const QString calendar_style = R"(
        /* Change the overall background color */
        QCalendarWidget QWidget {
            background-color: #2e3440;
        }

        /* Change the background color of the navigation bar (month/year selectors) */
        QCalendarWidget QToolButton {
            color: #d8dee9;          /* Font color */
            background-color: #4c566a;
            border: none;
            border-radius: 5px;
            padding: 5px;
        }

        /* Hover effect for navigation buttons */
        QCalendarWidget QToolButton:hover {
            background-color: #5e81ac;
        }

        /* Style for weekday headers (Mon, Tue, etc.) */
        QCalendarWidget QTableView QHeaderView::section {
            background-color: #4c566a;
            color: #eceff4;
            padding: 5px;
        }

        /* Style for the dates (numbers in the calendar) */
        QCalendarWidget QTableView {
            color: #d8dee9;          /* Font color for dates */
            background-color: #2e3440;
            gridline-color: #434c5e;
        }

        /* Change the appearance of the current selected date */
        QCalendarWidget QTableView::item:selected {
            background-color: #88c0d0;
            color: #2e3440;
        }

        /* Change the background of the current day */
        QCalendarWidget QTableView::item:focus {
            border: 1px solid #5e81ac;
            background-color: #88c0d0;
            color: #2e3440;
        }

        /* Customize weekend days */
        QCalendarWidget QTableView::item:disabled {
            color: #bf616a;
        }

        /* Style the grid lines between dates */
        QCalendarWidget QTableView::item {
            border: 1px solid #3b4252;
        }
    )";

const QString combobox_style =
                              "QComboBox {"
                              "color: Aqua;"
                              "background-color: #2e3440;"
                              "}"

                              "QComboBox QAbstractItemView {"
                              "background-color: #2e3440;"
                              "}";

const QString checkbox_style = "QCheckBox { color: white; }";

const QString lcd_number_style = "QLCDNumber {"
                "background-color: #2e3440;"    // Background of the LCD number
                "border: 2px solid Aqua;"  // Green border around the display
                "border-radius: 8px;"        // Rounded corners
                "padding: 5px;"               // Padding inside the border
                "}";

const QString time_edit_style = "QTimeEdit {"
                                "    background-color: #2e3440;"    // Default background color
                                "    color: white;"               // Default text color
                                "    border: 2px solid Aqua;"     // Default border
                                "    border-radius: 4px;"         // Rounded corners
                                "    padding: 4px;"
                                "}"
                                "QTimeEdit:hover {"
                                "    background-color: DarkSlateGray;"  // Background color on hover
                                "    color: DeepPink;"
                                "    border: 3px solid DeepPink;"     // Border color on hover
                                "}"
                                "QTimeEdit:focus {"
                                "    background-color: DarkGray"  // Background color when focused
                                "    color: DeepPink;"
                                "    border: 3px solid Aqua;"    // Blue border when focused
                                "}";

const QString list_widget_style = "QListWidget {"
                                  "background-color: #2e3440;"
                                  "border: 2px solid Aqua;"
                                  "border-radius: 10px;"
                                  "padding: 5px;"
                                  "}"

                                  "QListWidget::item {"
                                  "padding: 10px;"
                                  "margin: 5px;"
                                  "background-color: AntiqueWhite;"
                                  "border-radius: 5px;"
                                  "color: black;"
                                  "}"
                                  "QListWidget::item:hover {"
                                  "background-color: #a0a0ff;"
                                  "}"
                                  "QListWidget::item:selected {"
                                  "background-color: DeepPink;"
                                  "color: black;"
                                  "}"

                                  "QScrollBar:vertical {"
                                  "background: #e0e0e0;"
                                  "}"

                                  "QScrollBar::handle:vertical {"
                                  "background: #888;"
                                  "}"

                                  "QScrollBar:horizontal {"
                                  "background: #e0e0e0;"
                                  "}"

                                  "QScrollBar::handle:horizontal {"
                                  "background: #888;"
                                  "}";

const QString pushbtn_style = "QPushButton {"
                                 "background-color: transparent;"            // Transparent background
                                 "color: Aqua;"                           // Text color
                                 "font-weight: bold;"
                                 "border: 3px solid Aqua;"                // Border with custom color
                                 "border-radius: 5px;"                       // Rounded corners
                                 "padding: 10px 10px;"                       // Padding
                                 "font-size: 14px;"
                                 "}"

                                 "QPushButton:hover {"
                                 "background-color: DeepPink;"                // Background fills on hover
                                 "color: white;"                             // Text turns white on hover
                                 "}"

                                 "QPushButton:pressed {"
                                 "color: Aqua;"
                                 "background-color: black;"                // Darker background when pressed
                                 "}";

const QString scroll_area_style = "QScrollArea {"
                                  "    background-color: Aqua;"        // Light background color
                                  "    border: 1px solid #bdc3c7;"        // Border around the scroll area
                                  "    border-radius: 10px;"              // Rounded corners for the scroll area
                                  "    padding: 10px;"                    // Padding inside the scroll area
                                  "}"

                                  "QScrollBar:vertical {"
                                  "    border: none;"
                                  "    background-color: #e0e0e0;"        // Scrollbar background color
                                  "    width: 12px;"
                                  "    margin: 0px;"
                                  "    border-radius: 6px;"               // Rounded scrollbar for a modern look
                                  "}"

                                  "QScrollBar::handle:vertical {"
                                  "    background-color: #888;"        // Handle color
                                  "    border-radius: 6px;"
                                  "    min-height: 20px;"                 // Minimum height for the handle
                                  "}";

const QString label_style = "QLabel {"
                            "    background-color: #ecf0f1;"      // Light gray background
                            "    color: #2c3e50;"                 // Dark gray text color
                            "    border: 2px solid #3498db;"      // Blue border
                            "    border-radius: 10px;"            // Rounded corners
                            "    padding: 10px;"                  // Padding for a clean look
                            "    font-size: 14px;"                // Text size
                            "}";

const QString label_style_hover = "QLabel {"
                                  "    background-color: AntiqueWhite;"         // Initial background color
                                  "    color: black;"                      // Text color
                                  "    padding: 10px;"
                                  "    border-radius: 5px;"                // Rounded corners
                                  "}";

const QString text_edit_style = "QTextEdit {"
                                "    background-color: #2c3e50;"       // Dark background
                                "    color: #ecf0f1;"                  // Light text color
                                "    border: 2px solid Aqua;"       // Neon blue border
                                "    padding: 10px;"                   // Padding inside the text edit
                                "    font-size: 16px;"                 // Font size
                                "    font-family: 'Courier New';"      // Custom font
                                "}";

//------------------------------------------------------------------------------------
