#ifndef _CONFIG_INV_H
#define _CONFIG_INV_H

#ifdef CONFIG_INV_FUNCTION_G
#define CONFIG_INV_FUNCTION extern
#else
#define CONFIG_INV_FUNCTION
#endif

#define cCHG_BUS_VOLT_REF_MAX       cVdc490V
#define cCHG_BUS_VOLT_REF_MIN       cVdc320V

#define cCHG_BUS_VOLT_BURST_MAX     cVdc500V
#define cCHG_BUS_VOLT_BURST_BACK    cVdc5V

#endif
