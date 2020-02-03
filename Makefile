proj_dir = .
src_dir = $(proj_dir)/src
test_dir = $(proj_dir)/test
obj_dir = $(proj_dir)/obj

CXX = g++
debug_flags = -g
warning_flags = -Wall -Werror=vla
CXXFLAGS = $(warning_flags) $(debug_flags) -std=c++11 -I$(src_dir)

srcs = $(wildcard $(src_dir)/*.cc)
objs = $(srcs:$(src_dir)/%.cc=$(obj_dir)/%.o)
static_lib = $(obj_dir)/libnerver.a

test_srcs = $(wildcard $(test_dir)/*.cc)
tests = $(test_srcs:%.cc=%)

.PHONY: all
all: $(static_lib) test

$(obj_dir):
	@mkdir $@

$(obj_dir)/%.o: $(src_dir)/%.cc $(src_dir)/%.h | $(obj_dir)
	@echo compiling $<
	@$(CXX) -c -o $@ $< $(CXXFLAGS)

$(static_lib): $(objs)
	@echo building $@
	@ar rc $@ $^

# test srcs
$(test_dir)/%: $(test_dir)/%.cc $(static_lib) 
	@echo building $@
	@$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: test
test: $(tests)	

.PHONY: clean
clean:
	@rm -rf $(obj_dir)
	@rm -rf $(tests)

.PHONY: print
print: 
	@echo $(srcs)
