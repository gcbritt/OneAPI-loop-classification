#include "../../include/loops_inc.h" // defines LEN, ntimes, check(), init(), includes header files used by oneAPI

/*
 * runs anti dependency loop on kernel device, reports time taken and checksum
 */
int s211()
{
	init("s211 "); // initialize arrays with data for the anti-dependent loop
	
	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	
	constexpr int a_len = LEN, b_len = LEN, c_len = LEN, d_len = LEN, e_len = LEN; // the length of each array should be defined at compile time
	
	auto a_range = range<1>(a_len);
	auto b_range = range<1>(b_len);
	auto c_range = range<1>(c_len);
	auto d_range = range<1>(d_len);
	auto e_range = range<1>(e_len);
	auto ntimes_range = range<1>(ntimes); // create 1-D ranges that buffers will act over
	
	
	queue q(intel::fpga_emulator_selector{}); // define a queue to run the loop on an fpga_emulator device
	
	buffer<TYPE, 1> buf_a(&a[0], a_range);
	buffer<TYPE, 1> buf_b(&b[0], b_range); 
	buffer<TYPE, 1> buf_c(&c[0], c_range); 
	buffer<TYPE, 1> buf_d(&d[0], d_range); 
 	buffer<TYPE, 1> buf_e(&e[0], e_range); // buffers used to control kernel access to the host arrays	

	start_t = clock(); // save the start time just as the kernel is submitted

	// define a kernel and submit it to an accelerator using lambda
	q.submit([&](handler &h)
	{
		auto accessor_a = buf_a.get_access<dp_rw>(h);
		auto accessor_b = buf_b.get_access<dp_rw>(h);
		auto accessor_c = buf_c.get_access<dp_r>(h);
		auto accessor_d = buf_d.get_access<dp_r>(h);
		auto accessor_e = buf_e.get_access<dp_r>(h); // obtain r/w access to a & b, but only need to read c - e
							     // accelerator device asks host for access to the arrays
		// anti-dependent loop
		h.parallel_for(ntimes_range, [=](id<1> n)
		{
			for(int i = 1; i < LEN - 1; i++)
			{
				accessor_a[i] = accessor_b[i - 1] + accessor_c[i] * accessor_d[i];
				accessor_b[i] = accessor_b[i + 1] - accessor_e[i] * accessor_d[i];
			}
			
			dummy(a, b, c, d, e, aa, bb, cc, 0.); // check to make sure array memory isnt corrupted
		});
	});
	

	auto ans_a = buf_a.get_access<dp_r>(); 
	auto ans_b = buf_b.get_access<dp_r>(); // ask access to the buffer for array a & b. this forces host to wait for kernel to execute.
	

	end_t = clock(); clock_dif = end_t - start_t; // save end time, compute time diff
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S211\t %.2f \t\t", clock_dif_sec); // report time result
	check(12); // report checksum
	return 0;
}

/*
 * run s211()
 */
int main()
{
	std::cout << "Testing anti-dependent loop on fpga emulation device..." << std::endl;
	s211(); // test anti-dependent loop time
	std::cout << "Test finished\n\n" << std::endl;
	
	return 0;
}
