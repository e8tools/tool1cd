#include <memory>

#include "MessageRegistration.h"
#include "SupplierConfig.h"
#include "ConfigStorage.h"
#include "UZLib.h"

extern Registrator msreg_g;

SupplierConfig::SupplierConfig(TableFile *file,
							   const System::String &name,
							   const System::String &supplier,
							   const System::String &version)
	 : _file(file), _name(name), _supplier(supplier), _version(version)
{}

bool SupplierConfig::save_to_file(const boost::filesystem::path& file_name) {

	if (_file == nullptr) {
		msreg_g.AddError("Не указан файл таблицы",
			"Имя файла", file_name.string());
		return false;
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
		msreg_g.AddError("Ошибка открытия файла конфигурации поставщика",
			"Имя файла", file_name.string());
		return false;
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

String SupplierConfig::name() const {
	return _name;
}

String SupplierConfig::supplier() const {
	return _supplier;
}

String SupplierConfig::version() const {
	return _version;
}
