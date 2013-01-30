#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include <opencv2/highgui/highgui.hpp>
class Video : public QWidget
{
    Q_OBJECT
public:
    explicit Video(QWidget *parent = 0);
private:
    QLabel *file_name_label = new QLabel(this);
    QLabel *frames_count_label = new QLabel(this);
    QLabel *select_frames_label = new QLabel(this);
    QPushButton *select_file_button = new QPushButton(this);
    QPushButton *process_button = new QPushButton(this);
    QSpinBox *start_frame_spin = new QSpinBox(this);
    QSpinBox *end_frame_spin = new QSpinBox(this);
    QGridLayout *layout = new QGridLayout (this);
    QString file_path;
signals:
    
public slots:
    void select_file();
};

#endif // VIDEO_H
