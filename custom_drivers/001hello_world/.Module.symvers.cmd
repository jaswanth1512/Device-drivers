cmd_/home/jaswanth/Jaswanth/ldd/Device-drivers/custom_drivers/001hello_world/Module.symvers := sed 's/ko$$/o/' /home/jaswanth/Jaswanth/ldd/Device-drivers/custom_drivers/001hello_world/modules.order | scripts/mod/modpost -m -a   -o /home/jaswanth/Jaswanth/ldd/Device-drivers/custom_drivers/001hello_world/Module.symvers -e -i Module.symvers   -T -
