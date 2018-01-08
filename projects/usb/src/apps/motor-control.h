#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
#include "types.h"


#define MOTOR_ADDRESS 1
#define MOTOR_CMD_RETRY_COUNT	5
#define MOTOR_CMD_INTERVAL	200 //ms
//#define TURN_ORIENTATION	0X0004//转动方向
//	#define TURN_FORWARD	0x10//正转
//	#define TURN_REVERSE	0x20//反转
//	#define TURN_TOGGLE	0x30//改变方向
//	#define TURN_STOP			0x01
//	#define TURN_RUN_CONTINUE 0x02
//	#define TURN_RUN_ONE_TIME	0x03

#define TURN_ORIENTATION	0X2000//转动方向
	#define TURN_FORWARD	0x10//正转
	#define TURN_REVERSE	0x20//反转
	#define TURN_TOGGLE	0x30//改变方向
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
	#define PARAMS_RESTORE_INIT	0x10//恢复出厂
	#define PARAMS_CLEAR_ERROR	0x20//清除故障记录
	
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
	int16u idle_freq;//待机频率
	int16u accelerate_time;//加速时间 0.1-6000s
	int16u decelerate_time;//减速时间 0.1-6000s
	int16u over_load;//过载保护系数
	int16u flag;
	int16u crc16;
};

//4B
typedef struct time_params_item
{
	int16u period;//0.0~600.0s   单位0.1秒
	int16u work_freq;//0-100.00Hz 单位0.01Hz
}time_params_item_t;

//20B
//1-阶段1，2-阶段2，3-阶段3，4-开门频率
typedef struct time_params
{
	struct time_params_item item[TIME_SLOTS_COUNT+1];//运转参数数量+开门参数一个
	int16u flag;
	int16u crc16;
}time_params_t;


//当前工作16B
typedef struct work_state
{
	int16u serial;//序号，每次读取最大的
	int8s time_params_index;//当前时间参数序号0~9
	int8u reserved[7];
	int16u period;//当前工作已持续时间
	int16u flag;
	int16u crc16;
}work_state_t;

#define MOTOR_STATE_STOP	(0)
#define MOTOR_STATE_ITEM_READY	(1)
#define MOTOR_STATE_RUNNING	(2)
#define MOTOR_STATE_PAUSE	(3)
#define MOTOR_STATE_REMOVE	(4)

#define MOTOR_CTRL_FREQ		1//频率设置
#define MOTOR_CTRL_SIGNAL		2//开门信号，到位信号等
	#define MOTOR_DOOR_OPEN	1
	#define MOTOR_ITEM_READY	2
#define MOTOR_CTRL_COMPLETE	3//全部流程走完即进入结束模式
#define MOTOR_CTRL_GO_IDLE	4//开门一段时间以后进入idle模式
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
BOOL motor_set_max_voltage(int8u addr,int16u voltate);//最高电压
BOOL motor_set_work_freq(int8u addr,int16u freq);//工作频率
BOOL motor_set_normal_freq(int8u addr,int16u freq);//基本运行频率

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
