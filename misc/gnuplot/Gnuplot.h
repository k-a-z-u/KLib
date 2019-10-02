#ifndef GNUPLOT_H
#define GNUPLOT_H

#include "../../os/Process.h"
#include "../../fs/File.h"
#include <fstream>

#include "GnuplotSize.h"
#include "GnuplotDrawable.h"

namespace K {

	class Gnuplot {

	public:

		/** ctor */
		Gnuplot() : debug(false) {
			proc = new K::Process("/usr/bin/gnuplot");
			usleep(1000*25);
			setSize(640,480);
			//flush();
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
		void setSize(const float w, const float h) {
			if (w == this->width && h == this->height) {return;}
			this->width = w;
			this->height = h;
			//(*this) << "set terminal wxt dashed noraise size " << w << "," << h << "\n";
		}

		void close() {

			if (proc) {
				proc->close();
				usleep(1000*100);	// dunno why but some processes need a little time here to close correctly
				proc->kill();
			}
			delete proc;		// will close() and kill() the process;
			proc = nullptr;

		}

		float getWidth() {return width;}

		float getHeight() {return height;}

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
			(*this) << "set output " << '"' << file.getAbsolutePath() << '"' << "\n";
		}

		/** set the file to write to */
		void setOutput(const std::string& file) {
			(*this) << "set output " << '"' << file << '"' << "\n";
		}

		/** send all buffered commands to gnuplot */
		void flush() {

			// write to process
			*proc << getBuffer();
			if (debug) {std::cout << getBuffer() << std::endl;}
			proc->flush();

			// write to file?
			if (commandsFile != "") {
				std::ofstream out(commandsFile);
				out << getBuffer();
				out.close();
			}

			// reset
			buffer.str("");

		}

		/** attach the given drawablw to the draw buffer */
		void draw(const GnuplotDrawable& d) {
			d.addTo(buffer);
		}

		/** get the current buffer content as string */
		std::string getBuffer() const {
			return buffer.str();
		}

		/** set a filename to write the flush() data to [gnuplot commands] */
		void writePlotToFile(const std::string& fileName) {
			this->commandsFile = fileName;
		}

		/** write current data to the given file. now! */
		void writePlotToFileNow(const std::string& fileName) {
			std::ofstream out(fileName);
			out << getBuffer();
			out.close();
		}

	private:

		/** io stream to gnuplot */
		K::Process* proc;

		/** the buffer to use */
		std::stringstream buffer;

		float width;
		float height;

		/** perform debug output? */
		bool debug;

		/** write gnuplot commands to file? */
		std::string commandsFile;

	};

}

#endif // GNUPLOT_H
