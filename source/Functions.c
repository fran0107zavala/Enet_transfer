#include "Functions.h"
#include <stdlib.h>
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "aes.h"
#include "Clave_cfg.h"



uint32_t testTxNum = 0;
bool link = false;

/*******************************************************************************
 * Variables globales
 *
 *
 ******************************************************************************/
/*! @brief Buffer descriptors should be in non-cacheable region and should be align to "ENET_BUFF_ALIGNMENT". */
AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t g_rxBuffDescrip[ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t g_txBuffDescrip[ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);
/*! @brief The data buffers can be in cacheable region or in non-cacheable region.
 * If use cacheable region, the alignment size should be the maximum size of "CACHE LINE SIZE" and "ENET_BUFF_ALIGNMENT"
 * If use non-cache region, the alignment size is the "ENET_BUFF_ALIGNMENT".
 */
SDK_ALIGN(uint8_t g_rxDataBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
          APP_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t g_txDataBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT)],
          APP_ENET_BUFF_ALIGNMENT);

enet_handle_t g_handle;
uint8_t g_frame[ENET_DATA_LENGTH + 14 + 4];

/*! @brief The MAC address for ENET device. */
uint8_t g_macAddr[6] = MAC_ADDRESS;
uint8_t g_macdest[6] = MAC_DESTINO;

/*! @brief Enet PHY and MDIO interface handler. */
static mdio_handle_t mdioHandle = {.ops = &EXAMPLE_MDIO_OPS};
static phy_handle_t phyHandle   = {.phyAddr = EXAMPLE_PHY_ADDRESS, .mdioHandle = &mdioHandle, .ops = &EXAMPLE_PHY_OPS};




void send(const char *mensaje)
{
	//const char *nombre = "Francisco David Zavala Saldana";

	uint32_t count  = 0;
    uint32_t mensaje_length =  strlen(mensaje);
    uint32_t padded_length = (mensaje_length + 15) & ~15; // AES padding (bloque de 16)
    uint32_t length = ENET_DATA_LENGTH - HEADER_SIZE - padded_length - CRC_SIZE;//se modifico
    uint32_t total_payload = length + padded_length; //se modifico
    uint32_t crc_input = HEADER_SIZE;



    memcpy(&g_frame[0], &g_macdest[0], 6U);
    memcpy(&g_frame[6], &g_macAddr[0], 6U);
    memcpy(&g_frame[14], mensaje, mensaje_length);


    g_frame[12] = (padded_length >> 8) & 0xFFU;
    g_frame[13] = padded_length & 0xFF;



    encrypt_payload(&g_frame[14], mensaje_length);  //se modifico total_payload

    for (uint32_t i = 0; i < length; i++)
       {
    	g_frame[i + 14 + padded_length] = i % 0xFFU; //se modifico
       }


    uint32_t crc = crc32(&g_frame[HEADER_SIZE], total_payload);

    g_frame[crc_input + total_payload + 0] = (crc >> 24) & 0xFF;
    g_frame[crc_input + total_payload + 1] = (crc >> 16) & 0xFF;
    g_frame[crc_input + total_payload + 2] = (crc >> 8) & 0xFF;
    g_frame[crc_input + total_payload + 3] = crc & 0xFF;


    if (kStatus_Success == PHY_GetLinkStatus(&phyHandle, &link))
        	    {
        			if (link)
        	        {
        				testTxNum++;
        	            if (kStatus_Success == ENET_SendFrame(EXAMPLE_ENET, &g_handle, &g_frame[0], ENET_DATA_LENGTH, 0, false, NULL))
                        {
        	            	PRINTF("The %d frame transmitted success!\r\n", testTxNum);

        	            }
        	            else
        	            {
        	            	PRINTF(" \r\nTransmit frame failed!\r\n");
        	            }
        	        }
        	    }


}

uint32_t crc32(const uint8_t *data, uint32_t length)
{
	uint32_t crc = 0xFFFFFFFF;
	for(uint32_t i=0; i < length; i++){
		crc ^= data[i];
		for(int j = 0; j < 8; j++){
			if (crc & 1)
				crc = (crc >> 1) ^ (0xEDB88320);
			else
				crc >>= 1;

		}

	}
	return ~crc;
}

void encrypt_payload(uint8_t *data, uint32_t length)
{
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, AES_KEY); //clave del cfg.h
	uint32_t padded_length = (length + 15) & ~15;

	if (padded_length > length)
	{
		memset(data + length, 0, padded_length - length);
	}

	for (uint32_t i = 0; i < padded_length; i += 16) {
	        AES_ECB_encrypt(&ctx, data + i);
	    }
}

void init(void)
{

	enet_config_t config;
		    phy_config_t phyConfig = {0};
		    uint32_t length        = 0;

		    bool autonego          = false;
		    phy_speed_t speed;
		    phy_duplex_t duplex;

		    status_t status;
		    enet_data_error_stats_t eErrStatic;
		    volatile uint32_t count = 0;

		    enet_buffer_config_t buffConfig[] = {{
		   	           ENET_RXBD_NUM,
		   	           ENET_TXBD_NUM,
		   	           SDK_SIZEALIGN(ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
		   	           SDK_SIZEALIGN(ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT),
		   	           &g_rxBuffDescrip[0],
		   	           &g_txBuffDescrip[0],
		   	           &g_rxDataBuff[0][0],
		   	           &g_txDataBuff[0][0],
		   	           true,
		   	           true,
		   	           NULL,
		   	       }};
	    PRINTF("\r\nENET example start.\r\n");

	       /* Prepare the buffer configuration. */


	       ENET_GetDefaultConfig(&config);
#ifdef EXAMPLE_PHY_INTERFACE_RGMII
    config.miiMode = kENET_RgmiiMode;
#else
    config.miiMode = kENET_RmiiMode;
#endif
    phyConfig.phyAddr               = EXAMPLE_PHY_ADDRESS;
    phyConfig.autoNeg               = true;
    mdioHandle.resource.base        = EXAMPLE_ENET;
    mdioHandle.resource.csrClock_Hz = EXAMPLE_CLOCK_FREQ;

    /* Initialize PHY and wait auto-negotiation over. */
    PRINTF("Wait for PHY init...\r\n");
    do
    {
        status = PHY_Init(&phyHandle, &phyConfig);
        if (status == kStatus_Success)
        {
            PRINTF("Wait for PHY link up...\r\n");
            /* Wait for auto-negotiation success and link up */
            count = PHY_AUTONEGO_TIMEOUT_COUNT;
            do
            {
                PHY_GetAutoNegotiationStatus(&phyHandle, &autonego);
                PHY_GetLinkStatus(&phyHandle, &link);
                if (autonego && link)
                {
                    break;
                }
            } while (--count);
            if (!autonego)
            {
                PRINTF("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n");
            }
        }
    } while (!(link && autonego));

#if PHY_STABILITY_DELAY_US
    /* Wait a moment for PHY status to be stable. */
    SDK_DelayAtLeastUs(PHY_STABILITY_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
#endif

    /* Get the actual PHY link speed. */
    PHY_GetLinkSpeedDuplex(&phyHandle, &speed, &duplex);
    /* Change the MII speed and duplex for actual link status. */
    config.miiSpeed  = (enet_mii_speed_t)speed;
    config.miiDuplex = (enet_mii_duplex_t)duplex;

    ENET_Init(EXAMPLE_ENET, &g_handle, &config, &buffConfig[0], &g_macAddr[0], EXAMPLE_CLOCK_FREQ);
    ENET_ActiveRead(EXAMPLE_ENET);

}


void recv(void)
{
    status_t status;
    uint32_t length = 0;



    	// Preguntar si hay un frame y obtener su longitud
    	    status = ENET_GetRxFrameSize(&g_handle, &length, 0);

    	    if (status != kStatus_Success || length == 0) //if (status != kStatus_Success || length == 0)
    	    {
    	        return;
    	    }
    	    SDK_DelayAtLeastUs(10000U, SystemCoreClock); // Espera 10ms


    // Ahora que ya sabemos el tamaño, podemos reservar memoria
    uint8_t *data = (uint8_t *)malloc(length);
    if (data == NULL)
    {
        PRINTF("❌ Error al asignar memoria para el frame.\r\n");
        return;
    }

    // Leer el frame completo
    status = ENET_ReadFrame(EXAMPLE_ENET, &g_handle, data, length, 0, NULL);
    if (status == kStatus_Success)
    {
        static const uint8_t MAC_ADDR[6] = {0xd4, 0xbe, 0xd9, 0x45, 0x22, 0x60};

        // Filtrar por MAC destino
        if (memcmp(data, MAC_ADDR, 6) != 0)
        {
            free(data);
            return;  // No es para este dispositivo
        }


        // Mostrar contenido en ASCII (desde el payload)

        for (uint32_t i = 14; i < length; i++)
        {
            if (data[i] >= 32 && data[i] <= 126)
                PRINTF("%c", data[i]);

        }
        PRINTF("\r\n");
    }
    else if (status == kStatus_ENET_RxFrameError)
    {
        enet_data_error_stats_t eErrStatic;
        ENET_GetRxErrBeforeReadFrame(&g_handle, &eErrStatic, 0);
        ENET_ReadFrame(EXAMPLE_ENET, &g_handle, NULL, 0, 0, NULL);
        PRINTF("❌ Error al recibir frame Ethernet.\r\n");
    }

    // Siempre liberar memoria
    free(data);
}
