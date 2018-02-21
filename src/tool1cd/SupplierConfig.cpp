#include <memory>

#include "MessageRegistration.h"
#include "SupplierConfig.h"
#include "ConfigStorage.h"
#include "SupplierConfigBuilder.h"
#include "UZLib.h"

extern Registrator msreg_g;

SupplierConfig::SupplierConfig(TableFile *file,
							   const std::string &name,
							   const std::string &supplier,
							   const std::string &version)
	 : _file(file), _name(name), _supplier(supplier), _version(version)
{}

bool SupplierConfig::save_to_file(const boost::filesystem::path& file_name) {

	if (_file == nullptr) {
		throw DetailedException("Не указан файл таблицы")
			.add_detail("Имя файла", file_name.string());
	}

	std::unique_ptr<ContainerFile> container_file( new ContainerFile(_file, _file->name) );
	if(!container_file->open()) {
		return false;
	}

	std::unique_ptr<TFileStream> file_stream;

	try {
		file_stream.reset( new TFileStream(file_name, fmCreate) );
	}
	catch(...) {
		throw DetailedException("Ошибка открытия файла конфигурации поставщика")
			.add_detail("Имя файла", file_name.string());
	}

	try {
		ZInflateStream(container_file->stream, file_stream.get());
	}
	catch(ZError &e) {
		e.add_detail("Имя файла", file_name.string());
		e.show();
		return false;
	}

	return true;
}

std::string SupplierConfig::name() const {
	return _name;
}

std::string SupplierConfig::supplier() const {
	return _supplier;
}

std::string SupplierConfig::version() const {
	return _version;
}


std::shared_ptr<SupplierConfig> SupplierConfig::create_supplier_config(TableFile *table_file) {
	SupplierConfigBuilder builder(table_file);

	return builder.build();
}
