#ifndef TIMELINEMODEL_HPP
#define TIMELINEMODEL_HPP
#include <QObject>

class TimeLineModel : public QObject
{
	Q_OBJECT
public:
	bool setCurFrame (int nframe);
	bool incCurFrame ();
	void setMin (int mn);
	void setMax (int mx);
	int getMin ();
	int getMax ();
	TimeLineModel(int mn, int mx);
	int getCurFrame ();
private:
	int current_frame, min, max;
signals:
	void newFrame (int nfr);
};
#endif // TIMELINEMODEL_HPP
