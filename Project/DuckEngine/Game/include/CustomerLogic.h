#include "DuckEngine.h"
#include "StateMachine.h"
#include "CustomerIdleState.h"

class CustomerLogic : public GameLogic
{
public:
	StateMachine stateMachine;

	CustomerIdleState IdleState;


};