#ifndef HTU21_H
#define HTU21_H

#include "platform/i2c.h"

#define HTU21_CHIP_ID	0x80
#define HTU21_CMD_TRIGGER_T 0XF3
#define HTU21_CMD_TRIGGER_RH	0XF5
#define HTU21_CMD_WRITE_REG 	0XE6
#define HTU21_CMD_READ_REG		0XE7
#define HTU21_CMD_RESET				0XFE

#define HTU21_CONFIG_12_14	0
#define HTU21_CONFIG_8_12		0x01
#define HTU21_CONFIG_10_13	0x80
#define HTU21_CONFIG_11_11		0x81
#define HTU21_CONFIG_DISABLE_OTP	0x02

typedef struct htu21
{
	struct i2c_adapter *i2c;
	int8u address;
}htu21_t;

typedef enum htu21_result
{
	HTU21_OK=0,
	HTU21_BUS_ERR=1,
	HTU21_CRC_ERR=2,
	HTU21_TYPE_ERR=3,
	HTU21_BUSY=4,
}htu21_result_t;

#define DECLARE_HTU21(name,i2c,address) struct htu21 htu21_##name={&i2c,address}

htu21_result_t htu21_trigger_t(struct htu21 *htu);
htu21_result_t htu21_trigger_rh(struct htu21 *htu);
htu21_result_t htu21_read(struct htu21 *htu,int16u* data);
htu21_result_t htu21_set_config(struct htu21 *htu,int8u config);
htu21_result_t htu21_read_config(struct htu21 *htu,int8u* config);
htu21_result_t htu21_read_t(struct htu21 *htu,int16s* out);
htu21_result_t htu21_read_rh(struct htu21 *htu,int16u* out);
#endif

