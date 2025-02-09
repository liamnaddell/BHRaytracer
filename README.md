# Ray tracing black holes

## Install

* Need a recent boost version. Version 1.86.0 was chosen for the project. Later (and some earlier) versions are likely to work, but require you to change CMakeLists.txt. We tried compiling with some 1.7* versions, but were not able to compile GIL. Only boost headers are required, specifically GIL.
* `source teachsetup` on the scinet TEACH cluster
* `source niagarasetup` on the scinet TEACH cluster
* See these scripts for the versions of libraries required and tested on.

## Building

A Makefile is provided to build across multiple different build styles. Support exists for MPI, OpenMP, or a hybrid of the two. Support additionally exists for Release builds, and Debug builds.

* `source teachsetup`
* `cmake -DENABLE_OPENMP=True -DENABLE_MPI=True -DCMAKE_BUILD_TYPE=Release`

## Profiling

You can optionally enable MPI in addition to this command.

`cmake -DENABLE_OPENMP=True -DENABLE_PROF=True -DCMAKE_BUILD_TYPE=Release`

make sure to `module load binutils/2.42` if you are on SciNet.

## Running

You can run `./main -h` for an updated list of help options after building.

```
  -i <img_name>:
    default ../squares.jpg
  -b <black hole loc>
    default -400
  -B <background loc>
    default -500
  -M <mass>
    default 10
  -e <epsilon>
    default 5
  -W <width>
    default 400
  -d
    set debug, default false
  -c <cores>
    use <cores> for multiprocessing.
  -S
    wait for GDB to attach before initializing mpi..
  -s <number of samples>
    default 1
```

After building with MPI, you can run the binary using `mpirun -c ? ./main <options>`.
When supplying `-c` to `mpirun`, this controls the number of MPI worker processes. When supplying `-c` to `./main`, this controls the number of OMP threads. These options work concurrently when built with OpenMP and MPI support.

## Documentation

Documentation can be generated using `doxygen`
