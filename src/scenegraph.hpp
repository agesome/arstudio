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
typedef boost::shared_ptr<QCheckBox> qbox_ptr;

class Scenegraph : public QWidget
{
	Q_OBJECT
public:
	Scenegraph(QMainWindow * MainWindow = nullptr);

	void addCheckbox (int);
	void addSequence (seq_ptr);

	std::list<seq_ptr> sequences;
private:
	boost::shared_ptr<QVBoxLayout> layout;
	std::list< std::pair<int, qbox_ptr> > boxes;
signals:
	
public slots:
	
};

}
#endif // SCENEGRAPH_H
