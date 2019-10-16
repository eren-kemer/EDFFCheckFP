# EDFFCheckFP
 RG Frankfurt FlightPlan-RFL Checker

This plugin for Euroscope will check the Flight Plan for validity.
It will load the SIDs from "Sid.txt" depending on your airport. So if your callsign is "EDDF_TWR", "EDDF_DEL" or anything starting with "EDDF", it will load all SIDs for "EDDF". If there are no SIDs for this airport in the "Sid.txt", it won't load any.
You are welcome to submit the SIDs for your airport! "Instructions" are in the txt itself.

To set up the thing:
- Copy .dll in your Euroscope Plugin folder (if you have one)
- Copy Sid.txt in the same folder as the .dll
- Load the Plugin in Euroscope
- New item in Departure list => "EDFF Check FlightPlan / FlightPlan Check"
- Header name: "FPL", Width: "3", Align to center: "Checked"
- Left button action: "EDFF Check FlightPlan / Check FP" (to get a detailed description, what check the flightplan failed (or passed)

Available commands:
- .checkFP reload (reloads all SIDs depending on your callsign, deletes all loaded
- .checkFP debug (activates Debug Mode, there will be a lot of text output (what is loaded where, how, etc., just interesting if you want to test your SIDs and don't know what's wrong)
- .checkFP load ICAO (instead of ICAO, enter the identifier for the airport you want to load), the SIDs from that airport will be added to the loaded SIDs
- .checkFP check (shows all criteria of the selected aircraft's flight plan, just as "Check FP" does

Known "features":
- sometimes the SIDs don't load, when you log in. Just enter the command .checkFP reload and it will load all your SIDs.

Things to come:
- conditional SIDs, e.g. "Odd for airports with this identifier", "Prop only"
- Failure criteria directly in the list (alternating text or so)
- the possibility to save your own "SID-config", e.g. for CTR-controllers.


If you want to have the latest version, check the "nightly builds" in the "Release" folder. No warranty for a bugless experience!
