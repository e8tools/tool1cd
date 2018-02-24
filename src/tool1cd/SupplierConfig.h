#ifndef SUPPLIERCONFIG_H
#define SUPPLIERCONFIG_H

#include <boost/filesystem.hpp>

#include "TableFiles.h"

struct TableFile;

// класс конфигурации поставщика
class SupplierConfig
{
public:
	SupplierConfig(TableFile *file,
				   const std::string &name,
				   const std::string &supplier,
				   const std::string &version);
	~SupplierConfig() = default;

	bool save_to_file(const boost::filesystem::path& file_name);
	std::string name() const;
	std::string supplier() const;
	std::string version() const;

	static std::shared_ptr<SupplierConfig> create_supplier_config(TableFile* table_file);

private:
	TableFile* _file {nullptr};
	std::string _name;     // имя конфигурация поставщика
	std::string _supplier; // синоним конфигурация поставщика
	std::string _version;  // версия конфигурация поставщика
};

#endif // SUPPLIERCONFIG_H
