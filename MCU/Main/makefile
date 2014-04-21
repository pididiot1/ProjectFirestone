################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/msp430_4.2.1

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./application/vlo_rand.obj" \
"./application/main_rx.obj" \
"./bsp/bsp.obj" \
"./mrfi/mrfi.obj" \
"./nwk/nwk_globals.obj" \
"./nwk/nwk_frame.obj" \
"./nwk/nwk_api.obj" \
"./nwk/nwk_QMgmt.obj" \
"./nwk/nwk.obj" \
"./nwk applications/nwk_security.obj" \
"./nwk applications/nwk_ping.obj" \
"./nwk applications/nwk_mgmt.obj" \
"./nwk applications/nwk_link.obj" \
"./nwk applications/nwk_join.obj" \
"./nwk applications/nwk_ioctl.obj" \
"./nwk applications/nwk_freq.obj" \
"../lnk_cc430f5135.cmd" \
-l"libmath.a" \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include nwk\ applications/subdir_vars.mk
-include nwk/subdir_vars.mk
-include mrfi/subdir_vars.mk
-include bsp/subdir_vars.mk
-include application/subdir_vars.mk
-include subdir_rules.mk
-include nwk\ applications/subdir_rules.mk
-include nwk/subdir_rules.mk
-include mrfi/subdir_rules.mk
-include bsp/subdir_rules.mk
-include application/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: MSP430_LP.out

# Tool invocations
MSP430_LP.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" --cmd_file="C:\Users\Adam\Documents\GitHub\ProjectFirestone\MCU\configuration\smpl_nwk_config.dat" --cmd_file="C:\Users\Adam\Documents\GitHub\ProjectFirestone\MCU\end device\smpl_config.dat"  -vmsp --abi=eabi --advice:power="all" -g --define=__CC430F5135__ --define=MRFI_CC430 --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal -z -m"MSP430_LP.map" --heap_size=80 --stack_size=80 --use_hw_mpy=F5 -i"C:/ti/ccsv5/ccs_base/msp430/include" -i"C:/ti/ccsv5/tools/compiler/msp430_4.2.1/lib" -i"C:/ti/ccsv5/tools/compiler/msp430_4.2.1/include" -i"C:/ti/ccsv5/ccs_base/msp430/lib" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="MSP430_LP_linkInfo.xml" --rom_model -o "MSP430_LP.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(MSP430_EXECUTABLE_OUTPUTS__QUOTED) "MSP430_LP.out"
	-$(RM) "nwk applications\nwk_freq.pp" "nwk applications\nwk_ioctl.pp" "nwk applications\nwk_join.pp" "nwk applications\nwk_link.pp" "nwk applications\nwk_mgmt.pp" "nwk applications\nwk_ping.pp" "nwk applications\nwk_security.pp" "nwk\nwk.pp" "nwk\nwk_QMgmt.pp" "nwk\nwk_api.pp" "nwk\nwk_frame.pp" "nwk\nwk_globals.pp" "mrfi\mrfi.pp" "bsp\bsp.pp" "application\main_rx.pp" 
	-$(RM) "nwk applications\nwk_freq.obj" "nwk applications\nwk_ioctl.obj" "nwk applications\nwk_join.obj" "nwk applications\nwk_link.obj" "nwk applications\nwk_mgmt.obj" "nwk applications\nwk_ping.obj" "nwk applications\nwk_security.obj" "nwk\nwk.obj" "nwk\nwk_QMgmt.obj" "nwk\nwk_api.obj" "nwk\nwk_frame.obj" "nwk\nwk_globals.obj" "mrfi\mrfi.obj" "bsp\bsp.obj" "application\main_rx.obj" "application\vlo_rand.obj" 
	-$(RM) "application\vlo_rand.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

