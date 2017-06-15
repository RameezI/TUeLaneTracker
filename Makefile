.PHONY: clean All

All:
	@echo "----------Building project:[ TUeLDT - Debug ]----------"
	@cd "TUeLDT" && "$(MAKE)" -f  "TUeLDT.mk"
	@echo "----------Building project:[ UnitTests - Debug ]----------"
	@cd "UnitTests" && "$(MAKE)" -f  "UnitTests.mk"
clean:
	@echo "----------Cleaning project:[ TUeLDT - Debug ]----------"
	@cd "TUeLDT" && "$(MAKE)" -f  "TUeLDT.mk"  clean
	@echo "----------Cleaning project:[ UnitTests - Debug ]----------"
	@cd "UnitTests" && "$(MAKE)" -f  "UnitTests.mk" clean
