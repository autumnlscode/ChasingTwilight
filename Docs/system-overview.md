# System Overview

## Purpose

This project is a systems-first Unreal Engine 5 prototype built to support a narrative-driven RPG.  
The focus is on creating modular, reusable gameplay systems that can scale with increasing complexity.

Rather than building content first, this prototype establishes the technical foundation required for:
- Interaction
- Dialogue
- Time-based world logic
- Save/load state
- Debugging and iteration

---

## Core Architecture

The project is built around **GameInstance Subsystems**, which manage global systems in a centralized and decoupled way.

### Key Subsystems

#### Time Subsystem (`CT_TimeSubsystem`)
- Tracks in-game time in minutes
- Converts time into **discrete time blocks** (Dawn, Morning, Dusk, etc.)
- Broadcasts time updates to other systems
- Designed for scheduling, dialogue gating, and environmental logic

#### Flag Subsystem (`CT_FlagSubsystem`)
- Stores global gameplay and narrative state using Gameplay Tags
- Used for dialogue conditions, quest progression, and world state tracking

#### Event Scheduler Subsystem (`CT_EventSchedulerSubsystem`)
- Registers and triggers events based on time and conditions
- Allows systems to react to time progression without hard dependencies

#### Save Subsystem (`CT_SaveSubsystem`)
- Handles saving and loading of:
  - Time state
  - Flags
  - Player transform
- Tracks **dirty state** to determine when saving is required
- Broadcasts events when save data changes

---

## Interaction Layer

Interaction is handled through a dedicated **Interaction Component**, which:
- Maintains a list of nearby interactable actors
- Uses a trace to determine the current focus target
- Communicates with interactables via a Blueprint Interface

This allows interaction logic to remain **flexible and decoupled** from specific actor types.

---

## Dialogue System

The dialogue system is **data-driven**, using dialogue assets to define:
- Lines of dialogue
- Conditions for branching paths
- Fallback behavior when conditions fail

A central dialogue runner evaluates conditions using:
- Time state
- Flags
- Other gameplay context

---

## Debugging & Tooling

A **Debug Overlay Widget** is included to surface runtime state, including:
- Current time and time block
- Active gameplay tags
- Focused interactable actor
- Camera mode and locomotion state
- Save system status

This reduces iteration time and makes system interactions easier to verify.

---

## Design Philosophy

### Modular Systems
Each system is designed to operate independently and communicate through well-defined interfaces or subsystems.

### C++ Core, Blueprint Flexibility
Core logic is implemented in C++ for structure and performance, while Blueprint exposure allows rapid iteration and designer-friendly workflows.

### Data-Driven Behavior
Where possible, behavior is defined through data assets rather than hard-coded logic, allowing easier expansion and tuning.

### Observability First
Systems are built with debugging in mind, ensuring that internal state is visible and testable during development.

---

## What This Enables

This architecture supports:
- Time-based NPC schedules
- Conditional dialogue and quest progression
- Scalable world state tracking
- Rapid iteration and debugging
- Future expansion into full gameplay systems

---

## Future Improvements

- Expand event scheduling for more complex conditions
- Improve interaction prioritization and UI feedback
- Extend dialogue system with more advanced branching logic
- Integrate systems into a fully playable prototype area