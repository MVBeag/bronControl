/**
 * @mainpage BronCon controlling framework for Bron Devices via WiFi.
 * @authors Michael Voegel, Jörg Roth, Bron Elektronik AG, Allschwil
 *
 * The framework source code consist of two differnet parts.
 *
 * 1. everything in namespace Framework, this is the basic driver framework for Bron devices. It provides access to
 * bonjours zeroconf daemon to find devices, it hase an iterface for storing configuration, it provides so called studios (entities
 * with the same wifi address belong per definition to the same studio!). Entities with the same local address belong to the same group
 * within the studio. A group let you ajdust some setting group wise (e.g. if you change energy on a group, every Lamp in the group is
 * changed according the group change.
 *
 * the studioMgr holds the studios, the groupMgr hold the groups within a studio, every studio has a deviceMgr to get the devices.
 * every device consits of a parameter list. Parameter is responsible to communicate with the real device! (more generic). If there
 * are some device adjustments, needing more than one parameter, you have to register a service in this device.
 *
 *
 *
 *
 * 2. evereything with the namespace QMLInterface is used to expose the Framework to qml. We decided to use a proxy concept, because
 * Framework is cleaner. If you don't use QML, you shouldn't use this namespace
 *
 */


#ifndef DOXYGENMAIN_H
#define DOXYGENMAIN_H

#endif // DOXYGENMAIN_H
