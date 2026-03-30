# Chasing Twilight | Unreal Engine Systems Prototype

A modular Unreal Engine 5 prototype for **Chasing Twilight**, a narrative-driven RPG featuring time-based systems, branching dialogue, interaction logic, and save/load architecture.

This repository is focused on **core gameplay systems and technical architecture**, with an emphasis on reusable, production-minded design using **C++ and Blueprints**.

---

## Overview

This project demonstrates a systems-first approach to game development in Unreal Engine.  
Rather than building content first, the project focuses on the underlying architecture needed to support a larger narrative RPG, including:

- Interaction handling
- Dialogue flow and conditional branching
- Time and scheduling systems
- Save/load state management
- Debug tooling for rapid iteration

---

## What I Built

This project was designed and implemented by me as part of a larger indie RPG in development.

### Core systems implemented
- **Interaction Component**
  - Trace + overlap-based interactable detection
  - Focus resolution for nearby interactables
  - Blueprint interface support for interaction targets

- **Dialogue System**
  - Dialogue runner with condition evaluation
  - Support for fallback and gated dialogue paths
  - Data-driven dialogue asset workflow

- **Time System**
  - Global time tracking through a subsystem
  - Discrete time blocks such as Dawn, Morning, Dusk, and Darkest Hour
  - Designed for NPC schedules, event gating, and world-state logic

- **Event / Flag Architecture**
  - Global flag management for story and gameplay state
  - Event scheduling based on time progression

- **Save / Load System**
  - Centralized save subsystem
  - Dirty state tracking
  - Restores time, flags, and player state

- **Debug Overlay**
  - Runtime visibility into gameplay tags, camera mode, locomotion mode, focused actor, time state, and save state
  - Built to reduce iteration friction and surface system interactions during testing

---

## Technical Goals

This repository is intended to demonstrate:

- Unreal Engine 5 gameplay architecture
- Hybrid **C++ + Blueprint** workflows
- Modular system design for future scalability
- Debugging and tooling practices
- Separation of core logic from design-facing configuration

---

## Tech Stack

- **Unreal Engine 5**
- **C++**
- **Blueprints**
- **Gameplay Tags**
- **GameInstance Subsystems**
- **Data Assets**

---

## Architecture Notes

### Why subsystems?
Global systems such as time, flags, saves, and scheduling are implemented as subsystems to keep them centralized, accessible, and decoupled from individual actors.

### Why time blocks instead of precise timestamps?
Using named time blocks like `Dawn`, `Morning`, and `Dusk` makes narrative logic, scheduling, and content gating easier to reason about and maintain than tying everything to exact minute checks.

### Why C++ + Blueprint?
Core systems are implemented in C++ for structure and reliability, while Blueprint exposure allows faster iteration and easier content authoring.

### Why a debug overlay?
Large interconnected systems are easier to test when core state is visible in real time. The overlay reduces guesswork and speeds up debugging.

---

## Example Features in the Current Prototype

- Interact with a test actor
- Trigger dialogue with condition-based responses
- Advance and inspect game time
- View runtime state through the debug overlay
- Verify save/load behavior across core systems

---

## Project Structure

```text
Source/
  ChasingTwilight/
    Private/
      Core/
      Data/
      Dialogue/
      Movement/
      Subsystems/
      Time/
      Save/
      Flags/
      Events/
      UI/
    Public/
      Core/
      Data/
      Dialogue/
      Movement/
      Subsystems/
      Time/
      Save/
      Flags/
      Events/
      UI/

  

Content/
  Characters/
  Core/Blueprints/Utilities/
  Dev/
  Framework/
  Input/
  Systems/
  UI/

```
