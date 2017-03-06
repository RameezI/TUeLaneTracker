.PHONY: clean All

All:
	@echo "----------Building project:[ TUeLDT - Debug ]----------"
	@cd "TUeLDT" && "$(MAKE)" -f  "TUeLDT.mk"
clean:
	@echo "----------Cleaning project:[ TUeLDT - Debug ]----------"
	@cd "TUeLDT" && "$(MAKE)" -f  "TUeLDT.mk" clean
