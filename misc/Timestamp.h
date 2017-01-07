#ifndef K_TIMESTAMP_H
#define K_TIMESTAMP_H

#include <chrono>

namespace K {

	/**
	 * helper-class to handle timestamps
	 */
	struct Timestamp {

	private:

		/** internal timestamp in milliseconds */
		int64_t _ms;

		/** HIDDEN ctor. use factory methods */
		explicit Timestamp(const int64_t ms) : _ms(ms) {;}

	public:

		/** empty ctor */
		explicit Timestamp() : _ms(0) {;}

		/** get timestamp from the given value which represents milliesconds */
		static inline Timestamp fromMS(const int64_t ms)	{return Timestamp(ms);}

		/** get timestamp from the given value which represents seconds */
		static inline Timestamp fromSec(const float sec)	{return Timestamp(sec*1000);}

		/** get timestamp for the current unix-time */
		static inline Timestamp fromUnixTime() {
			auto now = std::chrono::system_clock::now();
			auto duration = now.time_since_epoch();
			auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
			return Timestamp(millis);
		}

		/** get timestamp for the current system-time */
		static inline Timestamp fromRunningTime() {
			static Timestamp startup = fromUnixTime();
			return fromUnixTime() - startup;
		}



	public:

		/** get timestamp in milliseconds */
		inline int64_t ms() const {return _ms;}

		/** get timestamp in seconds */
		inline float sec() const {return _ms/1000.0f;}



	public:

		/** is this timestamp zero? */
		bool isZero() const {return _ms == 0;}

		/** equal? */
		bool operator == (const Timestamp& o) const {return _ms == o._ms;}

		/** not equal? */
		bool operator != (const Timestamp& o) const {return _ms != o._ms;}

		/** smaller than the given one? */
		bool operator < (const Timestamp& o) const {return _ms < o._ms;}
		bool operator <= (const Timestamp& o) const {return _ms <= o._ms;}

		/** greater than the given one? */
		bool operator > (const Timestamp& o) const {return _ms > o._ms;}
		bool operator >= (const Timestamp& o) const {return _ms >= o._ms;}



		Timestamp operator - (const Timestamp& o) const {return Timestamp(_ms - o._ms);}

		Timestamp operator + (const Timestamp& o) const {return Timestamp(_ms + o._ms);}

		Timestamp operator * (const float val) const {return Timestamp(_ms * val);}

	};

}

#endif // K_TIMESTAMP_H
