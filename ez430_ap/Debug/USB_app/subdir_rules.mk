################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
USB_app/usbConstructs.obj: ../USB_app/usbConstructs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O3 --opt_for_speed=2 --use_hw_mpy=F5 --include_path="/Applications/ti/ccsv7/ccs_base/msp430/include" --include_path="/Users/asterix/src/msp430wave_radio_module/ez430_ap/driverlib/MSP430F5xx_6xx" --include_path="/Users/asterix/src/msp430wave_radio_module/ez430_ap" --include_path="/Users/asterix/src/msp430wave_radio_module/ez430_ap/USB_config" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__MSP430F5509__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="USB_app/usbConstructs.d" --obj_directory="USB_app" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

USB_app/usbEventHandling.obj: ../USB_app/usbEventHandling.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/bin/cl430" -vmspx -O3 --opt_for_speed=2 --use_hw_mpy=F5 --include_path="/Applications/ti/ccsv7/ccs_base/msp430/include" --include_path="/Users/asterix/src/msp430wave_radio_module/ez430_ap/driverlib/MSP430F5xx_6xx" --include_path="/Users/asterix/src/msp430wave_radio_module/ez430_ap" --include_path="/Users/asterix/src/msp430wave_radio_module/ez430_ap/USB_config" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-msp430_16.9.0.LTS/include" --advice:power="none" --define=DEPRECATED --define=__MSP430F5509__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="USB_app/usbEventHandling.d" --obj_directory="USB_app" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


