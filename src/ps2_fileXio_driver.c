/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2005, ps2dev - http://www.ps2dev.org
# Licenced under GNU Library General Public License version 2
# Review ps2sdk README & LICENSE files for further details.
#
# PS2_FILEXIO_DRIVER
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ps2_sio2man_driver.h>
#include <ps2_fileXio_driver.h>

#include <sifrpc.h>
#include <loadfile.h>

int fileXioInit(void);

/* References to IOMANX.IRX */
extern unsigned char iomanX_irx[] __attribute__((aligned(16)));
extern unsigned int size_iomanX_irx;

/* References to FILEXIO.IRX */
extern unsigned char fileXio_irx[] __attribute__((aligned(16)));
extern unsigned int size_fileXio_irx;

#ifdef F_internals_ps2_fileXio_driver
enum FILEXIO_INIT_STATUS __fileXio_init_status = FILEXIO_INIT_STATUS_UNKNOWN;
int32_t __iomanX_id = -1;
int32_t __fileXio_id = -1;
#else
extern enum FILEXIO_INIT_STATUS __fileXio_init_status;
extern int32_t __iomanX_id;
extern int32_t __fileXio_id;
#endif

#ifdef F_init_ps2_fileXio_driver
static enum FILEXIO_INIT_STATUS loadIRXs(void) {
    /* IOMANX.IRX */
    __iomanX_id = SifExecModuleBuffer(&iomanX_irx, size_iomanX_irx, 0, NULL, NULL);
    if (__iomanX_id < 0)
        return FILEXIO_INIT_STATUS_IOMANX_IRX_ERROR;

    /* FILEXIO.IRX */
    __fileXio_id = SifExecModuleBuffer(&fileXio_irx, size_fileXio_irx, 0, NULL, NULL);
    if (__fileXio_id < 0)
        return FILEXIO_INIT_STATUS_FILEXIO_IRX_ERROR;
    
    return FILEXIO_INIT_STATUS_IRX_OK;
}

static enum FILEXIO_INIT_STATUS initLibraries(void) {
    fileXioInit();

    return FILEXIO_INIT_STATUS_OK;
}

enum FILEXIO_INIT_STATUS init_fileXio_driver() {
    __fileXio_init_status = loadIRXs();
    if (__fileXio_init_status < 0)
        return __fileXio_init_status;

    __fileXio_init_status = initLibraries();

    return __fileXio_init_status;
}
#endif

#ifdef F_deinit_ps2_fileXio_driver
static void unloadIRXs(void) {
    /* FILEXIO.IRX */
    if (__fileXio_id > 0) {
        SifUnloadModule(__fileXio_id);
        __fileXio_id = -1;
    }
    
    /* IOMANX.IRX */
    if (__iomanX_id > 0) {
        SifUnloadModule(__iomanX_id);
        __iomanX_id = -1;
    }
}

void deinit_fileXio_driver() {    
    unloadIRXs();
}
#endif