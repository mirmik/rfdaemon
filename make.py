#!/usr/bin/env python3

import os
import licant 
import licant.install 
import subprocess

def ircc(tgt, src):
    def self_need():
        with subprocess.Popen(f"ircc {src} -o {tgt} --is-rebuild-needed", shell=True, stdout=subprocess.PIPE) as proc:
            ans = str(proc.communicate()[0].decode("utf-8")).strip() 
            if ans == "yes":
                return True
            else:
                return False

    src = os.path.expanduser(src)
    tgt = os.path.expanduser(tgt)
    licant.source(src)
    licant.core.core.add(
        licant.make.FileTarget(
            tgt=tgt,
            build=licant.make.Executor("ircc {src} -o {tgt}"),
            src=src,
            deps=[src],
            message="IRCC {src} {tgt}",
            self_need=self_need
        )
    )
    return tgt

ircc("./build/ircc_resource.gen.cpp", "resources.txt")

licant.cxx_application("rfdaemon",
	sources = [
		"src/*.cpp",
		"build/ircc_resource.gen.cpp"
	],
	include_paths = [ "./src" ],
	libs = ["pthread", "nos", "igris"],
	cxx_flags = "-pedantic-errors -Werror=all -Werror=extra -g",
)

licant.install.install_application(tgt="install", src="rfdaemon", dst="rfdaemon")

licant.ex("rfdaemon")
