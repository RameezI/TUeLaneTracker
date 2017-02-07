##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=InitStateLDT
ConfigurationName      :=Debug
WorkspacePath          := "/home/rameez/TUeLaneTracker"
ProjectPath            := "/home/rameez/TUeLaneTracker/InitState"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=rameez
Date                   :=06/02/17
CodeLitePath           :="/home/rameez/.codelite"
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="InitStateLDT.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g $(Preprocessors)
CFLAGS   :=  -g $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/createTemplate.cpp$(ObjectSuffix) $(IntermediateDirectory)/isrow.cpp$(ObjectSuffix) $(IntermediateDirectory)/morphopAlgo.cpp$(ObjectSuffix) $(IntermediateDirectory)/round.cpp$(ObjectSuffix) $(IntermediateDirectory)/rt_nonfinite.cpp$(ObjectSuffix) $(IntermediateDirectory)/rtGetInf.cpp$(ObjectSuffix) $(IntermediateDirectory)/rtGetNaN.cpp$(ObjectSuffix) $(IntermediateDirectory)/run_Init_State.cpp$(ObjectSuffix) $(IntermediateDirectory)/run_Init_State_data.cpp$(ObjectSuffix) $(IntermediateDirectory)/run_Init_State_emxAPI.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/run_Init_State_emxutil.cpp$(ObjectSuffix) $(IntermediateDirectory)/run_Init_State_initialize.cpp$(ObjectSuffix) $(IntermediateDirectory)/run_Init_State_rtwutil.cpp$(ObjectSuffix) $(IntermediateDirectory)/run_Init_State_terminate.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) $(Objects) $(ArLibs)
	@$(MakeDirCommand) "/home/rameez/TUeLaneTracker/.build-debug"
	@echo rebuilt > "/home/rameez/TUeLaneTracker/.build-debug/InitStateLDT"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/createTemplate.cpp$(ObjectSuffix): createTemplate.cpp $(IntermediateDirectory)/createTemplate.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/InitState/createTemplate.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/createTemplate.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/createTemplate.cpp$(DependSuffix): createTemplate.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/createTemplate.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/createTemplate.cpp$(DependSuffix) -MM "createTemplate.cpp"

$(IntermediateDirectory)/createTemplate.cpp$(PreprocessSuffix): createTemplate.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/createTemplate.cpp$(PreprocessSuffix) "createTemplate.cpp"

$(IntermediateDirectory)/isrow.cpp$(ObjectSuffix): isrow.cpp $(IntermediateDirectory)/isrow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/InitState/isrow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/isrow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/isrow.cpp$(DependSuffix): isrow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/isrow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/isrow.cpp$(DependSuffix) -MM "isrow.cpp"

$(IntermediateDirectory)/isrow.cpp$(PreprocessSuffix): isrow.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/isrow.cpp$(PreprocessSuffix) "isrow.cpp"

$(IntermediateDirectory)/morphopAlgo.cpp$(ObjectSuffix): morphopAlgo.cpp $(IntermediateDirectory)/morphopAlgo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/InitState/morphopAlgo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/morphopAlgo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/morphopAlgo.cpp$(DependSuffix): morphopAlgo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/morphopAlgo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/morphopAlgo.cpp$(DependSuffix) -MM "morphopAlgo.cpp"

$(IntermediateDirectory)/morphopAlgo.cpp$(PreprocessSuffix): morphopAlgo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/morphopAlgo.cpp$(PreprocessSuffix) "morphopAlgo.cpp"

$(IntermediateDirectory)/round.cpp$(ObjectSuffix): round.cpp $(IntermediateDirectory)/round.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/InitState/round.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/round.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/round.cpp$(DependSuffix): round.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/round.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/round.cpp$(DependSuffix) -MM "round.cpp"

$(IntermediateDirectory)/round.cpp$(PreprocessSuffix): round.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/round.cpp$(PreprocessSuffix) "round.cpp"

$(IntermediateDirectory)/rt_nonfinite.cpp$(ObjectSuffix): rt_nonfinite.cpp $(IntermediateDirectory)/rt_nonfinite.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/InitState/rt_nonfinite.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rt_nonfinite.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rt_nonfinite.cpp$(DependSuffix): rt_nonfinite.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rt_nonfinite.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rt_nonfinite.cpp$(DependSuffix) -MM "rt_nonfinite.cpp"

$(IntermediateDirectory)/rt_nonfinite.cpp$(PreprocessSuffix): rt_nonfinite.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rt_nonfinite.cpp$(PreprocessSuffix) "rt_nonfinite.cpp"

$(IntermediateDirectory)/rtGetInf.cpp$(ObjectSuffix): rtGetInf.cpp $(IntermediateDirectory)/rtGetInf.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/InitState/rtGetInf.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rtGetInf.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rtGetInf.cpp$(DependSuffix): rtGetInf.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rtGetInf.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rtGetInf.cpp$(DependSuffix) -MM "rtGetInf.cpp"

$(IntermediateDirectory)/rtGetInf.cpp$(PreprocessSuffix): rtGetInf.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rtGetInf.cpp$(PreprocessSuffix) "rtGetInf.cpp"

$(IntermediateDirectory)/rtGetNaN.cpp$(ObjectSuffix): rtGetNaN.cpp $(IntermediateDirectory)/rtGetNaN.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/InitState/rtGetNaN.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/rtGetNaN.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/rtGetNaN.cpp$(DependSuffix): rtGetNaN.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/rtGetNaN.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/rtGetNaN.cpp$(DependSuffix) -MM "rtGetNaN.cpp"

$(IntermediateDirectory)/rtGetNaN.cpp$(PreprocessSuffix): rtGetNaN.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/rtGetNaN.cpp$(PreprocessSuffix) "rtGetNaN.cpp"

$(IntermediateDirectory)/run_Init_State.cpp$(ObjectSuffix): run_Init_State.cpp $(IntermediateDirectory)/run_Init_State.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/InitState/run_Init_State.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/run_Init_State.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/run_Init_State.cpp$(DependSuffix): run_Init_State.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/run_Init_State.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/run_Init_State.cpp$(DependSuffix) -MM "run_Init_State.cpp"

$(IntermediateDirectory)/run_Init_State.cpp$(PreprocessSuffix): run_Init_State.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/run_Init_State.cpp$(PreprocessSuffix) "run_Init_State.cpp"

$(IntermediateDirectory)/run_Init_State_data.cpp$(ObjectSuffix): run_Init_State_data.cpp $(IntermediateDirectory)/run_Init_State_data.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/InitState/run_Init_State_data.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/run_Init_State_data.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/run_Init_State_data.cpp$(DependSuffix): run_Init_State_data.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/run_Init_State_data.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/run_Init_State_data.cpp$(DependSuffix) -MM "run_Init_State_data.cpp"

$(IntermediateDirectory)/run_Init_State_data.cpp$(PreprocessSuffix): run_Init_State_data.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/run_Init_State_data.cpp$(PreprocessSuffix) "run_Init_State_data.cpp"

$(IntermediateDirectory)/run_Init_State_emxAPI.cpp$(ObjectSuffix): run_Init_State_emxAPI.cpp $(IntermediateDirectory)/run_Init_State_emxAPI.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/InitState/run_Init_State_emxAPI.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/run_Init_State_emxAPI.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/run_Init_State_emxAPI.cpp$(DependSuffix): run_Init_State_emxAPI.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/run_Init_State_emxAPI.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/run_Init_State_emxAPI.cpp$(DependSuffix) -MM "run_Init_State_emxAPI.cpp"

$(IntermediateDirectory)/run_Init_State_emxAPI.cpp$(PreprocessSuffix): run_Init_State_emxAPI.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/run_Init_State_emxAPI.cpp$(PreprocessSuffix) "run_Init_State_emxAPI.cpp"

$(IntermediateDirectory)/run_Init_State_emxutil.cpp$(ObjectSuffix): run_Init_State_emxutil.cpp $(IntermediateDirectory)/run_Init_State_emxutil.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/InitState/run_Init_State_emxutil.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/run_Init_State_emxutil.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/run_Init_State_emxutil.cpp$(DependSuffix): run_Init_State_emxutil.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/run_Init_State_emxutil.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/run_Init_State_emxutil.cpp$(DependSuffix) -MM "run_Init_State_emxutil.cpp"

$(IntermediateDirectory)/run_Init_State_emxutil.cpp$(PreprocessSuffix): run_Init_State_emxutil.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/run_Init_State_emxutil.cpp$(PreprocessSuffix) "run_Init_State_emxutil.cpp"

$(IntermediateDirectory)/run_Init_State_initialize.cpp$(ObjectSuffix): run_Init_State_initialize.cpp $(IntermediateDirectory)/run_Init_State_initialize.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/InitState/run_Init_State_initialize.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/run_Init_State_initialize.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/run_Init_State_initialize.cpp$(DependSuffix): run_Init_State_initialize.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/run_Init_State_initialize.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/run_Init_State_initialize.cpp$(DependSuffix) -MM "run_Init_State_initialize.cpp"

$(IntermediateDirectory)/run_Init_State_initialize.cpp$(PreprocessSuffix): run_Init_State_initialize.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/run_Init_State_initialize.cpp$(PreprocessSuffix) "run_Init_State_initialize.cpp"

$(IntermediateDirectory)/run_Init_State_rtwutil.cpp$(ObjectSuffix): run_Init_State_rtwutil.cpp $(IntermediateDirectory)/run_Init_State_rtwutil.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/InitState/run_Init_State_rtwutil.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/run_Init_State_rtwutil.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/run_Init_State_rtwutil.cpp$(DependSuffix): run_Init_State_rtwutil.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/run_Init_State_rtwutil.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/run_Init_State_rtwutil.cpp$(DependSuffix) -MM "run_Init_State_rtwutil.cpp"

$(IntermediateDirectory)/run_Init_State_rtwutil.cpp$(PreprocessSuffix): run_Init_State_rtwutil.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/run_Init_State_rtwutil.cpp$(PreprocessSuffix) "run_Init_State_rtwutil.cpp"

$(IntermediateDirectory)/run_Init_State_terminate.cpp$(ObjectSuffix): run_Init_State_terminate.cpp $(IntermediateDirectory)/run_Init_State_terminate.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/InitState/run_Init_State_terminate.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/run_Init_State_terminate.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/run_Init_State_terminate.cpp$(DependSuffix): run_Init_State_terminate.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/run_Init_State_terminate.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/run_Init_State_terminate.cpp$(DependSuffix) -MM "run_Init_State_terminate.cpp"

$(IntermediateDirectory)/run_Init_State_terminate.cpp$(PreprocessSuffix): run_Init_State_terminate.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/run_Init_State_terminate.cpp$(PreprocessSuffix) "run_Init_State_terminate.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


