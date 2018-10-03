##region Settings
bPause = True
sProj = "CompleteControl/CompleteControl.vcxproj"
sSln = "CompleteControl.sln"
##endregion
##region Imports
import os, sys
import TM_CommonPy as TM
import VisualStudioAutomation as VS
import traceback
##endregion

def QueActions(vCommandSet):
    #---TMDefaultSettings
    vCommandSet.Que((VS.SetTMDefaultVSSettings.Do,VS.SetTMDefaultVSSettings.Undo),sProj)
    #---Integrate Conan-installed packages
    vCommandSet.Que([VS.IntegrateProps,VS.IntegrateProps_Undo],[sProj,"conanbuildinfo.props"])
    for sRoot in TM.GetDependencyRoots("conanbuildinfo.txt"):
        sPossibleRecommendedIntegrationPath = os.path.join(sRoot,"RecommendedIntegration.py")
        if os.path.isfile(sPossibleRecommendedIntegrationPath):
            vCommandSet.QueScript(sPossibleRecommendedIntegrationPath,[sRoot,sProj,sSln])

##region DoubleclickEvent
if __name__ == "__main__":
    try:
        TM.Run("conan install . -pr conanprofile_OBSEPlugin")
        vCommandSet = TM.CommandSet.TryLoad()
        QueActions(vCommandSet)
        print("Executing CommandSet..")
        vCommandSet.Execute()
        vCommandSet.Save()
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
