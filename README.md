# _numSCAL_: An Open Source Numerical Simulator for Special Core Analysis Laboratory

## Introduction

_numSCAL_ is a cross-platform pore-scale numerical simulator developed at Heriot-Watt University. The software has been implemented using C++ and Qt technologies with the idea of providing a solid platform to investigate multi-phase flow in porous media. A Graphical User Interface (GUI) has been also implemented to allow fast access to the underlying modules and to provide a real-time visualisation of the simulated flow processes (although a console version can be readily generated if the simulations are intended to run on a Linux supercomputer).

![numSCAL Screenshot](https://image.ibb.co/j0Tc3R/screenshot.png)

## Supported Networks

We model the porous medium in _numSCAL_ as a network of capillary elements, partitioned into nodes (pore bodies) and throats, with the nodes being linked by the throats. To each network element (a node or throat) we attach a range of geometric attributes (inscribed radius, length, volume, inter alia) that enables us to consider a range of different network modelling philosophies. At its most simplistic, we can use the framework to model the pore space as a simple 3D scaffold of interconnected capillary elements. At the other extreme, we can also use the methodology to model multi-phase flow through networks that are topologically and geometrically complex, where the pore space is comprised of irregular pore elements with distributed connectivity. 
Currently, _numSCAL_ offers full support of the network file format provided by the Imperial College database. Similar network files can be readily imported into the software.

![Supported Networks](https://image.ibb.co/dWz6xm/networks.png)

## Features

_numSCAL_ includes several flow models that cover a wide range of pore-scale phenomena. The modules included in the basic version include:

* Quasi-steady-state Two-phase Flow Model:

This module simulates quasi-steady state multiphase flow in both regular and microCT digital networks. A full cycle of displacements can be simulated: primary drainage, spontaneous imbibition, forced water injection, spontaneous oil invasion and secondary oil drainage. Snap-off mechanisms, cooperative pore body filling and film swelling are supported in triangular capillaries. Output results include capillary pressure and relative permeabilities curves. 

* Unsteady-state Two-phase Flow Model:

This model is appropriate when the balance between capillary and viscous forces is under investigation in two-phase systems. A dynamic approach is implemented to simulate fluid injection into a porous medium under constant flow rate. The algorithm solves the pressure field in the underlying network and updates phase saturations accordingly. Output results include water staturations, pressure gradient, fractional flows and  relative permeabilities curves. The model also supports "simulation replay" by saving network snapshots along the simulation and rendering them at post-processing stages.
(PS: Film support is not enabled in the basic version.)

![Viscous Fingering](https://image.ibb.co/bVraq6/USS.png)

* Tracer Flow Model:

This model is appropriate for investigating the convective and diffusive flow of a tracer in porous media. Although no output results are currently generated, the module is an ideal starting point to study phenomena that involve tracer flow modelling (i.e. surfactant flow, polymer flow).

![Tracer Flow](https://image.ibb.co/ddkeHm/tracer.png)

Other modules that are supported by numSCAL but not enabled in the basic version include:

* Quasi-steady State Three-Phase Flow
* Unsteady-State Flow with Film Support
* Depletion Processes
* Ganglia Mobilisation Processes
* Surfactant Flow
* Low Salinity Water Injection
* Polymer Injection

## Graphical Engine

An OpenGL graphical engine has been implemented into _numSCAL_ to provide real-time high quality visualisation of the running simulations. The engine uses advanced shading techniques (geometry shaders, sphere imposters, cylinder impostors) to minimise the CPU-GPU interaction and provide a smooth and real-time visualisation of large networks. 

![Core Scale Network](https://image.ibb.co/hGJc3R/drainage.png)

Several options are available to display the underlying network:

* Visualisation of separate phases.
* Visualisation of only oil-wet or water-wet elements.
* Changing phase colors.
* Slicing the underlying network to uncover obstructed elements.
* Real-time generation of capillary pressure, saturation and relative permeability data.
