#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <QMainWindow>
#include <QtGui/QCheckBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QVariant>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/make_shared.hpp>
#include <sequence.hpp>

namespace Workspace
{

class Scenegraph : public QWidget
{
	typedef boost::shared_ptr<QCheckBox> qbox_ptr;

	Q_OBJECT
public:
	Scenegraph(QMainWindow * MainWindow = nullptr);

	void addCheckbox (int);
	void addSequence (Sequence::ptr);

	std::list<Sequence::ptr> sequences;
private:
	boost::shared_ptr<QVBoxLayout> layout;
	std::list< std::pair<int, qbox_ptr> > boxes;
signals:
	
public slots:
	
};

}
#endif // SCENEGRAPH_H
