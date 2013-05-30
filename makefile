#.PHONY: all
ps : ps.c
	gcc $^ -o $@
