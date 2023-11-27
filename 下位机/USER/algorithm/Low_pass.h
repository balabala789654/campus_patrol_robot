#ifndef _LOW_PASS_H
#define _LOW_PASS_H

#include "Low_pass.h"
#include "Remote_Control.h"
#include "status.h"
#include "dead_zone.h"

typedef struct 
{
	float ch[5];
}LOW_PASS;

LOW_PASS Low_pass_output(DEAD_ZONE (*input)(RC_ctrl_t *_rc, REMOTE (*callback)(RC_ctrl_t *_rc)));

#endif

