#include <TimeLineModel.hpp>

TimeLineModel::TimeLineModel (int min, int max)
{
  this->min_frame_     = min;
  this->max_frame_     = max;
  this->current_frame_ = 1;
}

bool
TimeLineModel::set_frame (int frame)
{
  if (frame > max_frame_ || frame < min_frame_)
    return false;
  current_frame_ = frame;
  new_frame (current_frame_);
  return true;
}

bool
TimeLineModel::next_frame (void)
{
  return set_frame (current_frame_ + 1);
}

int
TimeLineModel::current_frame (void)
{
  return current_frame_;
}

void
TimeLineModel::set_range (int min, int max)
{
  this->min_frame_ = min;
  this->max_frame_ = max;
}

int
TimeLineModel::min_frame (void)
{
  return min_frame_;
}

int
TimeLineModel::max_frame (void)
{
  return max_frame_;
}
