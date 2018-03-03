#ifndef __MW_PARAMETER_DESCRIPTION_H__
#define __MW_PARAMETER_DESCRIPTION_H__
/******************************************************************************
 * (c) COPYRIGHT 2014 by NetModule AG, Switzerland.  All rights reserved.
 *
 * The program(s) may only be used and/or copied with the written permission
 * from NetModule AG or in accordance with the terms and conditions stipulated
 * in the agreement contract under which the program(s) have been supplied.
 *
 * PACKAGE : Middleware
 *
 * ABSTRACT:
 *  <Abstract>
 *
 * HISTORY:
 *  Date      Author       Description
 *  20140225  AHu          created
 *
 *****************************************************************************/

/*--- includes ---------------------------------------------------------------*/

/*--- defines ----------------------------------------------------------------*/
/* The parameter IDs from 0 - 65535 (0xffff) are reserved for middleware specific parameters */
#define PARAM_PROTOCOL_VERSION   (1)
#define PARAM_LICENSE_DATA       (3)
#define PARAM_MASTER_UNIQUE_ID   (4)

#define PARAM_LAST_INTERNAL_ID   (0xffff)

/* The global parameters */
#define PARAM_DEVICE_DESC        (0x10000) /* must exist always */

#define PARAM_STUDIO_ADDRESS     (0x10020)
#define PARAM_NUM_OF_LAMPS       (0x10021)
#define PARAM_DEVICE_NAME        (0x10022)
#define PARAM_SOFT_VERSION       (0x10023)

#define PARAM_APERTURE_ALL_LAMPS (0x10040)
//#define PARAM_APERTURE_OF_LAMP1  (0x10041)
//#define PARAM_APERTURE_OF_LAMP2  (0x10042)
//#define PARAM_APERTURE_OF_LAMP3  (0x10043)
//#define PARAM_APERTURE_OF_LAMP4  (0x10044)
//#define PARAM_APERTURE_OF_LAMP5  (0x10045)
//#define PARAM_APERTURE_OF_LAMP6  (0x10046)
//#define PARAM_APERTURE_OF_LAMP7  (0x10047)
//#define PARAM_APERTURE_OF_LAMP8  (0x10048)
//#define PARAM_APERTURE_OF_LAMP9  (0x10049)
//#define PARAM_APERTURE_OF_LAMP10 (0x1004a)
//#define PARAM_APERTURE_OF_LAMP11 (0x1004b)
//#define PARAM_APERTURE_OF_LAMP12 (0x1004c)
//#define PARAM_APERTURE_OF_LAMP13 (0x1004d)
//#define PARAM_APERTURE_OF_LAMP14 (0x1004e)
//#define PARAM_APERTURE_OF_LAMP15 (0x1004f)

#define PARAM_MODEL_LIGHT_ENA    (0x10060)
#define PARAM_MODEL_LIGHT_MODE   (0x10061)

#define PARAM_JOIN_NETWORK       (0x100a0)
#define PARAM_WLAN_LOGIN1_SSID   (0x100a1)
//#define PARAM_WLAN_LOGIN2_SSID   (0x100a2)
//#define PARAM_WLAN_LOGIN3_SSID   (0x100a3)
//#define PARAM_WLAN_LOGIN4_SSID   (0x100a4)
//#define PARAM_WLAN_LOGIN5_SSID   (0x100a5)
//#define PARAM_WLAN_LOGIN6_SSID   (0x100a6)
//#define PARAM_WLAN_LOGIN7_SSID   (0x100a7)
//#define PARAM_WLAN_LOGIN8_SSID   (0x100a8)
//#define PARAM_WLAN_LOGIN9_SSID   (0x100a9)
//#define PARAM_WLAN_LOGIN10_SSID  (0x100aa)
#define PARAM_WLAN_LOGIN1_PWD    (0x100ab)
//#define PARAM_WLAN_LOGIN2_PWD    (0x100ac)
//#define PARAM_WLAN_LOGIN3_PWD    (0x100ad)
//#define PARAM_WLAN_LOGIN4_PWD    (0x100ae)
//#define PARAM_WLAN_LOGIN5_PWD    (0x100af)
//#define PARAM_WLAN_LOGIN6_PWD    (0x100b0)
//#define PARAM_WLAN_LOGIN7_PWD    (0x100b1)
//#define PARAM_WLAN_LOGIN8_PWD    (0x100b2)
//#define PARAM_WLAN_LOGIN9_PWD    (0x100b3)
//#define PARAM_WLAN_LOGIN10_PWD   (0x100b4)

/* NO WLAN in description! */
#define PARAM_WLAN_MAX_ENGERGY        (0x10100)
#define PARAM_RF_MODE		      	  (0x10103)
#define PARAM_WLAN_F_STOP_RANGE       (0x10200)
#define PARAM_WLAN_SIROS_IN_STANDBYE  (0x10210)
#define PARAM_WLAN_SIROS_IN_SPEED     (0x10220) // #define PARAM_WLAN_F_CONTROL_MODE     (0x10220)
#define PARAM_WLAN_SIROS_IN_AUDIO     (0x10230)
#define PARAM_WLAN_SEQUENZ	          (0x10240)
#define PARAM_WLAN_LAMP_ADDRESS	      (0x10250)
#define PARAM_WLAN_ALARM		      	  (0x10260)

/* Charge */
//#define PARAM_VOLTAGE_REACHED         (0x10270)
//#define PARAM_VOLTAGE_TOO_HIGH        (0x10271)
//#define PARAM_FLASH_READY 			      (0x10272)
#define PARAM_FLASH_DURATION		      (0x10273)

#define PARAM_FREEMASK_ALTERMATE_ENA  (0x10280)
#define PARAM_MASKGROUP               (0x10281)
#define PARAM_DELAYTIME               (0x10282)
#define PARAM_SYNCRONIZED_SEQUENCE    (0x10283)
#define PARAM_CHARGE_TIME             (0x10284) // PARAM_SS_CHARGE_TIME
#define PARAM_MANUAL_SLOW_CHARGE      (0x10285)
#define PARAM_FLASH_COUNT             (0x10024)


#define PARAM_WLAN_COG_LIGHT_ENA      (0x10300)
#define PARAM_WLAN_COG_LIGHT_ACT      (0x10310)
#define PARAM_WLAN_COG_LIGHT_COL      (0x10320)

#define PARAM_WLAN_TEST  				      (0x10420)

#define PARAM_WLAN_LOGIN1_ENC_TYPE    (0x10800)
#define PARAM_WLAN_RF_SELECT          (0x10914)
#define PARAM_WLAN_TAG                (0x10915)

#define PARAM_WLAN_UPDATE_SERVICE     (0x11000)



/* We decided to keep device description and software version as the only two parameters
 * which will always remain on the same address to identify the device and its address map. */
enum scoroParam{
    SP_DEVICE_DESC          = 0x10000,   /* -p get; -c returns: n=0 invalid descriptor, n=1 ScoroE1600, n=2 ScoroE3200, n=3 ScoroS1600, n=4 ScoroS3200 */
    SP_STUDIO_ADDR          = 0x10020,   /* -p get, does already exist */
    SP_DEVICE_NAME          = 0x10022,
    SP_AB_SW_VER            = 0x10023,  /* -c adapterboard software version, 4 bytes: starting with most significant byte: major nr., minor nr., 2Byte revision nr. */
    SP_SCORO_ENERGY_TYPE    = 0x10100,  /* -p get; -c n=3200 or n=1600*/
    SP_F_STOP_RANGE         = 0x10200,  /* -c the range in 1/10-f stops the device supports (depends on speed mode) */
    SP_ID_STR               = 0x10700,  /* an identifier string to distinguish the device */
    SP_NW_JOIN              = 0x10800,  /* -p get/set; -c n=0 for Adhoc or n=1 for Infrastructure mode. In case n=1, do set ssid and pwd previously */
    SP_NW_SSID              = 0x10801,  /* -c the ssid of an infrastructure network */
    SP_NW_PWD               = 0x10802,  /* -c the password of an infrastructure network */
    SP_NUM_LAMP_SLOTS       = 0x10900,  /* -p get; -c number of slots the device has (constant) */
    SP_F_STOP_MAX           = 0x10901,  /* -c the top f-stop value the device can currently display in 1/10-f stops (depends on speed and display mode) */
    SP_UPDATE_SERVICE       = 0x10902,  /* -p get; -c returns a string of comma separated ID's writen in hexadecimal format. These ID's have changed recently. Not included are static parameters e.g. serial number and all MEM labeled ID's */
    SP_MULTICOMMAND_SERVICE	= 0x10903,  /* -p	 used to set an "ID1\t[I||S]param1\nID2\t[I||S]param2\n" String, where either 'I' for integer or 'S' for string must be written in front of each parameter. Command buffer is 14 elements long and receive buffer is 128, 512 or 2048 bytes long */
    SP_GEN_APERTURE         = 0x10904,	/* -p get/set; -c the aperture of the lamp/gen in 1/10-f stops */
    SP_L1_APERTURE          = 0x10905,
    SP_L2_APERTURE          = 0x10906,
    SP_L3_APERTURE          = 0x10907,
    SP_L1_SW                = 0x10908,	/* -p get/set; -c tells whether the lamp is switched on (1) or off (0) */
    SP_L2_SW                = 0x10909,
    SP_L3_SW                = 0x1090a,
    SP_L1_INSERTED          = 0x1090b,  /* -p get; -c tells whether the lamp is plugged in (1) or not (0) */
    SP_L2_INSERTED          = 0x1090c,
    SP_L3_INSERTED          = 0x1090d,
    SP_REQUEST_BLOCK        = 0x1090e,  /* -p set; -c can be used to request a block from the scoro MCU, namely 101-104 to read out the memory */
    SP_PILOTLIGHT_SW        = 0x1090f,  /* -p get/set; -c the pilot light state on (1) / off (0) */
    SP_PILOTLIGHT_MODE      = 0x10910,  /* -c the pilot light mode: n=0 pmax, n=1 full, n=2 spar, n=3-7 prop[1-5] */
    SP_IR_SW                = 0x10911,	/* -p get/set; infrared on (1) / off (0) */
    SP_CELL_SW              = 0x10912,  /* -c photocell on (1) / off (0) */
    SP_RFS_SW               = 0x10913,  /* -c rfs on (1) / off (0) */
    SP_RFS_IR_SW            = 0x10914,  /* -p/-c ir/rfs (3), --/rfs (2), ir/-- (1), off (0) */
    SP_T01                  = 0x10915,	/* -p get/set; -c the ignition duration, n=0 opt, n=0x80 min, n=4-9 others, n=0xff accept suggestion */
    SP_T01_VALUE            = 0x10916,  /* -p get; -c returns the ignition duration 1/n */
    SP_SEQUENCE             = 0x10917,	/* -p get/set; -c the number of flashes triggered with each release (n=0 off, n<=50) */
    SP_INTERVAL             = 0x10918,  /* -c the time between two flashes if sequence>=2 in [ms] (max. 50s) */
    SP_DELAY                = 0x10919,  /* -c the delay from the trigger event to the first flash in [ms] (max. 50s) */
    SP_SPEED_SW             = 0x1091a,  /* -c speed flash duration, on (1) / off (0) */
    SP_ALTERNATE_NUMBER_DEVICES= 0x1091b,/* -c 2<=n<=4, e.g. n=2 means that 2 devices take part with alternate and therefore, the device releases every second trigger */
    SP_ALTERNATE_DEVICE_DELAY  = 0x1091c,/* -c n=0 off (renders the above inactive), n=1 active, n=2-4 wait[1-3]. E.g. n=1 means release on first trigger. Always set the above in advance */
    SP_MEMORY_STORE         = 0x1091d,	/* -p set; -c n=1-4 to store current settings into memory slot */
    SP_MEMORY_RECALL        = 0x1091e,  /* -p set; -c n=1-4 to recall memory slot into current settings */
    SP_REFRESH_DATA         = 0x1091f,  /* -p set; -c makes scoro send every block (probably never used) */
    SP_RESET_AUX            = 0x10920,  /* -p set; -c make scoro reset the auxiliary settings */
    SP_STORE_AUX            = 0x10921,	/* -p get/set; -c make scoro save the auxiliary settings */
    SP_SENSITIVITY_PHOTO_CELL= 0x10922,	/* -p get/set; -c n=0 low, n=1 normal */
    SP_TEST_KEY_INTENSITY    = 0x10923, /* -c n=0 bright, n=1 dimmed (the blue light of the test button)*/
    SP_DISPLAY_SPEED_APERTURE= 0x10924, /* -c n=1 display 10 (respectively 9 if SP_DISPLAY_APERTURE_9=1) as the maximal aperture in speed mode */
    SP_REDUCED_PILOTLIGHT   = 0x10925,  /* -c n=1 reduce the pilotlight during charging */
    SP_SLOW_CHARGING        = 0x10926,  /* -c n=1 slow charging */
    SP_DISPLAY_APERTURE_9   = 0x10927,  /* -c n=1 display 9 as the maximal aperture */
    SP_RESET_FLASH_COUNTER  = 0x10928,  /* -p set */
    SP_DISPLAY_INTENSITY    = 0x10929,	/* -p get/set; -c n=0 enviromentlight dependent, n=1-3 low, medium, high */
    SP_RDY_SOUND_MODE       = 0x1092a,  /* -c n=0 standart, n=1 address specific */
    SP_RDY_SOUND_VOLUME     = 0x1092b,	/* -c n=0 off, n=1-3 quiet, medium, noisy */
    SP_COLOR_TEMP           = 0x1092c,	/* -c n=0 decrement, n=1 increment, n=2 suggestion takeover*/
    SP_COUNTRY_NR           = 0x1092d,	/* -p get */
    SP_SALES_YEAR           = 0x1092e,	/* -p get/set */
    SP_SALES_MONTH          = 0x1092f,
    SP_SERVICE_YEAR         = 0x10930,	/* -p get/set */
    SP_SERVICE_MONTH        = 0x10931,
    SP_STANDBY_SW           = 0x10932,	/* -p get/set; -c n=1 device into standby */
    SP_STROBO_SW            = 0x10933,	/* -p get/set; -c n=1 device into strobo mode */
    SP_USER_SW              = 0x10934,  /* -c user button on (1) / off (0) */
    SP_SCORO_VERSION        = 0x10935,	/* -p get; -c n=37 Scoro S, n=137 Scoro E */
    SP_SCORO_CHANGE_IDX     = 0x10936,
    SP_CHARGE_CHANGE_IDX    = 0x10937,
    SP_SCORO_VOLTAGE_TYPE   = 0x10938,  /* -c n=0 switching power supply, n=1-3 conventional 230V, 117V, 100V */
    SP_SERIAL_NR            = 0x10939,  /* -c an 8 byte ascii string */
    SP_DAILY_FLASH_COUNT    = 0x1093a,
    SP_TOTAL_FLASH_COUNT    = 0x1093b,
    SP_ALARM_REASON         = 0x1093c,  /* -c n=0 afterglow, n=1 overcharge, n=2 technical issue */
    SP_THERMO_STATE         = 0x1093d,  /* -c n=1 Flashcapacity, n=2 Lamp 1, n=4 Lamp 2, n=8 Lamp 3 */
    SP_THERMO_TIME          = 0x1093e,  /* -c time in seconds */
    SP_L1_ADDR              = 0x1093f,	/* -p get/set */
    SP_L2_ADDR              = 0x10940,
    SP_L3_ADDR              = 0x10941,
    SP_CHARGE_RDY           = 0x10942,	/* -p get */
    SP_FLASH_TEST           = 0x10943,	/* -p set, does already exist */
    SP_PROP_OVERFLOW        = 0x10944,  /* -p get, -c something with the modellinglight? */
    SP_T01_OVERFLOW         = 0x10945,  /* -c when increasing the aperture, the flash duration increases too and the desired setting might not be possible anymore. Scoro blocks... accept with SP_T01 = 0xff */
    SP_INTERVAL_OVERFLOW    = 0x10946,  /* -c when increasing the aperture, the minimal interval time increases and the desired setting might not be possible anymore. Scoro blocks... accept with SP_INTERVAL >= min */
    SP_COLORTEMP_OVERFLOW   = 0x10947,  /* -c when increasing the aperture, high color temperature is no more possible and the desired setting might not be possible anymore. Scoro blocks... accept with SP_COLOR_TEMP = 2 */
    SP_SLOWCHARGE_OVERFLOW  = 0x10948,  /* -c never occured to me... */
    SP_MIN_INTERVAL         = 0x10949,  /* -c minimal interval time that can be set (depending on aperture and chare mode slow/fast) */
    SP_MEM_APERTURE_L1      = 0x1094a,	/* -p get, -c first request a block (101-104) and then read the below parameters */
    SP_MEM_APERTURE_L2      = 0x1094b,
    SP_MEM_APERTURE_L3      = 0x1094c,
    SP_MEM_SEQUENCE         = 0x1094d,
    SP_MEM_ALTERNATE        = 0x1094e,
    SP_MEM_T01              = 0x1094f,
    SP_MEM_STROBO_SW        = 0x10950,
    SP_MEM_SPEED_SW         = 0x10951,
    SP_MEM_INTERVAL         = 0x10952
};

//#define PARAM_SCORO_TEST scoroParam::SP_FLASH_TEST

/*--- forward declarations ---------------------------------------------------*/

/*--- types ------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*--- function prototypes ----------------------------------------------------*/

#ifdef __cplusplus
} /*end extern c*/
#endif

#endif /* __MW_PARAMETER_DESCRIPTION_H__ */
