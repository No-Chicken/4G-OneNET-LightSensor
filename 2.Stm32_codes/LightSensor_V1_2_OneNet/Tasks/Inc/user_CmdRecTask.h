#ifndef __USER_CMDRECTASK_H__
#define __USER_CMDRECTASK_H__

#ifdef __cplusplus
extern "C" {
#endif

void CmdRecTask(void *argument);

#define K_MAX 2.0
#define K_MIN 0.5
#define B_MAX 2000
#define B_MIN -2000

extern float OFFSET_k;
extern int16_t OFFSET_b;

#ifdef __cplusplus
}
#endif

#endif

