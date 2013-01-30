#include "video.hpp"
Video::Video(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(300);
    this->setMinimumHeight(150);
    this->adjustSize();
    this->move(200,200);
    this->setWindowTitle("Video");
    file_name_label->setText("File: ");
    frames_count_label->setText("Frames: ");
    select_frames_label->setText("Select frames: ");
    select_file_button->setText("Select file");
    process_button->setText("Process");

    layout->addWidget(file_name_label, 0, 0, 1, 3, 0);
    layout->addWidget(frames_count_label, 1, 0, 1, 3, 0);
    layout->addWidget(select_frames_label, 2, 0, 1, 1, 0);
    layout->addWidget(start_frame_spin, 2, 1, 1, 1, 0);
    layout->addWidget(end_frame_spin, 2, 2, 1, 1, 0);
    layout->addWidget(select_file_button, 3, 0, 1, 1, 0);
    layout->addWidget(process_button, 3, 1, 1, 1, 0);

    connect (select_file_button, SIGNAL (clicked ()), this, SLOT (select_file()));


}


void Video::select_file()
{
    file_path = QFileDialog::getOpenFileName(this,
        tr("Open Video"), "~", tr("Video Files (*.avi *.mkv *.wmv *.mp4)"));
    file_name_label->setText("File: "+ file_path);
    file_name_label->setToolTip(file_path);

    cv::VideoCapture capture(file_path.toStdString());
    int frames_count;
    if(!capture.isOpened())
        frames_count = -1;
    else
        frames_count = capture.get(CV_CAP_PROP_FRAME_COUNT);
    frames_count_label->setText("Frames: "+ QString::number(frames_count));

}
