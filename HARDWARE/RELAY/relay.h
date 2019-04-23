
#ifndef __RELAY_H
#define __RELAY_H


#ifdef  __cplusplus
	extern "C" {
#endif




#define RELAY PBout(2)


void Relay_Init (void);

void Relay_On (void);
void Relay_Off (void);

#ifdef  __cplusplus
	}
#endif




#endif



