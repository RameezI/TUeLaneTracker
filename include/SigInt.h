#ifndef SIG_INT_H
#define SIG_INT_H
#include <signal.h>
#include "string.h"

enum SigStatus {INIT, STOP, FAILURE=-1};
class SigInt
{
public:
	static SigStatus sStatus;
	SigInt();
	~SigInt();
	
	static void	handler(int);
};

#endif // SIG_INT_H
