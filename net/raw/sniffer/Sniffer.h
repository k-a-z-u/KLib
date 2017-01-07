#ifndef K_NET_RAW_SNIFFER_SNIFFER_H
#define K_NET_RAW_SNIFFER_SNIFFER_H

#include <pcap.h>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

#include "../../../Exception.h"
#include "../../../Assertions.h"

#include "SnifferListener.h"

#define RING_BUFFER_SIZE	(2*1024*1024)
#define CAPTURE_BUFFER_SIZE (64*1024)
#define USE_PROMISCIOUS_MODE	0
#define DBG_SNIFFER

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

		/** used for stopping the background thread */
		bool running = false;

		/** background thread */
		std::thread thread;

		/** list of all attached listeners */
		std::vector<SnifferListener*> listeners;

	public:

		/** ctor */
		Sniffer() {
		}

		/** attach a new listener to the sniffer */
		void addListener(SnifferListener* l) {
			listeners.push_back(l);
		}

		/** detach the given listener from the sniffer */
		void removeListener(SnifferListener* l) {
			auto elem = std::find(listeners.begin(), listeners.end(), l);
			listeners.erase(elem);
		}

		/** open a device (e.g. eth0) for sniffing */
		void openDev(const std::string& dev, const int timeout_ms = 500) {

			this->dev = dev;

			// open
			handle = pcap_create(dev.c_str(), errbuf);
			if (handle == nullptr) {throw Exception("error while creating device");}

			// configure some options
			//pcap_set_snaplen(handle, CAPTURE_BUFFER_SIZE);
			pcap_set_buffer_size(handle, RING_BUFFER_SIZE);
			pcap_set_promisc(handle, USE_PROMISCIOUS_MODE);
			pcap_set_timeout(handle, timeout_ms);

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

		/** start the sniffer */
		void start() {

			_assertFalse(running, "must not call start() twice");

			// start the background thread
			running = true;
			thread = std::thread(&Sniffer::loop, this);

		}

		/** wait for the sniffer to terminate after stop was called */
		void join() {

			// wait for the thread to finish
			thread.join();

		}

		/** stop the sniffer */
		void stop() {

			_assertTrue(running, "must not call stop() without start()");
			running = false;

		}

		/** set additional filter-rules */
		void setFilter(const std::string& rule) {

			_assertFalse(running, "set filter rules before calling start()");

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

	private:

		void loop() {

			const u_char* pktData = 0;
			pcap_pkthdr* header = 0;

			while(running) {
				int res = pcap_next_ex(handle, &header, &pktData);
				if (res > 0) {
					for (SnifferListener* l : listeners) {
						const uint64_t ms = (header->ts.tv_sec * 1000) + (header->ts.tv_usec / 1000);
						const Timestamp ts = Timestamp::fromMS(ms);
						//std::cout << "###" << ts.ms() << std::endl;
						l->onPacket(pktData, header->len, ts);
					}
				}
			}

		}

	};

}

#endif // K_NET_RAW_SNIFFER_SNIFFER_H
