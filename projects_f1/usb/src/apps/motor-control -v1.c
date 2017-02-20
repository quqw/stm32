#include "includes.h"
#include "motor-control.h"
#include "storage.h"
#include "exception.h"
#include "board/board.h"
#include "drivers/modbus_master.h"
#include "platform/timer.h"
#include "eventsystem/eventsystem.h"
#include "board/board.h"
#include "platform/gpio.h"


struct time_params s_time_params;
struct work_params s_work_params;
struct work_state s_work_state;
static int8u _motor_stat=MOTOR_STATE_STOP;

#define CURRENT_PERIOD_TIME (s_time_params.item[s_work_state.time_params_index].period)
#define CURRENT_FREQ	(s_time_params.item[s_work_state.time_params_index].work_freq)
//����
extern struct modbus_master modbus_master_3;
//��ת
BOOL motor_turn_forward_continuous(int8u addr)
{
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,TURN_ORIENTATION,TURN_FORWARD |TURN_RUN_CONTINUE))
		return FALSE;
	return TRUE;		
}

//��ת
BOOL motor_turn_reverse_continuous(int8u addr)
{
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,TURN_ORIENTATION,TURN_REVERSE|TURN_RUN_CONTINUE))
		return FALSE;
	return TRUE;		
}

//��ת�㶯
BOOL motor_turn_forward_single(int8u addr)
{
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,TURN_ORIENTATION,TURN_FORWARD |TURN_RUN_ONE_TIME))
		return FALSE;
	return TRUE;		
}

//��ת�㶯
BOOL motor_turn_reverse_single(int8u addr)
{
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,TURN_ORIENTATION,TURN_REVERSE|TURN_RUN_ONE_TIME))
		return FALSE;
	return TRUE;		
}

//ֹͣ
BOOL motor_turn_stop(int8u addr)
{
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,TURN_ORIENTATION,TURN_STOP))
		return FALSE;
	return TRUE;		
}

//״̬��ѯ
BOOL motor_query_state(int8u addr,int16u *state)
{
	if(MB_EX_NONE!=modbus_master_read_registers_rtu(&modbus_master_3,addr,MOTOR_STATE_ADDRESS,1,state))
		return FALSE;
	return TRUE;
}

//�ָ���������
//��ָ���ֵ���ԣ���Ҫ���⴦��
//01 06 03 01 00 10 D9 82 
//01 06 00 39 00 10 58 0B 
BOOL motor_restore_factory_params(int8u addr)
{
	int8u response[]={0x01, 0x06 ,0x00 ,0x39, 0x00 ,0x10, 0x58, 0x0B };
	if(MB_EX_ACKNOWLEDGE==modbus_master_write_register_rtu(&modbus_master_3,addr,INIT_PARAMS_SETTLING_ADDRESS,PARAMS_RESTORE_INIT))
		return FALSE;
	if(memcmp(response,modbus_master_3.receive_buffer,8)==0)
		return TRUE;		
	return FALSE;
}

//ѡ�񴮿ڷ�ʽ�趨
BOOL motor_select_serial(int8u addr)
{
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,CMD_SET_WAY_ADDRESS,CMD_SET_FROME_SERIAL))
		return FALSE;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,FREQ_SET_WAY_ADDRESS,FREQ_SET_FROME_SERIAL))
		return FALSE;
	return TRUE;		
}
//��ס�����趨,����Ƶ��
BOOL motor_lock_params(int8u addr)
{
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,INIT_PARAMS_SETTLING_ADDRESS,PARAMS_NOT_ALLOW_MODIFY_BUT_THIS_P0002))
		return FALSE;
	return TRUE;		
}

//��ָ���ֵ����
//01 06 03 01 00 00 D8 4E 
//01 06 00 39 00 00 59 C7 
BOOL motor_unlock_params(int8u addr)
{
	int8u response[]={0x01, 0x06 ,0x00 ,0x39, 0x00 ,0x00, 0x59, 0xC7 };
	if(MB_EX_ACKNOWLEDGE==modbus_master_write_register_rtu(&modbus_master_3,addr,INIT_PARAMS_SETTLING_ADDRESS,PARAMS_ALLOW_MODIFY))
		return FALSE;
	if(memcmp(response,modbus_master_3.receive_buffer,8)==0)
		return TRUE;		
	return FALSE;
}

//�����趨
BOOL motor_set_overload_params(int8u addr,int16u param)
{
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,OVER_LOAD_ADDRESS,param))
		return FALSE;
	return TRUE;	
}
//������ߵ�ѹ
BOOL motor_set_max_voltage(int8u addr,int16u voltate)
{
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,MAX_VOLTAGE_ADDRESS,voltate))
		return FALSE;
	return TRUE;	
}

//�趨����Ƶ��
BOOL motor_set_work_freq(int8u addr,int16u freq)
{
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,FREQ_SET_ADDRESS,freq))
		return FALSE;
	return TRUE;	
}

//�趨��������Ƶ�ʣ���ߵ�ѹ���ʱ����СƵ��
BOOL motor_set_normal_freq(int8u addr,int16u freq)
{
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,NORMAL_WORK_FREQ_ADDRESS,freq))
		return FALSE;
	return TRUE;	
}

//�趨������Ƶ��
BOOL motor_set_max_freq(int8u addr,int16u freq)
{
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,MAX_WORK_FREQ_ADDRESS,freq))
		return FALSE;
	return TRUE;	
}

//�趨Ƶ������
BOOL motor_set_high_freq(int8u addr,int16u freq)
{
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,HIGH_WORK_FREQ_ADDRESS,freq))
		return FALSE;
	return TRUE;	
}
//�趨Ƶ������
BOOL motor_set_low_freq(int8u addr,int16u freq)
{
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,LOW_WORK_FREQ_ADDRESS,freq))
		return FALSE;
	return TRUE;	
}


void motor_work_state_init(struct work_state* work_state)
{
	work_state->serial=0;
	work_state->period=0;
	work_state->reserved[0]=0;
	work_state->reserved[1]=0;
	work_state->reserved[2]=0;
	work_state->flag=0;
}
void motor_work_params_init(struct work_params* work)
{
	work->low_freq=2500;//20hz
	work->normal_freq=10000;//100HZ
	work->high_freq=MAX_WORK_FREQ_MAX_VAL;//500Hz
	work->accelerate_time=10;//����ʱ�� 0.1-6000s
	work->decelerate_time=10;//����ʱ�� 0.1-6000s
	work->over_load=100;//���ر���ϵ��
	work->flag=0;
}
void motor_time_params_init(struct time_params* time)
{
	int i;
	for(i=0;i<TIME_SLOTS_COUNT;i++)
	{
		time->item[i].period=(i+1)*100;
		time->item[i].work_freq=(i+1)*500+3000;
	}
}

//��ȡ��ʼ����״̬
void motor_init_params_restore(void)
{
	if(!restore_work_params(&s_work_params))
	{
		throw_exception(EXCEPTION_STORAGE);
		motor_work_params_init(&s_work_params);
		store_work_params(&s_work_params);
	}
	if(!restore_time_params(&s_time_params))
	{
		throw_exception(EXCEPTION_STORAGE);
		motor_time_params_init(&s_time_params);
		store_time_params(&s_time_params);
	}
	//���з�ʽ������-������-������
	s_time_params.item[0].work_freq=s_work_params.normal_freq;
	s_time_params.item[1].work_freq=s_work_params.low_freq;
	s_time_params.item[2].work_freq=s_work_params.high_freq;
}

DECLARE_TIMER(motor,2);
void motor_control_init(void)
{
	int16u motor_state=0;
	int16u retry_count=0;
	timer_init(&timer_motor);//ʱ������Ϊ100ms
	timer_set_prescaler(&timer_motor,SystemCoreClock/10000-1);//10kHz~0.1ms
	#ifndef __DEBUG
	timer_set_auto_reload(&timer_motor,1000-1);//100ms
	#else
	timer_set_auto_reload(&timer_motor,100-1);//10ms���Լ���
	#endif
	timer_auto_reload_preload(&timer_motor,TRUE);
	timer_init_interrupt(&timer_motor,MOTOR_TIMER_IRQ_PRIORITY,0);
	
	motor_init_params_restore();
	delay_ms(5000);
	
	while(!motor_query_state(MOTOR_ADDRESS,&motor_state))//��ѯ״̬��ͬʱҲ�ǵȴ���Ƶ������
	{
		if(retry_count>MOTOR_CMD_RETRY_COUNT)
		{
			throw_exception(EXCEPTION_MOTOR_CONTROL);
			return;
		}
		retry_count++;
		delay_ms(500);
	}
	retry_count=0;
	delay_ms(MOTOR_CMD_INTERVAL);
	if(motor_state&MOTOR_STATE_RUNING)
		motor_turn_stop(MOTOR_ADDRESS);
	delay_ms(MOTOR_CMD_INTERVAL);
	while(!motor_unlock_params(MOTOR_ADDRESS))
	{
		if(retry_count>MOTOR_CMD_RETRY_COUNT)
		{
			throw_exception(EXCEPTION_MOTOR_CONTROL);
			return;
		}
		retry_count++;
		delay_ms(MOTOR_CMD_INTERVAL);
	}
	delay_ms(MOTOR_CMD_INTERVAL);
	while(!motor_restore_factory_params(MOTOR_ADDRESS))//�ָ������趨
	{
		if(retry_count>MOTOR_CMD_RETRY_COUNT)
		{
			throw_exception(EXCEPTION_MOTOR_CONTROL);
		}
		retry_count++;
		delay_ms(MOTOR_CMD_INTERVAL);
	}
	while(!motor_select_serial(MOTOR_ADDRESS))//ѡ�񴮿��趨
	{
		if(retry_count>MOTOR_CMD_RETRY_COUNT)
		{
			throw_exception(EXCEPTION_MOTOR_CONTROL);
			return;
		}
		retry_count++;
		delay_ms(MOTOR_CMD_INTERVAL);
	}
	delay_ms(MOTOR_CMD_INTERVAL);
	motor_set_max_voltage(MOTOR_ADDRESS,MOTOR_MAX_VOLTAGE);//������ߵ�ѹ
	delay_ms(MOTOR_CMD_INTERVAL);
	motor_set_max_freq(MOTOR_ADDRESS,s_work_params.high_freq);
	delay_ms(MOTOR_CMD_INTERVAL);
	motor_set_high_freq(MOTOR_ADDRESS,s_work_params.high_freq);
	delay_ms(MOTOR_CMD_INTERVAL);
	motor_set_work_freq(MOTOR_ADDRESS,s_work_params.low_freq);
	//motor_lock_params(MOTOR_ADDRESS);
	delay_ms(MOTOR_CMD_INTERVAL);
	motor_turn_forward_continuous(MOTOR_ADDRESS);
	delay_ms(MOTOR_CMD_INTERVAL);
	gpio_enable_interrupt(&gpio_ready);//��λ�ź��жϳ�ʼ��
}
void motor_restart(void)
{
	timer_disable_interrupt(&timer_motor,TIM_IT_Update);
	timer_set_counter(&timer_motor,1);
	timer_update(&timer_motor);
	timer_start(&timer_motor);
	timer_enable_interrupt(&timer_motor,TIM_IT_Update);
	_motor_stat=MOTOR_STATE_RUNNING;
}
////����ͣ״̬�ָ�
//void motor_resume(void)
//{
//	motor_set_work_freq(MOTOR_ADDRESS, s_time_params.item[s_work_state.time_params_index].work_freq);
//	timer_enable_interrupt(&timer_motor,TIM_IT_Update);
//	timer_start(&timer_motor);
//	_motor_stat=MOTOR_STATE_RUNNING;
//}

void motor_stop(void)
{
	timer_stop(&timer_motor);
	timer_disable_interrupt(&timer_motor,TIM_IT_Update);
	timer_clear_interrupt_flag(&timer_motor);
	timer_set_counter(&timer_motor,0);
	timer_update(&timer_motor);
	s_work_state.time_params_index=0;
	s_work_state.period=0;
	_motor_stat=MOTOR_STATE_STOP;
}
//��ͣ
void motor_pause(void)
{
	timer_stop(&timer_motor);
	timer_disable_interrupt(&timer_motor,TIM_IT_Update);
	_motor_stat=MOTOR_STATE_PAUSE;
}

void motor_tick(int ticks)
{
	struct motor_control ctrl;
	mem_address_t address;
	ctrl.control_type= MOTOR_CTRL_SIGNAL;
	ctrl.val=0;
	if(board_door_open())
		ctrl.val|=MOTOR_DOOR_OPEN;
	if(board_item_ready())
		ctrl.val|=MOTOR_ITEM_READY;
	address=mem_alloc(sizeof(struct motor_control),&ctrl);
	if(address==INVALID_ADDRESS)
	{
		throw_exception(EXCEPTION_MEM_ALLOCATE);
		return;
	}
	event_send(MOTOR_EVENT_ID,address);
}

//˵������λ�źſ��ƹ����Ƿ�ʼ�������źŴ������Ƶ�����
void motor_control(struct motor_control* ctrl)
{
	static BOOL _ready_set=FALSE,_door_open_set=FALSE;
	switch(ctrl->control_type)
	{
		case MOTOR_CTRL_FREQ:
			if(!board_door_open())
				motor_set_work_freq(MOTOR_ADDRESS,ctrl->val);
			break;
		case MOTOR_CTRL_SIGNAL://�����źţ��ţ���λ
			if(board_door_open())//���ź�ֻ������ͣ�ͼ���
			{
				if(_door_open_set== FALSE)
				{
					delay_us(10000);
					motor_set_work_freq(MOTOR_ADDRESS, s_work_params.high_freq);
					_door_open_set=TRUE;
				}
				return;
			}
			else
			{
				if(_door_open_set==TRUE )
				{
					_door_open_set=FALSE;
					delay_us(10000);
					if(_motor_stat==MOTOR_STATE_RUNNING)
						motor_set_work_freq(MOTOR_ADDRESS, CURRENT_FREQ);
					else
						motor_set_work_freq(MOTOR_ADDRESS,  s_work_params.low_freq);
				}
			}
			if(ctrl->val & MOTOR_ITEM_READY)//������λ�ź�
			{
				if(!_ready_set)//����һ��λ�Ϳ�ʼ������ֻ��ʼһ�Σ�ֱ���Ƴ����ٴε������ſ������¿�ʼ
				{
					if(!restore_work_params(&s_work_params))
					{
						throw_exception(EXCEPTION_STORAGE);
						return;
					}
					if(!restore_time_params(&s_time_params))
					{
						throw_exception(EXCEPTION_STORAGE);
						return;
					}
					_ready_set=TRUE;
					motor_restart();
					if(_door_open_set==FALSE)//��������²�����Ƶ��
					{
						delay_us(10000);
						motor_set_work_freq(MOTOR_ADDRESS, CURRENT_FREQ);
					}
				}
			}
			else //û�е�λ
			{
				_ready_set=FALSE;
				if(_motor_stat==MOTOR_STATE_STOP)
				{
					if(board_voltage_signal())
						board_relay_on(&gpio_relay1);
				}
			}
			break;
		case MOTOR_CTRL_COMPLETE:
			motor_stop();
			_motor_stat=MOTOR_STATE_STOP;
			delay_us(10000);
			motor_set_work_freq(MOTOR_ADDRESS, s_work_params.low_freq);
			break;
		default:
			break;
	}
}


//�ж�ÿ0.1sһ��
//�����޸ģ����浱ǰ״̬
void motor_control_isr(void)
{
	mem_address_t address;
	struct motor_control ctrl;
	timer_clear_interrupt_flag(&timer_motor);
	TEST_OUT;
	s_work_state.period++;
	if(s_work_state.period<CURRENT_PERIOD_TIME)
	{//��ǰ����ʱ��δ��
		return;
	}
	s_work_state.period=0;
	s_work_state.time_params_index++;
	store_work_state(&s_work_state);
	if(CURRENT_PERIOD_TIME ==0 || s_work_state.time_params_index>=TIME_SLOTS_COUNT)
	{
		//��ǰ�������
		TEST_OUT;
		ctrl.control_type=MOTOR_CTRL_COMPLETE;
		ctrl.val=1;//����ν��ֵ
		timer_stop(&timer_motor);
	}
	else
	{
		TEST_OUT;
		TEST_OUT;
		ctrl.control_type=MOTOR_CTRL_FREQ;
		ctrl.val=CURRENT_FREQ;
	}
	address=mem_alloc(sizeof(struct motor_control),&ctrl);
	if(address==INVALID_ADDRESS)
	{
		throw_exception(EXCEPTION_MEM_ALLOCATE);
		return;
	}
	event_send_from_isr(MOTOR_EVENT_ID,address);
}

void TIM2_IRQHandler(void)
{
	motor_control_isr();
}

void EXTI0_IRQHandler(void)
{
	mem_address_t address;
	struct motor_control ctrl;
	gpio_clear_interrupt_pending(&gpio_ready);
	ctrl.control_type=MOTOR_CTRL_SIGNAL;
	ctrl.val = MOTOR_ITEM_READY;
	address=mem_alloc(sizeof(struct motor_control),&ctrl);
	if(address==INVALID_ADDRESS)
	{
		throw_exception(EXCEPTION_MEM_ALLOCATE);
		return;
	}
	event_send_from_isr(MOTOR_EVENT_ID,address);
}
