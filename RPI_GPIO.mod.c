#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x70cf3864, "module_layout" },
	{ 0x12fb6301, "cdev_del" },
	{ 0x73e61f56, "cdev_init" },
	{ 0x6f1b5fd6, "device_destroy" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x2b4151d0, "phys_mem_access_prot" },
	{ 0xc5850110, "printk" },
	{ 0x4cb89471, "device_create" },
	{ 0x5cefef60, "cdev_add" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0x73fec2a0, "remap_pfn_range" },
	{ 0xe171d059, "class_destroy" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xd74cab92, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x9c6febfc, "add_uevent_var" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "EE8A4CD44830D59580A0164");
