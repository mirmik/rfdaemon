#!/usr/bin/env python3

import licant 
import licant.install 

licant.cxx_application("rfdaemon",
	sources = [
		"src/*.cpp",
	],
	include_paths = [ "./src" ],
	libs = ["jsoncpp", "pthread", "z", "nos", "igris"],
	cxx_flags = "-pedantic-errors -Werror=all -Werror=extra -g",
)

licant.install.install_application(tgt="install", src="rfdaemon", dst="rfdaemon")

licant.ex("rfdaemon")
