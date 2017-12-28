
print_msg = @printf "\n\033[1;34m***%s***\033[0m\n" "$(1)"

all: build test-fresh

build:
	$(call print_msg, Building... )
	bsb -make-world

# @TODO: Each test should clean up after itself, so this command should eventually be removed.
test-fresh:
	$(call print_msg, Running tests... )
	make reset-mock
	node ./tests/

test:
	$(call print_msg, Running tests... )
	node ./tests/

clean:
	$(call print_msg, Cleaning... )
	make reset-mock
	bsb -clean-world

python-tests:
	$(call print_msg, Running tests... )
	nosetests ./tests

reset-mock:
	rm -f ./tests/mockRoot/bsconfig.json
	rm -f ./tests/mockRoot/.merlin
	rm -f ./tests/mockRoot/node_modules/reason-package
	rm -rf ./tests/mockRoot/lib

.PHONY: all build test-fresh test clean
