include ../Makefile.inc

test_srcs := $(wildcard $(test_dir)/*.cc)
tests := $(test_srcs:%.cc=%)

.PHONY: all
all: $(tests)

$(test_dir)/%: $(test_dir)/%.cc $(static_lib) 
	@echo building $@
	@$(CXX) -o $@ $^ $(CXXFLAGS)

$(static_lib): 
	@$(MAKE) -C .. $@

.PHONY: clean
clean:
	@rm -rf $(tests)

.PHONY: print
print:
	@echo $(static_lib)
