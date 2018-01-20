#ifndef SUPPLIERCONFIG_H
#define SUPPLIERCONFIG_H

#include <boost/filesystem.hpp>

#include "TableFiles.h"

struct TableFile;

// класс конфигурации поставщика
class SupplierConfig
{
public:
	SupplierConfig(TableFile* file,
								 const String& name,
								 const String& supplier,
								 const String& version);
	~SupplierConfig() = default;

	bool save_to_file(const boost::filesystem::path& file_name);
	String name() const;
	String supplier() const;
	String version() const;

private:
	TableFile* _file {nullptr};
	String _name;     // имя конфигурация поставщика
	String _supplier; // синоним конфигурация поставщика
	String _version;  // версия конфигурация поставщика
};

#endif // SUPPLIERCONFIG_H
