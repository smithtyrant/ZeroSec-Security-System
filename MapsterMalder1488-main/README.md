This software is used to automate the reporting of StarCraft II maps, including melee, arcade, and mod maps. Reporting maps is a valid feature of the game.

*This software is intended to be used to help you remove malicious maps from the StarCraft II arcade, such as those which **violate** the Blizzard TOS and EULA for Custom Games.*

# Overview
### 1.Initialize one or multiple instances of StarCraft II. For multiple instances, use software like Sandboxie (separate install required).
Ten (10) instances are suggested in order to trigger bans. See advanced Sandboxie instructions below.
### 2. Using the instructions below, you will provide a list of map handles for StarCraft II maps.
Example, **battlenet:://starcraft/map/1/208271** the map handle is **208271**
### 3. The software will hook into each of your instances to trigger the built-in map reporting function for each of the handles you provided.
The automated system will then remove the map once the reports have been received.

NOTE: *The map creator can reupload the malicious map, but you may keep this software running automatically to continue sending reports periodically.*

# Compiling and Running
Precompiled binaries are provided, but the full source code and build script is available for those who wish to rebuild everything.

### 1. Extract the folder and run ./build.bat inside.
### 2. Then go to the generated .build folder and open the solution (sln) file in visual studio 2022. Make sure to have the latest cmake and msvc v143 installed.
### 3. Then right-click the solution and build-all.  The built .exe and .dll will be in .build/Release.  Follow the instructions on how to use the exe and dll. 


# Instructions for usage
### 1. Open the 64-bit (the SIXTY-FOUR bit) sc2 client and log in into the game

### 2. Navigate to the directory of the extracted folder with a terminal as ADMIN.

### 3. Launch .\sc2rtwp.exe and it should automatically inject sc2rtwp_in.dll into all open instances of sc2, including sandboxie ones
NOTE: the code is somewhat unstable and has a tendency to cause crashes (you'lll notice if it crashes sc2 in the first few seconds) when it is first injected, and sometimes when you issue the command to mass-report. You can just re-open sc2 until it doesnt crash when this gets injected. (You can ctrl-c to exit the injector program, but keep it open if it's working)

### 4. If you press the NUMLOCK key (the one that also toggles a small led on your keyboard), it will issue the command to start mass-reporting to all connected sessions of sc2.
The injector will read the handles of malicious maps from ./handles.txt in the folder and send all of them to the sc2 clients to mass-report. The clients start sending the packets to the server with the report information.

### 5. Monitoring of Packets
If you want, you can use wireshark on your local ethernet/wifi adapter with this filter ip.src == 192.168.1.1 && ip.dst == 137.221.106.59 (ip.dst is blizzards' ip, don't change. ip.src is your local IP,  you can change it if you configured yours to be different. 

Be aware that general/arcade chat packets and other in-game actions can cause packets to get sent, so go to the arcade section and preferrably leave the arcade/general chat. This way, the number of packets displayed (shown in the bottom-right corner of wireshark) will corresspond to the number of reports sent

NOTE: The first 200 reports (packets) are sent instnatly, but then the server throttles you. You will notice that the count goes up as 20 packets/second (per sc2 client you have open). So if you put 300 handles in handles.txt, it'll be 300*3 = 900 report (packet) per account.  When the number of packets is almost equal, you can move on to reporting the next batch or logging out to go into another account


# Creating accounts
On a residential IP, you can make like 10 accounts. At worst, it'll give you 2-3 easy captchas.

You can use fake/non-existant emails and you dont have to verify them. Just make sure to use @gmail.com or @outlook.com, not other domains

Enter any random date of birth like 2/2/2000. The name/surname can just be gibberish like jsakdnas

If it starts throwing captcha hell at you, or it errors out that you cant make mroe accounts (saying value is invalid or giving an hourglass icon), you can just switch IP.

If you have a dyanmic IP, just restart your modem or turn it off for 30 seconds and on again. Alternatively, you can use a VPN, but datacenter IPs might be limited to 2-5 accounts before they get prevented from making new accounts.

You don't need anything to start mass-reporting in-game. Just log into sc2 and follow the instrucitons for the mass-reporter program.

# Advanced instructions for Sandboxie
For using Sandboxie, you need to set an access exemption for this namedpipe wildcard: `\Device\NamedPipe\sc2rtwp_cmd_*`

"Open for all" and for all programs (or just the sc2 client at least)