.PHONY: clean All

All:
	@echo "----------Building project:[ InitStateLDT - Debug ]----------"
	@cd "InitState" && "$(MAKE)" -f  "InitStateLDT.mk"
	@echo "----------Building project:[ TUeLDT - Debug ]----------"
	@cd "TUeLDT" && "$(MAKE)" -f  "TUeLDT.mk"
	@echo "----------Building project:[ LaneTrackerApp - Debug ]----------"
	@cd "LaneTrackerApp" && "$(MAKE)" -f  "LaneTrackerApp.mk"
clean:
	@echo "----------Cleaning project:[ InitStateLDT - Debug ]----------"
	@cd "InitState" && "$(MAKE)" -f  "InitStateLDT.mk"  clean
	@echo "----------Cleaning project:[ TUeLDT - Debug ]----------"
	@cd "TUeLDT" && "$(MAKE)" -f  "TUeLDT.mk"  clean
	@echo "----------Cleaning project:[ LaneTrackerApp - Debug ]----------"
	@cd "LaneTrackerApp" && "$(MAKE)" -f  "LaneTrackerApp.mk" clean
