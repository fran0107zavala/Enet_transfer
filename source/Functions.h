#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "fsl_enet.h"
#include "fsl_phy.h"
#if defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
#include "fsl_memory.h"
#endif
#include "fsl_enet_mdio.h"
#include "fsl_phyksz8081.h"
#include "fsl_common.h"
#include "fsl_sysmpu.h"

#define EXAMPLE_ENET        ENET
#define EXAMPLE_PHY_ADDRESS 0x00U

/* MDIO operations. */
#define EXAMPLE_MDIO_OPS enet_ops
/* PHY operations. */
#define EXAMPLE_PHY_OPS phyksz8081_ops
/* ENET clock frequency. */
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define ENET_RXBD_NUM          (4)
#define ENET_TXBD_NUM          (4)
#define ENET_RXBUFF_SIZE       (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE       (ENET_FRAME_MAX_FRAMELEN)
#define ENET_DATA_LENGTH       (1500)
#define ENET_TRANSMIT_DATA_NUM (20)

#define HEADER_SIZE 14
#define CRC_SIZE 4

#ifndef APP_ENET_BUFF_ALIGNMENT
#define APP_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#endif
#ifndef PHY_AUTONEGO_TIMEOUT_COUNT
#define PHY_AUTONEGO_TIMEOUT_COUNT (100000)
#endif
#ifndef PHY_STABILITY_DELAY_US
#define PHY_STABILITY_DELAY_US (0U)
#endif

#ifndef MAC_ADDRESS
#define MAC_ADDRESS {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60}
#endif

#ifndef Mac_DESTINO
#define MAC_DESTINO {0x60, 0xCF, 0X84, 0X72, 0XD2, 0X9f}
#endif


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief Build ENET broadcast frame. */
void ENET_BuildBroadCastFrame(void);
void init(void);
void send(const char *mensaje);
uint32_t crc32(const uint8_t *data, uint32_t length);
void encrypt_payload(uint8_t *data, uint32_t length);
void recv(void);
//void stack_init(void *arg);

#endif /* FUNCTIONS_H_ */
