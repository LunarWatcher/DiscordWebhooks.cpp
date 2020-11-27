from conans import ConanFile
import os

class DWC(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "scons"

    def requirements(self):
        self.requires("cpr/1.5.0")
        self.requires("nlohmann_json/3.9.1")
