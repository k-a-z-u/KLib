#ifndef K_NET_RTMP_RTMP_H
#define K_NET_RTMP_RTMP_H

#include <vector>
#include <unordered_map>

#include "RTMPPacket.h"
#include "RTMPStream.h"
#include "RTMPListener.h"

#include "amf/AMF.h"

namespace K {

	/**
	 * a very simple RTMP parser.
	 * add data chunks and the parser will fire the corresponding events.
	 *
	 * RTMP is a protocol with control data and payload.
	 * to reduce the overhead, information are only added, when they
	 * have changed compared to previous commands.
	 * We thus have to keep track of the old state to get e.g. the current
	 * payload size as this information will be ommitted when it has not changed
	 * since the last packet.
	 *
	 */
	class RTMPParser {

		static constexpr const char* K_DBG_RTMP_PARSER = "RTMP_PAR";

	private:

		bool gotStart = false;

		/** all currently running streams */
		std::unordered_map<int, RTMPStream> streams;

		/** current data buffer */
		std::vector<uint8_t> data;

		/** the listener to inform */
		RTMPListener* listener;

		/** temporal data buffer */
		uint8_t* tmp;


	public:

		/** ctor */
		RTMPParser() : listener(nullptr) {
			tmp = new uint8_t[64*1024];
		}

		/** set the listener to inform */
		void setListener(RTMPListener* l) {
			this->listener = l;
		}


		/** get all contained streams and their state */
		const std::unordered_map<int, RTMPStream>& getStreams() {
			return streams;
		}

		/** append and parse the given payload */
		void append(K::Payload p) {

			// sanity checks
			if (p.length == 0) {return;}
			assert(listener != nullptr);

			// sync in the middle of a stream
			if (!gotStart) {
				if (p.length < 1440 && p.length > 8) {
					debug(K_DBG_RTMP_PARSER, "got a fix! (within a running stream)");
					gotStart = true;
				}
				return;
			}

			// append data
			const uint32_t curLen = data.size();
			data.resize(curLen + p.length);
			memcpy(data.data()+curLen, p.data, p.length);

			// try to get the next (full!!) packet from the buffer and get its corresponding stream
			RTMPStream* stream;

			// there might be more than one packet within the buffer! fetch all!
			while (true) {

				const int len = tryToGetNextPacketFromBuffer(tmp, &stream);

				// got a packet? -> parse it!
				if (len) {
					stream->totalDataBytes += len;
					parse(*stream, tmp, len);
				} else {
					break;
				}

			};

		}

	private:

		/** get the stream details for the given streamID. or create them if not yet present */
		RTMPStream* getOrAddStream(const uint32_t streamID, const uint32_t typeID) {
			if (streams.find(streamID) == streams.end()) {
				debug(K_DBG_RTMP_PARSER, "adding a new stream with streamID: " << streamID);
				streams[streamID] = RTMPStream(streamID);
				listener->onRTMPNewStream(streamID, typeID);
			}
			return &streams[streamID];
		}

		/** try to copy the next RTMP Packet from the buffer into DST */
		int tryToGetNextPacketFromBuffer(uint8_t* dst, RTMPStream** stream) {

			// parse
			RTMPPacket pkt(data.data(), data.size());

			// get previous stats for this stream
			*stream = getOrAddStream(pkt.getStreamID(), pkt.getTypeID());
			if (*stream == nullptr) {return 0;}

			// update the stream with the current values
			(*stream)->update(pkt);

			// complete(!!) packet present?
			const uint32_t totalLen = pkt.getHeaderLength() + (*stream)->lastPayloadSize;

			// could not find a clear specification on this one...
			// this is probaly a hack as i do not understand / can't find the protocol's specification
			//if ((*stream)->lastTypeID == 0x14) {totalLen = (*stream)->getChunkedPayloadSize(totalLen);}

			if ((*stream)->lastPayloadSize >= 0) {
				if (data.size() >= totalLen) {
					memcpy(dst, data.data(), totalLen);
					data.erase(data.begin(), data.begin() + totalLen);
					return totalLen;
				} else {
					debug(K_DBG_RTMP_PARSER, "waiting for buffer to fill");
				}
			} else {
				reset();
			}

			// nothing
			return 0;

		}

		/** parse the given data packet */
		void parse(RTMPStream& stream, const uint8_t* data, const uint32_t len) {

			// temporal packet
			RTMPPacket pkt(data, len);

			// debug output
			debug(K_DBG_RTMP_PARSER, pkt);
			debug(K_DBG_RTMP_PARSER, stream);

			// which type does the current stream contain
			switch (stream.lastTypeID) {
				case 0x01:										break;
				case 0x04:	parseControl(stream, pkt, len);		break;
				case 0x08:	parseAudio(stream, pkt, len);		break;
                //case 0x11:	parseAMF3(stream, pkt, len);		break;	// AMF3
                case 0x14:	parseAMF0(stream, pkt, len);		break;	// AMF0
				default:	break;
			}

		}

		void parseControl(RTMPStream& stream, RTMPPacket& pkt, const uint32_t len) {
			const uint8_t* data = pkt.getPayload();
			const uint32_t ctrlType = (data[0] << 8) | (data[1] << 0);
			switch(ctrlType) {
				case 0x0000: listener->onRTMPStreamStart(pkt.getStreamID()); resetStream(stream.lastMessageID); break;
				case 0x0001: listener->onRTMPStreamEnd(pkt.getStreamID()); break;
			}
		}

		void parseAudio(RTMPStream& stream, RTMPPacket& pkt, const uint32_t len) {
			const uint8_t* data = pkt.getPayload();
			const int32_t dataLen = stream.lastPayloadSize;
			if (dataLen > 0) {
				const uint8_t ctrl = data[0];
				listener->onRTMPData(stream.streamID, stream.lastTypeID, ctrl, data+1, dataLen-1);
			}
		}

		void parseAMF0(RTMPStream& stream, RTMPPacket& pkt, const uint32_t len) {
			const uint8_t* data = pkt.getPayload();
			//const int32_t dataLen = stream.getChunkedPayloadSize(stream.lastPayloadSize); // this is porbabily a hack.. cant find/understand the protocol specification on this one
			//const std::vector<uint8_t> unchunked = unchunk(stream, data, dataLen);
			//K::AMFResult res = K::AMF::parse(unchunked.data(), unchunked.size());
			const int32_t dataLen = stream.lastPayloadSize;

			bool errors;
			K::AMF amf(data, dataLen);
			K::AMFResult res = amf.parseErroneousData(errors);
			std::cout << res << std::endl;
			listener->onRTMPAMF(std::move(res));

			// error occurred.. try to reset the stream
			if (errors) {reset();}

		}




		/** reset the stream with the given ID */
		void resetStream(const uint32_t streamID) {
			debug(K_DBG_RTMP_PARSER, "reset stream: " << streamID);
			if (streamID == 0) {
	//			for (auto it : streams) {
	//				RTMPStream& stream = it.second;
	//				stream.reset();
	//			}
				streams.clear();
			} else {
				if (streams.find(streamID) != streams.end()) {
					streams[streamID].reset();
				}
			}
		}

		/** reset the whole parser */
		void reset() {
			debug(K_DBG_RTMP_PARSER, "resetting...");
			streams.clear();
			gotStart = false;
			data.resize(0);
			listener->onRTMPReset();
		}


		// could not find a clear specification on this one...
//		/**
//		 * RTMP chunks certain data types by adding an additonal byte every 128 bytes.
//		 * this method removes those bytes
//		 */
//		static std::vector<uint8_t> unchunk(const RTMPStream& stream, const uint8_t* data, uint32_t len) {

//			// the chunk size, depends on the streams current settings
//			static int chunkSize = stream.curChunkSize;

//			// copy all chunks
//			std::vector<uint8_t> res(0);
//			for (int i = 0; i < len; i+=chunkSize+1) {

//				// destination for this chunk within the output (just append)
//				const int dst = res.size();

//				// how many bytes to copy. usually: chunkSize, at the end: what-is-left
//				const int size = ((len - i) < chunkSize) ? (len - i) : (chunkSize);

//				// resize and copy
//				res.resize(res.size()+size);		// inefficient!
//				memcpy(&res[dst], &data[i], size);

//				// sanity check. ensure there actually is the correct chunking-byte after each chunk
//				if (size == chunkSize) {assert(data[i+chunkSize] == stream.getChunkDivider());}

//			}
//			return res;
//		}


	};

}

#endif // RTMP_H
