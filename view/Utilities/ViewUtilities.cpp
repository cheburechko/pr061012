/*
    Copyright (c) 2012, pr061012 Team.
    See the COPYING file for copying permission.
*/

#include "ViewUtilities.h"

void ViewUtilities::renderText(double x, double y, float size, std::string msg,
                               double line_size)
{
    for(uint i = 0; i < msg.length(); ++i)
    {
        if(msg[i] == '\t')msg[i] = ' ';
    }

    const char* txt = msg.c_str();

    glcScale(size, size);

    int i = 0;
    int begin = i;

    while (txt[i] != '\0')
    {
        if(txt[i] == '\n')
        {
            glcRenderCountedString(i - begin, txt + begin);

            begin = i;
            y -= line_size;
        }

        glRasterPos2d(x, y);


        i++;
    }

    glcRenderCountedString(i - begin, txt + begin);

    glcScale(1.f/size, 1.f/size);
}

void ViewUtilities::glRectf_blend(float x1, float y1, float x2, float y2,
                                  bool filled)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (filled) glRectf(x1, y1, x2, y2);
        else
        {
            glBegin(GL_LINES);
                glVertex2d(x1, y1);
                glVertex2d(x1, y2);
                glVertex2d(x2, y2);
                glVertex2d(x2, y1);
            glEnd();
        }
    glDisable(GL_BLEND);
}

void ViewUtilities::glCirclef_blend(float x, float y, float rad, bool filled)
{
    double angle;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (filled) glBegin(GL_TRIANGLE_FAN);
    else        glBegin(GL_LINE_LOOP);

        for (int i = 0; i < 100; i++) {
            angle = 2.0 * i * M_PI / 100;
            glVertex2d((x + cos(angle) * rad),
                       (y + sin(angle) * rad));
        }
    glEnd();

    glDisable(GL_BLEND);
}