
#ifndef STDEXT_DEMANGLE_H
#define STDEXT_DEMANGLE_H

#include <typeinfo>
#include <vector>
#include <string>

namespace stdext {

	class demangled_name {
		public:
			const std::string& last() const;
			std::string full() const;
			bool empty() const { return m_parts.empty(); }

			static demangled_name createFromString(const char* name);

			template<typename T>
			static demangled_name get() {
				return createFromString(typeid(T).name());
			}

			bool operator==(const demangled_name& n) const { return n.full() == full(); }

		private:
			std::vector<std::string> m_parts;
	};

}

#endif
