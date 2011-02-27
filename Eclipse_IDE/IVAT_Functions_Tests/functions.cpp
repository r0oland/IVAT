
//---------> Modulator <------------------------
/*takes array with time in seconds and compute days, hours...
time[]=={days, hours, minutes, seconds}*/
void modulator(unsigned long* time)
{
	unsigned long seconds[] =
	{ 86400, 3600, 60 };

	for (int i = 1; i <= 4; i++)
	{
		time[i] = time[i - 1] % seconds[i - 1];
	}

	for (int n = 3; n > 0; n--)
	{
		time[n] = time[n - 1] / seconds[n - 1];
	}
}
