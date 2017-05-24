#ifndef GNUPLOTSTRINGMOD_H
#define GNUPLOTSTRINGMOD_H

#include <string>

namespace K {

	class GnuplotStringMod {

	public:

		virtual std::string modLabel(const std::string& src) const = 0;
		virtual std::string modTitle(const std::string& src) const = 0;
		virtual std::string modTics(const std::string& src) const = 0;
		virtual std::string modEntryTitle(const std::string& src) const = 0;

	protected:

		static inline void replaceAll(std::string& str, const std::string& from, const std::string& to) {
			if(from.empty()) {return;}
			size_t start_pos = 0;
			while((start_pos = str.find(from, start_pos)) != std::string::npos) {
				str.replace(start_pos, from.length(), to);
				start_pos += to.length();
			}
		}

	};

	class GnuplotStringModNone : public GnuplotStringMod {

	public:

		virtual std::string modLabel(const std::string& src) const override {return src;}
		virtual std::string modTitle(const std::string& src) const override {return src;}
		virtual std::string modTics(const std::string& src) const override {return src;}
		virtual std::string modEntryTitle(const std::string& src) const override {return src;}

	};

	class GnuplotStringModLaTeX : public GnuplotStringMod {

	public:

		virtual std::string modLabel(const std::string& src) const override {return fix(toTex(src));}
		virtual std::string modTitle(const std::string& src) const override {return fix(toTex(src));}
		virtual std::string modTics(const std::string& src) const override {return fixAlt(toTex(src));}
		virtual std::string modEntryTitle(const std::string& src) const override {return fix(toTex(src));}

	private:

		std::string toTex(const std::string& src) const {
			std::string res = src;
			res = "\\footnotesize{" + src + "}";
			return res;
		}

		std::string fix(const std::string& src) const {
			std::string res = src;
			replaceAll(res, "%", "\\%");	// convert % to \%
			return res;
		}

		std::string fixAlt(const std::string& src) const {
			std::string res = src;
			replaceAll(res, "%%", "\\%%");	// convert %% to \%%	for tics-label format
			return res;
		}

	};

}

#endif // GNUPLOTSTRINGMOD_H
