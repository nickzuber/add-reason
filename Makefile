
print_msg = @printf "\n\033[1;34m***%s***\033[0m\n" "$(1)"

all:
	make build
	make test-fresh

build:
	$(call print_msg, Building... )
	bsb -make-world

# @TODO: Each test should clean up after itself, so this command should eventually be removed.
test-fresh:
	$(call print_msg, Running tests... )
	rm -f ./tests/mockRoot/bsconfig.json
	rm -f ./tests/mockRoot/node_modules/reason-package
	node ./tests/

test:
	$(call print_msg, Running tests... )
	node ./tests/

clean:
	$(call print_msg, Cleaning... )
	rm -f ./tests/mockRoot/bsconfig.json
	rm -f ./tests/mockRoot/node_modules/reason-package
	bsb -clean-world

.PHONY: all build test-fresh test clean
