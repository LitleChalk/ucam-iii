#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)

{
    stacked_widget = new QStackedWidget(this);

    page1 = new QWidget(this);
    page2 = new QWidget(this);

    switch_windows = new QPushButton(this);
    // 1 стр
    //контейнеры
    photo_section = new QWidget(page1);
    control_section = new QWidget(page1);

    buttons_section = new QWidget(control_section);
    info_section = new QWidget(control_section);
    //разметка
    page1_layout = new QHBoxLayout(page1);
    photo_layout = new QVBoxLayout(photo_section);
    control_layout = new QVBoxLayout(control_section);
    buttons_layout = new QVBoxLayout(buttons_section);
    auto_request_layout = new QHBoxLayout();
    info_layout = new QVBoxLayout(info_section);
    info_grid = new QGridLayout();
    //элементы
    photo_frame = new QFrame(photo_section);
    photo_label = new QLabel(photo_frame);

    auto_title = new QLabel("Автоматический запрос");
    start_auto_request = new QPushButton(buttons_section);
    stop_auto_request = new QPushButton(buttons_section);
    photo_request = new QPushButton(buttons_section);
    load_info = new QPushButton(buttons_section);

    change_info = new QPushButton(info_section);
    save_info = new QPushButton(info_section);

    info_title = new QLabel("Инфо:");
    number_label = new QLabel("№");
    time_label = new QLabel("Время");

    number_value = new QLabel("-");
    time_value = new QLabel("-");
    //сборка
    stacked_widget->addWidget(page1);
    stacked_widget->addWidget(page2);

    page1_layout->addWidget(photo_section);
    page1_layout->addWidget(control_section);

    photo_layout->addWidget(photo_frame, 0, Qt::AlignCenter);

    info_grid->addWidget(info_title,    0, 0);
    info_grid->addWidget(change_info,   0, 1);
    info_grid->addWidget(save_info,     0, 2);
    info_grid->addWidget(number_label,  1, 0);
    info_grid->addWidget(number_value,  1, 1, 1, 2);
    info_grid->addWidget(time_label,    2, 0);
    info_grid->addWidget(time_value,    2, 1, 1, 2);
    info_layout->addLayout(info_grid);

    control_layout->addWidget(buttons_section);
    control_layout->addWidget(info_section);

    auto_request_layout->addWidget(auto_title);
    auto_request_layout->addWidget(start_auto_request);
    auto_request_layout->addWidget(stop_auto_request);

    buttons_layout->addLayout(auto_request_layout);
    buttons_layout->addWidget(photo_request);
    buttons_layout->addWidget(load_info);
    //параметры рамки
    photo_frame->setFrameShape(QFrame::Box);
    photo_frame->setLineWidth(3);
    photo_frame->setFixedSize(250, 250);
    //параметры фото
    photo_label->setAlignment(Qt::AlignCenter);
    photo_label->setScaledContents(true);
    photo_label->setFixedSize(photo_frame->size());
    //параметры кнопок
    start_auto_request->setText("Старт");
    start_auto_request->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    stop_auto_request->setText("Стоп");
    stop_auto_request->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    photo_request->setText("Запросить фото");
    load_info->setText("Загрузить информацию");
    change_info->setText("Редактировать");
    save_info->setText("Сохранить");
    //параметры инфо
    //info_grid->setHorizontalSpacing(5);
    //number_value->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}

Widget::~Widget() = default;
