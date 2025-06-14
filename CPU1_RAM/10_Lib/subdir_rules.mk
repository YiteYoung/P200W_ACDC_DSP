################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
10_Lib/%.obj: ../10_Lib/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/empty_bitfield_project" --include_path="D:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/empty_bitfield_project/10_Lib" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/empty_bitfield_project/09_Protocol/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/empty_bitfield_project/08_Cmd/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/empty_bitfield_project/07_Driver/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/empty_bitfield_project/06_CSP/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/empty_bitfield_project/05_BSP/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/empty_bitfield_project/04_Boot/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/empty_bitfield_project/03_Task/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/empty_bitfield_project/02_Ctr/Include" --include_path="D:/Yite/PPS/P200W_ACDC_DSP_2800137/empty_bitfield_project/01_App/Include" --define=CPU1 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="10_Lib/$(basename $(<F)).d_raw" --obj_directory="10_Lib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


