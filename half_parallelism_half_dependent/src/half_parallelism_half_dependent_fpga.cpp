#include "../../include/loops_inc.h" // defines LEN, ntimes, init(), check(), defines arrays used in testing. includes headers related to oneAPI

/*
 * run half parallelism half dependent loop on accelerator device, report time taken and report array checksum
 */
int s221()
{
	init("s221 "); // initialize arrays with the values for half parallelism half dependency loop
	
	clock_t start_t, end_t, clock_dif; double clock_dif_sec; // clocks used to time kernel execution
	
	constexpr int a_len = LEN, b_len = LEN, c_len = LEN, d_len = LEN; // the length of each array should be defined at compile time
	
	auto a_range = range<1>(a_len);
	auto b_range = range<1>(b_len);
	auto c_range = range<1>(c_len);
	auto d_range = range<1>(d_len);
	auto ntimes_range = range<1>(ntimes/2); // create 1-D ranges that buffers will act over
	
	
	queue q(intel::fpga_emulator_selector{}); // define a queue to run the loop on an fpga emulator device
	
	buffer<TYPE, 1> buf_a(&a[0], a_range);
	buffer<TYPE, 1> buf_b(&b[0], b_range); 
	buffer<TYPE, 1> buf_c(&c[0], c_range); 
	buffer<TYPE, 1> buf_d(&d[0], d_range); // buffers used to control access to the arrays	

	start_t = clock(); // start time saved just as kernel is submitted

	// submit kernel to accelerator
	q.submit([&](handler &h)
	{
		auto accessor_a = buf_a.get_access<dp_rw>(h);
		auto accessor_b = buf_b.get_access<dp_rw>(h);
		auto accessor_c = buf_c.get_access<dp_r>(h);
		auto accessor_d = buf_d.get_access<dp_r>(h); // obtain r/w access to a & b, but only need to read c & d
	
		// half parallelism half dependent loop. run parallel from idx = 0 to idx = ntimes/2 (the range of ntimes_range)	
		h.parallel_for(ntimes_range, [=](id<1> n)
		{
			for(int i = 1; i < LEN; i++)
			{
				accessor_a[i] += accessor_c[i] * accessor_d[i];
				accessor_b[i] = accessor_b[i - 1] + accessor_a[i] + accessor_d[i];	
			}
			
			dummy(a, b, c, d, e, aa, bb, cc, 0.); // check arrays for corruption
		});
	});
	

	auto ans_a = buf_a.get_access<dp_r>(); 
	auto ans_b = buf_b.get_access<dp_r>(); // ask access to the buffer for array a & b. this forces host to wait for kernel to execute.
	

	end_t = clock(); clock_dif = end_t - start_t; // save end time, compute time diff
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S221\t %.2f \t\t", clock_dif_sec); // report time taken
	check(12); // compute and report checksum
	return 0;
}

/*
 * run s221()
 */
int main()
{
	std::cout << "Testing half-parallelism half-dependent loop on fpga emulator device..." << std::endl;
	s221(); // test half-parallelism half-dependent loop time
	std::cout << "Test finished\n\n" << std::endl;
	
	return 0;
}
