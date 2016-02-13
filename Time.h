#ifndef SIGMA_TIME
#define SIGMA_TIME

namespace sig
{

	class Time
	{
		static Time* ms_instance;

	public:
		static Time* Instance();
		static void Release();

		/**
		 * @brief Get current time
		 * @return Time in seconds
		 */
		double GetTime();
		
	private:
		Time();
		~Time();
		
	};

}

#endif // SIGMA_TIME
