
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "Functions.h"

int main(void)
{
    /* Hardware Initialization. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    /* Disable SYSMPU. */
    SYSMPU_Enable(SYSMPU, false);

    const char *mensajes[] = {
    		"No todo lo que es oro reluce...",
			"Aún en la oscuridad...",
			"¿Que es la vida?",
			"No temas a la oscuridad...",
			"Hasta los más pequenos...",
			"No digas que el sol se ha puesto...",
			"El coraje se encuentra...",
			"No todos los tesoros...",
			"Es peligroso...",
			"Un mago nunca llega tarde...",
			"Aun hay esperanza...",
			"El mundo esta cambiando...",
			"Las raices profundas...",
			"No se puede...",
			"Y sobre todo...",
			"De las cenizas, un fuego..."
    };

    int numMensajes = sizeof(mensajes) / sizeof(mensajes[0]);

    init();

    for(int i = 0; i < numMensajes; i++)
    {
    	send(mensajes[i]);
    	recv();
    	SDK_DelayAtLeastUs(4000000U, SystemCoreClock);
    }


}
