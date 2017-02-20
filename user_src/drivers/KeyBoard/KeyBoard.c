#include "keyboard.h"
#include "board/board.h"
static int32u _lastBtnVal=0,_currentBtnVal=0,_lastBtnState=0;
static struct key_event _key_event;
static u32 _btnTimer=0;
extern int32u board_key_get(void);
void key_board_tick(u16 tick)
{
	_currentBtnVal=board_key_get();
	if(_currentBtnVal==_lastBtnVal)
	{
		_btnTimer+=tick;
		if(_btnTimer==tick*2)//消抖
		{
			_key_event.modify_val= _currentBtnVal^_lastBtnState;//找出状态有变化的键值
			_key_event.key_action=(_key_event.modify_val & _currentBtnVal)?KEY_PRESS:KEY_RELEASE;
			_key_event.key_value=_currentBtnVal;
			//event_send(KEY_EVENT_ID,mem_alloc(sizeof(struct key_event),&_key_event));
			_lastBtnState=_currentBtnVal;
		}
	}
	else
	{
		_lastBtnVal=_currentBtnVal;
		_btnTimer=0;
	}
}

int32u key_board_get(int32u *pTimer)
{
	if(pTimer)
		*pTimer=_btnTimer;
	return _currentBtnVal;
}


