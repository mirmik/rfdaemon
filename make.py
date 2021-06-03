#!/usr/bin/env python3

import licant 

licant.cxx_application("rfdaemon",
	sources = [
		"main.cpp",
		"App.cpp",
		"AppManager.cpp",
		"TcpServer.cpp",
		"RFDaemonServer.cpp"
	],
	libs = ["jsoncpp", "igris", "pthread", "z"]
)

licant.ex("rfdaemon")
