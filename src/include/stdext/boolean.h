
#ifndef BOOLEAN_H
#define BOOLEAN_H

namespace stdext {

	template<bool def>
	struct boolean {
		public:
			boolean() : v(def) { }
			operator bool &() { return v; }
			operator bool const &() const { return v; }
			bool& operator=(const bool& o) { v = o; return v; }

		private:
			bool v;
	};

}

#endif
