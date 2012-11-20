#include "timelinemodel.hpp"

TimeLineModel::TimeLineModel()
{

}

void TimeLineModel::setCurFrame(int nframe)
{
    current_frame=nframe;
    emit newFrame(current_frame);
}

void TimeLineModel::incCurFrame()
{
    setCurFrame(current_frame+1);
}

int TimeLineModel::getCurFrame()
{
    return current_frame;
}
