#include <stdio.h>
int main()
{
	FILE * fp = fopen("input", "r");
	char t ;int chk;
	for(;;)
	{
		chk =	fscanf(fp, "%c", &t);
		if(chk<0)
			break;
		printf("\'%c\': \'%d\'\n",t, t);
	}
		return 0;
}
