#ifndef GNUPLOT_H
#define GNUPLOT_H

#include "../../os/Process.h"
#include "../../fs/File.h"
#include <fstream>
#include <chrono>
#include <thread>

#include "GnuplotDrawable.h"
#include "GnuplotDrawable3D.h"
#include "GnuplotMultiplot.h"
#include "GnuplotPlot.h"
#include "GnuplotPlotElement.h"
#include "GnuplotPlotElementColorLines.h"
#include "GnuplotPlotElementColorPoints.h"
#include "GnuplotPlotElementEmpty.h"
#include "GnuplotPlotElementHistogram.h"
#include "GnuplotPlotElementImage.h"
#include "GnuplotPlotElementLines.h"
#include "GnuplotPlotElementPoints.h"
#include "GnuplotPlotElementRaw.h"
#include "GnuplotScene.h"
#include "GnuplotSize.h"
#include "GnuplotSplot.h"
#include "GnuplotSplotElement.h"
#include "GnuplotSplotElementColorLines.h"
#include "GnuplotSplotElementColorPoints.h"
#include "GnuplotSplotElementEmpty.h"
#include "GnuplotSplotElementHeatMap.h"
#include "GnuplotSplotElementLines.h"
#include "GnuplotSplotElementMesh.h"
#include "GnuplotSplotElementPM3D.h"
#include "GnuplotSplotElementPoints.h"
#include "GnuplotSplotElementRaw.h"
#include "GnuplotStructs.h"

#include "attributes/GnuplotAttrColor.h"
#include "attributes/GnuplotAttrCustom.h"
#include "attributes/GnuplotAttrStroke.h"
#include "attributes/GnuplotAttrTitle.h"

#include "misc/GnuplotAxis.h"
#include "misc/GnuplotColor.h"
#include "misc/GnuplotCoordinate.h"
#include "misc/GnuplotDashtype.h"
#include "misc/GnuplotFill.h"
#include "misc/GnuplotFillstyle.h"
#include "misc/GnuplotKey.h"
#include "misc/GnuplotMargin.h"
#include "misc/GnuplotStringMod.h"
#include "misc/GnuplotStroke.h"
#include "misc/GnuplotView.h"

#include "objects/GnuplotObject.h"
#include "objects/GnuplotObjectArrow.h"
#include "objects/GnuplotObjectCircle.h"
#include "objects/GnuplotObjectPolygon.h"
#include "objects/GnuplotObjectRectangle.h"
#include "objects/GnuplotObjects.h"


namespace K {

	class Gnuplot {

	public:

		/** ctor */
		Gnuplot() : Gnuplot("/usr/bin/gnuplot") {

		}

		Gnuplot(const std::string& gnuPlotPath) : debug(false) {
			proc = new K::Process(gnuPlotPath);
			std::this_thread::sleep_for(std::chrono::milliseconds(25));
			setSize(640, 480);
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
				std::this_thread::sleep_for(std::chrono::milliseconds(100));	// dunno why but some processes need a little time here to close correctly
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
