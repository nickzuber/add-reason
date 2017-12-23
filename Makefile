
print_msg = @printf "\n\033[1;34m***%s***\033[0m\n" "$(1)"

all:
	make build
	make test

build:
	$(call print_msg, Building... )
	bsb -make-world

test:
	$(call print_msg, Running tests... )
	# @TODO: Each test should clean up after itself.
	rm -f ./tests/mockRoot/bsconfig.json
	rm -f ./tests/mockRoot/node_modules/reason-package
	node ./tests/

clean:
	$(call print_msg, Cleaning... )
	bsb -clean-world

.PHONY: all build test clean
