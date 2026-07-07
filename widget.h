#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QResizeEvent>
#include <QScrollArea>
#include <QLineEdit>
#include <QComboBox>

const double control_max_percent = 20;

const int window_start_width = 1200;
const int window_start_height = 700;

const int control_min_width = 300;

const int switch_button_margin = 5;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent =nullptr);
    ~Widget() override;

private:
    QStackedWidget *stacked_widget;

    QWidget *page1, *page2;

    QPushButton *switch_windows;

    //1 стр
    QPushButton
        *photo_request,
        *change_info,
        *save_info,
        *start_auto_request,
        *stop_auto_request,
        *load_info;

    QLabel
        *auto_title,
        *info_title,
        *number_label,
        *time_label,
        *number_value,
        *time_value;

    QFrame *photo_frame;
    QLabel *photo_label;

    QWidget
        *photo_section,
        *control_section,
        *buttons_section,
        *info_section;

    QHBoxLayout *page1_layout, *auto_request_layout;
    QVBoxLayout
        *main_layout,
        *page2_layout,
        *control_layout,
        *buttons_layout,
        *info_layout,
        *photo_layout;
    QGridLayout *info_grid;
    QPixmap original_photo;

    //2 стр
    QScrollArea *settings_scroll;

    QWidget
        *object_info_section,
        *auto_mode_section,
        *photo_settings_section,
        *save_settings_section;

    QLabel
        *object_info_title,
        *auto_mode_title,
        *photo_settings_title;

    QLabel
        *camera_number_label,
        *batch_number_label,
        *reset_frequency_label;

    QLineEdit
        *camera_number_input,
        *batch_number_input;

    QComboBox
        *reset_frequency_combo;

    QVBoxLayout
        *settings_layout,
        *object_info_layout,
        *auto_mode_layout,
        *photo_settings_layout,
        *save_settings_layout;

    QHBoxLayout
        *camera_number_layout,
        *batch_number_layout,
        *reset_frequency_layout;

    QPushButton
        *save_settings_button;
private slots:
    void ChangePage();
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // WIDGET_H