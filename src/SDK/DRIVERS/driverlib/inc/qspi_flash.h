#ifndef __QSPI_FLASH_H__
#define __QSPI_FLASH_H__

#include "hw_qspi_flash.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "debug.h"
#include "interrupt.h"
#include "replace.h"

#define FLASH_INDIRECT_MAX_BYTE     0x8000


#define BITS_PER_LONG       32
#define BIT(nr)             (1UL << (nr))
#define BIT_MASK(nr)        (1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)        ((nr) / BITS_PER_LONG)

#ifndef NULL
#define NULL    0
#endif

#define readw(addr)         (*(volatile unsigned short *)  (addr))
#define readl(addr)         (*(volatile unsigned int *)  (addr))
#define writew(b,addr)      ((*(volatile unsigned short *) (addr)) = (b))
#define writel(b,addr)      ((*(volatile unsigned int *) (addr)) = (b))

#define DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))

/* SPI mode flags */
#define SPI_CPHA            BIT(0)			/* clock phase */
#define SPI_CPOL            BIT(1)			/* clock polarity */
#define SPI_MODE_0          (0|0)			/* (original MicroWire) */
#define SPI_MODE_1          (0|SPI_CPHA)
#define SPI_MODE_2          (SPI_CPOL|0)
#define SPI_MODE_3          (SPI_CPOL|SPI_CPHA)
#define SPI_CS_HIGH         BIT(2)			/* CS active high */
#define SPI_LSB_FIRST       BIT(3)			/* per-word bits-on-wire */
#define SPI_3WIRE           BIT(4)			/* SI/SO signals shared */
#define SPI_LOOP            BIT(5)			/* loopback mode */
#define SPI_SLAVE           BIT(6)			/* slave mode */
#define SPI_PREAMBLE        BIT(7)			/* Skip preamble bytes */
#define SPI_TX_BYTE         BIT(8)			/* transmit with 1 wire byte */
#define SPI_TX_DUAL         BIT(9)			/* transmit with 2 wires */
#define SPI_TX_QUAD         BIT(10)			/* transmit with 4 wires */
#define SPI_RX_SLOW         BIT(11)			/* receive with 1 wire slow */
#define SPI_RX_DUAL         BIT(12)			/* receive with 2 wires */
#define SPI_RX_QUAD         BIT(13)			/* receive with 4 wires */


#define QSPI_IS_ADDR(cmd_len)               (cmd_len > 1 ? 1 : 0)

#define QSPI_NO_DECODER_MAX_CS              4
#define QSPI_DECODER_MAX_CS                 16
#define QSPI_READ_CAPTURE_MAX_DELAY	        16

typedef struct {
	unsigned char	*regbase;
	unsigned char	*ahbbase;

	unsigned int    page_size;
	unsigned int    block_size;
	unsigned int    tshsl_ns;
	unsigned int    tsd2d_ns;
	unsigned int    tchsh_ns;
	unsigned int    tslch_ns;
	unsigned int    flash_type;
	unsigned int    addr_bytes;
    unsigned int    otp_base;
} QSPI_FLASH_Def;

/* Functions call declaration */
extern void qspi_apb_controller_init(QSPI_FLASH_Def *plat);
extern void qspi_apb_controller_enable(unsigned char *reg_base_addr);
extern void qspi_apb_controller_disable(unsigned char *reg_base_addr);

extern int qspi_apb_command_read(unsigned char *reg_base_addr,
	unsigned int cmdlen, const unsigned char *cmdbuf, unsigned int rxlen, unsigned char *rxbuf);
extern int qspi_apb_command_write(unsigned char *reg_base_addr,
	unsigned int cmdlen, const unsigned char *cmdbuf,
	unsigned int txlen,  const unsigned char *txbuf);

extern int qspi_apb_indirect_read_setup(QSPI_FLASH_Def *plat,
	unsigned int cmdlen, unsigned int rx_width, const unsigned char *cmdbuf);
extern int qspi_apb_indirect_read_execute(QSPI_FLASH_Def *plat,
	unsigned int rxlen, unsigned char *rxbuf);
extern int qspi_apb_indirect_write_setup(QSPI_FLASH_Def *plat,
	unsigned int cmdlen, const unsigned char *cmdbuf);
extern int qspi_apb_indirect_write_execute(QSPI_FLASH_Def *plat,
	unsigned int txlen, const unsigned char *txbuf);

extern void qspi_apb_chipselect(unsigned char *reg_base,
	unsigned int chip_select, unsigned int decoder_enable); 
extern void qspi_apb_set_clk_mode(unsigned char *reg_base, unsigned int mode);
extern void qspi_apb_config_baudrate_div(unsigned char *reg_base,
	unsigned int ref_clk_hz, unsigned int sclk_hz);
extern void qspi_apb_delay(unsigned char *reg_base,
	unsigned int ref_clk, unsigned int sclk_hz,
	unsigned int tshsl_ns, unsigned int tsd2d_ns,
	unsigned int tchsh_ns, unsigned int tslch_ns);
extern void qspi_apb_enter_xip(unsigned char *reg_base, char xip_dummy);
extern void qspi_apb_exit_xip(unsigned char *reg_base);
extern void qspi_apb_readdata_capture(unsigned char *reg_base,
	unsigned int bypass, unsigned int delay);
extern void qspi_wait_indirect_complete(void);
extern void qspi_rbuf_wait_idle(void);
extern void qspi_wbuf_wait_idle(void);
extern void qspi_wait_idle(unsigned char *reg_base);
extern void qspi_apb_indirect_enable(QSPI_FLASH_Def *plat);
extern void qspi_apb_indirect_disable(QSPI_FLASH_Def *plat);
extern void qspi_apb_indirect_write(QSPI_FLASH_Def *plat, unsigned int dst, unsigned int n_bytes);
extern void qspi_apb_indirect_read(QSPI_FLASH_Def *plat, unsigned int src, unsigned int n_bytes);
extern void qspi_apb_set_read_instruction(unsigned char *reg_base, unsigned int config);
extern void qspi_apb_set_write_instruction(unsigned char *reg_base, unsigned int config);
extern void qspi_apb_interrupt_enable(unsigned char *reg_base, unsigned int intflags);
extern void qspi_apb_interrupt_disable(unsigned char *reg_base, unsigned int intflags);
extern unsigned long qspi_apb_interrupt_status_get(unsigned char *reg_base);
extern void QSPIIntRegister(unsigned long *g_pRAMVectors, void (*pfnHandler)(void));
extern void QSPIIntUnregister(unsigned long *g_pRAMVectors);

#endif /* __QSPI_FLASH_H__ */
