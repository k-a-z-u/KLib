#ifndef GNUPLOT_H
#define GNUPLOT_H

#include "../../os/Process.h"
#include "../../fs/File.h"

#include "GnuplotSize.h"
#include "GnuplotDrawable.h"

namespace K {

	class Gnuplot {

	public:

		/** ctor */
		Gnuplot() : debug(false) {
			proc = new K::Process("/usr/bin/gnuplot", "");
			setSize(640,480);
			flush();
		}

		/** dtor */
		~Gnuplot() {
			close();
		}

		template <typename T>
		friend Gnuplot& operator << (Gnuplot& gp, const T& cmd) {
			gp.buffer << cmd;
			return gp;
		}

		/** set whether to output all commands as debug to terminal */
		void setDebugOutput(const bool debug) {
			this->debug = debug;
		}

		/** resize the terminal */
		void setSize(const int w, const int h) {
			if (w == this->width && h == this->height) {return;}
			this->width = w;
			this->height = h;
			//(*this) << "set terminal wxt dashed noraise size " << w << "," << h << "\n";
		}

		void close() {
			delete proc;
			proc = nullptr;
		}

		int getWidth() {return width;}

		int getHeight() {return height;}

		/** configure the output terminal */
		void setTerminal(const std::string& terminal, GnuplotSize size) {
			std::string attrs = "";
			if (terminal == "eps")		{attrs = "color dashed";}
			if (terminal == "svg")		{attrs = "dashed";}
			if (terminal == "epslatex")	{attrs = "color dashed";}
			//if (terminal == "pngcairo")	{attrs = "transparent";}
			this->width = size.getWidth("wxt");
			this->height = size.getHeight("wxt");
			(*this) << "set terminal " << terminal << " " << attrs << " size " << size.getWidth(terminal) << "," << size.getHeight(terminal) << "\n";
		}

		/** unset the output (no file output) */
		void setOutput() {
			(*this) << "unset output\n";
		}

		/** set the file to write to */
		void setOutput(const K::File& file) {
			(*this) << "set output '" << file.getAbsolutePath() << "'\n";
		}

		/** send all buffered commands to gnuplot */
		void flush() {
			//buffer << std::endl;
			*proc << buffer.str();
			if (debug) {std::cout << buffer.str() << std::endl;}
			proc->flush();
			buffer.str("");
		}

		/** attach the given drawablw to the draw buffer */
		void draw(const GnuplotDrawable& d) {
			d.addTo(buffer);
		}

	private:

		/** io stream to gnuplot */
		K::Process* proc;

		/** the buffer to use */
		std::stringstream buffer;

		int width;
		int height;

		/** perform debug output? */
		bool debug;

	};

}

#endif // GNUPLOT_H
