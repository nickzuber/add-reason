
print_msg = @printf "\n\033[1;34m***%s***\033[0m\n" "$(1)"

build:
	$(call print_msg, Building... )
	bsb -make-world

test:
	$(call print_msg, Running tests... )
	node ./test/

clean:
	$(call print_msg, Cleaning... )
	bsb -clean-world

.PHONY: build test clean
