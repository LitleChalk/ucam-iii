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
};

#endif // WIDGET_H