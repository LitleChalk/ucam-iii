#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)

{
    stacked_widget = new QStackedWidget(this);

    page1 = new QWidget(this);
    page2 = new QWidget(this);

    switch_windows = new QPushButton(this);
    connect(switch_windows, &QPushButton::clicked, this,  &Widget::ChangePage);

    // 1 стр
    {
    //контейнеры
    photo_section = new QWidget(page1);
    control_section = new QWidget(page1);

    buttons_section = new QWidget(control_section);
    info_section = new QWidget(control_section);
    //разметка
    main_layout = new QVBoxLayout(this);
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
    photo_label->setGeometry(photo_frame->rect());
    photo_label->setAlignment(Qt::AlignCenter);

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
    main_layout->addWidget(stacked_widget);
    main_layout->setContentsMargins(0, 0, 0, 0);

    page1_layout->addWidget(photo_section, 100-control_max_percent);
    page1_layout->addWidget(control_section, control_max_percent);

    photo_layout->addWidget(photo_frame);
    photo_layout->setContentsMargins(0, 0, 0, 0);
    photo_layout->setSpacing(0);

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

    auto_request_layout->addWidget(auto_title, 1);
    auto_request_layout->addWidget(start_auto_request);
    auto_request_layout->addWidget(stop_auto_request);

    buttons_layout->addLayout(auto_request_layout);
    buttons_layout->addWidget(photo_request);
    buttons_layout->addWidget(load_info);
    //параметры рамки
    photo_frame->setFrameShape(QFrame::Box);
    photo_frame->setLineWidth(3);
    //параметры фото
    photo_label->setAlignment(Qt::AlignCenter);
    //параметры кнопок
    start_auto_request->setText("Старт");
    stop_auto_request->setText("Стоп");
    photo_request->setText("Запросить фото");
    load_info->setText("Загрузить информацию");
    change_info->setText("Редактировать");
    save_info->setText("Сохранить");
    switch_windows->setText("Страница");
    //параметры размеров
    resize(window_start_width, window_start_height);
    switch_windows->adjustSize();
    buttons_layout->setContentsMargins(0, 13, 0, 0);
    stacked_widget->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
        );
    photo_section->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
        );
    photo_frame->setMinimumSize(250, 250);
    photo_frame->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
        );
    control_section->setMinimumWidth(control_min_width);
    start_auto_request->setMinimumWidth(60);
    stop_auto_request->setMinimumWidth(60);
    }
    // 2 стр
    settings_scroll = new QScrollArea(page2);
    settings_scroll->setWidgetResizable(true);

    QWidget *settings_content = new QWidget();
    settings_scroll->setWidget(settings_content);

    settings_layout = new QVBoxLayout(settings_content);
}
void Widget::ChangePage(){

    stacked_widget->setCurrentIndex(abs(stacked_widget->currentIndex()-1));
}
void Widget::resizeEvent(QResizeEvent *event)
{
    //подгон размеров фото рамки
    QWidget::resizeEvent(event);
    if (!original_photo.isNull())
    {
        photo_label->setPixmap(
            original_photo.scaled(
                photo_frame->size(),
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                )
            );
        photo_label->setGeometry(photo_frame->rect());
    }
    // смена страниц в правый верхний угол
    switch_windows->move(
        width() - switch_windows->width() - 5,
        5
        );
}
Widget::~Widget() = default;
