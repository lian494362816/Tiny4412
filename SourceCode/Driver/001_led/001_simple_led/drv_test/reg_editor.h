#ifndef __REG_EDITOR_H
#define __REG_EDITOR_H

#include <asm-generic/ioctl.h>


typedef struct reg_editor_t{
    unsigned int addr;
    unsigned int val;
}reg_editor;

#define REG_EDITOR_MAGIC 'r'

#define REG_EDITOR_R8  _IOR(REG_EDITOR_MAGIC, 0, reg_editor)
#define REG_EDITOR_R16 _IOR(REG_EDITOR_MAGIC, 1, reg_editor)
#define REG_EDITOR_R32 _IOR(REG_EDITOR_MAGIC, 2, reg_editor)

#define REG_EDITOR_W8  _IOW(REG_EDITOR_MAGIC, 3, reg_editor)
#define REG_EDITOR_W16 _IOW(REG_EDITOR_MAGIC, 4, reg_editor)
#define REG_EDITOR_W32 _IOW(REG_EDITOR_MAGIC, 5, reg_editor)

#endif
