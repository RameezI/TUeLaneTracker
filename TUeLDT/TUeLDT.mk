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
Date                   :=06/03/17
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
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../include $(IncludeSwitch)./include $(IncludeSwitch)../GeneratedFcns/include $(IncludeSwitch)/usr/local/include/eigen3 
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
Objects0=$(IntermediateDirectory)/Camera.cpp$(ObjectSuffix) $(IntermediateDirectory)/Car.cpp$(ObjectSuffix) $(IntermediateDirectory)/Lane.cpp$(ObjectSuffix) $(IntermediateDirectory)/State.cpp$(ObjectSuffix) $(IntermediateDirectory)/LaneFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/VanishingPtFilter.cpp$(ObjectSuffix) $(IntermediateDirectory)/InitState.cpp$(ObjectSuffix) $(IntermediateDirectory)/ColorTransformer.cpp$(ObjectSuffix) $(IntermediateDirectory)/GradientExtractor.cpp$(ObjectSuffix) $(IntermediateDirectory)/ChannelsSplitter.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/MaxPoolingProcessor.cpp$(ObjectSuffix) $(IntermediateDirectory)/SigInit.cpp$(ObjectSuffix) $(IntermediateDirectory)/BufferingState.cpp$(ObjectSuffix) $(IntermediateDirectory)/StateMachine.cpp$(ObjectSuffix) $(IntermediateDirectory)/GaussianPreProcessor.cpp$(ObjectSuffix) $(IntermediateDirectory)/LDT_logger.cpp$(ObjectSuffix) $(IntermediateDirectory)/LDT_profiler.cpp$(ObjectSuffix) $(IntermediateDirectory)/TemplatesExtractor.cpp$(ObjectSuffix) $(IntermediateDirectory)/BufferingDAG.cpp$(ObjectSuffix) 



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

$(IntermediateDirectory)/State.cpp$(ObjectSuffix): State.cpp $(IntermediateDirectory)/State.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/State.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/State.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/State.cpp$(DependSuffix): State.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/State.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/State.cpp$(DependSuffix) -MM "State.cpp"

$(IntermediateDirectory)/State.cpp$(PreprocessSuffix): State.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/State.cpp$(PreprocessSuffix) "State.cpp"

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

$(IntermediateDirectory)/ColorTransformer.cpp$(ObjectSuffix): ColorTransformer.cpp $(IntermediateDirectory)/ColorTransformer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/ColorTransformer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ColorTransformer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ColorTransformer.cpp$(DependSuffix): ColorTransformer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ColorTransformer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ColorTransformer.cpp$(DependSuffix) -MM "ColorTransformer.cpp"

$(IntermediateDirectory)/ColorTransformer.cpp$(PreprocessSuffix): ColorTransformer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ColorTransformer.cpp$(PreprocessSuffix) "ColorTransformer.cpp"

$(IntermediateDirectory)/GradientExtractor.cpp$(ObjectSuffix): GradientExtractor.cpp $(IntermediateDirectory)/GradientExtractor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/GradientExtractor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GradientExtractor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GradientExtractor.cpp$(DependSuffix): GradientExtractor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GradientExtractor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GradientExtractor.cpp$(DependSuffix) -MM "GradientExtractor.cpp"

$(IntermediateDirectory)/GradientExtractor.cpp$(PreprocessSuffix): GradientExtractor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GradientExtractor.cpp$(PreprocessSuffix) "GradientExtractor.cpp"

$(IntermediateDirectory)/ChannelsSplitter.cpp$(ObjectSuffix): ChannelsSplitter.cpp $(IntermediateDirectory)/ChannelsSplitter.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/ChannelsSplitter.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/ChannelsSplitter.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/ChannelsSplitter.cpp$(DependSuffix): ChannelsSplitter.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/ChannelsSplitter.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/ChannelsSplitter.cpp$(DependSuffix) -MM "ChannelsSplitter.cpp"

$(IntermediateDirectory)/ChannelsSplitter.cpp$(PreprocessSuffix): ChannelsSplitter.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/ChannelsSplitter.cpp$(PreprocessSuffix) "ChannelsSplitter.cpp"

$(IntermediateDirectory)/MaxPoolingProcessor.cpp$(ObjectSuffix): MaxPoolingProcessor.cpp $(IntermediateDirectory)/MaxPoolingProcessor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/MaxPoolingProcessor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MaxPoolingProcessor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MaxPoolingProcessor.cpp$(DependSuffix): MaxPoolingProcessor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MaxPoolingProcessor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/MaxPoolingProcessor.cpp$(DependSuffix) -MM "MaxPoolingProcessor.cpp"

$(IntermediateDirectory)/MaxPoolingProcessor.cpp$(PreprocessSuffix): MaxPoolingProcessor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MaxPoolingProcessor.cpp$(PreprocessSuffix) "MaxPoolingProcessor.cpp"

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

$(IntermediateDirectory)/GaussianPreProcessor.cpp$(ObjectSuffix): GaussianPreProcessor.cpp $(IntermediateDirectory)/GaussianPreProcessor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/GaussianPreProcessor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/GaussianPreProcessor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/GaussianPreProcessor.cpp$(DependSuffix): GaussianPreProcessor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/GaussianPreProcessor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/GaussianPreProcessor.cpp$(DependSuffix) -MM "GaussianPreProcessor.cpp"

$(IntermediateDirectory)/GaussianPreProcessor.cpp$(PreprocessSuffix): GaussianPreProcessor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/GaussianPreProcessor.cpp$(PreprocessSuffix) "GaussianPreProcessor.cpp"

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

$(IntermediateDirectory)/TemplatesExtractor.cpp$(ObjectSuffix): TemplatesExtractor.cpp $(IntermediateDirectory)/TemplatesExtractor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/TemplatesExtractor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TemplatesExtractor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TemplatesExtractor.cpp$(DependSuffix): TemplatesExtractor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TemplatesExtractor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/TemplatesExtractor.cpp$(DependSuffix) -MM "TemplatesExtractor.cpp"

$(IntermediateDirectory)/TemplatesExtractor.cpp$(PreprocessSuffix): TemplatesExtractor.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TemplatesExtractor.cpp$(PreprocessSuffix) "TemplatesExtractor.cpp"

$(IntermediateDirectory)/BufferingDAG.cpp$(ObjectSuffix): BufferingDAG.cpp $(IntermediateDirectory)/BufferingDAG.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/TUeLDT/BufferingDAG.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BufferingDAG.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BufferingDAG.cpp$(DependSuffix): BufferingDAG.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BufferingDAG.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BufferingDAG.cpp$(DependSuffix) -MM "BufferingDAG.cpp"

$(IntermediateDirectory)/BufferingDAG.cpp$(PreprocessSuffix): BufferingDAG.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BufferingDAG.cpp$(PreprocessSuffix) "BufferingDAG.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


