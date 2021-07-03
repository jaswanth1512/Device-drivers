#include<linux/module.h>
static int  __init helloworld_init(void)
{
	pr_info("Hello world\n");
	return 0;
}
static void __exit helloworld_clean(void)
{
	pr_info("Bye World\n");
}

module_init(helloworld_init);
module_exit(helloworld_clean);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jaswanth");
MODULE_DESCRIPTION("basic hello world program");
MODULE_INFO(PC,"Linux PC");
