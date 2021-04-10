CXX = dpcpp
CXXFLAGS_CPU = -O2 -g -std=c++17 --gcc-toolchain=/home/user/gcbritt/GCC-7.4.0/ -B /home/user/gcbritt/GCC-7.4.0/lib/gcc/x86_64-pc-linux-gnu/7.4.0/ -B /home/user/gcbritt/intel/inteloneapi/compiler/2021.1-beta06/linux/lib/oclfpga/bin/ -L /home/user/gcbritt/GCC-7.4.0/lib/gcc/x86_64-pc-linux-gnu/7.4.0/ -isystem /home/user/gcbritt/GCC-7.4.0/include/c++/7.4.0/ -isystem /home/user/gcbritt/GCC-7.4.0/include/c++/7.4.0/x86_64-pc-linux-gnu/ -L /home/user/gcbritt/GCC-7.4.0/lib64/

CXXFLAGS_FPGA = -O2 -g -fintelfpga -std=c++17 --gcc-toolchain=/home/user/gcbritt/GCC-7.4.0/ -B /home/user/gcbritt/GCC-7.4.0/lib/gcc/x86_64-pc-linux-gnu/7.4.0/ -B /home/user/gcbritt/intel/inteloneapi/compiler/2021.1-beta06/linux/lib/oclfpga/bin/ -L /home/user/gcbritt/GCC-7.4.0/lib/gcc/x86_64-pc-linux-gnu/7.4.0/ -isystem /home/user/gcbritt/GCC-7.4.0/include/c++/7.4.0/ -isystem /home/user/gcbritt/GCC-7.4.0/include/c++/7.4.0/x86_64-pc-linux-gnu/ -L /home/user/gcbritt/GCC-7.4.0/lib64/


INTRA_CPU_EXE = intra_dimension_dependent/out/intra_dimension_dependent_cpu.out
INTRA_FPGA_EXE = intra_dimension_dependent/out/intra_dimension_dependent_fpga.out
INTRA_CPU_SOURCES = intra_dimension_dependent/src/intra_dimension_dependent_cpu.cpp
INTRA_FPGA_SOURCES = intra_dimension_dependent/src/intra_dimension_dependent_fpga.cpp

DIAG_CPU_EXE = diagonal_dependent/out/diagonal_dependent_cpu.out
DIAG_FPGA_EXE = diagonal_dependent/out/diagonal_dependent_fpga.out
DIAG_CPU_SOURCES = diagonal_dependent/src/diagonal_dependent_cpu.cpp
DIAG_FPGA_SOURCES = diagonal_dependent/src/diagonal_dependent_fpga.cpp

COND_CPU_EXE = conditional_dependent/out/conditional_dependent_cpu.out
COND_FPGA_EXE = conditional_dependent/out/conditional_dependent_fpga.out
COND_CPU_SOURCES = conditional_dependent/src/conditional_dependent_cpu.cpp
COND_FPGA_SOURCES = conditional_dependent/src/conditional_dependent_fpga.cpp

HALF_CPU_EXE = half_parallelism_half_dependent/out/half_parallelism_half_dependent_cpu.out
HALF_FPGA_EXE = half_parallelism_half_dependent/out/half_parallelism_half_dependent_fpga.out
HALF_CPU_SOURCES = half_parallelism_half_dependent/src/half_parallelism_half_dependent_cpu.cpp
HALF_FPGA_SOURCES = half_parallelism_half_dependent/src/half_parallelism_half_dependent_fpga.cpp

ANTI_CPU_EXE = anti_dependent/out/anti_dependent_cpu.out
ANTI_FPGA_EXE = anti_dependent/out/anti_dependent_fpga.out
ANTI_CPU_SOURCES = anti_dependent/src/anti_dependent_cpu.cpp
ANTI_FPGA_SOURCES = anti_dependent/src/anti_dependent_fpga.cpp


all: intra_build_cpu intra_build_fpga diag_build_cpu diag_build_fpga cond_build_cpu cond_build_fpga half_build_cpu half_build_fpga anti_build_cpu anti_build_fpga

intra_build_cpu:
	$(CXX) $(CXXFLAGS_CPU) -o $(INTRA_CPU_EXE) $(INTRA_CPU_SOURCES)
intra_build_fpga:
	$(CXX) $(CXXFLAGS_FPGA) -o $(INTRA_FPGA_EXE) $(INTRA_FPGA_SOURCES)

diag_build_cpu:
	$(CXX) $(CXXFLAGS_CPU) -o $(DIAG_CPU_EXE) $(DIAG_CPU_SOURCES)
diag_build_fpga:
	$(CXX) $(CXXFLAGS_FPGA) -o $(DIAG_FPGA_EXE) $(DIAG_FPGA_SOURCES)

cond_build_cpu:
	$(CXX) $(CXXFLAGS_CPU) -o $(COND_CPU_EXE) $(COND_CPU_SOURCES)
cond_build_fpga:
	$(CXX) $(CXXFLAGS_FPGA) -o $(COND_FPGA_EXE) $(COND_FPGA_SOURCES)

half_build_cpu:
	$(CXX) $(CXXFLAGS_CPU) -o $(HALF_CPU_EXE) $(HALF_CPU_SOURCES)
half_build_fpga:
	$(CXX) $(CXXFLAGS_FPGA) -o $(HALF_FPGA_EXE) $(HALF_FPGA_SOURCES)

anti_build_cpu:
	$(CXX) $(CXXFLAGS_CPU) -o $(ANTI_CPU_EXE) $(ANTI_CPU_SOURCES)

anti_build_fpga:
	$(CXX) $(CXXFLAGS_FPGA) -o $(ANTI_FPGA_EXE) $(ANTI_FPGA_SOURCES)

run: 
	./$(INTRA_CPU_EXE) && ./$(INTRA_FPGA_EXE) && ./$(DIAG_CPU_EXE) && ./$(DIAG_FPGA_EXE) && ./$(COND_CPU_EXE) && ./$(COND_FPGA_EXE) && ./$(HALF_CPU_EXE) && ./$(HALF_FPGA_EXE) && ./$(ANTI_CPU_EXE) && ./$(ANTI_FPGA_EXE)


clean: 
	rm -rf $(CPU_EXE_NAME) $(FPGA_EXE_NAME)
