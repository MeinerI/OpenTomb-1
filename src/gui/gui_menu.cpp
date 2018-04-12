
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "../core/system.h"
#include "../core/vmath.h"
#include "gui.h"
#include "gui_menu.h"


static void Gui_SetupMenuObj(gui_object_p root)
{
    root->w = screen_info.w * 0.5;
    root->h = screen_info.h * 0.6;
    root->x = screen_info.w * 0.25;
    root->y = screen_info.h * 0.2;
    root->color_border[0] = 65;
    root->color_border[1] = 21;
    root->color_border[2] = 22;
    root->color_border[3] = 255;
    root->color_background[0] = 32;
    root->color_background[1] = 21;
    root->color_background[2] = 22;
    root->color_background[3] = 126;
    root->border_width = 4;

    root->spacing = 4;
    root->margin_top = 8;
    root->margin_bottom = 8;
    root->margin_left = 8;
    root->margin_right = 8;

    root->flags.draw_border = 0x01;
    root->flags.draw_background = 0x01;
    root->flags.clip_children = 0x00;
    root->flags.h_content_align = GUI_ALIGN_CENTER;
    root->flags.v_content_align = GUI_ALIGN_TOP;
    root->flags.layout = GUI_LAYOUT_VERTICAL;
    root->flags.fit_inside = 0x01;
}

gui_object_p Gui_BuildSavesList()
{
    gui_object_p root = Gui_CreateObject();
    Gui_SetupMenuObj(root);

    gui_object_p obj = Gui_CreateChildObject(root);
    obj->h = 40;
    obj->flags.draw_border = 0x01;
    Gui_SetObjectLabel(obj, "Load game:", 1, 1);
    obj->border_width = 4;
    obj->flags.h_content_align = GUI_ALIGN_CENTER;
    obj->flags.v_content_align = GUI_ALIGN_CENTER;
    obj->flags.draw_label = 0x01;
    obj->flags.draw_border = 0x01;
    obj->flags.fixed_h = 0x01;
    obj->line_height = 0.8;

    gui_object_p cont = Gui_CreateChildObject(root);
    cont->w = root->w - root->margin_left - root->margin_right;
    cont->h = root->h - obj->h - root->margin_top - root->margin_bottom;

    cont->border_width = 0;
    cont->flags.clip_children = 0x01;
    cont->flags.draw_background = 0x00;
    cont->flags.draw_border = 0x00;
    cont->flags.layout = GUI_LAYOUT_VERTICAL;
    cont->flags.h_content_align = GUI_ALIGN_CENTER;
    cont->weight_y = 1;

    file_info_p list = Sys_ListDir("save", NULL);
    for(file_info_p it = list; it; it = it->next)
    {
        if(!it->is_dir)
        {
            obj = Gui_CreateChildObject(cont);
            obj->h = 32;
            obj->flags.draw_border = (obj->prev) ? (0x00) : (0x01);
            obj->border_width = 3;
            obj->color_border[0] = 220;
            obj->color_border[1] = 211;
            obj->color_border[2] = 242;
            obj->color_border[3] = 255;

            Gui_SetObjectLabel(obj, it->name, 2, 2);
            obj->flags.h_content_align = GUI_ALIGN_CENTER;
            obj->flags.v_content_align = GUI_ALIGN_CENTER;
            obj->flags.draw_label = 0x01;
            obj->flags.fixed_h = 0x01;
            obj->line_height = 0.8;
        }
    }
    Sys_ListDirFree(list);

    Gui_LayoutObjects(root);

    return root;
}

gui_object_p Gui_ListSaves(gui_object_p root, int dy)
{
    gui_object_p ret = NULL;
    if(root && root->childs && root->childs->next)
    {
        gui_object_p cont = root->childs->next;
        for(gui_object_p obj = cont->childs; obj; obj = obj->next)
        {
            if(obj->flags.draw_border)
            {
                ret = obj;
                if((dy > 0) && obj->prev)
                {
                    ret = obj->prev;
                    obj->flags.draw_border = 0x00;
                    ret->flags.draw_border = 0x01;
                }
                else if((dy < 0) && obj->next)
                {
                    ret = obj->next;
                    obj->flags.draw_border = 0x00;
                    ret->flags.draw_border = 0x01;
                }
                break;
            }
        }
        Gui_EnsureVisible(ret);
    }
    return ret;
}
