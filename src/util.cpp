#include "util.h"

QString Util::getPermissionString(QFileDevice::Permissions permissions)
{
    QString permissionString = "";

    if (permissions & QFileDevice::ReadUser)
        permissionString.append("r");
    else
        permissionString.append("-");

    if (permissions & QFileDevice::WriteUser)
        permissionString.append("w");
    else
        permissionString.append("-");

    if (permissions & QFileDevice::ExeUser)
        permissionString.append("x");
    else
        permissionString.append("-");

    if (permissions & QFileDevice::ReadGroup)
        permissionString.append("r");
    else
        permissionString.append("-");

    if (permissions & QFileDevice::WriteGroup)
        permissionString.append("w");
    else
        permissionString.append("-");

    if (permissions & QFileDevice::ExeGroup)
        permissionString.append("x");
    else
        permissionString.append("-");

    if (permissions & QFileDevice::ReadOwner)
        permissionString.append("r");
    else
        permissionString.append("-");

    if (permissions & QFileDevice::WriteOwner)
        permissionString.append("w");
    else
        permissionString.append("-");

    if (permissions & QFileDevice::ExeOwner)
        permissionString.append("x");
    else
        permissionString.append("-");

    return permissionString;
}
