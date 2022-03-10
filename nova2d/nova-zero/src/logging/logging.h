#pragma once
#include <iostream>
#include "../graphics/AnsiColor.h"
#include "../core/Defines.h"

namespace novazero
{
	namespace logging
	{

#define LVL_FATAL_ERROR "Fatal Error"
#define LVL_FE "Fatal Error"
#define LVL_NON_FATAL_ERROR "Non-Fatal Error"
#define LVL_NFE "Non-Fatal Error"
#define LVL_WARNING "Warning"
#define LVL_W "Warning"
#define LVL_INFO "Info"
#define LVL_I "Info"
#define LVL_CONFIRMATION "Confirmation"
#define LVL_C "Confirmation"

#define LOG(lvl, text) novazero::logging::Logging::LogOut(lvl, text);
#define LOGS(text) novazero::logging::Logging::LogSimple(text);

		class Logging
		{
		public:

			static void LogSimple(const std::string& text) 
			{
				std::cout << text << std::endl;
			}

			template<typename T>
			static void LogSimple(const T text)
			{
				std::cout << text << std::endl;
			}

			static void LogOut(const std::string& level, const std::string& text)
			{
				std::string out = "";
				
				if (level == LVL_CONFIRMATION)
				{
					out = "\x1b[92m";
				}
				else if (level == LVL_FATAL_ERROR)
				{
					out = "\x1b[91m";
				}
				else if (level == LVL_INFO)
				{
					out = "\x1b[96m";
				}
				else if (level == LVL_NON_FATAL_ERROR)
				{
					out = "\x1b[95m";
				}
				else if (level == LVL_WARNING)
				{
					out = "\x1b[93m";
				}

				out += level + ": " + text + "\n";

				setupConsole();
				printf(out.c_str());
				restoreConsole();
			}
		};

	}
}