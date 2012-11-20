#ifndef TIMELINE_MODEL_HPP
#define TIMELINE_MODEL_HPP
#include "QObject"
class timeline_model:public QObject
{
Q_OBJECT
public:
    void setCurFrame(int nframe);
    void incCurFrame( );
    timeline_model();
    int getCurFrame();
private:
    int current_frame;
signals:
    void newFrame(int nfr);
};

#endif // TIMELINE_MODEL_HPP
