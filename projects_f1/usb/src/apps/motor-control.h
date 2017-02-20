#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
#include "types.h"


#define MOTOR_ADDRESS 1
#define MOTOR_CMD_RETRY_COUNT	5
#define MOTOR_CMD_INTERVAL	200 //ms
//#define TURN_ORIENTATION	0X0004//ת������
//	#define TURN_FORWARD	0x10//��ת
//	#define TURN_REVERSE	0x20//��ת
//	#define TURN_TOGGLE	0x30//�ı䷽��
//	#define TURN_STOP			0x01
//	#define TURN_RUN_CONTINUE 0x02
//	#define TURN_RUN_ONE_TIME	0x03

#define TURN_ORIENTATION	0X2000//ת������
	#define TURN_FORWARD	0x10//��ת
	#define TURN_REVERSE	0x20//��ת
	#define TURN_TOGGLE	0x30//�ı䷽��
	#define TURN_STOP			0x01
	#define TURN_RUN_CONTINUE 0x02
	#define TURN_RUN_ONE_TIME	0x03
	
//#define MOTOR_ERROR_ADDRESS	0x2100
#define MOTOR_STATE_ADDRESS	0x2101
#define MOTOR_STATE_RUNING 0x01
#define MOTOR_STATE_ONE_TIME_RUN 0x04

//#define MOTOR_WORK_FREQ_ADDRESS	0x2102
//#define MOTOR_OUT_FREQ_ADDRESS	0x2103
//#define MOTOR_OUT_CURRENT_ADDRESS	0x2104


#define	FREQ_SET_WAY_ADDRESS	0X0001
	#define FREQ_SET_FROME_KEY						1
	#define FREQ_SET_FROME_OPERATE_BOARD	2
	#define FREQ_SET_FROME_SERIAL	4
#define FREQ_SET_ADDRESS			0x2001//0X0002

#define	CMD_SET_WAY_ADDRESS	0X0003
	#define CMD_SET_FROME_OPERATE_BOARD	0
	#define CMD_SET_FROME_SERIAL	2
	
#define	MAX_WORK_FREQ_ADDRESS	0X0006//50~500Hz
	#define MAX_WORK_FREQ_MIN_VAL 5000
	#define MAX_WORK_FREQ_MAX_VAL 7000
	
#define	NORMAL_WORK_FREQ_ADDRESS	0X0007
#define	MAX_VOLTAGE_ADDRESS	0X0008
	#define MOTOR_MAX_VOLTAGE	480
#define	ACCELERATE_TIME_ADDRESS	17
#define	DECELERATE_TIME_ADDRESS	18
#define	HIGH_WORK_FREQ_ADDRESS	19
#define	LOW_WORK_FREQ_ADDRESS	20
#define INIT_PARAMS_SETTLING_ADDRESS 0x0301
	#define PARAMS_ALLOW_MODIFY	0
	#define PARAMS_NOT_ALLOW_MODIFY_BUT_THIS	1
	#define PARAMS_NOT_ALLOW_MODIFY_BUT_THIS_P0002	2
	#define PARAMS_RESTORE_INIT	0x10//�ָ�����
	#define PARAMS_CLEAR_ERROR	0x20//������ϼ�¼
	
#define OVER_LOAD_ADDRESS	0x0501

#define WORK_PARAMS_COUNT	1
#define TIME_SLOTS_COUNT	3

#define FIRST_FREQ_INDEX	0
#define SECOND_FREQ_INDEX	1
#define LAST_FREQ_INDEX	2
#define DOOR_FREQ_INDEX	3
#define IDLE_FREQ_INDEX	4

#define MAX_TIME	60000 //0.1s
#define	OPEN_DOOR_PARAM_INDEX TIME_SLOTS_COUNT
//12B
struct work_params
{
	int16u idle_freq;//����Ƶ��
	int16u accelerate_time;//����ʱ�� 0.1-6000s
	int16u decelerate_time;//����ʱ�� 0.1-6000s
	int16u over_load;//���ر���ϵ��
	int16u flag;
	int16u crc16;
};

//4B
typedef struct time_params_item
{
	int16u period;//0.0~600.0s   ��λ0.1��
	int16u work_freq;//0-100.00Hz ��λ0.01Hz
}time_params_item_t;

//20B
//1-�׶�1��2-�׶�2��3-�׶�3��4-����Ƶ��
typedef struct time_params
{
	struct time_params_item item[TIME_SLOTS_COUNT+1];//��ת��������+���Ų���һ��
	int16u flag;
	int16u crc16;
}time_params_t;


//��ǰ����16B
typedef struct work_state
{
	int16u serial;//��ţ�ÿ�ζ�ȡ����
	int8s time_params_index;//��ǰʱ��������0~9
	int8u reserved[7];
	int16u period;//��ǰ�����ѳ���ʱ��
	int16u flag;
	int16u crc16;
}work_state_t;

#define MOTOR_STATE_STOP	(0)
#define MOTOR_STATE_ITEM_READY	(1)
#define MOTOR_STATE_RUNNING	(2)
#define MOTOR_STATE_PAUSE	(3)
#define MOTOR_STATE_REMOVE	(4)

#define MOTOR_CTRL_FREQ		1//Ƶ������
#define MOTOR_CTRL_SIGNAL		2//�����źţ���λ�źŵ�
	#define MOTOR_DOOR_OPEN	1
	#define MOTOR_ITEM_READY	2
#define MOTOR_CTRL_COMPLETE	3//ȫ���������꼴�������ģʽ
#define MOTOR_CTRL_GO_IDLE	4//����һ��ʱ���Ժ����idleģʽ
typedef struct motor_control
{
	int16u control_type;
	int16u val;
}motor_control_t;

extern struct time_params s_time_params;
extern struct work_params s_work_params;
extern struct work_state s_work_state;

#define CURRENT_PERIOD_TIME (s_time_params.item[s_work_state.time_params_index].period)
#define CURRENT_FREQ	(s_time_params.item[s_work_state.time_params_index].work_freq)
#define CURRENT_WORK_INDEX	(s_work_state.time_params_index)


BOOL motor_turn_reverse_single(int8u addr);
BOOL motor_turn_stop(int8u addr);
BOOL motor_query_state(int8u addr,int16u *state);
BOOL motor_restore_factory_params(int8u addr);
BOOL motor_select_serial(int8u addr);
BOOL motor_lock_params(int8u addr);
BOOL motor_unlock_params(int8u addr);
BOOL motor_set_overload_params(int8u addr,int16u param);
BOOL motor_set_max_voltage(int8u addr,int16u voltate);//��ߵ�ѹ
BOOL motor_set_work_freq(int8u addr,int16u freq);//����Ƶ��
BOOL motor_set_normal_freq(int8u addr,int16u freq);//��������Ƶ��

void motor_work_state_init(struct work_state* work_state);
void motor_work_params_init(void);
void motor_time_params_init(void);
void motor_params_restore(void);
void motor_control_init(void);
void motor_start(void);
void motor_restart(void);
void motor_stop(void);
void motor_pause(void);
void motor_control(struct motor_control* ctrl);
void motor_tick(int ticks);
void motor_ctrl_task(void *p);
#endif
