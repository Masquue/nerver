include Makefile.inc

srcs := $(wildcard $(src_dir)/*.cc)
objs := $(srcs:$(src_dir)/%.cc=$(obj_dir)/%.o)

.PHONY: all
all: $(static_lib) test

$(obj_dir):
	@mkdir $@

$(obj_dir)/%.o: $(src_dir)/%.cc | $(obj_dir)
	@echo compiling $<
	@$(CXX) -c -o $@ $< $(CXXFLAGS)

$(static_lib): $(objs)
	@echo building $@
	@ar rc $@ $^

.PHONY: test
test:
	@$(MAKE) -C test

.PHONY: clean
clean:
	@rm -rf $(obj_dir)
	@$(MAKE) -C test clean
