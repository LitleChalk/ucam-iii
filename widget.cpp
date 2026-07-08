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
    number_label = new QLabel("№ камеры");
    time_label = new QLabel("Время");
    photo_id_label = new QLabel("ID снимка");
    cap_is_set_label = new QLabel("Наличие крышки");
    info_changed = new QLabel("Было произведено ручное изменение");
    number_value = new QLabel("-");
    time_value = new QLabel("-");
    photo_id_value = new QLabel("-");
    cap_is_set_value = new QLabel("-");
    //сборка
    stacked_widget->addWidget(page1);
    stacked_widget->addWidget(page2);
    //main_layout->addWidget(switch_windows, 0, Qt::AlignRight);
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
    info_grid->addWidget(photo_id_label,  3, 0);
    info_grid->addWidget(photo_id_value,  3, 1, 1, 2);
    info_grid->addWidget(cap_is_set_label,    4, 0);
    info_grid->addWidget(cap_is_set_value,    4, 1, 1, 2);
    info_grid->addWidget(info_changed,    5, 0,1,3);
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
    /*switch_windows->setSizePolicy(
        QSizePolicy::Maximum,
        QSizePolicy::Maximum
        );*/
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
    {//контейнеры
    settings_scroll = new QScrollArea(page2);
    settings_content = new QWidget();

    object_info_section = new QWidget(page2);
    auto_mode_section = new QWidget(page2);
    photo_settings_section = new QWidget(page2);
    save_settings_section = new QWidget(page2);
    //разметка
    page2_layout = new QVBoxLayout(page2);

    settings_layout = new QVBoxLayout();
    object_info_layout = new QVBoxLayout(object_info_section);
    auto_mode_layout = new QVBoxLayout(auto_mode_section);
    photo_settings_layout = new QVBoxLayout(photo_settings_section);
    save_settings_layout = new QVBoxLayout(save_settings_section);

    object_info_grid = new QGridLayout();
    auto_mode_grid = new QGridLayout();
    photo_settings_grid= new QGridLayout();
    //элементы
    object_info_title = new QLabel("Сохраняемая информация");
    camera_number_label = new QLabel("Номер камеры");
    batch_number_label = new QLabel("Номер партии");
    reset_frequency_label = new QLabel("Частота обнуления ID");

    camera_number_input = new QLineEdit();
    batch_number_input = new QLineEdit();
    reset_frequency_combo = new QComboBox();

    auto_mode_title = new QLabel("Автоматический режим");
    polling_frequency_label = new QLabel("Частота опроса (сек)");
    save_objects_label = new QLabel("Сохраняемая информация");

    polling_frequency_input = new QLineEdit();
    tracked_objects_combo = new QComboBox();
    data_format_combo = new QComboBox();

    photo_settings_title = new QLabel("Параметры фото");
    resolution_label = new QLabel("Разрешение");
    format_label = new QLabel("Формат");
    resolution_combo = new QComboBox();
    format_combo = new QComboBox();

    save_settings_button = new QPushButton("Сохранить настройки");
    //сборка
    settings_scroll->setWidgetResizable(true);
    settings_scroll->setWidget(settings_content);
    settings_content->setLayout(settings_layout);
    //параметры сохранения
    object_info_grid->setAlignment(Qt::AlignLeft);
    object_info_grid->setColumnStretch(2, 1);
    object_info_grid->setHorizontalSpacing(15);

    object_info_grid->addWidget(camera_number_label, 0, 0);
    object_info_grid->addWidget(camera_number_input, 0, 1);

    object_info_grid->addWidget(batch_number_label, 1, 0);
    object_info_grid->addWidget(batch_number_input, 1, 1);

    object_info_grid->addWidget(reset_frequency_label, 2, 0);
    object_info_grid->addWidget(reset_frequency_combo, 2, 1);

    object_info_layout->addWidget(object_info_title);
    object_info_layout->addLayout(object_info_grid);
    object_info_section->setStyleSheet(
        "QWidget {"
        "border-bottom: 1px solid gray;"
        "}"
        );
    // параметры автоматического режима
    auto_mode_grid->setAlignment(Qt::AlignLeft);
    auto_mode_grid->setColumnStretch(3, 1);
    auto_mode_grid->setHorizontalSpacing(15);

    auto_mode_grid->addWidget(polling_frequency_label,0,0);
    auto_mode_grid->addWidget(polling_frequency_input,0,1,1,2);

    auto_mode_grid->addWidget(save_objects_label,1,0);
    auto_mode_grid->addWidget(tracked_objects_combo,1,1);
    auto_mode_grid->addWidget(data_format_combo,1,2);

    auto_mode_layout->addWidget(auto_mode_title);
    auto_mode_layout->addLayout(auto_mode_grid);
    auto_mode_section->setStyleSheet(
        "QWidget {"
        "border-bottom: 1px solid gray;"
        "}"
        );
    // параметры фото
    photo_settings_grid->setAlignment(Qt::AlignLeft);
    photo_settings_grid->setColumnStretch(2, 1);
    photo_settings_grid->setHorizontalSpacing(15);

    photo_settings_grid->addWidget(resolution_label,0,0);
    photo_settings_grid->addWidget(resolution_combo,0,1);

    photo_settings_grid->addWidget(format_label,1,0);
    photo_settings_grid->addWidget(format_combo,1,1);

    photo_settings_layout->addWidget(photo_settings_title);
    photo_settings_layout->addLayout(photo_settings_grid);
    photo_settings_section->setStyleSheet(
        "QWidget {"
        "border-bottom: 1px solid gray;"
        "}"
        );
    // сохранение
    save_settings_layout->addWidget(save_settings_button);
    // общая страница
    settings_layout->addWidget(object_info_section);
    settings_layout->addWidget(auto_mode_section);
    settings_layout->addWidget(photo_settings_section);
    settings_layout->addWidget(save_settings_section);

    settings_content->setLayout(settings_layout);

    page2_layout->addWidget(settings_scroll);
    //параметры стилей (временное)
    object_info_title->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        );
    auto_mode_title->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        );
    photo_settings_title->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        );

    //параметры подписей
    camera_number_label->setMinimumWidth(180);
    batch_number_label->setMinimumWidth(180);
    reset_frequency_label->setMinimumWidth(180);
    //параметры выпадающих списков
    reset_frequency_combo->addItem("Каждую партию");
    reset_frequency_combo->addItem("Каждый день");

    tracked_objects_combo->addItem("Все");
    tracked_objects_combo->addItem("Успешные");
    tracked_objects_combo->addItem("Бракованные");

    data_format_combo->addItem("Инфо");
    data_format_combo->addItem("Фото");
    data_format_combo->addItem("Инфо+фото");

    resolution_combo->addItem("160x120");

    format_combo->addItem("raw");
    format_combo->addItem("jpeg");
    //параметры размеров
    settings_scroll->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
        );

    camera_number_input->setMinimumWidth(150);
    batch_number_input->setMinimumWidth(150);

    reset_frequency_combo->setMinimumWidth(150);

    save_settings_button->setMinimumHeight(30);
    settings_scroll->setFrameShape(QFrame::NoFrame);
    }
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
