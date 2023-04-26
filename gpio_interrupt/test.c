#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>


int main(){
	int dev = open("/dev/my_dev_nr", O_RDONLY);
	if(dev == -1){
		printf("Opening was not possible!!\n");
		return -1;
	}
	printf("Opening succesfully the device!!\n");
	close(dev);
	return 0;
}
