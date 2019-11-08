# proj_dir is the directory where this Makefile.inc lies
proj_dir := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
obj_dir := $(proj_dir)/obj
src_dir := $(proj_dir)/src
test_dir := $(proj_dir)/test

CXX := g++
DEBUG := -g
CXXFLAGS := -Wall -std=c++11 $(DEBUG) -I$(src_dir)

static_lib := $(obj_dir)/libnerver.a
