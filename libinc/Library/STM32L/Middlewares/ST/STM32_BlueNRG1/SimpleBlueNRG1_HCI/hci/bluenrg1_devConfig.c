
#include "bluenrg_utils.h"

/************** Do not change this define section ************/

#define HS_CRYSTAL_16MHZ        0xFF
#define HS_CRYSTAL_32MHZ        0x00

#define LS_SOURCE_RO            0xFF
#define LS_SOURCE_32KHZ         0x00

#define SMSP_10uH               0xFF
#define SMSPS_4_7uH             0x00
#define SMPS_IND_OFF            0x01

#define USER_MODE               0xFF
#define LS_CRYSTAL_MEASURE      0x01
#define HS_STARTUP_TIME_MEASURE 0x02

#define MASTER_SCA_500ppm       0 /*!< 251 ppm to 500 ppm */
#define MASTER_SCA_250ppm       1 /*!< 151 ppm to 250 ppm */
#define MASTER_SCA_150ppm       2 /*!< 101 ppm to 150 ppm */
#define MASTER_SCA_100ppm       3 /*!< 76 ppm to 100 ppm */
#define MASTER_SCA_75ppm        4 /*!< 51 ppm to 75 ppm */
#define MASTER_SCA_50ppm        5 /*!< 31 ppm to 50 ppm */
#define MASTER_SCA_30ppm        6 /*!< 21 ppm to 30 ppm */
#define MASTER_SCA_20ppm        7 /*!< 0 ppm to 20 ppm */

/************************************************************/


/************** Definitions that can be changed. ************/

#define HS_SPEED_XTAL           HS_CRYSTAL_16MHZ
#define LS_SOURCE               LS_SOURCE_RO

#define SLAVE_SCA_PPM           250     /* Max 500 */
#define MASTER_SCA              MASTER_SCA_30ppm
#define HS_STARTUP_TIME_US      800
#define MAX_CONN_EVT_LEN        4000
#define SMPS_INDUCTOR           SMSPS_4_7uH



#define DAY                     02
#define MONTH                   05
#define YEAR                    85
/************************************************************/

#ifndef HS_SPEED_XTAL
#define HS_SPEED_XTAL   HS_CRYSTAL_32MHZ
#endif

#ifndef LS_SOURCE
#define LS_SOURCE       LS_SOURCE_RO
#endif

#ifndef SMPS_INDUCTOR
#define SMPS_INDUCTOR   SMSP_10uH
#endif

#ifndef TEST_MODE
#define TEST_MODE       USER_MODE
#endif


const devConfig_t deviceConfig = {
  HS_SPEED_XTAL,
  LS_SOURCE,
  SMPS_INDUCTOR,
  0xFF,
  htob(FROM_US_TO_SYS_TIME(HS_STARTUP_TIME_US), 2),
  htob(SLAVE_SCA_PPM, 0),
  MASTER_SCA,
  htob(FROM_US_TO_SYS_TIME((MAX_CONN_EVT_LEN*1000)), 4),
  TEST_MODE
};

