##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=UnitTests
ConfigurationName      :=Debug
WorkspacePath          := "/home/rameez/TUeLaneTracker"
ProjectPath            := "/home/rameez/TUeLaneTracker/UnitTests"
IntermediateDirectory  :=Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=rameez
Date                   :=09/05/17
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="UnitTests.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -fopenmp
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)$(UNIT_TEST_PP_SRC_DIR)/src $(IncludeSwitch)../TUeLDT/include $(IncludeSwitch)/usr/local/include/UnitTest++ $(IncludeSwitch)/usr/local/include/eigen3 $(IncludeSwitch)../include $(IncludeSwitch)../GeneratedFcns/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)UnitTest++ $(LibrarySwitch)TUeLDT $(LibrarySwitch)GeneratedFcns $(LibrarySwitch)opencv_shape $(LibrarySwitch)opencv_stitching $(LibrarySwitch)opencv_objdetect $(LibrarySwitch)opencv_superres $(LibrarySwitch)opencv_videostab $(LibrarySwitch)opencv_calib3d $(LibrarySwitch)opencv_features2d $(LibrarySwitch)opencv_highgui $(LibrarySwitch)opencv_videoio $(LibrarySwitch)opencv_imgcodecs $(LibrarySwitch)opencv_video $(LibrarySwitch)opencv_photo $(LibrarySwitch)opencv_ml $(LibrarySwitch)opencv_imgproc $(LibrarySwitch)opencv_flann $(LibrarySwitch)opencv_core 
ArLibs                 :=  "libUnitTest++.a" "libTUeLDT.a" "libGeneratedFcns.a" "opencv_shape" "opencv_stitching" "opencv_objdetect" "opencv_superres" "opencv_videostab" "opencv_calib3d" "opencv_features2d" "opencv_highgui" "opencv_videoio" "opencv_imgcodecs" "opencv_video" "opencv_photo" "opencv_ml" "opencv_imgproc" "opencv_flann" "opencv_core" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)$(UNIT_TEST_PP_SRC_DIR)/Debug $(LibraryPathSwitch)../TUeLDT/Debug $(LibraryPathSwitch)../GeneratedFcns/Debug/ 

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
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/LaneFilterTests.cpp$(ObjectSuffix) $(IntermediateDirectory)/vpFilterTests.cpp$(ObjectSuffix) $(IntermediateDirectory)/BootingStateTests.cpp$(ObjectSuffix) $(IntermediateDirectory)/time_measure.cpp$(ObjectSuffix) $(IntermediateDirectory)/BufferingStateTests.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d "../.build-debug/TUeLDT" $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

"../.build-debug/TUeLDT":
	@$(MakeDirCommand) "../.build-debug"
	@echo stam > "../.build-debug/TUeLDT"




MakeIntermediateDirs:
	@test -d Debug || $(MakeDirCommand) Debug


$(IntermediateDirectory)/.d:
	@test -d Debug || $(MakeDirCommand) Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/UnitTests/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/LaneFilterTests.cpp$(ObjectSuffix): LaneFilterTests.cpp $(IntermediateDirectory)/LaneFilterTests.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/UnitTests/LaneFilterTests.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LaneFilterTests.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LaneFilterTests.cpp$(DependSuffix): LaneFilterTests.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LaneFilterTests.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LaneFilterTests.cpp$(DependSuffix) -MM "LaneFilterTests.cpp"

$(IntermediateDirectory)/LaneFilterTests.cpp$(PreprocessSuffix): LaneFilterTests.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LaneFilterTests.cpp$(PreprocessSuffix) "LaneFilterTests.cpp"

$(IntermediateDirectory)/vpFilterTests.cpp$(ObjectSuffix): vpFilterTests.cpp $(IntermediateDirectory)/vpFilterTests.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/UnitTests/vpFilterTests.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/vpFilterTests.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/vpFilterTests.cpp$(DependSuffix): vpFilterTests.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/vpFilterTests.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/vpFilterTests.cpp$(DependSuffix) -MM "vpFilterTests.cpp"

$(IntermediateDirectory)/vpFilterTests.cpp$(PreprocessSuffix): vpFilterTests.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/vpFilterTests.cpp$(PreprocessSuffix) "vpFilterTests.cpp"

$(IntermediateDirectory)/BootingStateTests.cpp$(ObjectSuffix): BootingStateTests.cpp $(IntermediateDirectory)/BootingStateTests.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/UnitTests/BootingStateTests.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BootingStateTests.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BootingStateTests.cpp$(DependSuffix): BootingStateTests.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BootingStateTests.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BootingStateTests.cpp$(DependSuffix) -MM "BootingStateTests.cpp"

$(IntermediateDirectory)/BootingStateTests.cpp$(PreprocessSuffix): BootingStateTests.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BootingStateTests.cpp$(PreprocessSuffix) "BootingStateTests.cpp"

$(IntermediateDirectory)/time_measure.cpp$(ObjectSuffix): time_measure.cpp $(IntermediateDirectory)/time_measure.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/UnitTests/time_measure.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/time_measure.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/time_measure.cpp$(DependSuffix): time_measure.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/time_measure.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/time_measure.cpp$(DependSuffix) -MM "time_measure.cpp"

$(IntermediateDirectory)/time_measure.cpp$(PreprocessSuffix): time_measure.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/time_measure.cpp$(PreprocessSuffix) "time_measure.cpp"

$(IntermediateDirectory)/BufferingStateTests.cpp$(ObjectSuffix): BufferingStateTests.cpp $(IntermediateDirectory)/BufferingStateTests.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/rameez/TUeLaneTracker/UnitTests/BufferingStateTests.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BufferingStateTests.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BufferingStateTests.cpp$(DependSuffix): BufferingStateTests.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BufferingStateTests.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BufferingStateTests.cpp$(DependSuffix) -MM "BufferingStateTests.cpp"

$(IntermediateDirectory)/BufferingStateTests.cpp$(PreprocessSuffix): BufferingStateTests.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BufferingStateTests.cpp$(PreprocessSuffix) "BufferingStateTests.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r Debug/


