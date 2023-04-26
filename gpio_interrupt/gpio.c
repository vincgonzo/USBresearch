#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vincent Castanié");
MODULE_DESCRIPTION("A simple LKM for gpio interrupt");


/** Buffer for data */
static char buffer[255];
static int buffer_pointer;


/* variable contains pon number o interrupt controller to which GPIO 17 is mapped to */
unsigned int  irq_number;
static irq_handler_t gpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs){
	printk("gpio_irq: Interrupt was trigger and ISR was called!\n");
	return (irq_handler_t) IRQ_HANDLED;
}

/**
 *  * @brief This function is called, when the module is loaded into the kernel
 *   */
static int __init ModuleInit(void) {
	printk("gpio irq Loading module ...");

	/* GPIO 17 init */
	if(gpio_request(17, "rpi-gpio-17")) {
		printk("Can not allocate GPIO 17\n");
		return -1;
	}

	/* Set GPIO 17 direction */
	if(gpio_direction_input(17)) {
		printk("Can not set GPIO 17 to input!\n");
		gpio_free(17);
		return -1;
	}

	irq_number = gpio_to_irq(17);

	if(request_irq(irq_number, (irq_handler_t) gpio_irq_handler, IRQF_TRIGGER_RISING, "my_gpio_irq", NULL) != 0){
		printk("Error: can not request interrupt  nbr; %d\n", irq_number);
		gpio_free(17);
		return -1;
	}

	printk("Done !\n");
	printk("GPIO 17 is mapped to IRQ nbr : %d\n", irq_number);
	return 0;

Gpio17Error:
	gpio_free(17);
	return -1;
}

/**
 *  * @brief This function is called, when the module is removed from the kernel
 *   */
static void __exit ModuleExit(void) {
	printk("gpio_irq : unloading module ...\n");
	free_irq(irq_number, NULL);
	gpio_free(17);
	printk("gpio_irq : unloading done\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

