/*****************************************************************************
 * Topo
 *   Multi-platform system topology abstraction library.
 *****************************************************************************
 * Authored by Samuel Grossman
 * Department of Electrical Engineering, Stanford University
 * Copyright (c) 2016-2017
 *************************************************************************//**
 * @file topo.c
 *   Implementation of all external API functions.
 *****************************************************************************/

#include "../topo.h"

#include <hwloc.h>
#include <stdlib.h>
#include <stdint.h>


// -------- LOCALS --------------------------------------------------------- //

/// Holds the `hwloc` system topology object.
/// Exposes it as a global variable that can be lazily initialized and freed whenever required.
static hwloc_topology_t topoSystemTopology = NULL;


// -------- FUNCTIONS ------------------------------------------------------ //
// See "topo.h" for documentation.

uint32_t topoGetLibraryVersion(void)
{
    return TOPO_LIBRARY_VERSION;
}

// --------

hwloc_topology_t topoGetSystemTopologyObject(void)
{
    if (NULL == topoSystemTopology)
    {
        // Create and load the hardware topology of the current system.
        if (0 != hwloc_topology_init(&topoSystemTopology))
            topoSystemTopology = NULL;
        
        if (NULL == topoSystemTopology || 0 != hwloc_topology_load(topoSystemTopology))
            topoSystemTopology = NULL;
    }
    
    return topoSystemTopology;
}

// --------

hwloc_obj_t topoGetNUMANodeObjectAtIndex(uint32_t numaNodeIndex)
{
    hwloc_topology_t topology = topoGetSystemTopologyObject();
    hwloc_obj_t numaNodeObject = NULL;
    
    if (NULL == topology)
        return NULL;
    
    // Attempt to obtain the NUMA node object directly.
    numaNodeObject = hwloc_get_obj_by_type(topology, HWLOC_OBJ_NUMANODE, numaNodeIndex);
    
    if (NULL == numaNodeObject && 0 == numaNodeIndex)
    {
        // Sometimes a NUMA node object does not exist in a single-node system.
        // If requesting the first node and an object does not exist, retrieve instead the first package.
        numaNodeObject = hwloc_get_obj_by_type(topology, HWLOC_OBJ_PACKAGE, 0);
    }
    
    return numaNodeObject;
}

// --------

int32_t topoGetNUMANodeOSIndex(uint32_t numaNodeIndex)
{
    int32_t numaNodeOSIndex = -1;
    hwloc_obj_t numaNodeObject = topoGetNUMANodeObjectAtIndex(numaNodeIndex);
    
    if (NULL != numaNodeObject)
    {
        // In some cases a single-node machine has no NUMA node object, so ensure to return 0 in that case.
        if (1 == hwloc_bitmap_weight(numaNodeObject->nodeset))
            numaNodeOSIndex = numaNodeObject->os_index;
        else
            numaNodeOSIndex = 0;
    }
    
    return numaNodeOSIndex;
}

// --------

void topoDestroySystemTopologyObject(void)
{
    if (NULL != topoSystemTopology)
    {
        hwloc_topology_destroy(topoSystemTopology);
        topoSystemTopology = NULL;
    }
}

// --------

uint32_t topoGetSystemNUMANodeCount(void)
{
    hwloc_topology_t topology = topoGetSystemTopologyObject();
    uint32_t numNumaNodes = 0;

    if (NULL != topology)
    {
        numNumaNodes = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_NUMANODE);
        
        if (0 == numNumaNodes)
            numNumaNodes = 1;
    }    
    
    return numNumaNodes;
}

// --------

uint32_t topoGetNUMANodePhysicalCoreCount(uint32_t numaNodeIndex)
{
    hwloc_topology_t topology = topoGetSystemTopologyObject();
    hwloc_obj_t numaNodeObject = NULL;
    
    if (NULL == topology)
        return 0;
    
    numaNodeObject = topoGetNUMANodeObjectAtIndex(numaNodeIndex);
    if (NULL == numaNodeObject)
        return 0;
    
    return hwloc_get_nbobjs_inside_cpuset_by_type(topology, numaNodeObject->cpuset, HWLOC_OBJ_CORE);
}

// --------

uint32_t topoGetNUMANodeLogicalCoreCount(uint32_t numaNodeIndex)
{
    hwloc_topology_t topology = topoGetSystemTopologyObject();
    hwloc_obj_t numaNodeObject = NULL;
    
    if (NULL == topology)
        return 0;
    
    numaNodeObject = topoGetNUMANodeObjectAtIndex(numaNodeIndex);
    if (NULL == numaNodeObject)
        return 0;
    
    return hwloc_get_nbobjs_inside_cpuset_by_type(topology, numaNodeObject->cpuset, HWLOC_OBJ_PU);
}
