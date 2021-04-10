#include "../../include/loops_inc.h" // defines LEN, ntimes, init(), check(), includes headers related to oneAPI. defines arrays used in testing.

/*
 * run conditional dependent loop on accelerator, report time taken and array checksum value
 */
int s124()
{
	init("s124 "); // initialize the arrays with values for conditional dependent loop
	
	clock_t start_t, end_t, clock_dif; double clock_dif_sec; // clocks used to time the execution of the kernel
	
	constexpr int a_len = LEN, b_len = LEN, c_len = LEN, d_len = LEN, e_len = LEN; // the length of each array should be defined at compile time
	
	auto a_range = range<1>(a_len);
	auto b_range = range<1>(b_len);
	auto c_range = range<1>(c_len); 
	auto d_range = range<1>(d_len);
	auto e_range = range<1>(e_len); 
	auto ntimes_range = range<1>(ntimes); // create 1-D ranges that buffers will act over
	
	
	queue q(intel::fpga_selector{}); // define a queue to run the loop on an fpga_emulator device
	
	buffer<TYPE, 1> buf_a(&a[0], a_range);
	buffer<TYPE, 1> buf_b(&b[0], b_range);
	buffer<TYPE, 1> buf_c(&c[0], c_range);
	buffer<TYPE, 1> buf_d(&d[0], d_range);
	buffer<TYPE, 1> buf_e(&e[0], e_range); // buffers used to control kernel access to the host arrays	

	start_t = clock(); // save start time just as the kernel is submitted

	// submit a kernel to accelerator
	q.submit([&](handler &h)
	{
		auto accessor_a = buf_a.get_access<dp_rw>(h);
		auto accessor_b = buf_b.get_access<dp_r>(h);
		auto accessor_c = buf_c.get_access<dp_r>(h);
		auto accessor_d = buf_d.get_access<dp_r>(h);
		auto accessor_e = buf_e.get_access<dp_r>(h); // obtain r/w access to a, but only need to read b - e
		
		// conditional dependent loop
		h.parallel_for(ntimes_range, [=](id<1> n)
		{
			int j = -1;

			for(int i = 0; i < LEN; i++)
			{
				if (accessor_b[i] > (TYPE)0.) 
				{
					j++;
					accessor_a[j] = accessor_b[i] + accessor_d[i] * accessor_e[i];
				} 
				else 
				{
					j++;
					accessor_a[j] = accessor_c[i] + accessor_d[i] * accessor_e[i];
				}
			}
			
			dummy(a, b, c, d, e, aa, bb, cc, 0.); // check arrays for corruption
		});
	});
	

	auto ans = buf_a.get_access<dp_r>(); // ask access to the buffer for array a. this forces host to wait for kernel to execute.
	

	end_t = clock(); clock_dif = end_t - start_t; // save end time, compute time diff
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S124\t %.2f \t\t", clock_dif_sec); // report time taken
	check(1); // report checksum
	return 0;
}

/*
 * run s124()
 */
int main()
{
	std::cout << "Testing conditional dependent loop on fpga emulator device..." << std::endl;
	s124(); // test conditional loop time
	std::cout << "Test finished\n\n" << std::endl;
	
	return 0;
}
