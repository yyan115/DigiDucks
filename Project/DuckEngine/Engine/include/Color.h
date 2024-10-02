/******************************************************************************/
/*!
\file       Color.h
\author     Yan Yu, y.yan, 2301213
\par        y.yan@digipen.edu
\date       October 3 2024
\brief      Defines the Color struct, representing an RGBA color used for
            rendering objects in the game engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

/// <summary>
/// Represents a color with red, green, blue, and alpha (transparency) components.
/// Each component is a floating-point value typically ranging from 0 to 255.
/// </summary>
struct Color {
    /// <summary>
    /// The red component of the color.
    /// </summary>
    float r;

    /// <summary>
    /// The green component of the color.
    /// </summary>
    float g;

    /// <summary>
    /// The blue component of the color.
    /// </summary>
    float b;

    /// <summary>
    /// The alpha (transparency) component of the color.
    /// </summary>
    float a;

    /// <summary>
    /// Constructs a color with the specified red, green, blue, and alpha values.
    /// Default values are set to 255 for all components, representing a fully opaque white color.
    /// </summary>
    /// <param name="r">Red component, defaults to 255.</param>
    /// <param name="g">Green component, defaults to 255.</param>
    /// <param name="b">Blue component, defaults to 255.</param>
    /// <param name="a">Alpha component, defaults to 255.</param>
    Color(float r = 255, float g = 255, float b = 255, float a = 255)
        : r(r), g(g), b(b), a(a) {}
};
