##region Settings
bPause = True
sProj = "CompleteControl/CompleteControl.vcxproj"
##endregion
##region Imports
from conans import ConanFile
import xml.etree.ElementTree
import os, sys, ctypes
import VisualStudioAutomation as VS
import TM_CommonPy as TM
##endregion
##region DoubleclickEvent
if __name__ == "__main__":
    try:
        TM.Run("conan install . -pr conanprofile_OBSEPlugin")
        VS.SetTMDefaultSettings(sProj)
        VS.IntegrateProps(sProj,"conanbuildinfo.props")
        ##region Integrate OBSEPluginDevPackage RecommendedIntegrationFiles
        with TM.ElementTreeContext("conanbuildinfo.props") as vTree:
            sOBSEPluginDevPackageRoot = TM.FindElem(xml.etree.ElementTree.Element("Conan-OBSEPluginDevPackage-Root"),vTree).text
        sys.path.insert(0,sOBSEPluginDevPackageRoot)
        import RecommendedIntegration
        RecommendedIntegration.Main(os.path.abspath(sProj),sOBSEPluginDevPackageRoot)
        ##endregion
    except:
        ctypes.windll.user32.MessageBoxW(0, "Exception occured.", __file__, 1)
        raise
    if bPause:
        print("\n\t\t\tDone\n")
        os.system('pause')
##endregion

class CompleteControl_Conan(ConanFile):
    name="CompleteControl"
    requires="OBSEPluginDevPackage/0.1@Troy1010/channel","TM_CommonCPP/0.1@Troy1010/channel"
    generators="visual_studio"
    license = "MIT"
    description = "OBSE plugin to resolve EnableKey conflicts between mods."
