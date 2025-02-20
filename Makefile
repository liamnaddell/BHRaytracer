JLEVEL=-j4
all: debug release openmp
CARGS=-DBOOST_ROOT=${HOME}/sysroot  -DCMAKE_MODULE_PATH=${HOME}/sysroot/lib64/cmake/fmt/

### debug: Generate debug version of the code
debug: dirs
	cd debug && cmake ${CARGS} .. && make $(JLEVEL)

### openmp: Generate OMP shared-memory parallelism version of the code
openmp: dirs
	cd openmp && cmake ${CARGS} -DCMAKE_BUILD_TYPE=Release -DENABLE_MPI=False -DENABLE_OPENMP=True .. && make $(JLEVEL)

### release: Generate MPI+OMP parallelism version of the code
release: dirs
	cd release && cmake ${CARGS} -DCMAKE_BUILD_TYPE=Release -DENABLE_MPI=True -DENABLE_OPENMP=True .. && make $(JLEVEL)

dirs:
	mkdir -p debug release openmp


### docs: Generate documentation with Doxygen
docs:
	doxygen Doxyfile

### help: Makefile's descriptive rules help
help: Makefile
	@sed -n 's/^###//p' $<

clean:
	cd debug && make clean
	cd release && make clean
	cd openmp && make clean

distclean:
	-rm -rf debug release openmp

#cppcheck:
#	cppcheck --language=c++ --std=c++17 main.cc mov_avg.cc mov_avg.h
format:
	find src -iname '*.h' -o -iname '*.cc' | xargs clang-format -i
