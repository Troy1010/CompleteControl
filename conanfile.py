##region Settings
bPause = True
sProj = "CompleteControl/CompleteControl.vcxproj"
##endregion
##region Imports
from conans import ConanFile
import xml.etree.ElementTree
import os, sys, ctypes
import TM_CommonPy as TM
import VisualStudioAutomation as VS
import pickle
import dill
##endregion
##region DoubleclickEvent
if __name__ == "__main__":
    try:
        TM.Run("conan install . -pr conanprofile_OBSEPlugin")
        vCommandSet = TM.CommandSet.TryLoad()
        vCommandSet.Que((VS.SetTMDefaultVSSettings.Do,VS.SetTMDefaultVSSettings.Undo),sProj)
        vCommandSet.Que([VS.IntegrateProps,VS.IntegrateProps_Undo],[sProj,"conanbuildinfo.props"])
        vCommandSet.QueConanPackageRecommendedIntegrations(sProj,"conanbuildinfo.txt")
        vCommandSet.Execute()
        vCommandSet.Save()
    except Exception as e:
        if bPause:
            print(e)
            os.system('pause')
        else:
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
