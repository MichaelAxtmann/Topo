/*****************************************************************************
 * Topo
 *   Multi-platform system topology abstraction library.
 *****************************************************************************
 * Authored by Samuel Grossman
 * Department of Electrical Engineering, Stanford University
 * Copyright (c) 2016
 *************************************************************************//**
 * @file topo.h
 *   Declaration of all external API functions.
 *****************************************************************************/

#pragma once

#include <hwloc.h>


// -------- FUNCTIONS ------------------------------------------------------ //
#ifdef __cplusplus
extern "C" {
#endif

/// Retrieves and returns the system topology object handle.
/// This function will lazily instantiate the system topology object if it is not yet instantiated.
/// @return System topology object handle from `hwloc`, or `NULL` in the event of an instantiation failure.
hwloc_topology_t topoGetSystemTopologyObject(void);

/// Retrieves and returns the system topology object that corresponds to the specified NUMA node, by `hwloc` index.
/// This is useful because some single-node systems lack NUMA node objects entirely.
/// @param [in] numaNodeIndex `hwloc` index of the NUMA node of interest.
/// @return NUMA node object handle from `hwloc`, or `NULL` in the event of a failure (object does not exist, instantiation failure, etc.).
hwloc_obj_t topoGetNUMANodeObjectAtIndex(uint32_t numaNodeIndex);

/// Destroys and frees all system resources held to maintain the `hwloc` system topology.
/// This function is idempotent and can be invoked anytime outside of a code region parallelized by this library.
void topoDestroySystemTopologyObject(void);

/// Retrieves the number of NUMA nodes in the system.
/// NUMA nodes are identified by a zero-based index up to 1 less than the number returned by this function.
/// @return Number of NUMA nodes in the system, or 0 in the event of an error.
uint32_t topoGetSystemNUMANodeCount(void);

/// Retrieves the number of physical cores on the specified NUMA node.
/// NUMA nodes are identified by a zero-based index.
/// @return Number of physical cores on the specified NUMA node, or 0 in the event of an error.
uint32_t topoGetNUMANodePhysicalCoreCount(uint32_t numaNodeIndex);

/// Retrieves the number of threads (logical cores) available on the specified NUMA node.
/// NUMA nodes are identified by a zero-based index.
/// @return Number of logical cores available on the specified NUMA node, or 0 in the event of an error.
uint32_t topoGetNUMANodeLogicalCoreCount(uint32_t numaNodeIndex);

#ifdef __cplusplus
}
#endif
