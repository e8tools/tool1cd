#ifndef CTOOL1CD_APPH
#define CTOOL1CD_APPH

#include <memory>

#include "Class_1CD.h"
#include "ParseCommandLine.h"
#include "String.hpp"
#include "Messenger.h"

class App {
public:
		App(char **szArglist, int nArgs, Messenger &mess);
		~App();

		int Run();

		App(const App &app) = delete;

private:
		std::unique_ptr<T_1CD> base1CD;
		CommandParse comm;
		Messenger& mess;

		bool ActionOpenBaseNotMonopolyChecked{ false };
		bool ActionXMLSaveBLOBToFileChecked{ false };
		bool ActionXMLUnpackBLOBChecked{ true };

		bool IsTrueString(const String &str) const;
		void export_all_to_xml(const ParsedCommand& pc);
		void export_to_xml(const ParsedCommand& pc);

		void export_to_binary(const ParsedCommand& pc);
		void import_from_binary(const ParsedCommand& pc);

		void save_config(const ParsedCommand& pc);
		void save_configsave(const ParsedCommand& pc);
		void save_vendors_configs(const ParsedCommand& pc);
		void save_all_configs(const ParsedCommand& pc);

		void save_depot_config(const ParsedCommand& pc);
		void save_depot_config_part(const ParsedCommand& pc);

		void find_and_save_lost_objects(const ParsedCommand& pc);

};

#endif
