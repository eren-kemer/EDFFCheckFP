#pragma once
#include "EuroScopePlugIn.h"
#include <iostream>
#include <string>
#include "Constant.hpp"
#include <fstream>
#include <vector>

#define MY_PLUGIN_NAME      "EDFF Check FlightPlan"
#define MY_PLUGIN_VERSION   "1.0"
#define MY_PLUGIN_DEVELOPER "Nikolas Goerlitz"
#define MY_PLUGIN_COPYRIGHT "GPL v4"
#define MY_PLUGIN_VIEW_AVISO  "Check FlightPlan Plugin"

#define PLUGIN_WELCOME_MESSAGE	"Willkommen beim RG-Frankfurt Flugplan-RFL checker"
#define PLUGIN_VERSION			"1.0"

using namespace std;
using namespace EuroScopePlugIn;

class CcheckFPPlugin :
	public EuroScopePlugIn::CPlugIn
{
public:
	CcheckFPPlugin();
	virtual ~CcheckFPPlugin();

	virtual void getSids();

	//Define OnGetTagItem function
	virtual void OnGetTagItem(CFlightPlan FlightPlan,
		CRadarTarget RadarTarget,
		int ItemCode,
		int TagData,
		char sItemString[16],
		int* pColorCode,
		COLORREF* pRGB,
		double* pFontSize);

	virtual bool OnCompileCommand(const char * sCommandLine);

	virtual void debugMessage(string type, string message);

	virtual void sendMessage(string type, string message);

	virtual void checkFPDetail();

	virtual void OnTimer(int Count);
};

