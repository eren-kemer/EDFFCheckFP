#include "stdafx.h"
#include "analyzeFP.hpp"

extern "C" IMAGE_DOS_HEADER __ImageBase;

bool debugMode, initialSidLoad;

ifstream sidDatei;
char DllPathFile[_MAX_PATH];
string pfad;

vector<string> sidName;
vector<string> sidEven;
vector<int> sidMin;
vector<int> sidMax;

using namespace std;
using namespace EuroScopePlugIn;

	// Run on Plugin Initialization
CcheckFPPlugin::CcheckFPPlugin(void) :CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE, MY_PLUGIN_NAME, MY_PLUGIN_VERSION, MY_PLUGIN_DEVELOPER, MY_PLUGIN_COPYRIGHT)
{
		// Private Message giving more information on Plugin, Version number etc. 
	//DisplayUserMessage(MY_PLUGIN_NAME, MY_PLUGIN_NAME , PLUGIN_WELCOME_MESSAGE " - Version: " MY_PLUGIN_VERSION, true, true, true, true, true);

		// Register Tag Item "FlightPlan Check"
	RegisterTagItemType("FlightPlan Check", TAG_ITEM_FPCHECK);
	RegisterTagItemFunction("Check FP", TAG_FUNC_CHECKFP_MENU);

	// Get Path of the Sid.txt
	GetModuleFileNameA(HINSTANCE(&__ImageBase), DllPathFile, sizeof(DllPathFile));
	pfad = DllPathFile;
	pfad.resize(pfad.size() - strlen("EDFFCheckFP.dll"));
	pfad += "Sid.txt";

	debugMode = false;
	initialSidLoad = false;
}

// Run on Plugin destruction, Ie. Closing EuroScope or unloading plugin
CcheckFPPlugin::~CcheckFPPlugin()
{
}


/*
	Custom Functions
*/

void CcheckFPPlugin::debugMessage(string type, string message) {
	// Display Debug Message if debugMode = true
	if (debugMode) {
		DisplayUserMessage("checkFP", type.c_str(), message.c_str(), true, true, true, false, false);
	}
}

void CcheckFPPlugin::sendMessage(string type, string message) {
	// Show a message
	DisplayUserMessage("checkFP", type.c_str(), message.c_str(), true, true, true, true, false);
}

void CcheckFPPlugin::sendMessage(string message) {
	DisplayUserMessage("Message", "checkFP", message.c_str(), true, true, true, false, false);
}

// Get data from sid.txt
void CcheckFPPlugin::getSids() {
	sidDatei.open(pfad, ios::in);
	if (!sidDatei) {	// Display Error, if something bad happened.
		string error{ pfad };
		error += "couldn't be opened!";
		sendMessage("Error", error.c_str());
	}

	sendMessage("Loading Sids...");
	string read, readbuffer;
	string sid[4];			// 0 = Name, 1 = Even/Odd, 2 = Min FL, 3 = Max FL
	string airport{ ControllerMyself().GetCallsign() };
	airport.resize(4);	// Take e.g. "EDDF_TWR", or "EDDF_N_APP" and make it to "EDDF"
	if (airport.find_first_of('_O') == 3) {	// Test if logged in as observer, defaulting to EDDF
		airport = "EDDF";	// Standard this to EDDF for testing purposes
	}
	sendMessage("Loading", airport);
	// To-Do: override, if there is a different callsign, e.g. EDGG_E_CTR => getSids(string airportOverride) => airport = airportOverride => function from console, maybe with safing setting

	while (getline(sidDatei, read)) {	// Read the line! Love isn't always on time!
		readbuffer = read;
		readbuffer.resize(4);
		debugMessage("Reading", read);
		debugMessage("Readbuffer", readbuffer);
		if (readbuffer == airport) {
			sid[0] = read;  // sid[0] = "EDDF;OBOKA;E;0;0";
			sid[0].erase(0, sid[0].find_first_of(';') + 1);   //sid[0] = "OBOKA;E;0;0";
			debugMessage("Sid[0]", sid[0]);
			sid[1] = sid[0];
			sid[0].erase(sid[0].find_first_of(';'), sid[0].length());   //sid[0] = "OBOKA";
			sid[1].erase(0, sid[1].find_first_of(';') + 1);	//sid[1] = E;0;0";
			debugMessage("Sid[1]", sid[1]);
			sid[2] = sid[1];
			sid[1].erase(sid[1].find_first_of(';'), sid[1].length());
			sid[2].erase(0, sid[2].find_first_of(';') + 1);
			debugMessage("Sid[2]", sid[2]);
			sid[3] = sid[2];
			sid[2].erase(sid[2].find_first_of(';'), sid[2].length());
			sid[3].erase(0, sid[3].find_first_of(';') + 1);
			debugMessage("Sid[3]", sid[3]);
			//sid[3].erase(sid[3].find_first_of(';'), sid[3].length());

			// Adding all the data to the vectors
			sidName.push_back(sid[0]);
			sidEven.push_back(sid[1]);
			sidMin.push_back(stoi(sid[2]));
			sidMax.push_back(stoi(sid[3]));

			// Showing what is added to the sid-vectors
			if (debugMode) {
				string sidContainer{ sid[0] };
				sidContainer += sid[1];
				sidContainer += sid[2];
				sidContainer += sid[3];

				debugMessage("Adding", sidContainer);
			}
		}
	}

	sidDatei.close();
	string sidCount{ "SID loading finished: " };
	sidCount += to_string(sidName.size());
	sidCount += " SIDs loaded.";
	sendMessage(sidCount.c_str());

	// Output of all vectors with their data in it
	if (debugMode) {
		string names{};
		for (string buf : sidName)
		{
			names += buf;
			names += ";";
		}
		string even{};
		for (string buf : sidEven)
		{
			even += buf;
			even += ";";
		}
		string min{};
		for (int buf : sidMin)
		{
			min += to_string(buf);
			min += ";";
		}
		string max{};
		for (int buf : sidMax)
		{
			max += to_string(buf);
			max += ";";
		}
		debugMessage("Sid Name", names);
		debugMessage("Sid Even", even);
		debugMessage("Sid Min", min);
		debugMessage("Sid Max", max);
	}
}

void CcheckFPPlugin::OnFunctionCall(int FunctionId, const char * ItemString, POINT Pt, RECT Area) {
	if (FunctionId == TAG_FUNC_CHECKFP_MENU) {
		OpenPopupList(Area, "Check FP", 1);
		AddPopupListElement("Show Checks", "", TAG_FUNC_CHECKFP_CHECK, false, 2, false);
	}
	if (FunctionId == TAG_FUNC_CHECKFP_CHECK) {
		checkFPDetail();
	}
}

// Get FlightPlan, and therefore get the first waypoint of the flightplan (ie. SID). Check if the (RFL/1000) corresponds to the SID Min FL and report output "OK" or "FPL"
void CcheckFPPlugin::OnGetTagItem(CFlightPlan FlightPlan, CRadarTarget RadarTarget, int ItemCode, int TagData, char sItemString[16], int* pColorCode, COLORREF* pRGB, double* pFontSize)
{
	if (ItemCode == TAG_ITEM_FPCHECK)
	{
		string FlightPlanString = FlightPlan.GetFlightPlanData().GetRoute();
		int RFL = FlightPlan.GetFlightPlanData().GetFinalAltitude();

		*pColorCode = TAG_COLOR_RGB_DEFINED;

		for (int i = 0; i < sidName.size(); i++) {
			bool passed{ false };
			if (FlightPlanString.find(sidName.at(i)) != string::npos) {
				if (((RFL / 1000) % 2 == 0) && (sidEven.at(i) == "Even")) {
					passed = true;
				} 
				else if (((RFL / 1000) % 2 != 0) && (sidEven.at(i) == "Odd")) {
					passed = true;
				}
				if (sidMin.at(i) != 0) {
					if ((RFL / 100) >= sidMin.at(i)) {
						passed = true;
					} else {
						passed = false;
					}
				}
				if (sidMax.at(i) != 0) {
					if ((RFL / 100) <= sidMax.at(i)) {
						passed = true;
					}
					else {
						passed = false;
					}
				}
			}
			if (passed) {
				*pRGB = TAG_GREEN;
				strcpy_s(sItemString, 16, "OK!");
				break;
			}
			else {
				*pRGB = TAG_RED;
				strcpy_s(sItemString, 16, "FPL");
			}
		}

	}
}

bool CcheckFPPlugin::OnCompileCommand(const char * sCommandLine) {
	if (startsWith(".checkFP reload", sCommandLine))
	{
		sidName.clear();
		sidEven.clear();
		sidMin.clear();
		sidMax.clear();
		initialSidLoad = false;
		return true;
	}
	if (startsWith(".checkFP debug", sCommandLine)) {
		if (debugMode) {
			debugMessage("DebugMode", "Deactivating Debug Mode!");
			debugMode = false;
		} else {
			debugMode = true;
			debugMessage("DebugMode", "Activating Debug Mode!");
		}
		return true;
	}
	if (startsWith(".checkFP load", sCommandLine)) {
		// ToDo: Destructing the command and parse the Airport to the getSid-Function
		return true;
	}
	if (startsWith(".checkFP check", sCommandLine))
	{
		checkFPDetail();
		return true;
	}
	return false;
}

// Sends to you, which checks were failed and which were passed on the selected aircraft
void CcheckFPPlugin::checkFPDetail() {
	CFlightPlan flightPlan = FlightPlanSelectASEL();
	string FlightPlanString = flightPlan.GetFlightPlanData().GetRoute();
	int RFL = flightPlan.GetFlightPlanData().GetFinalAltitude();
	sendMessage(flightPlan.GetCallsign(), "Checking");
	bool valid{ false };
	for (int i = 0; i < sidName.size(); i++) {
		bool passed{ false };
		if (FlightPlanString.find(sidName.at(i)) != string::npos) {
			valid = true;
			string sidFound{ "Found: " };
			sidFound += sidName.at(i);
			sendMessage(flightPlan.GetCallsign(), sidFound);
			if (((RFL / 1000) % 2 == 0) && (sidEven.at(i) == "Even")) {
				sendMessage(flightPlan.GetCallsign(), "Passed Even");
				passed = true;
			}
			else if (((RFL / 1000) % 2 != 0) && (sidEven.at(i) == "Odd")) {
				sendMessage(flightPlan.GetCallsign(), "Passed Odd");
				passed = true;
			}
			if (sidMin.at(i) != 0) {
				if ((RFL / 100) >= sidMin.at(i)) {
					sendMessage(flightPlan.GetCallsign(), "Passed Min");
					passed = true;
				}
				else {
					sendMessage(flightPlan.GetCallsign(), "Failed Min");
					passed = false;
				}
			}
			if (sidMax.at(i) != 0) {
				if ((RFL / 100) <= sidMax.at(i)) {
					sendMessage(flightPlan.GetCallsign(), "Passed Max");
					passed = true;
				}
				else {
					sendMessage(flightPlan.GetCallsign(), "Failed Max");
					passed = false;
				}
			}
			break;
		}
	}
	if (!valid) {
		sendMessage(flightPlan.GetCallsign(), "No valid SID found");
	}
}

void CcheckFPPlugin::OnTimer(int Counter) {
	// Loading proper Sids, when logged in
	if (GetConnectionType() != CONNECTION_TYPE_NO && !initialSidLoad) {
		string callsign{ ControllerMyself().GetCallsign() };
		if (callsign.find_first_of('_O') != 3) {
			getSids();		
		} else {
			sendMessage("Observer Mode, no SIDs loaded");
		}
		initialSidLoad = true;
	} else if (GetConnectionType() == CONNECTION_TYPE_NO && initialSidLoad) {
		sidName.clear();
		sidEven.clear();
		sidMin.clear();
		sidMax.clear();
		initialSidLoad = false;
		sendMessage("Unloading", "All loaded SIDs");
	}
}