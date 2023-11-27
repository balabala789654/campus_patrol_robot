#include "Low_pass.h"
#include "Remote_Control.h"
#include "status.h"
#include "dead_zone.h"

static LOW_PASS Low_pass_output_last;
static float p = 0.05;
LOW_PASS Low_pass_output(DEAD_ZONE (*input)(RC_ctrl_t *_rc, REMOTE (*callback)(RC_ctrl_t *_rc)))
{
	LOW_PASS _low_pass;
	for(int i=0; i<5; i++)
		_low_pass.ch[i] = ((*input)(&rc_ctrl, status_remote_callback).ch[i]*p) + (Low_pass_output_last.ch[i]*(1-p));
	Low_pass_output_last = _low_pass;
	return _low_pass;
}


