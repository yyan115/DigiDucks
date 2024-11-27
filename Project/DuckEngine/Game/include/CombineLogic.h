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


bool canCombine(ItemType lhs, ItemType rhs);

std::pair<int, ItemType> combineObjects(std::pair<int, ItemType> lhs, std::pair<int, ItemType> rhs);