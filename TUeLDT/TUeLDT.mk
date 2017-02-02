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
Date                   :=02/02/17
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
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)./include $(IncludeSwitch)/usr/local/include/eigen3 $(IncludeSwitch)../InitState/ 
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
Objects0=$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) $(IntermediateDirectory)/Car.cpp$(ObjectSuffix) $(IntermediateDirectory)/Lane.cpp$(ObjectSuffix) $(IntermediateDirectory)/sigInit.cpp$(ObjectSuffix) $(IntermediateDirectory)/State.cpp$(ObjectSuffix) $(IntermediateDirectory)/StateMachine.cpp$(ObjectSuffix) $(IntermediateDirectory)/LaneFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/VanishingPtFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/InitState.cpp$(ObjectSuffix) $(IntermediateDirectory)/include_BufferingState.cpp$(ObjectSuffix) \
	



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
$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix): Camera.cpp $(IntermediateDirectory)/Camera.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/Camera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Camera.cpp$(DependSuffix): Camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Camera.cpp$(DependSuffix) -MM "Camera.cpp"

$(IntermediateDirectory)/Camera.cpp$(PreprocessSuffix): Camera.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Camera.cpp$(PreprocessSuffix) "Camera.cpp"

$(IntermediateDirectory)/Car.cpp$(ObjectSuffix): Car.cpp $(IntermediateDirectory)/Car.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/Car.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Car.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Car.cpp$(DependSuffix): Car.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Car.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Car.cpp$(DependSuffix) -MM "Car.cpp"

$(IntermediateDirectory)/Car.cpp$(PreprocessSuffix): Car.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Car.cpp$(PreprocessSuffix) "Car.cpp"

$(IntermediateDirectory)/Lane.cpp$(ObjectSuffix): Lane.cpp $(IntermediateDirectory)/Lane.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/Lane.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Lane.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Lane.cpp$(DependSuffix): Lane.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Lane.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Lane.cpp$(DependSuffix) -MM "Lane.cpp"

$(IntermediateDirectory)/Lane.cpp$(PreprocessSuffix): Lane.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Lane.cpp$(PreprocessSuffix) "Lane.cpp"

$(IntermediateDirectory)/sigInit.cpp$(ObjectSuffix): sigInit.cpp $(IntermediateDirectory)/sigInit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/sigInit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sigInit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sigInit.cpp$(DependSuffix): sigInit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sigInit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/sigInit.cpp$(DependSuffix) -MM "sigInit.cpp"

$(IntermediateDirectory)/sigInit.cpp$(PreprocessSuffix): sigInit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sigInit.cpp$(PreprocessSuffix) "sigInit.cpp"

$(IntermediateDirectory)/State.cpp$(ObjectSuffix): State.cpp $(IntermediateDirectory)/State.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/State.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/State.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/State.cpp$(DependSuffix): State.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/State.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/State.cpp$(DependSuffix) -MM "State.cpp"

$(IntermediateDirectory)/State.cpp$(PreprocessSuffix): State.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/State.cpp$(PreprocessSuffix) "State.cpp"

$(IntermediateDirectory)/StateMachine.cpp$(ObjectSuffix): StateMachine.cpp $(IntermediateDirectory)/StateMachine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/StateMachine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/StateMachine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/StateMachine.cpp$(DependSuffix): StateMachine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/StateMachine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/StateMachine.cpp$(DependSuffix) -MM "StateMachine.cpp"

$(IntermediateDirectory)/StateMachine.cpp$(PreprocessSuffix): StateMachine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/StateMachine.cpp$(PreprocessSuffix) "StateMachine.cpp"

$(IntermediateDirectory)/LaneFilter.cpp$(ObjectSuffix): LaneFilter.cpp $(IntermediateDirectory)/LaneFilter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/LaneFilter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LaneFilter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LaneFilter.cpp$(DependSuffix): LaneFilter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LaneFilter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LaneFilter.cpp$(DependSuffix) -MM "LaneFilter.cpp"

$(IntermediateDirectory)/LaneFilter.cpp$(PreprocessSuffix): LaneFilter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LaneFilter.cpp$(PreprocessSuffix) "LaneFilter.cpp"

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

$(IntermediateDirectory)/include_BufferingState.cpp$(ObjectSuffix): include/BufferingState.cpp $(IntermediateDirectory)/include_BufferingState.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/include/BufferingState.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/include_BufferingState.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/include_BufferingState.cpp$(DependSuffix): include/BufferingState.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/include_BufferingState.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/include_BufferingState.cpp$(DependSuffix) -MM "include/BufferingState.cpp"

$(IntermediateDirectory)/include_BufferingState.cpp$(PreprocessSuffix): include/BufferingState.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/include_BufferingState.cpp$(PreprocessSuffix) "include/BufferingState.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


