from conans import ConanFile
import os

class DWC(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "scons"

    def requirements(self):
        # TODO: update to 1.5.2 when the bugs are fixed
        self.requires("cpr/1.4.0")
        self.requires("nlohmann_json/3.9.1")
