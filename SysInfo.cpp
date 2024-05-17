#include "SysInfo.h"

#include <utility>

SysInfo::SysInfo(std::string OSFamily) :
    OSFamily(std::move(OSFamily))
{

}