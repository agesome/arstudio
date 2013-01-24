#ifndef CONFIG_H
#define CONFIG_H

#include <QStandardItemModel>
#include <QDomDocument>
#include <QFile>
#include <string>
#include <boost/filesystem.hpp>

using namespace boost;

class Config : public QStandardItemModel
{
public:
	static Config & getInstance (void);

	bool importXml (const filesystem::path &);
private:
	Config ();
	Config (const Config &);
	void operator= (const Config &);
};

#endif // CONFIG_H