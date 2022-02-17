#!/usr/bin/env python3

import licant 

licant.cxx_application("rfdaemon",
	sources = [
		"src/*.cpp",
	],
	include_paths = [ "./src" ],
	libs = ["jsoncpp", "pthread", "z"]
)

licant.ex("rfdaemon")
