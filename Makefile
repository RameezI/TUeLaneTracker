.PHONY: clean All

All:
	@echo "----------Building project:[ TUeLDT - Debug ]----------"
	@cd "TUeLDT" && "$(MAKE)" -f  "TUeLDT.mk"
	@echo "----------Building project:[ LaneTrackerApp - Debug ]----------"
	@cd "LaneTrackerApp" && "$(MAKE)" -f  "LaneTrackerApp.mk"
clean:
	@echo "----------Cleaning project:[ TUeLDT - Debug ]----------"
	@cd "TUeLDT" && "$(MAKE)" -f  "TUeLDT.mk"  clean
	@echo "----------Cleaning project:[ LaneTrackerApp - Debug ]----------"
	@cd "LaneTrackerApp" && "$(MAKE)" -f  "LaneTrackerApp.mk" clean
