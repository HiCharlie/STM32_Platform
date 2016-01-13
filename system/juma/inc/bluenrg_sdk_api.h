#ifndef _BLUENRG_SDK_API_H_
#define _BLUENRG_SDK_API_H_


#include "cube_hal.h"
#include "hal_types.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gap.h"
#include "string.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "hci_const.h"
#include "gp_timer.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_aci_const.h"
#include "hci.h"
#include "hal.h"
#include "sm.h"
#include "debug.h"
#include <stdlib.h>

void sleep_flag_set(uint8_t flag);

#define run_after_delay(func, args, delay)          run_at_time((func), (args), current_time() + (delay))

typedef void (*function_t)(void* args);

/*Ble State*/
#define BLE_CONNECTABLE		1
#define BLE_NOCONNECTABLE	0

/*connected*/
#define CONNECT_DEVICE_ENABLE  0x03
/*scan interval*/
#define SCAN_P (0x4000)
#define SCAN_L (0x4000)

/**
* @brief Supervision timeout, arg in msec.
*/
#define SUPERV_TIMEOUT (60)
/**
* @brief Connection period, arg in msec.
*/
#define CONN_P(x) ((int)((x)/1.25f))

/**
* @brief Connection length, arg in msec.
*/
#define CONN_L(x) ((int)((x)/0.625f))

#define CONN_P1     (CONN_P(50))
#define CONN_P2     (CONN_P(50))

#define CONN_L1     (CONN_L(1250))
#define CONN_L2     (CONN_L(1250))

typedef struct scan_device_found {
    uint8_t		bdaddr_type;  /**< Type of the peer address (@ref PUBLIC_ADDR, @ref RANDOM_ADDR). */
    tBDAddr	 bdaddr;       /**< Address of the peer device found during scanning. */
    uint8_t		local_name_len;  /**< Length of advertising or scan response data. */
    uint8_t  local_name[VARIABLE_SIZE];
    uint8_t		uuid_type; /**< Advertising or scan response data + RSSI. RSSI is last octect (signed integer). */
    uint8_t  uuid[VARIABLE_SIZE];
    int8_t RSSI;
} scan_device_found_info;

typedef enum {
   SCAN_FILTER_DUPLICATE_DISABLE = 0, /**< scan filter duplicate disable. */
   SCAN_FILTER_DUPLICATE_ENABLE = 1  /**< scan filter duplicate enable. */
} SCAN_FilterDuplicateType;

typedef enum {
    SCAN_PASSIVE = 0,  /**< SERVER is for Peripheral role. */
    SCAN_ACTIVE = 1   /**< CLIENT is for Central role. */
} SCAN_Type;

typedef enum {
    CLIENT = 0, /**< CLIENT is for Central role. */
    SERVER = 1  /**< SERVER is for Peripheral role. */
} BLE_RoleTypeDef;

/*scan param struct*/
/*adv parameter structure*/
typedef struct
{
    uint8_t               type;                 /**< See @ref BLE_GAP_ADV_TYPES. */
    //ble_gap_addr_t       *p_peer_addr;          /**< For @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND mode only, known peer address. */
    uint8_t               fp;                   /**< Filter Policy, see @ref BLE_GAP_ADV_FILTER_POLICIES. */
    //ble_gap_whitelist_t  *p_whitelist;          /**< Pointer to whitelist, NULL if none is given. */
    uint16_t              scan_interval;             /**< Advertising interval between 0x0020 and 0x4000 in 0.625 ms units (20ms to 10.24s), see @ref BLE_GAP_ADV_INTERVALS.
                                                   - If type equals @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND, this parameter must be set to 0 for high duty cycle directed advertising.
                                                   - If type equals @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND, set @ref BLE_GAP_ADV_INTERVAL_MIN <= interval <= @ref BLE_GAP_ADV_INTERVAL_MAX for low duty cycle advertising.*/
    uint16_t              scan_window;              /**< Advertising timeout between 0x0001 and 0x3FFF in seconds, 0x0000 disables timeout. See also @ref BLE_GAP_ADV_TIMEOUT_VALUES. If type equals @ref BLE_GAP_ADV_TYPE_ADV_DIRECT_IND, this parameter must be set to 0 for High duty cycle directed advertising. */
// ble_gap_adv_ch_mask_t channel_mask;         /**< Advertising channel mask. @see ble_gap_channel_mask_t for documentation. */
} ble_gap_scan_params_t;


/*Ble Connection State*/
extern volatile uint8_t Ble_conn_state;


uint32_t current_time(void);
void run_when_idle(function_t func, void* args);
void run_at_time(function_t func, void* args, uint32_t time);

void on_ready(void);
/*Init BLUENrg, HCI, Add Service*/
tBleStatus ble_init_bluenrg(void);
/*Set Adv Namee*/
tBleStatus ble_device_set_name(const char* new_device_name);
/*Add Adv Address*/
tBleStatus advertise_address(uint8_t* advaddress);
/*Config Adv Interval (min_adv_interval > 32*0.625)*/
void ble_device_set_advertising_interval(uint16_t interval);
/*Start to adv*/
tBleStatus ble_device_start_advertising(void);
/*set adv param*/
void ble_set_adv_param(char* adv_name, uint8_t*adv_address, uint8_t tx_power_level, uint16_t adv_interval);
/*Stop Adv*/
tBleStatus ble_device_stop_advertising(void);
/*Tx data(Notify)*/
tBleStatus ble_device_send(uint8_t type, uint32_t length, uint8_t* value);
/*To Disconnect Ble Connection*/
tBleStatus ble_user_disconnect_device(void);
/*Rx Data(write / write without responce)*/
void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value);
/*BLE On Connnection State*/
void ble_device_on_connect( void );
/*BLE On Disconnection State*/
void ble_device_on_disconnect(uint8_t reason);
/*BLE Set Tx Power*/
tBleStatus ble_device_set_tx_power(uint8_t level);

/*Host*/
/*BLE scan param*/
void ble_host_set_scan_param(uint8_t* own_address, uint8_t tx_power_level, uint16_t scan_interval);
/*ble host scan*/
tBleStatus ble_host_start_scan(void);
/*ble host stop scan*/
tBleStatus ble_host_stop_scan(void);
/*BLE Host On Connect*/
tBleStatus ble_host_connect(tBDAddr bdaddr);
void ble_host_discover_char(void);
/*send data*/
tBleStatus ble_host_send(uint8_t type, uint32_t length, uint8_t* value);
/*receive data*/
void ble_host_on_message(uint8_t type, uint16_t length, uint8_t* value);
/*host connected*/
/*BLE On Connnection State*/
void ble_host_on_connect( void );


void Error_Handler(void);

#endif //_BLUENRG_SDK_API_H_




