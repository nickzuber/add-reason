
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
	rm -f ./tests/mockRoot/bsconfig.json
	rm -f ./tests/mockRoot/.merlin
	rm -f ./tests/mockRoot/node_modules/reason-package
	rm -rf ./tests/mockRoot/lib

.PHONY: all build test clean reset-mock
