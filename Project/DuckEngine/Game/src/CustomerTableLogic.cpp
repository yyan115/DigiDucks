/******************************************************************************/
/*!
\file       CustomerTableLogic.cpp
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       April 6 2025
\brief      Implements the CustomerTableLogic class. This logic component is
            designed to attach to table entities and can track associated seat
            or customer state. Currently provides basic lifecycle hooks (Start,
            Update, FixedUpdate) and supports deep cloning via the GameLogic
            interface.

            Future expansion may include customer interaction handling or
            table-based event responses.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "CustomerTableLogic.h"

//class CustomerTable{
//public:
//	CustomerLogic* customer;
//};