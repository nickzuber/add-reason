
print_msg = @printf "\n\033[1;34m***%s***\033[0m\n" "$(1)"

all: build test

build:
	$(call print_msg, Building... )
	bsb -make-world

test:
	$(call print_msg, Running tests... )
	nosetests ./tests

clean:
	$(call print_msg, Cleaning... )
	make reset-mock
	bsb -clean-world

reset-mock:
	rm -f ./tests/root_for_testing/bsconfig.json
	rm -f ./tests/root_for_testing/.merlin
	rm -f ./tests/root_for_testing/node_modules/reason-package
	rm -rf ./tests/root_for_testing/lib
	cp package.empty_scripts.json package.json

.PHONY: all build test clean reset-mock
