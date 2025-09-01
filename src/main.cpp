#include "GB/CPU.h"

int main()
{
	CPU Cpu;

	int retVal = 1;

	while (retVal == 1)
	{
		retVal = Cpu.Step();
	}
}