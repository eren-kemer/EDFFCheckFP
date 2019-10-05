#include "stdafx.h"
#include "analyzeFP.hpp"

using namespace std;
using namespace EuroScopePlugIn;

	// Run on Plugin Initialization
CcheckFPPlugin::CcheckFPPlugin(void) :CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE, MY_PLUGIN_NAME, MY_PLUGIN_VERSION, MY_PLUGIN_DEVELOPER, MY_PLUGIN_COPYRIGHT)
{
		// Private Message giving more information on Plugin, Version number etc. 
	DisplayUserMessage(MY_PLUGIN_NAME, MY_PLUGIN_NAME , PLUGIN_WELCOME_MESSAGE " - Version: " PLUGIN_VERSION, true, true, true, true, true);

		// Register Tag Item "FlightPlan Check"
	RegisterTagItemType("EDFF FlighPlan Check", TAG_ITEM_FPCHECK);

}

	// Run on Plugin destruction, Ie. Closing EuroScope or unloading plugin
CcheckFPPlugin::~CcheckFPPlugin()
{
}


/*
	Custom Functions
*/

void CcheckFPPlugin::OnGetTagItem(CFlightPlan FlightPlan, CRadarTarget RadarTarget, int ItemCode, int TagData, char sItemString[16], int* pColorCode, COLORREF* pRGB, double* pFontSize)
{
	if ( ItemCode == TAG_ITEM_FPCHECK )
	{
		string FlightPlanString = FlightPlan.GetFlightPlanData().GetRoute();
		int RFL = FlightPlan.GetFlightPlanData().GetFinalAltitude();

		*pColorCode = TAG_COLOR_RGB_DEFINED;

		if ( FlightPlanString.find("OBOKA") != string::npos )
		{
			if ((RFL / 1000) % 2 == 0)
			{
				//Even
				*pRGB = RGB(0, 190, 0);
				strcpy_s(sItemString, 16, "OK!");
			}
			else
			{
				//Odd
				*pRGB = RGB(190, 0, 0);
				strcpy_s(sItemString, 16, "FPL");
			}
		}
		else
		if (FlightPlanString.find("MARUN") != string::npos)
		{
			if ((RFL/1000) % 2 == 0)
			{
				//Even
				*pRGB = RGB(0, 190, 0);
				strcpy_s(sItemString, 16, "OK!");
			}
			else
			{
				//Odd
				*pRGB = RGB(190, 0, 0);
				strcpy_s(sItemString, 16, "FPL");
			}
		}
		else
		if (FlightPlanString.find("TOBAK") != string::npos)
		{
			if ((RFL/1000) % 2 == 0)
			{
				//Even
				*pRGB = RGB(190, 0, 0);
				strcpy_s(sItemString, 16, "FPL");
			}
			else
			{
				//Odd
				*pRGB = RGB(0, 190, 0);
				strcpy_s(sItemString, 16, "OK!");
			}
		}
		else
		if (FlightPlanString.find("SOBRA") != string::npos)
		{
			if ((RFL/1000) % 2 == 0 && (RFL/1000) > 250)
			{
				//Even
				*pRGB = RGB(0, 190, 0);
				strcpy_s(sItemString, 16, "OK!");
			}
			else
			{
				//Odd
				*pRGB = RGB(190, 0, 0);
				strcpy_s(sItemString, 16, "FPL");
			}
		}
		else
		if (FlightPlanString.find("ULKIG") != string::npos)
		{
			if ((RFL/1000) % 2 == 0 && (RFL/1000) > 250)
			{
				//Even
				*pRGB = RGB(0, 190, 0);
				strcpy_s(sItemString, 16, "OK!");
			}
			else
			{
				//Odd
				*pRGB = RGB(190, 0, 0);
				strcpy_s(sItemString, 16, "FPL");
			}
		}
		else
		if (FlightPlanString.find("ANEKI") != string::npos)
		{
			if ((RFL/1000) % 2 == 0)
			{
				//Even
				*pRGB = RGB(190, 0, 0);
				strcpy_s(sItemString, 16, "FPL");
			}
			else
			{
				//Odd
				*pRGB = RGB(0, 190, 0);
				strcpy_s(sItemString, 16, "OK!");
			}
		}
		else
		if (FlightPlanString.find("CINDY") != string::npos)
		{
			if ((RFL/1000) % 2 == 0)
			{
				//Even
				*pRGB = RGB(190, 0, 0);
				strcpy_s(sItemString, 16, "FPL");
			}
			else
			{
				//Odd
				*pRGB = RGB(0, 190, 0);
				strcpy_s(sItemString, 16, "OK!");
			}
		}
		else
		if (FlightPlanString.find("KOMIB") != string::npos)
		{
			if ((RFL/1000) % 2 == 0)
			{
				//Even
				*pRGB = RGB(190, 0, 0);
				strcpy_s(sItemString, 16, "FPL");
			}
			else
			{
				//Odd
				*pRGB = RGB(0, 190, 0);
				strcpy_s(sItemString, 16, "OK!");
			}
		}
		else
		if (FlightPlanString.find("SULUS") != string::npos)
		{
			if ((RFL/1000) % 2 == 0)
			{
				//Even
				*pRGB = RGB(190, 0, 0);
				strcpy_s(sItemString, 16, "FPL");
			}
			else
			{
				//Odd
				*pRGB = RGB(0, 190, 0);
				strcpy_s(sItemString, 16, "OK!");
			}
		}

	}
}




// Get FlightPlan, and therefore get the first waypoint of the flightplan (ie. SID). Check if the (RFL/1000) corresponds to the SID Min FL and report output "OK" or "FPL"