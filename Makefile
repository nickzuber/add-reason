
print_msg = @printf "\n\033[1;34m***%s***\033[0m\n" "$(1)"

all: build test

build:
	$(call print_msg, Building... )
	bsb -make-world

watch:
	$(call print_msg, Building & Watching... )
	bsb -w -make-world

test:
	$(call print_msg, Running tests... )
	nosetests ./tests

try:
	make build
	@echo "\n"
	cd ./tests/root_for_testing; \
		add-reason setup src/myCode
	@echo "\n"
	make reset-mock

clean:
	$(call print_msg, Cleaning... )
	make reset-mock
	bsb -clean-world

reset-mock:
	rm -f ./tests/root_for_testing/bsconfig.json
	rm -f ./tests/root_for_testing/.merlin
	rm -rf ./tests/root_for_testing/lib
	rm -f ./tests/root_for_testing/node_modules/*
	touch ./tests/root_for_testing/node_modules/persist.lock
	cp ./tests/root_for_testing/package.empty_scripts.json ./tests/root_for_testing/package.json

.PHONY: all build watch test try clean reset-mock
