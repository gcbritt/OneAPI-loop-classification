CXX = dpcpp
CXXFLAGS_CPU = -O2 -g -std=c++17 --gcc-toolchain=/home/user/gcbritt/GCC-7.4.0/ -B /home/user/gcbritt/GCC-7.4.0/lib/gcc/x86_64-pc-linux-gnu/7.4.0/ -B /home/user/gcbritt/intel/inteloneapi/compiler/2021.1-beta06/linux/lib/oclfpga/bin/ -L /home/user/gcbritt/GCC-7.4.0/lib/gcc/x86_64-pc-linux-gnu/7.4.0/ -isystem /home/user/gcbritt/GCC-7.4.0/include/c++/7.4.0/ -isystem /home/user/gcbritt/GCC-7.4.0/include/c++/7.4.0/x86_64-pc-linux-gnu/ -L /home/user/gcbritt/GCC-7.4.0/lib64/

CXXFLAGS_FPGA = -O2 -g -fintelfpga -std=c++17 --gcc-toolchain=/home/user/gcbritt/GCC-7.4.0/ -B /home/user/gcbritt/GCC-7.4.0/lib/gcc/x86_64-pc-linux-gnu/7.4.0/ -B /home/user/gcbritt/intel/inteloneapi/compiler/2021.1-beta06/linux/lib/oclfpga/bin/ -L /home/user/gcbritt/GCC-7.4.0/lib/gcc/x86_64-pc-linux-gnu/7.4.0/ -isystem /home/user/gcbritt/GCC-7.4.0/include/c++/7.4.0/ -isystem /home/user/gcbritt/GCC-7.4.0/include/c++/7.4.0/x86_64-pc-linux-gnu/ -L /home/user/gcbritt/GCC-7.4.0/lib64/

CXXFLAGS_HW = -O2 -g -v -fsycl-unnamed-lambda -fintelfpga -fsycl-link=early -Xshardware -std=c++17 --gcc-toolchain=/home/user/gcbritt/GCC-7.4.0/ -B /home/user/gcbritt/GCC-7.4.0/lib/gcc/x86_64-pc-linux-gnu/7.4.0/ -B /home/tools/altera/20.1-pro/hld/bin -L /home/user/gcbritt/GCC-7.4.0/lib/gcc/x86_64-pc-linux-gnu/7.4.0/ -isystem /home/user/gcbritt/GCC-7.4.0/include/c++/7.4.0/ -isystem /home/user/gcbritt/GCC-7.4.0/include/c++/7.4.0/x86_64-pc-linux-gnu/ -L /home/user/gcbritt/GCC-7.4.0/lib64/ -Xs -board-package=/home/tools/altera/20.1-pro/hld/board/a10_ref/


CPU_EXE_NAME = out/conditional_dependent_cpu.out
FPGA_EXE_NAME = out/conditional_dependent_fpga.out
HW_EXE_NAME = out/conditional_dependent_hw.out

HW_SOURCES = src/conditional_dependent_hw.cpp
CPU_SOURCES = src/conditional_dependent_cpu.cpp
FPGA_SOURCES = src/conditional_dependent_fpga.cpp

all: build_cpu build_fpga

hw: build_hw

build_hw:
	$(CXX) $(CXXFLAGS_HW) -o $(HW_EXE_NAME) $(HW_SOURCES)

build_cpu:
	$(CXX) $(CXXFLAGS_CPU) -o $(CPU_EXE_NAME) $(CPU_SOURCES)

build_fpga:
	$(CXX) $(CXXFLAGS_FPGA) -o $(FPGA_EXE_NAME) $(FPGA_SOURCES)

run: 
	./$(CPU_EXE_NAME) && ./$(FPGA_EXE_NAME)
runhw:
	./$(HW_EXE_NAME)


clean: 
	rm -rf $(CPU_EXE_NAME) $(FPGA_EXE_NAME)
