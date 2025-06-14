################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./01_App/Source/main.obj" \
"./06_CSP/Source/f280013x_adc.obj" \
"./06_CSP/Source/f280013x_codestartbranch.obj" \
"./06_CSP/Source/f280013x_cputimers.obj" \
"./06_CSP/Source/f280013x_dbgier.obj" \
"./06_CSP/Source/f280013x_defaultisr.obj" \
"./06_CSP/Source/f280013x_epwm.obj" \
"./06_CSP/Source/f280013x_globalvariabledefs.obj" \
"./06_CSP/Source/f280013x_gpio.obj" \
"./06_CSP/Source/f280013x_piectrl.obj" \
"./06_CSP/Source/f280013x_pievect.obj" \
"./06_CSP/Source/f280013x_spi.obj" \
"./06_CSP/Source/f280013x_sysctrl.obj" \
"./06_CSP/Source/f280013x_tempsensorconv.obj" \
"./06_CSP/Source/f280013x_usdelay.obj" \
"../08_Cmd/280013x_generic_flash_lnk.cmd" \
"../08_Cmd/f280013x_headers_nonBIOS.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include 01_App/Source/subdir_vars.mk
-include 06_CSP/Source/subdir_vars.mk
-include 08_Cmd/subdir_vars.mk
-include 01_App/Source/subdir_rules.mk
-include 06_CSP/Source/subdir_rules.mk
-include 08_Cmd/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
P200W_ACDC_DSP_2800137_V1.000.out 

EXE_OUTPUTS__QUOTED += \
"P200W_ACDC_DSP_2800137_V1.000.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "P200W_ACDC_DSP_2800137_V1.000.out"

# Tool invocations
P200W_ACDC_DSP_2800137_V1.000.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"D:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -O3 --fp_mode=relaxed --define=CPU1 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi -z -m"P200W_ACDC_DSP_2800137_V1.000.map" --stack_size=0x100 --warn_sections -i"D:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/lib" -i"D:/ti/ccs2020/ccs/tools/compiler/ti-cgt-c2000_22.6.2.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="P200W_ACDC_DSP_2800137_V1.000_linkInfo.xml" --entry_point=code_start --rom_model -o "P200W_ACDC_DSP_2800137_V1.000.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "01_App\Source\main.obj" "06_CSP\Source\f280013x_adc.obj" "06_CSP\Source\f280013x_codestartbranch.obj" "06_CSP\Source\f280013x_cputimers.obj" "06_CSP\Source\f280013x_dbgier.obj" "06_CSP\Source\f280013x_defaultisr.obj" "06_CSP\Source\f280013x_epwm.obj" "06_CSP\Source\f280013x_globalvariabledefs.obj" "06_CSP\Source\f280013x_gpio.obj" "06_CSP\Source\f280013x_piectrl.obj" "06_CSP\Source\f280013x_pievect.obj" "06_CSP\Source\f280013x_spi.obj" "06_CSP\Source\f280013x_sysctrl.obj" "06_CSP\Source\f280013x_tempsensorconv.obj" "06_CSP\Source\f280013x_usdelay.obj" 
	-$(RM) "01_App\Source\main.d" "06_CSP\Source\f280013x_adc.d" "06_CSP\Source\f280013x_cputimers.d" "06_CSP\Source\f280013x_defaultisr.d" "06_CSP\Source\f280013x_epwm.d" "06_CSP\Source\f280013x_globalvariabledefs.d" "06_CSP\Source\f280013x_gpio.d" "06_CSP\Source\f280013x_piectrl.d" "06_CSP\Source\f280013x_pievect.d" "06_CSP\Source\f280013x_spi.d" "06_CSP\Source\f280013x_sysctrl.d" "06_CSP\Source\f280013x_tempsensorconv.d" 
	-$(RM) "06_CSP\Source\f280013x_codestartbranch.d" "06_CSP\Source\f280013x_dbgier.d" "06_CSP\Source\f280013x_usdelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

