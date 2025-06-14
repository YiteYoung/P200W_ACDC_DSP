################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
02_Ctr/Source/%.obj: ../02_Ctr/Source/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP_2800137_V1.000/09_Protocol/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP_2800137_V1.000/08_Cmd" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP_2800137_V1.000/07_Driver/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP_2800137_V1.000/06_CSP/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP_2800137_V1.000/05_BSP/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP_2800137_V1.000/04_Boot/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP_2800137_V1.000/03_Task/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP_2800137_V1.000/02_Ctr/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP_2800137_V1.000/01_App/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/P200W_ACDC_DSP_2800137_V1.000" --include_path="C:/TI/C2000Ware_5_04_00_00/device_support/f280013x/common/include/" --include_path="C:/TI/C2000Ware_5_04_00_00/device_support/f280013x/headers/include/" --include_path="D:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --define=CPU1 --define=_FLASH --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="02_Ctr/Source/$(basename $(<F)).d_raw" --obj_directory="02_Ctr/Source" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


