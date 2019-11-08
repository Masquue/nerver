include Makefile.inc

srcs := $(wildcard $(src_dir)/*.cc)
objs := $(srcs:$(src_dir)/%.cc=$(obj_dir)/%.o)

.PHONY: all
all: $(static_lib)

$(obj_dir):
	@mkdir $@

$(obj_dir)/%.o: $(src_dir)/%.cc | $(obj_dir)
	@echo compiling $<
	@$(CXX) -c -o $@ $< $(CXXFLAGS)

$(static_lib): $(objs)
	@echo building $@
	@ar rc $@ $^

.PHONY: clean
clean:
	@rm -rf $(obj_dir)
	@$(MAKE) -C test clean

.PHONY: print
print:
	@echo $(proj_dir)

.PHONY: print_in_parent
print_in_parent:
	@echo $(test_dir)
