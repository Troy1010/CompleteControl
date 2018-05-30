from conans import ConanFile
import xml.etree.ElementTree, os

class TM_HelloWorld_Conan(ConanFile):
    requires="OBSEPluginDevPackage/0.1@Troy1010/channel","TM_CommonCPP/0.1@Troy1010/channel"
    generators="visual_studio"

    sProjPath = "CompleteControl.vcxproj"
    sConanPropsPath = 'conanbuildinfo.props'

    def imports(self):
        #---Hook all build info, including common
        import BuildInfoIntegration as BII
        BII.HookBuildInfo(self.sProjPath,self.sConanPropsPath)
        #---Integrate obse
        #-Get sOBSEPluginDevRoot
        import VisualStudioAutomation as VSA
        import TM_CommonPy as TMC
        vTree = xml.etree.ElementTree.parse(self.sConanPropsPath)
        vElemToFind = xml.etree.ElementTree.Element("Conan-OBSEPluginDevPackage-Root")
        sOBSEPluginDevRoot = TMC.FindElem(vElemToFind,vTree).text
        sRoot = os.path.join(sOBSEPluginDevRoot,"include","obse")
        #-
        cRecommendedIntegrationFiles = ["StdAfx.cpp","StdAfx.h",
            "obse/GameActorValues.cpp","obse/GameActorValues.h","obse/GameAPI.cpp","obse/GameAPI.h",
            "obse/GameBSExtraData.cpp","obse/GameBSExtraData.h","obse/GameData.cpp","obse/GameData.h",
            "obse/GameExtraData.cpp","obse/GameExtraData.h","obse/GameForms.cpp","obse/GameForms.h",
            "obse/GameObjects.cpp","obse/GameObjects.h","obse/GameRTTI_1_1.inl","obse/GameRTTI_1_2.inl",
            "obse/GameRTTI_1_2_416.inl","obse/GameTasks.cpp","obse/GameTasks.h","obse/GameTypes.cpp","obse/GameTypes.h",
            "obse/NiAPI.cpp","obse/NiAPI.h","obse/NiNodes.cpp","obse/NiNodes.h","obse/NiRTTI.cpp","obse/NiRTTI.h",
            "obse/ParamInfos.h","obse/PluginAPI.h","obse/Script.cpp","obse/Script.h","obse/Utilities.cpp","obse/Utilities.h"]
        vDTE = VSA.InstantiateDTE()
        vProj = VSA.OpenProj(vDTE,self.sProjPath)
        for vItem in cRecommendedIntegrationFiles:
            print("vItem:"+vItem)
            print("os.path.join(sRoot,vItem):"+os.path.join(sRoot,vItem))
            VSA.AddFileToProj(vProj,os.path.join(sRoot,vItem),"obse")
        vProj.Save()
        VSA.QuitDTE(vDTE)
