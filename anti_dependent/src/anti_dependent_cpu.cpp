#include "../../include/loops_inc.h" // defines LEN, ntimes, init(), check(), includes all oneAPI related headers
/*
 * runs anti-dependency loop on accelerator, reports time taken and checksum
 */
int s211()
{
	init("s211 "); // initialize arrays with values for s211() which is the anti-dependent loop
	
	clock_t start_t, end_t, clock_dif; double clock_dif_sec; // clocks used to time how long it takes for the loop to execute
	
	constexpr int a_len = LEN, b_len = LEN, c_len = LEN, d_len = LEN, e_len = LEN; // the length of each array should be defined at compile time
	
	auto a_range = range<1>(a_len);
	auto b_range = range<1>(b_len);
	auto c_range = range<1>(c_len);
	auto d_range = range<1>(d_len);
	auto e_range = range<1>(e_len);
	auto ntimes_range = range<1>(ntimes); // create 1-D ranges that buffers will act over
	
	
	queue q(cpu_selector{}); // define a queue to run the loop on a cpu device
	
	buffer<TYPE, 1> buf_a(&a[0], a_range);
	buffer<TYPE, 1> buf_b(&b[0], b_range); 
	buffer<TYPE, 1> buf_c(&c[0], c_range); 
	buffer<TYPE, 1> buf_d(&d[0], d_range); 
 	buffer<TYPE, 1> buf_e(&e[0], e_range); // buffers used to control kernel access to the arrays	

	start_t = clock(); // start time saved just as the kernel is submitted

	q.submit([&](handler &h)
	{
		auto accessor_a = buf_a.get_access<dp_rw>(h);
		auto accessor_b = buf_b.get_access<dp_rw>(h);
		auto accessor_c = buf_c.get_access<dp_r>(h);
		auto accessor_d = buf_d.get_access<dp_r>(h);
		auto accessor_e = buf_e.get_access<dp_r>(h); // obtain r/w access to a & b, but only need to read c - e.
							     // accessors must be used to share host array memory with kernel device.
		
		// kernel that will run in parallel. loop modeled from TSVC anti-dependency loop
		h.parallel_for(ntimes_range, [=](id<1> n)
		{
			for(int i = 1; i < LEN - 1; i++)
			{
				accessor_a[i] = accessor_b[i - 1] + accessor_c[i] * accessor_d[i];
				accessor_b[i] = accessor_b[i + 1] - accessor_e[i] * accessor_d[i];
			}
			
			dummy(a, b, c, d, e, aa, bb, cc, 0.); // check to make sure arrays aren't corrupted
		});
	});
	

	auto ans_a = buf_a.get_access<dp_r>(); 
	auto ans_b = buf_b.get_access<dp_r>(); // ask access to the buffer for array a & b. this forces host to wait here for kernel to execute.
	

	end_t = clock(); clock_dif = end_t - start_t; // save end time once the kernel is done executing, compute time diff
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S211\t %.2f \t\t", clock_dif_sec); // print time diff
	check(12); // compute and print array checksum
	return 0;
}

/*
 * run the s211() test
 */
int main()
{
	std::cout << "Testing anti-dependent loop on cpu..." << std::endl;
	s211(); // test anti-dependent loop time
	std::cout << "Test finished\n\n" << std::endl;
	
	return 0;
}
