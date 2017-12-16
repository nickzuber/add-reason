
print_msg = @printf "\n\033[1;34m***%s***\033[0m\n" "$(1)"

build:
	$(call print_msg, Building... )
	yarn run build

test:
	$(call print_msg, Running tests... )
	node ./test/

.PHONY: test