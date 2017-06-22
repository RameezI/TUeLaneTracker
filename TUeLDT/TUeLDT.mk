##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=TUeLDT
ConfigurationName      :=Debug
WorkspacePath          := "/home/rameez/TUeLaneTracker"
ProjectPath            := "/home/rameez/TUeLaneTracker/TUeLDT"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=rameez
Date                   :=22/06/17
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
ObjectsFileList        :="TUeLDT.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../include $(IncludeSwitch)./include $(IncludeSwitch)../3rdPartyLib/eigen3 
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
CXXFLAGS :=  -g -std=c++11 $(Preprocessors)
CFLAGS   :=  -g $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/State.cpp$(ObjectSuffix) $(IntermediateDirectory)/VanishingPtFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/InitState.cpp$(ObjectSuffix) $(IntermediateDirectory)/SigInit.cpp$(ObjectSuffix) $(IntermediateDirectory)/BufferingState.cpp$(ObjectSuffix) $(IntermediateDirectory)/StateMachine.cpp$(ObjectSuffix) $(IntermediateDirectory)/LDT_logger.cpp$(ObjectSuffix) $(IntermediateDirectory)/LDT_profiler.cpp$(ObjectSuffix) $(IntermediateDirectory)/LaneFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/TrackingLaneState.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/BufferingDAG_generic.cpp$(ObjectSuffix) $(IntermediateDirectory)/TrackingLanesDAG_generic.cpp$(ObjectSuffix) 



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
	@echo rebuilt > "/home/rameez/TUeLaneTracker/.build-debug/TUeLDT"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/State.cpp$(ObjectSuffix): State.cpp $(IntermediateDirectory)/State.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/State.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/State.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/State.cpp$(DependSuffix): State.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/State.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/State.cpp$(DependSuffix) -MM "State.cpp"

$(IntermediateDirectory)/State.cpp$(PreprocessSuffix): State.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/State.cpp$(PreprocessSuffix) "State.cpp"

$(IntermediateDirectory)/VanishingPtFilter.cpp$(ObjectSuffix): VanishingPtFilter.cpp $(IntermediateDirectory)/VanishingPtFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/VanishingPtFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/VanishingPtFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/VanishingPtFilter.cpp$(DependSuffix): VanishingPtFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/VanishingPtFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/VanishingPtFilter.cpp$(DependSuffix) -MM "VanishingPtFilter.cpp"

$(IntermediateDirectory)/VanishingPtFilter.cpp$(PreprocessSuffix): VanishingPtFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/VanishingPtFilter.cpp$(PreprocessSuffix) "VanishingPtFilter.cpp"

$(IntermediateDirectory)/InitState.cpp$(ObjectSuffix): InitState.cpp $(IntermediateDirectory)/InitState.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/InitState.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InitState.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InitState.cpp$(DependSuffix): InitState.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InitState.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InitState.cpp$(DependSuffix) -MM "InitState.cpp"

$(IntermediateDirectory)/InitState.cpp$(PreprocessSuffix): InitState.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InitState.cpp$(PreprocessSuffix) "InitState.cpp"

$(IntermediateDirectory)/SigInit.cpp$(ObjectSuffix): SigInit.cpp $(IntermediateDirectory)/SigInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/SigInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SigInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SigInit.cpp$(DependSuffix): SigInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SigInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SigInit.cpp$(DependSuffix) -MM "SigInit.cpp"

$(IntermediateDirectory)/SigInit.cpp$(PreprocessSuffix): SigInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SigInit.cpp$(PreprocessSuffix) "SigInit.cpp"

$(IntermediateDirectory)/BufferingState.cpp$(ObjectSuffix): BufferingState.cpp $(IntermediateDirectory)/BufferingState.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/BufferingState.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BufferingState.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BufferingState.cpp$(DependSuffix): BufferingState.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BufferingState.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BufferingState.cpp$(DependSuffix) -MM "BufferingState.cpp"

$(IntermediateDirectory)/BufferingState.cpp$(PreprocessSuffix): BufferingState.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BufferingState.cpp$(PreprocessSuffix) "BufferingState.cpp"

$(IntermediateDirectory)/StateMachine.cpp$(ObjectSuffix): StateMachine.cpp $(IntermediateDirectory)/StateMachine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/StateMachine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/StateMachine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/StateMachine.cpp$(DependSuffix): StateMachine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/StateMachine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/StateMachine.cpp$(DependSuffix) -MM "StateMachine.cpp"

$(IntermediateDirectory)/StateMachine.cpp$(PreprocessSuffix): StateMachine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/StateMachine.cpp$(PreprocessSuffix) "StateMachine.cpp"

$(IntermediateDirectory)/LDT_logger.cpp$(ObjectSuffix): LDT_logger.cpp $(IntermediateDirectory)/LDT_logger.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/LDT_logger.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LDT_logger.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LDT_logger.cpp$(DependSuffix): LDT_logger.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LDT_logger.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LDT_logger.cpp$(DependSuffix) -MM "LDT_logger.cpp"

$(IntermediateDirectory)/LDT_logger.cpp$(PreprocessSuffix): LDT_logger.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LDT_logger.cpp$(PreprocessSuffix) "LDT_logger.cpp"

$(IntermediateDirectory)/LDT_profiler.cpp$(ObjectSuffix): LDT_profiler.cpp $(IntermediateDirectory)/LDT_profiler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/LDT_profiler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LDT_profiler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LDT_profiler.cpp$(DependSuffix): LDT_profiler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LDT_profiler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LDT_profiler.cpp$(DependSuffix) -MM "LDT_profiler.cpp"

$(IntermediateDirectory)/LDT_profiler.cpp$(PreprocessSuffix): LDT_profiler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LDT_profiler.cpp$(PreprocessSuffix) "LDT_profiler.cpp"

$(IntermediateDirectory)/LaneFilter.cpp$(ObjectSuffix): LaneFilter.cpp $(IntermediateDirectory)/LaneFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/LaneFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LaneFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LaneFilter.cpp$(DependSuffix): LaneFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LaneFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LaneFilter.cpp$(DependSuffix) -MM "LaneFilter.cpp"

$(IntermediateDirectory)/LaneFilter.cpp$(PreprocessSuffix): LaneFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LaneFilter.cpp$(PreprocessSuffix) "LaneFilter.cpp"

$(IntermediateDirectory)/TrackingLaneState.cpp$(ObjectSuffix): TrackingLaneState.cpp $(IntermediateDirectory)/TrackingLaneState.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/TrackingLaneState.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TrackingLaneState.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TrackingLaneState.cpp$(DependSuffix): TrackingLaneState.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TrackingLaneState.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TrackingLaneState.cpp$(DependSuffix) -MM "TrackingLaneState.cpp"

$(IntermediateDirectory)/TrackingLaneState.cpp$(PreprocessSuffix): TrackingLaneState.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TrackingLaneState.cpp$(PreprocessSuffix) "TrackingLaneState.cpp"

$(IntermediateDirectory)/BufferingDAG_generic.cpp$(ObjectSuffix): BufferingDAG_generic.cpp $(IntermediateDirectory)/BufferingDAG_generic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/BufferingDAG_generic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BufferingDAG_generic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BufferingDAG_generic.cpp$(DependSuffix): BufferingDAG_generic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BufferingDAG_generic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BufferingDAG_generic.cpp$(DependSuffix) -MM "BufferingDAG_generic.cpp"

$(IntermediateDirectory)/BufferingDAG_generic.cpp$(PreprocessSuffix): BufferingDAG_generic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BufferingDAG_generic.cpp$(PreprocessSuffix) "BufferingDAG_generic.cpp"

$(IntermediateDirectory)/TrackingLanesDAG_generic.cpp$(ObjectSuffix): TrackingLanesDAG_generic.cpp $(IntermediateDirectory)/TrackingLanesDAG_generic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/TrackingLanesDAG_generic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TrackingLanesDAG_generic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TrackingLanesDAG_generic.cpp$(DependSuffix): TrackingLanesDAG_generic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TrackingLanesDAG_generic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TrackingLanesDAG_generic.cpp$(DependSuffix) -MM "TrackingLanesDAG_generic.cpp"

$(IntermediateDirectory)/TrackingLanesDAG_generic.cpp$(PreprocessSuffix): TrackingLanesDAG_generic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TrackingLanesDAG_generic.cpp$(PreprocessSuffix) "TrackingLanesDAG_generic.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


