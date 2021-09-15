#!/usr/bin/env python3

import licant 

licant.cxx_application("rfdaemon",
	sources = [
		"main.cpp",
		"App.cpp",
		"AppManager.cpp",
		"TcpServer.cpp",
		"RFDaemonServer.cpp",
		"crc32_ccitt/crc32_ccitt.c"
	],
	include_paths = [ "./" ],
	libs = ["jsoncpp", "pthread", "z"]
)

licant.ex("rfdaemon")
