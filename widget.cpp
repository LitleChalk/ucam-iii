#include "widget.h"
bool requestInProgress = false;

Widget::Widget(QWidget *parent)
    : QWidget(parent)

{
    autoRequestTimer = new QTimer(this);

    connect(autoRequestTimer, &QTimer::timeout,
            this, &Widget::photoRequest);
    stacked_widget = new QStackedWidget(this);

    page1 = new QWidget(this);
    page2 = new QWidget(this);

    switch_windows = new QPushButton(this);

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
        photo_frame->setObjectName("photoFrame");
        photo_label = new QLabel(photo_frame);
        //photo_label->setGeometry(photo_frame->rect());
        photo_label->setAlignment(Qt::AlignCenter);
        photo_label->setStyleSheet("border: none;");

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
        info_changed->setVisible(false);

        control_layout->addWidget(buttons_section);
        control_layout->addWidget(info_section);

        auto_request_layout->addWidget(auto_title, 1);
        auto_request_layout->addWidget(start_auto_request);
        auto_request_layout->addWidget(stop_auto_request);

        buttons_layout->addLayout(auto_request_layout);
        buttons_layout->addWidget(photo_request);
        buttons_layout->addWidget(load_info);
        //параметры рамки
        photo_frame->setFrameShape(QFrame::NoFrame);
        photo_frame->setStyleSheet(
            QString("#photoFrame { border:%1px solid black; }")
                .arg(LineWidth));
        //параметры фото
        photo_label->setAlignment(Qt::AlignCenter);
        //параметры кнопок
        start_auto_request->setText("Старт");
        stop_auto_request->setText("Стоп");
        stop_auto_request->setEnabled(false);
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
        reset_id_label = new QLabel("Частота обновления ID");

        camera_number_input = new QLineEdit();
        batch_number_input = new QLineEdit();
        reset_id_combo = new QComboBox();

        auto_mode_title = new QLabel("Автоматический режим");
        polling_frequency_label = new QLabel("Частота опроса (сек)");
        save_objects_label = new QLabel("Сохраняемая информация");

        polling_frequency_input = new QLineEdit();
        tracked_objects_combo = new QComboBox();
        data_format_combo = new QComboBox();

        photo_settings_title = new QLabel("Параметры фото");
        resolution_label = new QLabel("Разрешение");
        photo_format_label = new QLabel("Формат");
        resolution_combo = new QComboBox();
        photo_format_combo = new QComboBox();

        save_settings_button = new QPushButton("Сохранить настройки");
        set_default_settings_button =  new QPushButton("Установить стандартные настройки");
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

        object_info_grid->addWidget(reset_id_label, 2, 0);
        object_info_grid->addWidget(reset_id_combo, 2, 1);

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

        photo_settings_grid->addWidget(photo_format_label,0,0);
        photo_settings_grid->addWidget(photo_format_combo,0,1);

        photo_settings_grid->addWidget(resolution_label,1,0);
        photo_settings_grid->addWidget(resolution_combo,1,1);

        photo_settings_layout->addWidget(photo_settings_title);
        photo_settings_layout->addLayout(photo_settings_grid);
        photo_settings_section->setStyleSheet(
            "QWidget {"
            "border-bottom: 1px solid gray;"
            "}"
            );
        // сохранение
        save_settings_layout->addWidget(save_settings_button);
        save_settings_layout->addSpacing(15);
        save_settings_layout->addWidget(set_default_settings_button);
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
        for (QGridLayout *grid : {photo_settings_grid, auto_mode_grid, object_info_grid})
        {
            grid->setColumnMinimumWidth(0, 200);
            grid->setColumnStretch(0, 0);
        }
        //параметры подписей
        camera_number_label->setMinimumWidth(180);
        batch_number_label->setMinimumWidth(180);
        reset_id_label->setMinimumWidth(180);
        //параметры выпадающих списков
        reset_id_combo->addItem("Каждую партию");
        reset_id_combo->addItem("Каждый день");

        tracked_objects_combo->addItem("Все");
        tracked_objects_combo->addItem("Успешные");
        tracked_objects_combo->addItem("Бракованные");

        data_format_combo->addItem("Инфо");
        data_format_combo->addItem("Фото");
        data_format_combo->addItem("Инфо+фото");

        //resolution_combo->addItem("160x120");

        photo_format_combo->addItem("raw 8-bit for Y only");
        //photo_format_combo->addItem("raw CrYCbY");
        //photo_format_combo->addItem("raw 565(RGB)");
        //photo_format_combo->addItem("jpeg");

        //параметры размеров
        settings_scroll->setSizePolicy(
            QSizePolicy::Expanding,
            QSizePolicy::Expanding
            );

        camera_number_input->setMinimumWidth(150);
        batch_number_input->setMinimumWidth(150);

        reset_id_combo->setMinimumWidth(150);

        save_settings_button->setMinimumHeight(30);
        set_default_settings_button->setMinimumHeight(30);
        settings_scroll->setFrameShape(QFrame::NoFrame);
    }
    //Подключение кнопок
    connect(switch_windows, &QPushButton::clicked, this,  &Widget::ChangePage);
    connect(save_settings_button, &QPushButton::clicked, this,  &Widget::ChangeSettings);
    connect(set_default_settings_button, &QPushButton::clicked, this,  &Widget::SetDefaultSettings);
    connect(photo_format_combo,&QComboBox::currentTextChanged,this,&Widget::UpdateResolutionCombo);
    connect(photo_request,&QPushButton::clicked,this,&Widget::photoRequest);
    connect(load_info,&QPushButton::clicked,this,&Widget::loadFromFile);
    connect(start_auto_request, &QPushButton::clicked,this, &Widget::startAutoRequest);
    connect(stop_auto_request, &QPushButton::clicked,this, &Widget::stopAutoRequest);

    DisplayCurrentSettings();
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
        photo_label->setGeometry(photo_frame->rect().adjusted(
            LineWidth,
            LineWidth,
            -LineWidth,
            -LineWidth));
        //photo_label->setGeometry(photo_frame->rect());
    }
    // смена страниц в правый верхний угол
    switch_windows->move(
        width() - switch_windows->width() - 5,
        5
        );

}
void Widget::ChangePage(){
    int index=abs(stacked_widget->currentIndex()-1);
    stacked_widget->setCurrentIndex(index);
    UpdateResolutionCombo();
    DisplayCurrentSettings();
}
void Widget::ChangeSettings(){
    QSettings settings("settings.ini", QSettings::IniFormat);
    bool ok1;
    int batch_number = this->batch_number_input->text().toInt(&ok1);

    if (!ok1)
    {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Номер партии должен быть целым числом.");
        return;
    }
    int pollingFrequency = this->polling_frequency_input->text().toInt(&ok1);
    if (!ok1)
    {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Частота опроса должна быть целым числом.");
        return;
    }
    settings.setValue("camera_number", this->camera_number_input->text());
    settings.setValue("batch_number", batch_number);
    settings.setValue("reset_ID", this->reset_id_combo->currentText());
    settings.setValue("polling_frequency", pollingFrequency);
    settings.setValue("tracked_objects", this->tracked_objects_combo->currentText());
    settings.setValue("data_format", this->data_format_combo->currentText());
    settings.setValue("resolution", this->resolution_combo->currentText());
    settings.setValue("format", this->photo_format_combo->currentText());

    settings.sync();
}
void Widget::DisplayCurrentSettings(const QString &fileName) // default fileName = "settings.ini"
{
    QSettings settings(fileName, QSettings::IniFormat);

    if (settings.contains("camera_number"))
        camera_number_input->setText(settings.value("camera_number").toString());

    if (settings.contains("batch_number"))
        batch_number_input->setText(settings.value("batch_number").toString());

    if (settings.contains("polling_frequency"))
        polling_frequency_input->setText(settings.value("polling_frequency").toString());

    LoadComboFromSettings(settings, "reset_ID", reset_id_combo);
    LoadComboFromSettings(settings, "tracked_objects", tracked_objects_combo);
    LoadComboFromSettings(settings, "data_format", data_format_combo);
    LoadComboFromSettings(settings, "resolution", resolution_combo);
    LoadComboFromSettings(settings, "format", photo_format_combo);
}
void Widget::LoadComboFromSettings(QSettings &settings, const QString &key, QComboBox *combo)
{
    if (!settings.contains(key))
        return;

    QString value = settings.value(key).toString();
    int index = combo->findText(value);
    if (index != -1)
        combo->setCurrentIndex(index);
}
void Widget::SetDefaultSettings(){
    DisplayCurrentSettings("default_settings.ini");
    ChangeSettings();
}
void Widget::UpdateResolutionCombo()
{
    resolution_combo->clear();

    if (photo_format_combo->currentText() == "jpeg")
    {
        resolution_combo->addItems(jpegResolutions);
    }
    else    // raw
    {
        resolution_combo->addItems(rawResolutions);
    }
    resolution_combo->setCurrentIndex(0);
}
void Widget::photoRequest(){
    if (requestInProgress)
        return;
    requestInProgress = true;
    RawImage image=RawImage(80,60);
    image.GenerateRawImage2();
    //showRawImage(image);
    showRawImage2(image.buffer.data(),80,60);
    setPhotoFrameColor(Qt::red);
    if (photo_format_combo->currentText() == "jpeg"){}
    else{
        if(data_format_combo->currentText() == "Инфо+фото"){
            image.SaveRawImage("D:/projects/coding/ucam-iii/interface1_coding/interface1_coding/saved_info/1",
                               "photo.raw",
                               image.buffer.data(),
                               image.expectedSize);
            image.saveToCsv(photo_format_combo->currentText(),
                            resolution_combo->currentText());
        }
        else if(data_format_combo->currentText() == "Инфо"){
            image.saveToCsv(photo_format_combo->currentText(),
                            resolution_combo->currentText());
        }
        else{
            image.SaveRawImage("D:/projects/coding/ucam-iii/interface1_coding/interface1_coding/saved_info/1",
                               "photo.raw",
                               image.buffer.data(),
                               image.expectedSize);}

    }
    requestInProgress=false;
}

void Widget::setPhotoFrameColor(const QColor &color)
{
    photo_frame->setStyleSheet(
        QString("#photoFrame { border: %1px solid %2; }")
            .arg(LineWidth)
            .arg(color.name()));
}
void Widget::showRawImage(const RawImage& image)
{
    QImage img(image.buffer.data(),
               image.width,
               image.height,
               image.width,              // bytesPerLine
               QImage::Format_Grayscale8);

    QLabel *label = photo_frame->findChild<QLabel *>();

    if (!label)
    {
        label = new QLabel(photo_frame);
        label->setAlignment(Qt::AlignCenter);
        label->setGeometry(photo_frame->rect());
    }

    label->setPixmap(QPixmap::fromImage(img).scaled(
        photo_frame->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation));
}

bool Widget::showRawImage2(const uint8_t *data,
                           int width,
                           int height)
{
    if (data == nullptr)
        return false;

    imageBuffer = QByteArray(reinterpret_cast<const char *>(data),
                             width * height);

    QImage image(reinterpret_cast<const uchar *>(imageBuffer.constData()),
                 width,
                 height,
                 width,
                 QImage::Format_Grayscale8);

    if (photo_label == nullptr)
    {
        photo_label = new QLabel(photo_frame);
        photo_label->setAlignment(Qt::AlignCenter);
    }

    photo_label->setGeometry(photo_frame->rect().adjusted(
        LineWidth,
        LineWidth,
        -LineWidth,
        -LineWidth));
    //photo_label->setGeometry(photo_frame->rect());

    original_photo = QPixmap::fromImage(image);

    photo_label->setPixmap(
        original_photo.scaled(
            photo_frame->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));
    return true;
}
bool Widget::showRawFileImage(const QString &fileName,
                              int width,
                              int height)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QByteArray raw = file.readAll();
    file.close();

    if (raw.size() != width * height)
        return false;

    return showRawImage2(
        reinterpret_cast<const uint8_t *>(raw.constData()),
        width,
        height);
}
void Widget::loadFromFile(){
    PhotoDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        photo = Photo(
            dialog.idEdit->text().toInt(),
            dialog.cameraIdEdit->text().toInt(),
            dialog.batchEdit->text()
            );
    }

    showRawFileImage("D:/projects/coding/ucam-iii/interface1_coding/interface1_coding/saved_info/1/photo.raw",80,60);
}
void Widget::startAutoRequest()
{
    if (!QFile::exists("settings.ini")) {
        QMessageBox::warning(this, "Ошибка", "Настройки не сохранены. Файл настроек settings.ini отсутствует.");
        return;
    }

    QSettings settings("settings.ini", QSettings::IniFormat);

    if (!settings.contains("polling_frequency")) {
        QMessageBox::warning(this, "Ошибка", "В файле отсутствует параметр частоты опроса.");
        return;
    }

    int frequency = settings.value("polling_frequency").toInt(); // секунд

    autoRequestTimer->start(frequency * 1000);

    start_auto_request->setEnabled(false);
    stop_auto_request->setEnabled(true);
}
void Widget::stopAutoRequest()
{
    autoRequestTimer->stop();

    start_auto_request->setEnabled(true);
    stop_auto_request->setEnabled(false);
}
Widget::~Widget() = default;


PhotoDialog::PhotoDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Параметры фото");

    idEdit= new QLineEdit(this);
    cameraIdEdit= new QLineEdit(this);
    batchEdit = new QLineEdit(this);


    QFormLayout *layout = new QFormLayout(this);

    layout->addRow("ID:", idEdit);
    layout->addRow("Camera ID:", cameraIdEdit);
    layout->addRow("Batch:", batchEdit);


    QDialogButtonBox *buttons =
        new QDialogButtonBox(
            QDialogButtonBox::Ok |
                QDialogButtonBox::Cancel,
            this);


    connect(buttons,
            &QDialogButtonBox::accepted,
            this,
            &QDialog::accept);

    connect(buttons,
            &QDialogButtonBox::rejected,
            this,
            &QDialog::reject);


    layout->addWidget(buttons);
}