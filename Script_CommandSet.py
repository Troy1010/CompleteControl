import sys
import os
import VisualStudioAutomation as VS
import TM_CommonPy as TM
# Settings
bPause = True
sProj = "CompleteControl/CompleteControl.vcxproj"
sSln = "CompleteControl.sln"


def QueActions(vCommandSet):
    # ---TMDefaultSettings
    vCommandSet.Que((VS.SetTMDefaultVSSettings.Do,
                     VS.SetTMDefaultVSSettings.Undo), sProj)
    # ---Integrate Conan-installed packages
    for sRoot in TM.conan.GetDependencyRoots("conanbuildinfo.txt"):
        sPossibleSourceCodeIntegration = os.path.join(
            sRoot, "SourceCodeIntegration_VS.py")
        if os.path.isfile(sPossibleSourceCodeIntegration):
            vCommandSet.QueScript(sPossibleSourceCodeIntegration, [
                                  sRoot, sProj, sSln])
    vCommandSet.Que([VS.IntegrateProps, VS.IntegrateProps_Undo], [
                    sProj, "conanbuildinfo.props"])


try:
    TM.Run("conan install . -pr conanprofile_OBSEPlugin")
    vCommandSet = TM.CommandSet.TryLoad()
    QueActions(vCommandSet)
    print("Executing CommandSet..")
    vCommandSet.Execute()
    vCommandSet.Save()
except Exception as e:
    TM.DisplayException(e)
    sys.exit(1)
if bPause:
    TM.DisplayDone()
