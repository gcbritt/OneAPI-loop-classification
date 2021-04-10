#include "../../include/loops_inc.h" // defines LEN2, ntimes, init(), check(), defines arrays used in testing. includes headers related to oneAPI

/*
 * runs intra dimension dependent loop on accelerator, reports time taken and array checksum
 */
int s1119()
{
	init("s1119 "); // initialize arrays for this specific test
	
	clock_t start_t, end_t, clock_dif; double clock_dif_sec; // clocks used to time kernel execution
	
	constexpr int ax_len = LEN2, ay_len = LEN2, bx_len = LEN2, by_len = LEN2; // the length of each array should be defined at compile time
	
	auto aa_range = range<2>(ax_len, ay_len);
	auto bb_range = range<2>(bx_len, by_len); // 2-D ranges that the array buffers will act over, aa and bb are 2-D arrays 
	auto ntimes_range = range<1>(200*(ntimes/(LEN2))); // range that the loop will execute over in parallel
	
	
	queue q(intel::fpga_emulator_selector{}); // define a queue to run the loop on an fpga_emulator device
	
	buffer<TYPE, 2> buf_aa(&aa[0][0], aa_range);
	buffer<TYPE, 2> buf_bb(&bb[0][0], bb_range); // buffers used to control access to the arrays	

	start_t = clock(); // start time saved just as kernel is submitted
	
	// submit kernel to accelerator
	q.submit([&](handler &h)
	{
		auto accessor_aa = buf_aa.get_access<dp_rw>(h);
		auto accessor_bb = buf_bb.get_access<dp_r>(h); // obtain r/w access to a, but only need to read b - e
	
		// intra-dimension dependent loop executed in parallel over the range idx = 0 to idx = 200*(ntimes/LEN2)	
		h.parallel_for(ntimes_range, [=](id<1> n)
		{
			for(int i = 1; i < LEN2; i++)
				for(int j = 0; j < LEN2; j++)
				{
					accessor_aa[i][j] = accessor_aa[i-1][j] + accessor_bb[i][j];
				}
			
			dummy(a, b, c, d, e, aa, bb, cc, 0.); // check arrays for corruption
		});
	});
	

	auto ans = buf_aa.get_access<dp_r>(); // ask access to the buffer for array a. this forces host to wait for kernel to execute.
	

	end_t = clock(); clock_dif = end_t - start_t; // save end time, compute time diff
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S1119\t %.2f \t\t", clock_dif_sec); // report time taken
	check(11); // compute and report checksum
	return 0;
}

/*
 * run s1119()
 */
int main()
{
	std::cout << "Testing intra-dimension dependent loop on fpga emulator device..." << std::endl;
	s1119(); // test diagonal loop time
	std::cout << "Test finished\n\n" << std::endl;
	
	return 0;
}
