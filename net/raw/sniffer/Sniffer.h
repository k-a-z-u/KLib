#ifndef K_NET_RAW_SNIFFER_SNIFFER_H
#define K_NET_RAW_SNIFFER_SNIFFER_H

#include <pcap.h>
#include <string>
#include <iostream>
#include <exception>

#include "SnifferListener.h"

#include "../../../Exception.h"

#define RING_BUFFER_SIZE	(32*1024*1024)
#define CAPTURE_BUFFER_SIZE 4096
#define USE_PROMISCIOUS_MODE	0
#define DBG_SNIFFER

//class Exception : public std::exception {
//private:
//	std::string str;
//public:
//	Exception(const std::string str) : str(str) {;}
//	const char* what() const throw() {return str.c_str();}
//};

namespace K {

	/**
	 * very simple network sniffer based on libPCAP
	 */
	class Sniffer {

		/** the device to capture from */
		std::string dev;

		char errbuf[PCAP_ERRBUF_SIZE] = {0};

		/** libPCAP device handle */
		pcap_t* handle;

		/** the listener to inform */
		SnifferListener* listener;

	public:

		/** ctor */
		Sniffer() {
		}

		/** set the listener to inform */
		void setListener(SnifferListener* l) {
			listener = l;
		}

		/** open a device (e.g. eth0) for sniffing */
		void openDev(const std::string& dev) {

			this->dev = dev;

			// open
			handle = pcap_create(dev.c_str(), errbuf);
			if (handle == nullptr) {throw "error while creating device";}

			// configure some options
			pcap_set_snaplen(handle, CAPTURE_BUFFER_SIZE);
			pcap_set_buffer_size(handle, RING_BUFFER_SIZE);
			pcap_set_promisc(handle, USE_PROMISCIOUS_MODE);
			pcap_set_timeout(handle, 500);

			// open
			int ret = pcap_activate(handle);
			if (ret != 0) {throw Exception("error while opening device");}

		}

		/** open a pcap-ng capture file */
		void openCaptureFile(const std::string& file) {

			this->dev = file;

			handle = pcap_open_offline(file.c_str(), errbuf);
			if (handle == nullptr) {throw "error while opening capture file";}

		}

		void run() {

			const u_char* pktData = 0;
			pcap_pkthdr* header = 0;

			while(true) {
				int res = pcap_next_ex(handle, &header, &pktData);
				if (res > 0) { listener->onPacket(pktData, header->len, header->ts); }
			}

		}

		void setFilter(const std::string& rule) {

			// sanity check
			if (handle == nullptr) {throw Exception("open() the Sniffer before adding filters!");}

			// compile berkeley-packet-filter
			struct bpf_program pgm;
			int ret = pcap_compile(handle, &pgm, rule.c_str(), 1, PCAP_NETMASK_UNKNOWN);
			if (ret < 0) {throw Exception("error while compiling filter '" + rule + "': " + std::string(pcap_geterr(handle)));}
			//debug(DBG_SNIFFER, DBG_LVL_INFO, "filter compiled");

			// apply compiled filter
			ret = pcap_setfilter(handle, &pgm);
			if (ret < 0) {throw Exception("error while activating filter '" + rule + "': " + std::string(pcap_geterr(handle)));}
			//debug(DBG_SNIFFER, DBG_LVL_INFO, "filter activated");

			// cleanup
			pcap_freecode(&pgm);

		}

	};

}

#endif // K_NET_RAW_SNIFFER_SNIFFER_H
