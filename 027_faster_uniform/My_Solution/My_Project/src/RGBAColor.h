#pragma once

#include "Shader.h"
#include <iostream>


class RGBAColor
{
private:
    float m_red = 0.0f;
    float m_green = 0.0f;
    float m_blue = 0.0f;
    float m_alpha = 1.0f;

    //                        <- in positive direction
    short m_red_direction = 1;
    short m_green_direction = 1;
    short m_blue_direction = 1;
    short m_alpha_direction = 1;

private:
    void change_color_value(float& color_value, short& direction, float increment)
    {
        // for this to work, increment must be reasonable
        if ((color_value + increment) > 1)
        {
            direction = -1;
        }
        else if ((color_value - increment) < 0)
        {
            direction = 1;
        }

        color_value = color_value + direction * increment;

    }

public:
    RGBAColor(float red, float green, float blue, float alpha)
        : m_red(red), m_green(green), m_blue(blue), m_alpha(alpha) {}

    //                            <- doesn't change any class variables
    void set_color(Shader& shader) const
    {
        shader.set_uniform_4f("u_color", m_red, m_green, m_blue, m_alpha);
    }

    void change_color(float increment_r, float increment_g, float increment_b, float increment_a)
    {
        change_color_value(m_red, m_red_direction, increment_r);
        change_color_value(m_green, m_green_direction, increment_g);
        change_color_value(m_blue, m_blue_direction, increment_b);
        change_color_value(m_alpha, m_alpha_direction, increment_a);

#ifdef DEBUG
        std::cout << "r: " << m_red << "\tg: " << m_green << "\tb: " << m_blue << "\ta: " << m_alpha << std::endl;
#endif
    }

    void rotate_color(Shader& shader, float increment_r, float increment_g, float increment_b, float increment_a)
    {
        change_color(increment_r, increment_g, increment_b, increment_a);
        set_color(shader);
    }
};
