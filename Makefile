
print_msg = @printf "\n\033[1;34m***%s***\033[0m\n" "$(1)"

all:
	$(call print_msg, Building and Testing... )
	bsb -make-world
	node ./__tests__/

build:
	$(call print_msg, Building... )
	bsb -make-world

test:
	$(call print_msg, Running tests... )
	node ./__tests__/

travis-test:
	$(call print_msg, Running tests in Travis Environment... )
	ln -sv ../lib/js/src/initializer ./node_modules/initializer
	node ./__tests__/

clean:
	$(call print_msg, Cleaning... )
	bsb -clean-world

.PHONY: all build test clean
