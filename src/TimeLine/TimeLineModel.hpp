#ifndef TIMELINEMODEL_HPP
#define TIMELINEMODEL_HPP

#include <QObject>

/**
        This class implements a model for the TimeLine widget.
 */

class TimeLineModel : public QObject
{
	Q_OBJECT
public:
	TimeLineModel(int mn, int mx);

	bool setCurFrame (int nframe);
	bool incCurFrame ();
	void setMin (int mn);
	void setMax (int mx);
	int getMin ();
	int getMax ();
	int getCurFrame ();
private:
	int current_frame, min, max;
signals:
	void newFrame (int nfr);
};
#endif // TIMELINEMODEL_HPP
