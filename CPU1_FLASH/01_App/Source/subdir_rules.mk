################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
01_App/Source/%.obj: ../01_App/Source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 -Ooff --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP/09_Protocol/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP/08_Cmd" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP/07_Driver/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP/06_CSP/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP/05_BSP/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP/04_Boot/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP/03_Task/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP/02_Ctr/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP/01_App/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP" --include_path="C:/TI/C2000Ware_5_04_00_00/device_support/f280013x/common/include/" --include_path="C:/TI/C2000Ware_5_04_00_00/device_support/f280013x/headers/include/" --include_path="D:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --define=CPU1 --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="01_App/Source/$(basename $(<F)).d_raw" --obj_directory="01_App/Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


