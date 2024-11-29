/******************************************************************************/
/*!
\file       CombineLogic.h
\author     Ernest Ho, h.yonghengernest, 2301223
\par        h.yonghengernestt@digipen.edu
\date       November 27 2024
\brief      Declartion of all Combine Logic functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "IngredientType.h"


/****************************************************************
* @brief Check if two items can be combined
* 
* @param lhs - the first item
* @param rhs - the second item
* 
* @return true if the two items can be combined, false otherwise*
* ****************************************************************/
bool canCombine(ItemType lhs, ItemType rhs);

/****************************************************************
* @brief Combine two items
* 
* @param lhs - the first item
* @param rhs - the second item
*	
* @return the combined item
* ****************************************************************/
std::pair<int, ItemType> combineObjects(std::pair<int, ItemType> lhs, std::pair<int, ItemType> rhs);