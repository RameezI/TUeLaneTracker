#ifndef SIGINIT_H
#define SIGINIT_H
#include <signal.h>
#include "string.h"

enum SigStatus {INIT, STOP, FAILURE=-1};
class SigInit
{
public:
	static SigStatus sStatus;
	SigInit();
	~SigInit();
	
private:
	static void	handler(int);
};

#endif // SIGINIT_H
