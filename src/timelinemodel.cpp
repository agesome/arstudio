#include "timelinemodel.hpp"

TimeLineModel::TimeLineModel(int mn, int mx)
{
    setMin(mn);
    setMax(mx);
    current_frame = 1;
}

bool TimeLineModel::setCurFrame(int nframe)
{
    if(nframe > max)
        return false;
    current_frame=nframe;
    emit newFrame(current_frame);
    return true;
}

bool TimeLineModel::incCurFrame()
{
    return setCurFrame(current_frame+1);

}

int TimeLineModel::getCurFrame()
{
    return current_frame;
}


void TimeLineModel::setMin(int mn)
{
    min = mn;

}


void TimeLineModel::setMax(int mx)
{
    max = mx;
}



int TimeLineModel::getMin()
{
    return min;
}


int TimeLineModel::getMax()
{
    return max;
}
