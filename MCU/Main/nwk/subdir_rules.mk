################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
nwk/nwk.obj: C:/Texas\ Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" --cmd_file="C:\Users\Adam\Documents\GitHub\ProjectFirestone\MCU\configuration\smpl_nwk_config.dat" --cmd_file="C:\Users\Adam\Documents\GitHub\ProjectFirestone\MCU\end device\smpl_config.dat"  -vmsp --abi=eabi --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp/boards/CC430EM" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp/drivers" --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/mrfi" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk_applications" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power="all" -g --define=__CC430F5135__ --define=MRFI_CC430 --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="nwk/nwk.pp" --obj_directory="nwk" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

nwk/nwk_QMgmt.obj: C:/Texas\ Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_QMgmt.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" --cmd_file="C:\Users\Adam\Documents\GitHub\ProjectFirestone\MCU\configuration\smpl_nwk_config.dat" --cmd_file="C:\Users\Adam\Documents\GitHub\ProjectFirestone\MCU\end device\smpl_config.dat"  -vmsp --abi=eabi --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp/boards/CC430EM" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp/drivers" --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/mrfi" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk_applications" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power="all" -g --define=__CC430F5135__ --define=MRFI_CC430 --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="nwk/nwk_QMgmt.pp" --obj_directory="nwk" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

nwk/nwk_api.obj: C:/Texas\ Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_api.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" --cmd_file="C:\Users\Adam\Documents\GitHub\ProjectFirestone\MCU\configuration\smpl_nwk_config.dat" --cmd_file="C:\Users\Adam\Documents\GitHub\ProjectFirestone\MCU\end device\smpl_config.dat"  -vmsp --abi=eabi --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp/boards/CC430EM" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp/drivers" --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/mrfi" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk_applications" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power="all" -g --define=__CC430F5135__ --define=MRFI_CC430 --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="nwk/nwk_api.pp" --obj_directory="nwk" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

nwk/nwk_frame.obj: C:/Texas\ Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_frame.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" --cmd_file="C:\Users\Adam\Documents\GitHub\ProjectFirestone\MCU\configuration\smpl_nwk_config.dat" --cmd_file="C:\Users\Adam\Documents\GitHub\ProjectFirestone\MCU\end device\smpl_config.dat"  -vmsp --abi=eabi --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp/boards/CC430EM" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp/drivers" --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/mrfi" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk_applications" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power="all" -g --define=__CC430F5135__ --define=MRFI_CC430 --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="nwk/nwk_frame.pp" --obj_directory="nwk" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

nwk/nwk_globals.obj: C:/Texas\ Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk/nwk_globals.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" --cmd_file="C:\Users\Adam\Documents\GitHub\ProjectFirestone\MCU\configuration\smpl_nwk_config.dat" --cmd_file="C:\Users\Adam\Documents\GitHub\ProjectFirestone\MCU\end device\smpl_config.dat"  -vmsp --abi=eabi --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp/boards/CC430EM" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp/drivers" --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/bsp" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/mrfi" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk" --include_path="C:/Texas Instruments/SimpliciTI-CCS-1.1.1/Components/simpliciti/nwk_applications" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --advice:power="all" -g --define=__CC430F5135__ --define=MRFI_CC430 --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="nwk/nwk_globals.pp" --obj_directory="nwk" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


