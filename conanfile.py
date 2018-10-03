##region Settings
bPause = True
sProj = "CompleteControl/CompleteControl.vcxproj"
sSln = "CompleteControl.sln"
##endregion
##region Imports
from conans import ConanFile
import os, sys
import TM_CommonPy as TM
import traceback
##endregion
##region DoubleclickEvent
if __name__ == "__main__":
    try:
        TM.Run("conan install . -pr conanprofile_OBSEPlugin")
    except Exception as e:
        print("====================================================================")
        print("Traceback (most recent call last):")
        traceback.print_tb(e.__traceback__)
        print(e)
        os.system('pause')
        sys.exit(1)
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
