/*
 * base.h
 *
 *  Created on: 8 lis 2017
 *      Author: pregusia
 */

#ifndef INCLUDE_APPLICATION_BASE_H_
#define INCLUDE_APPLICATION_BASE_H_

#include <base.h>

namespace io {
	class FileDescriptorsPoller;
}

namespace application {

	namespace config {
		namespace parser {
			class ConfigEntry;
			class ConfigEntryValue;
			class ConfigEntriesCollection;
		}
	}

	class ProcessToSpawn;
	typedef stdext::object_ptr<ProcessToSpawn> ProcessToSpawnPtr;

	class StreamProxy;
	typedef stdext::object_ptr<StreamProxy> StreamProxyPtr;


	class ProcessDeathAction {
		public:
			typedef enum: int32_t {
				INVALID = 0,

				NONE = 1,
				RERUN = 2,
				STOP = 3,
			} Enum;

			static Enum fromString(const std::string& val);

		private:
			ProcessDeathAction() { }
	};

}





#endif /* INCLUDE_APPLICATION_BASE_H_ */
