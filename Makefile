proj_dir = .
src_dir = $(proj_dir)/src
test_dir = $(proj_dir)/test
obj_dir = $(proj_dir)/obj
build_dir = $(proj_dir)/build

CXX = g++
debug_flags = -g
warning_flags = -Wall -Werror=vla
CXXFLAGS = $(warning_flags) $(debug_flags) -std=c++11 -I$(src_dir)

srcs_tmp = $(wildcard $(src_dir)/*.cc)
srcs = $(filter-out $(src_dir)/main.cc, $(srcs_tmp))
objs = $(srcs:$(src_dir)/%.cc=$(obj_dir)/%.o)
# static_lib = $(obj_dir)/libnerver.a
nerver = $(build_dir)/nerver

test_srcs = $(wildcard $(test_dir)/*.cc)
tests = $(test_srcs:$(test_dir)/%.cc=$(build_dir)/%)

.PHONY: all
all: $(nerver)

$(obj_dir) $(build_dir):
	@mkdir $@

$(obj_dir)/main.o: $(src_dir)/main.cc | $(obj_dir)
	@echo compiling $<
	@$(CXX) -c -o $@ $< $(CXXFLAGS)

$(obj_dir)/%.o: $(src_dir)/%.cc $(src_dir)/%.h | $(obj_dir)
	@echo compiling $<
	@$(CXX) -c -o $@ $< $(CXXFLAGS)

# $(static_lib): $(objs)
# 	@echo building $@
# 	@ar rc $@ $^

$(nerver): $(src_dir)/main.cc $(objs) | $(build_dir)
	@echo building $@
	@$(CXX) -o $@ $^ $(CXXFLAGS)

# test srcs
$(build_dir)/%: $(test_dir)/%.cc $(objs) | $(build_dir)
	@echo building $@
	@$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: test
test: $(tests)	

.PHONY: clean
clean:
	@rm -rf $(obj_dir)
	@rm -rf $(build_dir)

.PHONY: clean-test
clean-test:
	@rm -rf $(tests)
