# numSCAL: An Open Source Numerical Special Core Analysis Laboratory

## Introduction

_numSCAL_ is a pore-scale numerical simulator developed at Heriot-Watt University as part of a PhD project. The software has been implemented using C++ and Qt technologies and can be used on Windows, MacOS and Linux machines. Moreover, a Graphical User Interface (GUI) has been coded to allow fast access to the features provided by numSCAL (although a console version is also available if the simulations are intended to run on a Linux supercomputer).

![](https://lh3.googleusercontent.com/sqHIpd7fucnw3dmTlVQyYdY1a708vaE-L3bfZ4YQZ3FM590Y22hXPJt32PpgkjKIaTAPoGst0lhXjts=w1920-h925-rw)

## Supported Networks

We model the porous medium in _numSCAL_ as a network of capillary elements, partitioned into nodes (pore bodies) and bonds (pore throats), with the nodes being linked by the bonds. To each network element (a node or bond) we attach a range of geometric attributes (inscribed radius, length, volume, inter alia) that enables us to consider a range of different network modelling philosophies. At its most simplistic, we can use the framework to model the pore space as a simple 3D scaffold of interconnected cylindrical bonds; an approach that is useful for obtaining rapid, qualitative results, as fewer pore-scale entities need to be considered. At the other extreme, we can also use the methodology to model multi-phase flow through networks that are topologically and geometrically complex, where the pore space is comprised of irregular pore elements with distributed connectivity. 

![](https://lh6.googleusercontent.com/YGopaBj9IsPQgJ7eCeEp2I7zfJMVTLMe5Zq-ioL5Jae37A7DhP1rQGwIWhDnoFvyRbh5IvET0ktPIpE=w1920-h925)

## Features

_numSCAL_ includes several flow models that cover a wide range of pore-scale phenomena and is briefly discussed below.
* Quasi-steady state two-phase and three-phase flow:

This model simulates quasi-steady state multiphase flow in both regular and microCT digital networks. By quasi steady-state we mean that phase saturations are assumed to be invariant with time at each fixed capillary pressure value and only change if the controlling capillary pressure changes. A full cycle of displacements can be simulated: primary drainage, primary imbibition, secondary drainage, secondary imbibition and tertiary drainage. Snap-off mechanisms, cooperative pore body filling and film flow are supported in triangular capillaries. 
When three phases are present, a graph-theory-based approach has been implemented to simulate the multi-displacement chains occurring at the pore-scale. Three-phase contact angles are used and spreading phenomena have been incorporated using geometrical conditions corresponding to triangular cross-sections. This model can be used to simulate quasi-static flow in any three-phase configuration, and three-phase saturation and relative permeability curves can be generated. WAG simulations are also supported. 

* Unsteady-state drainage model:

This model is appropriate when the balance between capillary and viscous forces is under investigation in two-phase systems. A dynamic approach is implemented to simulate fluid injection into a porous medium under either constant flow rate or constant differential pressure conditions. The algorithm solves the pressure field in the underlying network and updates phase saturations accordingly. 

* Unsteady-state ganglion model:

This model provides valuable insights into the conditions under which it is likely to observe ganglia displacements during a flood. The model extends the dynamic approach to solve the pressure field and update phase saturations in the network. Two fluids can simultaneously flow through a capillary element and no distinction is made between the invading and the defending phase. The model is appropriate when investigating high rate injection scenarios or scenarios characterised by low capillarity and can estimate the additional hydrocarbon recovery due to ganglia mobilisation. 

* Unsteady-state EOR model:

This model is based on the unsteady-state drainage model and is useful for simulating EOR processes at the pore-scale and their manifestations at the core-scale. Low salinity water, surfactant and polymer injection protocols are supported. The model is useful for investigating the efficiency of EOR techniques and estimate the additional recovery they could eventually yield. This model will be discussed in detail in this paper.

* Two-phase and three-phase depletion:

This model is used to simulate depletion experiments, where the stages of the depletion are implemented – this includes nucleation, diffusion, gas growth and gas migration. When depletion is simulated in a water-flooded system, an alternative three-phase model based on graph theory is used to simulate the interactions and chain-displacements between gas, oil and water. Oil spreading and film flow are also considered in this model.

Only quasi-steady state two-phase flow is supported in the basic version of numSCAL.



## Graphical Engine

A new graphical engine based on OpenGL has been developed in _numSCAL_ to provide real-time visualisation of the running simulations. Several options are available to display the underlying network:

* Visualisation of a single phase.
* Visualisation of only oil-wet or water-wet elements.
* Visualisation of one or multiple tracers during EOR processes (i.e. low salinity water, polymer, etc.).
* Slicing the underlying network to uncover obstructed elements.
* Real-time generation of capillary pressure, saturation and relative permeability data.
