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
#include <QScrollBar>
#include <QSettings>
#include <QMessageBox>
#include <QTimer>
#include <QInputDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include "Photo.h"

const double control_max_percent = 20;

const int window_start_width = 1200;
const int window_start_height = 700;

const int control_min_width = 300;
const int LineWidth = 3;
const int switch_button_margin = 5;

extern bool requestInProgress;

const QStringList rawResolutions = {
    "80x60",
    "160x120",
    "128x128",
    "128x96"
};

const QStringList jpegResolutions = {
    "160x128",
    "320x240",
    "640x480",
    "128x96"
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent =nullptr);
    ~Widget() override;
    int current_ID=0;
    QLabel
        *number_value,
        *time_value,
        *photo_id_value,
        *cap_is_set_value,
        *info_changed;

    QFrame *photo_frame;
    QLabel *photo_label;

private:
    Photo photo;

    QStackedWidget *stacked_widget;

    QWidget *page1, *page2;

    QPushButton *switch_windows;

    QTimer *autoRequestTimer;

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
        *photo_id_label,
        *cap_is_set_label;

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
        *settings_content,
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
        *reset_id_label,
        *polling_frequency_label,
        *save_objects_label,
        *resolution_label,
        *photo_format_label;

    QLineEdit
        *camera_number_input,
        *batch_number_input,
        *polling_frequency_input;

    QComboBox
        *reset_id_combo,
        *tracked_objects_combo,
        *data_format_combo,
        *resolution_combo,
        *photo_format_combo;

    QVBoxLayout
        *settings_layout,
        *object_info_layout,
        *auto_mode_layout,
        *photo_settings_layout,
        *save_settings_layout;

    QGridLayout
        *object_info_grid,
        *auto_mode_grid,
        *photo_settings_grid;

    QPushButton
        *save_settings_button,
        *set_default_settings_button;


    QByteArray imageBuffer;
private slots:
    void ChangePage();
    void ChangeSettings();
    void DisplayCurrentSettings(const QString &fileName = "settings.ini");
    void LoadComboFromSettings(QSettings &settings, const QString &key, QComboBox *combo);
    void SetDefaultSettings();
    void UpdateResolutionCombo();
    void showRawImage(const RawImage& image);
    bool showRawImage2(const uint8_t *data,
                          int width,
                       int height);
    bool showRawFileImage(const QString &fileName,
                          int id,
                          int width,
                          int height);
    void loadFromFile();
    void startAutoRequest();
    void stopAutoRequest();
protected:
    void resizeEvent(QResizeEvent *event) override;
    void photoRequest();
    void setPhotoFrameColor(const QColor &color);
};

class PhotoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PhotoDialog(QWidget *parent = nullptr);

    int getId() const;
    int getCameraId() const;
    QString getBatch() const;

    QLineEdit *idEdit;
    QLineEdit *cameraIdEdit;
    QLineEdit *batchEdit;
};

#endif // WIDGET_H