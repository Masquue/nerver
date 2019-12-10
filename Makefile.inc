# proj_dir is the directory where this Makefile.inc lies
proj_dir := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
obj_dir := $(proj_dir)/obj
src_dir := $(proj_dir)/src
test_dir := $(proj_dir)/test

CXX := g++
debug_flags := -g
warning_flags := -Wall -Werror=vla
CXXFLAGS :=  $(warning_flags) $(debug_flags) -std=c++11 -I$(src_dir)

static_lib := $(obj_dir)/libnerver.a
