#include <algo_pipeline.hpp>

std::list <AlgoPipeline::module_t> AlgoPipeline::modules;

AlgoPipeline::AlgoPipeline ()
{
	int rc;

	try
		{
			rc = lt_dlinit ();
			if (rc != 0)
				throw rc;
		}
	catch (int e)
		{
			throw;
		}
}

AlgoPipeline::~AlgoPipeline ()
{
	lt_dlexit ();
}

void
AlgoPipeline::loadModules (Config * config)
{
	filesystem::directory_iterator dir (MODULE_SEARCH_PATH);
	filesystem::directory_iterator end;
	module_t module;

// TODO: error checking
	for (; dir != end; dir++)
		{
			filesystem::path p = dir->path ();
			if (p.extension () == ".xml")
				{
					module.name = p.stem ().string ();
					std::cout << "module loaded: " << module.name << std::endl;
					module.handle = lt_dlopenext ((MODULE_SEARCH_PATH + module.name).c_str ());
					*(void * *) (&module.process) = lt_dlsym (module.handle, "process");
					modules.push_back (module);
					importXml (p, config);
				}
		}
}

bool
AlgoPipeline::importXml (const filesystem::path & fp, Config * config)
{
	static int row = 0;
	int subrow = 0;

	QString fpath = QString::fromStdString (fp.string ());
	QFile desc_file (fpath);
	QDomDocument desc;
	QStandardItem   *i, *c;

	if (!desc_file.open (QIODevice::ReadOnly))
		{
			return false;
		}
	if (!desc.setContent (&desc_file))
		{
			desc_file.close ();
			return false;
		}
	desc_file.close ();
	QDomElement root = desc.documentElement ();
	if (root.tagName () != "module")
		return false;
	QString moduleName = QString::fromStdString (fp.stem ().string ());
	i = new QStandardItem (moduleName);
	i->setEditable (false);
	config->insertRow (row++, i);

	QDomElement p = root.firstChildElement ("parameter");
	for (; p != QDomElement (); p = p.nextSiblingElement ("parameter"))
		{
			QString sn (p.firstChildElement ("symbol").text ());
			c = new QStandardItem (sn);
			i->setChild (subrow, 0, c);
			c->setEditable (false);
			QString sv (p.firstChildElement ("value").text ());
			c = new QStandardItem (sv);
			i->setChild (subrow++, 1, c);
			// set default value
			double * v = accessModuleVariable (fp.stem ().string (), sn.toStdString ());
			*v = sv.toDouble ();
		}

	return true;
}


double *
AlgoPipeline::accessModuleVariable (std::string mod, std::string var)
{
	for (auto m : modules)
		{
			if (m.name == mod)
				{
					return (double *) lt_dlsym (m.handle, var.c_str ());
				}
		}
	return NULL;
}

void
AlgoPipeline::processFrame (cv::Mat & image, cv::Mat & dmap)
{
	for (auto m : modules)
		{
			std::cout << "processing with '" << m.name << "': "
			          << (m.process (image, dmap) ? "[OK]" : "[FAIL]") << std::endl;
		}
	Logger::getInstance ().advanceFrame ();
}


void
AlgoPipeline::itemChangedHandler (QStandardItem *item)
{
	QString mn = item->parent ()->text ();
	QString sn = item->parent ()->child (0, 0)->text ();
	QString sv = item->text ();
	double * v = accessModuleVariable (mn.toStdString (), sn.toStdString ());

	*v = sv.toDouble ();
	std::cout << "changed symbol '" << sn.toStdString () << "' in module '"
	          << mn.toStdString () << "' to " << sv.toDouble () << std::endl;
}
