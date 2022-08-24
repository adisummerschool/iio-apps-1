#include <stdio.h>
#include <iio.h>
#include <unistd.h>
struct iio_context *ctx = NULL;
struct iio_device *dev = NULL;
struct iio_channel *chan [4]; //define a struct array

#define URI "ip:10.76.84.217"
#define DEVICE_NAME "ad5592r"

int main() {
    char *p;
    ctx = iio_create_context_from_uri(URI) ;
    if(ctx == NULL){
	printf ("No context \n");
	return -1;
    }
    p = iio_context_get_attr_value(ctx,"hw_carrier");

    int nr_ctx_attr;
    nr_ctx_attr = iio_context_get_attrs_count(ctx);
    printf ("%d context atributes found \n",nr_ctx_attr);



    for(int i = 0; i < nr_ctx_attr; i++){
	char *name;
	char *value;
	iio_context_get_attr(ctx,i,&name,&value);
	printf ("ctxattr[%d] - name:%lx %s - value%:lx %s",i,name,name,value,value);
	printf("\n");
    }

    dev = iio_context_find_device(ctx,DEVICE_NAME);
    if(dev == NULL){
	printf ("No device \n");
	return -1;
    }
    printf ("Got device %s %lx\n",DEVICE_NAME,dev);


    unsigned int chan_count = iio_device_get_channels_count(dev);// determin the numbers of channels
    for(int i = 0 ; i < chan_count; i++){
	chan[i] = iio_device_get_channel(dev,i);//
	if(chan[i] == NULL){
	    printf ("Channel not found\n");
	    return -1;
	}
	printf("Got channel %s %s %lx \n",iio_channel_get_id(chan[i]),chan[i]);
    }

    int values[4];
    int x,y;
    while(1){
	for(int i = 0; i<chan_count;i++){
	    char buf[10];
	    iio_channel_attr_read((chan[i]),"raw",&buf,10);
	    values[i]=atoi(buf);
	}
	x=values[0]-values[1];
	y=values[2]-values[3];
	printf("x= %5d       y=%5d\n",x,y);
	fflush(stdout);
	usleep( 500*1000 );
    }
	return 0;
}