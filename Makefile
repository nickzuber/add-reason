
print_msg = @printf "\n\033[1;34m***%s***\033[0m\n" "$(1)"

all:
	$(call print_msg, Building and Testing... )
	bsb -make-world
	node ./test/

build:
	$(call print_msg, Building... )
	bsb -make-world

watch:
	$(call print_msg, Building w/ Watch... )
	bsb -make-world-w

test:
	$(call print_msg, Running tests... )
	node ./test/

clean:
	$(call print_msg, Cleaning... )
	bsb -clean-world

.PHONY: build test clean
