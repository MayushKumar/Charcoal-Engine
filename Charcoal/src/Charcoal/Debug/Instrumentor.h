#pragma once

#include <fstream>
#include <chrono>
#include <thread>

namespace Charcoal
{

	struct ProfileResult
	{
		const char* Name;
		long long Start, End;
		long long ThreadID;
	};

	class Instrumentor
	{
	public:

		void BeginSession(const char* name, const char* filePath)
		{
			m_ProfileCount = 0;
			m_FileStream.open(filePath);
			m_FileStream << "[";
		}

		void EndSession()
		{
			m_FileStream << "]";
			m_FileStream.close();
		}

		void WriteProfile(ProfileResult& profile)
		{
			if (m_ProfileCount++ > 0)
				m_FileStream << ",";
			m_FileStream << "{";
			m_FileStream << "\"name\": \"" << profile.Name << "\",";
			m_FileStream << "\"cat\": \"category\",";
			m_FileStream << "\"ph\": \"X\",";
			m_FileStream << "\"ts\": \"" << profile.Start << "\",";
			m_FileStream << "\"dur\": \"" << profile.End - profile.Start << "\",";
			m_FileStream << "\"pid\": \"0\",";
			m_FileStream << "\"tid\": \"" << profile.ThreadID << "\"";
			m_FileStream << "}";
			m_FileStream.flush();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		uint32_t m_ProfileCount;
		std::ofstream m_FileStream;
	};

	class InstrumentorTimer
	{
	public:
		InstrumentorTimer(const char* name)
		{
			m_ProfileResult.Name = name;
			m_ProfileResult.ThreadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			m_ProfileResult.Start = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
		}

		~InstrumentorTimer()
		{
			if (!m_Stopped)
				Stop();
			Instrumentor::Get().WriteProfile(m_ProfileResult);
		}

		void Stop()
		{
			m_ProfileResult.End = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
			m_Stopped = true;
		}

	private:
		ProfileResult m_ProfileResult;
		bool m_Stopped = false;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
	};

}

#define CH_PROFILE

#ifdef CH_PROFILE
	#define CH_PROFILE_BEGIN_SESSION(name , filePath) ::Charcoal::Instrumentor::Get().BeginSession(name, filePath)
	#define CH_PROFILE_END_SESSION() ::Charcoal::Instrumentor::Get().EndSession()
	#define CH_PROFILE_SCOPE(name) ::Charcoal::InstrumentorTimer timer##__LINE__(name);
	#define CH_PROFILE_FUNCTION() CH_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define CH_PROFILE_BEGIN_SESSION(name , filePath)
	#define CH_PROFILE_END_SESSION()
	#define CH_PROFILE_SCOPE(name)
	#define CH_PROFILE_FUNCTION()
#endif