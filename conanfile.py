from conans import ConanFile


class CompleteControl_Conan(ConanFile):
    name = "CompleteControl"
    requires = "OBSEPluginDevPackage/0.1@Troy1010/channel", "TM_CommonCPP/0.1@Troy1010/beta"
    generators = "visual_studio"
    license = "MIT"
    description = "OBSE plugin to resolve EnableKey conflicts between mods."
