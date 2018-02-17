#ifndef SUPPLIERCONFIGBUILDER_H
#define SUPPLIERCONFIGBUILDER_H

#include <memory>

#include "SupplierConfig.h"
#include "TableFiles.h"
#include "cfapi/V8Catalog.h"


class SupplierConfigBuilder
{
public:
	SupplierConfigBuilder() = delete;
	explicit SupplierConfigBuilder(TableFile *table_file);

	~SupplierConfigBuilder() = default;

	std::shared_ptr<SupplierConfig> build();

private:
	TableFile *_table_file {nullptr};
	std::unique_ptr<V8Catalog> main_catalog;

	void create_main_catalog();
	int32_t get_version() const;
	V8File* get_file(const std::string &file_name) const;


};

#endif // SUPPLIERCONFIGBUILDER_H
