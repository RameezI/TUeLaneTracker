.PHONY: clean All

All:
	@echo "----------Building project:[ UnitTests - Debug ]----------"
	@cd "UnitTests" && "$(MAKE)" -f  "UnitTests.mk"
clean:
	@echo "----------Cleaning project:[ UnitTests - Debug ]----------"
	@cd "UnitTests" && "$(MAKE)" -f  "UnitTests.mk" clean
