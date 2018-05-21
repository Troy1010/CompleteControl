from conans import ConanFile

class TM_HelloWorld_Conan(ConanFile):
    requires="OBSEPluginDevPackage/0.1@Troy1010/channel","TM_CommonCPP/0.1@Troy1010/channel"
    generators="visual_studio"

    import HookBuildInfo as HBI
    HBI.HookBuildInfo('CompleteControl.vcxproj','conanbuildinfo.props')
