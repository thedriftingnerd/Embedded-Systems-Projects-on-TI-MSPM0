################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
build-1846057121: ../empty.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"/home/mihir/CMPE146/CCS_20.2.0.00012_linux/ccs2020/ccs/utils/sysconfig_1.24.0/sysconfig_cli.sh" --script "/home/mihir/workspace_ccstheia/Lab2-exercise5/empty.syscfg" -o "." -s "/home/mihir/ti/mspm0_sdk_2_05_01_00/.metadata/product.json" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-1846057121 ../empty.syscfg
device.opt: build-1846057121
device.cmd.genlibs: build-1846057121
ti_msp_dl_config.c: build-1846057121
ti_msp_dl_config.h: build-1846057121

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/mihir/CMPE146/CCS_20.2.0.00012_linux/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"/home/mihir/workspace_ccstheia/Lab2-exercise5" -I"/home/mihir/workspace_ccstheia/Lab2-exercise5/Debug" -I"/home/mihir/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"/home/mihir/ti/mspm0_sdk_2_05_01_00/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  @"./device.opt" -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: /home/mihir/ti/mspm0_sdk_2_05_01_00/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/mihir/CMPE146/CCS_20.2.0.00012_linux/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"/home/mihir/workspace_ccstheia/Lab2-exercise5" -I"/home/mihir/workspace_ccstheia/Lab2-exercise5/Debug" -I"/home/mihir/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"/home/mihir/ti/mspm0_sdk_2_05_01_00/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/mihir/CMPE146/CCS_20.2.0.00012_linux/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"/home/mihir/workspace_ccstheia/Lab2-exercise5" -I"/home/mihir/workspace_ccstheia/Lab2-exercise5/Debug" -I"/home/mihir/ti/mspm0_sdk_2_05_01_00/source/third_party/CMSIS/Core/Include" -I"/home/mihir/ti/mspm0_sdk_2_05_01_00/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


