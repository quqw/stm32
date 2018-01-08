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
#define MODBUS_RETRY 1//通信重试次数
static xQueueHandle _motor_queue; 
struct time_params s_time_params;
struct work_params s_work_params;
struct work_state s_work_state;
static int8u _motor_stat=MOTOR_STATE_STOP;

#define COMMAND_DELAY(x)	vTaskDelay(x)

//操作
extern struct modbus_master modbus_master_3;
//正转
BOOL motor_turn_forward_continuous(int8u addr)
{
	BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,TURN_ORIENTATION,TURN_FORWARD |TURN_RUN_CONTINUE))
		ret= FALSE;
	ret= TRUE;
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);	
	return ret;
}

//反转
BOOL motor_turn_reverse_continuous(int8u addr)
{
	BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,TURN_ORIENTATION,TURN_REVERSE|TURN_RUN_CONTINUE))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;	
}

//正转点动
BOOL motor_turn_forward_single(int8u addr)
{
	BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,TURN_ORIENTATION,TURN_FORWARD |TURN_RUN_ONE_TIME))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;		
}

//反转点动
BOOL motor_turn_reverse_single(int8u addr)
{
	BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,TURN_ORIENTATION,TURN_REVERSE|TURN_RUN_ONE_TIME))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;		
}

//停止
BOOL motor_turn_stop(int8u addr)
{
	BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,TURN_ORIENTATION,TURN_STOP))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;		
}

//状态查询
BOOL motor_query_state(int8u addr,int16u *state)
{
	BOOL ret;
	if(MB_EX_NONE!=modbus_master_read_registers_rtu(&modbus_master_3,addr,MOTOR_STATE_ADDRESS,1,state))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;	
}

//恢复出厂设置
//此指令返回值不对，需要特殊处理
//01 06 03 01 00 10 D9 82 
//01 06 00 39 00 10 58 0B 
BOOL motor_restore_factory_params(int8u addr)
{
	BOOL ret;
	int8u response[]={0x01, 0x06 ,0x00 ,0x39, 0x00 ,0x10, 0x58, 0x0B };
	if(MB_EX_ACKNOWLEDGE==modbus_master_write_register_rtu(&modbus_master_3,addr,INIT_PARAMS_SETTLING_ADDRESS,PARAMS_RESTORE_INIT))
		ret= FALSE;
	if(memcmp(response,modbus_master_3.receive_buffer,8)==0)
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;	
}

//选择串口方式设定
BOOL motor_select_serial(int8u addr)
{
	BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,CMD_SET_WAY_ADDRESS,CMD_SET_FROME_SERIAL))
	{
		COMMAND_DELAY(MOTOR_CMD_INTERVAL);
		return FALSE;
	}
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,FREQ_SET_WAY_ADDRESS,FREQ_SET_FROME_SERIAL))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;		
}
//锁住参数设定,除了频率
BOOL motor_lock_params(int8u addr)
{
	BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,INIT_PARAMS_SETTLING_ADDRESS,PARAMS_NOT_ALLOW_MODIFY_BUT_THIS_P0002))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;		
}

//此指令返回值不对
//01 06 03 01 00 00 D8 4E 
//01 06 00 39 00 00 59 C7 
BOOL motor_unlock_params(int8u addr)
{
	BOOL ret;
	int8u response[]={0x01, 0x06 ,0x00 ,0x39, 0x00 ,0x00, 0x59, 0xC7 };
	if(MB_EX_ACKNOWLEDGE==modbus_master_write_register_rtu(&modbus_master_3,addr,INIT_PARAMS_SETTLING_ADDRESS,PARAMS_ALLOW_MODIFY))
	{
		COMMAND_DELAY(MOTOR_CMD_INTERVAL);
		return FALSE;
	}
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	if(memcmp(response,modbus_master_3.receive_buffer,8)==0)
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;	
}

//过载设定
BOOL motor_set_overload_params(int8u addr,int16u param)
{
	BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,OVER_LOAD_ADDRESS,param))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;		
}
//设置最高电压
BOOL motor_set_max_voltage(int8u addr,int16u voltate)
{
		BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,MAX_VOLTAGE_ADDRESS,voltate))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;		
}

//设定工作频率
BOOL motor_set_work_freq(int8u addr,int16u freq)
{
		BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,FREQ_SET_ADDRESS,freq))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;	
}

//设定基本工作频率，最高电压输出时的最小频率
BOOL motor_set_normal_freq(int8u addr,int16u freq)
{
		BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,NORMAL_WORK_FREQ_ADDRESS,freq))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;	
}

//设定最高输出频率
BOOL motor_set_max_freq(int8u addr,int16u freq)
{
		BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,MAX_WORK_FREQ_ADDRESS,freq))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;	
}

//设定频率上限
BOOL motor_set_high_freq(int8u addr,int16u freq)
{
		BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,HIGH_WORK_FREQ_ADDRESS,freq))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;		
}
//设定频率下限
BOOL motor_set_low_freq(int8u addr,int16u freq)
{
		BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,LOW_WORK_FREQ_ADDRESS,freq))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;		
}

//设定加速时间(0.1S)
BOOL motor_set_accelerate_time(int8u addr,int16u time)
{
		BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,ACCELERATE_TIME_ADDRESS,time))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;		
}

//设定减速时间(0.1S)
BOOL motor_set_decelerate_time(int8u addr,int16u time)
{
		BOOL ret;
	if(MB_EX_NONE!=modbus_master_write_register_rtu(&modbus_master_3,addr,DECELERATE_TIME_ADDRESS,time))
		ret=FALSE;
	ret=TRUE;	
	COMMAND_DELAY(MOTOR_CMD_INTERVAL);
	return ret;		
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
void motor_work_params_init(void)
{
	s_work_params.idle_freq=1000;//10Hz
	s_work_params.accelerate_time=50;//加速时间 0.1-6000s
	s_work_params.decelerate_time=50;//减速时间 0.1-6000s
	s_work_params.over_load=100;//过载保护系数
	s_work_params.flag=0;
	store_work_params(&s_work_params);
}

void motor_time_params_init(void)
{
	int i;
	for(i=0;i<TIME_SLOTS_COUNT;i++)
	{
		s_time_params.item[i].period=(i+1)*100;
		s_time_params.item[i].work_freq=(i+1)*500+3000;
	}
	store_time_params(&s_time_params);
}

//获取初始运行状态
void motor_params_restore(void)
{
	if(!restore_work_params(&s_work_params))
	{
		throw_exception(EXCEPTION_STORAGE);
		motor_work_params_init();
	}
	if(!restore_time_params(&s_time_params))
	{
		throw_exception(EXCEPTION_STORAGE);
		motor_time_params_init();
	}
	//运行方式：中速-》低速-》高速
//	s_time_params.item[0].work_freq=s_work_params.normal_freq;
//	s_time_params.item[1].work_freq=s_work_params.low_freq;
//	s_time_params.item[2].work_freq=s_work_params.high_freq;
//	s_time_params.item[3].work_freq=s_work_params.door_freq;
}

DECLARE_TIMER(motor,2);
void motor_control_init(void)
{
	int16u motor_state=0;
	int16u retry_count=0;
	timer_init(&timer_motor);//时钟设置为100ms
	timer_set_prescaler(&timer_motor,SystemCoreClock/10000-1);//10kHz~0.1ms
	timer_set_auto_reload(&timer_motor,1000-1);//100ms
	//timer_set_auto_reload(&timer_motor,1000-1);//10ms测试加速

	timer_auto_reload_preload(&timer_motor,TRUE);
	timer_init_interrupt(&timer_motor,MOTOR_TIMER_IRQ_PRIORITY,0);
	//查询状态，同时也是等待变频器启动
	while(!motor_query_state(MOTOR_ADDRESS,&motor_state))
	{
		if(retry_count>MOTOR_CMD_RETRY_COUNT)
		{
			throw_exception(EXCEPTION_MOTOR_CONTROL);
		}
		retry_count++;
	}
	retry_count=0;
	while(motor_state & MOTOR_STATE_RUNING)//关闭变频器
	{
		if(!motor_turn_stop(MOTOR_ADDRESS))
			retry_count++;
		if(!motor_query_state(MOTOR_ADDRESS,&motor_state))
			retry_count++;
		if(retry_count>MOTOR_CMD_RETRY_COUNT)
		{
			throw_exception(EXCEPTION_MOTOR_CONTROL);
		}
	}
	retry_count=0;
	while(!motor_unlock_params(MOTOR_ADDRESS))
	{
		if(retry_count>MOTOR_CMD_RETRY_COUNT)
		{
			throw_exception(EXCEPTION_MOTOR_CONTROL);
			//return;
		}
		retry_count++;
	}
	while(!motor_restore_factory_params(MOTOR_ADDRESS))//恢复出厂设定
	{
		if(retry_count>MOTOR_CMD_RETRY_COUNT)
		{
			throw_exception(EXCEPTION_MOTOR_CONTROL);
			//return;
		}
		retry_count++;
	}
	while(!motor_select_serial(MOTOR_ADDRESS))//选择串口设定
	{
		if(retry_count>MOTOR_CMD_RETRY_COUNT)
		{
			throw_exception(EXCEPTION_MOTOR_CONTROL);
			//return;
		}
		retry_count++;
	}
	if(!motor_set_max_voltage(MOTOR_ADDRESS,MOTOR_MAX_VOLTAGE))//设置最高电压
		throw_exception(EXCEPTION_MOTOR_CONTROL);
	if(!motor_set_max_freq(MOTOR_ADDRESS,MAX_WORK_FREQ_MAX_VAL))
		throw_exception(EXCEPTION_MOTOR_CONTROL);
	if(!motor_set_high_freq(MOTOR_ADDRESS,MAX_WORK_FREQ_MAX_VAL))
		throw_exception(EXCEPTION_MOTOR_CONTROL);
//	if(!motor_set_work_freq(MOTOR_ADDRESS,s_work_params.idle_freq))
//		throw_exception(EXCEPTION_MOTOR_CONTROL);
	//motor_lock_params(MOTOR_ADDRESS);
	if(!motor_set_accelerate_time(MOTOR_ADDRESS,s_work_params.accelerate_time*20))//0.01S，说明书错误
		throw_exception(EXCEPTION_MOTOR_CONTROL);
	if(!motor_set_decelerate_time(MOTOR_ADDRESS,s_work_params.decelerate_time*20))//0.01S，说明书错误
		throw_exception(EXCEPTION_MOTOR_CONTROL);
//	if(!motor_turn_forward_continuous(MOTOR_ADDRESS))
//		throw_exception(EXCEPTION_MOTOR_CONTROL);
	
}
void motor_timer_restart(void)
{
	timer_start(&timer_motor);
	timer_enable_interrupt(&timer_motor,TIM_IT_Update);
	_motor_stat=MOTOR_STATE_RUNNING;
}
void motor_restart(void)
{
	int i;
	for(i=0;i<MODBUS_RETRY;i++)
	{
		if(motor_turn_forward_continuous(MOTOR_ADDRESS))
			break;
	}
	if(i>=MODBUS_RETRY)
		throw_exception(EXCEPTION_MOTOR_CONTROL);	

//	motor_params_restore();
	s_work_state.time_params_index=0;
	s_work_state.period=0;
	timer_disable_interrupt(&timer_motor,TIM_IT_Update);
	timer_set_counter(&timer_motor,1);
	timer_update(&timer_motor);
	for(i=0;i<MODBUS_RETRY;i++)
	{
		if(motor_set_work_freq(MOTOR_ADDRESS, s_time_params.item[0].work_freq))
			break;
	}
	if(i>=MODBUS_RETRY)
		throw_exception(EXCEPTION_MOTOR_CONTROL);	
	motor_timer_restart();
}

void motor_open_door(void)
{
	int i;
	__disable_irq();
	s_work_state.time_params_index=DOOR_FREQ_INDEX;	
	s_work_state.period=0;	
	__enable_irq();
	if(_motor_stat==MOTOR_STATE_STOP)
	{
		for(i=0;i<MODBUS_RETRY;i++)
		{
			if(motor_turn_forward_continuous(MOTOR_ADDRESS))
				break;
		}
		if(i>=MODBUS_RETRY)
			throw_exception(EXCEPTION_MOTOR_CONTROL);	
		motor_timer_restart();
	}
	for(i=0;i<MODBUS_RETRY;i++)
	{
		if(motor_set_work_freq(MOTOR_ADDRESS, s_time_params.item[DOOR_FREQ_INDEX].work_freq))
			break;
	}
	if(i>=MODBUS_RETRY)
		throw_exception(EXCEPTION_MOTOR_CONTROL);	
}
////从暂停状态恢复
//void motor_resume(void)
//{
//	motor_set_work_freq(MOTOR_ADDRESS, s_time_params.item[s_work_state.time_params_index].work_freq);
//	timer_enable_interrupt(&timer_motor,TIM_IT_Update);
//	timer_start(&timer_motor);
//	_motor_stat=MOTOR_STATE_RUNNING;
//}

void motor_stop(void)
{
	int i;
	for(i=0;i<MODBUS_RETRY;i++)
	{
		if(motor_turn_stop(MOTOR_ADDRESS))
			break;
	}
	if(i>=MODBUS_RETRY)
		throw_exception(EXCEPTION_MOTOR_CONTROL);	
	timer_stop(&timer_motor);
	timer_disable_interrupt(&timer_motor,TIM_IT_Update);
	timer_clear_interrupt_flag(&timer_motor);
	timer_set_counter(&timer_motor,0);
	timer_update(&timer_motor);
	s_work_state.time_params_index=-1;
	s_work_state.period=0;
	_motor_stat=MOTOR_STATE_STOP;
}
//暂停
void motor_pause(void)
{
	timer_stop(&timer_motor);
	timer_disable_interrupt(&timer_motor,TIM_IT_Update);
	_motor_stat=MOTOR_STATE_PAUSE;
}

//暂停
void motor_continue(void)
{
	timer_start(&timer_motor);
	timer_enable_interrupt(&timer_motor,TIM_IT_Update);
	_motor_stat=MOTOR_STATE_PAUSE;
}

void motor_tick(int ticks)
{
	static BOOL _door_open_set=FALSE;//说明开门信号已经获知并已经传递
	static int _signal_set=0;
	struct motor_control ctrl;
	if(board_voltage_signal())
	{
		_signal_set+=ticks;//防抖
		if(_signal_set>1000)
			board_relay_on(&gpio_relay1);
	}else{
		_signal_set=0;
	}
	ctrl.control_type= MOTOR_CTRL_SIGNAL;
	ctrl.val=0;
	if(board_door_open())
	{
		if(_door_open_set== FALSE)//如果已经设置过开门频率，则不再重新设置
		{
			ctrl.val|=MOTOR_DOOR_OPEN;
			_door_open_set=TRUE;
		}
	}
	else if(_door_open_set==TRUE)
	{
		_door_open_set=FALSE;
		ctrl.val|=MOTOR_DOOR_OPEN;
	}
//	if(board_item_ready())
//		ctrl.val|=MOTOR_ITEM_READY;
	if(ctrl.val==0)
		return;
	if(pdPASS!=xQueueSend(_motor_queue,&ctrl,0))
	{
		_door_open_set=_door_open_set?FALSE:TRUE;
	}
	
}

//说明：到位信号控制工件是否开始，开门信号触发最大频率输出
void motor_control(struct motor_control* ctrl)
{
	int i;
	switch(ctrl->control_type)
	{
		case MOTOR_CTRL_FREQ:
			if(!board_door_open())
			{
				for(i=0;i<MODBUS_RETRY;i++)
				{
					if(motor_set_work_freq(MOTOR_ADDRESS,ctrl->val))
					{
						break;
					}
				}
				if(i>=MODBUS_RETRY)
					throw_exception(EXCEPTION_MOTOR_CONTROL);
			}
			break;
		case MOTOR_CTRL_SIGNAL://控制信号：门，到位
			if(board_door_open())//门信号只控制暂停和继续
			{
				motor_open_door();
			}
			else 
			{
				//motor_set_work_freq(MOTOR_ADDRESS,s_time_params.item[0].work_freq);//开关门之后从头开始？？
				motor_restart();
			}
			break;
		case MOTOR_CTRL_COMPLETE:
			if(!board_door_open())//若门被打开，则应该进行开门后的动作，不应该停止
			{
			motor_stop();
			_motor_stat=MOTOR_STATE_STOP;
			//motor_set_work_freq(MOTOR_ADDRESS,s_work_params.idle_freq);
			//motor_restart();
			}
			break;
		case MOTOR_CTRL_GO_IDLE://开门后时间到，进入idle
			if(board_door_open())//若门已经关上则应该继续关门以后的操作，不应该进idle
			{
				motor_stop();
				_motor_stat=MOTOR_STATE_STOP;
//			for(i=0;i<MODBUS_RETRY;i++)
//			{
//				if(motor_set_work_freq(MOTOR_ADDRESS, ctrl->val))
//					break;
//			}
//			if(i>=MODBUS_RETRY)
//				throw_exception(EXCEPTION_MOTOR_CONTROL);	
			}
			break;
		default:
			break;
	}
}


//中断每0.1s一次
//尚需修改：保存当前状态
void motor_control_isr(void)
{
	//mem_address_t address;
	struct motor_control ctrl;
	portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
	timer_clear_interrupt_flag(&timer_motor);
	TEST_OUT;
	s_work_state.period++;
	if(s_work_state.period<CURRENT_PERIOD_TIME)
	{//当前工序时间未到
		return;
	}
	s_work_state.period=0;
	s_work_state.time_params_index++;
	store_work_state(&s_work_state);
	if( s_work_state.time_params_index>DOOR_FREQ_INDEX)
	{
		ctrl.control_type=MOTOR_CTRL_GO_IDLE;
		ctrl.val=s_work_params.idle_freq;
		timer_stop(&timer_motor);
	}else if(CURRENT_PERIOD_TIME ==0 || s_work_state.time_params_index>=TIME_SLOTS_COUNT ||s_work_state.time_params_index==-1)
	{
					//当前任务结束
			ctrl.control_type=MOTOR_CTRL_COMPLETE;
			ctrl.val=1;//无所谓的值
			timer_stop(&timer_motor);
	}
	else
	{
		TEST_OUT;
		TEST_OUT;
		ctrl.control_type=MOTOR_CTRL_FREQ;
		ctrl.val=CURRENT_FREQ;
	}
	xQueueSendFromISR(_motor_queue,&ctrl,&xHigherPriorityTaskWoken);
//	address=mem_alloc(sizeof(struct motor_control),&ctrl);
//	if(address==INVALID_ADDRESS)
//	{
//		throw_exception(EXCEPTION_MEM_ALLOCATE);
//		return;
//	}
//	if(pdPASS!=xQueueSendFromISR(_motor_queue,&ctrl,&xHigherPriorityTaskWoken))
//	{
//		mem_fetch(address,NULL);
//	}
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

void TIM2_IRQHandler(void)
{
	motor_control_isr();
}


void motor_ctrl_task(void *p)//电机控制中有阻塞，所以只能单独开一个任务
{
	static struct motor_control ctrl;
	_motor_queue=xQueueCreate( 20, sizeof( struct motor_control) );
	if(!_motor_queue)
		return;
	//motor_control_init();
	motor_restart();
	while(1)
	{
		xQueueReceive(_motor_queue,&ctrl,portMAX_DELAY);
		motor_control(&ctrl);
	}
	
}

