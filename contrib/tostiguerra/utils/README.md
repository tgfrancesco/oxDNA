# ANNaMo v2

This branch contains ongoing development of ANNaMo v2, extending the
ANNaMo coarse-grained model for DNA/RNA folding and self-assembly with a new
hybridisation-dependent semiflexibility interaction.

ANNaMo was introduced in:

F. Tosti Guerra, E. Poppleton, P. Šulc, L. Rovigatti,  
*ANNaMo: Coarse-grained modelling for folding and assembly of RNA and DNA systems*,  
J. Chem. Phys. 160, 205102 (2024).  
DOI: 10.1063/5.0202829

General ANNaMo documentation and simulation utilities are available in the
[official oxDNA repository](https://github.com/lorenzo-rovigatti/oxDNA/tree/master/contrib/tostiguerra/utils).

## What's new in v2?

ANNaMo v1 uses a Gaussian semiflexibility interaction to reproduce the
increased rigidity of double-stranded regions. While reproducing the target
persistence length, this interaction can generate an unphysical population of
strongly bent dsDNA configurations.

ANNaMo v2 replaces this term with a hybridisation-dependent many-body
interaction that couples local chain stiffness directly to duplex formation.

The interaction is designed so that:

- unpaired regions remain flexible;
- local stiffness increases upon hybridisation;
- the interaction remains smooth and bounded in multi-contact configurations,
  including transient strand-exchange states.

The implementation includes analytical forces and torques and is integrated
directly into the ANNaMo interaction in oxDNA.

## Current status

The new interaction is implemented and has undergone initial numerical
verification:

- translational forces checked against finite-difference energy gradients;
- energy conservation tested in the microcanonical (`NVE`) ensemble;
- analytical torque contributions implemented.

Independent finite-difference verification of the torques has not yet been
performed.

**Physical parameterisation and validation are ongoing. **** The current
v2-specific parameter values are placeholders and should not be used for
production simulations.

## Usage

ANNaMo v2 uses the same system-preparation workflow as ANNaMo v1.

Enable it in the oxDNA input file with:

```text
ANNAMO_annamo_version = 2
```

The new interaction can be controlled through:

```text
ANNAMO_enable_semiflex_ds = true
ANNAMO_semiflex_ds_k = ...
ANNAMO_sf_r0 = ...
```

For system preparation, bead division, JSON input format, and the general
ANNaMo workflow, see the documentation in the official oxDNA repository.

## Source

The v2 implementation is located in:

```text
contrib/tostiguerra/src/ANNaMoInteraction.cpp
contrib/tostiguerra/src/ANNaMoInteraction.h
```
