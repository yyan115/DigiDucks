#include <iostream>
#include "CustomerWalkState.h"


void CustomerWalkState::Enter()
{
	std::cout << "Customer enters Walk State" << std::endl;
}


void CustomerWalkState::Update()
{

}

void CustomerWalkState::Exit()
{
	std::cout << "Customer exits Walk State" << std::endl;
}